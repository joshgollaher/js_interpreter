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

        void consume_semicolon_if_exists()
        {
            if(peek().type == TokenType::SEMICOLON)
            {
                consume();
            }
        }

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

        [[nodiscard]] Token peek(const size_t off) const
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

        [[nodiscard]] std::vector<std::shared_ptr<AST::Statement>> parse_block(const std::vector<TokenType>& stoppers);
        [[nodiscard]] std::vector<std::shared_ptr<AST::Parameter>> parse_parameters();
        [[nodiscard]] std::shared_ptr<AST::Expression> parse_expression();
        [[nodiscard]] std::shared_ptr<AST::FunctionCall> parse_function_call();
        [[nodiscard]] std::shared_ptr<AST::BinaryExpression> parse_binary_expression();
        [[nodiscard]] std::shared_ptr<AST::Literal> parse_literal();
        [[nodiscard]] std::shared_ptr<AST::VariableAssignment> parse_variable_assignment();
        [[nodiscard]] std::shared_ptr<AST::FunctionDeclaration> parse_function_declaration();
        [[nodiscard]] std::shared_ptr<AST::VariableDeclaration> parse_variable_declaration();
        [[nodiscard]] std::shared_ptr<AST::FunctionCallStatement> parse_function_call_statement();
        [[nodiscard]] std::shared_ptr<AST::IfStatement> parse_if_statement();
        [[nodiscard]] std::shared_ptr<AST::WhileStatement> parse_while_statement();
        [[nodiscard]] std::shared_ptr<AST::ForStatement> parse_for_statement();
        [[nodiscard]] std::shared_ptr<AST::ReturnStatement> parse_return_statement();
    };
}


#endif //PARSER_H
