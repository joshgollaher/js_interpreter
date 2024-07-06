#include "Lexer.h"

#include <cstdint>
#include <iostream>
#include <algorithm>

#include "errors.h"
#include "Log.h"

/*
 * TODO
 * Multisymbol parsing (++, --, &=, etc
 * Make add regex, single quoted strings, double quoted strings, keywords
 */

namespace JS
{
    Lexer::Lexer(const std::span<const char> input)
    {
        m_index = 0;
        m_input = input;

        m_keywords = {
            {"function", TokenType::FUNCTION},
            {"let", TokenType::LET},
            {"const", TokenType::CONST},
            {"var", TokenType::VAR},
            {"return", TokenType::RETURN},
            {"for", TokenType::FOR},
            {"while", TokenType::WHILE},
            {"continue", TokenType::CONTINUE},
            {"break", TokenType::BREAK},
        };

        m_three_character_symbols = {
            {"===", TokenType::EQUAL_EQUAL_EQUAL},
            {"!==", TokenType::NOT_EQUAL_EQUAL},
        };

        m_two_character_symbols = {
            {"+=", TokenType::PLUS_EQUALS},
            {"-=", TokenType::MINUS_EQUALS},
            {"*=", TokenType::MULT_EQUALS},
            {"/=", TokenType::DIV_EQUALS},
            {"++", TokenType::INCREMENT},
            {"--", TokenType::DECREMENT},
            {"%=", TokenType::MOD_EQUALS},
            {"|=", TokenType::OR_EQUALS},
            {"&=", TokenType::AND_EQUALS},
            {"^=", TokenType::MOD_EQUALS},
            {">>", TokenType::SHIFT_RIGHT},
            {"<<", TokenType::SHIFT_LEFT},
            {"=>", TokenType::ARROW},
        };

        m_one_character_symbols = {
            {"+", TokenType::PLUS},
            {"-", TokenType::MINUS},
            {"/", TokenType::DIV},
            {"*", TokenType::MULT},
            {"(", TokenType::LEFT_PAREN},
            {")", TokenType::RIGHT_PAREN},
            {"[", TokenType::LEFT_SQUARE_BRACKET},
            {"]", TokenType::RIGHT_SQUARE_BRACKET},
            {"{", TokenType::LEFT_CURLY_BRACE},
            {"}", TokenType::RIGHT_CURLY_BRACE},
            {"%", TokenType::MOD},
            {"^", TokenType::XOR},
            {"&", TokenType::AND},
            {".", TokenType::PERIOD},
            {",", TokenType::COMMA},
            {"=", TokenType::EQUALS},
            {";", TokenType::SEMICOLON},
            {":", TokenType::COLON},
        };
    }

    Token Lexer::next()
    {
        if(m_index >= m_input.size())
        {
            return {TokenType::END_OF_FILE, span_from_here()};
        }
        if (std::isspace(peek()))
        {
            if (peek() == '\n') // FIXME: handle carriage return
            {
                ++m_line_number;
                m_col = 0;
            }
            else
            {
                ++m_col;
            }
            consume();
            return {TokenType::WHITESPACE, span_from_here()};
        }

        switch (peek())
        {
        case '"':
            {
                consume();
                // Consume double quoted string
                auto string_span = consume_until('"');
                consume();  // Consume trailing quote
                std::string string {string_span.data(), string_span.size()};
                return {TokenType::DOUBLE_QUOTED_STRING, string, span_from_here(string.size() + 1)};
            }
            break;
        case '\'':
            {
                // Consume single quoted string
                consume();
                // Consume double quoted string
                auto string_span = consume_until('\'');
                consume();  // Consume trailing quote
                std::string string {string_span.data(), string_span.size()};
                return {TokenType::SINGLE_QUOTED_STRING, string, span_from_here(string.size() + 1)};
            }
            break;
        default:
            break;
        }

        // Progressively narrow matches
        for (const auto& [keyword, token_type] : m_keywords)
        {
            if (matches(keyword))
            {
                // Insert keyword token
                consume(keyword.size());
                return {token_type, span_from_here(keyword.size())};
            }
        }

        for (const auto& [symbol, token_type] : m_three_character_symbols)
        {
            if (matches(symbol))
            {
                // Insert keyword token
                consume(symbol.size());
                return {token_type, span_from_here(3)};
            }
        }

        for (const auto& [symbol, token_type] : m_two_character_symbols)
        {
            if (matches(symbol))
            {
                // Insert keyword token
                consume(symbol.size());
                return {token_type, span_from_here(2)};
            }
        }

        for (const auto& [symbol, token_type] : m_one_character_symbols)
        {
            if (matches(symbol))
            {
                // Insert keyword token
                consume();
                return {token_type, span_from_here(1)};
            }
        }

        if (std::isalpha(peek()))
        {
            // Consume identifier
            auto str_span = consume_while([](const char c)
            {
                return std::isalnum(c);
            });

            std::string str{str_span.data(), str_span.size()};
            // Parse string against keywords
            return {TokenType::IDENTIFIER, str, span_from_here(str.size())};
        }

        if (std::isdigit(peek()))
        {
            // Consume number
            bool has_seen_period = false;
            auto num_span = consume_while([&](const char c)
            {
                if (c == '.')
                {
                    if (has_seen_period)
                    {
                        throw InvalidSyntax{};
                    }
                    has_seen_period = true;
                    return true;
                }
                return std::isdigit(c) != 0;
            });

            const std::string num_string{num_span.data(), num_span.size()};

            //Janky way to find floating point
            if (num_string.find('.') != std::string::npos)
            {
                const double num = std::atof(num_string.c_str());
                return {TokenType::FLOAT, num, span_from_here(num_string.size())};
            }

            const int32_t num = std::atoi(num_string.c_str());
            return {TokenType::INTEGER, num, span_from_here(num_string.size())};
        }

        Log::the().error("Unknown character: ", consume());
        return {TokenType::INVALID, span_from_here()};
    }


    std::vector<Token> Lexer::lex(const std::string& file_name)
    {
        m_filename = file_name;
        m_line_number = 0;
        m_col = 0;

        std::vector skipped_types {
            TokenType::INVALID, TokenType::WHITESPACE
        };

        std::vector<Token> ret;
        while(true)
        {
            auto tok = next();
            if(std::find(skipped_types.begin(), skipped_types.end(), tok.type) != skipped_types.end())
            {
                // Skip
                continue;
            }
            ret.push_back(tok);
            if(tok.type == TokenType::END_OF_FILE)
            {
                break;
            }
        }

        return ret;
    }

    std::span<const char> Lexer::consume_until(const char stop)
    {
        size_t len = 0;

        while (true)
        {
            if (peek(len) == stop)
            {
                break;
            }

            ++len;
        }

        const std::span<const char> span{m_input.begin() + static_cast<long long>(m_index), len};
        m_index += len;
        return span;
    }

    std::span<const char> Lexer::consume(const size_t n)
    {
        const std::span<const char> ret {m_input.begin() + static_cast<long long>(m_index), n};
        m_index += n;
        return ret;
    }

    char Lexer::consume()
    {
        return m_input[m_index++];
    }

    Span Lexer::span_from_here(const size_t num_characters_back) const
    {
        // FIXME: This is tricky because be need to underflow col on a multiline string for example
        // Also theres a bug where they underflow on the first col
        // Thus, col numbers currently overflow, lol
        return {
            m_filename,
            {m_line_number, m_col - num_characters_back, m_index - num_characters_back},
            {m_line_number, m_col, m_index}
        };
    }

    void Lexer::rewind(const size_t off)
    {
        m_index -= off;
    }

    char Lexer::peek(const size_t off) const
    {
        return m_input[m_index + off];
    }

    bool Lexer::matches(const std::string& pattern) const
    {
        if (m_index + pattern.size() >= m_input.size())
        {
            return false;
        }

        size_t i = 0;
        for (auto& c : pattern)
        {
            if (peek(i) != c)
            {
                return false;
            }
            ++i;
        }

        return true;
    }

    template <typename P>
    std::span<const char> Lexer::consume_while(P predicate)
    {
        size_t len = 0;

        while (true)
        {
            if (!predicate(peek(len)))
            {
                break;
            }

            ++len;
        }

        const std::span span{m_input.begin() + static_cast<long long>(m_index), len};
        m_index += len;
        return span;
    }
}
