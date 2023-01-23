//
// Created by wuser on 20/01/23.
//
#include "japc/AST/variable_definition.h"
using namespace Pascal;

VariableDefinition::VariableDefinition(const std::string &name, const std::shared_ptr<TypeDeclaration> &typeDeclaration,
                   VariableDefinitionFlags flags)
    : NamedObject(NamedObjectType::VARIABLE, name, typeDeclaration), flags(flags), initValue(0)
{
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

std::shared_ptr<InitValue> VariableDefinition::getInitValue()
{
    return this->initValue;
}
void VariableDefinition::setInitValue(const std::shared_ptr<InitValue> &initValue)
{
    VariableDefinition::initValue = initValue;
}
bool VariableDefinition::isClassOf(NamedObject *namedObject)
{
    return namedObject->getNamedObjectType() == namedObjectType;
}
bool VariableDefinition::isClassOf(const NamedObject *namedObject)
{
    return namedObject->getNamedObjectType() == namedObjectType;
}
// used by llvm:isa and dyn_cast, faster and secure casting
bool VariableDefinition::classof(const NamedObject *namedObject)
{
    return isClassOf(namedObject);
}