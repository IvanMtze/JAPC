//
// Created by wuser on 15/10/22.
//
#include "japc/AST/named_object.h"
using namespace Pascal;
constexpr VariableDefinitionFlags operator&(const VariableDefinitionFlags lhs, const VariableDefinitionFlags rhs)
{
    return static_cast<VariableDefinitionFlags>(static_cast<const int>(lhs) & static_cast<const int>(rhs));
}

constexpr VariableDefinitionFlags operator|(const VariableDefinitionFlags lhs, const VariableDefinitionFlags rhs)
{
    return static_cast<VariableDefinitionFlags>(static_cast<const int>(lhs) | static_cast<const int>(rhs));
}

constexpr VariableDefinitionFlags operator|=(VariableDefinitionFlags lhs, const VariableDefinitionFlags rhs)
{
    lhs = lhs | rhs;
    return lhs;
}

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

bool operator<(const VariableDefinition &lhs, const VariableDefinition &rhs)
{
    return lhs.getName() < rhs.getName();
}