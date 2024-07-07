#ifndef AST_H
#define AST_H
#include <memory>
#include <string>

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

        };

        class FunctionCall : public Expression
        {

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
