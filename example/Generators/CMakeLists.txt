cmake_minimum_required(VERSION 3.31)
project(Generators)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (MSVC)
    # 字符集
    add_compile_options(/utf-8)
    # 生成pdb
    add_compile_options(/Zi)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    # 字符集
    add_compile_options(-finput-charset=UTF-8 -fexec-charset=UTF-8 -g)
    # 生成pdb
    #    add_compile_options(-g)
    #    set(CMAKE_BUILD_TYPE Debug)
elseif (MINGW)
    # 字符集
    add_compile_options(-finput-charset=UTF-8 -fexec-charset=UTF-8 -g)
    # 生成pdb
    #    add_compile_options(-g)
    #    set(CMAKE_BUILD_TYPE Debug)
endif()

# 判断平台
if (WIN32)
    set(PLATFORM "win")
    set(LIB_SUFFIX ".dll")
elseif (UNIX)
    set(PLATFORM "linux")
    set(LIB_SUFFIX ".so")
endif ()

include(FetchContent)

# 下载并编译 gflags
FetchContent_Declare(
        gflags
        GIT_REPOSITORY https://github.com/gflags/gflags.git
        GIT_TAG v2.2.2
)

FetchContent_MakeAvailable(gflags)
# 源文件
aux_source_directory(source DIR_SRC)
aux_source_directory(source/generators DIR_SRC)
aux_source_directory(source/test DIR_SRC)

## 调试：直接引用Clucid源码
#aux_source_directory(../../source DIR_SRC)
#aux_source_directory(../../source/structure DIR_SRC)
#aux_source_directory(../../source/support DIR_SRC)
#aux_source_directory(../../source/matcher DIR_SRC)
## 调试：直接引用Clucid源码

add_executable(${PROJECT_NAME} ${DIR_SRC})

# 头文件
target_include_directories(${PROJECT_NAME} PRIVATE include)
target_include_directories(${PROJECT_NAME} PRIVATE gen)

# 第三方库inja
target_include_directories(${PROJECT_NAME} PRIVATE third_party/inja)


# 从自己的third_party获取CLucid
target_include_directories(${PROJECT_NAME} PRIVATE third_party/CLucid/include)
find_library(CLUCID_LIBRARY NAMES CLucid libCLucid PATHS third_party/CLucid/lib/${PLATFORM})
find_library(CLANG_LIBRARY NAMES clang libclang PATHS third_party/CLucid/lib/${PLATFORM})

## 调试：直接引用Clucid源码
#target_include_directories(${PROJECT_NAME} PRIVATE ../../include)
#target_include_directories(${PROJECT_NAME} PRIVATE ../../third_party/clang/include)
#find_library(CLANG_LIBRARY NAMES clang libclang PATHS ../../third_party/clang/lib/${PLATFORM})
## 调试：直接引用Clucid源码

target_link_libraries(${PROJECT_NAME}
        gflags::gflags
        ${CLUCID_LIBRARY}
        ${CLANG_LIBRARY}
)


# 拷贝libclang库到编译目录
add_custom_command(
        TARGET ${PROJECT_NAME}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/third_party/CLucid/lib/${PLATFORM}/libclang${LIB_SUFFIX} ${CMAKE_BINARY_DIR}
        COMMENT "Copying CLucid to binary directory finish!"
)


set(DEST_PROJECT_GENERATOR_DIR ${CMAKE_SOURCE_DIR}/../TestProject/Generators)
# 拷贝libclang库到编译目录
add_custom_command(
        TARGET ${PROJECT_NAME}
        POST_BUILD

        COMMAND ${CMAKE_COMMAND} -E make_directory ${DEST_PROJECT_GENERATOR_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PROJECT_NAME}> ${DEST_PROJECT_GENERATOR_DIR}>/$<TARGET_FILE_NAME:${PROJECT_NAME}>
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/third_party/CLucid/lib/${PLATFORM}/libclang${LIB_SUFFIX} ${DEST_PROJECT_GENERATOR_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/templates ${DEST_PROJECT_GENERATOR_DIR}/templates
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/cmake ${DEST_PROJECT_GENERATOR_DIR}/cmake

        COMMENT "Copying executable to TestProject directory finish!"
)