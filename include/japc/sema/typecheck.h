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
    void checkBinaryExpr(std::shared_ptr<BinaryExpression> binaryExpression);
    void checkAssignExpr(std::shared_ptr<AssignExpression> assignExpression);
    void checkRangeExpr(std::shared_ptr<RangeExpression> rangeExpression);
    void checkSetExpr(std::shared_ptr<SetExpression> setExpression);
    void checkArrayExpr(std::shared_ptr<ArrayExpression> arrayExpression);
    void checkBuiltInExpr(std::shared_ptr<BuiltInExpression> builtInExpression);
    void checkCallExpr(std::shared_ptr<CallFunctExpression> callFunctExpression);
    void checkForExpr(std::shared_ptr<ForExpression> forExpression);
    void checkReadExpr(std::shared_ptr<ReadExpression> readExpression);
    void checkWriteExpr(std::shared_ptr<WriteExpression> writeExpression);
    void checkCaseExpr(std::shared_ptr<CaseExpression> caseExpression);
    static std::shared_ptr<RangeDeclaration> getRangeDeclaration(std::shared_ptr<TypeDeclaration> typeDeclaration);
    void visit(ExpressionAST *elem) override;

  private:
    SemanticAnalizer *semanticAnalizer;
};
} // namespace Pascal
#endif // JAPC_TYPECHECK_H
