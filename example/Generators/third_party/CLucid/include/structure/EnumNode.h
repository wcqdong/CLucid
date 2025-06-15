//
// Created by weichuanqi on 2025/6/4.
//

#ifndef CLUCID_ENUMNODE_H
#define CLUCID_ENUMNODE_H

#include "structure/NodeBase.h"

namespace CLucid{
    class EnumNode : public NodeBase{
    public:

        /** 类全名nameSpace + 类名 */
        std::string fullName;
        /** 命名空间 */
        std::string nameSpace;
        /** 所在文件 */
        std::string filePath;
        std::string includePath;
        /** 枚举项 */
        std::vector<std::tuple<std::string, int64_t>> enumItems;

        bool bClassEnum;
        EnumNode(const CXCursor& cursor, const std::string& annotation)
            : NodeBase(cursor, annotation)
            , bClassEnum(false)
        {

        }

        EMetaType getType() override{
            return EMetaType::ENUM_TYPE;
        }

        std::string toString(const char* prefix = "") const;
    };
}



#endif //CLUCID_ENUMNODE_H
