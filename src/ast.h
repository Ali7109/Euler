#ifndef AST_H
#define AST_H

#include <string>
#include <vector>

enum class ASTNodeType
{
    PROGRAM,
    VAR_DECL,
    OUTPUT,
    EXPRESSION,
    ADD,          // Addition
    SUBTRACT,     // Subtraction
    MULTIPLY,     // Multiplication
    DIVIDE,       // Division
    POWER,        // Power
    NUMBER,       // Number literal
    IDENTIFIER,   // Variable reference
    FUNCTION_CALL // Function call
    // Add other node types as needed
};

class ASTNode
{
public:
    ASTNodeType type;
    std::string value;
    std::vector<ASTNode *> children;

    // Existing constructor
    ASTNode(ASTNodeType type, const std::string &value = "", ASTNode *child = nullptr)
        : type(type), value(value)
    {
        if (child)
        {
            children.push_back(child);
        }
    }

    // New constructor to match the argument list in parser.cpp
    ASTNode(ASTNodeType type, ASTNode *child)
        : type(type)
    {
        if (child)
        {
            children.push_back(child);
        }
    }

    ~ASTNode()
    {
        for (ASTNode *child : children)
        {
            delete child;
        }
    }
};

#endif // AST_H
