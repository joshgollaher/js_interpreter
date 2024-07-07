#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <utility>
#include <variant>
#include <span>

#include <assert.h>
#include <cstdint>
#include <sstream>
#include <unordered_map>
#include <vector>

#include "Log.h"
#include "Span.h"

namespace JS
{
    enum class TokenType: int
    {
        EXCLAMATION_MARK,
        QUESTION_MARK,

        PERIOD,
        COMMA,

        LEFT_SQUARE_BRACKET,
        RIGHT_SQUARE_BRACKET,

        LEFT_CURLY_BRACE,
        RIGHT_CURLY_BRACE,

        LEFT_PAREN,
        RIGHT_PAREN,

        LESS_THAN,
        GREATER_THAN,

        LESS_THAN_EQUAL_TO,
        GREATER_THAN_EQUAL_TO,

        MINUS,
        PLUS,
        MULT,
        DIV,
        BACK_SLASH,
        MOD,
        XOR,
        EQUALS,
        OR,
        AND,

        PLUS_EQUALS,
        MINUS_EQUALS,
        DIV_EQUALS,
        MULT_EQUALS,
        MOD_EQUALS,
        AND_EQUALS,
        XOR_EQUALS,
        OR_EQUALS,
        EQUAL_EQUAL,
        NOT_EQUAL,
        SHIFT_LEFT,
        SHIFT_RIGHT,
        INCREMENT,
        DECREMENT,

        EQUAL_EQUAL_EQUAL,
        NOT_EQUAL_EQUAL,

        ARROW,

        SEMICOLON,
        COLON,

        SINGLE_QUOTED_STRING,
        DOUBLE_QUOTED_STRING,

        IDENTIFIER,
        INTEGER,
        FLOAT,

        LET,
        CONST,
        VAR,
        FUNCTION,
        RETURN,
        FOR,
        WHILE,
        CONTINUE,
        BREAK,

        NEWLINE,
        END_OF_FILE,
        INVALID,
        WHITESPACE
    };

    using TokenData = std::variant<std::monostate, std::string, int32_t, double>;

    struct Token
    {
        TokenType type;
        TokenData data;
        Span span;

        // ReSharper disable once CppNonExplicitConvertingConstructor
        Token(const TokenType _type, TokenData _data, Span _span) : type(_type), data(std::move(_data)),
                                                                    span(std::move(_span))
        {
        }

        Token(const TokenType _type, Span _span) : type(_type), data(std::monostate{}), span(std::move(_span))
        {
        } // NOLINT(*-explicit-constructor)

        [[nodiscard]] std::string to_string() const
        {
            //FIXME: make a macro that enumerates token types, this sucks

            const std::unordered_map<TokenType, std::string> token_names = {
                {TokenType::EXCLAMATION_MARK, "!"},
                {TokenType::QUESTION_MARK, "?"},
                {TokenType::PERIOD, "."},
                {TokenType::COMMA, ","},
                {TokenType::LEFT_SQUARE_BRACKET, "["},
                {TokenType::RIGHT_SQUARE_BRACKET, "]"},
                {TokenType::LEFT_CURLY_BRACE, "{"},
                {TokenType::RIGHT_CURLY_BRACE, "}"},
                {TokenType::LEFT_PAREN, "("},
                {TokenType::RIGHT_PAREN, ")"},
                {TokenType::LESS_THAN, "<"},
                {TokenType::GREATER_THAN, ">"},
                {TokenType::LESS_THAN_EQUAL_TO, "<="},
                {TokenType::GREATER_THAN_EQUAL_TO, ">="},
                {TokenType::MINUS, "-"},
                {TokenType::PLUS, "+"},
                {TokenType::MULT, "*"},
                {TokenType::DIV, "/"},
                {TokenType::BACK_SLASH, "\\"},
                {TokenType::MOD, "%"},
                {TokenType::XOR, "^"},
                {TokenType::EQUALS, "="},
                {TokenType::OR, "|"},
                {TokenType::AND, "&"},
                {TokenType::PLUS_EQUALS, "+="},
                {TokenType::MINUS_EQUALS, "-="},
                {TokenType::DIV_EQUALS, "/="},
                {TokenType::MULT_EQUALS, "*="},
                {TokenType::MOD_EQUALS, "%="},
                {TokenType::AND_EQUALS, "&="},
                {TokenType::XOR_EQUALS, "^="},
                {TokenType::OR_EQUALS, "|="},
                {TokenType::EQUAL_EQUAL, "=="},
                {TokenType::NOT_EQUAL, "!="},
                {TokenType::SHIFT_LEFT, "<<"},
                {TokenType::SHIFT_RIGHT, ">>"},
                {TokenType::INCREMENT, "++"},
                {TokenType::DECREMENT, "--"},
                {TokenType::EQUAL_EQUAL_EQUAL, "==="},
                {TokenType::NOT_EQUAL_EQUAL, "!=="},
                {TokenType::ARROW, "=>"},
                {TokenType::SEMICOLON, ";"},
                {TokenType::COLON, ":"},
                {TokenType::SINGLE_QUOTED_STRING, "Single quoted string"},
                {TokenType::DOUBLE_QUOTED_STRING, "Double quoted string"},
                {TokenType::IDENTIFIER, "Identifier"},
                {TokenType::INTEGER, "Integer"},
                {TokenType::FLOAT, "Float"},
                {TokenType::LET, "let"},
                {TokenType::VAR, "var"},
                {TokenType::CONST, "const"},
                {TokenType::FUNCTION, "function"},
                {TokenType::RETURN, "return"},
                {TokenType::FOR, "for"},
                {TokenType::WHILE, "while"},
                {TokenType::CONTINUE, "continue"},
                {TokenType::BREAK, "break"},
                {TokenType::NEWLINE, "\\n"},
                {TokenType::END_OF_FILE, "EOF"},
                {TokenType::INVALID, "Invalid"},
                {TokenType::WHITESPACE, "\\w"}
            };

            if (!token_names.contains(type))
            {
                Log::the().critical("No name for TokenType ", static_cast<int>(type));
                return "";
            }

            std::ostringstream data_string;
            data_string << "(";
            bool has_extra_data = true;
            switch (type)
            {
            case TokenType::IDENTIFIER:
            case TokenType::SINGLE_QUOTED_STRING:
            case TokenType::DOUBLE_QUOTED_STRING:
                data_string << unwrap<std::string>();
                break;
            case TokenType::INTEGER:
                data_string << unwrap<int32_t>();
                break;
            case TokenType::FLOAT:
                data_string << unwrap<double>();
                break;
            default:
                has_extra_data = false;
                break;
            }
            data_string << ") at " << span.to_string();

            std::ostringstream output;
            output << token_names.at(type);
            if (has_extra_data)
            {
                output << data_string.str();
            }
            return output.str();
        }

        template <typename T>
        [[nodiscard]] T unwrap() const
        {
            assert(std::holds_alternative<T>(data));
            return std::get<T>(data);
        }
    };

    class Lexer
    {
    public:
        explicit Lexer(std::span<const char>);
        std::vector<Token> lex(const std::string& file_name);
        Token next();

    private:
        template <typename P>
        std::span<const char> consume_while(P predicate);

        std::span<const char> consume_until(char c);

        std::span<const char> consume(size_t n);
        char consume();

        [[nodiscard]] Span span_from_here(size_t num_characters_back = 1) const;

        void rewind(size_t off = 1);

        [[nodiscard]] char peek(size_t off = 0) const;

        [[nodiscard]] bool matches(const std::string& pattern) const;

        std::unordered_map<std::string, TokenType> m_keywords;
        std::unordered_map<std::string, TokenType> m_three_character_symbols;
        std::unordered_map<std::string, TokenType> m_two_character_symbols;
        std::unordered_map<std::string, TokenType> m_one_character_symbols;

        std::span<const char> m_input;
        size_t m_index;

        size_t m_col{0};
        size_t m_line_number{0};
        std::string m_filename{};
    };
}


#endif //LEXER_H
