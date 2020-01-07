/*
 * Author: Hardik Soni
 * Email: hks57@cornell.edu
 */
#include "annotateFields.h"
#include "toTofino.h"
#include "../../midend/identifyStorage.h"

namespace CSA {

bool LearnConcatenatedFields::preorder(const IR::AssignmentStatement* asmt) {

    IdentifyStorage is(refMap, typeMap, 1);
    asmt->left->apply(is);
    /*
    std::cout<<" ---  isMSAHeaderStorage: "<<is.isMSAHeaderStorage()<<" --- \n";
    */
    if (is.isMSAHeaderStorage() && asmt->right->is<IR::Concat>()) {
        
        // std::cout<<" --- "<<asmt<<" --- \n";
        // std::cout<<"ArrayIndex : "<<is.getArrayIndex()<<"\n";
        auto c = is.getArrayIndex();
        auto fn = is.getFieldName();
        cstring str = CreateTofinoArchBlock::csaPacketStructInstanceName +
          "."+fn+"["+cstring::to_cstring(c)+"]."+NameConstants::bitStreamFieldName;
        fieldFQDN->emplace_back(str);
        // std::cout<<" --- "<<str<<" --- \n";
    }
    return false;
}


const IR::Node* AnnotateFields::preorder(IR::P4Program* p4program) {
    LearnConcatenatedFields lc(refMap, typeMap, &fieldFQDN);
    auto orig = getOriginal();
    orig->apply(lc);
    return p4program;
}
const IR::Node* AnnotateFields::preorder(IR::Type_Struct* ts) {
    return ts;
}
    
}// namespace CSA
