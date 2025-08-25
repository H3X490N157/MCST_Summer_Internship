#include "llvm/IR/Function.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/GlobalVariable.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"

#include <unordered_map>
#include <vector>

using namespace llvm;

//у меня получилось небольшое отклонение от классическое gcp 

namespace {

//решётка
struct LatticeVal { 
    //идея следующаю: в качестве Top можно указывать неизвестные значение, Const - однозначно константные значения, Bottom - конфликтные значения (например, фи-узлы, в которые идёт несколько разных переменных); костыль
    enum Kind {Top, Const, Bottom} kind; 
    ConstantInt *val; //если kind == Const, тут лежит сама константа

    static LatticeVal TopVal() { 
        return {Top, nullptr}; 
    }
    
    static LatticeVal ConstantVal(ConstantInt *v) { 
        return {Const, v}; 
    }
    
    static LatticeVal BottomVal() {
        return {Bottom, nullptr}; 
    }
};

//сравнение двух значений решётки
static bool Equal(const LatticeVal &a, const LatticeVal &b) {
    if (a.kind != b.kind) {
        return false;
    }
    if (a.kind == LatticeVal::Const && b.kind == LatticeVal::Const) {
        return a.val->equalsInt(b.val->getZExtValue());
    }
    return true;
}

//объединение двух значений решётки
static LatticeVal Join(const LatticeVal &a, const LatticeVal &b) {
    if (a.kind == LatticeVal::Top) {
        return b;
    }
    if (b.kind == LatticeVal::Top) {
        return a;
    }

    if (a.kind == LatticeVal::Const && b.kind == LatticeVal::Const) {
        if (a.val->equalsInt(b.val->getZExtValue())) {
            return a;
        }
        return LatticeVal::BottomVal();
    }
    return LatticeVal::BottomVal();
}

//возвращает текущую информацию о значении V из решётки (учитывая literals и глобалы)
static LatticeVal GetVal(Value *V, std::unordered_map<Value*, LatticeVal> &values) {
    if (auto *C = dyn_cast<ConstantInt>(V)) {
        return LatticeVal::ConstantVal(C);
    }

    if (auto *GV = dyn_cast<GlobalVariable>(V)) {
        auto it = values.find(GV);
        if (it != values.end()) {
            return it->second;
        }

        if (GV->hasInitializer()) {
            if (auto *CI = dyn_cast<ConstantInt>(GV->getInitializer())) {
                return LatticeVal::ConstantVal(CI);
            }
        }
        return LatticeVal::TopVal();
    }

    auto it = values.find(V);
    if (it != values.end()) {
        return it->second;
    }
    return LatticeVal::TopVal();
}

//для конкретной инструкции возвращаем её состояние (оценка по текущей решётке)
static LatticeVal Evaluate(Instruction *I, std::unordered_map<Value*, LatticeVal> &values) {
    if (auto *BO = dyn_cast<BinaryOperator>(I)) {
        //определяем левый и правый операнды
        auto L = GetVal(BO->getOperand(0), values);
        auto R = GetVal(BO->getOperand(1), values);

        if (L.kind == LatticeVal::Const && R.kind == LatticeVal::Const) {
            APInt lv = L.val->getValue();
            APInt rv = R.val->getValue();
            APInt res;
            switch (BO->getOpcode()) {
                case Instruction::Add: res = lv + rv; break;
                case Instruction::Sub: res = lv - rv; break;
                case Instruction::Mul: res = lv * rv; break;
                case Instruction::And: res = lv & rv; break;
                case Instruction::Or:  res = lv | rv; break;
                case Instruction::Xor: res = lv ^ rv; break;
                default: return LatticeVal::BottomVal(); //случай, когда операция не поддерживается
            }
            return LatticeVal::ConstantVal(cast<ConstantInt>(ConstantInt::get(I->getType(), res)));
        }
        if (L.kind == LatticeVal::Bottom || R.kind == LatticeVal::Bottom) {
            return LatticeVal::BottomVal();
        }
        return LatticeVal::TopVal();//по умолчанию - ничего не знаем
    }

    if (auto *CI = dyn_cast<ICmpInst>(I)) {
        auto L = GetVal(CI->getOperand(0), values);
        auto R = GetVal(CI->getOperand(1), values);

        if (L.kind == LatticeVal::Const && R.kind == LatticeVal::Const) {
            auto pred = CI->getPredicate();
            Constant *c = ConstantExpr::getICmp(pred, L.val, R.val);
            if (auto *cint = dyn_cast<ConstantInt>(c)) {
                return LatticeVal::ConstantVal(cint);
            }
            return LatticeVal::BottomVal();
        }
        if (L.kind == LatticeVal::Bottom || R.kind == LatticeVal::Bottom) {
            return LatticeVal::BottomVal();
        }
        return LatticeVal::TopVal();//по умолчанию - ничего не знаем
    }

   //фи-узел: объединяем значения входов через join
    if (auto *PN = dyn_cast<PHINode>(I)) {
        LatticeVal acc = LatticeVal::TopVal(); 
        bool first = true;
        for (unsigned i = 0, e = PN->getNumIncomingValues(); i < e; ++i) {
            auto V = PN->getIncomingValue(i);
            auto LV = GetVal(V, values);
            if (first) {//берем первое значение без join
                acc = LV;
                first = false;
            } else {//для последующих — делаем join
                acc = Join(acc, LV);
            }
            if (acc.kind == LatticeVal::Bottom) {//если дошли до Bottom, дальше уже неважно, то очев, что дальше будет только Bottom
                break;
            }
        }
        return acc.kind == LatticeVal::Top ? LatticeVal::TopVal() : acc;
    }

    //Load: если грузим из глобала — возвращаем его текущее lattice-значение
    if (auto *LI = dyn_cast<LoadInst>(I)) {
        Value *ptr = LI->getPointerOperand();
        if (auto *GV = dyn_cast<GlobalVariable>(ptr)) {
            auto LV = GetVal(GV, values);
            return LV;
        }
        return LatticeVal::TopVal(); //загрузка не из глобала — ничего не гарантируем
    }

    //Store: если пишем в глобал — обновим его решётку
    if (auto *SI = dyn_cast<StoreInst>(I)) {
        Value *ptr = SI->getPointerOperand();
        Value *val = SI->getValueOperand();

        if (auto *GV = dyn_cast<GlobalVariable>(ptr)) {
            auto V = GetVal(val, values);

            //если глобал объявлен как constant — запись в него невозможна, считаем Bottom
            if (GV->isConstant()) {
                values[GV] = LatticeVal::BottomVal();
            } else {
                //для изменяемых глобалов: объединяем старое значение с новым
                auto it = values.find(GV);
                if (it == values.end()) {
                    values[GV] = V.kind == LatticeVal::Const ? V : LatticeVal::BottomVal();
                } else {
                    values[GV] = Join(it->second, V);
                }
                //если в итоге не константа — обнуляем до Bottom
                if (values[GV].kind != LatticeVal::Const) {
                    values[GV] = LatticeVal::BottomVal();
                }
            }
        }
        return LatticeVal::TopVal(); //сама по себе store не производит значения
    }

    return LatticeVal::TopVal();
}

struct GlobalConstPropPass : PassInfoMixin<GlobalConstPropPass> {
    PreservedAnalyses run(Function &F, FunctionAnalysisManager &) {
        std::unordered_map<Value*, LatticeVal> values;
        std::vector<Instruction*> worklist;

        Module *M = F.getParent();
        for (auto &GV : M->globals()) {
            if (GV.hasInitializer()) {
                if (auto *CI = dyn_cast<ConstantInt>(GV.getInitializer())) {
                    values[&GV] = LatticeVal::ConstantVal(CI);
                } else {
                    values[&GV] = LatticeVal::TopVal();
                }
            } else {
                values[&GV] = LatticeVal::TopVal();
            }
        }
        
        //инициализация
        for (auto &BB : F) {
            for (auto &I : BB) {
                values[&I] = LatticeVal::TopVal();
                worklist.push_back(&I);
            }
        }


        //анализ
        while (!worklist.empty()) {
            Instruction *I = worklist.back();
            worklist.pop_back();

            LatticeVal newVal = Evaluate(I, values);
            LatticeVal oldVal = values[I];

            if (!Equal(newVal, oldVal)) {
                values[I] = newVal;
                for (auto *U : I->users()) {
                    if (auto *UI = dyn_cast<Instruction>(U)) {
                        worklist.push_back(UI);
                    }
                }
            }
        }
        
        //замена
        bool changed = false;
        for (auto &BB : F) {
            for (auto it = BB.begin(); it != BB.end(); ) {
                Instruction &I = *it++;

                if (auto *LI = dyn_cast<LoadInst>(&I)) {
                    if (auto *GV = dyn_cast<GlobalVariable>(LI->getPointerOperand())) {
                        auto itGV = values.find(GV);
                        if (itGV != values.end() && itGV->second.kind == LatticeVal::Const) {
                            LI->replaceAllUsesWith(itGV->second.val);
                            LI->eraseFromParent();
                            changed = true;
                            continue;
                        }
                    }
                }

                auto val = values[&I];
                if (val.kind == LatticeVal::Const) {
                    I.replaceAllUsesWith(val.val);
                    I.eraseFromParent();
                    changed = true;
                }
            }
        }

        return changed ? PreservedAnalyses::none() : PreservedAnalyses::all();
    }

    static bool isRequired() {
        return true;
    }
};

} 

extern "C" ::llvm::PassPluginLibraryInfo LLVM_ATTRIBUTE_WEAK
llvmGetPassPluginInfo() {
    return {
        LLVM_PLUGIN_API_VERSION,
        "GlobalConstPropPass",
        LLVM_VERSION_STRING,
        [](PassBuilder &PB) {
            PB.registerPipelineParsingCallback(
                [](StringRef Name, FunctionPassManager &FPM,
                   ArrayRef<PassBuilder::PipelineElement>) {
                    if (Name == "gcp") {
                        FPM.addPass(GlobalConstPropPass());
                        return true;
                    }
                    return false;
                });
        }
    };
}

