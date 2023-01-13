#ifndef JAPC_SEMA_H
#define JAPC_SEMA_H
#include "japc/AST/expression.h"
#include "japc/basic/diagnostics.h"
#include <vector>

namespace Pascal
{
class SemanticAnalizer
{
  private:
    int errorCount;
  public:
    std::shared_ptr<JAPCDiagnostics> diagnosticsEngine;

    SemanticAnalizer(const std::shared_ptr<JAPCDiagnostics> &diagnosticsEngine);
    SemanticAnalizer();
    void analize(std::shared_ptr<ExpressionAST> ast);
};
} // namespace Pascal
#endif