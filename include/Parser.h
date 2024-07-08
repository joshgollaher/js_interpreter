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
        size_t m_index{0};

        Token consume()
        {
            return m_tokens[m_index++];
        }

        Token consume(const TokenType expected_type)
        {
            const auto token = m_tokens[m_index++];
            if(token.type != expected_type)
            {
                throw std::runtime_error("Consumed invalid token type");
            }

            return token;
        }

        [[nodiscard]] Token peek() const
        {
            return m_tokens[m_index];
        }

        [[nodiscard]] bool match(const std::vector<TokenType>& types) const
        {

        }

        std::shared_ptr<AST::Node> parse_program();
        std::shared_ptr<AST::Node> parse_expression();
        std::shared_ptr<AST::Node> parse_function_call();
        std::shared_ptr<AST::Node> parse_binary_expression();
        std::shared_ptr<AST::Node> parse_literal();
        std::shared_ptr<AST::Node> parse_variable_assignment();
        std::shared_ptr<AST::Node> parse_block();
        std::shared_ptr<AST::Node> parse_function();
        std::shared_ptr<AST::Node> parse_variable_declaration();
        std::shared_ptr<AST::Node> parse_if_statement();
        std::shared_ptr<AST::Node> parse_while_statement();
        std::shared_ptr<AST::Node> parse_for_statement();
    };
}


#endif //PARSER_H
