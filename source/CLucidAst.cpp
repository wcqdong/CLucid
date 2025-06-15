//
// Created by weichuanqi on 2025/5/20.
//

#include "CLucidAst.h"
#include "CLucidParser.h"

CLucid::CLucidAst::CLucidAst()
    : success(true)
    , parser(nullptr)
{
}
void CLucid::CLucidAst::printDetails() {
    ELogLevel logLevel = GLog.getLevel();
    GLog.setLevel(ELogLevel::INFO);
    GLog.info("");
    GLog.info("---------------- parser info -----------------");
    if (parser){
        GLog.info("source files:");
        for  (const std::string& sourceFile : parser->sourceFils) {
            GLog.info("\t%s", sourceFile.c_str());
        }
        GLog.info("include directories:");
        for  (const std::string& includePath : parser->includePaths) {
            GLog.info("\t%s", includePath.c_str());
        }
    }

    GLog.info("");
    GLog.info("---------------- class info -----------------");
    for (const std::pair<const std::string, ClassNode>& classInfo : metaClassData) {
        GLog.info(classInfo.second.toString().c_str());
    }

    GLog.info("");
    GLog.info("---------------- enum info -----------------");
    for (const std::pair<const std::string, EnumNode>& enumInfo : metaEnumData) {
        GLog.info(enumInfo.second.toString().c_str());
    }

    GLog.info("");
    GLog.info("---------------- function info -----------------");
    for (const std::pair<const std::string, FunctionNode>& functionInfo : metaFunctionData) {
        GLog.info(functionInfo.second.toString().c_str());
    }
    GLog.setLevel(logLevel);
}
bool CLucid::CLucidAst::isSuccess() {
    return success;
}
