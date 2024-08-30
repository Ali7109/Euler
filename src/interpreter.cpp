#include "interpreter.h"
#include <iostream>
#include <stdexcept>
#include <cmath>

void Interpreter::interpret(ASTNode *node)
{
    if (node->type == ASTNodeType::PROGRAM)
    {
        for (ASTNode *child : node->children)
        {
            interpret(child);
        }
    }
    else if (node->type == ASTNodeType::VAR_DECL)
    {
        double value = evaluate(node->children[0]);
        variables[node->value] = value;
    }
    else if (node->type == ASTNodeType::OUTPUT)
    {
        double value = evaluate(node->children[0]);

        std::cout << value << std::endl;
    }
}

double Interpreter::evaluate(ASTNode *node)
{
    switch (node->type)
    {
    case ASTNodeType::EXPRESSION:
        return evaluate(node->children[0]);

    case ASTNodeType::VAR_DECL:
        return variables[node->value];

    case ASTNodeType::OUTPUT:
        return evaluate(node->children[0]);

    // Handle arithmetic operations
    case ASTNodeType::ADD:
        return evaluate(node->children[0]) + evaluate(node->children[1]);

    case ASTNodeType::SUBTRACT:
        return evaluate(node->children[0]) - evaluate(node->children[1]);

    case ASTNodeType::MULTIPLY:
        return evaluate(node->children[0]) * evaluate(node->children[1]);

    case ASTNodeType::DIVIDE:
        return evaluate(node->children[0]) / evaluate(node->children[1]);

    case ASTNodeType::POWER:
        return std::pow(evaluate(node->children[0]), evaluate(node->children[1]));

    // Handle numbers
    case ASTNodeType::NUMBER:
        return std::stod(node->value);

    // Handle variable references
    case ASTNodeType::IDENTIFIER:
        if (variables.find(node->value) != variables.end())
        {
            return variables[node->value];
        }
        else
        {
            throw std::runtime_error("Undefined variable: " + node->value);
        }

    // Handle function calls
    case ASTNodeType::FUNCTION_CALL:
        if (node->value == "add")
            return evaluate(node->children[0]) + evaluate(node->children[1]);
        if (node->value == "sub")
            return evaluate(node->children[0]) - evaluate(node->children[1]);
        if (node->value == "prod")
            return evaluate(node->children[0]) * evaluate(node->children[1]);
        if (node->value == "div")
        {
            double arg2 = evaluate(node->children[1]);
            if (arg2 == 0.0)
            {
                throw std::runtime_error("Division by zero is not admissible");
            }
            return evaluate(node->children[0]) / arg2;
        }
        if (node->value == "pow")
            return std::pow(evaluate(node->children[0]), evaluate(node->children[1]));

    default:
        throw std::runtime_error("Unknown AST node type");
    }
}
