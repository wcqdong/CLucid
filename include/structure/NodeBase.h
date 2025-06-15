//
// Created by weichuanqi on 2025/5/20.
//

#ifndef CLUCID_NODEBASE_H
#define CLUCID_NODEBASE_H

#include "support/PreProcessor.h"
#include <clang-c/Index.h>
#include <string>
#include <vector>
#include "support/Utils.h"
#include "support/Log.h"
#include <functional>

namespace CLucid{

    using EAccessSpecifier = CX_CXXAccessSpecifier;

    enum class EMetaType{
        CLASS_TYPE,
        CONSTRUCTOR_TYPE,
        DECONSTRUCTOR_TYPE,
        METHOD_TYPE,
        PROPERTY_TYPE,
        FUNCTION_TYPE,
        PARAM_TYPE,
        ENUM_TYPE,
    };

    struct AnnotationContainer{
    private:
        std::vector<std::tuple<std::string, std::string>> annotationPairs;
    public:
        FORCE_INLINE std::string findAnnotation(const std::string& annotationName) const{
            for(auto& pair : annotationPairs){
                if(std::get<0>(pair) == annotationName){
                    return std::get<1>(pair);
                }
            }
            return "";
        }

        FORCE_INLINE void addAnnotation(const std::string& annotationName, const std::string& annotationValue){
            annotationPairs.emplace_back(annotationName, annotationValue);
        }

        FORCE_INLINE void foreach(const std::function<void(const std::string&, const std::string&)>& pFunction) const{
            for (auto& pair : annotationPairs) {
                pFunction(std::get<0>(pair), std::get<1>(pair));
            }
        }

    };

    /**
     * 结构的父类
     */
    class NodeBase {
    protected:
        const CXCursor cursor;
        /** 注解 */
        std::string annotation;
        /** 解析注解后的结构，key-value */
        AnnotationContainer annotationContainer;
        /** 父节点 */
        NodeBase* parent;
    private:
        void init();

        /// 即系注解字符串
        void parseAnnotation();

    public:
        /** 节点名，可能是类名，，可能是函数名，可能是属性名，可能是参数名 */
        std::string name;

        explicit NodeBase(const CXCursor& cursor, const std::string& annotation);

        virtual EMetaType getType() = 0;

        void setParent(NodeBase* inParent);

        [[nodiscard]] const std::string& getName() const;

        void setName(const char* inName);

        [[nodiscard]] const AnnotationContainer& getAnnotationContainer() const;
    };
}

#endif //CLUCID_NODEBASE_H
