#include "nitroc.h"

int main() {
    std::string code = "\
    set input [read];            \
    echo [input];                \
    while [input] {echo [input]} \
    ";
    LexedBlock block = LexedBlock(code); 
}