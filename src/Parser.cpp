#include "Parser.h"

#include "AST.h"

namespace JS
{
    AST Parser::parse()
    {
        m_index = 0;
        auto program = std::make_shared<AST::Program>();
        auto global_scope = std::make_shared<Scope>();

        return {program, global_scope};
    }

    std::vector<std::shared_ptr<AST::Node>> parse_statement_list()
    {

    }

    std::shared_ptr<AST::Node> Parser::parse_expression() {
        not_implemented();
    }
    std::shared_ptr<AST::Node> Parser::parse_function_call() {
        not_implemented();
    }
    std::shared_ptr<AST::Node> Parser::parse_binary_expression() {
        not_implemented();
    }
    std::shared_ptr<AST::Node> Parser::parse_literal() {
        not_implemented();
    }
    std::shared_ptr<AST::Node> Parser::parse_variable_assignment() {
        not_implemented();
    }
    std::shared_ptr<AST::Node> Parser::parse_block() {
        not_implemented();
    }
    std::shared_ptr<AST::Node> Parser::parse_function() {
        not_implemented();
    }
    std::shared_ptr<AST::Node> Parser::parse_variable_declaration() {
        not_implemented();
    }
    std::shared_ptr<AST::Node> Parser::parse_if_statement() {
        not_implemented();
    }
    std::shared_ptr<AST::Node> Parser::parse_while_statement() {
        not_implemented();
    }
    std::shared_ptr<AST::Node> Parser::parse_for_statement() {
        not_implemented();
    }

}
