#include "nitroc.h"

int main() {
    std::string code = "\
    one two, \n\
    three four \n\
    five six, seven eight \n\
    ";
    LexedBlock L = LexedBlock(code);

    for (auto token : L.tokens) {
        std::cout << LexedToken::TypeToString(token.type) << "\n";
    }

    ParsedBlock P = ParsedBlock(code);
}