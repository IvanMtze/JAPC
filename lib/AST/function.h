//
// Created by wuser on 24/09/22.
//

#ifndef JAPC_FUNCTION_H
#define JAPC_FUNCTION_H
#include <string>
#include <vector>
#include "context.h"
#include "type.h"

namespace Pascal
{
class FunctionParameter{
  public:
    std::string identifier;
    Types type;
    std::unique_ptr<AccessModifier> accessModifier;
};
class Function
{
  public:
    std::string identifier;
    std::unique_ptr<std::vector<std::unique_ptr<FunctionParameter>>> paramList;
    std::unique_ptr<FunctionContext> functionContext;
    Types returnType;
};
} // namespace Pascal
#endif // JAPC_FUNCTION_H
