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

        [[nodiscard]] Token peek(size_t off) const
        {
            return m_tokens[m_index + off];
        }

        [[nodiscard]] bool match(const std::vector<TokenType>& types) const
        {
            size_t i = 0;
            for(const auto& type : types)
            {
                if(peek(i).type != type)
                {
                    return false;
                }
                ++i;
            }

            return true;
        }

        [[nodiscard]] std::vector<std::shared_ptr<AST::Node>> parse_block(const std::vector<TokenType>& stoppers) const;
        [[nodiscard]] std::shared_ptr<AST::Node> parse_expression() const;
        [[nodiscard]] std::shared_ptr<AST::Node> parse_function_call() const;
        [[nodiscard]] std::shared_ptr<AST::Node> parse_binary_expression() const;
        [[nodiscard]] std::shared_ptr<AST::Node> parse_literal() const;
        [[nodiscard]] std::shared_ptr<AST::Node> parse_variable_assignment() const;
        [[nodiscard]] std::shared_ptr<AST::Node> parse_block() const;
        [[nodiscard]] std::shared_ptr<AST::Node> parse_function() const;
        [[nodiscard]] std::shared_ptr<AST::Node> parse_variable_declaration() const;
        [[nodiscard]] std::shared_ptr<AST::Node> parse_if_statement() const;
        [[nodiscard]] std::shared_ptr<AST::Node> parse_while_statement() const;
        [[nodiscard]] std::shared_ptr<AST::Node> parse_for_statement() const;
        [[nodiscard]] std::shared_ptr<AST::Node> parse_return_statement() const;
    };
}


#endif //PARSER_H
