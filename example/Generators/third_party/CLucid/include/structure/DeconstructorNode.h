//
// Created by weichuanqi on 2025/6/10.
//

#ifndef CLUCID_DECONSTRUCTORNODE_H
#define CLUCID_DECONSTRUCTORNODE_H

#include "NodeBase.h"

namespace CLucid {
    class DeconstructorNode : public NodeBase{
    public:
        /** 是否为虚函数 */
        bool bVirtual;
        /** 是否为纯虚函数 */
        bool bPureVirtual;
        /** 是否为override */
        bool bOverride;
        /** 访问修饰符 */
        EAccessSpecifier accessSpecifier;
    public:

        explicit DeconstructorNode(const CXCursor& cursor, const std::string& annotation);

        EMetaType getType() override{
            return EMetaType::DECONSTRUCTOR_TYPE;
        }

        std::string toString(const char* prefix = "") const;

        class ClassNode* getClassNode(){
            return reinterpret_cast<class ClassNode*>(parent);
        }
    };
}



#endif //CLUCID_DECONSTRUCTORNODE_H
