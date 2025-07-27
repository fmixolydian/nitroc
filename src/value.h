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

	std::string to_string() {
		switch (type) {
			case Type::NIL:     return "(null)"; break;
			case Type::INTEGER: return std::to_string(TValue<int>); break;
		}
	}
};

typedef std::string                  nitro_str;
typedef std::list<Value>             nitro_list;
typedef std::map<std::string, Value> nitro_object;

template <typename T>
class TValue : public Value {
public:
    T value;

    TValue (Type type, T val) {
        value = val;
        this->type = type;
    }

    TValue (T val) {
        value = val;
    }

    operator T () {
        return value;
    }
};