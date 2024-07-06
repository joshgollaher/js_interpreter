#ifndef PARSER_H
#define PARSER_H
#include "AST.h"
#include "Lexer.h"

namespace JS {

    class Parser {
    public:

        explicit Parser(const std::vector<Token>& tokens) : m_tokens(tokens) {}

        AST parse();

    private:
        std::vector<Token> m_tokens;

    };
}


#endif //PARSER_H
