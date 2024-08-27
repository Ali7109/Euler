#include "parser.h"
#include <stdexcept>
#include <cmath>

// Constructor implementation
Parser::Parser(const std::vector<Token> &tokens) : tokens(tokens), pos(0) {}

double Parser::parseExpression()
{
    double result = parseTerm(); // Start by parsing terms

    while (pos < tokens.size() && (tokens[pos].type == OPERATOR && (tokens[pos].value == "+" || tokens[pos].value == "-")))
    {
        std::string op = tokens[pos++].value;

        double nextTerm = parseTerm(); // Parse the next term

        if (op == "+")
            result += nextTerm;
        else if (op == "-")
            result -= nextTerm;
    }

    return result;
}

double Parser::parseTerm()
{
    double result = parseFactor(); // Start by parsing factors

    while (pos < tokens.size() && (tokens[pos].type == OPERATOR && (tokens[pos].value == "*" || tokens[pos].value == "/")))
    {
        std::string op = tokens[pos++].value;

        double nextFactor = parseFactor(); // Parse the next factor

        if (op == "*")
            result *= nextFactor;
        else if (op == "/")
        {
            if (nextFactor == 0.0)
            {
                throw std::runtime_error("Division by zero is not admissible");
            }
            result /= nextFactor;
        }
    }

    return result;
}

double Parser::parseFactor()
{
    if (tokens[pos].type == NUMBER)
    {
        return std::stod(tokens[pos++].value); // Convert to double
    }
    else if (tokens[pos].type == IDENTIFIER)
    {
        return parseFunctionCall();
    }
    else if (tokens[pos].type == LPAREN)
    {
        pos++;
        double result = parseExpression(); // Parse the expression inside parentheses
        if (tokens[pos].type != RPAREN)
        {
            throw std::runtime_error("Expected ')'");
        }
        pos++;
        return result;
    }
    else
    {
        throw std::runtime_error("Unexpected token");
    }
}

double Parser::parseFunctionCall()
{
    std::string funcName = tokens[pos++].value;
    if (tokens[pos].type != LPAREN)
    {
        throw std::runtime_error("Expected '(' after function name");
    }
    pos++;

    double arg1 = parseExpression();
    if (tokens[pos].type == COMMA)
    {
        pos++;
        double arg2 = parseExpression();
        if (tokens[pos].type != RPAREN)
        {
            throw std::runtime_error("Expected ')'");
        }
        pos++;
        if (funcName == "add")
            return arg1 + arg2;
        if (funcName == "sub")
            return arg1 - arg2;
        if (funcName == "prod")
            return arg1 * arg2;
        if (funcName == "div")
        {
            if (arg2 == 0.0)
            {
                throw std::runtime_error("Division by zero is not admissible");
            }
            return arg1 / arg2;
        }
        if (funcName == "pow")
            return std::pow(arg1, arg2);
    }
    throw std::runtime_error("Invalid function call");
}
