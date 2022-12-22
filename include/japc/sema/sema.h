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
    std::shared_ptr<JAPCDiagnostics> diagnosticsEngine;

  public:
    explicit SemanticAnalizer(const std::shared_ptr<JAPCDiagnostics> &diagnosticsEngine);

  public:
    SemanticAnalizer();
    virtual ~SemanticAnalizer();
    void analize(std::shared_ptr<ExpressionAST> ast);
};
} // namespace Pascal
#endif