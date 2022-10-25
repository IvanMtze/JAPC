//
// Created by wuser on 10/10/22.
//

#ifndef JAPC_COMPILER_H
#define JAPC_COMPILER_H

#include "japc/AST/named_object.h"
#include "japc/AST/stack.h"
#include "japc/builder/builder.h"
#include "japc/parser/parser.h"
#include "japc/scanner/scanner.h"
#include "japc/AST/type.h"

#include <memory>
#include <vector>

namespace Pascal
{

class Compiler
{
  private:
    std::shared_ptr<Stack<NamedObject>> stack;
    std::shared_ptr<Scanner> scanner;
    std::shared_ptr<Parser> parser;
    void initCompiler();
    void addBuiltinType(std::string name, std::shared_ptr<TypeDeclaration> type);
  public:
    Compiler();
    enum Status
    {
        FAILED_WITH_ERROR,
        FAILED_WITH_WARNINGS,
        FAILED_UNKNOWN,
        COMPILED_WITH__WARNINGS,
        COMPILED_SUCCESS,
    };
    Status compile();
};
} // namespace Pascal
#endif // JAPC_COMPILER_H
