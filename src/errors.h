#include <list>
#include <any>
#include <map>
#include <iostream>
#include "value.h"

#pragma once

class SyntaxError : public std::exception {
private:
    std::string reason;
    int line_no;
    int line_ch;

public:
    SyntaxError(std::string reason, int line_no, int line_ch) {
        this->reason = reason;
        this->line_no = line_no;
        this->line_ch = line_ch;
    }

    const char* what() const noexcept {
        return ("Syntax error at " +
                std::to_string(line_no) + "," +
                std::to_string(line_ch) + " - " +
                reason).c_str();
    }
};
