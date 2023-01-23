#ifndef JAPC_TYPECHECK_UTILS_H
#define JAPC_TYPECHECK_UTILS_H
#include "japc/AST/expression.h"
#include "japc/AST/typecast_expression.h"
#include "japc/AST/type.h"
#include "memory"

class TypecheckUtils
{
  public:
    static std::shared_ptr<Pascal::ExpressionAST> recast(std::shared_ptr<Pascal::ExpressionAST> expressionAst,
                                                         const std::shared_ptr<Pascal::TypeDeclaration> type)
    {
        if (*type != *expressionAst->getTypeDeclaration())
        {
            std::shared_ptr<Pascal::ExpressionAST> e = expressionAst;
            expressionAst = std::make_shared<Pascal::TypeCastExpression>(e->getLocation(), e, type);
        }
        return expressionAst;
    }
    static std::shared_ptr<Pascal::ExpressionAST> recast(Pascal::ExpressionAST *expressionAst,
                                                         const Pascal::TypeDeclaration *type)
    {
        if (*type != *expressionAst->getTypeDeclaration())
        {
            Pascal::ExpressionAST *e = expressionAst;
            Pascal::TypeDeclaration* typeDecl = const_cast<Pascal::TypeDeclaration*>(type);
            expressionAst = new Pascal::TypeCastExpression(e->getLocation(), std::shared_ptr<Pascal::ExpressionAST>(e),
                                                       std::shared_ptr<Pascal::TypeDeclaration>(typeDecl));
        }
        return std::shared_ptr<Pascal::ExpressionAST>(expressionAst);
    }
};
#endif
