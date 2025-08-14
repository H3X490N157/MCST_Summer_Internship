#include "llvm/Passes/PassPlugin.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/CFG.h"

using namespace llvm;

struct MyPass : PassInfoMixin<MyPass> {
    PreservedAnalyses run(Function &F, FunctionAnalysisManager &) {
        errs() << "Function: " << F.getName() << "\n";

        for (auto &BB : F) {
            errs() << "  BasicBlock: " << BB.getName() << "\n";
            if (auto *TInst = BB.getTerminator()) {
                for (unsigned i = 0; i < TInst->getNumSuccessors(); ++i) {
                    errs() << "    Successor: " << TInst->getSuccessor(i)->getName() << "\n";
                }
            }
        }

        return PreservedAnalyses::all();
    }
};

extern "C" PassPluginLibraryInfo LLVM_ATTRIBUTE_WEAK llvmGetPassPluginInfo() {
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

