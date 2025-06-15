//
// Created by weichuanqi on 2025/5/19.
//

#ifndef CLUCID_PROPERTYNODE_H
#define CLUCID_PROPERTYNODE_H

#include "structure/NodeBase.h"
#include "support/Helper.h"
#include <string>

namespace CLucid {
    class PropertyNode : public NodeBase {

    public:
        /** 参数类型 */
        Helper::ParamTypeInfo typeInfo;
        /** 访问修饰符 */
        EAccessSpecifier accessSpecifier;
        /** 偏移 */
        int64_t offset;
        /** 大小 */
        int64_t size;

        explicit PropertyNode(const CXCursor& cursor, const std::string& annotation);

        EMetaType getType() override{
            return EMetaType::PROPERTY_TYPE;
        }

        std::string toString(const char* prefix = "") const;

        class ClassNode* getClassNode(){
            return reinterpret_cast<class ClassNode*>(parent);
        }
    };
}

#endif //CLUCID_PROPERTYNODE_H
