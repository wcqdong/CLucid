//
// Created by weichuanqi on 2025/5/22.
//

#include <vector>

#include "CLucidParser.h"
#include "matcher/AnnotationMatcher.h"

using namespace CLucid;
int main() {
    // 配置parser
    CLucidParser parser;
    parser.setCppVersion(17);
    std::vector<std::string> sourceFils = {"../../target/source/main.cpp", "../../target/source/Test.cpp"};
    parser.addSourceFiles(sourceFils);
    parser.addIncludePath("../../target/include");

    // 解析c++结构
    CLucidAst metaData;
    parser.parse(metaData);

    // 打印被注解的类、函数、属性信息
    metaData.printDetails();

    // 匹配注解上自定义的名字
    AnnotationMatcherClass classMatcher;
    // 匹配注解中有AnnoName1并且有AnnoName2的类
    classMatcher.addKey("AnnoName1");
    classMatcher.addKey("AnnoName2");

    std::printf("\n\n");
    std::printf("---------------- Matched Classes-----------------\n");
    // 通过matcher遍历类，如果匹配，则会执行lamda
    classMatcher.walk([&](const CLucid::ClassNode& ast, const CLucid::AnnotationContainer& annotationContainer) {
        // 输出类信息
        std::printf("class(AnnoName1=%s, AnnoName1=%s) %s {}",annotationContainer.findAnnotation("AnnoName1").c_str(),
                    annotationContainer.findAnnotation("AnnoName2").c_str(),
                    ast.fullName.c_str());

        // 遍历所有属性
        AnnotationMatcherProperty propertyMatcher;
        propertyMatcher.addKey("wantProp");
        propertyMatcher.walk([](const CLucid::PropertyNode& ast, const CLucid::AnnotationContainer& annotationContainer) {
            // 输出属性信息
            std::printf("\n");
            std::printf("  property(wantProp=%s)\n", annotationContainer.findAnnotation("wantProp").c_str());
            std::printf("%s\n", ast.toString("  ").c_str());
        }, &metaData, &ast);


        // 遍历所有函数
        AnnotationMatcherMethod methodMatcher;
        methodMatcher.addKey("wantMethod");
        methodMatcher.walk([](const CLucid::MethodNode& ast, const CLucid::AnnotationContainer& annotationContainer) {
            // 输出函数信息
            std::printf("\n");
            std::printf("  method(wantMethod=%s)\n", annotationContainer.findAnnotation("wantMethod").c_str());
            std::printf("%s\n", ast.toString("  ").c_str());
        }, &metaData, &ast);

    }, &metaData);

    std::printf("\n\n");
    std::printf("---------------- Matched Enums-----------------\n");

    AnnotationMatcherEnum enumMatcher;
    // 不添加任何注解key，则匹配所有
    // enumMatcher.addKey();
    enumMatcher.walk([](const CLucid::EnumNode& ast, const CLucid::AnnotationContainer& annotationContainer) {
        std::printf("enum() %s\n", ast.fullName.c_str());
        for (auto& item : ast.enumItems) {
            std::printf(" %s = %lld\n", std::get<0>(item).c_str(), std::get<1>(item));
        }
    }, &metaData);


    std::printf("\n\n");
    std::printf("---------------- Matched Functions-----------------\n");

    AnnotationMatcherFunction functionMatcher;
    functionMatcher.addKey("globalFunc");
    functionMatcher.walk([](const CLucid::FunctionNode& ast, const CLucid::AnnotationContainer& annotationContainer) {
         std::printf("function(globalFunc=%s)\n", annotationContainer.findAnnotation("globalFunc").c_str());
         std::printf("%s\n", ast.toString("").c_str());
    }, &metaData);

    return 0;
}