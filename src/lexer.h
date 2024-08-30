#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>

// Define different types of tokens that the lexer will recognize
enum TokenType
{
    NUMBER,     // Numeric literals
    IDENTIFIER, // Variable names or function names
    OPERATOR,   // Operators like +, -, *, /
    LPAREN,     // Left parenthesis
    RPAREN,     // Right parenthesis
    LBRACE,     // Left Bracket {
    RBRACE,     // Right Bracket }
    COMMA,      // Comma
    ASSIGN,     // Assignment operator '='
    END         // End of Input
};

// Define a token structure to hold the type and value of a token
struct Token
{
    TokenType type;
    std::string value;
};

// Lexer class definition
class Lexer
{
public:
    Lexer(const std::string &input);
    std::vector<Token> tokenize();

private:
    std::string input; // The input string to be tokenized
    size_t pos;        // Current position in the input string

    std::string readNumber();
    std::string readIdentifier();
};

#endif // LEXER_H
