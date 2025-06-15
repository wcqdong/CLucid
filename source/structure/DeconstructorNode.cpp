//
// Created by weichuanqi on 2025/6/10.
//

#include "structure/DeconstructorNode.h"
#include "support/Helper.h"

using namespace CLucid;
DeconstructorNode::DeconstructorNode(const CXCursor& cursor, const std::string& annotation)
    : NodeBase(cursor, annotation)
    , bVirtual(false)
    , bPureVirtual(false)
    , bOverride(false)
    , accessSpecifier(CX_CXXInvalidAccessSpecifier)
{

}

std::string DeconstructorNode::toString(const char* prefix) const {
    std::string format = Utils::format("{0}DECONSTRUCTOR({1})\n{2}{3} {4}(){{}", prefix, annotation, prefix, Helper::accessSpecifierToString(accessSpecifier), name);
    return format;
}
