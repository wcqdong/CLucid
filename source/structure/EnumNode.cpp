//
// Created by weichuanqi on 2025/6/4.
//

#include "structure/EnumNode.h"
std::string CLucid::EnumNode::toString(const char* prefix) const {
    std::string enumPrefix = std::string(prefix) + "\t";

    std::string format = Utils::format("\n{0}namespace {1} {{\n {2}// {3}\n{4}// {5}\n{6}ENUM({7})\n{8}enum {9} {10}", prefix, nameSpace, enumPrefix, includePath, enumPrefix, filePath, enumPrefix, annotation,  enumPrefix, bClassEnum?"class":"",name);
    format += "{\n";

    for (const std::tuple<std::string, int64_t>& item : enumItems) {
        format += enumPrefix + "\t" + std::get<0>(item) + " = " + std::to_string(std::get<1>(item)) + ",\n";
    }

    format = Utils::format("{0}{1}}\n{2} }", format, enumPrefix, prefix);
    return format;
}
