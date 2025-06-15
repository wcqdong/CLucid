//
// Created by weichuanqi on 2025/5/20.
//

#ifndef CLUCID_LOG_H
#define CLUCID_LOG_H


#include <string>

namespace CLucid{
    enum class ELogLevel{
        TRACE,
        DEBUG,
        INFO,
        WARNING,
        ERROR,
        IGNORE,
    };

    class Log {
    private:
        std::string prefix;
        ELogLevel level;
    public:
        explicit Log(const char* prefix, ELogLevel level = ELogLevel::INFO)
            : prefix(prefix)
            , level(level)
        {
//            this->prefix = "[" + std::string(prefix) + "]";
        };

        void setLevel(ELogLevel inLevel){
            level = inLevel;
        };
        ELogLevel getLevel(){
            return level;
        };

        void trace(const char* format, ...);
        void debug(const char* format, ...);
        void info(const char* format, ...);
        void error(const char* format, ...);
        void warning(const char* format, ...);
    };

    extern Log GLog;
}


#endif //CLUCID_LOG_H
