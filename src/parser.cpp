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
            std::string errStr = "Expected '()' after 'main'";
            std::string buffer = getOutputLog("ERROR --", errStr);
            logFile << buffer << std::endl;
            throw std::runtime_error(errStr);
        }
        return parseBlock();
    }
    else if (tokens[pos].type == IDENTIFIER && tokens[pos].value == "let")
    {
        std::string buffer = getOutputLog("IDENTIFIER", tokens[pos].value.c_str());
        logFile << buffer << std::endl;
        pos++;
        std::string varName = tokens[pos++].value;
        if (tokens[pos++].type != ASSIGN)
        {
            std::string errStr = "Expected '=' after variable name";
            std::string buffer = getOutputLog("ERROR --", errStr);
            logFile << buffer << std::endl;
            throw std::runtime_error(errStr);
        }
        ASTNode *expr = parseExpression();
        return new ASTNode(ASTNodeType::VAR_DECL, varName, expr);
    }
    else if (tokens[pos].type == IDENTIFIER && tokens[pos].value == "output")
    {
        std::string buffer = getOutputLog("IDENTIFIER", tokens[pos].value.c_str());
        logFile << buffer << std::endl;
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
    std::string buffer = getOutputLog("BLOCK", "");
    logFile << buffer << std::endl;

    if (tokens[pos++].type != LBRACE)
    {
        std::string errStr = "Expected '{' at the beginning of block";
        std::string buffer = getOutputLog("ERROR --", errStr);
        logFile << buffer << std::endl;
        throw std::runtime_error(errStr);
    }

    ASTNode *block = new ASTNode(ASTNodeType::PROGRAM);
    while (tokens[pos].type != RBRACE)
    {
        block->children.push_back(parseStatement());
    }

    if (tokens[pos++].type != RBRACE)
    {
        std::string errStr = "Expected '}' at the end of block";
        std::string buffer = getOutputLog("ERROR --", errStr);
        logFile << buffer << std::endl;
        throw std::runtime_error(errStr);
    }

    return block;
}

ASTNode *Parser::parseExpression()
{
    ASTNode *node = parseTerm();

    while (pos < tokens.size() && (tokens[pos].type == OPERATOR && (tokens[pos].value == "+" || tokens[pos].value == "-")))
    {
        std::string op = tokens[pos++].value;
        std::string buffer = getOutputLog("IDENTIFIER", op);
        logFile << buffer << std::endl;
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
    ASTNode *node = parseFactor();

    while (pos < tokens.size() && (tokens[pos].type == OPERATOR && (tokens[pos].value == "*" || tokens[pos].value == "/")))
    {
        std::string op = tokens[pos++].value;
        ASTNode *nextFactor = parseFactor();

        std::string buffer = getOutputLog("IDENTIFIER", op);
        logFile << buffer << std::endl;

        if (op == "*")
            node = new ASTNode(ASTNodeType::MULTIPLY, node);
        else if (op == "/")
        {
            if (nextFactor == nullptr)
            {
                std::string errStr = "Division by zero is not admissible";
                std::string buffer = getOutputLog("ERROR --", errStr);
                logFile << buffer << std::endl;
                throw std::runtime_error(errStr);
            }
            node = new ASTNode(ASTNodeType::DIVIDE, node);
        }

        node->children.push_back(nextFactor);
    }

    return node;
}

ASTNode *Parser::parseFactor()
{
    if (tokens[pos].type == NUMBER)
    {
        std::string buffer = getOutputLog("FACTOR", tokens[pos].value);
        logFile << buffer << std::endl;
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
            std::string errStr = "Expected ')'";
            std::string buffer = getOutputLog("ERROR --", errStr);
            logFile << buffer << std::endl;
            throw std::runtime_error(errStr);
        }
        pos++;
        return node;
    }
    else
    {
        std::string errStr = "Unexpected token";
        std::string buffer = getOutputLog("ERROR --", errStr);
        logFile << buffer << std::endl;
        throw std::runtime_error(errStr);
    }
}

ASTNode *Parser::parseFunctionCall(const std::string &funcName)
{
    std::string buffer = getOutputLog("FUNCTION NAME", funcName);
    logFile << buffer << std::endl;

    if (tokens[pos++].type != LPAREN)
    {
        std::string errStr = "Expected '(' after function name";
        std::string buffer = getOutputLog("ERROR --", errStr);
        logFile << buffer << std::endl;
        throw std::runtime_error(errStr);
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
        std::string errStr = "Expected ')'";
        std::string buffer = getOutputLog("ERROR --", errStr);
        logFile << buffer << std::endl;
        throw std::runtime_error(errStr);
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
