//
// Created by wuser on 21/10/22.
//

#ifndef JAPC_PARSER_UTILS_H
#define JAPC_PARSER_UTILS_H

#include "japc/AST/type.h"
#include "japc/scanner/scanner.h"
#include <memory>
#include <vector>
class ParserUtils
{
  public:
    static bool isAnyOf(Pascal::Token to, std::vector<Pascal::Token> tkList);
    static bool isAnyOf(Pascal::Token to, std::vector<Pascal::TokenType> tkList);
    static int getPrecedence(Pascal::TokenType tk);
    static std::shared_ptr<Pascal::ConstantDeclaration> evaluateConstant(
        const std::shared_ptr<Pascal::ConstantDeclaration> rhs, const Pascal::TokenType tk,
        const std::shared_ptr<Pascal::ConstantDeclaration> lhs);
    static int64_t constantDeclarationToInteger(const Pascal::ConstantDeclaration* c);
    static bool numericLiteralIsInteger(Pascal::Token tk);
};
#endif // JAPC_PARSER_UTILS_H
