//
// Created by wuser on 20/01/23.
//
#include "japc/AST/with_definition.h"
using namespace Pascal;
WithDefinition::WithDefinition(const std::string &name, const std::shared_ptr<TypeDeclaration> &typeDeclaration,
                               const std::shared_ptr<ExpressionAST> &expression)
    : NamedObject(NamedObjectType::WITH, name, typeDeclaration), expression(expression)
{
}
const std::shared_ptr<ExpressionAST> &WithDefinition::getExpression() const
{
    return expression;
}
bool WithDefinition::isClassOf(NamedObject *namedObject)
{
    return namedObject->getNamedObjectType() == namedObjectType;
}
bool WithDefinition::isClassOf(const NamedObject *namedObject)
{
    return namedObject->getNamedObjectType() == namedObjectType;
}
// used by llvm:isa and dyn_cast, faster and secure casting
bool WithDefinition::classof(const NamedObject *namedObject)
{
    return isClassOf(namedObject);
}