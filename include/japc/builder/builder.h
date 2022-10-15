//
// Created by wuser on 10/10/22.
//

#ifndef JAPC_BUILDER_H
#define JAPC_BUILDER_H

#include "japc/AST/expression.h"
#include "japc/AST/context.h"
#include "japc/AST/namespace.h"
#include "japc/AST/moduledef.h"
#include "japc/AST/type.h"

namespace Pascal
{
class FuncCall;
class StrConst;
class IntConst;
class FloatConst;
class VoidConst;
}
namespace Builder
{
class Builder
{
  protected:
    int optimizeLevel;

  public:
    Builder() : optimizeLevel(0)
    {
    }
    virtual Builder *createChildBuilder() = 0;

    virtual Pascal::ResultExpr *emitFuncCall(Pascal::Context &context, Pascal::FuncCall *funcCall) = 0;
    virtual Pascal::ResultExpr *emitStrConst(Pascal::Context &context, Pascal::StrConst *strConst) = 0;
    virtual Pascal::ResultExpr *emitIntConst(Pascal::Context &context, Pascal::IntConst *val) = 0;
    virtual Pascal::ResultExpr *emitFloatConst(Pascal::Context &context, Pascal::FloatConst *var) = 0;
    virtual Pascal::ResultExpr *emitNull(Pascal::Context &context, Pascal::VoidConst *var) = 0;

};
} // namespace Builder
#endif // JAPC_BUILDER_H
