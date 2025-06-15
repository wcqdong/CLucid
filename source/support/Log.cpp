//
// Created by weichuanqi on 2025/5/20.
//

#include "support/Log.h"
#include "support/Utils.h"
#include <cstdarg>
#include <cstdio>

using namespace CLucid;

void Log::trace(const char* format, ...) {
    if (level > ELogLevel::TRACE)
        return;
    std::string str = Utils::format("[trace] {0}{1}", prefix, format);
    const char* strChar = str.c_str();
    va_list args;
    va_start(args, format);
    vfprintf(stdout, strChar, args);
    va_end(args);
    fprintf(stdout, "\n");
}

void Log::debug(const char* format, ...) {
    if (level > ELogLevel::DEBUG)
        return;
    std::string str = Utils::format("[debug] {0}{1}", prefix, format);
    const char* strChar = str.c_str();
    va_list args;
    va_start(args, format);
    vfprintf(stdout, strChar, args);
    va_end(args);
    fprintf(stdout, "\n");
}

void Log::info(const char* format, ...) {
    if (level > ELogLevel::INFO)
        return;
    std::string str = Utils::format("[info] {0}{1}", prefix, format);
    const char* strChar = str.c_str();
    va_list args;
    va_start(args, format);
    vfprintf(stdout, strChar, args);
    va_end(args);
    fprintf(stdout, "\n");
}

void Log::warning(const char* format, ...) {
    if (level > ELogLevel::WARNING)
        return;
    std::string str = Utils::format("[warn] {0}{1}", prefix, format);
    const char* strChar = str.c_str();
    va_list args;
    va_start(args, format);
    vfprintf(stderr, strChar, args);
    va_end(args);
    fprintf(stdout, "\n");
}

void Log::error(const char* format, ...) {
    if (level < ELogLevel::ERROR)
        return;
    std::string str = Utils::format("[error] {0}{1}", prefix, format);
    const char* strChar = str.c_str();
    va_list args;
    va_start(args, format);
    vfprintf(stderr, strChar, args);
    va_end(args);
    fprintf(stdout, "\n");
}



Log CLucid::GLog("");


//Log* Glog{};
