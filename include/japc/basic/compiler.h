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
#include "japc/AST/enum_definition.h"
#include "japc/AST/type_definition.h"
#include "japc/basic/options_parser.h"
#include "japc/basic/file.h"
#include "japc/sema/sema.h"
#include <memory>
#include <vector>
#include <llvm/Analysis/Passes.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/Support/CommandLine.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Transforms/IPO.h>
#include <llvm/Transforms/InstCombine/InstCombine.h>
#include <llvm/Transforms/Scalar.h>
#include <llvm/Transforms/Scalar/GVN.h>
#include <llvm/Transforms/Utils.h>

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
    std::shared_ptr<JAPCDiagnostics> diagnosticsEngine;
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
