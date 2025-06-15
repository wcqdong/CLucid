//
// Created by weichuanqi on 2025/5/20.
//

#ifndef CLUCID_HELPER_H
#define CLUCID_HELPER_H


#include "clang-c/Index.h"
#include <cstring>
#include <string>
#include <vector>

namespace CLucid::Helper {

    /**
     * 获得名字
     * @param cursor
     * @return
     */
    std::string clangGetName(const CXCursor& cursor);

    std::string clangGetName(const CXCursorKind& kind);
    /**
     * 获得字符串
     * @param str
     * @return
     */
    std::string clangConvertString(const CXString& str);

    void getNameSpacePrivate(CXCursor cursor, char* namespaceBuffer, size_t bufferSize);

    /**
     * 获得NameSpace
     */
    std::string clangGetNameSpace(const CXCursor& cursor);

    /**
     * 获得类全名
     */
    std::string clangGetFullName(const CXCursor& cursor);

    /**
     * 获得头文件
     */
    std::string ClangGetHeaderFile(const CXCursor& cursor);

    /**
     * 查找是否有符合的注解
     */
    class AnnotationFinder {
    public:
        bool founded;
        std::string name;
        std::string annotationContent;
        explicit AnnotationFinder(const char* inName)
            : founded(false)
        {
            if(inName != nullptr){
                name = inName;
            }
        }
    };

    /**
     * 遍历注解，找到匹配的注解
     * @param cursor
     * @param parent
     * @param client_data
     * @return
     */
    enum CXChildVisitResult annotationVisitor(CXCursor cursor, CXCursor parent, CXClientData client_data);

    /**
     * 是否有匹配的注解
     * @param cursor
     * @param annotationName
     * @return
     */
    AnnotationFinder hasAnnotation(const CXCursor& cursor, const std::string& annotationName);

    /**
     * 获得Cursor的位置
     * @param cursor
     * @param filePath 所在的文件
     * @param line 所在的行
     * @param column 所在的列
     * @return
     */
    bool clangGetLocation(const CXCursor& cursor, std::string& filePath, unsigned* line = nullptr, unsigned* column = nullptr);

//    std::string clangGetFile(const CXCursor& cursor);

    std::string accessSpecifierToString( CX_CXXAccessSpecifier accessSpecifier);


    struct ParamTypeLayer{
        bool bConst;
        std::string name;

        ParamTypeLayer(bool bConst, const std::string& name): bConst(bConst), name(name) {}

        [[nodiscard]] std::string toString() const{
            return bConst ? name + " const ": name;
        }

    };
    struct ParamTypeInfo {
        /// 例如: const Item * const * & it ,  Item的namespace为ItemSpace
        /// 参数根类型，Item的cursor
        CXCursor cleanCursor{};
        /// namespace+名字 ItemSpace::Item
        std::string cleanFullName;
        /// const ItemSpace::Item * const * &
        std::string fullName;
        /// Item之前的const
        bool bConst;
        /// 多层修饰符，[{bConst:false, name:"&"}, {bConst:true, name:"*"}, {bConst:false, name:"*"}]
        std::vector<ParamTypeLayer> paramTypeLayers;

        ParamTypeInfo() : cleanCursor(), bConst(false){}

        std::string& buildFullName(){
            fullName = bConst ? "const " + cleanFullName: cleanFullName;
            for (auto& paramTypeLayer : paramTypeLayers) {
                fullName += paramTypeLayer.toString();
            }

            return fullName;
        }
    };

    /**
     * 是否为内置类型
     * @param type
     * @return
     */
    bool isBuiltinType(CXType type);
    bool isBuiltinType(CXTypeKind type);

    void clangGetParamDecl(CXType& type, ParamTypeInfo& info) ;

    bool isTemplateType(CXType type);


} // namespace CLucid::Helper

//std::ostream& operator<<(std::ostream& stream, const CXString& str);

#endif //CLUCID_HELPER_H
