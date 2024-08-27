#include "lexer.h"
#include <cctype>

// Constructor implementation
Lexer::Lexer(const std::string &input) : input(input), pos(0) {}

// Tokenization method implementation
std::vector<Token> Lexer::tokenize()
{

    std::vector<Token> tokens;

    while (pos < input.size())
    {
        if (std::isspace(input[pos]))
        {
            // Skip whitespace characters
            pos++;
            continue;
        }
        if (std::isdigit(input[pos]))
        {
            // Process numeric literals
            tokens.push_back(Token{NUMBER, readNumber()});
            continue;
        }
        if (std::isalpha(input[pos]))
        {
            // Process identifiers (variable or function names)
            tokens.push_back(Token{IDENTIFIER, readIdentifier()});
            continue;
        }
        switch (input[pos])
        {
        case '+':
        case '-':
        case '*':
        case '/':
            // Process operators
            tokens.push_back(Token{OPERATOR, std::string(1, input[pos++])});
            break;
        case '(':
            // Process left parenthesis
            tokens.push_back(Token{LPAREN, "("});
            pos++;
            break;
        case ')':
            // Process right parenthesis
            tokens.push_back(Token{RPAREN, ")"});
            pos++;
            break;
        case ',':
            // Process comma
            tokens.push_back(Token{COMMA, ","});
            pos++;
            break;
        default:
            // Skip any unrecognized characters
            pos++;
            break;
        }
    }
    // Add an end token to signify the end of input
    tokens.push_back(Token{END, ""});
    return tokens;
}

// Method to read a number from the input string
std::string Lexer::readNumber()
{
    size_t start = pos;
    bool hasDecPoint = false;

    while (pos < input.size() && (std::isdigit(input[pos]) || input[pos] == '.'))
    {

        if (input[pos] == '.')
        {
            if (hasDecPoint)
                break;
            hasDecPoint = true;
        }
        pos++;
    }
    return input.substr(start, pos - start);
}

// Method to read an identifier (variable or function name) from the input string
std::string Lexer::readIdentifier()
{
    size_t start = pos;
    while (pos < input.size() && std::isalnum(input[pos]))
        pos++;
    return input.substr(start, pos - start);
}