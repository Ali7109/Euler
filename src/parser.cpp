#include "parser.h"
#include "helperFunctions.h"
#include <stdexcept>
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>

Parser::Parser(const std::vector<Token> &tokens) : tokens(tokens), pos(0) {}
std::ofstream logFile("execution.log");

ASTNode *Parser::parse()
{

    ASTNode *root = new ASTNode(ASTNodeType::PROGRAM);
    while (pos < tokens.size() && tokens[pos].type != TokenType::END)
    {
        root->children.push_back(parseStatement());
    }
    return root;
}

ASTNode *Parser::parseStatement()
{

    if (tokens[pos].type == IDENTIFIER && tokens[pos].value == "main")
    {

        std::string buffer = getOutputLog("IDENTIFIER", tokens[pos].value.c_str());

        logFile << buffer << std::endl;

        pos++;
        if (tokens[pos++].type != LPAREN || tokens[pos++].type != RPAREN)
        {
            throw std::runtime_error("Expected '()' after 'main'");
        }
        return parseBlock();
    }
    else if (tokens[pos].type == IDENTIFIER && tokens[pos].value == "let")
    {
        pos++;
        std::string varName = tokens[pos++].value;
        if (tokens[pos++].type != ASSIGN)
        {
            throw std::runtime_error("Expected '=' after variable name");
        }
        ASTNode *expr = parseExpression();
        return new ASTNode(ASTNodeType::VAR_DECL, varName, expr);
    }
    else if (tokens[pos].type == IDENTIFIER && tokens[pos].value == "output")
    {
        pos++;
        ASTNode *expr = parseExpression();
        return new ASTNode(ASTNodeType::OUTPUT, expr);
    }
    else
    {
        return parseExpression();
    }
}

ASTNode *Parser::parseBlock()
{
    logFile << "Parsing block..." << std::endl;
    if (tokens[pos++].type != LBRACE)
    {
        throw std::runtime_error("Expected '{' at the beginning of block");
    }

    ASTNode *block = new ASTNode(ASTNodeType::PROGRAM);
    while (tokens[pos].type != RBRACE)
    {
        block->children.push_back(parseStatement());
    }

    if (tokens[pos++].type != RBRACE)
    {
        throw std::runtime_error("Expected '}' at the end of block");
    }

    return block;
}

ASTNode *Parser::parseExpression()
{
    logFile << "Parsing expression..." << std::endl;
    ASTNode *node = parseTerm();

    while (pos < tokens.size() && (tokens[pos].type == OPERATOR && (tokens[pos].value == "+" || tokens[pos].value == "-")))
    {
        std::string op = tokens[pos++].value;
        ASTNode *nextTerm = parseTerm();

        if (op == "+")
            node = new ASTNode(ASTNodeType::ADD, node);
        else if (op == "-")
            node = new ASTNode(ASTNodeType::SUBTRACT, node);

        node->children.push_back(nextTerm);
    }

    return node;
}

ASTNode *Parser::parseTerm()
{
    logFile << "Parsing term..." << std::endl;
    ASTNode *node = parseFactor();

    while (pos < tokens.size() && (tokens[pos].type == OPERATOR && (tokens[pos].value == "*" || tokens[pos].value == "/")))
    {
        std::string op = tokens[pos++].value;
        ASTNode *nextFactor = parseFactor();

        if (op == "*")
            node = new ASTNode(ASTNodeType::MULTIPLY, node);
        else if (op == "/")
        {
            if (nextFactor == nullptr)
            {
                throw std::runtime_error("Division by zero is not admissible");
            }
            node = new ASTNode(ASTNodeType::DIVIDE, node);
        }

        node->children.push_back(nextFactor);
    }

    return node;
}

ASTNode *Parser::parseFactor()
{
    logFile << "Parsing factor..." << std::endl;
    if (tokens[pos].type == NUMBER)
    {
        return new ASTNode(ASTNodeType::NUMBER, tokens[pos++].value);
    }
    else if (tokens[pos].type == IDENTIFIER)
    {
        std::string identifier = tokens[pos++].value;
        if (tokens[pos].type == LPAREN)
        {
            return parseFunctionCall(identifier);
        }
        else
        {
            return new ASTNode(ASTNodeType::IDENTIFIER, identifier);
        }
    }
    else if (tokens[pos].type == LPAREN)
    {
        pos++;
        ASTNode *node = parseExpression();
        if (tokens[pos].type != RPAREN)
        {
            throw std::runtime_error("Expected ')'");
        }
        pos++;
        return node;
    }
    else
    {
        throw std::runtime_error("Unexpected token");
    }
}

ASTNode *Parser::parseFunctionCall(const std::string &funcName)
{
    logFile << "Parsing function call..." << std::endl;
    if (tokens[pos++].type != LPAREN)
    {
        throw std::runtime_error("Expected '(' after function name");
    }

    ASTNode *arg1 = parseExpression();
    ASTNode *arg2 = nullptr;
    if (tokens[pos].type == COMMA)
    {
        pos++;
        arg2 = parseExpression();
    }
    if (tokens[pos].type != RPAREN)
    {
        throw std::runtime_error("Expected ')'");
    }
    pos++;

    ASTNode *funcCall = new ASTNode(ASTNodeType::FUNCTION_CALL, funcName);
    funcCall->children.push_back(arg1);
    if (arg2)
    {
        funcCall->children.push_back(arg2);
    }

    return funcCall;
}
