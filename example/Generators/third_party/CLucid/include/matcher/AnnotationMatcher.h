//
// Created by weichuanqi on 2025/5/21.
//

#ifndef CLUCID_ANNOTATIONMATCHER_H
#define CLUCID_ANNOTATIONMATCHER_H

#include "CLucidAst.h"
#include "structure/NodeBase.h"
#include <string>
#include <vector>
#include <functional>
#include <unordered_set>

namespace CLucid{

    /**
     * 注解匹配器
     */
    class AnnotationMatcher {
        /** 匹配那种类型的注解 */
        EMetaType type;
    protected:
        /** 要匹配的注解中的key */
        std::unordered_set<std::string> keys;

    public:
        explicit AnnotationMatcher(EMetaType type) : type(type) {}

        FORCE_INLINE void addKey(const std::string& name) {
            keys.emplace(name);
        }

        [[nodiscard]] FORCE_INLINE EMetaType getType() const {
            return type;
        }
    };

    /**
     * 类注解匹配器
     */
    class AnnotationMatcherClass : public AnnotationMatcher {
    public:
        explicit AnnotationMatcherClass() : AnnotationMatcher(EMetaType::CLASS_TYPE) {}

        void walk(const std::function<void(const ClassNode&, const AnnotationContainer&)>& pFunction, const CLucidAst* metaAst);

    };

    /**
     * 方法注解匹配器
     */
    class AnnotationMatcherMethod : public AnnotationMatcher {
    public:
        explicit AnnotationMatcherMethod() : AnnotationMatcher(EMetaType::METHOD_TYPE) {}
        void walk(const std::function<void(const MethodNode&, const AnnotationContainer&)>& pFunction, const CLucidAst* metaAst, const ClassNode* classNode = nullptr);
    };

    /**
     * 属性注解匹配器
     */
    class AnnotationMatcherProperty : public AnnotationMatcher {
    public:
        explicit AnnotationMatcherProperty() : AnnotationMatcher(EMetaType::PROPERTY_TYPE) {}
        void walk(const std::function<void(const PropertyNode&, const AnnotationContainer&)>& pFunction, const CLucidAst* metaAst, const ClassNode* classNode = nullptr);
    };

    /**
     * 方法注解匹配器
     */
    class AnnotationMatcherFunction : public AnnotationMatcher {
    public:
        explicit AnnotationMatcherFunction() : AnnotationMatcher(EMetaType::FUNCTION_TYPE) {}
        void walk(const std::function<void(const FunctionNode&, const AnnotationContainer&)>& pFunction, const CLucidAst* metaAst);
    };

    /**
     * 枚举注解匹配器
     */
    class AnnotationMatcherEnum : public AnnotationMatcher {
    public:
        explicit AnnotationMatcherEnum() : AnnotationMatcher(EMetaType::ENUM_TYPE) {}
        void walk(const std::function<void(const EnumNode&, const AnnotationContainer&)>& pFunction, const CLucidAst* metaAst);
    };

    template<typename T>
    void matchAnnotation(const T& node, const std::unordered_set<std::string>& keys, const std::function<void(const T&, const AnnotationContainer&)>& pFunction, std::unordered_set<std::string>& matchedKeys){
        // 遍历function的annotation
        node.getAnnotationContainer().foreach([&](const std::string& key, const std::string& value){
            if(keys.find(key) != keys.end()){
                matchedKeys.emplace(value);
            }
        });
        if (matchedKeys.size() == keys.size()) {
            pFunction(node, node.getAnnotationContainer());
        }
        matchedKeys.clear();
    }

}


#endif //CLUCID_ANNOTATIONMATCHER_H
