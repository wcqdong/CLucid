//
// Created by weichuanqi on 2025/5/30.
//

#ifndef GENERATORS_GENERATORBASE_H
#define GENERATORS_GENERATORBASE_H

#include <string>
#include "inja.hpp"
#include "CLucidParser.h"

namespace Generators {
    class GeneratorBase {
    protected:
        bool bInit;
        CLucid::CLucidAst* ast;

        inja::Environment env;
        std::string templateRootPath;
        std::string outputRootPath;

        inja::json globalJson;
        std::vector<inja::json> jsons;

    public:
        GeneratorBase();
        virtual ~GeneratorBase();
    protected:
        virtual void init() = 0;
        virtual void collectData() = 0;
        virtual void genFile(inja::json& json) = 0;
        virtual void genGlobalFile(inja::json& json) = 0;
        virtual void clear(){
            jsons.clear();
            globalJson.clear();
        }
    public:

        void setAst(CLucid::CLucidAst& inAst);

        virtual void gen();

        /**
         * @brief 设置模板根路径
         * @param inTemplateRootPath
         */
        void setTemplateRootPath(const char* inTemplateRootPath);
        /**
         * @brief 设置输出根据路
         * @param inTargetRootPath
         */
        void setOutputRootPath(const char* inTargetRootPath);
    };
} // namespace Generator


#endif //GENERATORS_GENERATORBASE_H
