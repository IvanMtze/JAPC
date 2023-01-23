//
// Created by wuser on 21/10/22.
//

#ifndef JAPC_PARSER_UTILS_H
#define JAPC_PARSER_UTILS_H

#include "japc/AST/type.h"
#include "japc/AST/real_expression.h"
#include "japc/AST/integer_expression.h"
#include "japc/scanner/scanner.h"
#include <memory>
namespace Pascal
{
class ParserUtils
{
  public:
    static bool isAnyOf(Token to, std::vector<TokenType>& tkList);
    static int getPrecedence(TokenType tk);
    static std::shared_ptr<ConstantDeclaration> evaluateConstant(
        const std::shared_ptr<ConstantDeclaration> rhs, const TokenType tk,
        const std::shared_ptr<ConstantDeclaration> lhs);
    static int64_t constantDeclarationToInteger(const ConstantDeclaration *c);
    static bool numericLiteralIsInteger(Token tk);
    static std::shared_ptr<ExpressionAST> constantDeclarationToExpression(
        Location loc, ConstantDeclaration *constantDeclaration);
    static std::shared_ptr<TypeDeclaration> copyWithInitialValue(
        std::shared_ptr<TypeDeclaration> typeDeclaration, std::shared_ptr<ExpressionAST> initial);
    static int64_t convertStringToInteger(std::basic_string<char> ref);
    static bool isFunctionCall(NamedObject* named, Token next);
};
}
#endif // JAPC_PARSER_UTILS_H
