#include "Lexer.h"

#include <cstdint>
#include <iostream>

#include "errors.h"
#include "Log.h"

namespace JS
{
    Lexer::Lexer(std::span<const char> input)
    {
        m_index = 0;
        m_input = input;
    }

    std::vector<Token> Lexer::lex(const std::string& file_name)
    {
        m_filename = file_name;
        m_line_number = 0;
        m_col = 0;

        std::vector<Token> ret;

        while(m_index < m_input.size()) {
            if(std::isspace(peek())) {
                if(peek() == '\n')  // FIXME: handle carriage return
                {
                    ++m_line_number;
                    m_col = 0;
                } else
                {
                    ++m_col;
                }
                consume();
                continue;
            }

            switch(consume())
            {
            case '+':
                ret.emplace_back(TokenType::PLUS, span_from_here());
                break;
            case '-':
                ret.emplace_back(TokenType::MINUS, span_from_here());
                break;
            case '/':
                ret.emplace_back(TokenType::FORWARD_SLASH, span_from_here());
                break;
            case '$':
                ret.emplace_back(TokenType::DOLLAR_SIGN, span_from_here());
                break;
            case '!':
                ret.emplace_back(TokenType::EXCLAMATION_MARK, span_from_here());
                break;
            case '@':
                ret.emplace_back(TokenType::AT_SIGN, span_from_here());
                break;
            case '%':
                ret.emplace_back(TokenType::PERCENT_SIGN, span_from_here());
                break;
            case '^':
                ret.emplace_back(TokenType::CARROT, span_from_here());
                break;
            case '&':
                ret.emplace_back(TokenType::AMPERSAND, span_from_here());
                break;
            case '*':
                ret.emplace_back(TokenType::ASTERISK, span_from_here());
                break;
            case '(':
                ret.emplace_back(TokenType::LEFT_PAREN, span_from_here());
                break;
            case ')':
                ret.emplace_back(TokenType::RIGHT_PAREN, span_from_here());
                break;
            case '[':
                ret.emplace_back(TokenType::LEFT_SQUARE_BRACKET, span_from_here());
                break;
            case ']':
                ret.emplace_back(TokenType::RIGHT_SQUARE_BRACKET, span_from_here());
                break;
            case '{':
                ret.emplace_back(TokenType::LEFT_CURLY_BRACE, span_from_here());
                break;
            case '}':
                ret.emplace_back(TokenType::RIGHT_CURLY_BRACE, span_from_here());
                break;
            case '\'':
                ret.emplace_back(TokenType::SINGLE_QUOTE, span_from_here());
                break;
            case '"':
                ret.emplace_back(TokenType::DOUBLE_QUOTE, span_from_here());
                break;
            case '<':
                ret.emplace_back(TokenType::LESS_THAN, span_from_here());
                break;
            case '>':
                ret.emplace_back(TokenType::GREATER_THAN, span_from_here());
                break;
            case ',':
                ret.emplace_back(TokenType::COMMA, span_from_here());
                break;
            case '.':  // TODO: Check if floating point
                ret.emplace_back(TokenType::PERIOD, span_from_here());
                break;
            case '?':
                ret.emplace_back(TokenType::QUESTION_MARK, span_from_here());
                break;
            case ';':
                ret.emplace_back(TokenType::SEMICOLON, span_from_here());
                break;
            case ':':
                ret.emplace_back(TokenType::COLON, span_from_here());
                break;
            case '=':
                ret.emplace_back(TokenType::EQUALS, span_from_here());
                break;
            default:
                {
                    rewind();
                    char curr_char = peek();
                    if(std::isalpha(curr_char))
                    {
                        // FIXME: REPLACE THIS
                        // At the " character handler, read until " and insert a string, dont do this
                        auto str_span = consume_while([](char c)
                        {
                            return std::isalnum(c);
                        });

                        std::string str{str_span.data(), str_span.size()};
                        ret.emplace_back(TokenType::STRING, str, span_from_here(str.size()));
                    } else if(std::isdigit(curr_char))
                    {
                        bool has_seen_period = false;
                        auto num_span = consume_while([&](const char c)
                        {
                            if(c == '.')
                            {
                                if(has_seen_period)
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
                        if(num_string.find('.') != std::string::npos)
                        {
                            const double num = std::atof(num_string.c_str());
                            ret.emplace_back(TokenType::FLOAT, num, span_from_here(num_string.size()));
                        } else
                        {
                            const int32_t num = std::atoi(num_string.c_str());
                            ret.emplace_back(TokenType::INTEGER, num, span_from_here(num_string.size()));
                        }

                    } else
                    {
                        Log::the().warn("Unknown char: ", curr_char);
                        consume();
                    }

                    break;
                }
            }

            ++m_col;
        }

        ret.emplace_back(TokenType::END_OF_FILE, span_from_here());
        return ret;
    }

    std::span<const char> Lexer::consume_until(const char stop)
    {
        size_t len = 0;

        while(true)
        {
            if(peek(len) == stop)
            {
                break;
            }

            ++len;
        }

        const std::span<const char> span{m_input.begin() + static_cast<long long>(m_index), len};
        m_index += len;
        return span;
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

    template <typename P>
    std::span<const char> Lexer::consume_while(P predicate)
    {
        size_t len = 0;

        while(true)
        {
            if(!predicate(peek(len)))
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
