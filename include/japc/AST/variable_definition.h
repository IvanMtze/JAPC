//
// Created by wuser on 20/01/23.
//

#ifndef JAPC_VARIABLE_DEFINITION_H
#define JAPC_VARIABLE_DEFINITION_H

#include "japc/AST/named_object.h"
#include "japc/AST/type.h"
namespace Pascal
{
class VariableDefinition : public NamedObject
{
  public:
    VariableDefinition(const std::string &name, const std::shared_ptr<TypeDeclaration> &typeDeclaration,
                       VariableDefinitionFlags flags = VariableDefinitionFlags::NONE);
    bool isReference() const;
    bool isExternal() const;
    bool isProtected() const;
    bool isClosure() const;
    std::shared_ptr<InitValue> getInitValue();
    void setInitValue(const std::shared_ptr<InitValue> &initValue);
    static bool isClassOf(NamedObject *namedObject);
    static bool isClassOf(const NamedObject *namedObject);
    // used by llvm:isa and dyn_cast, faster and secure casting
    static bool classof(const NamedObject *namedObject);

  private:
    VariableDefinitionFlags flags;
    std::shared_ptr<InitValue> initValue;
    const static NamedObjectType namedObjectType = NamedObjectType::VARIABLE;
};
bool inline operator<(const VariableDefinition lhs, const VariableDefinition rhs)
{
    return lhs.getName() < rhs.getName();
}
constexpr inline VariableDefinitionFlags operator&(const VariableDefinitionFlags lhs, const VariableDefinitionFlags rhs)
{
    return static_cast<VariableDefinitionFlags>(static_cast<const int>(lhs) & static_cast<const int>(rhs));
}

constexpr inline VariableDefinitionFlags operator|(const VariableDefinitionFlags lhs, const VariableDefinitionFlags rhs)
{
    return static_cast<VariableDefinitionFlags>(static_cast<const int>(lhs) | static_cast<const int>(rhs));
}

constexpr inline VariableDefinitionFlags operator|=(VariableDefinitionFlags lhs, const VariableDefinitionFlags rhs)
{
    lhs = lhs | rhs;
    return lhs;
}

} // namespace Pascal
#endif // JAPC_VARIABLE_DEFINITION_H
