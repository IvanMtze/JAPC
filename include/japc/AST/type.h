//
// Created by wuser on 12/10/22.
//

#ifndef JAPC_TYPE_H
#define JAPC_TYPE_H

#include <llvm/IR/DIBuilder.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Type.h>

namespace Pascal
{
class TypeDeclaration;
std::shared_ptr<TypeDeclaration> getIntegerType();
std::shared_ptr<TypeDeclaration> getLongIntType();
std::shared_ptr<TypeDeclaration> getCharType();
std::shared_ptr<TypeDeclaration> getBooleanType();
std::shared_ptr<TypeDeclaration> getRealType();
std::shared_ptr<TypeDeclaration> getVoidType();
std::shared_ptr<TypeDeclaration> getTextType();
std::shared_ptr<TypeDeclaration> getStringType();
std::shared_ptr<TypeDeclaration> getTimeStampType();

enum TypeKind
{
    TYPE_TYPE,
    TYPE_CHAR,
    TYPE_INTEGER,
    TYPE_LONG_INT,
    TYPE_REAL,
    TYPE_VOID,
    TYPE_ARRAY,
    TYPE_STRING,
    TYPE_LAST_ARRAY,
    TYPE_RANGE,
    TYPE_ENUM,
    TYPE_BOOLEAN,
    TYPE_POINTER,
    TYPE_FIELD,
    TYPE_RECORD,
    TYPE_FUNCION_POINTER,
    TYPE_FUNCTION,
    TYPE_FILE,
    TYPE_TEXT,
    TYPE_SET,
    TYPE_VRIANT,
    TYPE_CLASS,
    TYPE_MEMBER_FUNCTION,
    TYPE_FORWARD,
};

class Range
{
  public:
    Range(int start, int anEnd) : start(start), end(anEnd)
    {
    }
    int getStart() const
    {
        return start;
    }
    void setStart(int start)
    {
        Range::start = start;
    }
    int getEnd() const
    {
        return end;
    }
    void setEnd(int End)
    {
        end = End;
    }
    int getSize()
    {
        return end - start + 1;
    }

  private:
    int start;
    int end;
};
class TypeDeclaration
{
  public:
    TypeDeclaration(TypeKind kind) : kind(kind), ltype(0), diType(0), name("")
    {
    }
    virtual TypeKind getTypeKind() const
    {
        return kind;
    }
    virtual ~TypeDeclaration()
    {
    }
    virtual bool isIncomplete() const
    {
        return false;
    }
    virtual bool isIntegral() const
    {
        return false;
    }
    virtual bool isStringLike() const
    {
        return false;
    }
    virtual bool isUnsigned() const
    {
        return false;
    }
    virtual std::shared_ptr<Range> getRange() const;
    virtual std::shared_ptr<TypeDeclaration> getSubtype() const
    {
        return 0;
    }
    virtual unsigned bits() const
    {
        return 0;
    }
    virtual bool isSameAs(const TypeDeclaration *typeDeclaration) const = 0;
    virtual const TypeDeclaration *isCompatibleType(const TypeDeclaration *ty) const;
    virtual const TypeDeclaration *isAssignableType(const TypeDeclaration *ty) const
    {
        return isCompatibleType(ty);
    }
    const TypeKind getKind() const
    {
        return kind;
    }
    virtual bool hasLlvmType() const = 0;
    virtual size_t getSize() const  ;
    size_t getAllignSize() const;
    const std::shared_ptr<llvm::Type> &getLtype() const
    {
        return ltype;
    }
    void setLtype(const std::shared_ptr<llvm::Type> &ltype)
    {
        TypeDeclaration::ltype = ltype;
    }
    const std::shared_ptr<llvm::DIType> &getDiType1() const
    {
        return diType;
    }
    void setDiType(const std::shared_ptr<llvm::DIType> &diType)
    {
        TypeDeclaration::diType = diType;
    }
    const std::string &getName() const
    {
        return name;
    }
    void setName(const std::string &name)
    {
        TypeDeclaration::name = name;
    }

  protected:
    virtual std::shared_ptr<llvm::Type> getLlvmType() const = 0;
    virtual std::shared_ptr<llvm::DIType> getDiType(llvm::DIType *builder) const = 0;
    const TypeKind kind;
    mutable std::shared_ptr<llvm::Type> ltype;
    mutable std::shared_ptr<llvm::DIType> diType;
    std::string name;
};

class ForwardDeclaration : TypeDeclaration
{
};

class BaseTypeDeclaration : public TypeDeclaration
{
};

class RealDeclaration : public BaseTypeDeclaration
{
};

class VoidDeclaration : public BaseTypeDeclaration
{
};

class CompoundDeclaration : public TypeDeclaration
{
};

class ArrayDeclaration : public CompoundDeclaration
{
};

class EnumDeclaration : public CompoundDeclaration
{
};

class BoolDeclaration : public EnumDeclaration
{
};

class PointerDeclaration : public CompoundDeclaration
{
};

class FunctionDeclaration : public CompoundDeclaration
{
};

class FieldDeclaration : public CompoundDeclaration
{
};

class FieldCollection : public TypeDeclaration
{
};

class VariantDeclaration : public FieldCollection
{
};
class RecordDeclaration : public FieldCollection
{
};
class MemberFunctionDeclaration : public TypeDeclaration
{
};

class FunctionPointerDeclaration : public CompoundDeclaration
{
};

class FileDeclaration : public CompoundDeclaration
{
};

class TextDeclaration : public FileDeclaration
{
};

class SetDeclaration : public CompoundDeclaration
{
};
class StringDeclaration : public ArrayDeclaration
{
};
class RangeDeclaration
{
};
} // namespace Pascal
#endif // JAPC_TYPE_H
