#include <list>
#include <any>
#include <map>
#include <iostream>

#pragma once

class Value {
public:
    enum class Type {
        NIL,
        INTEGER,
        FLOAT,
        STRING,
        CODEBLOCK,
        LIST,
        OBJECT
    };

    Type type = Type::NIL;
};

template <typename T>
class TValue : public Value {
public:
    T value;

    TValue (T val) {
        value = val;
    }
};

typedef std::string                  nitro_str;
typedef std::list<Value>             nitro_list;
typedef std::map<std::string, Value> nitro_object;

