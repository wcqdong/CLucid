//
// Created by weichuanqi on 2025/5/19.
//

#ifndef CLUCID_CLASSNODE_H
#define CLUCID_CLASSNODE_H

#include "ConstructorNode.h"
#include "structure/ConstructorNode.h"
#include "structure/MethodNode.h"
#include "structure/NodeBase.h"
#include "structure/PropertyNode.h"
#include "structure/DeconstructorNode.h"
#include <clang-c/Index.h>
#include <vector>
#include "support/SmartPoint.h"


namespace CLucid {

    /**
     * 类结构
     */
    class ClassNode : public NodeBase {
    public:
        /** 类全名nameSpace + 类名 */
        std::string fullName;
        /** 命名空间 */
        std::string nameSpace;
        /** 所有成员变量 */
        std::vector<PropertyNode> properties;
        /** 构造函数 */
        std::vector<ConstructorNode> constructors;
        /** 析构函数 */
        TOwningPtr<DeconstructorNode> deconstructor;
        /** 所有成员函数 */
        std::vector<MethodNode> methods;
        /** 所在文件 */
        std::string filePath;
        /** include所在文件 */
        std::string includePath;
        /** 父类 */
        std::vector<ClassNode*> baseClasses;
        /** 子类 */
        std::vector<ClassNode*> subClasses;
        /** 是否为struct */
        bool bStruct;

        explicit ClassNode(const CXCursor& cursor, const std::string& annotation);

        ~ClassNode();

        EMetaType getType() override{
            return EMetaType::CLASS_TYPE;
        }

        void addProperty(PropertyNode& info);

        void addMethod(MethodNode& info);

        std::string toString(const char* prefix = "") const;
        void addBaseClass(ClassNode* pNode);
        void addSubClass(ClassNode* pNode);
        void addConstructor(ConstructorNode& node);
    };
}


#endif //CLUCID_CLASSNODE_H

