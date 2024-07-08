#include "Parser.h"

#include "AST.h"

namespace JS
{
    AST Parser::parse()
    {
        m_index = 0;
        auto program = std::make_shared<AST::Program>();
        auto global_scope = std::make_shared<Scope>();

        for(const auto& node : parse_block())
        {
            program->add_statement(node);
        }

        return {program, global_scope};
    }

    std::vector<std::shared_ptr<AST::Node>> Parser::parse_block(const std::vector<TokenType>& stoppers) const
    {
        std::vector<std::shared_ptr<AST::Node>> statements;
        while(!match(stoppers) && peek().type != TokenType::END_OF_FILE)
        {
            // Detect token type and dispatch
            if(match({TokenType::VAR}) || match({TokenType::LET}))  // TODO: Separate these
            {
                statements.push_back(parse_variable_declaration());
            } else if(match({TokenType::FUNCTION}))
            {
                statements.push_back(parse_function());
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

    std::shared_ptr<AST::Node> Parser::parse_expression() const
    {
        not_implemented();
    }
    std::shared_ptr<AST::Node> Parser::parse_function_call() const
    {
        not_implemented();
    }
    std::shared_ptr<AST::Node> Parser::parse_binary_expression() const
    {
        not_implemented();
    }
    std::shared_ptr<AST::Node> Parser::parse_literal() const
    {
        not_implemented();
    }
    std::shared_ptr<AST::Node> Parser::parse_variable_assignment() const
    {
        not_implemented();
    }
    std::shared_ptr<AST::Node> Parser::parse_block() const
    {
        not_implemented();
    }
    std::shared_ptr<AST::Node> Parser::parse_function() const
    {
        not_implemented();
    }
    std::shared_ptr<AST::Node> Parser::parse_variable_declaration() const
    {
        not_implemented();
    }
    std::shared_ptr<AST::Node> Parser::parse_if_statement() const
    {
        not_implemented();
    }
    std::shared_ptr<AST::Node> Parser::parse_while_statement() const
    {
        not_implemented();
    }
    std::shared_ptr<AST::Node> Parser::parse_for_statement() const
    {
        not_implemented();
    }
    std::shared_ptr<AST::Node> Parser::parse_return_statement() const
    {
        not_implemented();
    }
}
