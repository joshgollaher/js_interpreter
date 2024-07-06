#ifndef AST_H
#define AST_H
#include <string>

namespace JS
{
    namespace ASTNode
    {
        class Node
        {
        public:
            virtual ~Node() = default;
            virtual std::string to_string() = 0;
        protected:

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

        };

        class BinaryExpression: public Expression
        {

        };

        class Literal : public Expression
        {

        };

        class Declaration: public Node
        {

        };

        class FunctionDeclaration: public Declaration
        {

        };

        class VariableDeclaration: public Declaration
        {

        };

    }

    class AST
    {

    };
}

#endif //AST_H
