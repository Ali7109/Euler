#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <string>

// Include Token definition from lexer.h
#include "lexer.h"

class Parser
{
public:
    Parser(const std::vector<Token> &tokens);

    double parseExpression(); // Handles addition and subtraction

private:
    std::vector<Token> tokens;
    size_t pos;

    double parseTerm();         // Handles multiplication and division
    double parseFactor();       // Handles numbers, parentheses, and functions
    double parseFunctionCall(); // Handles function calls like add, sub, etc.
};

#endif // PARSER_H
