#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <list>
#include <algorithm>
#include <functional>
#include <sstream>
#include <tuple>
#include <regex>
#include <fstream>
#include <chrono>
#include <cmath>
#include <array>
#include <queue>
#include <thread>
#include <optional>
#include "String.h"
#include "Vector.h"
namespace agumi
{
    class String;
    enum class ValueType
    {
        string,
        number,
        null,
        boolean,
        object,
        array,
        function
        //  bigint,
        // symbol
    };
    static std::vector<std::string> type_emun2str{
        "string",
        "number",
        "null",
        "boolean",
        "object",
        "array",
        "function"};

    static std::map<std::string, ValueType> typestr_2enum {
        {"string", ValueType::string}, 
        {"number", ValueType::number}, 
        {"null", ValueType::null},
        { "boolean", ValueType::boolean },
        {"object", ValueType::object},
        {"array", ValueType::array},
        {"function", ValueType::function}
    };
    class Value;
    using ObjectMap = std::map<String, Value>;
    using ArrayVec = Vector<Value>;

#define THROW_MSG(msg, ...) throw std::logic_error(String::Format("{} \n\t发生在: {}:{} ", String::Format(msg, {__VA_ARGS__}), __FILE__, __LINE__));
#define P(msg, ...) std::cout<<String::Format("\033[32m{}:{}\033[0m ",  __FILE__, __LINE__)<<String::Format(msg, {__VA_ARGS__})<<std::endl;

}