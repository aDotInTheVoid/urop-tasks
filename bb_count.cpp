#include <algorithm>
#include <map>

#include "llvm/IR/Function.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"

namespace {
struct Counter : public llvm::FunctionPass {
  static char ID;
  size_t nFunc;
  size_t nBasicBlock;

  Counter() : FunctionPass(ID) {}

  bool runOnFunction(llvm::Function &F) override {
    nFunc++;
    nBasicBlock += F.size();
    return false;
  }

  ~Counter() {
    llvm::errs() << "The program has a total of " << nFunc << " functions and "
                 << nBasicBlock << " basic blocks.\n";
  }
};  // end of struct Counter
}  // end of anonymous namespace

char Counter::ID = 0;
static llvm::RegisterPass<Counter> X("bb_count", "Instruction Counter Pass");

static llvm::RegisterStandardPasses Y(
    llvm::PassManagerBuilder::EP_EarlyAsPossible,
    [](const llvm::PassManagerBuilder &Builder,
       llvm::legacy::PassManagerBase &PM) { PM.add(new Counter()); });