#include "parser.cpp"
#include <iomanip>
#include <iostream>
// #include "lexer.h"
// #include "parser.h"

int main()
{
    std::string code = "6+pow(8, 0)*8+2";

    Lexer lexer(code);
    std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    double result = parser.parseExpression();

    std::cout << "Result: " << std::fixed << std::setprecision(5) << result << std::endl;

    return 0;
}