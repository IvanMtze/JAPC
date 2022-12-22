//
// Created by wuser on 11/10/22.
//
#include "japc/basic/compiler.h"
using namespace Pascal;

void Compiler::addBuiltinType(std::string name, std::shared_ptr<TypeDeclaration> type)
{
    this->stack->insert(name, std::make_shared<TypeDefinition>(name, type, false));
}
void Compiler::initCompiler()
{
    addBuiltinType("INTEGER", getIntegerType());
    addBuiltinType("LONGINT", getLongIntType());
    addBuiltinType("REAL", getRealType());
    addBuiltinType("CHAR", getCharType());
    addBuiltinType("TEXT", getCharType());
    addBuiltinType("TIMESTAMP", getTimeStampType());
    addBuiltinType("BOOLEAN", getBooleanType());
    // Add boolean values
    this->stack->insert("FALSE", std::make_shared<EnumDefinition>("FALSE", getBooleanType(), 0));
    this->stack->insert("TRUE", std::make_shared<EnumDefinition>("TRUE", getBooleanType(), 0));
}
Compiler::Status Compiler::compile()
{
    this->scanner = std::make_shared<Scanner>(Scanner());
    this->scanner->setSkipTrivia(true);
    this->parser = std::make_shared<Parser>(this->scanner, this->diagnosticsEngine, this->stack);
    for (auto file : sourceManager->getFileNameByType(File::FileType::DEPENDENCY))
    {
        auto fileReaded = this->sourceManager->getFile(file);
        this->scanner->setFileName(file);
        this->scanner->setTextSource(fileReaded->getContent());
        this->parser->parseFile();
    }
    for (auto file : sourceManager->getFileNameByType(File::FileType::MAIN_PROGRAM))
    {
        auto fileReaded = this->sourceManager->getFile(file);
        this->scanner->setFileName(file);
        this->scanner->setTextSource(fileReaded->getContent());
        std::shared_ptr<ExpressionAST> expressionAst = this->parser->parseFile();
        SemanticAnalizer semaAnalizer(this->diagnosticsEngine);
        semaAnalizer.analize(expressionAst);
    }
    return Compiler::FAILED_WITH_WARNINGS;
}
Compiler::Compiler(ParsedOptions config)
{
    this->stack = std::make_shared<Stack<std::shared_ptr<NamedObject>>>();
    this->diagnosticsEngine = std::shared_ptr<JAPCDiagnostics>(new JAPCDiagnostics());
    this->sourceManager = std::unique_ptr<SourceManager>(new SourceManager());
    this->config = config;
    this->diagnosticsEngine->setEmitWarningsAsErrors(this->config.warningOptions.werror);
    this->diagnosticsEngine->setEmitWarnings(this->config.warningOptions.wall);
    for (auto filepath : this->config.files)
    {
        this->sourceManager->insertFile(filepath, File::FileType::MAIN_PROGRAM);
    }
    for (auto filepath : this->config.include)
    {
        this->sourceManager->insertFile(filepath, File::FileType::DEPENDENCY);
    }
    this->initCompiler();
}
