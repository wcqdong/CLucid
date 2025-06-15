cmake_minimum_required(VERSION 3.31)
project(TestProject)

set(CMAKE_CXX_STANDARD 17)

include_directories(./)
include_directories(include)
include_directories(gen)


aux_source_directory(. DIR_SRC)
aux_source_directory(source DIR_SRC)

add_executable(${PROJECT_NAME} ${DIR_SRC})


# 引用生成器脚本
include(${CMAKE_SOURCE_DIR}/Generators/cmake/Generators.cmake)

# 创建生成器目标
create_generator_target(
        SOURCES "${DIR_SRC}"
        CMD "serialize" # 执行序列化生成工具
#        OUTPUT_PATH gen
#        GENERATOR "${CMAKE_SOURCE_DIR}/Generators/Generators.exe"
)
