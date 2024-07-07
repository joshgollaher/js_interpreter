#ifndef AST_H
#define AST_H
#include <memory>
#include <string>
#include <vector>

#include "Value.h"

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

        class Scope : public Node
        {
        public:
        };

        class Program : public Scope
        {
        };

        class Block : public Scope
        {
        };

        class Expression : public Node
        {
        public:
            virtual std::shared_ptr<Value> value() = 0;
        };

        class FunctionCall : public Expression
        {
        public:

            FunctionCall(const std::string& name, const std::vector<Value>& arguments) : m_name(name), m_arguments(arguments) {} // NOLINT(*-pass-by-value)

            std::string to_string() override;

        private:
            // TODO: switch to a flyweight string
            std::string m_name;
            std::vector<Value> m_arguments;
        };

        class BinaryExpression : public Expression
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

            BinaryExpression(std::unique_ptr<Expression> left, std::unique_ptr<Expression> right, Op op) : m_left(std::move(left)), m_right(std::move(right)), m_op(op) {}

            [[nodiscard]] const Expression& left() const { return *m_left; }
            [[nodiscard]] const Expression& right() const { return *m_right; }
            [[nodiscard]] const Op op() const { return m_op; }

        private:
            std::unique_ptr<Expression> m_left;
            std::unique_ptr<Expression> m_right;
            Op m_op;
        };

        class Literal : public Expression
        {
        };

        class Declaration : public Node
        {
        };

        class FunctionDeclaration : public Declaration
        {
        };

        class VariableDeclaration : public Declaration
        {
        };

        class VariableAssignment : public Expression
        {
        };
    };
}

#endif //AST_H
