/*
 * Author: Hardik Soni
 * Email: hks57@cornell.edu
 */

#ifndef _EXTENSIONS_CSA_LINKER_PARSERCONVERTER_H_ 
#define _EXTENSIONS_CSA_LINKER_PARSERCONVERTER_H_ 

#include "ir/ir.h"
#include "lib/ordered_map.h"
#include "frontends/common/resolveReferences/resolveReferences.h"
#include "frontends/p4/typeMap.h"
#include "midend/parserUnroll.h"
#include "controlStateReconInfo.h"
/*
 * This pass converts parser into DAG of MATs
 */
namespace CSA {

class ParserConverter final : public Transform {

// global throught the pass
    P4::ReferenceMap* refMap;
    P4::TypeMap* typeMap;
    cstring structTypeName;
    cstring fieldName;
    unsigned* bitMaxOffset;
    P4ControlStateReconInfoMap* controlToReconInfoMap;
    P4::ParserStructuresMap *parserStructures;

    cstring noActionName;

    cstring tableName = "parser_tbl";

    std::vector<std::vector<unsigned>*> offsetsStack;


// per parser data structures, they get refreshed
    P4::ParserStructure* parserEval = nullptr;
    IR::IndexedVector<IR::StatOrDecl> statOrDeclsOfControlBody;
    IR::IndexedVector<IR::Declaration> varDecls;
    IR::IndexedVector<IR::Declaration> actionDecls;
    IR::IndexedVector<IR::Property> tablePropertyList;
    IR::IndexedVector<IR::Declaration> tableDecls;
    IR::Vector<IR::KeyElement> keyElementList;
    IR::IndexedVector<IR::ActionListElement> actionList;
    IR::Vector<IR::Entry> entryList;
    std::map<cstring, IR::IndexedVector<IR::StatOrDecl>> toAppendStats;

    cstring pktParamName;

    bool stateIterator(IR::ParserState* state);
    bool hasDefaultSelectCase(const IR::ParserState* state) const;
    bool hasSelectExpression(const IR::ParserState* state) const;

    cstring createHeaderInvalidAction(IR::P4Parser* parser);
    
    cstring getStateVisitVarName(const IR::ParserState* state) const {
        return "visit_csa_"+state->name.name;
    }

 public:
    using Transform::preorder;
    using Transform::postorder;

    explicit ParserConverter(P4::ReferenceMap* refMap, P4::TypeMap* typeMap,
                             cstring structTypeName, cstring fieldName,
                             unsigned* maxOffset, 
                             P4ControlStateReconInfoMap* controlToReconInfoMap,
                             P4::ParserStructuresMap *parserStructures)
        : refMap(refMap), typeMap(typeMap), structTypeName(structTypeName), 
          fieldName(fieldName), bitMaxOffset(maxOffset), 
          controlToReconInfoMap(controlToReconInfoMap),
          parserStructures(parserStructures) { 
        CHECK_NULL(maxOffset); CHECK_NULL(parserStructures);
        setName("ParserConverter"); 
        *bitMaxOffset = 0;
        noActionName = "NoAction";
    }

    const IR::Node* preorder(IR::P4Parser* parser) override;
    const IR::Node* postorder(IR::P4Parser* parser) override;
    const IR::Node* preorder(IR::ParserState* state) override;
    // const IR::Node* preorder(IR::Parameter* param) override;

    // control the visit order of P4Parser nodes to visit in 
    // execution call order
    const IR::Node* preorder(IR::P4Program* p4Program) override;

    const IR::Node* preorder(IR::P4ComposablePackage* cp) override;
    // used to visit P4Parser of callee P4ComposablePackage
    const IR::Node* preorder(IR::P4Control* p4Control) override;

    const IR::Node* preorder(IR::MethodCallStatement* mcs) override;

};

class ExtractSubstitutor final : public Transform {
    P4::ReferenceMap* refMap;
    P4::TypeMap* typeMap;
    const P4::ParserStateInfo* parserStateInfo;
    cstring pktParamName;
    cstring fieldName;

    P4::SymbolicValueFactory svf;
    std::vector<const IR::AssignmentStatement*> createPerFieldAssignmentStmts(
          const IR::Expression* hdrVar, unsigned start);

 public:
    using Transform::preorder;
    using Transform::postorder;

    explicit ExtractSubstitutor(P4::ReferenceMap* refMap, P4::TypeMap* typeMap, 
                                const P4::ParserStateInfo* parserStateInfo, 
                                cstring pktParamName,
                                cstring fieldName)
        : refMap(refMap), typeMap(typeMap), parserStateInfo(parserStateInfo), 
          pktParamName(pktParamName), fieldName(fieldName), svf(typeMap) { 
        setName("ExtractSubstitutor"); 
    }


    const IR::Node* preorder(IR::MethodCallStatement* mcs) override;
};

/*

// FIXME: check IR::P4Parser has std::less or replace it with IR::Node* to have
// std::less, otherwise pointer addresses are being compared here.
// It is fine to compare as long nodes cloned in previous passes are not
// invalidated.
typedef std::map<const IR::P4Parser*, P4::ParserStructure*> ParserEvalMap;

class EvaluateAllParsers final : public Inspector { 
    P4::ReferenceMap* refMap;
    P4::TypeMap* typeMap;
    ParserEvalMap* parserEvalMap;
    unsigned* maxOffset = nullptr;
  public:
    
    EvaluateAllParsers(P4::ReferenceMap* refMap, P4::TypeMap* typeMap, 
                       // out param
                       ParserEvalMap *parserEvalMap, 
                       unsigned* maxOffset) 
        : refMap(refMap), typeMap(typeMap), parserEvalMap(parserEvalMap), 
          maxOffset(maxOffset) {
        CHECK_NULL(refMap); CHECK_NULL(typeMap); 
        CHECK_NULL(parserEvalMap); CHECK_NULL(maxOffset);
        *maxOffset = 0;
    }

    bool preorder(const IR::P4Parser* parser) override;
};

*/


}  // namespace CSA

#endif /* _EXTENSIONS_CSA_LINKER_PARSERCONVERTER_H_ */
