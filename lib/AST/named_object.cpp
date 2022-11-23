//
// Created by wuser on 15/10/22.
//
#include "japc/AST/named_object.h"
using namespace Pascal;

bool VariableDefinition::isReference() const
{
    return (this->flags & VariableDefinitionFlags::REFERENCE) != VariableDefinitionFlags::NONE;
}

bool VariableDefinition::isExternal() const
{
    return (this->flags & VariableDefinitionFlags::EXTERNAL) != VariableDefinitionFlags::NONE;
}

bool VariableDefinition::isProtected() const
{
    return (this->flags & VariableDefinitionFlags::PROTECTED) != VariableDefinitionFlags::NONE;
}

bool VariableDefinition::isClosure() const
{
    return (this->flags & VariableDefinitionFlags::CLOSURE) != VariableDefinitionFlags::NONE;
}