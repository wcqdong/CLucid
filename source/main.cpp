#include <iostream>

#include "CLucidAst.h"
#include "CLucidParser.h"
#include "matcher/AnnotationMatcher.h"


using namespace CLucid;

int main1() {

    CLucidParser parser;
    parser.setCppVersion(17);

    //    std::vector<const char*> sourceFils = {"../source/mm.cpp", "../source/mm.cpp"};
    //    parser.addIncludePath("../include");

    std::vector<std::string> sourceFils = {"../example/TestProject/source/Actor.cpp"};
    parser.addIncludePath("../include");
    parser.addIncludePath("../example/TestProject/include");

    parser.addSourceFiles(sourceFils);
    //    parser.setVisitorFunc([](CXCursor cursor, CXCursor parent, CXClientData data) -> CXChildVisitResult{
    //        return CXChildVisitResult::CXChildVisit_Break;
    //    });

    // 解析c++结构
    CLucidAst metaAst;
    parser.parse(metaAst);

    metaAst.printDetails();

    // 匹配注解上自定义的name
    AnnotationMatcherClass classMatcher;
//    classMatcher.addKey("ClassAAA");
//    classMatcher.addKey("ClassBBB");

    std::vector<CLucid::ClassNode> classNames;
    // 通过matcher遍历类，如果匹配，则会执行lamda
//    classMatcher.walk([&](const CLucid::ClassNode& ast, const std::vector<AnnotationItem>& matchedNames) {
//        GLog.info("");
//        std::string format = "CLASS(";
//        for (const AnnotationItem& item: matchedNames) {
//            format += item.name + "=" + item.value + ",";
//        }
//        format.append(")");
//        GLog.info("%s", format.c_str());
//        GLog.info("class %s {}", ast.getName().c_str());
//
//        classNames.push_back(const_cast<ClassNode&>(ast));
//
//    }, &metaAst);

    return 0;
}
