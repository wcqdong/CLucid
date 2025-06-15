//
// Created by weichuanqi on 2025/5/30.
//

#include "generators/SerializeGenerator.h"
#include "matcher/AnnotationMatcher.h"

using namespace Generators;

void SerializeGenerator::init() {
    // 初始化模板
    serializeTemplate = env.parse_template(templateRootPath + TEMPLATE_FILE_NAME);
}

void SerializeGenerator::collectData() {

    // 匹配带有"serializable"的属性
    CLucid::AnnotationMatcherProperty propertyMatcher;
    propertyMatcher.addKey("serializable");

    // 匹配所有的类(匹配器中没有keys，所以匹配所有)
    CLucid::AnnotationMatcherClass classMatcher;
    // 遍历匹配类
    classMatcher.walk([&](const CLucid::ClassNode& classNode, const CLucid::AnnotationContainer& annoContainer) {

        std::vector<const CLucid::PropertyNode*> properties;
        // 遍历匹配属性
        propertyMatcher.walk([&](const CLucid::PropertyNode& propertyNode, const CLucid::AnnotationContainer& annoContainer) {
            properties.push_back(&propertyNode);
        }, ast, &classNode);

        if (!properties.empty()){
            classes.emplace_back(&classNode);
            ClassInfo& classInfo = classes.back();
            classInfo.properties = properties;
        }

    }, ast);

    for (auto& classInfo : classes) {
//        inja::json methods = inja::json::array();
//        for (CLucid::MethodNode& methodNode: classNode.methods) {
//            inja::json methodJson;
//            methodJson["name"] = methodNode.name;
//            methodJson["return_type"] = methodNode.returnTypeName;
//            std::string sbf;
//            for (int i = 0; i < methodNode.params.size(); ++i) {
//                CLucid::ParamNode& param = methodNode.params[i];
//                sbf += param.declType + ' ' + param.name;
//                if (i < methodNode.params.size() - 1){
//                    sbf += ',';
//                }
//            }
//
//
//
//            methodJson["params"] = sbf;
//
//            methods.push_back(methodJson);
//        }
        inja::json properties = inja::json::array();
        for (const CLucid::PropertyNode* propertyNode : classInfo.properties) {
            inja::json propertyJson;
            propertyJson["name"] = propertyNode->name;

            auto index = propertyNode->typeInfo.cleanFullName.rfind("::");
            if (index == std::string::npos){
                propertyJson["type"] = propertyNode->typeInfo.cleanFullName;
            }else{
                propertyJson["type"] = propertyNode->typeInfo.cleanFullName.substr(index + strnlen_s("::", 2));
            }

            properties.push_back(propertyJson);
        }

        inja::json classJson;
        classJson["name"] = classInfo.classNode->name;
        classJson["properties"] = properties;

        inja::json json;
        json["clazz"] = classJson;
        size_t index = classInfo.classNode->filePath.rfind('\\');
        size_t pointIndex = classInfo.classNode->filePath.rfind('.');
        json["file_name"] = classInfo.classNode->filePath.substr(index + 1, pointIndex - index - 1);

        jsons.push_back(json);

    }
}
void SerializeGenerator::genFile(inja::json& json) {
    std::string file_name = json["file_name"];
    env.write(serializeTemplate, json, outputRootPath + file_name + "Serializer.h");
}

void SerializeGenerator::genGlobalFile(inja::json& json) {

}

void SerializeGenerator::clear(){
    classes.clear();
}

