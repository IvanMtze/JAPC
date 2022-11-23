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
#include "japc/basic/options_parser.h"
#include "japc/basic/file.h"
#include <memory>
#include <vector>

namespace Pascal
{

class Compiler
{
  private:
    ParsedOptions config;
    std::unique_ptr<SourceManager> sourceManager;
    std::shared_ptr<Stack<std::shared_ptr<NamedObject>>> stack;
    std::shared_ptr<Scanner> scanner;
    std::shared_ptr<Parser> parser;
    std::unique_ptr<JAPCDiagnostics> diagnosticsEngine;
    void initCompiler();
    void addBuiltinType(std::string name, std::shared_ptr<TypeDeclaration> type);
  public:
    Compiler(ParsedOptions config);
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
