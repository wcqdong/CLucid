cmake_minimum_required(VERSION 3.31)
project(CLucid)

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
elseif (UNIX)
    set(PLATFORM "linux")
endif ()


aux_source_directory(source DIR_SRC)
aux_source_directory(source/matcher DIR_SRC)
aux_source_directory(source/structure DIR_SRC)
aux_source_directory(source/support DIR_SRC)

# 查找本地 Clang 库
#find_path(CLANG_INCLUDE_DIR clang-c/Index.h "C:/Program Files/LLVM/include")
#find_library(CLANG_LIBRARY NAMES clang libclang HINTS "C:/Program Files/LLVM/lib")
# 如果找不到 Clang 库，手动设置路径
#if (NOT CLANG_INCLUDE_DIR)
#    message(FATAL_ERROR "Could not find Clang include directory")
#else ()
#    message("find Clang include directory")
#endif()
# 包含 Clang 头文件
#include_directories(${CLANG_INCLUDE_DIR})

add_library(${PROJECT_NAME} STATIC ${DIR_SRC})
#add_executable(${PROJECT_NAME} ${DIR_SRC})

target_include_directories(${PROJECT_NAME} PRIVATE include)

# 使用工程中的clang
target_include_directories(${PROJECT_NAME} PRIVATE third_party/clang/include)
find_library(CLANG_LIBRARY NAMES clang libclang PATHS third_party/clang/lib/${PLATFORM} REQUIRED)
if (NOT CLANG_LIBRARY)
    message(FATAL_ERROR "Could not find Clang library")
endif()

message(STATUS "All source files: ${DIR_SRC}")

# 链接 Clang 库
target_link_libraries(${PROJECT_NAME} PRIVATE ${CLANG_LIBRARY})

# 生成pdb
#target_link_options(${PROJECT_NAME} PRIVATE /DEBUG /PDB:${CMAKE_BINARY_DIR}/${PROJECT_NAME}.pdb)

#add_custom_command(
#        TARGET CLucid
#        POST_BUILD
#
#        # msvc好用
#        COMMAND ${CMAKE_COMMAND} -E copy ${CLANG_LIBRARY} ${CMAKE_BINARY_DIR}/intermediate/clang_temp.lib
#        COMMAND lib.exe /OUT:${CMAKE_BINARY_DIR}/$<TARGET_FILE_NAME:CLucid> ${CMAKE_BINARY_DIR}/$<TARGET_FILE_NAME:CLucid> ${CMAKE_BINARY_DIR}/intermediate/clang_temp.lib
#        COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/intermediate
#        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
#        COMMENT "Including clang.lib generators files into CLucid.lib"
#)


# 设置使用CLude库的工程的第三方库目录
#set(DEST_PROJECT_THIRD_DIR ${CMAKE_SOURCE_DIR}/example/demo/third_party)
set(DEST_PROJECT_THIRD_DIR ${CMAKE_SOURCE_DIR}/example/Generators/third_party)
# 把编译好的库拷贝到目标工程
if(DEFINED DEST_PROJECT_THIRD_DIR)
    add_custom_command(
            TARGET ${PROJECT_NAME}
            POST_BUILD

            # 先清理
            COMMAND ${CMAKE_COMMAND} -E remove_directory ${DEST_PROJECT_THIRD_DIR}/${PROJECT_NAME}/include
            COMMAND ${CMAKE_COMMAND} -E remove_directory ${DEST_PROJECT_THIRD_DIR}/${PROJECT_NAME}/lib/${PLATFORM}

            # 拷贝CLucid的头文件
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/include/ ${DEST_PROJECT_THIRD_DIR}/${PROJECT_NAME}/include
            # 拷贝clang库的头文件
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/third_party/clang/include ${DEST_PROJECT_THIRD_DIR}/${PROJECT_NAME}/include
#            # 拷贝clang库的lib
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/third_party/clang/lib ${DEST_PROJECT_THIRD_DIR}/${PROJECT_NAME}/lib
            # 拷贝CLucide库
            COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PROJECT_NAME}> ${DEST_PROJECT_THIRD_DIR}/${PROJECT_NAME}/lib/${PLATFORM}/$<TARGET_FILE_NAME:${PROJECT_NAME}>

            COMMENT "Copying CLucid to ${DEST_PROJECT_THIRD_DIR} finish!"
    )
endif ()
