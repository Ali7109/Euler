#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "ast.h"
#include <vector>
#include <string>

class Parser
{
public:
    Parser(const std::vector<Token> &tokens);
    ASTNode *parse(); // Parse the entire script

private:
    std::vector<Token> tokens;
    size_t pos;

    ASTNode *parseStatement();
    ASTNode *parseBlock();
    ASTNode *parseExpression();
    ASTNode *parseTerm();
    ASTNode *parseFactor();
    ASTNode *parseFunctionCall(const std::string &funcName);
};

#endif // PARSER_H
