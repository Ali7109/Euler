#include "lexer.h"
#include <cctype>
#include <stdexcept>

Lexer::Lexer(const std::string &input) : input(input), pos(0) {}

std::vector<Token> Lexer::tokenize()
{
    std::vector<Token> tokens;
    while (pos < input.size())
    {
        char current = input[pos];
        if (std::isspace(current))
        {
            pos++;
        }
        else if (std::isalpha(current))
        {
            tokens.push_back(Token{TokenType::IDENTIFIER, readIdentifier()});
        }
        else if (std::isdigit(current))
        {
            tokens.push_back(Token{TokenType::NUMBER, readNumber()});
        }
        else if (current == '=')
        {
            tokens.push_back(Token{TokenType::ASSIGN, "="});
            pos++;
        }
        else if (current == '+')
        {
            tokens.push_back(Token{TokenType::OPERATOR, "+"});
            pos++;
        }
        else if (current == '-')
        {
            tokens.push_back(Token{TokenType::OPERATOR, "-"});
            pos++;
        }
        else if (current == '*')
        {
            tokens.push_back(Token{TokenType::OPERATOR, "*"});
            pos++;
        }
        else if (current == '/')
        {
            tokens.push_back(Token{TokenType::OPERATOR, "/"});
            pos++;
        }
        else if (current == '(')
        {
            tokens.push_back(Token{TokenType::LPAREN, "("});
            pos++;
        }
        else if (current == ')')
        {
            tokens.push_back(Token{TokenType::RPAREN, ")"});
            pos++;
        }
        else if (current == ',')
        {
            tokens.push_back(Token{TokenType::COMMA, ","});
            pos++;
        }
        else if (current == '{')
        {
            tokens.push_back(Token{TokenType::LBRACE, "{"});
            pos++;
        }
        else if (current == '}')
        {
            tokens.push_back(Token{TokenType::RBRACE, "}"});
            pos++;
        }
        else
        {
            throw std::runtime_error("Unexpected character: " + std::string(1, current));
        }
    }
    tokens.push_back(Token{TokenType::END, ""});
    return tokens;
}

std::string Lexer::readNumber()
{
    std::string number;
    while (pos < input.size() && std::isdigit(input[pos]))
    {
        number += input[pos++];
    }
    return number;
}

std::string Lexer::readIdentifier()
{
    std::string identifier;
    while (pos < input.size() && std::isalnum(input[pos]))
    {
        identifier += input[pos++];
    }
    return identifier;
}
