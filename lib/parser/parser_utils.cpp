//
// Created by wuser on 21/10/22.
//
#include "japc/parser/parser_utils.h"

using namespace Pascal;

bool ParserUtils::isAnyOf(Token to, std::vector<TokenType> &tkList)
{
    for (TokenType tk : tkList)
    {
        if (tk == to.getTokenType())
        {
            return true;
        }
    }
    return false;
}
int ParserUtils::getPrecedence(TokenType tk)
{
    switch (tk)
    {
    case TokenType::SYMBOL_NOT:
        return 60;
    case TokenType::SYMBOL_POW:
    case TokenType::SYMBOL_STAR_STAR:
        return 50;
    case TokenType::SYMBOL_DIV:
    case TokenType::SYMBOL_MOD:
    case TokenType::SYMBOL_AND:
    case TokenType::SYMBOL_AND_THEN:
    case TokenType::SYMBOL_GREATER_LESS_THAN:
    case TokenType::SYMBOL_STAR:
    case TokenType::SYMBOL_SLASH:
        return 40;
    case TokenType::SYMBOL_OR:
    case TokenType::SYMBOL_OR_ELSE:
    case TokenType::SYMBOL_PLUS:
    case TokenType::SYMBOL_MINUS:
        return 10;
    case TokenType::SYMBOL_IN:
    case TokenType::SYMBOL_LESS_THAN:
    case TokenType::SYMBOL_LESS_EQUAL_THAN:
    case TokenType::SYMBOL_GREATER_THAN:
    case TokenType::SYMBOL_GREATER_EQUAL_THAN:
    case TokenType::SYMBOL_EQUAL:
    case TokenType::SYMBOL_LESS_GREATER_THAN:
        return 5;
    case TokenType::SYMBOL_COLON_EQUAL:
        return 2;
    }
    return -1;
}
std::shared_ptr<ConstantDeclaration> ParserUtils::evaluateConstant(const std::shared_ptr<ConstantDeclaration> rhs,
                                                                   const TokenType tk,
                                                                   const std::shared_ptr<ConstantDeclaration> lhs)
{
    switch (tk)
    {
    case TokenType::SYMBOL_PLUS:
        return lhs + rhs;
        break;
    case TokenType::SYMBOL_MINUS:
        return lhs - rhs;
        break;
    case TokenType::SYMBOL_STAR:
        return lhs * rhs;
        break;
    case TokenType::SYMBOL_DIV:
        return lhs / rhs;
        break;
    default:
        break;
    }
    return nullptr;
}

int64_t ParserUtils::constantDeclarationToInteger(const ConstantDeclaration *c)
{
    if (auto ci = llvm::dyn_cast<IntConstantDeclaration>(c))
    {
        return ci->getValue();
    }
    if (auto cc = llvm::dyn_cast<CharConstantDeclaration>(c))
    {
        return cc->getValue();
    }
    if (auto ce = llvm::dyn_cast<EnumConstantDeclaration>(c))
    {
        return ce->getValue();
    }
    if (auto cb = llvm::dyn_cast<BooleanConstantDeclaration>(c))
    {
        return cb->getValue();
    }
    return -1;
}
bool ParserUtils::numericLiteralIsInteger(Token tk)
{
    if (tk.getTokenType() != TokenType::NUMERIC_LITERAL)
    {
        return false;
    }
    char *p;
    long converted = strtol(tk.getValue().c_str(), &p, 10);
    return *p == 0;
}
std::shared_ptr<ExpressionAST> ParserUtils::constantDeclarationToExpression(const Location loc,
                                                                            ConstantDeclaration *constantDeclaration)
{
    std::shared_ptr<TypeDeclaration> type = constantDeclaration->getType();
    if (constantDeclaration->getType()->isIntegral())
    {
        int64_t val = ParserUtils::constantDeclarationToInteger(constantDeclaration);
        return std::make_shared<IntegerExpression>(loc, val, type);
    }
    if (auto rc = llvm::dyn_cast_or_null<RealConstantDeclaration>(constantDeclaration))
    {
        return std::make_shared<RealExpression>(loc, rc->getValue(), type);
    }
    return nullptr;
}
std::shared_ptr<TypeDeclaration> ParserUtils::copyWithInitialValue(std::shared_ptr<TypeDeclaration> typeDeclaration,
                                                                   std::shared_ptr<ExpressionAST> initial)
{
    std::shared_ptr<TypeDeclaration> typeDeclarationObj = typeDeclaration->clone();
    typeDeclarationObj->setInitialValue(initial);
    return typeDeclarationObj;
}
int64_t ParserUtils::convertStringToInteger(std::basic_string<char> ref)
{
    return stoi(ref);
}
bool ParserUtils::isFunctionCall(NamedObject *named, Token next)
{
    std::shared_ptr<TypeDeclaration> typeDeclaration = named->getTypeDeclaration();
    TypeKind typeKind = typeDeclaration->getTypeKind();
    if (typeKind == TypeKind::TYPE_FUNCION_POINTER ||
        (typeKind == TypeKind::TYPE_FUNCTION && next.getTokenType() != TokenType::SYMBOL_COLON_EQUAL))
        return true;
    return false;
}
