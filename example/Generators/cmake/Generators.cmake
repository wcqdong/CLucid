# Generators - 生成器执行脚本

# 创建生成器目标的函数

# CMD           【必选】 要执行的命令
# SOURCES       【必选】 源文件的列表
# INCLUDES      【可选】 头文件目录列表，默认会搜索所有include_directories和target_include_directories值
# TARGET_NAME   【可选】 cmake目标的名字，默认generator_target
# GENERATOR     【可选】 生成器的路径，默认为当前目录下的Generators.exe
# WORKING_DIR   【可选】 工作目录，默认为当前目录
# CPP_VERSION   【可选】 C++版本，默认17
# TEMPLATE_PATH 【可选】 模板路径，默认为当前目录下的templates
# OUTPUT_PATH   【可选】 生成目标文件路径，默认问引用该cmake的cmake文件所在目录下的gen文件夹
# PRO_NAME      【可选】 项目名，默认为使用${PROJECT_NAME}

function(create_generator_target)
    # 定义函数参数
    set(options "")
    set(multiValueArgs SOURCES INCLUDES)
    set(oneValueArgs CMD TARGET_NAME GENERATOR WORKING_DIR CPP_VERSION TEMPLATE_PATH OUTPUT_PATH PRO_NAME)

    # 所有参数加前缀
    cmake_parse_arguments(GEN "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if(NOT GEN_TARGET_NAME)
        set(GEN_TARGET_NAME generator_target)
    endif()

    if(NOT GEN_WORKING_DIR)
        message(STATUS "${CMAKE_SOURCE_DIR}")
        set(GEN_WORKING_DIR ${CMAKE_SOURCE_DIR})
    endif()

    if(NOT EXISTS ${GEN_GENERATOR})
        message(STATUS "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/../")
        set(GEN_GENERATOR "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/../Generators.exe")
        if(NOT EXISTS ${GEN_GENERATOR})
            message(FATAL_ERROR "Analyzer tool not found: ${GEN_GENERATOR}")
        endif ()
    endif()

    # 设置默认值
    if(NOT GEN_CPP_VERSION)
        set(GEN_CPP_VERSION 17)
    endif()

    if(NOT GEN_TEMPLATE_PATH)
        set(GEN_TEMPLATE_PATH ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/../templates)
    endif()

    if(NOT GEN_OUTPUT_PATH)
        set(GEN_OUTPUT_PATH ${CMAKE_CURRENT_LIST_DIR}/gen)
    endif()

    if (NOT GEN_PRO_NAME)
        set(GEN_PRO_NAME ${PROJECT_NAME})
    endif ()

    if (NOT GEN_INCLUDES)
        # 合并包含目录
        set(GEN_INCLUDES "")
        get_target_property(GEN_TARGET_INCLUDES ${GEN_PRO_NAME} INCLUDE_DIRECTORIES)
        get_directory_property(GEN_GLOBAL_INCLUDES INCLUDE_DIRECTORIES)

        if(GEN_TARGET_INCLUDES)
            list(APPEND GEN_INCLUDES ${GEN_TARGET_INCLUDES})
        endif()
        if(GEN_GLOBAL_INCLUDES)
            list(APPEND GEN_INCLUDES ${GEN_GLOBAL_INCLUDES})
        endif()
        list(REMOVE_DUPLICATES GEN_INCLUDES)
        # 不添加生成目录
        list(REMOVE_ITEM GEN_INCLUDES "${GEN_OUTPUT_PATH}")
    endif ()


    # 打印信息
    message(STATUS "Creating generator target: ${GEN_TARGET_NAME}")
    message(STATUS "  Cmd: ${GEN_CMD}")
    message(STATUS "  Generator: ${GEN_GENERATOR}")
    message(STATUS "  Sources: ${GEN_SOURCES}")
    message(STATUS "  Includes: ${GEN_INCLUDES}")
    message(STATUS "  CPP Version: ${GEN_CPP_VERSION}")
    message(STATUS "  Template Path: ${GEN_TEMPLATE_PATH}")
    message(STATUS "  Output Path: ${GEN_OUTPUT_PATH}")
    message(STATUS "  Working Directory: ${GEN_WORKING_DIR}")

    # 创建自定义目标
    add_custom_target(
            ${GEN_TARGET_NAME}
            COMMAND ${GEN_GENERATOR}
                --cmd ${GEN_CMD}
                --cpp_version ${GEN_CPP_VERSION}
                --sources "${GEN_SOURCES}"
                --includes "${GEN_INCLUDES}"
                --template_path ${GEN_TEMPLATE_PATH}
                --output_path ${GEN_OUTPUT_PATH}
            WORKING_DIRECTORY ${GEN_WORKING_DIR}
            COMMENT "Running ${GEN_GENERATOR_PATH}..."
            VERBATIM
    )
    add_dependencies(${GEN_PRO_NAME} ${GEN_TARGET_NAME})
endfunction()
