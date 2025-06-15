//
// Created by weichuanqi on 2025/5/19.
//

#ifndef CLUCID_UTILS_H
#define CLUCID_UTILS_H

#include "support/PreProcessor.h"
#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <vector>
#include <filesystem>

namespace CLucid::Utils{
    const std::string path_separator = std::string(1, std::filesystem::path::preferred_separator);

    [[nodiscard]] FORCE_INLINE std::string pathNormalize(const char* str){
        std::filesystem::path path(str);
        return path.make_preferred().string();
    }
    FORCE_INLINE bool startsWith(std::string& str, const std::string& strStart){
        return str.compare(0, strStart.size(), strStart) == 0;
    }
    FORCE_INLINE bool removePrefix(const std::string& str, const std::string& prefix, std::string& outResult){
        if (str.find(prefix) == 0){
            outResult = str.substr(prefix.length());
            return true;
        }else{
            outResult = str;
            return false;
        }
    }

    FORCE_INLINE bool removeSuffix(const std::string& str, const std::string& suffix, std::string& outResult){
        if (str.rfind(suffix) == str.length() - suffix.length()){
            outResult = str.substr(0, str.length() - 1);
            return true;
        }else{
            outResult = str;
            return false;
        }
    }

    FORCE_INLINE bool endsWith(const std::string& str, const std::string& strEnd) {
        if (strEnd.size() > str.size()) {
            return false;
        }
        return str.compare(str.size() - strEnd.size(), strEnd.size(), strEnd) == 0;
    }
    FORCE_INLINE std::string trim(std::string& str){
        std::string result;
        for (char c : str) {
            if (c != ' ') {
                result += c;
            }
        }
        return result;
    }

    FORCE_INLINE std::vector<std::string> split(const std::string &str, char delimiter) {
        std::vector<std::string> tokens;
        std::string token;
        std::stringstream ss(str);
        while (std::getline(ss, token, delimiter)) {
            tokens.push_back(token);
        }
        return tokens;
    }

    template<typename T>
    FORCE_INLINE void appendVector(std::vector<T>& source, std::vector<T>& target){
        source.reserve(source.size() + target.size());
        source.insert(source.end(), target.begin(), target.end());
    }

    class ArgBase
    {
    public:
        ArgBase() = default;
        virtual ~ArgBase() = default;
        virtual void format(std::ostringstream &ss, const std::string& fmt) = 0;
    };

    template <class T>
    class Arg : public ArgBase
    {
    public:
        explicit Arg(T arg) : m_arg(arg) {}
        ~Arg() override = default;
        void format(std::ostringstream &ss, const std::string& fmt) override
        {
            ss << m_arg;
        }
    private:
        T m_arg;
    };

    class ArgArray : public std::vector < ArgBase* >
    {
    public:
        ArgArray() = default;
        ~ArgArray()
        {
            std::for_each(begin(), end(), [](ArgBase* p){ delete p; });
        }
    };

    static void formatItem(std::ostringstream& ss, const std::string& item, const ArgArray& args)
    {
        int index = 0;
        int alignment = 0;
        std::string fmt;

        char* endptr = nullptr;
        index = strtol(&item[0], &endptr, 10);
        if (index < 0 || index >= args.size())
        {
            return;
        }

        if (*endptr == ',')
        {
            alignment = strtol(endptr + 1, &endptr, 10);
            if (alignment > 0)
            {
                ss << std::right << std::setw(alignment);
            }
            else if (alignment < 0)
            {
                ss << std::left << std::setw(-alignment);
            }
        }

        if (*endptr == ':')
        {
            fmt = endptr + 1;
        }

        args[index]->format(ss, fmt);
    }

    template <class T>
    static void Transfer(ArgArray& argArray, T t)
    {
        argArray.push_back(new Arg<T>(t));
    }

    template <class T, typename... Args>
    static void Transfer(ArgArray& argArray, T t, Args&&... args)
    {
        Transfer(argArray, t);
        Transfer(argArray, args...);
    }

    template <typename... Args>
    std::string format(const std::string& format, Args&&... args)
    {
        if (sizeof...(args) == 0)
        {
            return format;
        }

        ArgArray argArray;
        Transfer(argArray, args...);
        size_t start = 0;
        size_t pos = 0;
        std::ostringstream ss;
        while (true)
        {
            pos = format.find('{', start);
            if (pos == std::string::npos)
            {
                ss << format.substr(start);
                break;
            }

            ss << format.substr(start, pos - start);
            if (format[pos + 1] == '{')
            {
                ss << '{';
                start = pos + 2;
                continue;
            }

            start = pos + 1;
            pos = format.find('}', start);
            if (pos == std::string::npos)
            {
                ss << format.substr(start - 1);
                break;
            }

            formatItem(ss, format.substr(start, pos - start), argArray);
            start = pos + 1;
        }

        return ss.str();
    }
}

#endif //CLUCID_UTILS_H
