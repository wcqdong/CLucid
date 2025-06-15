//
// Created by weichuanqi on 2025/5/20.
//

#ifndef CLUCID_CLUCIDAST_H
#define CLUCID_CLUCIDAST_H


#include "structure/ClassNode.h"
#include "structure/EnumNode.h"
#include "structure/FunctionNode.h"
#include <string>
#include <unordered_map>

namespace CLucid {

    class CLucidAst {
        bool success;
        /// 遍历记录，value如果为true则不重复解析
        std::unordered_map<std::string, bool> traverseRecord;
        /// 类
        std::unordered_map<std::string, ClassNode> metaClassData;
        /// 枚举
        std::unordered_map<std::string, EnumNode> metaEnumData;
        /// 静态函数
        std::unordered_map<std::string, FunctionNode> metaFunctionData;

    public:
        class CLucidParser* parser;

        CLucidAst();

        FORCE_INLINE void push(const std::string& classFullName) {
            traverseRecord.emplace(classFullName, true);
        }

        FORCE_INLINE void pop(const std::string& classFullName) {
            traverseRecord.find(classFullName)->second = false;
        }

        FORCE_INLINE void emplaceClass(const std::string& classFullName, const ClassNode&& info) {
            metaClassData.emplace(classFullName, info);
        }

        FORCE_INLINE void emplaceEnum(const std::string& classFullName, const EnumNode&& info) {
            metaEnumData.emplace(classFullName, info);
        }

        [[nodiscard]] FORCE_INLINE bool has(const std::string& classFullName) {
            return traverseRecord.find(classFullName) != traverseRecord.end();
        }

        FORCE_INLINE bool inProcess(const std::string& classFullName) {
            return traverseRecord.find(classFullName) != traverseRecord.end() && traverseRecord.find(classFullName)->second;
        }

        FORCE_INLINE void addMetaFunction(const std::string& name, const FunctionNode& info) {
            metaFunctionData.emplace(name, info);
        }

        [[nodiscard]] FORCE_INLINE  bool findMetaClass(const std::string& basicString, ClassNode*& outClassInfo) {
            auto iter = metaClassData.find(basicString);
            if (iter == metaClassData.end()) {
                return false;
            }
            outClassInfo = &iter->second;
            return true;
        }

        [[nodiscard]] FORCE_INLINE const std::unordered_map<std::string, ClassNode>& getMetaClassData() const {
            return metaClassData;
        }

        [[nodiscard]] FORCE_INLINE const std::unordered_map<std::string, FunctionNode>& getMetaFunctionData() const{
            return metaFunctionData;
        }

        [[nodiscard]] FORCE_INLINE const std::unordered_map<std::string, EnumNode>& getMetaEnumData() const {
            return metaEnumData;
        }

        void printDetails();
        bool isSuccess();
    };
} // namespace CLucid


#endif //CLUCID_CLUCIDAST_H
