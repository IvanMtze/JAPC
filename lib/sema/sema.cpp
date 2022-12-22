//
// Created by wuser on 21/12/22.
//
#include "japc/sema/sema.h"
using namespace Pascal;
SemanticAnalizer::SemanticAnalizer()
{
}
SemanticAnalizer::SemanticAnalizer(const std::shared_ptr<JAPCDiagnostics> &diagnosticsEngine)
    : diagnosticsEngine(diagnosticsEngine)
{
}
