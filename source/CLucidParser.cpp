//
// Created by weichuanqi on 2025/5/22.
//

#include "CLucidParser.h"
#include "support/Helper.h"
#include <functional>

using namespace CLucid;
CLucidParser::CLucidParser()
    : cppVersion(17)
    , annotationClassName(ANO_CLASS)
    , annotationMethodName(ANO_METHOD)
    , annotationFunctionName(ANO_FUNCTION)
    , annotationPropertyName(ANO_PROPERTY)
    , annotationParamName(ANO_PARAM)
    , annotationEnumName(ANO_ENUM)
{
    visitorFunc = [](CXCursor cursor, CXCursor parent, CXClientData data) -> CXChildVisitResult {
        return visitor(cursor, parent, data);
    };
}

CLucidParser::~CLucidParser() {
    if(index)
        clang_disposeIndex(index);
}

void CLucidParser::buildArguments(std::vector<std::string>& arguments) const {
    arguments.emplace_back("c++");
    arguments.emplace_back("-E");
    arguments.emplace_back(("-std=c++" + std::to_string(cppVersion)).c_str());
    arguments.emplace_back("-D __clang__");
    arguments.emplace_back("-D __CLUCID_PARSER__");

    for (auto& path: includePaths) {
        arguments.emplace_back(("-I" + std::string(path)).c_str());
    }
}

void CLucidParser::setCppVersion(int inCppVersion) {
    cppVersion = inCppVersion;
}

void CLucidParser::parse(CLucid::CLucidAst& outData) {
    // 按长度从高到低排序
    std::sort(includePaths.begin(), includePaths.end(),
              [](const std::string& a, const std::string& b) {
                  return a.length() > b.length();
              });

    outData.parser = this;
    CLucidWrapper wrapper(*this, outData);

    // 构建参数
    std::vector<std::string> strArguments;
    buildArguments(strArguments);

    std::vector<const char*> arguments;
    arguments.reserve(strArguments.size());
    for (auto& argument: strArguments) {
        arguments.push_back(argument.c_str());
    }

    // 初始化Index
    index = clang_createIndex(0, 1);

    // 遍历解析源文件
    for (auto& sourceFile: sourceFils) {
        translate(sourceFile, arguments, wrapper);
    }
}

void CLucidParser::translate(std::string& sourceFile, const std::vector<const char*>& arguments, CLucidWrapper& wrapper) {
//    CXTranslationUnit_None
//    unsigned options = CXTranslationUnit_DetailedPreprocessingRecord |
//                       CXTranslationUnit_KeepGoing |
//                       CXTranslationUnit_IncludeAttributedTypes;
    unsigned options = CXTranslationUnit_DetailedPreprocessingRecord;
    translator = clang_parseTranslationUnit(
            index, sourceFile.c_str(), arguments.data(), arguments.size(), nullptr, 0, options);
    if (!translator) {
        throw std::runtime_error("❌ Failed to parse translation unit.");
    }

    GLog.info("✅ Start parsing translation unit: 【%s】", sourceFile.c_str());

    CXCursor rootCursor = clang_getTranslationUnitCursor(translator);

    clang_visitChildren(rootCursor, visitorFunc, reinterpret_cast<CXClientData>(&wrapper));

    clang_disposeTranslationUnit(translator);
}

CXChildVisitResult CLucid::CLucidParser::visitor(CXCursor cursor, CXCursor parent, CXClientData data) {

    auto* wrapper = reinterpret_cast<CLucidWrapper*>(data);

    CLucidAst& metaData = wrapper->data;
    CLucidParser& parser = wrapper->parser;

    // 遍历失败
    if (!metaData.isSuccess()) {
        return CXChildVisit_Break;
    }

    GLog.trace("------- cursor: [%s]        ------- parentCursor: [%s]", Helper::clangGetName(cursor).c_str(), Helper::clangGetName(parent).c_str());

    auto kind = clang_getCursorKind(cursor);
    Helper::AnnotationFinder finder = Helper::AnnotationFinder(nullptr);
//    if (kind == CXCursor_InclusionDirective){
//        std::string includedFilePath;
//        // 获取包含的文件
//        CXFile includedFile = clang_getIncludedFile(cursor);
//        if (includedFile) {
//            CXString fileName = clang_getFileName(includedFile);
//            includedFilePath = clang_getCString(fileName);
//            clang_disposeString(fileName);
//        }
//
//        // 获取包含文本
//        CXString includeText = clang_getCursorSpelling(cursor);
//        std::string includeTextStr = clang_getCString(includeText);
//        clang_disposeString(includeText);
//
//        // 获取位置信息
//        std::string filePath;
//        Helper::clangGetLocation(cursor, filePath);
//
//        std::string parentStr = Helper::clangGetName(parent);
//        return CXChildVisit_Continue;
//    }
    if (kind == CXCursor_ClassDecl ||  kind == CXCursor_StructDecl) {
        finder = Helper::hasAnnotation(cursor, parser.annotationClassName);
        if (finder.founded) {
            GLog.debug("------- cursor: [%s]        ------- parentCursor: [%s]", Helper::clangGetName(cursor).c_str(), Helper::clangGetName(parent).c_str());
            std::string classFullName = Helper::clangGetFullName(cursor);
            // 第一次遍历到
            if(!metaData.has(classFullName)){
                metaData.push(classFullName);
                metaData.emplaceClass(classFullName, parser.parseClass(cursor, finder.annotationContent, metaData));
                metaData.pop(classFullName);
            }
        }
        return CXChildVisit_Continue;
    }
    // 暂时不支持全局函数
    else if (kind == CXCursor_FunctionDecl) {
        finder = Helper::hasAnnotation(cursor, parser.annotationFunctionName);
        if (finder.founded) {
            GLog.debug("------- cursor: [%s]        ------- parentCursor: [%s]", Helper::clangGetName(cursor).c_str(), Helper::clangGetName(parent).c_str());

            std::string name = Helper::clangGetName(cursor);

            FunctionNode functionInfo = parser.parseFunction(cursor, finder.annotationContent);
            std::string functionFullName = functionInfo.nameSpace + "::" + functionInfo.fullName;

            if (!metaData.has(functionFullName)) {
                metaData.addMetaFunction(functionFullName, functionInfo);
            }
        }
        return CXChildVisit_Continue;
    }
    else if (kind == CXCursor_EnumDecl){
        finder = Helper::hasAnnotation(cursor, parser.annotationEnumName);
        if (finder.founded) {
            GLog.debug("------- cursor: [%s]        ------- parentCursor: [%s]", Helper::clangGetName(cursor).c_str(), Helper::clangGetName(parent).c_str());
            std::string enumFullName = Helper::clangGetFullName(cursor);
            // 第一次遍历到
            if (!metaData.has(enumFullName)) {
                metaData.push(enumFullName);
                metaData.emplaceEnum(enumFullName, parser.parseEnum(cursor, finder.annotationContent));
                metaData.pop(enumFullName);
            }
        }
        return CXChildVisit_Continue;
    }

    return CXChildVisit_Recurse;
}
ClassNode CLucidParser::parseClass(const CXCursor& cursor, const std::string& annotationContent, CLucidAst& ast) const{
    ClassNode classInfo(cursor, annotationContent);
    classInfo.name = Helper::clangGetName(cursor);
    classInfo.nameSpace = Helper::clangGetNameSpace(cursor);
    classInfo.fullName = classInfo.nameSpace + "::" + classInfo.name;
    if (classInfo.nameSpace.empty()){
        classInfo.nameSpace = "::";
    }

    Helper::clangGetLocation(cursor, classInfo.filePath);
    classInfo.includePath = getIncludePath(classInfo.filePath);
    auto classKind = clang_getCursorKind(cursor);
    classInfo.bStruct = classKind == CXCursor_StructDecl;

    struct BaseClassWrapper{
        const CLucidParser& parser;
        CLucidAst& ast;
        ClassNode& classNode;
        BaseClassWrapper(const CLucidParser& parser, CLucidAst& ast, ClassNode& classNode) : parser(parser), ast(ast), classNode(classNode) {
        }
    };

    BaseClassWrapper wrapper(*this, ast, classInfo);

    // 查找父类
    clang_visitChildren(cursor, [](CXCursor c, CXCursor parent, CXClientData client_data) {
        GLog.trace("------- cursor: [%s]        ------- parentCursor: [%s]", Helper::clangGetName(c).c_str(), Helper::clangGetName(parent).c_str());

        CXCursorKind kind = clang_getCursorKind(c);
        if (kind == CXCursor_CXXBaseSpecifier) {
            GLog.debug("------- cursor: [%s]        ------- parentCursor: [%s]", Helper::clangGetName(c).c_str(), Helper::clangGetName(parent).c_str());

            auto* wrapper = static_cast<BaseClassWrapper*>(client_data);
            CLucidAst& ast = wrapper->ast;
            ClassNode& classNode = wrapper->classNode;

            CXType baseType = clang_getCursorType(c);
            CXCursor baseCursor = clang_getTypeDeclaration(baseType);
            std::string baseClassName = Helper::clangGetName(baseCursor);

            std::string baseClassNameSpace = Helper::clangGetNameSpace(baseCursor);
            std::string baseClassFullName = baseClassNameSpace + "::" + baseClassName;
            ClassNode* baseClassNode;
            if (ast.findMetaClass(baseClassFullName, baseClassNode)) {
                classNode.addBaseClass(baseClassNode);
            }else{
                GLog.error("[%s] not found parent class [%s]", classNode.name.c_str(), baseClassName.c_str());
            }
        }
        return CXChildVisit_Continue;
    }, &wrapper);

    // 查找成员
    clang_visitChildren(cursor, [](CXCursor c, CXCursor parent, CXClientData client_data) {
        GLog.trace("------- cursor: [%s]        ------- parentCursor: [%s]", Helper::clangGetName(c).c_str(), Helper::clangGetName(parent).c_str());
        auto wrapper = static_cast<BaseClassWrapper*>(client_data);
        const CLucidParser& parser = wrapper->parser;
        CLucidAst& metaData = wrapper->ast;
        ClassNode& classNode = wrapper->classNode;

        CXCursorKind kind = clang_getCursorKind(c);
        Helper::AnnotationFinder finder = Helper::AnnotationFinder(nullptr);
        if (kind == CXCursor_CXXMethod) {
            finder = Helper::hasAnnotation(c, parser.annotationMethodName);
            if (finder.founded) {
                GLog.debug("------- cursor: [%s]        ------- parentCursor: [%s]", Helper::clangGetName(c).c_str(), Helper::clangGetName(parent).c_str());
                MethodNode methodInfo = parser.parseMethod(c, finder.annotationContent);
                classNode.addMethod(methodInfo);
            }
        } else if(kind == CXCursor_Constructor){
            GLog.debug("------- cursor: [%s]        ------- parentCursor: [%s]", Helper::clangGetName(c).c_str(), Helper::clangGetName(parent).c_str());
            finder = Helper::hasAnnotation(c, parser.annotationMethodName);
            ConstructorNode constructorInfo = parser.parseConstructor(c, finder.annotationContent);
            classNode.addConstructor(constructorInfo);
        } else if(kind == CXCursor_Destructor){
            finder = Helper::hasAnnotation(c, parser.annotationMethodName);
            classNode.deconstructor = parser.parseDeconstructor(c, finder.annotationContent);
        }else if (kind == CXCursor_FieldDecl) {
            finder = Helper::hasAnnotation(c, parser.annotationPropertyName);
            if (finder.founded) {
                GLog.debug("------- cursor: [%s]        ------- parentCursor: [%s]", Helper::clangGetName(c).c_str(), Helper::clangGetName(parent).c_str());
                PropertyNode propertyInfo = parser.parseProperty(c, finder.annotationContent);
                classNode.addProperty(propertyInfo);
            }
        }

        return CXChildVisit_Continue;
    }, &wrapper);

    return classInfo;
}
FunctionNode CLucidParser::parseFunction(const CXCursor& cursor, const std::string& annotationContent) const{
    FunctionNode functionInfo(cursor, annotationContent);
    functionInfo.name = Helper::clangGetName(cursor);
    functionInfo.nameSpace = Helper::clangGetNameSpace(cursor);

    // 获取该函数的类型
    CXType functionType = clang_getCursorType(cursor);
    // 获取函数的返回类型
    CXType returnType = clang_getResultType(functionType);
    // 解析returnType
    CXString returnTypeSpelling = clang_getTypeSpelling(returnType);
    functionInfo.returnTypeName = clang_getCString(returnTypeSpelling);

    int numArgs = clang_getNumArgTypes(functionType);
    if (numArgs == -1) {
        GLog.error("解析函数参数错误 func=%s", functionInfo.name.c_str());
        return functionInfo;
    }

    // 遍历每个参数，获取参数类型和名称
    for (int i = 0; i < numArgs; i++) {
        CXCursor argCursor = clang_Cursor_getArgument(cursor, i);
        ParamNode paramInfo = parseParam(argCursor);
        functionInfo.addParam(paramInfo);
    }

    // 函数唯一名字 【static? 返回值(参数类型1, 参数类型2, ……)】
    std::string fullNameFormat = Utils::format("{0}::{1}(", functionInfo.nameSpace, functionInfo.name);
    for (int i = 0; i < functionInfo.params.size(); ++i) {
        const ParamNode& param = functionInfo.params[i];
        fullNameFormat += param.typeInfo.fullName;
        if (i != functionInfo.params.size() - 1) {
            fullNameFormat += ", ";
        }
    }
    fullNameFormat += ")";
    functionInfo.fullName = fullNameFormat;
    if (functionInfo.nameSpace.empty()){
        functionInfo.nameSpace = "::";
    }

    Helper::clangGetLocation(cursor, functionInfo.filePath);
    functionInfo.includePath = getIncludePath(functionInfo.filePath);

    return functionInfo;
}
MethodNode CLucidParser::parseMethod(const CXCursor& cursor, const std::string& annotationContent) const{
    MethodNode methodNode(cursor, annotationContent);
    methodNode.name = Helper::clangGetName(cursor);
    methodNode.bStatic = clang_CXXMethod_isStatic(cursor);
    methodNode.bConst = clang_CXXMethod_isConst(cursor);
    methodNode.bVirtual = clang_CXXMethod_isVirtual(cursor);
    methodNode.bPureVirtual = clang_CXXMethod_isPureVirtual(cursor);
    methodNode.accessSpecifier = clang_getCXXAccessSpecifier(cursor);

    if(methodNode.bVirtual){
        // 检查是否为 override
        CXCursor *overridden;
        unsigned num_overridden;
        clang_getOverriddenCursors(cursor, &overridden, &num_overridden);
        if (num_overridden > 0) {
            methodNode.bOverride = true;
            clang_disposeOverriddenCursors(overridden);
        }
    }

    // 获取该函数的类型
    CXType functionType = clang_getCursorType(cursor);
    // 获取函数的返回类型
    CXType returnType = clang_getResultType(functionType);
    // 解析returnType
    CXString returnTypeSpelling = clang_getTypeSpelling(returnType);
    methodNode.returnTypeName = clang_getCString(returnTypeSpelling);
    // 参数类型
    Helper::clangGetParamDecl(returnType, methodNode.returnTypeInfo);

    int numArgs = clang_getNumArgTypes(functionType);
    if (numArgs == -1) {
        GLog.error("解析函数参数错误 func=%s", methodNode.name.c_str());
        return methodNode;
    }

    // 遍历每个参数，获取参数类型和名称
    for (int i = 0; i < numArgs; i++) {
        CXCursor argCursor = clang_Cursor_getArgument(cursor, i);
        ParamNode paramInfo = parseParam(argCursor);
        methodNode.params.push_back(paramInfo);
    }

    // 函数唯一名字 【static? 返回值(参数类型1, 参数类型2, ……)】
    std::string fullNameFormat = Utils::format("{0}(", methodNode.name);
    for (int i = 0; i < methodNode.params.size(); ++i) {
        const ParamNode& param = methodNode.params[i];
        fullNameFormat += param.typeInfo.fullName;
        if (i != methodNode.params.size() - 1) {
            fullNameFormat += ", ";
        }
    }
    fullNameFormat += ")";
    fullNameFormat += (methodNode.bConst ? " const" : "");
    methodNode.fullName = fullNameFormat;

    return methodNode;
}

ParamNode CLucidParser::parseParam(const CXCursor& cursor) const {
    std::string paramAnnotation;
    Helper::AnnotationFinder finder = Helper::hasAnnotation(cursor, ANO_PARAM);
    if (finder.founded) {
        paramAnnotation = finder.annotationContent;
    }
    ParamNode paramInfo(cursor, paramAnnotation);
    // 获得参数名
    paramInfo.name = Helper::clangGetName(cursor);
    // 获取参数的类型
    CXType type = clang_getCursorType(cursor);
    Helper::clangGetParamDecl(type, paramInfo.typeInfo);
    // 参数路径
    Helper::clangGetLocation(paramInfo.typeInfo.cleanCursor, paramInfo.filePath);
    paramInfo.includePath = getIncludePath(paramInfo.filePath);

    return paramInfo;
}

PropertyNode CLucidParser::parseProperty(const CXCursor& cursor, const std::string& annotationContent) const{
    CXType type = clang_getCursorType(cursor);

    PropertyNode propertyNode(cursor, annotationContent);
    propertyNode.name = Helper::clangGetName(cursor);
    propertyNode.accessSpecifier = clang_getCXXAccessSpecifier(cursor);
    propertyNode.offset = clang_Cursor_getOffsetOfField(cursor);
    propertyNode.size = clang_Type_getSizeOf(type);

    Helper::clangGetParamDecl(type, propertyNode.typeInfo);

    return propertyNode;
}
ConstructorNode CLucidParser::parseConstructor(const CXCursor& cursor, const std::string& annotationContent) const{
    ConstructorNode constructorNode(cursor, annotationContent);
    constructorNode.name = Helper::clangGetName(cursor);
    constructorNode.bCopy = clang_CXXConstructor_isCopyConstructor(cursor);
    constructorNode.bMove = clang_CXXConstructor_isMoveConstructor(cursor);
    constructorNode.bDefault = clang_CXXConstructor_isDefaultConstructor(cursor);
    constructorNode.accessSpecifier = clang_getCXXAccessSpecifier(cursor);

    int numArgs = clang_Cursor_getNumArguments(cursor);
    if (numArgs == -1) {
        GLog.error("解析函数参数错误 func=%s", constructorNode.name.c_str());
        return constructorNode;
    }

    CXType functionType = clang_getCursorType(cursor);

    // 遍历每个参数，获取参数类型和名称
    for (int i = 0; i < numArgs; i++) {
        CXCursor argCursor = clang_Cursor_getArgument(cursor, i);
        ParamNode paramInfo = parseParam(argCursor);

        constructorNode.params.push_back(paramInfo);
    }

    // 函数唯一名字 【static? 返回值(参数类型1, 参数类型2, ……)】
    std::string fullNameFormat = Utils::format("{0}(", constructorNode.name);
    for (int i = 0; i < constructorNode.params.size(); ++i) {
        const ParamNode& param = constructorNode.params[i];
        fullNameFormat += param.typeInfo.fullName;
        if (i != constructorNode.params.size() - 1) {
            fullNameFormat += ", ";
        }
    }
    fullNameFormat += ")";
    constructorNode.fullName = fullNameFormat;

    return constructorNode;
}
EnumNode CLucidParser::parseEnum(const CXCursor& cursor, const std::string& annotationContent) const{
    EnumNode enumNode(cursor, annotationContent);

    enumNode.name = Helper::clangGetName(cursor);
    enumNode.nameSpace = Helper::clangGetNameSpace(cursor);
    enumNode.fullName = enumNode.nameSpace + "::" + enumNode.name;
    if (enumNode.nameSpace.empty()){
        enumNode.nameSpace = "::";
    }
    Helper::clangGetLocation(cursor, enumNode.filePath);
    enumNode.includePath = getIncludePath(enumNode.filePath);

    enumNode.bClassEnum = clang_EnumDecl_isScoped(cursor);

    clang_visitChildren(cursor, [](CXCursor c, CXCursor parent, CXClientData client_data) {
        auto* enumNode = reinterpret_cast<EnumNode*>(client_data);
        CXCursorKind kind = clang_getCursorKind(c);
        if (kind == CXCursor_EnumConstantDecl) {
            std::string name = Helper::clangGetName(c);
            int64_t value = clang_getEnumConstantDeclValue(c);
            enumNode->enumItems.emplace_back(name, value);
        }
        return CXChildVisit_Continue;
    }, &enumNode);

    return enumNode;
}
std::string CLucidParser::getIncludePath(const std::string& fullPath) const{
    std::string result;
    for (auto& includePath: includePaths) {
        if(Utils::removePrefix(fullPath, includePath, result)){
            Utils::removePrefix(result, Utils::path_separator, result);
            return result;
        }
    }
    return fullPath;
}
TOwningPtr<DeconstructorNode> CLucidParser::parseDeconstructor(const CXCursor& cursor, const std::string& annotationContent) const {
    TOwningPtr<DeconstructorNode> deconstructorNode(new DeconstructorNode(cursor, annotationContent));
    deconstructorNode->name = Helper::clangGetName(cursor);
    deconstructorNode->bVirtual = clang_CXXMethod_isVirtual(cursor);
    deconstructorNode->bPureVirtual = clang_CXXMethod_isPureVirtual(cursor);
    deconstructorNode->accessSpecifier = clang_getCXXAccessSpecifier(cursor);

    if(deconstructorNode->bVirtual){
        // 检查是否为 override
        CXCursor *overridden;
        unsigned num_overridden;
        clang_getOverriddenCursors(cursor, &overridden, &num_overridden);
        if (num_overridden > 0) {
            deconstructorNode->bOverride = true;
            clang_disposeOverriddenCursors(overridden);
        }
    }

    return deconstructorNode;
}
