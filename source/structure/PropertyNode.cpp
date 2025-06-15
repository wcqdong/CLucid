//
// Created by weichuanqi on 2025/5/19.
//


#include "structure/PropertyNode.h"
#include "support/Helper.h"

using namespace CLucid;

std::string PropertyNode::toString(const char *prefix) const {
    std::string format = Utils::format("{0}PROPERTY({1})\n{2}{3} {4} {5};", prefix, annotation, prefix, Helper::accessSpecifierToString(accessSpecifier), typeInfo.fullName, name);
    return format;
}
PropertyNode::PropertyNode(const CXCursor& cursor, const std::string& annotation)
        : NodeBase(cursor, annotation)
        , accessSpecifier(CX_CXXInvalidAccessSpecifier)
        , offset(0)
        , size(0)
{

}
