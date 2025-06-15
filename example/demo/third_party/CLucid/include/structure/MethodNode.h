//
// Created by weichuanqi on 2025/5/19.
//

#ifndef CLUCID_METHODNODE_H
#define CLUCID_METHODNODE_H

#include <vector>
#include "structure/NodeBase.h"
#include "support/Log.h"
#include "support/Utils.h"
#include "structure/ParamNode.h"

namespace CLucid {

    /**
     * 成员函数
     */
    class MethodNode : public NodeBase {
    public:
        /** 返回值类型 */
        std::string returnTypeName;

        Helper::ParamTypeInfo returnTypeInfo;
        /** 参数列表 */
        std::vector<ParamNode> params;
        /** 全名：函数名(Param1 param1, Param2 param2, ...) const*/
        std::string fullName;
        /** 是否为静态函数 */
        bool bStatic;
        /** 是否为const */
        bool bConst;
        /** 是否为虚函数 */
        bool bVirtual;
        /** 是否为纯虚函数 */
        bool bPureVirtual;
        /** 是否为override */
        bool bOverride;
        /** 访问修饰符 */
        EAccessSpecifier accessSpecifier;

        explicit MethodNode(const CXCursor& cursor, const std::string& annotation);

        EMetaType getType() override{
            return EMetaType::METHOD_TYPE;
        }

        std::string toString(const char* prefix = "") const;

        class ClassNode* getClassNode(){
            return reinterpret_cast<class ClassNode*>(parent);
        }
    };
}

#endif //CLUCID_METHODNODE_H
