//
// Created by wuser on 20/01/23.
//
#include "japc/AST/enum_definition.h"
using namespace Pascal;
EnumDefinition::EnumDefinition(const std::string name, const std::shared_ptr<TypeDeclaration> &typeDeclaration, int value)
    : NamedObject(NamedObjectType::ENUM, name, typeDeclaration), value(value)
{
}
int EnumDefinition::getValue() const
{
    return value;
}
bool EnumDefinition::isClassOf(const NamedObject *namedObject)
{
    return namedObject->getNamedObjectType() == namedObjectType;
}
//used by llvm:isa and dyn_cast, faster and secure casting
bool EnumDefinition::classof(const NamedObject *namedObject){
    return isClassOf(namedObject);
}