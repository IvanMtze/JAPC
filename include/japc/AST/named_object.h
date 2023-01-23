//
// Created by wuser on 14/10/22.
//

#ifndef JAPC_NAMED_OBJECT_H
#define JAPC_NAMED_OBJECT_H

#include "japc/AST/expression.h"
#include "japc/AST/type.h"
#include <memory>

namespace Pascal
{
class InitValue;
class ExpressionAST;
class PrototypeExpression;
class ConstantDeclaration;
enum class VariableDefinitionFlags
{
    REFERENCE = 1 << 0,
    EXTERNAL = 1 << 1,
    PROTECTED = 1 << 2,
    CLOSURE = 1 << 3,
    NONE = 0,
    ALL = REFERENCE | EXTERNAL | PROTECTED | CLOSURE,
};

enum NamedObjectType
{
    VARIABLE,
    FUNCTION,
    TYPE,
    CONST,
    ENUM,
    BUILTIN,
    WITH,
    MEMBER_FUNCTION,
    LABEL
};
class NamedObject
{
  public:
    NamedObject(NamedObjectType namedObjectType, const std::string &name,
                const std::shared_ptr<TypeDeclaration> &typeDeclaration)
        : namedObjectType(namedObjectType), name(name), typeDeclaration(typeDeclaration)
    {
    }
    virtual ~NamedObject()
    {
    }
    NamedObjectType getNamedObjectType() const
    {
        return namedObjectType;
    }
    void setNamedObjectType(NamedObjectType namedObjectType)
    {
        NamedObject::namedObjectType = namedObjectType;
    }
    const std::string &getName() const
    {
        return name;
    }
    void setName(const std::string &name)
    {
        NamedObject::name = name;
    }
    const std::shared_ptr<TypeDeclaration> &getTypeDeclaration() const
    {
        return typeDeclaration;
    }
    void setTypeDeclaration(const std::shared_ptr<TypeDeclaration> &typeDeclaration)
    {
        NamedObject::typeDeclaration = typeDeclaration;
    }

  private:
    NamedObjectType namedObjectType;
    std::string name;
    std::shared_ptr<TypeDeclaration> typeDeclaration;
};

} // namespace Pascal
#endif // JAPC_NAMED_OBJECT_H
