//
// Created by weichuanqi on 2025/5/30.
//

#include "GeneratorBase.h"

using namespace Generators;

GeneratorBase::GeneratorBase()
    : bInit(false)
    , ast(nullptr)
{
}

GeneratorBase::~GeneratorBase() {
}

void GeneratorBase::setAst(CLucid::CLucidAst& inAst) {
    ast = &inAst;
}

void GeneratorBase::setTemplateRootPath(const char* inTemplatePath) {
    std::string tempPath = CLucid::Utils::pathNormalize(inTemplatePath);
    if (!CLucid::Utils::endsWith(tempPath, CLucid::Utils::path_separator)){
        tempPath += CLucid::Utils::path_separator;
    }
    templateRootPath = tempPath;
}

void GeneratorBase::setOutputRootPath(const char* inTargetRootPath) {
    std::string tempPath = CLucid::Utils::pathNormalize(inTargetRootPath);
    if (!CLucid::Utils::endsWith(tempPath, CLucid::Utils::path_separator)){
        tempPath += CLucid::Utils::path_separator;
    }
    outputRootPath = tempPath;
}
void GeneratorBase::gen() {
    if (!bInit){
        init();
        bInit = true;
    }
    // 清理
    clear();
    // 收集数据
    collectData();
    // 生成全局文件
    genGlobalFile(globalJson);
    // 生成单个文件
    for (auto& json : jsons) {
        genFile(json);
    }

}

