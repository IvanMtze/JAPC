//
// Created by wuser on 15/10/22.
//

#include "japc/AST/type.h"

using namespace Pascal;

std::shared_ptr<TypeDeclaration> Pascal::getIntegerType()
{
    static std::shared_ptr<TypeDeclaration> type;
    if (!type)
    {
        type = std::make_shared<IntegerDeclaration>(IntegerDeclaration());
    }
    return type;
}
std::shared_ptr<TypeDeclaration> Pascal::getLongIntType()
{
    static std::shared_ptr<TypeDeclaration> type;
    if (!type)
    {
        type = std::make_shared<LongIntegerDeclaration>(LongIntegerDeclaration());
    }
    return type;
}
std::shared_ptr<TypeDeclaration> Pascal::getCharType()
{
    static std::shared_ptr<TypeDeclaration> type;
    if (!type)
    {
        type = std::make_shared<CharDeclaration>(CharDeclaration());
    }
    return type;
}
std::shared_ptr<TypeDeclaration> Pascal::getBooleanType()
{
    static std::shared_ptr<TypeDeclaration> type;
    if (!type)
    {
        type = std::make_shared<BoolDeclaration>(BoolDeclaration());
    }
    return type;
}
std::shared_ptr<TypeDeclaration> Pascal::getRealType()
{
    static std::shared_ptr<TypeDeclaration> type;
    if (!type)
    {
        type = std::make_shared<RealDeclaration>(RealDeclaration());
    }
    return type;
}
std::shared_ptr<TypeDeclaration> Pascal::getVoidType()
{
    static std::shared_ptr<TypeDeclaration> type;
    if (!type)
    {
        type = std::make_shared<VoidDeclaration>(VoidDeclaration());
    }
    return type;
}
std::shared_ptr<TypeDeclaration> Pascal::getTextType()
{
    static std::shared_ptr<TypeDeclaration> type;
    if (!type)
    {
        type = std::make_shared<TextDeclaration>(TextDeclaration());
    }
    return type;
}
std::shared_ptr<TypeDeclaration> Pascal::getStringType()
{
    static std::shared_ptr<TypeDeclaration> type;
    if (!type)
    {
        type = std::make_shared<StringDeclaration>(StringDeclaration(255));
    }
    return type;
}
std::shared_ptr<TypeDeclaration> Pascal::getTimeStampType()
{
    static std::shared_ptr<TypeDeclaration> type;
    if (!type)
    {
        type = std::make_shared<StringDeclaration>(StringDeclaration(255));
    }
    return type;
}
bool Pascal::isCharArray(TypeDeclaration *typeDeclaration)
{
    return (typeDeclaration->getTypeKind() == TypeKind::TYPE_ARRAY &&
            typeDeclaration->getSubtype()->getTypeKind() == TypeKind::TYPE_CHAR);
}

const TypeDeclaration *TypeDeclaration::isCompatibleType(const Pascal::TypeDeclaration *ty) const
{
    if (isSameAs(ty))
    {
        return this;
    }
    return nullptr;
}
std::shared_ptr<Range> TypeDeclaration::getRange() const
{
    if (isIntegral())
    {
        switch (kind)
        {
        case TYPE_CHAR:
            return std::make_shared<Range>(Range(0, UCHAR_MAX));
        case TYPE_INTEGER:
            return std::make_shared<Range>(Range(INT_MIN, INT_MAX));
        default:
            return 0;
        }
    }
    return nullptr;
}
size_t TypeDeclaration::getSize() const
{
    return 0;
}
size_t TypeDeclaration::getAllignSize() const
{
    return 0;
}
const std::shared_ptr<ExpressionAST> &TypeDeclaration::getInitial() const
{
    return initial;
}
void TypeDeclaration::setInitial(const std::shared_ptr<ExpressionAST> &initial)
{
    TypeDeclaration::initial = initial;
}

SetDeclaration::SetDeclaration(std::shared_ptr<RangeDeclaration> range, std::shared_ptr<TypeDeclaration> ty)
    : CompoundDeclaration(TypeKind::TYPE_SET, ty), range(range)
{
    if (range)
    {
        assert(range->getRange()->getSize() <= 1000 && "Set too large");
    }
}
std::shared_ptr<Range> SetDeclaration::getRange() const
{
    if (range)
    {
        return range->getRange();
    }
    return nullptr;
}
void SetDeclaration::updateSubtype(std::shared_ptr<TypeDeclaration> ty)
{
    baseType = ty;
}
const TypeDeclaration *SetDeclaration::isCompatibleType(const TypeDeclaration *ty) const
{
    if (const SetDeclaration *sty = llvm::dyn_cast<SetDeclaration>(ty))
    {
        if (*baseType == *sty->baseType)
        {
            return this;
        }
    }
    return 0;
}
bool SetDeclaration::isSameAs(const TypeDeclaration *ty) const
{
    if (CompoundDeclaration::isSameAs(ty))
    {
        if (const SetDeclaration *sty = llvm::dyn_cast<SetDeclaration>(ty))
        {
            return sty->range && *range == *sty->range;
        }
    }
    return false;
}
TypeKind SetDeclaration::getTypeKind() const
{
    return CompoundDeclaration::getTypeKind();
}
bool SetDeclaration::isIncomplete() const
{
    return CompoundDeclaration::isIncomplete();
}
bool SetDeclaration::isIntegral() const
{
    return CompoundDeclaration::isIntegral();
}
bool SetDeclaration::isStringLike() const
{
    return CompoundDeclaration::isStringLike();
}
bool SetDeclaration::isUnsigned() const
{
    return CompoundDeclaration::isUnsigned();
}
unsigned int SetDeclaration::bits() const
{
    return CompoundDeclaration::bits();
}
const TypeDeclaration *SetDeclaration::isAssignableType(const TypeDeclaration *ty) const
{
    return CompoundDeclaration::isAssignableType(ty);
}
size_t SetDeclaration::getSize() const
{
    return CompoundDeclaration::getSize();
}
std::shared_ptr<TypeDeclaration> SetDeclaration::clone() const
{
    return CompoundDeclaration::clone();
}
bool SetDeclaration::isCompound() const
{
    return CompoundDeclaration::isCompound();
}
std::shared_ptr<TypeDeclaration> SetDeclaration::getSubtype() const
{
    return CompoundDeclaration::getSubtype();
}
TypeKind CharDeclaration::getTypeKind() const
{
    return TypeDeclaration::getTypeKind();
}
bool CharDeclaration::isIncomplete() const
{
    return TypeDeclaration::isIncomplete();
}
bool CharDeclaration::isCompound() const
{
    return TypeDeclaration::isCompound();
}
std::shared_ptr<Range> CharDeclaration::getRange() const
{
    return TypeDeclaration::getRange();
}
std::shared_ptr<TypeDeclaration> CharDeclaration::getSubtype() const
{
    return TypeDeclaration::getSubtype();
}
size_t CharDeclaration::getSize() const
{
    return TypeDeclaration::getSize();
}
std::shared_ptr<TypeDeclaration> CharDeclaration::clone() const
{
    return TypeDeclaration::clone();
}
const TypeDeclaration *CharDeclaration::isCompatibleType(const TypeDeclaration *ty) const
{
    if (*this == *ty || ty->getTypeKind() == TypeKind::TYPE_CHAR)
    {
        return this;
    }
    return nullptr;
}
const TypeDeclaration *CharDeclaration::isAssignableType(const TypeDeclaration *ty) const
{
    if (*this == *ty)
    {
        return this;
    };
    return nullptr;
}
template <> const TypeDeclaration *IntegerDeclaration::isCompatibleType(const TypeDeclaration *ty) const
{
    if (ty->getTypeKind() == TypeKind::TYPE_INTEGER)
    {
        return this;
    }
    if (ty->getTypeKind() == TypeKind::TYPE_LONG_INT || ty->getTypeKind() == TypeKind::TYPE_REAL)
    {
        return ty;
    }
    return nullptr;
}
template <> const TypeDeclaration *IntegerDeclaration::isAssignableType(const TypeDeclaration *ty) const
{
    if (isSameAs(ty))
    {
        return ty;
    }
    return nullptr;
}

template <> const TypeDeclaration *LongIntegerDeclaration::isCompatibleType(const TypeDeclaration *ty) const
{
    if (ty->getTypeKind() == TypeKind::TYPE_LONG_INT || ty->getTypeKind() == TypeKind::TYPE_INTEGER)
    {
        return this;
    }
    if (ty->getTypeKind() == TypeKind::TYPE_REAL)
    {
        return ty;
    }
    return nullptr;
}

template <> const TypeDeclaration *LongIntegerDeclaration::isAssignableType(const TypeDeclaration *ty) const
{
    if (isSameAs(ty) || ty->getTypeKind() == TypeKind::TYPE_INTEGER)
    {
        return this;
    }
    return nullptr;
}

std::shared_ptr<llvm::Type> CharDeclaration::getLlvmType() const
{
    return BaseTypeDeclaration::getLlvmType();
}
TypeKind VoidDeclaration::getTypeKind() const
{
    return TypeKind::TYPE_VOID;
}
bool VoidDeclaration::isIncomplete() const
{
    return TypeDeclaration::isIncomplete();
}
bool VoidDeclaration::isIntegral() const
{
    return TypeDeclaration::isIntegral();
}
bool VoidDeclaration::isStringLike() const
{
    return TypeDeclaration::isStringLike();
}
bool VoidDeclaration::isUnsigned() const
{
    return TypeDeclaration::isUnsigned();
}
bool VoidDeclaration::isCompound() const
{
    return TypeDeclaration::isCompound();
}
std::shared_ptr<Range> VoidDeclaration::getRange() const
{
    return TypeDeclaration::getRange();
}
std::shared_ptr<TypeDeclaration> VoidDeclaration::getSubtype() const
{
    return TypeDeclaration::getSubtype();
}
unsigned int VoidDeclaration::bits() const
{
    return TypeDeclaration::bits();
}
const TypeDeclaration *VoidDeclaration::isAssignableType(const TypeDeclaration *ty) const
{
    return TypeDeclaration::isAssignableType(ty);
}
size_t VoidDeclaration::getSize() const
{
    return TypeDeclaration::getSize();
}
std::shared_ptr<TypeDeclaration> VoidDeclaration::clone() const
{
    return TypeDeclaration::clone();
}
std::shared_ptr<llvm::Type> VoidDeclaration::getLlvmType() const
{
    return BaseTypeDeclaration::getLlvmType();
}
TypeKind CompoundDeclaration::getTypeKind() const
{
    return TypeDeclaration::getTypeKind();
}
bool CompoundDeclaration::isIncomplete() const
{
    return TypeDeclaration::isIncomplete();
}
bool CompoundDeclaration::isIntegral() const
{
    return TypeDeclaration::isIntegral();
}
bool CompoundDeclaration::isStringLike() const
{
    return TypeDeclaration::isStringLike();
}
bool CompoundDeclaration::isUnsigned() const
{
    return TypeDeclaration::isUnsigned();
}
std::shared_ptr<Range> CompoundDeclaration::getRange() const
{
    return TypeDeclaration::getRange();
}
unsigned int CompoundDeclaration::bits() const
{
    return TypeDeclaration::bits();
}
const TypeDeclaration *CompoundDeclaration::isCompatibleType(const TypeDeclaration *ty) const
{
    return TypeDeclaration::isCompatibleType(ty);
}
const TypeDeclaration *CompoundDeclaration::isAssignableType(const TypeDeclaration *ty) const
{
    return TypeDeclaration::isAssignableType(ty);
}
size_t CompoundDeclaration::getSize() const
{
    return TypeDeclaration::getSize();
}
std::shared_ptr<TypeDeclaration> CompoundDeclaration::clone() const
{
    return TypeDeclaration::clone();
}
bool CompoundDeclaration::isSameAs(const TypeDeclaration *ty) const
{
    if (this == ty)
    {
        return true;
    }
    if (getTypeKind() != ty->getTypeKind())
    {
        return false;
    }
    const CompoundDeclaration *cty = llvm::dyn_cast<CompoundDeclaration>(ty);
    return cty && *cty->getSubtype() == *baseType;
}
bool CompoundDeclaration::isClassOf(const TypeDeclaration *e)
{
    return classof(e);
}
bool CompoundDeclaration::classof(const TypeDeclaration *e)
{
    switch (e->getTypeKind())
    {
    case TypeKind::TYPE_ARRAY:
    case TypeKind::TYPE_STRING:
    case TypeKind::TYPE_POINTER:
    case TypeKind::TYPE_FIELD:
    case TypeKind::TYPE_FUNCION_POINTER:
    case TypeKind::TYPE_FILE:
    case TypeKind::TYPE_TEXT:
    case TypeKind::TYPE_SET:
        return true;
    default:
        break;
    }
    return false;
}
void CompoundDeclaration::setBaseType(const std::shared_ptr<TypeDeclaration> &baseType)
{
    CompoundDeclaration::baseType = baseType;
}
TypeKind ArrayDeclaration::getTypeKind() const
{
    return CompoundDeclaration::getTypeKind();
}
bool ArrayDeclaration::isIncomplete() const
{
    return CompoundDeclaration::isIncomplete();
}
bool ArrayDeclaration::isIntegral() const
{
    return CompoundDeclaration::isIntegral();
}
bool ArrayDeclaration::isUnsigned() const
{
    return CompoundDeclaration::isUnsigned();
}
std::shared_ptr<Range> ArrayDeclaration::getRange() const
{
    return CompoundDeclaration::getRange();
}
unsigned int ArrayDeclaration::bits() const
{
    return CompoundDeclaration::bits();
}
const TypeDeclaration *ArrayDeclaration::isAssignableType(const TypeDeclaration *ty) const
{
    return CompoundDeclaration::isAssignableType(ty);
}
size_t ArrayDeclaration::getSize() const
{
    return CompoundDeclaration::getSize();
}
std::shared_ptr<TypeDeclaration> ArrayDeclaration::clone() const
{
    return CompoundDeclaration::clone();
}
bool ArrayDeclaration::isCompound() const
{
    return CompoundDeclaration::isCompound();
}
std::shared_ptr<TypeDeclaration> ArrayDeclaration::getSubtype() const
{
    return CompoundDeclaration::getSubtype();
}
bool ArrayDeclaration::isSameAs(const TypeDeclaration *ty) const
{
    if (CompoundDeclaration::isSameAs(ty))
    {
        if (const ArrayDeclaration *aty = llvm::dyn_cast<ArrayDeclaration>(ty))
        {
            if (ranges.size() != aty->getRanges().size())
            {
                return false;
            }
            for (size_t i = 0; i < ranges.size(); i++)
            {
                if (*ranges[i] != *aty->getRanges()[i])
                {
                    return false;
                }
            }
            return true;
        }
    }
    return false;
}
const TypeDeclaration *ArrayDeclaration::isCompatibleType(const TypeDeclaration *ty) const
{
    if (isSameAs(ty))
    {
        return this;
    }
    if (const ArrayDeclaration *aty = llvm::dyn_cast<ArrayDeclaration>(ty))
    {
        if (ty->getSubtype() == getSubtype() && ranges.size() == aty->getRanges().size())
        {
            for (size_t i = 0; i < ranges.size(); i++)
            {
                if (ranges[i]->getSize() != aty->getRanges()[i]->getSize())
                {
                    return 0;
                }
            }
        }
    }
    return this;
}
std::shared_ptr<llvm::Type> ArrayDeclaration::getLlvmType() const
{
    return CompoundDeclaration::getLlvmType();
}
TypeKind EnumDeclaration::getTypeKind() const
{
    return CompoundDeclaration::getTypeKind();
}
bool EnumDeclaration::isIncomplete() const
{
    return CompoundDeclaration::isIncomplete();
}
bool EnumDeclaration::isStringLike() const
{
    return CompoundDeclaration::isStringLike();
}
const TypeDeclaration *EnumDeclaration::isCompatibleType(const TypeDeclaration *ty) const
{
    return CompoundDeclaration::isCompatibleType(ty);
}
const TypeDeclaration *EnumDeclaration::isAssignableType(const TypeDeclaration *ty) const
{
    return CompoundDeclaration::isAssignableType(ty);
}
size_t EnumDeclaration::getSize() const
{
    return CompoundDeclaration::getSize();
}
std::shared_ptr<TypeDeclaration> EnumDeclaration::clone() const
{
    return CompoundDeclaration::clone();
}
std::shared_ptr<TypeDeclaration> EnumDeclaration::getSubtype() const
{
    return CompoundDeclaration::getSubtype();
}
void EnumDeclaration::setValues(const std::vector<std::string> &nmv)
{
    unsigned int v = 0;
    for (auto n : nmv)
    {
        values.push_back(EnumValue(n, v));
        v++;
    }
}
unsigned EnumDeclaration::bits() const
{
    return CompoundDeclaration::bits();
}
bool EnumDeclaration::isSameAs(const TypeDeclaration *ty) const
{
    if (const EnumDeclaration *ety = llvm::dyn_cast<EnumDeclaration>(ty))
    {
        if (ety->getTypeKind() != getTypeKind() || values.size() != ety->values.size())
        {
            return false;
        }
        for (size_t i = 0; i < values.size(); i++)
        {
            if (values[i] != ety->values[i])
            {
                return false;
            }
        }
    }
    else
    {
        return false;
    }
    return true;
}
TypeKind BoolDeclaration::getTypeKind() const
{
    return EnumDeclaration::getTypeKind();
}
bool BoolDeclaration::isIncomplete() const
{
    return EnumDeclaration::isIncomplete();
}
bool BoolDeclaration::isStringLike() const
{
    return EnumDeclaration::isStringLike();
}
const TypeDeclaration *BoolDeclaration::isCompatibleType(const TypeDeclaration *ty) const
{
    return EnumDeclaration::isCompatibleType(ty);
}
const TypeDeclaration *BoolDeclaration::isAssignableType(const TypeDeclaration *ty) const
{
    return EnumDeclaration::isAssignableType(ty);
}
size_t BoolDeclaration::getSize() const
{
    return EnumDeclaration::getSize();
}
std::shared_ptr<TypeDeclaration> BoolDeclaration::clone() const
{
    return EnumDeclaration::clone();
}
std::shared_ptr<TypeDeclaration> BoolDeclaration::getSubtype() const
{
    return EnumDeclaration::getSubtype();
}
std::shared_ptr<Range> BoolDeclaration::getRange() const
{
    return EnumDeclaration::getRange();
}
bool BoolDeclaration::isIntegral() const
{
    return EnumDeclaration::isIntegral();
}
bool BoolDeclaration::isUnsigned() const
{
    return EnumDeclaration::isUnsigned();
}
bool BoolDeclaration::isCompound() const
{
    return EnumDeclaration::isCompound();
}
unsigned int BoolDeclaration::bits() const
{
    return EnumDeclaration::bits();
}
std::shared_ptr<llvm::Type> BoolDeclaration::getLlvmType() const
{
    return EnumDeclaration::getLlvmType();
}
void BoolDeclaration::init()
{
    EnumDeclaration::setBaseType(shared_from_this());
}
TypeKind PointerDeclaration::getTypeKind() const
{
    return CompoundDeclaration::getTypeKind();
}
bool PointerDeclaration::isIntegral() const
{
    return CompoundDeclaration::isIntegral();
}
bool PointerDeclaration::isStringLike() const
{
    return CompoundDeclaration::isStringLike();
}
bool PointerDeclaration::isUnsigned() const
{
    return CompoundDeclaration::isUnsigned();
}
std::shared_ptr<Range> PointerDeclaration::getRange() const
{
    return CompoundDeclaration::getRange();
}
unsigned int PointerDeclaration::bits() const
{
    return CompoundDeclaration::bits();
}
const TypeDeclaration *PointerDeclaration::isCompatibleType(const TypeDeclaration *ty) const
{
    return CompoundDeclaration::isCompatibleType(ty);
}
const TypeDeclaration *PointerDeclaration::isAssignableType(const TypeDeclaration *ty) const
{
    return CompoundDeclaration::isAssignableType(ty);
}
size_t PointerDeclaration::getSize() const
{
    return CompoundDeclaration::getSize();
}
std::shared_ptr<TypeDeclaration> PointerDeclaration::clone() const
{
    return CompoundDeclaration::clone();
}
std::shared_ptr<TypeDeclaration> PointerDeclaration::getSubtype() const
{
    return CompoundDeclaration::getSubtype();
}
std::shared_ptr<llvm::Type> PointerDeclaration::getLlvmType() const
{
    return CompoundDeclaration::getLlvmType();
}
bool FunctionDeclaration::isIncomplete() const
{
    return CompoundDeclaration::isIncomplete();
}
bool FunctionDeclaration::isIntegral() const
{
    return CompoundDeclaration::isIntegral();
}
bool FunctionDeclaration::isStringLike() const
{
    return CompoundDeclaration::isStringLike();
}
bool FunctionDeclaration::isUnsigned() const
{
    return CompoundDeclaration::isUnsigned();
}
std::shared_ptr<Range> FunctionDeclaration::getRange() const
{
    return CompoundDeclaration::getRange();
}
unsigned int FunctionDeclaration::bits() const
{
    return CompoundDeclaration::bits();
}
size_t FunctionDeclaration::getSize() const
{
    return CompoundDeclaration::getSize();
}
std::shared_ptr<TypeDeclaration> FunctionDeclaration::clone() const
{
    return CompoundDeclaration::clone();
}
std::shared_ptr<TypeDeclaration> FunctionDeclaration::getSubtype() const
{
    return CompoundDeclaration::getSubtype();
}
FunctionDeclaration::FunctionDeclaration(std::shared_ptr<PrototypeExpression> proto)
    : CompoundDeclaration(TypeKind::TYPE_FUNCTION, proto->getTypeDeclaration()), prototype(proto)
{
}
bool FunctionDeclaration::isSameAs(const TypeDeclaration *ty) const
{
    {
        if (ty->getTypeKind() == TypeKind::TYPE_FUNCION_POINTER)
        {
            const FunctionPointerDeclaration *fty = llvm::dyn_cast<FunctionPointerDeclaration>(ty);
            return prototype == fty->getPrototype();
        }
        if (ty->getTypeKind() == TypeKind::TYPE_FUNCTION)
        {
            const FunctionDeclaration *fty = llvm::dyn_cast<FunctionDeclaration>(ty);
            return prototype == fty->getPrototypeExpression();
        }
        return false;
    }
}
TypeKind FieldDeclaration::getTypeKind() const
{
    return CompoundDeclaration::getTypeKind();
}
bool FieldDeclaration::isIncomplete() const
{
    return CompoundDeclaration::isIncomplete();
}
bool FieldDeclaration::isStringLike() const
{
    return CompoundDeclaration::isStringLike();
}
bool FieldDeclaration::isUnsigned() const
{
    return CompoundDeclaration::isUnsigned();
}
std::shared_ptr<Range> FieldDeclaration::getRange() const
{
    return CompoundDeclaration::getRange();
}
unsigned int FieldDeclaration::bits() const
{
    return CompoundDeclaration::bits();
}
const TypeDeclaration *FieldDeclaration::isCompatibleType(const TypeDeclaration *ty) const
{
    return CompoundDeclaration::isCompatibleType(ty);
}
const TypeDeclaration *FieldDeclaration::isAssignableType(const TypeDeclaration *ty) const
{
    return CompoundDeclaration::isAssignableType(ty);
}
size_t FieldDeclaration::getSize() const
{
    return CompoundDeclaration::getSize();
}
std::shared_ptr<TypeDeclaration> FieldDeclaration::clone() const
{
    return CompoundDeclaration::clone();
}
std::shared_ptr<TypeDeclaration> FieldDeclaration::getSubtype() const
{
    return CompoundDeclaration::getSubtype();
}
TypeKind FieldCollection::getTypeKind() const
{
    return TypeDeclaration::getTypeKind();
}
bool FieldCollection::isIncomplete() const
{
    return TypeDeclaration::isIncomplete();
}
bool FieldCollection::isIntegral() const
{
    return TypeDeclaration::isIntegral();
}
bool FieldCollection::isStringLike() const
{
    return TypeDeclaration::isStringLike();
}
bool FieldCollection::isUnsigned() const
{
    return TypeDeclaration::isUnsigned();
}
std::shared_ptr<Range> FieldCollection::getRange() const
{
    return TypeDeclaration::getRange();
}
std::shared_ptr<TypeDeclaration> FieldCollection::getSubtype() const
{
    return TypeDeclaration::getSubtype();
}
unsigned int FieldCollection::bits() const
{
    return TypeDeclaration::bits();
}
const TypeDeclaration *FieldCollection::isCompatibleType(const TypeDeclaration *ty) const
{
    return TypeDeclaration::isCompatibleType(ty);
}
const TypeDeclaration *FieldCollection::isAssignableType(const TypeDeclaration *ty) const
{
    return TypeDeclaration::isAssignableType(ty);
}
size_t FieldCollection::getSize() const
{
    return TypeDeclaration::getSize();
}
std::shared_ptr<TypeDeclaration> FieldCollection::clone() const
{
    return TypeDeclaration::clone();
}
int FieldCollection::Element(const std::string &name) const
{
    int i = 0;
    for (auto f : fields)
    {
        // Check for special record type
        if (f->getName() == "")
        {
            RecordDeclaration *rd = llvm::dyn_cast<RecordDeclaration>(f->getFieldType().get());
            assert(rd && "Expected record declarataion here!");
            if (rd->Element(name) >= 0)
            {
                return i;
            }
        }
        if (f->getName() == name)
        {
            return i;
        }
        i++;
    }
    return -1;
}
void FieldCollection::ensureSized() const
{
    if (opaqueType && opaqueType->isOpaque())
    {
        std::shared_ptr<llvm::Type> ty = getLlvmType();
        // TODO: REVIEW
        (void)ty.get();
    }
}
bool FieldCollection::isSameAs(const TypeDeclaration *ty) const
{
    if (getTypeKind() != ty->getTypeKind())
    {
        return false;
    }

    if (const FieldCollection *fty = llvm::dyn_cast<FieldCollection>(ty))
    {
        if (fields.size() != fty->fields.size())
        {
            return false;
        }
        for (size_t i = 0; i < fields.size(); i++)
        {
            if (fields[i] != fty->fields[i])
            {
                return false;
            }
        }
        return true;
    }
    return false;
}
TypeKind VariantDeclaration::getTypeKind() const
{
    return FieldCollection::getTypeKind();
}
bool VariantDeclaration::isIncomplete() const
{
    return FieldCollection::isIncomplete();
}
bool VariantDeclaration::isIntegral() const
{
    return FieldCollection::isIntegral();
}
bool VariantDeclaration::isStringLike() const
{
    return FieldCollection::isStringLike();
}
bool VariantDeclaration::isUnsigned() const
{
    return FieldCollection::isUnsigned();
}
std::shared_ptr<Range> VariantDeclaration::getRange() const
{
    return FieldCollection::getRange();
}
std::shared_ptr<TypeDeclaration> VariantDeclaration::getSubtype() const
{
    return FieldCollection::getSubtype();
}
unsigned int VariantDeclaration::bits() const
{
    return FieldCollection::bits();
}
const TypeDeclaration *VariantDeclaration::isCompatibleType(const TypeDeclaration *ty) const
{
    return FieldCollection::isCompatibleType(ty);
}
const TypeDeclaration *VariantDeclaration::isAssignableType(const TypeDeclaration *ty) const
{
    return FieldCollection::isAssignableType(ty);
}
size_t VariantDeclaration::getSize() const
{
    return FieldCollection::getSize();
}
std::shared_ptr<TypeDeclaration> VariantDeclaration::clone() const
{
    return FieldCollection::clone();
}
int VariantDeclaration::Element(const std::string &name) const
{
    return FieldCollection::Element(name);
}
const FieldDeclaration *VariantDeclaration::getElement(unsigned int n) const
{
    return FieldCollection::getElement(n);
}
int VariantDeclaration::getFieldCount() const
{
    return FieldCollection::getFieldCount();
}
bool VariantDeclaration::isCompound() const
{
    return FieldCollection::isCompound();
}
std::shared_ptr<llvm::Type> VariantDeclaration::getLlvmType() const
{
    return FieldCollection::getLlvmType();
}
TypeKind RecordDeclaration::getTypeKind() const
{
    return FieldCollection::getTypeKind();
}
bool RecordDeclaration::isIncomplete() const
{
    return FieldCollection::isIncomplete();
}
bool RecordDeclaration::isIntegral() const
{
    return FieldCollection::isIntegral();
}
bool RecordDeclaration::isStringLike() const
{
    return FieldCollection::isStringLike();
}
bool RecordDeclaration::isUnsigned() const
{
    return FieldCollection::isUnsigned();
}
std::shared_ptr<Range> RecordDeclaration::getRange() const
{
    return FieldCollection::getRange();
}
std::shared_ptr<TypeDeclaration> RecordDeclaration::getSubtype() const
{
    return FieldCollection::getSubtype();
}
unsigned int RecordDeclaration::bits() const
{
    return FieldCollection::bits();
}
const TypeDeclaration *RecordDeclaration::isCompatibleType(const TypeDeclaration *ty) const
{
    return FieldCollection::isCompatibleType(ty);
}
const TypeDeclaration *RecordDeclaration::isAssignableType(const TypeDeclaration *ty) const
{
    return FieldCollection::isAssignableType(ty);
}
std::shared_ptr<TypeDeclaration> RecordDeclaration::clone() const
{
    return FieldCollection::clone();
}
int RecordDeclaration::Element(const std::string &name) const
{
    return FieldCollection::Element(name);
}
const FieldDeclaration *RecordDeclaration::getElement(unsigned int n) const
{
    return FieldCollection::getElement(n);
}
int RecordDeclaration::getFieldCount() const
{
    return FieldCollection::getFieldCount();
}
bool RecordDeclaration::isCompound() const
{
    return FieldCollection::isCompound();
}
size_t RecordDeclaration::getSize() const
{
    ensureSized();
    return TypeDeclaration::getSize();
}
std::shared_ptr<llvm::Type> RecordDeclaration::getLlvmType() const
{
    return FieldCollection::getLlvmType();
}
TypeKind MemberFunctionDeclaration::getTypeKind() const
{
    return TypeDeclaration::getTypeKind();
}
bool MemberFunctionDeclaration::isIncomplete() const
{
    return TypeDeclaration::isIncomplete();
}
bool MemberFunctionDeclaration::isIntegral() const
{
    return TypeDeclaration::isIntegral();
}
bool MemberFunctionDeclaration::isStringLike() const
{
    return TypeDeclaration::isStringLike();
}
bool MemberFunctionDeclaration::isUnsigned() const
{
    return TypeDeclaration::isUnsigned();
}
bool MemberFunctionDeclaration::isCompound() const
{
    return TypeDeclaration::isCompound();
}
std::shared_ptr<Range> MemberFunctionDeclaration::getRange() const
{
    return TypeDeclaration::getRange();
}
std::shared_ptr<TypeDeclaration> MemberFunctionDeclaration::getSubtype() const
{
    return TypeDeclaration::getSubtype();
}
unsigned int MemberFunctionDeclaration::bits() const
{
    return TypeDeclaration::bits();
}
const TypeDeclaration *MemberFunctionDeclaration::isCompatibleType(const TypeDeclaration *ty) const
{
    return TypeDeclaration::isCompatibleType(ty);
}
const TypeDeclaration *MemberFunctionDeclaration::isAssignableType(const TypeDeclaration *ty) const
{
    return TypeDeclaration::isAssignableType(ty);
}
size_t MemberFunctionDeclaration::getSize() const
{
    return TypeDeclaration::getSize();
}
std::shared_ptr<TypeDeclaration> MemberFunctionDeclaration::clone() const
{
    return TypeDeclaration::clone();
}
TypeKind FunctionPointerDeclaration::getTypeKind() const
{
    return CompoundDeclaration::getTypeKind();
}
bool FunctionPointerDeclaration::isIncomplete() const
{
    return CompoundDeclaration::isIncomplete();
}
bool FunctionPointerDeclaration::isIntegral() const
{
    return CompoundDeclaration::isIntegral();
}
bool FunctionPointerDeclaration::isStringLike() const
{
    return CompoundDeclaration::isStringLike();
}
bool FunctionPointerDeclaration::isUnsigned() const
{
    return CompoundDeclaration::isUnsigned();
}
std::shared_ptr<Range> FunctionPointerDeclaration::getRange() const
{
    return CompoundDeclaration::getRange();
}
unsigned int FunctionPointerDeclaration::bits() const
{
    return CompoundDeclaration::bits();
}
const TypeDeclaration *FunctionPointerDeclaration::isCompatibleType(const TypeDeclaration *ty) const
{
    return CompoundDeclaration::isCompatibleType(ty);
}
const TypeDeclaration *FunctionPointerDeclaration::isAssignableType(const TypeDeclaration *ty) const
{
    return CompoundDeclaration::isAssignableType(ty);
}
size_t FunctionPointerDeclaration::getSize() const
{
    return CompoundDeclaration::getSize();
}
std::shared_ptr<TypeDeclaration> FunctionPointerDeclaration::clone() const
{
    return CompoundDeclaration::clone();
}
std::shared_ptr<TypeDeclaration> FunctionPointerDeclaration::getSubtype() const
{
    return CompoundDeclaration::getSubtype();
}
FunctionPointerDeclaration::FunctionPointerDeclaration(std::shared_ptr<PrototypeExpression> func)
    : CompoundDeclaration(TypeKind::TYPE_FUNCION_POINTER, nullptr), proto(func)
{
}
bool FunctionPointerDeclaration::isSameAs(const TypeDeclaration *ty) const
{
    if (ty->getTypeKind() == TypeKind::TYPE_FUNCION_POINTER)
    {
        const FunctionPointerDeclaration *fty = llvm::dyn_cast<FunctionPointerDeclaration>(ty);
        assert(fty && "Expect to convert to function pointer!");
        return *proto == *fty->proto;
    }
    if (ty->getTypeKind() == TypeKind::TYPE_FUNCTION)
    {
        const FunctionPointerDeclaration *fty = llvm::dyn_cast<FunctionPointerDeclaration>(ty);
        assert(fty && "Expect to convert to function declaration");
        return *proto == *fty->getPrototype();
    }
    return false;
}
std::shared_ptr<llvm::Type> FunctionPointerDeclaration::getLlvmType() const
{
    return CompoundDeclaration::getLlvmType();
}
TypeKind FileDeclaration::getTypeKind() const
{
    return CompoundDeclaration::getTypeKind();
}
bool FileDeclaration::isIncomplete() const
{
    return CompoundDeclaration::isIncomplete();
}
bool FileDeclaration::isIntegral() const
{
    return CompoundDeclaration::isIntegral();
}
bool FileDeclaration::isStringLike() const
{
    return CompoundDeclaration::isStringLike();
}
bool FileDeclaration::isUnsigned() const
{
    return CompoundDeclaration::isUnsigned();
}
std::shared_ptr<Range> FileDeclaration::getRange() const
{
    return CompoundDeclaration::getRange();
}
unsigned int FileDeclaration::bits() const
{
    return CompoundDeclaration::bits();
}
const TypeDeclaration *FileDeclaration::isCompatibleType(const TypeDeclaration *ty) const
{
    return CompoundDeclaration::isCompatibleType(ty);
}
const TypeDeclaration *FileDeclaration::isAssignableType(const TypeDeclaration *ty) const
{
    return CompoundDeclaration::isAssignableType(ty);
}
size_t FileDeclaration::getSize() const
{
    return CompoundDeclaration::getSize();
}
std::shared_ptr<TypeDeclaration> FileDeclaration::clone() const
{
    return CompoundDeclaration::clone();
}
bool FileDeclaration::isCompound() const
{
    return CompoundDeclaration::isCompound();
}
std::shared_ptr<TypeDeclaration> FileDeclaration::getSubtype() const
{
    return CompoundDeclaration::getSubtype();
}
std::shared_ptr<llvm::Type> FileDeclaration::getLlvmType() const
{
    return CompoundDeclaration::getLlvmType();
}
TypeKind TextDeclaration::getTypeKind() const
{
    return FileDeclaration::getTypeKind();
}
bool TextDeclaration::isIncomplete() const
{
    return FileDeclaration::isIncomplete();
}
bool TextDeclaration::isIntegral() const
{
    return FileDeclaration::isIntegral();
}
bool TextDeclaration::isStringLike() const
{
    return FileDeclaration::isStringLike();
}
bool TextDeclaration::isUnsigned() const
{
    return FileDeclaration::isUnsigned();
}
std::shared_ptr<Range> TextDeclaration::getRange() const
{
    return FileDeclaration::getRange();
}
unsigned int TextDeclaration::bits() const
{
    return FileDeclaration::bits();
}
const TypeDeclaration *TextDeclaration::isCompatibleType(const TypeDeclaration *ty) const
{
    return FileDeclaration::isCompatibleType(ty);
}
const TypeDeclaration *TextDeclaration::isAssignableType(const TypeDeclaration *ty) const
{
    return FileDeclaration::isAssignableType(ty);
}
size_t TextDeclaration::getSize() const
{
    return FileDeclaration::getSize();
}
std::shared_ptr<TypeDeclaration> TextDeclaration::clone() const
{
    return FileDeclaration::clone();
}
bool TextDeclaration::isCompound() const
{
    return FileDeclaration::isCompound();
}
std::shared_ptr<TypeDeclaration> TextDeclaration::getSubtype() const
{
    return FileDeclaration::getSubtype();
}
TypeKind StringDeclaration::getTypeKind() const
{
    return ArrayDeclaration::getTypeKind();
}
bool StringDeclaration::isIncomplete() const
{
    return ArrayDeclaration::isIncomplete();
}
bool StringDeclaration::isIntegral() const
{
    return ArrayDeclaration::isIntegral();
}
bool StringDeclaration::isUnsigned() const
{
    return ArrayDeclaration::isUnsigned();
}
std::shared_ptr<Range> StringDeclaration::getRange() const
{
    return ArrayDeclaration::getRange();
}
unsigned int StringDeclaration::bits() const
{
    return ArrayDeclaration::bits();
}
const TypeDeclaration *StringDeclaration::isAssignableType(const TypeDeclaration *ty) const
{
    return ArrayDeclaration::isAssignableType(ty);
}
int StringDeclaration::getCapacity() const
{
    std::shared_ptr<RangeDeclaration> range = ranges[0];
    return range->getRange()->getSize() - 1;
}
size_t StringDeclaration::getSize() const
{
    return ArrayDeclaration::getSize();
}
std::shared_ptr<TypeDeclaration> StringDeclaration::clone() const
{
    return ArrayDeclaration::clone();
}
bool StringDeclaration::isCompound() const
{
    return ArrayDeclaration::isCompound();
}
std::shared_ptr<TypeDeclaration> StringDeclaration::getSubtype() const
{
    return ArrayDeclaration::getSubtype();
}
bool StringDeclaration::isSameAs(const TypeDeclaration *ty) const
{
    return ArrayDeclaration::isSameAs(ty);
}
std::shared_ptr<llvm::Type> StringDeclaration::getLlvmType() const
{
    return ArrayDeclaration::getLlvmType();
}
const TypeDeclaration *StringDeclaration::isCompatibleType(const TypeDeclaration *ty) const
{
    if (isSameAs(ty) || ty->getTypeKind() == TypeKind::TYPE_CHAR)
    {
        return this;
    }
    if (ty->getTypeKind() == TypeKind::TYPE_STRING)
    {
        if (llvm::dyn_cast<StringDeclaration>(ty)->getCapacity() > getCapacity())
        {
            return ty;
        }
        return this;
    }
    if (ty->getTypeKind() == TypeKind::TYPE_ARRAY)
    {
        if (const ArrayDeclaration *aty = llvm::dyn_cast<ArrayDeclaration>(ty))
        {
            if (aty->getRanges().size() == 1)
            {
                return this;
            }
        }
    }
    return 0;
}
TypeKind RangeDeclaration::getTypeKind() const
{
    return TypeDeclaration::getTypeKind();
}
bool RangeDeclaration::isIncomplete() const
{
    return TypeDeclaration::isIncomplete();
}
bool RangeDeclaration::isStringLike() const
{
    return TypeDeclaration::isStringLike();
}
std::shared_ptr<TypeDeclaration> RangeDeclaration::getSubtype() const
{
    return TypeDeclaration::getSubtype();
}
size_t RangeDeclaration::getSize() const
{
    return TypeDeclaration::getSize();
}
std::shared_ptr<TypeDeclaration> RangeDeclaration::clone() const
{
    return TypeDeclaration::clone();
}
const TypeDeclaration *RangeDeclaration::isCompatibleType(const TypeDeclaration *ty) const
{
    if (*this == *ty)
    {
        return this;
    }
    if (ty->getTypeKind() == getTypeKind())
    {
        return ty;
    }
    return nullptr;
}
const TypeDeclaration *RangeDeclaration::isAssignableType(const TypeDeclaration *ty) const
{
    if (isSameAs(ty) || ty->getTypeKind() == getTypeKind())
    {
        return ty;
    }
    return nullptr;
}
bool RangeDeclaration::isSameAs(const TypeDeclaration *ty) const
{
    if (const RangeDeclaration *rty = llvm::dyn_cast<RangeDeclaration>(ty))
    {
        return rty->getTypeKind() == getTypeKind() && *range == *rty->range;
    }
    return getTypeKind() == ty->getTypeKind();
}
unsigned RangeDeclaration::bits() const
{
    return baseType->bits();
}
const std::shared_ptr<TypeDeclaration> &RangeDeclaration::getBaseType() const
{
    return baseType;
}
TypeKind ForwardDeclaration::getTypeKind() const
{
    return TypeDeclaration::getTypeKind();
}
bool ForwardDeclaration::isIntegral() const
{
    return TypeDeclaration::isIntegral();
}
bool ForwardDeclaration::isStringLike() const
{
    return TypeDeclaration::isStringLike();
}
bool ForwardDeclaration::isUnsigned() const
{
    return TypeDeclaration::isUnsigned();
}
bool ForwardDeclaration::isCompound() const
{
    return TypeDeclaration::isCompound();
}
std::shared_ptr<Range> ForwardDeclaration::getRange() const
{
    return TypeDeclaration::getRange();
}
std::shared_ptr<TypeDeclaration> ForwardDeclaration::getSubtype() const
{
    return TypeDeclaration::getSubtype();
}
unsigned int ForwardDeclaration::bits() const
{
    return TypeDeclaration::bits();
}
const TypeDeclaration *ForwardDeclaration::isCompatibleType(const TypeDeclaration *ty) const
{
    return TypeDeclaration::isCompatibleType(ty);
}
const TypeDeclaration *ForwardDeclaration::isAssignableType(const TypeDeclaration *ty) const
{
    return TypeDeclaration::isAssignableType(ty);
}
size_t ForwardDeclaration::getSize() const
{
    return TypeDeclaration::getSize();
}
std::shared_ptr<TypeDeclaration> ForwardDeclaration::clone() const
{
    return TypeDeclaration::clone();
}
