//
// Created by weichuanqi on 2025/5/19.
//

#include "structure/ParamNode.h"
#include "structure/MethodNode.h"
#include "support/Utils.h"

using namespace CLucid;
std::string ParamNode::toString(const char* prefix) const {

    return Utils::format("{0}{1} {2}", prefix, typeInfo.fullName, name);
}
MethodNode* ParamNode::getMethodNode() {
    return reinterpret_cast<class MethodNode*>(parent);
}
