//
// Created by wuser on 20/01/23.
//
#include "japc/AST/constant_definition.h"
using namespace Pascal;
ConstantDefinition::ConstantDefinition(const std::string& nm, const std::shared_ptr<ConstantDeclaration> cv)
    : NamedObject(NamedObjectType::CONST, nm, 0), constValue(cv)
{
}
bool ConstantDefinition::isClassOf(NamedObject *namedObject)
{
    return namedObject->getNamedObjectType() == namedObjectType;
}
bool ConstantDefinition::isClassOf(const NamedObject *namedObject)
{
    return namedObject->getNamedObjectType() == namedObjectType;
}
//used by llvm:isa and dyn_cast, faster and secure casting
bool ConstantDefinition::classof(const NamedObject *namedObject){
    return isClassOf(namedObject);
}
const std::shared_ptr<ConstantDeclaration> ConstantDefinition::getConstValue() const
{
    return constValue;
}