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
    return Compiler::FAILED_WITH_WARNINGS;
}
Compiler::Compiler()
{
    Stack<NamedObject> stack;
    this->stack = std::make_shared<Stack<NamedObject>>(stack);
}
