//
// Created by wuser on 21/10/22.
//
#include "parser_utils.h"

bool ParserUtils::isAnyOf(Pascal::Token to, std::vector<Pascal::Token> tkList)
{
    for (Pascal::Token tk : tkList)
    {
        if (tk.getTokenType() == to.getTokenType())
        {
            return true;
        }
    }
    return false;
}

bool ParserUtils::isAnyOf(Pascal::Token to, std::vector<Pascal::TokenType> tkList)
{
    for (Pascal::TokenType tk : tkList)
    {
        if (tk == to.getTokenType())
        {
            return true;
        }
    }
    return false;
}
int ParserUtils::getPrecedence(Pascal::TokenType tk)
{
    return 0;
}
std::shared_ptr<Pascal::ConstantDeclaration> ParserUtils::evaluateConstant(
    const std::shared_ptr<Pascal::ConstantDeclaration> rhs, const Pascal::TokenType tk,
    const std::shared_ptr<Pascal::ConstantDeclaration> lhs)
{
    switch(tk){
    case Pascal::TokenType::SYMBOL_PLUS:
        return lhs.get() + rhs.get();
        break;
    case Pascal::TokenType::SYMBOL_MINUS:
        return lhs.get() - rhs.get();
        break;
    case Pascal::TokenType::SYMBOL_STAR:
        return lhs.get() * rhs.get();
        break;
    case Pascal::TokenType::SYMBOL_DIV:
        return lhs.get() / rhs.get();
        break;
    default:
        break;
    }
    return nullptr;
}

int64_t ParserUtils::constantDeclarationToInteger(const Pascal::ConstantDeclaration* c)
{
    if (auto ci = llvm::dyn_cast<Pascal::IntConstantDeclaration>(c))
    {
        return ci->getValue();
    }
    if (auto cc = llvm::dyn_cast<Pascal::CharConstantDeclaration>(c))
    {
        return cc->getValue();
    }
    if (auto ce = llvm::dyn_cast<Pascal::EnumConstantDeclaration>(c))
    {
        return ce->getValue();
    }
    if (auto cb = llvm::dyn_cast<Pascal::BooleanConstantDeclaration>(c))
    {
        return cb->getValue();
    }
    return -1;
}
bool ParserUtils::numericLiteralIsInteger(Pascal::Token tk)
{
    if(tk.getTokenType() != Pascal::TokenType::NUMERIC_LITERAL){
        return false;
    }
    return (tk.getValue().find(".") != std::string::npos);
}
