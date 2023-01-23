//
// Created by wuser on 20/01/23.
//
#include "japc/AST/function_definition.h"
using namespace Pascal;
FunctionDefinition::FunctionDefinition(const std::string &name, const std::shared_ptr<TypeDeclaration> &typeDeclaration,
                   const std::shared_ptr<PrototypeExpression> &prototype)
    : NamedObject(NamedObjectType::FUNCTION, name, typeDeclaration), prototype(prototype)
{
}
const std::shared_ptr<PrototypeExpression> &FunctionDefinition::getPrototype() const
{
    return prototype;
}
 bool FunctionDefinition::isClassOf(NamedObject *namedObject)
{
    return namedObject->getNamedObjectType() == namedObjectType;
}
bool FunctionDefinition::isClassOf(const NamedObject *namedObject)
{
    return namedObject->getNamedObjectType() == namedObjectType;
}
//used by llvm:isa and dyn_cast, faster and secure casting
bool FunctionDefinition::classof(const NamedObject *namedObject){
    return isClassOf(namedObject);
}
bool FunctionDefinition::classof(const std::shared_ptr<NamedObject> namedObject){
    return isClassOf(namedObject.get());
}