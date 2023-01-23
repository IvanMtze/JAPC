//
// Created by wuser on 20/01/23.
//
#include "japc/AST/label_definition.h"
using namespace Pascal;
LabelDefinition::LabelDefinition(const int name) : NamedObject(NamedObjectType::LABEL, std::to_string(name), nullptr)
{
}
bool LabelDefinition::isClassOf(NamedObject *namedObject)
{
    return namedObject->getNamedObjectType() == namedObjectType;
}
bool LabelDefinition::isClassOf(const NamedObject *namedObject)
{
    return namedObject->getNamedObjectType() == namedObjectType;
}
//used by llvm:isa and dyn_cast, faster and secure casting
bool LabelDefinition::classof(const NamedObject *namedObject){
    return isClassOf(namedObject);
}