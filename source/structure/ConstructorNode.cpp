//
// Created by weichuanqi on 2025/6/2.
//

#include "structure/ConstructorNode.h"
#include "support/Helper.h"

CLucid::ConstructorNode::ConstructorNode(const CXCursor& cursor, const std::string& annotation)
    : NodeBase(cursor, annotation)
    , bCopy(false)
    , bMove(false)
    , bDefault(false)
{

}
std::string CLucid::ConstructorNode::toString(const char* prefix) const {
    std::string format = Utils::format("{0}CONSTRUCTOR({1})\n{2}{3} {4}(", prefix, annotation, prefix, Helper::accessSpecifierToString(accessSpecifier), name);
    for (int i = 0; i < params.size(); ++i) {
        const ParamNode& param = params[i];
        format += param.toString();
        if (i != params.size() - 1) {
            format += ", ";
        }
    }
    format += ")";
//    if (!initList.empty()){
//        format += " : ";
//        for (int i = 0; i < initList.size(); ++i) {
//            format += std::get<0>(initList[i]);
//            format += "(";
//            format += std::get<1>(initList[i]);
//            format += ")";
//            if (i != initList.size() - 1) {
//                format += ", ";
//            }
//        }
//    }
    format += "{}";
    return format;

}

