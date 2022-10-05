//
// Created by wuser on 24/09/22.
//

#ifndef JAPC_PROGRAM_H
#define JAPC_PROGRAM_H
#include "expression.h"
#include "function.h"
#include "procedure.h"
#include "vector"
namespace Pascal
{
class Program
{
  public:
    std::vector<std::unique_ptr<Function>> functions;
    std::vector<std::unique_ptr<Procedure>> procedures;
    std::string name;
    std::unique_ptr<ProgramContext> programContext;

};

class Module
{
};
} // namespace Pascal
#endif // JAPC_PROGRAM_H
