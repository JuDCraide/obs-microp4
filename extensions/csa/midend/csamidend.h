/*
 * Author: Hardik Soni
 * Email: hks57@cornell.edu
 */

#ifndef EXTENSIONS_CSA_SWITCH_CSAMIDEND_H_
#define EXTENSIONS_CSA_SWITCH_CSAMIDEND_H_

#include "ir/ir.h"
#include "frontends/common/options.h"
#include "../switch/options.h"
#include "frontends/common/resolveReferences/resolveReferences.h"
#include "frontends/p4/typeMap.h"
#include "controlStateReconInfo.h"

namespace CSA {

class DebugPass final : public Inspector {
    unsigned c = 0;
  public:
    static unsigned i;
    explicit DebugPass() { c= i++;}

    bool preorder(const IR::Declaration_Instance* di) {
        std::cout<<"DebugPass: "<<c<<" "<<di<<"\n";
        return true;
    }
};

class CSAMidEnd {
  private:
    std::vector<DebugHook> hooks;

    CSAOptions csaOptions;
    MidendContext*  midendContext;
    // const IR::P4Program* getV1ModelIR();
    // const IR::P4Program* getTofinoIR();
    const IR::P4Program* getCoreIR();

 public:
    P4::ReferenceMap       refMap;
    P4::TypeMap            typeMap;
    bool isv1;

    explicit CSAMidEnd(CSAOptions& csaOptions, MidendContext*  midendContext) {
        CHECK_NULL(midendContext);
        csaOptions = csaOptions;
        midendContext = midendContext;
        isv1 = csaOptions.isv1();
        hooks.push_back(csaOptions.getDebugHook());
        refMap.setIsV1(isv1);
    }
    void addDebugHook(DebugHook hook) { hooks.push_back(hook); }
    const IR::P4Program* run(const IR::P4Program* program, 
                             std::vector<const IR::P4Program*> precompiledIRs);

};

}  // namespace CSA

#endif /* EXTENSIONS_CSA_SWITCH_CSAMIDEND_H_ */
