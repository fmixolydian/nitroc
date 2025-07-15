#include <list>
#include <any>
#include <map>
#include <iostream>

#include "value.h"

#pragma once

class LexedToken {
private:
public:
    enum class Type {
        CONSTANT,
        LITERAL,

        COMMAND_SEP,
        CODE_START,
        CODE_END,
        EXPR_START,
        EXPR_END
    };

    Type type;
    Value data;

    LexedToken(Type type) {
        this->type = type;
    }

    LexedToken(Type type, Value data) {
        this->data = data;
        this->type = type;
    }
};

class LexedBlock {
private:
    enum class Mode {
        NORMAL,
        STRING_S,
        STRING_D,
        COMMENT
    };

public:
    std::list<LexedToken> tokens;

    LexedBlock(std::string code) {
        tokens.clear();

        Mode mode = Mode::NORMAL;
        
        std::string buffer;
        
        int line_no = 1;
        int line_ch = 0;

        for (auto iter = code.begin();
             iter != code.end();
             ++iter) {
            
            if (*iter == '\n') {
                line_no++;
                line_ch = 0;
            }

            line_ch++;

            if (mode == Mode::NORMAL) {
                if (std::string("\t\r\n\f #,;:()[]{}\"'").find(*iter) != std::string::npos) {
                    // modes
                    switch (*iter) {
                        case '"':
                            // string (double-quoted) start
                            mode = Mode::STRING_D;
                            break;
                        case '\'':
                            // string (single-quoted) start
                            mode = Mode::STRING_S;
                            break;
                        case '#':
                            // comment start
                            mode = Mode::COMMENT;
                            break;
                    }
                        
                    if (!buffer.empty()) {
                        // general tokens
                        char *p_int;
                        char *p_float;

                        // try convertsing string to number
                        long i = strtol(buffer.c_str(), &p_int,   0);
                        long f = strtof(buffer.c_str(), &p_float);

                        if (!*p_int)         tokens.push_back(LexedToken(LexedToken::Type::CONSTANT, TValue<int>(   i )));
                        else if (!*p_float)  tokens.push_back(LexedToken(LexedToken::Type::CONSTANT, TValue<float>( f )));
                        else {
                            if (buffer == "true")  tokens.push_back(LexedToken(LexedToken::Type::CONSTANT, TValue<bool>(true)));
                            if (buffer == "false") tokens.push_back(LexedToken(LexedToken::Type::CONSTANT, TValue<bool>(false)));
                            else {
                                tokens.push_back(LexedToken(LexedToken::Type::LITERAL, TValue<nitro_str>(buffer)));
                            }
                        }

                        buffer.clear();
                    }

                    switch (*iter) {
                        case ';': tokens.push_back(LexedToken(LexedToken::Type::COMMAND_SEP)); break;
                        case '[': tokens.push_back(LexedToken(LexedToken::Type::EXPR_START)); break;
                        case ']': tokens.push_back(LexedToken(LexedToken::Type::EXPR_END)); break;
                        case '{': tokens.push_back(LexedToken(LexedToken::Type::CODE_START)); break;
                        case '}': tokens.push_back(LexedToken(LexedToken::Type::CODE_END)); break;
                    }
                } else {
                    buffer += *iter;
                }
            } else {
                switch (mode) {
                    case Mode::STRING_D:
                        if (*iter == '"') {
                            mode = Mode::NORMAL;
                            tokens.push_back(LexedToken(LexedToken::Type::CONSTANT, TValue<nitro_str>(buffer)));
                            buffer.clear();
                        } else {
                            buffer += *iter;
                        }

                        break;

                    case Mode::STRING_S:
                        if (*iter == '\'') {
                            mode = Mode::NORMAL;
                            tokens.push_back(LexedToken(LexedToken::Type::CONSTANT, TValue<nitro_str>(buffer)));
                            buffer.clear();
                        } else {
                            buffer += *iter;
                        }

                        break;
                    
                    case Mode::COMMENT:
                        if (*iter == '\n') mode = Mode::NORMAL;
                        break;
                }
            }
        }
    }
};