//
// Created by wuser on 12/10/22.
//

#ifndef JAPC_TYPE_H
#define JAPC_TYPE_H

#include <japc/AST/expression.h>
#include <japc/scanner/scanner.h>
#include <llvm/IR/DIBuilder.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Type.h>
#include <memory>

#define __MAX_SET_WORDS__ 16
#define __SET_BITS__ 32
#define __MAX__SET__SIZE__ (__MAX_SET_WORDS__ * __SET_BITS__)
#define __SET_MASK__ (__SET_BITS__ - 1)
#define __SET_POW_TO_BITS__ 5
namespace Pascal
{

enum ConstantsOperatorError{
    TYPES_NOT_COMPATIBLE
};
class PrototypeExpression;
class ExpressionAST;
class TypeDeclaration;
class RangeDeclaration;
class FunctionPointerDeclaration;

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

enum Access
{
    Private,
    Protected,
    Public
};

enum
{
    Handle,
    Buffer,
    RecordSize,
    IsText
} FileFields;

std::shared_ptr<TypeDeclaration> getIntegerType();
std::shared_ptr<TypeDeclaration> getLongIntType();
std::shared_ptr<TypeDeclaration> getCharType();
std::shared_ptr<TypeDeclaration> getBooleanType();
std::shared_ptr<TypeDeclaration> getRealType();
std::shared_ptr<TypeDeclaration> getVoidType();
std::shared_ptr<TypeDeclaration> getTextType();
std::shared_ptr<TypeDeclaration> getStringType();
std::shared_ptr<TypeDeclaration> getTimeStampType();

enum ConstantKind
{
    CONSTANT_DECLARATION,
    INT_CONSTANT_DECLARATION,
    ENUM_CONSTANT_DECLARATION,
    REAL_CONSTANT_DECLARATION,
    CHAR_CONSTANT_DECLARATION,
    BOOL_CONSTANT_DECLARATION,
    STRING_CONSTANT_DECLARATION,
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

class TypeDeclaration : public std::enable_shared_from_this<TypeDeclaration>
{
  public:
    TypeDeclaration(TypeKind kind) : kind(kind), ltype(0), name("")
    {
    }
    virtual TypeKind getTypeKind() const
    {
        return kind;
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
    virtual bool isCompound() const
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
    virtual size_t getSize() const;
    size_t getAllignSize() const;
    const std::shared_ptr<llvm::Type> &getLtype() const
    {
        return ltype;
    }
    void setLtype(const std::shared_ptr<llvm::Type> &ltype)
    {
        TypeDeclaration::ltype = ltype;
    }
    const std::string &getName() const
    {
        return name;
    }
    void setName(const std::string &name)
    {
        TypeDeclaration::name = name;
    }
    void setInitialValue(std::shared_ptr<ExpressionAST> i)
    {
    }
    virtual std::shared_ptr<TypeDeclaration> clone() const
    {
        return 0;
    }
    const std::shared_ptr<ExpressionAST> &getInitial() const;
    void setInitial(const std::shared_ptr<ExpressionAST> &initial);

  protected:
    virtual std::shared_ptr<llvm::Type> getLlvmType() const = 0;
    const TypeKind kind;
    mutable std::shared_ptr<llvm::Type> ltype;
    std::string name;
    std::shared_ptr<ExpressionAST> initial;
};

class ForwardDeclaration : public TypeDeclaration
{
  public:
    virtual TypeKind getTypeKind() const;
    virtual bool isIntegral() const;
    virtual bool isStringLike() const;
    virtual bool isUnsigned() const;
    virtual bool isCompound() const;
    virtual std::shared_ptr<Range> getRange() const;
    virtual std::shared_ptr<TypeDeclaration> getSubtype() const;
    virtual unsigned int bits() const;
    virtual const TypeDeclaration *isCompatibleType(const TypeDeclaration *ty) const;
    virtual const TypeDeclaration *isAssignableType(const TypeDeclaration *ty) const;
    virtual size_t getSize() const;
    virtual std::shared_ptr<TypeDeclaration> clone() const;
    bool isIncomplete() const override
    {
        return true;
    }
    bool hasLlvmType() const override
    {
        return false;
    }
    bool isSameAs(const TypeDeclaration *ty) const override
    {
        return false;
    }

  protected:
    std::shared_ptr<llvm::Type> getLlvmType() const override
    {
        return 0;
    }
};

class BaseTypeDeclaration : public TypeDeclaration
{
  public:
    BaseTypeDeclaration(TypeKind tk) : TypeDeclaration(tk)
    {
    }
    bool isSameAs(const TypeDeclaration *ty) const override
    {
        return kind == ty->getTypeKind();
    }
    bool hasLlvmType() const override
    {
        return false;
    }

  protected:
    std::shared_ptr<llvm::Type> getLlvmType() const override
    {
        return std::shared_ptr<llvm::Type>();
    }
};

class RealDeclaration : public BaseTypeDeclaration
{
  public:
    RealDeclaration() : BaseTypeDeclaration(TypeKind::TYPE_REAL)
    {
    }
    const TypeDeclaration *isCompatibleType(const TypeDeclaration *ty) const override
    {
        if (isSameAs(ty) || ty->getTypeKind() == TypeKind::TYPE_INTEGER || ty->getTypeKind() == TypeKind::TYPE_LONG_INT)
        {
            return this;
        }
        return nullptr;
    }
    const TypeDeclaration *isAssignableType(const TypeDeclaration *ty) const override
    {
        return isCompatibleType(ty);
    }
    unsigned bits() const override
    {
        return 64;
    }
    bool hasLlvmType() const override
    {
        return true;
    }
    static bool isClassOf(const TypeDeclaration *e)
    {
        return e->getKind() == TypeKind::TYPE_REAL;
    }
    static bool classof(const TypeDeclaration *e)
    {
        return isClassOf(e);
    }
    bool isSameAs(const TypeDeclaration *ty) const override
    {
        return BaseTypeDeclaration::isSameAs(ty);
    }

  protected:
    std::shared_ptr<llvm::Type> getLlvmType() const override
    {
        return BaseTypeDeclaration::getLlvmType();
    }
};

template <int bit, TypeKind tk> class IntegerXDecl : public BaseTypeDeclaration
{
  public:
    IntegerXDecl() : BaseTypeDeclaration(tk)
    {
    }
    bool isIntegral() const override
    {
        return true;
    }
    unsigned bits() const override
    {
        return bit;
    }
    const TypeDeclaration *isCompatibleType(const TypeDeclaration *ty) const override
    {
        if (ty->getKind() == tk)
        {
            return this;
        }
        return nullptr;
    }
    const TypeDeclaration *isAssignableType(const TypeDeclaration *ty) const override
    {
        if (ty->getKind() == tk)
        {
            return this;
        }
        return nullptr;
    }
    static bool classof(const TypeDeclaration *e)
    {
        return e->getKind() == tk;
    }

  protected:
    std::shared_ptr<llvm::Type> getLlvmType() const override
    {
    }
};

using IntegerDeclaration = IntegerXDecl<32, TypeKind::TYPE_INTEGER>;
using LongIntegerDeclaration = IntegerXDecl<64, TypeKind::TYPE_LONG_INT>;
class CharDeclaration : public BaseTypeDeclaration
{
  public:
    CharDeclaration() : BaseTypeDeclaration(TypeKind::TYPE_CHAR)
    {
    }
    bool isIntegral() const override
    {
        return true;
    }
    bool isUnsigned() const override
    {
        return true;
    }
    bool isStringLike() const override
    {
        return true;
    }
    const TypeDeclaration *isCompatibleType(const TypeDeclaration *ty) const override;
    const TypeDeclaration *isAssignableType(const TypeDeclaration *ty) const override;
    unsigned bits() const override
    {
        return 8;
    }
    bool hasLlvmType() const override
    {
        return true;
    }
    static bool classof(const TypeDeclaration *e)
    {
        return e->getKind() == TypeKind::TYPE_CHAR;
    }
    bool isSameAs(const TypeDeclaration *ty) const override
    {
        return BaseTypeDeclaration::isSameAs(ty);
    }
    virtual TypeKind getTypeKind() const;
    virtual bool isIncomplete() const;
    virtual bool isCompound() const;
    virtual std::shared_ptr<Range> getRange() const;
    virtual std::shared_ptr<TypeDeclaration> getSubtype() const;
    virtual size_t getSize() const;
    virtual std::shared_ptr<TypeDeclaration> clone() const;

  protected:
    std::shared_ptr<llvm::Type> getLlvmType() const override;
};

class VoidDeclaration : public BaseTypeDeclaration
{
  public:
    virtual TypeKind getTypeKind() const;
    virtual bool isIncomplete() const;
    virtual bool isIntegral() const;
    virtual bool isStringLike() const;
    virtual bool isUnsigned() const;
    virtual bool isCompound() const;
    virtual std::shared_ptr<Range> getRange() const;
    virtual std::shared_ptr<TypeDeclaration> getSubtype() const;
    virtual unsigned int bits() const;
    virtual const TypeDeclaration *isAssignableType(const TypeDeclaration *ty) const;
    virtual size_t getSize() const;
    virtual std::shared_ptr<TypeDeclaration> clone() const;

  public:
    VoidDeclaration() : BaseTypeDeclaration(TypeKind::TYPE_VOID)
    {
    }
    const TypeDeclaration *isCompatibleType(const TypeDeclaration *ty) const override
    {
        return 0;
    }
    bool isSameAs(const TypeDeclaration *ty) const override
    {
        return BaseTypeDeclaration::isSameAs(ty);
    }
    bool hasLlvmType() const override
    {
        return true;
    }
    static bool isClassOf(const TypeDeclaration *e)
    {
        return e->getKind() == TypeKind::TYPE_VOID;
    }

  protected:
    std::shared_ptr<llvm::Type> getLlvmType() const override;
};

class CompoundDeclaration : public TypeDeclaration
{
  public:
    virtual TypeKind getTypeKind() const;
    virtual bool isIncomplete() const;
    virtual bool isIntegral() const;
    virtual bool isStringLike() const;
    virtual bool isUnsigned() const;
    virtual std::shared_ptr<Range> getRange() const;
    virtual unsigned int bits() const;
    virtual const TypeDeclaration *isCompatibleType(const TypeDeclaration *ty) const;
    virtual const TypeDeclaration *isAssignableType(const TypeDeclaration *ty) const;
    virtual size_t getSize() const;
    virtual std::shared_ptr<TypeDeclaration> clone() const;
    CompoundDeclaration(TypeKind typeKind, std::shared_ptr<TypeDeclaration> typeDeclaration)
        : TypeDeclaration(typeKind), baseType(typeDeclaration)
    {
    }
    CompoundDeclaration(TypeKind typeKind) : TypeDeclaration(typeKind)
    {
    }
    bool isSameAs(const TypeDeclaration *ty) const override;
    bool isCompound() const override
    {
        return true;
    }
    std::shared_ptr<TypeDeclaration> getSubtype() const override
    {
        return baseType;
    }
    bool hasLlvmType() const override
    {
        return baseType->hasLlvmType();
    }
    static bool isClassOf(const TypeDeclaration *e);
    static bool classof(const TypeDeclaration *e);
    void setBaseType(const std::shared_ptr<TypeDeclaration> &baseType);

  protected:
    std::shared_ptr<llvm::Type> getLlvmType() const override
    {
        return std::shared_ptr<llvm::Type>();
    }

    std::shared_ptr<TypeDeclaration> baseType;
};

class ArrayDeclaration : public CompoundDeclaration
{
  public:
    virtual TypeKind getTypeKind() const;
    virtual bool isIncomplete() const;
    virtual bool isIntegral() const;
    virtual bool isUnsigned() const;
    virtual std::shared_ptr<Range> getRange() const;
    virtual unsigned int bits() const;
    virtual const TypeDeclaration *isAssignableType(const TypeDeclaration *ty) const;
    virtual size_t getSize() const;
    virtual std::shared_ptr<TypeDeclaration> clone() const;
    virtual bool isCompound() const;
    virtual std::shared_ptr<TypeDeclaration> getSubtype() const;
    ArrayDeclaration(std::shared_ptr<TypeDeclaration> b, const std::vector<std::shared_ptr<RangeDeclaration>> &r)
        : CompoundDeclaration(TypeKind::TYPE_ARRAY, b), ranges(r)
    {
    }
    ArrayDeclaration(TypeKind tk, std::shared_ptr<TypeDeclaration> b,
                     const std::vector<std::shared_ptr<RangeDeclaration>> &r)
        : CompoundDeclaration(tk, b), ranges(r)
    {
    }
    const std::vector<std::shared_ptr<RangeDeclaration>> getRanges() const
    {
        return ranges;
    }
    bool isStringLike() const override
    {
        return (baseType->getTypeKind() == TypeKind::TYPE_CHAR);
    }
    bool isSameAs(const TypeDeclaration *ty) const override;
    const TypeDeclaration *isCompatibleType(const TypeDeclaration *ty) const override;
    static bool isClassOf(const TypeDeclaration *e)
    {
        return e->getKind() >= TypeKind::TYPE_ARRAY && e->getKind() <= TypeKind::TYPE_LAST_ARRAY;
    }
    static bool classof(const TypeDeclaration *e){
        return isClassOf(e);
    }
    bool hasLlvmType() const override
    {
        return CompoundDeclaration::hasLlvmType();
    }

  protected:
    std::shared_ptr<llvm::Type> getLlvmType() const override;
    std::vector<std::shared_ptr<RangeDeclaration>> ranges;
};

class EnumDeclaration : public CompoundDeclaration
{
  public:
    virtual TypeKind getTypeKind() const;
    virtual bool isIncomplete() const;
    virtual bool isStringLike() const;
    virtual const TypeDeclaration *isCompatibleType(const TypeDeclaration *ty) const;
    virtual const TypeDeclaration *isAssignableType(const TypeDeclaration *ty) const;
    virtual size_t getSize() const;
    virtual std::shared_ptr<TypeDeclaration> clone() const;
    virtual std::shared_ptr<TypeDeclaration> getSubtype() const;
    struct EnumValue
    {
        EnumValue(const std::string &nm, int v) : name(nm), value(v)
        {
        }
        EnumValue(const EnumValue &e) : name(e.name), value(e.value)
        {
        }
        std::string name;
        int value;
    };

    typedef std::vector<EnumValue> EnumValues;

  public:
    EnumDeclaration(TypeKind tk, const std::vector<std::string> &nmv, std::shared_ptr<TypeDeclaration> ty)
        : CompoundDeclaration(tk, ty)
    {
        setValues(nmv);
    }
    EnumDeclaration(const std::vector<std::string> &nmv, std::shared_ptr<TypeDeclaration> ty)
        : EnumDeclaration(TypeKind::TYPE_ENUM, nmv, ty)
    {
    }
    EnumDeclaration(TypeKind tk, const std::vector<std::string> &nmv) : CompoundDeclaration(tk)
    {
        setValues(nmv);
    }

  private:
    void setValues(const std::vector<std::string> &nmv);

  public:
    std::shared_ptr<Range> getRange() const override
    {
        return std::make_shared<Range>(Range(0, values.size() - 1));
    }
    const EnumValues &getValues() const
    {
        return values;
    }
    bool isIntegral() const override
    {
        return true;
    }
    bool isUnsigned() const override
    {
        return true;
    }
    bool isCompound() const override
    {
        return false;
    }
    unsigned bits() const override;
    static bool isClassOf(const TypeDeclaration *e)
    {
        return e->getKind() == TypeKind::TYPE_ENUM;
    }
    bool isSameAs(const TypeDeclaration *ty) const override;
    bool hasLlvmType() const override
    {
        return CompoundDeclaration::hasLlvmType();
    }

  protected:
    std::shared_ptr<llvm::Type> getLlvmType() const override
    {
        return baseType->getLtype();
    }

  private:
    EnumValues values;
};

class BoolDeclaration : public EnumDeclaration
{
  public:
    virtual TypeKind getTypeKind() const;
    virtual bool isIncomplete() const;
    virtual bool isStringLike() const;
    virtual const TypeDeclaration *isCompatibleType(const TypeDeclaration *ty) const;
    virtual const TypeDeclaration *isAssignableType(const TypeDeclaration *ty) const;
    virtual size_t getSize() const;
    virtual std::shared_ptr<TypeDeclaration> clone() const;
    virtual std::shared_ptr<TypeDeclaration> getSubtype() const;
    virtual std::shared_ptr<Range> getRange() const;
    virtual bool isIntegral() const;
    virtual bool isUnsigned() const;
    virtual bool isCompound() const;
    virtual unsigned int bits() const;
    BoolDeclaration() : EnumDeclaration(TypeKind::TYPE_BOOLEAN, std::vector<std::string>{"FALSE", "TRUE"})
    {
        auto ptr = std::shared_ptr<BoolDeclaration>(this, [](BoolDeclaration *) {});
        EnumDeclaration::setBaseType(shared_from_this());
    }
    void init();
    bool isSameAs(const TypeDeclaration *ty) const override
    {
        return ty == this;
    }

  protected:
    std::shared_ptr<llvm::Type> getLlvmType() const override;
};

class PointerDeclaration : public CompoundDeclaration
{
  public:
    virtual TypeKind getTypeKind() const;
    virtual bool isIntegral() const;
    virtual bool isStringLike() const;
    virtual bool isUnsigned() const;
    virtual std::shared_ptr<Range> getRange() const;
    virtual unsigned int bits() const;
    virtual const TypeDeclaration *isCompatibleType(const TypeDeclaration *ty) const;
    virtual const TypeDeclaration *isAssignableType(const TypeDeclaration *ty) const;
    virtual size_t getSize() const;
    virtual std::shared_ptr<TypeDeclaration> clone() const;
    virtual std::shared_ptr<TypeDeclaration> getSubtype() const;
    PointerDeclaration(std::shared_ptr<ForwardDeclaration> fwd)
        : incomplete(true), forward(true), CompoundDeclaration(TypeKind::TYPE_POINTER, fwd)
    {
    }
    PointerDeclaration(std::shared_ptr<TypeDeclaration> ty)
        : incomplete(false), forward(false), CompoundDeclaration(TypeKind::TYPE_POINTER, ty)
    {
    }
    bool isIncomplete() const override
    {
        return incomplete;
    }
    bool isForward() const
    {
        return forward;
    }
    bool isCompound() const override
    {
        return false;
    }
    static bool isClassOf(const TypeDeclaration *e)
    {
        return e->getKind() == TypeKind::TYPE_POINTER;
    }
    bool hasLlvmType() const override
    {
        return baseType->hasLlvmType();
    }
    void SetSubType(std::shared_ptr<TypeDeclaration> t)
    {
        baseType = t;
        incomplete = false;
    }
    static bool classof(const TypeDeclaration *e)
    {
        return isClassOf(e);
    }
    bool isSameAs(const TypeDeclaration *ty) const override
    {
        return CompoundDeclaration::isSameAs(ty);
    }

  protected:
    std::shared_ptr<llvm::Type> getLlvmType() const override;

  private:
    bool incomplete;
    bool forward;
};

class FunctionDeclaration : public CompoundDeclaration
{
  public:
    virtual bool isIncomplete() const;
    virtual bool isIntegral() const;
    virtual bool isStringLike() const;
    virtual bool isUnsigned() const;
    virtual std::shared_ptr<Range> getRange() const;
    virtual unsigned int bits() const;
    virtual size_t getSize() const;
    virtual std::shared_ptr<TypeDeclaration> clone() const;
    virtual std::shared_ptr<TypeDeclaration> getSubtype() const;
    FunctionDeclaration(std::shared_ptr<PrototypeExpression> proto);
    const TypeDeclaration *isCompatibleType(const TypeDeclaration *ty) const override
    {
        return baseType->isCompatibleType(ty);
    }
    const TypeDeclaration *isAssignableType(const TypeDeclaration *ty) const override
    {
        return baseType->isAssignableType(ty);
    }
    bool isCompound() const override
    {
        return false;
    }
    bool hasLlvmType() const override
    {
        return false;
    }
    static bool isClassOf(const TypeDeclaration *e)
    {
        return e->getKind() == TypeKind::TYPE_FUNCTION;
    }
    std::shared_ptr<PrototypeExpression> getPrototypeExpression() const
    {
        return prototype;
    }
    static bool classof(const TypeDeclaration *e)
    {
        return isClassOf(e);
    }
    bool isSameAs(const TypeDeclaration *ty) const override;

  protected:
    std::shared_ptr<llvm::Type> getLlvmType() const override
    {
        return 0;
    }

  private:
    std::shared_ptr<PrototypeExpression> prototype;
};

class FieldDeclaration : public CompoundDeclaration
{
  public:
    virtual TypeKind getTypeKind() const;
    virtual bool isIncomplete() const;
    virtual bool isStringLike() const;
    virtual bool isUnsigned() const;
    virtual std::shared_ptr<Range> getRange() const;
    virtual unsigned int bits() const;
    virtual const TypeDeclaration *isCompatibleType(const TypeDeclaration *ty) const;
    virtual const TypeDeclaration *isAssignableType(const TypeDeclaration *ty) const;
    virtual size_t getSize() const;
    virtual std::shared_ptr<TypeDeclaration> clone() const;
    virtual std::shared_ptr<TypeDeclaration> getSubtype() const;
    FieldDeclaration(const std::string &nm, std::shared_ptr<TypeDeclaration> ty, bool stat, Access ac = Public)
        : isStat(stat), CompoundDeclaration(TypeKind::TYPE_FIELD, ty)
    {
        setName(nm);
    }

  public:
    std::shared_ptr<TypeDeclaration> getFieldType() const
    {
        return baseType;
    }
    bool isIntegral() const override
    {
        return baseType->isIntegral();
    }
    bool isCompound() const override
    {
        return baseType->isCompound();
    }
    bool isStatic() const
    {
        return isStat;
    }
    bool isSameAs(const TypeDeclaration *ty) const override
    {
        return baseType->isSameAs(ty);
    }
    static bool isClassOf(const TypeDeclaration *e)
    {
        return e->getKind() == TypeKind::TYPE_FIELD;
    }
    operator Access()
    {
        return access;
    }
    bool hasLlvmType() const override
    {
        return CompoundDeclaration::hasLlvmType();
    }

  protected:
    std::shared_ptr<llvm::Type> getLlvmType() const override
    {
        return baseType->getLtype();
    }

  private:
    bool isStat;
    Access access;
};

class FieldCollection : public TypeDeclaration
{
  public:
    virtual TypeKind getTypeKind() const;
    virtual bool isIncomplete() const;
    virtual bool isIntegral() const;
    virtual bool isStringLike() const;
    virtual bool isUnsigned() const;
    virtual std::shared_ptr<Range> getRange() const;
    virtual std::shared_ptr<TypeDeclaration> getSubtype() const;
    virtual unsigned int bits() const;
    virtual const TypeDeclaration *isCompatibleType(const TypeDeclaration *ty) const;
    virtual const TypeDeclaration *isAssignableType(const TypeDeclaration *ty) const;
    virtual size_t getSize() const;
    virtual std::shared_ptr<TypeDeclaration> clone() const;

  public:
    FieldCollection(TypeKind k, const std::vector<std::shared_ptr<FieldDeclaration>> flds)
        : TypeDeclaration(k), fields(flds), opaqueType(0)
    {
    }
    virtual int Element(const std::string &name) const;
    virtual const FieldDeclaration *getElement(unsigned int n) const
    {
    }
    void ensureSized() const;
    virtual int getFieldCount() const
    {
        return fields.size();
    }
    bool isCompound() const override
    {
        return true;
    }
    bool isSameAs(const TypeDeclaration *ty) const override;
    bool hasLlvmType() const override
    {
        return getLlvmType() != nullptr;
    }
    static bool isClassOf(const TypeDeclaration *e)
    {
        return e->getKind() == TypeKind::TYPE_VRIANT || e->getKind() == TypeKind::TYPE_RECORD ||
               e->getKind() == TypeKind::TYPE_CLASS;
    }
    static bool classof(const TypeDeclaration *e)
    {
        return isClassOf(e);
    }

  protected:
    std::shared_ptr<llvm::Type> getLlvmType() const override
    {
        return std::shared_ptr<llvm::Type>();
    }
    std::vector<std::shared_ptr<FieldDeclaration>> fields;
    mutable std::shared_ptr<llvm::StructType> opaqueType;
};

class VariantDeclaration : public FieldCollection
{
  public:
    virtual TypeKind getTypeKind() const;
    virtual bool isIncomplete() const;
    virtual bool isIntegral() const;
    virtual bool isStringLike() const;
    virtual bool isUnsigned() const;
    virtual std::shared_ptr<Range> getRange() const;
    virtual std::shared_ptr<TypeDeclaration> getSubtype() const;
    virtual unsigned int bits() const;
    virtual const TypeDeclaration *isCompatibleType(const TypeDeclaration *ty) const;
    virtual const TypeDeclaration *isAssignableType(const TypeDeclaration *ty) const;
    virtual size_t getSize() const;
    virtual std::shared_ptr<TypeDeclaration> clone() const;
    virtual int Element(const std::string &name) const;
    virtual const FieldDeclaration *getElement(unsigned int n) const;
    virtual int getFieldCount() const;
    virtual bool isCompound() const;
    VariantDeclaration(const std::vector<std::shared_ptr<FieldDeclaration>> flds)
        : FieldCollection(TypeKind::TYPE_VRIANT, flds){};
    static bool isClassOf(const TypeDeclaration *e)
    {
        return e->getKind() == TypeKind::TYPE_VRIANT;
    }
    bool isSameAs(const TypeDeclaration *ty) const override
    {
        return FieldCollection::isSameAs(ty);
    }
    bool hasLlvmType() const override
    {
        return FieldCollection::hasLlvmType();
    }

  protected:
    std::shared_ptr<llvm::Type> getLlvmType() const override;
};

class RecordDeclaration : public FieldCollection
{
  public:
    virtual TypeKind getTypeKind() const;
    virtual bool isIncomplete() const;
    virtual bool isIntegral() const;
    virtual bool isStringLike() const;
    virtual bool isUnsigned() const;
    virtual std::shared_ptr<Range> getRange() const;
    virtual std::shared_ptr<TypeDeclaration> getSubtype() const;
    virtual unsigned int bits() const;
    virtual const TypeDeclaration *isCompatibleType(const TypeDeclaration *ty) const;
    virtual const TypeDeclaration *isAssignableType(const TypeDeclaration *ty) const;
    virtual std::shared_ptr<TypeDeclaration> clone() const;
    virtual int Element(const std::string &name) const;
    virtual const FieldDeclaration *getElement(unsigned int n) const;
    virtual int getFieldCount() const;
    virtual bool isCompound() const;

  public:
    RecordDeclaration(const std::vector<std::shared_ptr<FieldDeclaration>> flds, VariantDeclaration *v)
        : FieldCollection(TypeKind::TYPE_RECORD, flds), variant(v){};
    size_t getSize() const override;
    std::shared_ptr<VariantDeclaration> getVariant() const
    {
        return variant;
    }
    bool isSameAs(const TypeDeclaration *ty) const override
    {
        return this == ty;
    }
    static bool isClassOf(const TypeDeclaration *e)
    {
        return e->getKind() == TypeKind::TYPE_RECORD;
    }
    static bool classof(const TypeDeclaration *e)
    {
        return isClassOf(e);
    }
    bool hasLlvmType() const override
    {
        return FieldCollection::hasLlvmType();
    }

  protected:
    std::shared_ptr<llvm::Type> getLlvmType() const override;

  private:
    std::shared_ptr<VariantDeclaration> variant;
};

class MemberFunctionDeclaration : public TypeDeclaration
{
  public:
    virtual TypeKind getTypeKind() const;
    virtual bool isIncomplete() const;
    virtual bool isIntegral() const;
    virtual bool isStringLike() const;
    virtual bool isUnsigned() const;
    virtual bool isCompound() const;
    virtual std::shared_ptr<Range> getRange() const;
    virtual std::shared_ptr<TypeDeclaration> getSubtype() const;
    virtual unsigned int bits() const;
    virtual const TypeDeclaration *isCompatibleType(const TypeDeclaration *ty) const;
    virtual const TypeDeclaration *isAssignableType(const TypeDeclaration *ty) const;
    virtual size_t getSize() const;
    virtual std::shared_ptr<TypeDeclaration> clone() const;

  public:
    bool isSameAs(const TypeDeclaration *typeDeclaration) const override
    {
        return false;
    }
    bool hasLlvmType() const override
    {
        return false;
    }

  protected:
    std::shared_ptr<llvm::Type> getLlvmType() const override
    {
        return std::shared_ptr<llvm::Type>();
    }
};

class FunctionPointerDeclaration : public CompoundDeclaration
{
  public:
    virtual TypeKind getTypeKind() const;
    virtual bool isIncomplete() const;
    virtual bool isIntegral() const;
    virtual bool isStringLike() const;
    virtual bool isUnsigned() const;
    virtual std::shared_ptr<Range> getRange() const;
    virtual unsigned int bits() const;
    virtual const TypeDeclaration *isCompatibleType(const TypeDeclaration *ty) const;
    virtual const TypeDeclaration *isAssignableType(const TypeDeclaration *ty) const;
    virtual size_t getSize() const;
    virtual std::shared_ptr<TypeDeclaration> clone() const;
    virtual std::shared_ptr<TypeDeclaration> getSubtype() const;
    FunctionPointerDeclaration(std::shared_ptr<PrototypeExpression> func);
    std::shared_ptr<PrototypeExpression> getPrototype() const
    {
        return proto;
    }
    bool isCompound() const override
    {
        return false;
    }
    bool isSameAs(const TypeDeclaration *ty) const override;
    bool hasLlvmType() const override
    {
        return true;
    }
    static bool isClassOf(const TypeDeclaration *e)
    {
        return e->getKind() == TypeKind::TYPE_FUNCION_POINTER;
    }

  protected:
    std::shared_ptr<llvm::Type> getLlvmType() const override;

  private:
    std::shared_ptr<PrototypeExpression> proto;
};

class FileDeclaration : public CompoundDeclaration
{
  public:
    virtual TypeKind getTypeKind() const;
    virtual bool isIncomplete() const;
    virtual bool isIntegral() const;
    virtual bool isStringLike() const;
    virtual bool isUnsigned() const;
    virtual std::shared_ptr<Range> getRange() const;
    virtual unsigned int bits() const;
    virtual const TypeDeclaration *isCompatibleType(const TypeDeclaration *ty) const;
    virtual const TypeDeclaration *isAssignableType(const TypeDeclaration *ty) const;
    virtual size_t getSize() const;
    virtual std::shared_ptr<TypeDeclaration> clone() const;
    virtual bool isCompound() const;
    virtual std::shared_ptr<TypeDeclaration> getSubtype() const;
    FileDeclaration(std::shared_ptr<TypeDeclaration> ty) : CompoundDeclaration(TypeKind::TYPE_FILE, ty)
    {
    }
    FileDeclaration(TypeKind k, std::shared_ptr<TypeDeclaration> ty) : CompoundDeclaration(k, ty)
    {
    }
    static bool classof(const TypeDeclaration *e)
    {
        return e->getKind() == TypeKind::TYPE_FILE || e->getKind() == TypeKind::TYPE_TEXT;
    }
    bool isSameAs(const TypeDeclaration *ty) const override
    {
        return CompoundDeclaration::isSameAs(ty);
    }
    bool hasLlvmType() const override
    {
        return CompoundDeclaration::hasLlvmType();
    }

  protected:
    std::shared_ptr<llvm::Type> getLlvmType() const override;
};

class TextDeclaration : public FileDeclaration
{
  public:
    virtual TypeKind getTypeKind() const;
    virtual bool isIncomplete() const;
    virtual bool isIntegral() const;
    virtual bool isStringLike() const;
    virtual bool isUnsigned() const;
    virtual std::shared_ptr<Range> getRange() const;
    virtual unsigned int bits() const;
    virtual const TypeDeclaration *isCompatibleType(const TypeDeclaration *ty) const;
    virtual const TypeDeclaration *isAssignableType(const TypeDeclaration *ty) const;
    virtual size_t getSize() const;
    virtual std::shared_ptr<TypeDeclaration> clone() const;
    virtual bool isCompound() const;
    virtual std::shared_ptr<TypeDeclaration> getSubtype() const;

  public:
    TextDeclaration() : FileDeclaration(TypeKind::TYPE_TEXT, std::shared_ptr<CharDeclaration>(new CharDeclaration))
    {
    }
    bool hasLlvmType() const override
    {
        return true;
    }
    static bool isClassOf(const TypeDeclaration *e)
    {
        return e->getKind() == TypeKind::TYPE_TEXT;
    }
    bool isSameAs(const TypeDeclaration *ty) const override
    {
        return FileDeclaration::isSameAs(ty);
    }

  protected:
    std::shared_ptr<llvm::Type> getLlvmType() const override
    {
        return FileDeclaration::getLlvmType();
    }
};

class StringDeclaration : public ArrayDeclaration
{
  public:
    virtual TypeKind getTypeKind() const;
    virtual bool isIncomplete() const;
    virtual bool isIntegral() const;
    virtual bool isUnsigned() const;
    virtual std::shared_ptr<Range> getRange() const;
    virtual unsigned int bits() const;
    virtual const TypeDeclaration *isAssignableType(const TypeDeclaration *ty) const;
    virtual size_t getSize() const;
    virtual std::shared_ptr<TypeDeclaration> clone() const;
    virtual bool isCompound() const;
    virtual std::shared_ptr<TypeDeclaration> getSubtype() const;
    virtual bool isSameAs(const TypeDeclaration *ty) const;

  protected:
    virtual std::shared_ptr<llvm::Type> getLlvmType() const;

  public:
    StringDeclaration(unsigned size)
        : ArrayDeclaration(
              TypeKind::TYPE_STRING, static_cast<std::shared_ptr<TypeDeclaration>>(new CharDeclaration),
              std::vector<std::shared_ptr<RangeDeclaration>>(
                  1, std::make_shared<RangeDeclaration>(std::make_shared<Range>(Range(0, size)), getIntegerType())))
    {
    }
    int getCapacity() const;
    static bool isClassOf(const TypeDeclaration *e)
    {
        return e->getKind() == TypeKind::TYPE_STRING;
    }
    bool isStringLike() const override
    {
        return true;
    }
    bool hasLlvmType() const override
    {
        return true;
    }

    const TypeDeclaration *isCompatibleType(const TypeDeclaration *ty) const override;
};

class RangeDeclaration : public TypeDeclaration
{
  public:
    virtual TypeKind getTypeKind() const;
    virtual bool isIncomplete() const;
    virtual bool isStringLike() const;
    virtual std::shared_ptr<TypeDeclaration> getSubtype() const;
    virtual size_t getSize() const;
    virtual std::shared_ptr<TypeDeclaration> clone() const;
    RangeDeclaration(const std::shared_ptr<Range> &range, const std::shared_ptr<TypeDeclaration> &baseType)
        : TypeDeclaration(TypeKind::TYPE_RANGE), range(range), baseType(baseType)
    {
    }

  public:
    const TypeDeclaration *isCompatibleType(const TypeDeclaration *ty) const override;
    const TypeDeclaration *isAssignableType(const TypeDeclaration *ty) const override;

    bool isCompound() const override
    {
        return false;
    }
    bool isIntegral() const override
    {
        return true;
    }
    bool isUnsigned() const override
    {
        return this->range->getStart() >= 0;
    }
    static bool isClassOf(const TypeDeclaration *typeDeclaration)
    {
        return typeDeclaration->getKind() == TypeKind::TYPE_RANGE;
    }
    static bool classof(const TypeDeclaration *typeDeclaration)
    {
        return isClassOf(typeDeclaration);
    }
    bool isSameAs(const TypeDeclaration *ty) const override;
    unsigned bits() const override;
    std::shared_ptr<Range> getRange() const override
    {
        return range;
    }

    bool hasLlvmType() const override
    {
        return baseType->hasLlvmType();
    }

  protected:
    std::shared_ptr<llvm::Type> getLlvmType() const override
    {
        return baseType->getLtype();
    }

  private:
    std::shared_ptr<Range> range;
    std::shared_ptr<TypeDeclaration> baseType;
};

class SetDeclaration : public CompoundDeclaration
{
  public:
    virtual TypeKind getTypeKind() const;
    virtual bool isIncomplete() const;
    virtual bool isIntegral() const;
    virtual bool isStringLike() const;
    virtual bool isUnsigned() const;
    virtual unsigned int bits() const;
    virtual const TypeDeclaration *isAssignableType(const TypeDeclaration *ty) const;
    virtual size_t getSize() const;
    virtual std::shared_ptr<TypeDeclaration> clone() const;
    virtual bool isCompound() const;
    virtual std::shared_ptr<TypeDeclaration> getSubtype() const;
    typedef unsigned int ElemType;
    SetDeclaration(std::shared_ptr<RangeDeclaration> range, std::shared_ptr<TypeDeclaration> ty);
    static bool isClassOf(const TypeDeclaration *e)
    {
        return e->getKind() == TypeKind::TYPE_SET;
    }
    size_t SetWords() const
    {
        return (this->range->getRange()->getSize() + __SET_MASK__) >> __SET_POW_TO_BITS__;
    }
    std::shared_ptr<Range> getRange() const override;
    void UpdateRange(std::shared_ptr<RangeDeclaration> rangeDecl)
    {
        range = rangeDecl;
    }
    void UpdateSubtype(std::shared_ptr<TypeDeclaration> ty);
    bool isSameAs(const TypeDeclaration *ty) const;
    const TypeDeclaration *isCompatibleType(const TypeDeclaration *ty) const override;
    bool hasLlvmType() const override
    {
        return true;
    }

  private:
    std::shared_ptr<llvm::Type> getLlvmType() const
    {
    }

  private:
    std::shared_ptr<RangeDeclaration> range;
};

class ConstantDeclaration
{
  public:
    ConstantDeclaration(std::shared_ptr<TypeDeclaration> typeDeclaration, ConstantKind kind, Location loc)
        : type(typeDeclaration), loc(loc), constantKind(kind)
    {
    }
    const std::shared_ptr<TypeDeclaration> &getType() const
    {
        return type;
    }
    ConstantKind getConstantKind() const
    {
        return constantKind;
    }
    const Location &getLoc() const
    {
        return loc;
    }
    static bool isClassOf(const ConstantDeclaration *constantDeclaration)
    {
        return constantDeclaration->getConstantKind() == ConstantKind::CONSTANT_DECLARATION;
    }
    static bool classof(const ConstantDeclaration *namedObject)
    {
        return isClassOf(namedObject);
    }
    virtual Token toTokenType() = 0;

  protected:
    std::shared_ptr<TypeDeclaration> type;
    ConstantKind constantKind;
    Location loc;
};

class IntConstantDeclaration : public ConstantDeclaration
{
  public:
    IntConstantDeclaration(const Location &loc, int64_t value)
        : value(value), ConstantDeclaration(getIntegerType(), ConstantKind::INT_CONSTANT_DECLARATION, loc)
    {
    }
    static bool isClassOf(const ConstantDeclaration *constantDeclaration)
    {
        return constantDeclaration->getConstantKind() == ConstantKind::INT_CONSTANT_DECLARATION;
    }
    int64_t getValue() const
    {
        return value;
    }
    static bool classof(const ConstantDeclaration *namedObject)
    {
        return isClassOf(namedObject);
    }
    Token toTokenType() override
    {
        return Token(TokenType::NUMERIC_LITERAL, std::to_string(value), loc);
    }

  private:
    int64_t value;
};

class EnumConstantDeclaration : public ConstantDeclaration
{
  public:
    EnumConstantDeclaration(const Location &loc, uint64_t value)
        : value(value), ConstantDeclaration(getIntegerType(), ConstantKind::ENUM_CONSTANT_DECLARATION, loc)
    {
    }
    EnumConstantDeclaration(std::shared_ptr<TypeDeclaration> type, const Location &loc, uint64_t value)
        : value(value), ConstantDeclaration(type, ConstantKind::ENUM_CONSTANT_DECLARATION, loc)
    {
    }
    static bool isClassOf(const ConstantDeclaration *constantDeclaration)
    {
        return constantDeclaration->getConstantKind() == ConstantKind::ENUM_CONSTANT_DECLARATION;
    }
    int64_t getValue() const
    {
        return value;
    }
    static bool classof(const ConstantDeclaration *namedObject)
    {
        return isClassOf(namedObject);
    }
    Token toTokenType() override
    {
        return Token(TokenType::NUMERIC_LITERAL, std::to_string(value), loc);
    }

  private:
    uint64_t value;
};

class RealConstantDeclaration : public ConstantDeclaration
{
  public:
    RealConstantDeclaration(const Location &loc, long double value)
        : value(value), ConstantDeclaration(getRealType(), ConstantKind::REAL_CONSTANT_DECLARATION, loc)
    {
    }
    static bool isClassOf(const ConstantDeclaration *constantDeclaration)
    {
        return constantDeclaration->getConstantKind() == ConstantKind::REAL_CONSTANT_DECLARATION;
    }
    long double getValue() const
    {
        return value;
    }
    static bool classof(const ConstantDeclaration *namedObject)
    {
        return isClassOf(namedObject);
    }
    Token toTokenType() override
    {
        return Token(TokenType::NUMERIC_LITERAL, std::to_string(value), loc);
    }

  private:
    long double value;
};

class CharConstantDeclaration : public ConstantDeclaration
{
  public:
    CharConstantDeclaration(const Location &loc, char value)
        : value(value), ConstantDeclaration(getCharType(), ConstantKind::CHAR_CONSTANT_DECLARATION, loc)
    {
    }
    static bool isClassOf(const ConstantDeclaration *constantDeclaration)
    {
        return constantDeclaration->getConstantKind() == ConstantKind::CHAR_CONSTANT_DECLARATION;
    }
    int64_t getValue() const
    {
        return value;
    }
    static bool classof(const ConstantDeclaration *namedObject)
    {
        return isClassOf(namedObject);
    }
    Token toTokenType() override
    {
        std::string val= ""+value;
        return Token(TokenType::STRING_LITERAL, val, loc);
    }

  private:
    char value;
};

class BooleanConstantDeclaration : public ConstantDeclaration
{
  public:
    BooleanConstantDeclaration(const Location &loc, bool value)
        : value(value), ConstantDeclaration(getBooleanType(), ConstantKind::BOOL_CONSTANT_DECLARATION, loc)
    {
    }
    static bool isClassOf(const ConstantDeclaration *constantDeclaration)
    {
        return constantDeclaration->getConstantKind() == ConstantKind::BOOL_CONSTANT_DECLARATION;
    }
    bool getValue() const
    {
        return value;
    }
    static bool classof(const ConstantDeclaration *namedObject)
    {
        return isClassOf(namedObject);
    }
    Token toTokenType() override
    {
        std::string s = (value) ? "true": "false";
        return Token(TokenType::IDENTIFIER, s, loc);
    }

  private:
    bool value;
};

class StringConstantDeclaration : public ConstantDeclaration
{
  public:
    StringConstantDeclaration(const Location &loc, std::string value)
        : value(value), ConstantDeclaration(getStringType(), ConstantKind::STRING_CONSTANT_DECLARATION, loc)
    {
    }
    static bool isClassOf(const ConstantDeclaration *constantDeclaration)
    {
        return constantDeclaration->getConstantKind() == ConstantKind::STRING_CONSTANT_DECLARATION;
    }
    std::string getValue() const
    {
        return value;
    }
    static bool classof(const ConstantDeclaration *namedObject)
    {
        return isClassOf(namedObject);
    }
    Token toTokenType() override
    {
        return Token(TokenType::STRING_LITERAL, value, loc);
    }

  private:
    std::string value;
};

inline std::shared_ptr<ConstantDeclaration> operator+(std::shared_ptr<ConstantDeclaration> const &lhs,
                                                      std::shared_ptr<ConstantDeclaration> const &rhs)
{
    throw ConstantsOperatorError::TYPES_NOT_COMPATIBLE;
}
inline std::shared_ptr<ConstantDeclaration> operator-(const std::shared_ptr<ConstantDeclaration> &lhs,
                                                      const std::shared_ptr<ConstantDeclaration> &rhs)
{
    throw ConstantsOperatorError::TYPES_NOT_COMPATIBLE;
}
inline std::shared_ptr<ConstantDeclaration> operator*(const std::shared_ptr<ConstantDeclaration> &lhs,
                                                      const std::shared_ptr<ConstantDeclaration> &rhs)
{
    throw ConstantsOperatorError::TYPES_NOT_COMPATIBLE;
}
inline std::shared_ptr<ConstantDeclaration> operator/(const std::shared_ptr<ConstantDeclaration> &lhs,
                                                      const std::shared_ptr<ConstantDeclaration> &rhs)
{
    throw ConstantsOperatorError::TYPES_NOT_COMPATIBLE;
}

inline bool operator==(const TypeDeclaration &lty, const TypeDeclaration &rty)
{
    return lty.isSameAs(&rty);
}
inline bool operator!=(const TypeDeclaration &lty, const TypeDeclaration &rty)
{
    return !(lty == rty);
}

inline bool operator==(const Range &a, const Range &b)
{
    return (a.getStart() == b.getStart() && a.getEnd() == b.getEnd());
}

inline bool operator!=(const Range &a, const Range &b)
{
    return !(b == a);
}

inline bool operator==(const EnumDeclaration::EnumValue &a, const EnumDeclaration::EnumValue &b)
{
    return (a.value == b.value && a.name == b.name);
}
inline bool operator!=(const EnumDeclaration::EnumValue &a, const EnumDeclaration::EnumValue &b)
{
    return !(a == b);
}
} // namespace Pascal
#endif // JAPC_TYPE_H
