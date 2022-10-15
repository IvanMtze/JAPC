//
// Created by wuser on 14/10/22.
//

#ifndef JAPC_NAMED_OBJECT_H
#define JAPC_NAMED_OBJECT_H

#include "japc/AST/expression.h"
#include "japc/AST/type.h"

namespace Pascal
{
class InitValue;
class PrototypeExpression;
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

class VariableDefinition : public NamedObject
{
  public:
    VariableDefinition(const std::string &name, const std::shared_ptr<TypeDeclaration> &typeDeclaration,
                       VariableDefinitionFlags flags = VariableDefinitionFlags::NONE)
        : NamedObject(NamedObjectType::VARIABLE, name, typeDeclaration), flags(flags), initValue(0)
    {
    }
    bool isReference() const;
    bool isExternal() const;
    bool isProtected() const;
    bool isClosure() const;
    std::shared_ptr<InitValue> getInitValue()
    {
        return this->initValue;
    }
    void setInitValue(const std::shared_ptr<InitValue> &initValue)
    {
        VariableDefinition::initValue = initValue;
    }
    static bool isClassOf(NamedObject *namedObject)
    {
        return namedObject->getNamedObjectType() == NamedObjectType::VARIABLE;
    }

  private:
    VariableDefinitionFlags flags;
    std::shared_ptr<InitValue> initValue;
};

class FunctionDefinition :public  NamedObject
{
  public:
    FunctionDefinition(const std::string &name, const std::shared_ptr<TypeDeclaration> &typeDeclaration,
                       const std::shared_ptr<PrototypeExpression> &prototype)
        : NamedObject(NamedObjectType::FUNCTION, name, typeDeclaration), prototype(prototype)
    {
    }
    const std::shared_ptr<PrototypeExpression> &getPrototype() const
    {
        return prototype;
    }
    static bool isClassOf(NamedObject *namedObject)
    {
        return namedObject->getNamedObjectType() == NamedObjectType::FUNCTION;
    }

  private:
    std::shared_ptr<PrototypeExpression> prototype;
};

class TypeDefinition : public NamedObject
{
  public:
    TypeDefinition(const std::string &name, const std::shared_ptr<TypeDeclaration> &typeDeclaration, bool restricted)
        : NamedObject(NamedObjectType::TYPE, name, typeDeclaration), restricted(restricted)
    {
    }
    static bool isClassOf(NamedObject *namedObject)
    {
        namedObject->getNamedObjectType() == NamedObjectType::TYPE;
    }
    bool isRestricted() const
    {
        return restricted;
    }

  private:
    bool restricted;
};

class ConstDefinition : public NamedObject
{
};

class EnumDefinition : public NamedObject
{
  public:
    EnumDefinition(const std::string &name, const std::shared_ptr<TypeDeclaration> &typeDeclaration, int value)
        : NamedObject(NamedObjectType::ENUM, name, typeDeclaration), value(value)
    {
    }
    int getValue() const
    {
        return value;
    }
    static bool isClassOf(NamedObject *namedObject)
    {
        return namedObject->getNamedObjectType() == NamedObjectType::ENUM;
    }

  private:
    int value;
};

class WithDefinition : public NamedObject
{
  public:
    WithDefinition(const std::string &name, const std::shared_ptr<TypeDeclaration> &typeDeclaration,
                   const std::shared_ptr<ExpressionAST> &expression)
        : NamedObject(NamedObjectType::WITH, name, typeDeclaration), expression(expression)
    {
    }
    const std::shared_ptr<ExpressionAST> &getExpression() const
    {
        return expression;
    }
    static bool isClassOf(NamedObject *namedObject)
    {
        return namedObject->getNamedObjectType() == NamedObjectType::WITH;
    }

  private:
    std::shared_ptr<ExpressionAST> expression;
};

class MemberFunctionDefinition : public NamedObject
{
  public:
    MemberFunctionDefinition(const std::string &name, const std::shared_ptr<TypeDeclaration> &typeDeclaration,
                             int index)
        : NamedObject(NamedObjectType::MEMBER_FUNCTION, name, typeDeclaration), index(index)
    {
    }
    int getIndex() const
    {
        return index;
    }
    static bool isClassOf(NamedObject *namedObject)
    {
        return namedObject->getNamedObjectType() == NamedObjectType::WITH;
    }

  private:
    int index;
};

class LabelDefinition : public NamedObject
{
  public:
    LabelDefinition(const int name) : NamedObject(NamedObjectType::LABEL, std::to_string(name), nullptr)
    {
    }
    static bool isClassOf(NamedObject *namedObject)
    {
        return namedObject->getNamedObjectType() == NamedObjectType::LABEL;
    }
};
} // namespace Pascal
#endif // JAPC_NAMED_OBJECT_H
