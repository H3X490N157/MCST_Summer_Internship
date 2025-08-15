#include "llvm/IR/Function.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"

#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <string>
#include <algorithm>

namespace {

std::string GetBBLabel(llvm::BasicBlock *BB, std::unordered_map<llvm::BasicBlock*, std::string> &Ids, unsigned &nextId) {
    auto it = Ids.find(BB);
    if (it != Ids.end()) {
        return it->second;
    }
    if (BB->hasName()) {
        return Ids.emplace(BB, BB->getName().str()).first->second;
    }
    std::string gen = ("bb" + std::to_string(nextId++));
    return Ids.emplace(BB, gen).first->second;
}

struct MyPass : llvm::PassInfoMixin<MyPass> {
    void DFSRPO(llvm::BasicBlock *BB,
                std::unordered_set<llvm::BasicBlock*> &visited,
                std::vector<llvm::BasicBlock*> &postorder,
                std::vector<llvm::BasicBlock*> &stack,
                std::unordered_map<llvm::BasicBlock*, std::vector<llvm::BasicBlock*>> &backEdges) {
        if (visited.count(BB)) {
            return;
        }
        visited.insert(BB);
        stack.push_back(BB);

        auto *T = BB->getTerminator();//использование типа auto - костыль, связанный с тем, что были проблемы c Instructions.h - вероятно, в моей версии LLVM отдельных типов для терминаторов просто не было
        const unsigned n = T ? T->getNumSuccessors() : 0;
        for (unsigned i = 0; i < n; ++i) {
            llvm::BasicBlock *successor = T->getSuccessor(i);
            if (!visited.count(successor)) {
                DFSRPO(successor, visited, postorder, stack, backEdges);
            } else {
                if (std::find(stack.begin(), stack.end(), successor) != stack.end()) {// обратная дуга — если потомок ууже в текущем стеке 
                    backEdges[BB].push_back(successor);
                }
            }
        }

        stack.pop_back();
        postorder.push_back(BB); //потом переверну RPO - микрокостыль
    }

    llvm::PreservedAnalyses run(llvm::Function &F, llvm::FunctionAnalysisManager &) {

        std::unordered_set<llvm::BasicBlock*> visited;
        std::vector<llvm::BasicBlock*> postorder;
        std::vector<llvm::BasicBlock*> stack;
        std::unordered_map<llvm::BasicBlock*, std::vector<llvm::BasicBlock*>> backEdges;

        if (!F.empty()) {
            DFSRPO(&F.getEntryBlock(), visited, postorder, stack, backEdges);
        }

        std::unordered_map<llvm::BasicBlock*, std::string> bbIds;//привоение id, чтобы можно было поиграться с именами
        unsigned nextId = 0;

        llvm::errs() << "  RPO order:\n";
        for (auto it = postorder.rbegin(); it != postorder.rend(); ++it) {//разворот RPO уже на месте вывода
            llvm::BasicBlock *BB = *it;
            llvm::errs() << "    " << GetBBLabel(BB, bbIds, nextId) << "\n";
        }

        llvm::errs() << "  Back edges:\n";
        if (backEdges.empty()) {
            llvm::errs() << "    (none)\n";
        } else {
            for (auto &kv : backEdges) {
                auto from = GetBBLabel(kv.first, bbIds, nextId);
                llvm::errs() << "    " << from << " ->";
                for (auto *toBB : kv.second) {
                    llvm::errs() << " " << GetBBLabel(toBB, bbIds, nextId);
                }
                llvm::errs() << "\n";
            }
        }
        llvm::errs() << "Function " << F.getName() << "():\n";


        std::unordered_map<std::string, unsigned> instCount;      
        for (auto &BB : F) {//подсчёт числа инструкций через перебор всех инструкций
            for (auto &I : BB) {
                instCount[std::string(I.getOpcodeName())]++;
            }
        }
        std::vector<std::pair<std::string,unsigned>> sorted;
        sorted.reserve(instCount.size());
        for (auto &p : instCount) sorted.push_back(p);
        std::sort(sorted.begin(), sorted.end(),
                  [](auto &a, auto &b){ return a.first < b.first; });
        for (auto &p : sorted) {
            llvm::errs() << "    " << p.first << ": " << p.second << "\n";
        }


        return llvm::PreservedAnalyses::all(); //ничего не меняем
    }

    static bool isRequired() {//обрабатываем -O0 / optnone
        return true; 
    } 
};

} 

extern "C" ::llvm::PassPluginLibraryInfo LLVM_ATTRIBUTE_WEAK
llvmGetPassPluginInfo() {
    return {
        LLVM_PLUGIN_API_VERSION,
        "MyPass",
        LLVM_VERSION_STRING,
        [](llvm::PassBuilder &PB) { //оставил название MyPass от выданного тестового образца прохода
            PB.registerPipelineParsingCallback(
                [](llvm::StringRef Name, llvm::FunctionPassManager &FPM,
                   llvm::ArrayRef<llvm::PassBuilder::PipelineElement>) {
                    if (Name == "MyPass") {
                        FPM.addPass(MyPass());
                        return true;
                    }
                    return false;
                });
        }
    };
}
