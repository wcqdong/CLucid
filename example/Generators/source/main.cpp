#include "generators/SerializeGenerator.h"
#include <gflags/gflags.h>

#include "test/AActor.generated.h"
#include "test/AActor2.generated.h"

using namespace inja;
using namespace Generators;

DEFINE_string(cmd, "", "generator name, e.g.: 'serialize;reflect', split with ';' if multy");
DEFINE_int32(cpp_version, 17, "cpp version");
DEFINE_string(sources, "", "source files, split with ';' if multy");
DEFINE_string(includes, "", "include directories, split with ';' if multy");
DEFINE_string(template_path, "./", "template files path");
DEFINE_string(output_path, "./", "generate files output path");

void PrintAllFlags() {
    std::cout << "==== Commandline Input Parameters ==== " << std::endl;
    std::cout << "    --cmd: " << FLAGS_cmd << std::endl;
    std::cout << "    --cpp_version: " << FLAGS_cpp_version << std::endl;
    std::cout << "    --sources: " << FLAGS_sources << std::endl;
    std::cout << "    --includes: " << FLAGS_includes << std::endl;
    std::cout << "    --template_path: " << FLAGS_template_path << std::endl;
    std::cout << "    --output_path: " << FLAGS_output_path << std::endl;
}

/// 自测启动参数：
/// --cmd=serialize --cpp_version=17 --sources=../source/Actor.cpp --includes=../../../include;../include --template_path=../templates --output_path=../gen
int main(int argc, char* argv[]) {
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    PrintAllFlags();

    CLucid::CLucidParser parser;
    parser.setLogLevel(CLucid::ELogLevel::INFO);
    parser.setCppVersion(FLAGS_cpp_version);
    std::vector<std::string> sourceFiles = CLucid::Utils::split( FLAGS_sources, ';');
    parser.addSourceFiles(sourceFiles);
    std::vector<std::string> includeDirectories = CLucid::Utils::split( FLAGS_includes, ';');
    parser.addIncludePaths(includeDirectories);

    // 解析c++结构
    CLucid::CLucidAst metaData;
    parser.parse(metaData);

    // 打印被注解的类、函数、属性信息
    metaData.printDetails();

    std::vector<std::string> cmds = CLucid::Utils::split( FLAGS_cmd, ';');
    for(const std::string& cmd : cmds){
        GeneratorBase* generator = nullptr;
        if(cmd == "serialize"){
            generator = new SerializeGenerator();
        }else if(cmd == "reflect"){

        }else{
            std::cout << "cmd: [" << cmd << "] is not supported" << std::endl;
        }

        if (generator){
            // 设置元数据
            generator->setAst(metaData);
            // 设置模板根目录
            generator->setTemplateRootPath(FLAGS_template_path.c_str());
            // 设置输出文件根目录
            generator->setOutputRootPath(FLAGS_output_path.c_str());
            // 生成
            generator->gen();
            // 销毁
            delete generator;
        }

    }

    visitor_reflect_for_AActor();
    visitor_reflect_for_AActor2();
    return 0;
}