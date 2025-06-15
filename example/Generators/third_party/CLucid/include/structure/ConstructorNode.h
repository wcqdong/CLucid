//
// Created by weichuanqi on 2025/6/2.
//

#ifndef CLUCID_CONSTRUCTORNODE_H
#define CLUCID_CONSTRUCTORNODE_H

#include "NodeBase.h"
#include "ParamNode.h"

namespace CLucid{

    class ConstructorNode : public NodeBase{
     public:
         /** 全名：函数名(Param1 param1, Param2 param2, ...) const*/
         std::string fullName;
         /** 参数 */
         std::vector<ParamNode> params;
         /** 是否为复制构造函数 */
         bool bCopy;
         /** 是否为移动构造函数 */
         bool bMove;
         /** 是否为默认构造函数 */
         bool bDefault;
         /** 访问修饰符 */
         EAccessSpecifier accessSpecifier;

         explicit ConstructorNode(const CXCursor& cursor, const std::string& annotation);

         EMetaType getType() override{
            return EMetaType::CONSTRUCTOR_TYPE;
         }

         std::string toString(const char* prefix = "") const;
    };
}




#endif //CLUCID_CONSTRUCTORNODE_H
