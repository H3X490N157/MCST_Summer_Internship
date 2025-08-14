#include "llvm/IR/Function.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Support/raw_ostream.h"

#include <map>
#include <string>

using namespace llvm;

namespace {
struct MyPass : PassInfoMixin<MyPass> {
    PreservedAnalyses run(Function &F, FunctionAnalysisManager &) {
        // Словарь для подсчета инструкций
        std::map<std::string, unsigned> instrCount;

        // Обход всех базовых блоков
        for (auto &BB : F) {
            // Обход всех инструкций в блоке
            for (auto &I : BB) {
                std::string opcodeName = I.getOpcodeName();
                instrCount[opcodeName]++;
            }
        }

        // Вывод результата
        errs() << "Function " << F.getName() << "():\n";
        for (auto &entry : instrCount) {
            errs() << "  " << entry.first << ": " << entry.second << "\n";
        }

        return PreservedAnalyses::all();
    }
};
} // namespace

// Регистрация плагина в New Pass Manager
extern "C" ::llvm::PassPluginLibraryInfo LLVM_ATTRIBUTE_WEAK
llvmGetPassPluginInfo() {
    return {
        LLVM_PLUGIN_API_VERSION, "MyPass", LLVM_VERSION_STRING,
        [](PassBuilder &PB) {
            PB.registerPipelineParsingCallback(
                [](StringRef Name, FunctionPassManager &FPM,
                   ArrayRef<PassBuilder::PipelineElement>) {
                    if (Name == "MyPass") {
                        FPM.addPass(MyPass());
                        return true;
                    }
                    return false;
                });
        }
    };
}

