//
// Created by weichuanqi on 2025/5/20.
//

#include "support/Helper.h"
#include "support/Utils.h"

namespace CLucid::Helper {
    void getNameSpacePrivate(CXCursor cursor, char* namespaceBuffer, size_t bufferSize) {
        if (clang_Cursor_isNull(cursor)) {
            return;
        }

        CXCursor parentCursor = clang_getCursorSemanticParent(cursor);
        if (clang_Cursor_isNull(parentCursor)) {
            return;
        }

        if (clang_getCursorKind(parentCursor) == CXCursor_Namespace) {
            CXString namespaceName = clang_getCursorSpelling(parentCursor);
            const char* namespaceNameCStr = clang_getCString(namespaceName);

            // 递归地获取父命名空间
            getNameSpacePrivate(parentCursor, namespaceBuffer, bufferSize);

            // 添加当前命名空间到缓冲区
            if (strlen(namespaceBuffer) > 0) {
                strncat_s(namespaceBuffer, bufferSize, "::", bufferSize - strlen(namespaceBuffer) - 1);
            }
            strncat_s(namespaceBuffer, bufferSize, namespaceNameCStr, bufferSize - strlen(namespaceBuffer) - 1);

            clang_disposeString(namespaceName);
        } else {
            // 继续向上查找父光标
            getNameSpacePrivate(parentCursor, namespaceBuffer, bufferSize);
        }
    }

    std::string clangGetNameSpace(const CXCursor& cursor) {
        char namespaceBuffer[1024] = {0};
        size_t bufferSize = sizeof(namespaceBuffer);
        getNameSpacePrivate(cursor, namespaceBuffer, bufferSize);
        return namespaceBuffer;
    }

    std::string clangGetFullName(const CXCursor& cursor) {
        char namespaceBuffer[1024] = {0};
        size_t bufferSize = sizeof(namespaceBuffer);
        getNameSpacePrivate(cursor, namespaceBuffer, bufferSize);
        strncat_s(namespaceBuffer, "::", bufferSize - strlen(namespaceBuffer) - 1);
        strncat_s(namespaceBuffer, clangGetName(cursor).c_str(), bufferSize - strlen(namespaceBuffer) - 1);

        return namespaceBuffer;
    }

    AnnotationFinder hasAnnotation(const CXCursor& cursor, const std::string& annotationName) {
        AnnotationFinder Finder(annotationName.c_str());
        clang_visitChildren(cursor, annotationVisitor, &Finder);
        return Finder;
    }

    enum CXChildVisitResult annotationVisitor(CXCursor cursor, CXCursor parent, CXClientData client_data) {
        if (clang_getCursorKind(cursor) == CXCursor_AnnotateAttr) {
            auto* finder = (AnnotationFinder*) client_data;
            std::string annotationContent = clangGetName(cursor);
            if (Utils::startsWith(annotationContent, finder->name)) {
                finder->founded = true;
                finder->annotationContent = annotationContent.substr(finder->name.size(),
                                                                     annotationContent.length() - finder->name.size());
            }
            return CXChildVisit_Break; // Stop further traversal
        }
        return CXChildVisit_Continue;
    }

    std::string ClangGetHeaderFile(const CXCursor& cursor) {
        std::string result;

        CXSourceLocation location = clang_getCursorLocation(cursor);
        CXFile file;
        unsigned line, column, offset;
        clang_getExpansionLocation(location, &file, &line, &column, &offset);

        if (file) {
            CXString filename = clang_File_tryGetRealPathName(file);
            const char* cstr = clang_getCString(filename);
            if (cstr) {
                // Check if the file has a header extension
                const char* headerExtensions[] = {".h", ".hpp", ".hh", ".hxx", ".h++"};
                size_t len = sizeof(headerExtensions) / sizeof(headerExtensions[0]);
                for (size_t i = 0; i < len; ++i) {
                    if (strstr(cstr, headerExtensions[i])) {
                        result = cstr;
                        //                    printf("Cursor is in header file: %s\n", cstr);
                        break;
                    }
                }
            }
            clang_disposeString(filename);
        } else {
            printf("Cursor is not in a header file.\n");
        }

        return result;
    }

    std::string clangConvertString(const CXString& str) {
        std::string c_str = clang_getCString(str);
        clang_disposeString(str);
        return c_str;
    }

    std::string clangGetName(const CXCursor& cursor) {
        const CXString& str = clang_getCursorSpelling(cursor);
        std::string c_str = clang_getCString(str);
        clang_disposeString(str);
        return c_str;
    }

    std::string clangGetName(const CXCursorKind& kind) {
        const CXString& str = clang_getCursorKindSpelling(kind);
        std::string c_str = clang_getCString(str);
        clang_disposeString(str);
        return c_str;
    }

    bool clangGetLocation(const CXCursor& cursor, std::string& filePath, unsigned int* line, unsigned int* column) {
        CXSourceLocation location = clang_getCursorLocation(cursor);

        // 获取文件，行和列的信息
        CXFile file;
        unsigned offset;
        clang_getExpansionLocation(location, &file, line, column, &offset);

        // 获取文件名
        if (file) {
            CXString fileName = clang_getFileName(file);
            filePath = clang_getCString(fileName);
            filePath = Utils::pathNormalize(filePath.c_str());
            clang_disposeString(fileName);
            return true;
        } else {
            return false;
        }
    }
    std::string accessSpecifierToString(CX_CXXAccessSpecifier accessSpecifier) {
        if (accessSpecifier == CX_CXXPublic) {
            return "public";
        } else if (accessSpecifier == CX_CXXProtected) {
            return "protected";
        } else if (accessSpecifier == CX_CXXPrivate) {
            return "private";
        }
        return "invalid";
    }
    bool isBuiltinType(CXType type) {
        return isBuiltinType(type.kind);
    }

    bool isBuiltinType(CXTypeKind kind){
        return (kind >= CXType_FirstBuiltin && kind <= CXType_LastBuiltin);
    }


    std::string getParamTypeLayer(CXType type, ParamTypeInfo& paramDecl, bool parentConst = false) {
        bool bConst = clang_isConstQualifiedType(type);
        switch (type.kind) {
            case CXType_Elaborated: {
                // 处理详细说明类型
                CXType namedType = clang_Type_getNamedType(type);
                if (namedType.kind != CXType_Invalid) {
                    return getParamTypeLayer(namedType, paramDecl, bConst); // 递归处理
                }
                break;
            }

            case CXType_Typedef: {
                // 处理typedef类型
                CXCursor typedefDecl = clang_getTypeDeclaration(type);
                CXType underlyingType = clang_getTypedefDeclUnderlyingType(typedefDecl);
                return getParamTypeLayer(underlyingType, paramDecl); // 递归处理
            }

            case CXType_Pointer: {
                // 处理指针类型
                CXType pointeeType = clang_getPointeeType(type);
                paramDecl.paramTypeLayers.emplace_back(bConst, "*");
                return getParamTypeLayer(pointeeType, paramDecl) + "*" + (bConst ? " const " : "");
            }

            case CXType_LValueReference: {
                // 处理左值引用
                CXType pointeeType = clang_getPointeeType(type);
                paramDecl.paramTypeLayers.emplace_back(bConst, "&");
                return getParamTypeLayer(pointeeType, paramDecl) + "&" + (bConst ? " const " : "");
            }

            case CXType_RValueReference: {
                // 处理右值引用
                CXType pointeeType = clang_getPointeeType(type);
                paramDecl.paramTypeLayers.emplace_back(bConst, "&&");
                return getParamTypeLayer(pointeeType, paramDecl) + "&&" + (bConst ? " const " : "");
            }

            case CXType_Record: {
                // 获取类型声明的cursor
                CXCursor recordDecl = clang_getTypeDeclaration(type);

                paramDecl.bConst = bConst | parentConst;
                paramDecl.cleanCursor = recordDecl;
                paramDecl.cleanFullName = clangGetFullName(recordDecl);
                return (paramDecl.bConst ? " const " : "") + paramDecl.cleanFullName;
            }

            case CXType_Void: {
                CXCursor decl = clang_getTypeDeclaration(type);
                paramDecl.cleanCursor = decl;
                paramDecl.bConst  = bConst | parentConst;
                paramDecl.cleanFullName = "void";
                paramDecl.paramTypeLayers.emplace_back(bConst, "void");
                return paramDecl.cleanFullName;
            }
            default: {
                // 默认情况：返回类型的字符串表示
                CXString typeSpelling = clang_getTypeSpelling(type);
                paramDecl.bConst = bConst | parentConst;
                CXCursor decl = clang_getTypeDeclaration(type);
                paramDecl.cleanCursor  = decl;
                paramDecl.cleanFullName = clang_getCString(typeSpelling);
                Utils::removePrefix(paramDecl.cleanFullName, "const ", paramDecl.cleanFullName);
                clang_disposeString(typeSpelling);
                return (paramDecl.bConst? " const ": "") + paramDecl.cleanFullName;
            }
        }

        return "";
    }

    void clangGetParamDecl(CXType& type, ParamTypeInfo& info) {
        std::string fullName = getParamTypeLayer(type, info);
        info.fullName = fullName;
    }

    bool isTemplateType(CXType type) {
        CXString typeSpelling = clang_getTypeSpelling(type);
        std::string spelling = clang_getCString(typeSpelling);
        clang_disposeString(typeSpelling);

        return spelling.find('<') != std::string::npos &&
               spelling.find('>') != std::string::npos;
    }
} // namespace CLucid

//std::ostream &operator<<(std::ostream &stream, const CXString &str) {
//    std::string c_str = CLucid::clangConvertString(str);
//    stream << c_str;
//    return stream;
//}
