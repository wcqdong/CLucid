//
// Created by weichuanqi on 2025/5/20.
//

#include "structure/FunctionNode.h"
#include "support/Helper.h"

using namespace CLucid;


std::string FunctionNode::toString(const char* prefix) const {
    std::string functionPrefix = std::string(prefix) + "\t";
    std::string format = Utils::format("\n{0}namespace {1} {{\n{2}// {3}\n{4}// {5}\n{6}FUNCTION({7})\n{8}{9} {10}(", prefix, nameSpace, functionPrefix, includePath,functionPrefix, filePath, functionPrefix, annotation, functionPrefix,
                                     returnTypeName, name);
    for (int i = 0; i < params.size(); ++i) {
        const ParamNode& param = params[i];
        format += param.toString();
        if (i != params.size() - 1) {
            format += ", ";
        }
    }
    return Utils::format("{0});\n}", format, functionPrefix);
}

void FunctionNode::addParam(ParamNode& node) {
    node.setParent(this);
    params.push_back(node);
}
