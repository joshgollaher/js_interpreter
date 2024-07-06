#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <utility>
#include <variant>
#include <span>

#include <assert.h>
#include <cstdint>
#include <sstream>
#include <vector>

#include "Log.h"
#include "Span.h"

namespace JS {

    enum class TokenType: int {
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

        MINUS,
        PLUS,
        ASTERISK,
        FORWARD_SLASH,
        PERCENT_SIGN,
        CARROT,
        EQUALS,

        DOLLAR_SIGN,
        AT_SIGN,
        AMPERSAND,

        SEMICOLON,
        COLON,

        SINGLE_QUOTE,
        DOUBLE_QUOTE,

        STRING,
        INTEGER,
        FLOAT,

        NEWLINE,
        END_OF_FILE
    };

    using TokenData = std::variant<std::monostate, std::string, int32_t, double>;

    struct Token
    {
        TokenType type;
        TokenData data;
        Span span;

        // ReSharper disable once CppNonExplicitConvertingConstructor
        Token(const TokenType _type, TokenData  _data, Span _span) : type(_type), data(std::move(_data)), span(std::move(_span)) {}
        Token(const TokenType _type, Span _span) : type(_type), data(std::monostate{}), span(std::move(_span)) {} // NOLINT(*-explicit-constructor)

        [[nodiscard]] std::string to_string() const
        {
            //FIXME: make a macro that enumerates token types, this sucks
            const std::vector<std::string> token_names = {
                "!", "?",
                ".", ",", "[", "]",
                "{", "}", "(", ")",
                "<", ">", "-", "+", "*",
                "/", "%", "^", "=",
                "$", "@", "&", ";", ":",
                "'", "\"", "String", "Integer",
                "Float", "\\n", "EOF"
            };

            std::ostringstream data_string;
            data_string << "(";

            switch(type)
            {
            case TokenType::STRING:
                data_string << unwrap<std::string>();
                break;
            case TokenType::INTEGER:
                data_string << unwrap<int32_t>();
                break;
            case TokenType::FLOAT:
                data_string << unwrap<double>();
                break;
            default:
                break;
            }
            data_string << ") at " << span.to_string();

            std::ostringstream output;
            output << token_names[static_cast<int>(type)] << data_string.str();
            return output.str();
        }

        template<typename T>
        T unwrap() const
        {
            assert(std::holds_alternative<T>(data));
            return std::get<T>(data);
        }
    };

    class Lexer {
    public:

        explicit Lexer(std::span<const char>);
        std::vector<Token> lex(const std::string& file_name);

    private:

        template<typename P>
        std::span<const char> consume_while(P predicate);

        std::span<const char> consume_until(char c);

        char consume();

        [[nodiscard]] Span span_from_here(size_t num_characters_back = 1) const;

        void rewind(size_t off = 1);

        [[nodiscard]] char peek(size_t off = 0) const;

        std::span<const char> m_input;
        size_t m_index;

        size_t m_col{0};
        size_t m_line_number{0};
        std::string m_filename{};

    };
}


#endif //LEXER_H
