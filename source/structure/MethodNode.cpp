//
// Created by weichuanqi on 2025/5/19.
//

#include "structure/MethodNode.h"
#include "structure/ParamNode.h"
#include "support/Helper.h"

using namespace CLucid;

MethodNode::MethodNode(const CXCursor& cursor, const std::string& annotation)
    : NodeBase(cursor, annotation)
    , bStatic(false)
    , bConst(false)
    , bVirtual(false)
    , bPureVirtual(false)
    , bOverride(false)
    , accessSpecifier(CX_CXXInvalidAccessSpecifier)
{
}

std::string MethodNode::toString(const char* prefix) const {
    std::string staticStr = bStatic ? "static " : "";
    std::string virtualStr = bVirtual ? "virtual " : "";
    std::string pureVirtualStr = bPureVirtual ? " = 0" : "";
    std::string overrideStr = bOverride ? " override" : "";
    std::string format = Utils::format("{0}METHOD({1})\n{2}{3} {4}{5}{6} {7}(", prefix, annotation, prefix, Helper::accessSpecifierToString(accessSpecifier), virtualStr, staticStr, returnTypeName, name);
    for (int i = 0; i < params.size(); ++i) {
        const ParamNode& param = params[i];
        format += param.toString();
        if (i != params.size() - 1) {
            format += ", ";
        }
    }
    return Utils::format("{0}){1}{2}{3};", format, bConst ? " const":"", pureVirtualStr, overrideStr);
}
