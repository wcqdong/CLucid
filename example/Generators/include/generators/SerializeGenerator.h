//
// Created by weichuanqi on 2025/5/30.
//

#ifndef GENERATORS_SERIALIZEGENERATOR_H
#define GENERATORS_SERIALIZEGENERATOR_H


#include "GeneratorBase.h"
#include <vector>

namespace Generators{

    struct ClassInfo{
        const CLucid::ClassNode* classNode;
        std::vector<const CLucid::PropertyNode*> properties;

        explicit ClassInfo(const CLucid::ClassNode* classNode) : classNode(classNode){

        }
    };

    class SerializeGenerator : public GeneratorBase{
        const char* TEMPLATE_FILE_NAME = "serialize.template";

    private:
        inja::Template serializeTemplate;
        std::vector<ClassInfo> classes;
    public:
        void init() override;
        void collectData() override;
        void genFile(inja::json& json) override;
        void genGlobalFile(inja::json& json)override;
        void clear() override;
    };
}



#endif //GENERATORS_SERIALIZEGENERATOR_H
