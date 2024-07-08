#ifndef AST_H
#define AST_H
#include <format>
#include <memory>
#include <memory>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "errors.h"
#include "Scope.h"
#include "Value.h"

#include "magic_enum/magic_enum.hpp"

namespace JS
{
    class AST
    {
    public:

        class Node
        {
        public:
            virtual ~Node() = default;
            virtual std::string to_string() = 0;
        };

        class Parameter final : public Node
        {
        public:
            explicit Parameter(const std::string& name) : m_name(name) {}

            std::string to_string() override
            {
                return std::format("Parameter [name={}]", m_name);
            }

        private:
            std::string m_name;
        };

        class Program final : public Node
        {
        public:
            void add_statement(const std::shared_ptr<Node>& statement)
            {
                m_statements.push_back(statement);
            }

            std::string to_string() override
            {
                std::ostringstream str;
                for(const auto& statement : m_statements)
                {
                    str << statement->to_string();
                }

                return std::format("Program [statements={}]", str.str());
            }

        private:
            std::vector<std::shared_ptr<Node>> m_statements;
        };

        class Expression : public Node
        {
        public:
            [[nodiscard]] virtual std::shared_ptr<Value> evaluate(std::shared_ptr<Scope> scope) const = 0;
        };

        class FunctionCall final : public Expression
        {
        public:
            FunctionCall(std::string name, const std::vector<std::shared_ptr<Value>>& arguments) : m_name(std::move(name)),
                m_arguments(arguments)
            {
            } // NOLINT(*-pass-by-value)

            [[nodiscard]] std::shared_ptr<Value> evaluate(std::shared_ptr<Scope> scope) const override
            {
                not_implemented();
            }

            std::string to_string() override
            {
                std::ostringstream str;
                for(const auto& arg : m_arguments)
                {
                    str << arg->to_string() << ",";
                }

                return std::format("Function call [name={}, args={}]", m_name, str.str());
            }

        private:
            // TODO: switch to a flyweight string
            std::string m_name;
            std::vector<std::shared_ptr<Value>> m_arguments;
        };

        class BinaryExpression final : public Expression
        {
        public:
            enum class Op
            {
                PLUS,
                MINUS,
                MULT,
                DIV,
                MOD,
                AND,
                OR,
                XOR,
                NOT,
                EQUAL_EQUAL,
                EQUAL_EQUAL_EQUAL,
                NOT_EQUAL,
                NOT_EQUAL_EQUAL
            };

            BinaryExpression(std::shared_ptr<Expression> left, std::shared_ptr<Expression> right, Op op) :
                m_left(std::move(left)), m_right(std::move(right)), m_op(op) {}

            std::string to_string() override
            {
                return std::format("Binary Op [{} {} {}]", m_left->to_string(), magic_enum::enum_name(m_op), m_right->to_string());
            }

            [[nodiscard]] std::shared_ptr<Value> evaluate(std::shared_ptr<Scope> scope) const override
            {
                not_implemented();
            }

            [[nodiscard]] const Expression& left() const { return *m_left; }
            [[nodiscard]] const Expression& right() const { return *m_right; }
            [[nodiscard]] Op op() const { return m_op; }

        private:
            std::shared_ptr<Expression> m_left;
            std::shared_ptr<Expression> m_right;
            Op m_op;
        };

        class Literal final : public Expression
        {
        public:
            explicit Literal(const std::shared_ptr<Value>& value) : m_value(value)
            {
            }

            [[nodiscard]] std::shared_ptr<Value> evaluate(std::shared_ptr<Scope> scope) const override { return m_value; }

            std::string to_string() override
            {
                return std::format("Literal [{}]", m_value->to_string());
            }

        private:
            std::shared_ptr<Value> m_value;
        };

        class VariableAssignment final : public Expression
        {
        public:
            VariableAssignment(std::string name, const std::shared_ptr<Value>& value) : m_name(std::move(name)), m_value(value) {}

            [[nodiscard]] std::shared_ptr<Value> evaluate(std::shared_ptr<Scope> scope) const override
            {
                not_implemented();
            }

            std::string to_string() override
            {
                return std::format("VariableAssignment [{}={}]", m_name, m_value->to_string());
            }

        private:
            std::string m_name;
            std::shared_ptr<Value> m_value;
        };

        class Statement : public Node
        {
        public:
            virtual void execute(std::shared_ptr<Scope> scope) const = 0;
        };

        class BlockStatement final : public Statement
        {
        public:

            explicit BlockStatement(const std::vector<std::shared_ptr<Statement>>& statements)
            {
                m_statements = std::vector(statements);
            }

            void add_statement(const std::shared_ptr<Statement>& statement)
            {
                m_statements.push_back(statement);
            }

            void execute(std::shared_ptr<Scope> scope) const override
            {
                not_implemented();
            }

            std::string to_string() override
            {
                std::ostringstream str;
                for(const auto& statement : m_statements)
                {
                    str << statement->to_string() << "\n";
                }
                str << "]";

                return std::format("BlockStatement [{}", str.str());
            }

        private:
            std::vector<std::shared_ptr<Statement>> m_statements;
        };

        class FunctionDeclaration final : public Statement
        {
        public:

            FunctionDeclaration(std::string name, const std::vector<std::shared_ptr<Parameter>>& parameters, std::shared_ptr<BlockStatement> body) : m_name(std::move(name)), m_parameters(parameters), m_body(body) {}

            std::string to_string() override
            {
                return std::format("FunctionDeclaration[name={}, arg_count={}, body={}]", m_name, m_parameters.size(), m_body->to_string());
            }

            void execute(std::shared_ptr<Scope> scope) const override
            {
                not_implemented();
            }

        private:
            std::string m_name;
            std::vector<std::shared_ptr<Parameter>> m_parameters;
            std::shared_ptr<BlockStatement> m_body;
        };

        class VariableDeclaration final : public Statement
        {
        public:

            VariableDeclaration(std::string name, const std::shared_ptr<Value>& initial_value) : m_name(std::move(name)), m_initial_value(initial_value) {}

            std::string to_string() override;
            void execute(std::shared_ptr<Scope> scope) const override
            {
                not_implemented();
            }

        private:
            std::string m_name;
            std::shared_ptr<Value> m_initial_value;
        };

        class IfStatement final : public Statement
        {
        public:

            IfStatement(const std::shared_ptr<Expression>& condition, const std::shared_ptr<BlockStatement>& body) : m_condition(condition), m_body(body) {}

            std::string to_string() override
            {
                return std::format("IfStatement [condition={}, body={}]", m_condition->to_string(), m_body->to_string());
            }

            void execute(std::shared_ptr<Scope> scope) const override
            {
                not_implemented();
            }

        private:
            std::shared_ptr<Expression> m_condition;
            std::shared_ptr<BlockStatement> m_body;
        };

        class WhileStatement final : public Statement
        {
        public:

            WhileStatement(const std::shared_ptr<Expression>& condition, const std::shared_ptr<BlockStatement>& body) : m_condition(condition), m_body(body) {}

            std::string to_string() override
            {
                return std::format("WhileStatement [condition={}, body={}]", m_condition->to_string(), m_body->to_string());
            }

            void execute(std::shared_ptr<Scope> scope) const override
            {
                not_implemented();
            }

        private:
            std::shared_ptr<Expression> m_condition;
            std::shared_ptr<BlockStatement> m_body;
        };

        class ForStatement final : public Statement
        {
        public:

            ForStatement(const std::shared_ptr<Expression>& condition, const std::shared_ptr<BlockStatement>& body) : m_condition(condition), m_body(body) {}

            std::string to_string() override
            {
                return std::format("ForStatement [condition={}, body={}]", m_condition->to_string(), m_body->to_string());
            }

            void execute(std::shared_ptr<Scope> scope) const override
            {
                not_implemented();
            }

        private:
            std::shared_ptr<Expression> m_condition;
            std::shared_ptr<BlockStatement> m_body;
        };

        class ReturnStatement final : public Statement
        {
        public:
            std::string to_string() override
            {
                return "ReturnStatement";
            }
            void execute(std::shared_ptr<Scope> scope) const override
            {
                not_implemented();
            }
        };

        //TODO: missing switch statement, import statement, class declarations

        AST(const std::shared_ptr<Program>& program, const std::shared_ptr<Scope>& global_scope) : m_program(program), m_global_scope(global_scope) {}
        void execute();

        [[nodiscard]] std::shared_ptr<Program> program() const { return m_program; }

    private:
        std::shared_ptr<Program> m_program;
        std::shared_ptr<Scope> m_global_scope;
    };
}

#endif //AST_H
