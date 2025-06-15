//
// Created by weichuanqi on 2025/5/20.
//

#ifndef CLUCID_FUNCTIONNODE_H
#define CLUCID_FUNCTIONNODE_H

#include "structure/NodeBase.h"
#include "structure/ParamNode.h"
#include <vector>

namespace CLucid {

    /**
     * 静态函数结构
     */
    class FunctionNode : public NodeBase {
    public:
        /** 命名空间 */
        std::string nameSpace;
        /** 返回值类型 */
        std::string returnTypeName;
        /** 形参 */
        std::vector<ParamNode> params;
        /** 全名：函数名(Param1 param1, Param2 param2, ...) */
        std::string fullName;
        /** 所在文件 */
        std::string filePath;
        std::string includePath;

        explicit FunctionNode(const CXCursor& cursor, const std::string& annotation)
            : NodeBase(cursor, annotation) {
        }

        EMetaType getType() override {
            return EMetaType::FUNCTION_TYPE;
        }

        void addParam(ParamNode& node);

        std::string toString(const char* prefix = "") const;
    };
} // namespace CLucid


#endif //CLUCID_FUNCTIONNODE_H
