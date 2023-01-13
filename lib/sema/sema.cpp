//
// Created by wuser on 21/12/22.
//
#include "japc/sema/sema.h"
#include "japc/sema/typecheck.h"
using namespace Pascal;
SemanticAnalizer::SemanticAnalizer()
{
}
SemanticAnalizer::SemanticAnalizer(const std::shared_ptr<JAPCDiagnostics> &diagnosticsEngine)
    : diagnosticsEngine(diagnosticsEngine)
{
}
void SemanticAnalizer::analize(std::shared_ptr<ExpressionAST> ast)
{
    TypeCheckVisitor typeCheckVisitor(this);
    ast->accept(typeCheckVisitor);
}
