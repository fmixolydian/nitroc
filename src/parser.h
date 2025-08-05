#include <list>
#include <any>
#include <map>
#include <iostream>

#include "value.h"
#include "errors.h"
#include "lexer.h"

#pragma once

class ParsedWord {
    public:
        Value value;

        enum Type {
            NONE,

            CONSTANT,
            LITERAL,

            CODE_BLOCK,
            EXPR_BLOCK
        };

        Type type;

        ParsedWord(Type type, Value value) {
            this->type  = type;
            this->value = value;
        }

        ParsedWord(Type type) {
            this->type = type;
        }

        ParsedWord() {
            this->type = Type::NONE;
        }
        
        std::string dump();
};

class ParsedCommand {
    public:
        ParsedWord subject;
        std::list<ParsedWord> args;

        ParsedCommand(ParsedWord subject, std::list<ParsedWord> args) {
            this->subject = subject;
            this->args    = args;
        }

        std::string dump();
};

class ParsedBlock {
    private:
    public:
        std::list<ParsedCommand> commands;

        ParsedBlock() {}

        ParsedWord parse_block(LexedBlock block, std::list<LexedToken>::iterator it, LexedToken::Type from, LexedToken::Type until, ParsedWord::Type into) {
            int nest_level = 0;

            std::list<LexedToken> tokens;

            while (it != block.tokens.end()) {
                it++;
                switch (it->type) {
                    case LexedToken::Type::CODE_START:
                    case LexedToken::Type::EXPR_START:
                        nest_level++;
                        break;
                    
                    case LexedToken::Type::CODE_END:
                    case LexedToken::Type::EXPR_END:
                        nest_level--;
                        if (nest_level == -1) {
                            if (it->type == until) return ParsedWord(into, TValue<ParsedBlock>(Value::Type::CODEBLOCK, ParsedBlock(LexedBlock(tokens))));
                        }
                        break;
                }

                tokens.push_back(*it);
            }
        }

        ParsedBlock(LexedBlock L) {
            std::list<ParsedWord> command;
            ParsedWord subject;

            for (auto it = L.tokens.begin(); it != L.tokens.end(); ++it) {
                switch (it->type) {
                    case LexedToken::Type::CONSTANT:
                        command.push_back(ParsedWord(ParsedWord::Type::CONSTANT, it->value));
                        break;

                    case LexedToken::Type::LITERAL:
                        command.push_back(ParsedWord(ParsedWord::Type::LITERAL, it->value));
                        break;
                    
                    case LexedToken::Type::CODE_START:
                        command.push_back(parse_block(L, it,
                                                      LexedToken::Type::CODE_START,
                                                      LexedToken::Type::CODE_END,
                                                      ParsedWord::Type::CODE_BLOCK));
                        break;

                    case LexedToken::Type::EXPR_START:
                        command.push_back(parse_block(L, it,
                                                      LexedToken::Type::EXPR_START,
                                                      LexedToken::Type::EXPR_END,
                                                      ParsedWord::Type::EXPR_BLOCK));
                        break;
                    
                    case LexedToken::Type::COMMAND_SEP:
                        if (!command.empty()) {
                            subject = command.front();
                            command.pop_front();
                            commands.push_back(ParsedCommand(subject, command));
                            command.clear();
                        }
                        break;
                }
            }
        }

        std::string dump();
};

std::string ParsedWord::dump() {
    switch (type) {
        case Type::NONE:
			return "[code None]";

        case Type::CONSTANT:
			return "[code Constant ]";
        
        case Type::LITERAL:
            break;

        case Type::CODE_BLOCK:
            break;

        case Type::EXPR_START:
            break;
    }
}

std::string ParsedCommand::dump() {
    std::string words;

    for (auto arg : args) {
        words.append(arg.dump() + " ");
    }

    return "[code Command " + subject.dump() + " " + words + "]";
}

std::string ParsedBlock::dump() {
    std::string args;

    for (auto command : commands) {
        args.append(command.dump() + " ");
    }
    
    return "[code Block " + args + "]";
}