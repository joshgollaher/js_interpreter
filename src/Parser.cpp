#include "Parser.h"

#include "AST.h"

namespace JS
{
    AST Parser::parse()
    {
        m_index = 0;
        auto program = std::make_shared<AST::Program>();
        auto global_scope = std::make_shared<Scope>();

        for(const auto& node : parse_block({TokenType::END_OF_FILE}))
        {
            program->add_statement(node);
        }

        return {program, global_scope};
    }

    std::vector<std::shared_ptr<AST::Statement>> Parser::parse_block(const std::vector<TokenType>& stoppers)
    {
        std::vector<std::shared_ptr<AST::Statement>> statements;
        while(!match(stoppers) && peek().type != TokenType::END_OF_FILE)
        {
            // Detect token type and dispatch
            if(match({TokenType::VAR}) || match({TokenType::LET}))  // TODO: Separate these
            {
                statements.push_back(parse_variable_declaration());
            } else if(match({TokenType::FUNCTION}))
            {
                statements.push_back(parse_function_declaration());
            } else if(match({TokenType::IF}))
            {
                statements.push_back(parse_if_statement());
            } else if(match({TokenType::RETURN}))
            {
                statements.push_back(parse_return_statement());
            }
        }

        return {};
    }

    std::shared_ptr<AST::Expression> Parser::parse_expression()
    {
        not_implemented();
    }
    std::shared_ptr<AST::FunctionCall> Parser::parse_function_call()
    {
        auto name = consume();
        assert(name.type == TokenType::IDENTIFIER);
        consume(TokenType::LEFT_PAREN);
        auto params = parse_parameters();
        consume(TokenType::RIGHT_PAREN);
        consume_semicolon_if_exists();
        return std::make_shared<AST::FunctionCall>(name.unwrap<std::string>(), params);
    }

    std::shared_ptr<AST::FunctionCallStatement> Parser::parse_function_call_statement()
    {
        auto function_call = parse_function_call();
        consume_semicolon_if_exists();
        return std::make_shared<AST::FunctionCallStatement>(function_call);
    }


    std::shared_ptr<AST::FunctionDeclaration> Parser::parse_function_declaration()
    {
        consume(TokenType::FUNCTION);
        const auto identifier = consume();
        assert(identifier.type == TokenType::IDENTIFIER);
        consume(TokenType::LEFT_PAREN);
        auto params = parse_parameters();
        consume(TokenType::RIGHT_PAREN);
        consume(TokenType::LEFT_CURLY_BRACE);
        auto body = parse_block({TokenType::RIGHT_CURLY_BRACE});
        consume(TokenType::RIGHT_CURLY_BRACE);

        return std::make_shared<AST::FunctionDeclaration>(identifier.unwrap<std::string>(), params, std::make_shared<AST::BlockStatement>(body));
    }
    std::shared_ptr<AST::BinaryExpression> Parser::parse_binary_expression()
    {
        not_implemented();
    }
    std::shared_ptr<AST::Literal> Parser::parse_literal()
    {
        not_implemented();
    }

    std::vector<std::shared_ptr<AST::Parameter>> Parser::parse_parameters()
    {
        not_implemented();
    }

    std::shared_ptr<AST::VariableAssignment> Parser::parse_variable_assignment()
    {
        not_implemented();
    }
    std::shared_ptr<AST::VariableDeclaration> Parser::parse_variable_declaration()
    {
        not_implemented();
    }
    std::shared_ptr<AST::IfStatement> Parser::parse_if_statement()
    {
        not_implemented();
    }
    std::shared_ptr<AST::WhileStatement> Parser::parse_while_statement()
    {
        not_implemented();
    }
    std::shared_ptr<AST::ForStatement> Parser::parse_for_statement()
    {
        not_implemented();
    }
    std::shared_ptr<AST::ReturnStatement> Parser::parse_return_statement()
    {
        not_implemented();
    }
}
