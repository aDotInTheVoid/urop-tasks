#include <algorithm>
#include <map>

#include "llvm/IR/Function.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"

// RIP https://reviews.llvm.org/D69121

static bool pair_compare(std::pair<size_t, size_t> l,
                         std::pair<size_t, size_t> r) {
  return l.second < r.second;
}

namespace {
struct Counter : public llvm::FunctionPass {
  static char ID;
  int bbCount = 0;
  int funcCount = 0;
  // nBB, freq
  std::map<size_t, size_t> bbInstrFreq = {};

  Counter() : FunctionPass(ID) {}

  bool runOnFunction(llvm::Function &F) override {
    funcCount++;
    bbCount += F.size();

    llvm::errs() << "Function " << F.getName() << "\n";

    for (auto &bb : F) {
      llvm::errs() << " BasicBlock\n";
      bbInstrFreq[bb.size()]++;
      for (auto &i : bb) {
        llvm::errs() << "  " << i << "\n";
      }
    }

    llvm::errs() << "function " << F.getName() << " has " << F.size()
                 << " bbs\n";

    return false;
  }

  ~Counter() {
    llvm::errs() << "The program has a total of " << funcCount
                 << " functions and " << bbCount << " basic blocks.\n";

    for (auto [size, freq] : bbInstrFreq) {
      llvm::errs() << size << " " << freq << "\n";
    }
  }
};  // end of struct Counter
}  // end of anonymous namespace

char Counter::ID = 0;
static llvm::RegisterPass<Counter> X("count", "Counter World Pass");

static llvm::RegisterStandardPasses Y(
    llvm::PassManagerBuilder::EP_EarlyAsPossible,
    [](const llvm::PassManagerBuilder &Builder,
       llvm::legacy::PassManagerBase &PM) { PM.add(new Counter()); });