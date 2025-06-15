//
// Created by weichuanqi on 2025/5/19.
//

#include "structure/ClassNode.h"
#include "support/Helper.h"
#include "support/Utils.h"

using namespace CLucid;

ClassNode::ClassNode(const CXCursor& cursor, const std::string& annotation)
    : NodeBase(cursor, annotation)
    , deconstructor(nullptr)
    , bStruct(false)
{

}

ClassNode::~ClassNode() {
//    if (deconstructor){
//        delete deconstructor;
//        deconstructor = nullptr;
//    }
}

void ClassNode::addProperty(PropertyNode& info) {
    if (std::find_if(properties.begin(), properties.end(), [&](PropertyNode& ele) { return ele.getName() == info.getName(); }) == properties.end()){
        info.setParent(this);
        properties.emplace_back(info);
    }

}

void ClassNode::addMethod(MethodNode& info) {
    if (std::find_if(methods.begin(), methods.end(), [&](MethodNode& ele) { return ele.fullName == info.fullName; }) == methods.end()) {
        info.setParent(this);
        methods.emplace_back(info);
    }
}

std::string ClassNode::toString(const char* prefix) const {
    std::string classPrefix = std::string(prefix) + "\t";

    std::string format = Utils::format("\n{0}namespace {1} {{\n {2}// {3}\n{4}// {5}\n{6}CLASS({7})\n{8}{9} {10}", prefix, nameSpace, classPrefix, includePath, classPrefix, filePath, classPrefix, annotation, classPrefix, bStruct? "struct":"class",name);
    if (!baseClasses.empty()){
        format += " : ";
        for (int i = 0; i < baseClasses.size(); ++i) {
            ClassNode* baseClass = baseClasses[i];
            format += baseClass->name;
            if (i < baseClasses.size() - 1) {
                format += ", ";
            }
        }
    }
    format += "{\n";

    std::string memberPrefix = std::string(prefix) + "\t\t";
    for (const ConstructorNode& constructor: constructors) {
        format += constructor.toString(memberPrefix.c_str()) + "\n\n";
    }
    if (deconstructor.isValid()){
        format += deconstructor->toString(memberPrefix.c_str()) + "\n\n";
    }
    for (const PropertyNode& property: properties) {
        format += property.toString(memberPrefix.c_str()) + "\n\n";
    }
    for (const MethodNode& method: methods) {
        format += method.toString(memberPrefix.c_str()) + "\n\n";
    }
    format = Utils::format("{0}{1}}\n{2} }", format, classPrefix, prefix);
    return format;
}
void ClassNode::addBaseClass(ClassNode* parentClassNode) {
    baseClasses.push_back(parentClassNode);
    parentClassNode->addSubClass(this);
}
void ClassNode::addSubClass(ClassNode* subClassNode) {
    subClasses.push_back(subClassNode);
}
void ClassNode::addConstructor(ConstructorNode& info) {
    if (std::find_if(constructors.begin(), constructors.end(), [&](ConstructorNode& ele) { return ele.fullName == info.fullName; }) == constructors.end()) {
        info.setParent(this);
        constructors.emplace_back(info);
    }
}

