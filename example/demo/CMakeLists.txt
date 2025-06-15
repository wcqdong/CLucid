cmake_minimum_required(VERSION 3.31)
project(demo)

set(CMAKE_CXX_STANDARD 17)

if (MSVC)
    add_compile_options(/utf-8)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_compile_options(-finput-charset=UTF-8 -fexec-charset=UTF-8)
elseif (MINGW)
    add_compile_options(-finput-charset=UTF-8 -fexec-charset=UTF-8)
endif()

aux_source_directory(. DIR_SRC)

# 判断平台
if (WIN32)
    set(PLATFORM "win")
    set(LIB_SUFFIX ".dll")
elseif (UNIX)
    set(PLATFORM "linux")
    set(LIB_SUFFIX ".so")
endif ()

# 引用CLucid和CLucid依赖的库
include_directories(third_party/CLucid/include)
include_directories(third_party/clang/include)


find_library(CLUCID_LIBRARY NAMES CLucid libCLucid PATHS third_party/CLucid/lib/${PLATFORM})
find_library(CLANG_LIBRARY NAMES clang libclang PATHS third_party/CLucid/lib/${PLATFORM})

# 打印查找到的库路径
message(STATUS "CLUCID_LIBRARY: ${CLUCID_LIBRARY}")
message(STATUS "CLANG_LIBRARY: ${CLANG_LIBRARY}")

add_executable(${PROJECT_NAME} ${DIR_SRC})


target_link_libraries(${PROJECT_NAME} ${CLUCID_LIBRARY} ${CLANG_LIBRARY})

# 拷贝libclang库到编译目录
add_custom_command(
        TARGET ${PROJECT_NAME}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/third_party/CLucid/lib/${PLATFORM}/libclang${LIB_SUFFIX} ${CMAKE_BINARY_DIR}
        COMMENT "Copying CLucid to binary directory finish!"
)