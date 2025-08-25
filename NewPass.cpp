#include "llvm/IR/Function.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Constants.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"

#include <unordered_map>
#include <vector>

using namespace llvm;

//у меня получилось небольшое отклонение от классическое gcp 

namespace {

struct LatticeVal {//решётка, 
    enum Kind {Top, Const, Bottom} kind;//идея следующаю: в качестве Top можно указывать неизвестные значение, Const - однозначно константные значения, Bottom - конфликтные значения (например, фи-узлы, в которые идёт несколько разных переменных); костыль
    ConstantInt *val;

    static LatticeVal top() { 
        return {Top, nullptr}; 
    }
    
    static LatticeVal constant(ConstantInt *v) { 
        return {Const, v}; 
    }
    
    static LatticeVal bottom() {
        return {Bottom, nullptr}; 
    }
};

bool equal(const LatticeVal &a, const LatticeVal &b) {//сравенение двух решёток
    if (a.kind != b.kind) {
        return false;
    }
    
    if (a.kind == LatticeVal::Const && b.kind == LatticeVal::Const) {
        return a.val->equalsInt(b.val->getZExtValue());
    }
    
    return true;
}

LatticeVal join(const LatticeVal &a, const LatticeVal &b) {//объединение двух решётиок
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
        return LatticeVal::bottom();
    }
    return LatticeVal::bottom();
}

LatticeVal getVal(Value *V, std::unordered_map<Value*, LatticeVal> &values) {//проверка состояния
    if (auto *C = dyn_cast<ConstantInt>(V)) {//auto использую, чтобы не разводить лишний код, для каждого типа, можно в дальнейшем оптимизировать
        return LatticeVal::constant(C);
    }
    auto it = values.find(V);
    if (it != values.end()) {
        return it->second;
    }
    return LatticeVal::top();//по умолчанию - неизвестные значения
}

LatticeVal evaluate(Instruction *I, std::unordered_map<Value*, LatticeVal> &values) {//для конкретной инструкции возвращает её состояние
    if (auto *BO = dyn_cast<BinaryOperator>(I)) {
        //получаем значения для левого и правого операндв
        auto L = getVal(BO->getOperand(0), values);
        auto R = getVal(BO->getOperand(1), values);

        if (L.kind == LatticeVal::Const && R.kind == LatticeVal::Const) {//случай, если оба операнды - константы
            APInt lv = L.val->getValue();
            APInt rv = R.val->getValue();
            APInt res;
            switch (BO->getOpcode()) {//вычисляем результат в зависимости от типа операции
                case Instruction::Add: res = lv + rv; break;
                case Instruction::Sub: res = lv - rv; break;
                case Instruction::Mul: res = lv * rv; break;
                case Instruction::And: res = lv & rv; break;
                case Instruction::Or:  res = lv | rv; break;
                case Instruction::Xor: res = lv ^ rv; break;
                default: return LatticeVal::bottom(); //случай, когда операция не поддердивается
            }
            return LatticeVal::constant(cast<ConstantInt>(ConstantInt::get(I->getType(), res)));
        }
        if (L.kind == LatticeVal::Bottom || R.kind == LatticeVal::Bottom){//проверка, если один из операндов - это Bottom
            return LatticeVal::bottom();
        }
        return LatticeVal::top();//по умолчанию, возвращаем - top
    }

    if (auto *CI = dyn_cast<ICmpInst>(I)) {//проверка, если инструкция - сравнение
        auto L = getVal(CI->getOperand(0), values);
        auto R = getVal(CI->getOperand(1), values);

        if (L.kind == LatticeVal::Const && R.kind == LatticeVal::Const) {//случай, если оба операнды - константы
            auto pred = CI->getPredicate();
            Constant *c = ConstantExpr::getICmp(pred, L.val, R.val);
            if (auto *cint = dyn_cast<ConstantInt>(c)) {
                return LatticeVal::constant(cint);
            }
        }
        if (L.kind == LatticeVal::Bottom || R.kind == LatticeVal::Bottom){//если хоть один операнд не определён, то очев, что в результате будет неопределённость
            return LatticeVal::bottom();
        }
        return LatticeVal::top();
    }

    return LatticeVal::top();//по умолчанию - ничего не знаем, отдаём только вершину
}

struct GlobalConstPropPass : PassInfoMixin<GlobalConstPropPass> {
    servedAnalyses run(Function &F, FunctionAnalysisManager &) {
        std::unordered_map<Value*, LatticeVal> values;
        std::vector<Instruction*> worklist;

        //инициализация
        for (auto &BB : F) {
            for (auto &I : BB) {
                values[&I] = LatticeVal::top();
                worklist.push_back(&I);
            }
        }

        //анализ
        while (!worklist.empty()) {
            Instruction *I = worklist.back();
            worklist.pop_back();

            LatticeVal newVal = evaluate(I, values);
            LatticeVal oldVal = values[I];

            if (!equal(newVal, oldVal)) {
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
                auto val = values[&I];
                if (val.kind == LatticeVal::Const) {
                    I.replaceAllUsesWith(val.val);
                    I.eraseFromParent();
                    changed = true;
                }
            }
        }

        return changed ? PreservedAnalyses::none()
                       : PreservedAnalyses::all();
    }

    static bool isRequired() { return true; }
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
                    if (Name == "gcp") {//gcp - сокращение от global constant propagation 
                        FPM.addPass(GlobalConstPropPass());
                        return true;
                    }
                    return false;
                });
        }
    };
}

