//
// Created by weichuanqi on 2025/5/19.
//

#ifndef CLUCID_PARAMNODE_H
#define CLUCID_PARAMNODE_H


#include <string>
#include "structure/NodeBase.h"
#include "support/Log.h"
#include "support/Helper.h"

namespace CLucid {
    /**
     * 参数节点
     */
    class ParamNode : public NodeBase {

    public:
        /** 参数类型 */
        Helper::ParamTypeInfo typeInfo;
        /** 所在文件 */
        std::string filePath;
        /** include所在文件 */
        std::string includePath;

        explicit ParamNode(const CXCursor& cursor, std::string& annotation) : NodeBase(cursor, annotation){

        }

        inline EMetaType getType() override{
            return EMetaType::PARAM_TYPE;
        }

        std::string toString(const char* prefix = "") const;

        class MethodNode* getMethodNode();
    };
}


#endif //CLUCID_PARAMNODE_H
