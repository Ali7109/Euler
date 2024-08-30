#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "ast.h"
#include <unordered_map>
#include <string>

class Interpreter
{
public:
    void interpret(ASTNode *node);

private:
    std::unordered_map<std::string, double> variables;

    double evaluate(ASTNode *node);
};

#endif // INTERPRETER_H
