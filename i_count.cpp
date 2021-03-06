#include <map>

#include "llvm/IR/Function.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"

namespace {
struct Counter : public llvm::FunctionPass {
  static char ID;
  // bbInstrFreq[nInstr] = freq
  std::map<size_t, size_t> bbInstrFreq = {};

  Counter() : FunctionPass(ID) {}

  bool runOnFunction(llvm::Function &F) override {
    for (auto &bb : F) {
      bbInstrFreq[bb.size()]++;
    }
    return false;
  }

  ~Counter() {
    auto end = bbInstrFreq.rbegin()->first;
    for (int i = 0; i <= end; i++) {
      llvm::errs() << i << " " << bbInstrFreq[i] << "\n";
    }
  }
};
}  // namespace

char Counter::ID = 0;
static llvm::RegisterPass<Counter> X("i_count", "Instruction Counter Pass");

static llvm::RegisterStandardPasses Y(
    llvm::PassManagerBuilder::EP_EarlyAsPossible,
    [](const llvm::PassManagerBuilder &Builder,
       llvm::legacy::PassManagerBase &PM) { PM.add(new Counter()); });
