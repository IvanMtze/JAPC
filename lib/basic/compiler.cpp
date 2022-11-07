//
// Created by wuser on 11/10/22.
//
#include "japc/basic/compiler.h"
using namespace Pascal;

void Compiler::addBuiltinType(std::string name, std::shared_ptr<TypeDeclaration> type)
{
    this->stack->insert(name, TypeDefinition(name, type, false));
}
void Compiler::initCompiler()
{
    addBuiltinType("integer", getIntegerType());
    addBuiltinType("real", getRealType());
    addBuiltinType("char", getCharType());
    addBuiltinType("text", getCharType());
    addBuiltinType("timestamp", getTimeStampType());
    addBuiltinType("boolean", getBooleanType());
}
Compiler::Status Compiler::compile()
{
    this->scanner = std::make_shared<Scanner>(Scanner());
    this->scanner->setSkipTrivia(true);
    this->parser = std::make_shared<Parser>(this->scanner,std::move(this->diagnosticsEngine));
    for(auto file: sourceManager->getFileNameByType(File::FileType::DEPENDENCY)){
        auto fileReaded = this->sourceManager->getFile(file);
        this->scanner->setFileName(file);
        this->scanner->setTextSource(fileReaded->getContent());
        this->parser->parseFile();
    }
    for(auto file: sourceManager->getFileNameByType(File::FileType::MAIN_PROGRAM)){
        auto fileReaded = this->sourceManager->getFile(file);
        this->scanner->setFileName(file);
        this->scanner->setTextSource(fileReaded->getContent());
        this->parser->parseFile();
    }
    return Compiler::FAILED_WITH_WARNINGS;
}
Compiler::Compiler(ParsedOptions config)
{
    Stack<NamedObject> stack;
    this->diagnosticsEngine = std::unique_ptr<JAPCDiagnostics>(new JAPCDiagnostics());
    this->sourceManager = std::unique_ptr<SourceManager>(new SourceManager());
    this->stack = std::make_shared<Stack<NamedObject>>(stack);
    this->config = config;

    this->diagnosticsEngine->setEmitWarningsAsErrors(this->config.warningOptions.werror);
    this->diagnosticsEngine->setEmitWarnings(this->config.warningOptions.wall);
    for(auto filepath: this->config.files){
        this->sourceManager->insertFile(filepath, File::FileType::MAIN_PROGRAM);
    }
    for(auto filepath: this->config.include){
        this->sourceManager->insertFile(filepath, File::FileType::DEPENDENCY);
    }
    this->initCompiler();
}
