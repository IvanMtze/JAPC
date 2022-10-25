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

};
} // namespace Builder
#endif // JAPC_BUILDER_H
