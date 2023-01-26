//
// Created by wuser on 21/12/22.
//

#ifndef JAPC_TYPECHECK_H
#define JAPC_TYPECHECK_H

#include "japc/AST/array_expression.h"
#include "japc/AST/assign_expression.h"
#include "japc/AST/binary_expression.h"
#include "japc/AST/builtin_expression.h"
#include "japc/AST/call_function_expression.h"
#include "japc/AST/case_expression.h"
#include "japc/AST/for_expression.h"
#include "japc/AST/range_expression.h"
#include "japc/AST/read_expression.h"
#include "japc/AST/set_expression.h"
#include "japc/AST/type.h"
#include "japc/AST/visitor.h"
#include "japc/AST/write_expression.h"
#include "japc/AST/integer_expression.h"
#include "japc/AST/string_expression.h"
#include "japc/AST/null_expression.h"
#include "japc/AST/closure_expression.h"
#include "japc/AST/trampoline_expression.h"
#include "japc/AST/function_expression.h"
#include "japc/AST/function.h"

#include "japc/sema/sema.h"
#include "japc/sema/typecheck_utils.h"

#include <vector>

namespace Pascal
{
class TypeCheckVisitor : public ExpressionVisitor
{
  public:
    TypeCheckVisitor(SemanticAnalizer *semanticAnalizer): semanticAnalizer(semanticAnalizer)
    {
    }
    void checkBinaryExpr(BinaryExpression* binaryExpression);
    void checkAssignExpr(AssignExpression* assignExpression);
    void checkRangeExpr(RangeExpression* rangeExpression);
    void checkSetExpr(SetExpression* setExpression);
    void checkArrayExpr(ArrayExpression* arrayExpression);
    void checkBuiltInExpr(BuiltInExpression* builtInExpression);
    void checkCallExpr(CallFunctExpression* callFunctExpression);
    void checkForExpr(ForExpression* forExpression);
    void checkReadExpr(ReadExpression* readExpression);
    void checkWriteExpr(WriteExpression* writeExpression);
    void checkCaseExpr(CaseExpression* caseExpression);
    static std::shared_ptr<RangeDeclaration> getRangeDeclaration(std::shared_ptr<TypeDeclaration> typeDeclaration);
    void visit(ExpressionAST *elem) override;

  private:
    SemanticAnalizer *semanticAnalizer;
};
} // namespace Pascal
#endif // JAPC_TYPECHECK_H
