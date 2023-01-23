//
// Created by wuser on 20/01/23.
//

#ifndef JAPC_TYPE_DEFINITION_H
#define JAPC_TYPE_DEFINITION_H

namespace Pascal{
class TypeDefinition : public NamedObject
{
  public:
    TypeDefinition(const std::string &name, const std::shared_ptr<TypeDeclaration> &typeDeclaration, bool restricted)
        : NamedObject(NamedObjectType::TYPE, name, typeDeclaration), restricted(restricted)
    {
    }
    bool isRestricted() const
    {
        return restricted;
    }
    static bool isClassOf(NamedObject *namedObject)
    {
        return namedObject->getNamedObjectType() == namedObjectType;
    }
    static bool isClassOf(const NamedObject *namedObject)
    {
        return namedObject->getNamedObjectType() == namedObjectType;
    }
    //used by llvm:isa and dyn_cast, faster and secure casting
    static bool classof(const NamedObject *namedObject){
        return isClassOf(namedObject);
    }
  private:
    bool restricted;
    const static NamedObjectType namedObjectType = NamedObjectType::TYPE;
};
}
#endif // JAPC_TYPE_DEFINITION_H
