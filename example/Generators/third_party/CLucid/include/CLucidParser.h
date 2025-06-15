//
// Created by weichuanqi on 2025/5/22.
//

#ifndef CLUCID_CLUCIDPARSER_H
#define CLUCID_CLUCIDPARSER_H


#include "CLucidAst.h"
#include "CLucidDefines.h"
#include "support/Utils.h"
#include <string>
#include <vector>

namespace CLucid {

    /**
     * 解析器
     */
    class CLucidParser {
        friend class CLucidAst;
    private:
        CXIndex index;
        CXTranslationUnit translator;
    private:
        /** cpp版本，默认17 */
        int cppVersion;
        /** 要解析的源文件 */
        std::vector<std::string> sourceFils;
        /** 源文件需要的头文件 */
        std::vector<std::string> includePaths;
        std::string annotationClassName;
        std::string annotationMethodName;
        std::string annotationPropertyName;
        std::string annotationFunctionName;
        std::string annotationParamName;
        std::string annotationEnumName;

        enum CXChildVisitResult (*visitorFunc)(CXCursor, CXCursor, CXClientData);

    public:
        CLucidParser();
        ~CLucidParser();

        void addSourceFile(const char* sourceFile){
            sourceFils.emplace_back(sourceFile);
        }

        void addSourceFiles(std::vector<std::string>& inSourceFiles){
            for (auto& sourceFile:inSourceFiles) {
                addSourceFile(Utils::pathNormalize(sourceFile.c_str()).c_str());
            }
        }
        void addIncludePath(const char* includePath){
            std::string pathStr = Utils::pathNormalize(includePath);
            Utils::removeSuffix(pathStr, ".", pathStr);
            Utils::removeSuffix(pathStr, Utils::path_separator, pathStr);
            includePaths.emplace_back(pathStr);
        }

        void addIncludePaths(std::vector<std::string>& inIncludePaths){
            for (auto& includePath : inIncludePaths) {
                addIncludePath(includePath.c_str());
            }
        }

        void buildArguments(std::vector<std::string>& arguments) const;

        void setCppVersion(int inCppVersion);

        void parse(CLucidAst& outData);

        FORCE_INLINE void setVisitorFunc(CXChildVisitResult (*customVisitorFunc)(CXCursor, CXCursor, CXClientData)){
            visitorFunc = customVisitorFunc;
        }

        FORCE_INLINE void setLogLevel(ELogLevel inLevel){
            GLog.setLevel(inLevel);
        }

    private:
        void translate(std::string& sourceFile, const std::vector<const char *> &arguments, class CLucidWrapper& wrapper);

        static enum CXChildVisitResult visitor(CXCursor cursor, CXCursor parent, CXClientData data);

        ClassNode parseClass(const CXCursor& cursor, const std::string& annotationContent, CLucidAst& ast) const;
        [[nodiscard]] FunctionNode parseFunction(const CXCursor& cursor, const std::string& annotationContent) const;
        [[nodiscard]] MethodNode parseMethod(const CXCursor& cursor, const std::string& annotationContent) const;
        [[nodiscard]] ParamNode parseParam(const CXCursor& cursor) const;

        [[nodiscard]] PropertyNode parseProperty(const CXCursor& cursor, const std::string& annotationContent) const;
        [[nodiscard]] ConstructorNode parseConstructor(const CXCursor& cursor, const std::string& annotationContent) const;
        [[nodiscard]] TOwningPtr<DeconstructorNode> parseDeconstructor(const CXCursor& cursor, const std::string& annotationContent) const;
        [[nodiscard]] EnumNode parseEnum(const CXCursor& cursor, const std::string& annotationContent) const;

        [[nodiscard]] std::string getIncludePath(const std::string& fullPath) const;
    };


    class CLucidWrapper{
    public:
        CLucidParser& parser;
        CLucidAst& data;
        CLucidWrapper(CLucidParser& parser, CLucidAst& data) : parser(parser), data(data) {

        }
    };
}


#endif //CLUCID_CLUCIDPARSER_H
