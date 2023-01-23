//
// Created by wuser on 20/01/23.
//
#include "japc/AST/member_function_definition.h"
using namespace Pascal;
MemberFunctionDefinition::MemberFunctionDefinition(const std::string &name,
                                                   const std::shared_ptr<TypeDeclaration> &typeDeclaration, int index)
    : NamedObject(NamedObjectType::MEMBER_FUNCTION, name, typeDeclaration), index(index)
{
}
int MemberFunctionDefinition::getIndex() const
{
    return index;
}
bool MemberFunctionDefinition::isClassOf(NamedObject *namedObject)
{
    return namedObject->getNamedObjectType() == namedObjectType;
}
bool MemberFunctionDefinition::isClassOf(const NamedObject *namedObject)
{
    return namedObject->getNamedObjectType() == namedObjectType;
}
// used by llvm:isa and dyn_cast, faster and secure casting
bool MemberFunctionDefinition::classof(const NamedObject *namedObject)
{
    return isClassOf(namedObject);
}