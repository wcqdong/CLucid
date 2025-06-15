//
// Created by weichuanqi on 2025/5/21.
//

#include "matcher/AnnotationMatcher.h"

using namespace CLucid;

void AnnotationMatcherClass::walk(const std::function<void(const ClassNode&, const AnnotationContainer&)>& pFunction, const CLucidAst* metaAst) {
    std::unordered_set<std::string> matchedKeys;

    for (auto& item: metaAst->getMetaClassData()) {
        const ClassNode& classInfo = item.second;
        matchAnnotation(classInfo, keys, pFunction, matchedKeys);
    }
}
void AnnotationMatcherMethod::walk(const std::function<void(const MethodNode&, const AnnotationContainer&)>& pFunction, const CLucidAst* metaAst, const ClassNode* classNode) {
    std::unordered_set<std::string> matchedKeys;
    if(classNode){
        for (auto& methodInfo: classNode->methods) {
            matchAnnotation(methodInfo, keys, pFunction, matchedKeys);
        }
        return;
    }
    for (auto& item: metaAst->getMetaClassData()) {
        for (auto& methodInfo: item.second.methods) {
            matchAnnotation(methodInfo, keys, pFunction, matchedKeys);
        }
    }
}

void AnnotationMatcherProperty::walk(const std::function<void(const PropertyNode&, const AnnotationContainer&)>& pFunction, const CLucidAst* metaAst, const ClassNode* classNode) {
    std::unordered_set<std::string> matchedKeys;
    if(classNode){
        for (auto& propertyInfo: classNode->properties) {
            matchAnnotation(propertyInfo, keys, pFunction, matchedKeys);
        }
        return;
    }
    for (auto& item: metaAst->getMetaClassData()) {
        for (auto& propertyInfo: item.second.properties) {
            matchAnnotation(propertyInfo, keys, pFunction, matchedKeys);
        }
    }
}

void AnnotationMatcherFunction::walk(const std::function<void(const FunctionNode&, const AnnotationContainer&)>& pFunction, const CLucidAst* metaAst) {
    std::unordered_set<std::string> matchedKeys;
    for (auto& item: metaAst->getMetaFunctionData()) {
        const FunctionNode& functionInfo = item.second;
        matchAnnotation(functionInfo, keys, pFunction, matchedKeys);
    }
}

void AnnotationMatcherEnum::walk(const std::function<void(const EnumNode&, const AnnotationContainer&)>& pFunction, const CLucidAst* metaAst) {
    std::unordered_set<std::string> matchedKeys;
    for (auto& item: metaAst->getMetaEnumData()) {
        const EnumNode& enumInfo = item.second;
        matchAnnotation(enumInfo, keys, pFunction, matchedKeys);
    }
}
