//
// Created by wuser on 12/10/22.
//

#ifndef JAPC_EXPRESSION_H
#define JAPC_EXPRESSION_H
#include "japc/AST/builtin.h"
#include "japc/AST/named_object.h"
#include "japc/AST/type.h"
#include "japc/AST/visitor.h"
#include "japc/scanner/scanner.h"
#include <llvm/IR/Value.h>
#include <memory>
#include <set>

namespace Pascal
{
class RangeDeclaration;
class FunctionPointerDeclaration;
class VariableDefinition;
std::shared_ptr<TypeDeclaration> getIntegerType();
class ExpressionAST : public Visitable<ExpressionAST>
{
  public:
    enum ExpressionType
    {
        TYPE_EXPRE,
        TYPE_REAL_EXPRE,
        TYPE_INTEGER_EXPRE,
        TYPE_CHAR_EXPRE,
        TYPE_NIL_EXPRE,

        TYPE_ADDRESSABLE_EXPRE,
        TYPE_STRING_EXPRE,
        TYPE_SET_EXPRE,
        TYPE_VARIABLE_EXPRE,
        TYPE_ARRAY_EXPRE,
        TYPE_POINTER_EXPRE,
        TYPE_FILE_POINTER_EXPRE,
        TYPE_FIELD_EXPRE,
        TYPE_VARIANT_EXPRE,
        TYPE_FUNCTION_EXPRE,
        TYPE_TYPE_CAST_EXPRE,
        TYPE_LAST_ADDRESSABLE,

        TYPE_BINARY_EXPRE,
        TYPE_UNARY_EXPRE,
        TYPE_RANGE_EXPRE,
        TYPE_BLOCK_EXPRE,
        TYPE_ASSIGN_EXPRE,
        TYPE_VAR_DECL_EXPRE,
        TYPE_FUNCTION,
        TYPE_PROTOTYPE,
        TYPE_CALL_EXPRE,
        TYPE_BUILTIN_EXPRE,
        TYPE_IF_EXPRE,
        TYPE_FOR_EXPRE,
        TYPE_WHILE_EXPRE,
        TYPE_REPEAT_EXPRE,
        TYPE_WRITE,
        TYPE_READ,
        TYPE_LABEL_EXPRE,
        TYPE_CASE_EXPRE,
        TYPE_WITH_EXPRE,
        TYPE_RANGE_REDUCE_EXPRE,
        TYPE_RANGE_CHECK_EXPRE,
        TYPE_SIZE_OF_EXPRE,
        TYPE_V_TABLE_EXPRE,
        TYPE_VIRT_FUNCTION,
        TYPE_INIT_VALUE,
        TYPE_GOTO,
        TYPE_UNIT,
        TYPE_CLOSURE,
        TYPE_TRAMPOLINE,
    };

  public:
    ExpressionAST(const Location &location, ExpressionType expressionType)
        : location(location), expressionType(expressionType), typeDeclaration(0)
    {
    }
    ExpressionAST(const Location &location, ExpressionType expressionType,
                  std::shared_ptr<TypeDeclaration> typeDeclaration)
        : location(location), expressionType(expressionType), typeDeclaration(typeDeclaration)
    {
    }
    void accept(ExpressionVisitor &visitor) override
    {
        visitor.visit(this);
    }
    virtual std::shared_ptr<llvm::Value> codeGen()
    {
        assert(0 && "WHAT?!");
        return 0;
    };
    ExpressionType getExpressionType() const
    {
        return expressionType;
    }
    void setTypeDeclaration(std::shared_ptr<TypeDeclaration> typeDeclaration)
    {
        this->typeDeclaration = typeDeclaration;
    }
    virtual std::shared_ptr<TypeDeclaration> getTypeDeclaration() const
    {
        return typeDeclaration;
    }
    void ensureSized() const;
    const Location getLocation() const
    {
        return location;
    }

  private:
    const Location location;
    const ExpressionType expressionType;

  protected:
    std::shared_ptr<TypeDeclaration> typeDeclaration;
};

class RealExpression : public ExpressionAST
{
  public:
    RealExpression(const Location &loc, double value, std::shared_ptr<TypeDeclaration> typeDec)
        : ExpressionAST(loc, ExpressionType::TYPE_REAL_EXPRE, typeDec), value(value)
    {
    }
    std::shared_ptr<llvm::Value> codeGen() override;
    static bool isClassOf(const ExpressionAST *expressionAst)
    {
        return expressionAst->getExpressionType() == ExpressionType::TYPE_REAL_EXPRE;
    };

    static bool classof(const ExpressionAST *expressionAst)
    {
        return isClassOf(expressionAst);
    }

  private:
    double value;
};

class IntegerExpression : public ExpressionAST
{
  public:
    IntegerExpression(const Location &loc, int64_t value, std::shared_ptr<TypeDeclaration> typeDec)
        : value(value), ExpressionAST(loc, ExpressionType::TYPE_INTEGER_EXPRE, typeDec)
    {
    }
    IntegerExpression(const Location &loc, ExpressionType expressionType, int64_t value,
                      std::shared_ptr<TypeDeclaration> typeDec)
        : value(value), ExpressionAST(loc, expressionType, typeDec)
    {
    }
    int64_t getValue()
    {
        return this->value;
    }
    std::shared_ptr<llvm::Value> codeGen() override;
    static bool isClassOf(const ExpressionAST *expressionAst)
    {
        return expressionAst->getExpressionType() == ExpressionType::TYPE_INTEGER_EXPRE ||
               expressionAst->getExpressionType() == ExpressionType::TYPE_CHAR_EXPRE;
    }
    static bool classof(const ExpressionAST *expressionAst)
    {
        return isClassOf(expressionAst);
    }

  protected:
    int64_t value;
};

class CharExpression : public IntegerExpression
{
  public:
    CharExpression(const Location &loc, char values, std::shared_ptr<TypeDeclaration> typeDeclaration)
        : IntegerExpression(loc, ExpressionType::TYPE_CHAR_EXPRE, values, typeDeclaration)
    {
    }
    std::shared_ptr<llvm::Value> codeGen() override;
    static bool isClassOf(const ExpressionAST *expressionAst)
    {
        return expressionAst->getExpressionType() == ExpressionType::TYPE_CHAR_EXPRE;
    }
    static bool classof(const ExpressionAST *expressionAst)
    {
        return isClassOf(expressionAst);
    }
};

class NullExpression : public ExpressionAST
{
  public:
    NullExpression(const Location &loc) : ExpressionAST(loc, ExpressionType::TYPE_NIL_EXPRE)
    {
    } // TODO: Add ExpreType
    std::shared_ptr<llvm::Value> codeGen() override;
    static bool isClassOf(const ExpressionAST *expressionAst)
    {
        return expressionAst->getExpressionType() == ExpressionType::TYPE_NIL_EXPRE;
    }
    static bool classof(const ExpressionAST *expressionAst)
    {
        return isClassOf(expressionAst);
    }
};

class AddressableExpression : public ExpressionAST
{
  public:
    AddressableExpression(const Location &location, ExpressionType expressionType,
                          std::shared_ptr<TypeDeclaration> typeDeclaration)
        : ExpressionAST(location, expressionType, typeDeclaration)
    {
    }
    virtual std::shared_ptr<llvm::Value> getAddress()
    {
        assert(0 && "What?!");
        return 0;
    }
    std::shared_ptr<llvm::Value> codeGen() override;
    virtual const std::string getName() const
    {
        return "";
    }
    static bool isClassOf(const ExpressionAST *expressionAst)
    {
        return expressionAst->getExpressionType() >= ExpressionType::TYPE_ADDRESSABLE_EXPRE &&
               expressionAst->getExpressionType() <= ExpressionType::TYPE_LAST_ADDRESSABLE;
    }
    static bool classof(const ExpressionAST *expressionAst)
    {
        return isClassOf(expressionAst);
    }
};

class StringExpression : public AddressableExpression
{
  public:
    StringExpression(const Location &location, const std::string &value,
                     std::shared_ptr<TypeDeclaration> typeDeclaration)
        : value(value), AddressableExpression(location, ExpressionType::TYPE_STRING_EXPRE, typeDeclaration)
    {
    }
    std::shared_ptr<llvm::Value> codeGen() override;
    std::shared_ptr<llvm::Value> getAddress() override;
    const std::string &getValue() const
    {
        return value;
    }
    static bool isClassOf(const ExpressionAST *expressionAst)
    {
        return expressionAst->getExpressionType() == ExpressionType::TYPE_STRING_EXPRE;
    }

  private:
    std::string value;
};

class SetExpression : public AddressableExpression
{
  public:
    SetExpression(const Location &location, ExpressionType expressionType,
                  std::shared_ptr<TypeDeclaration> typeDeclaration,
                  const std::vector<std::shared_ptr<ExpressionAST>> &values)
        : values(values), AddressableExpression(location, expressionType, typeDeclaration)
    {
    }
    std::shared_ptr<llvm::Value> getAddress() override;
    std::shared_ptr<llvm::Value> makeConstantSet(TypeDeclaration *type);
    static bool isClassOf(const ExpressionAST *expressionAst)
    {
        return expressionAst->getExpressionType() == ExpressionType::TYPE_SET_EXPRE;
    }
    const std::vector<std::shared_ptr<ExpressionAST>> &getValues() const;

  private:
    std::vector<std::shared_ptr<ExpressionAST>> values;
};

class VariableExpression : public AddressableExpression
{
  public:
    VariableExpression(const Location &location, const std::string &name,
                       std::shared_ptr<TypeDeclaration> typeDeclaration)
        : AddressableExpression(location, ExpressionType::TYPE_VARIABLE_EXPRE, typeDeclaration), name(name)
    {
    }
    VariableExpression(const Location &location, ExpressionType expressionType, const std::string &name,
                       std::shared_ptr<TypeDeclaration> typeDeclaration)
        : AddressableExpression(location, expressionType, typeDeclaration), name(name)
    {
    }
    VariableExpression(const Location &location, ExpressionType expressionType,
                       std::shared_ptr<VariableExpression> variableExpre,
                       std::shared_ptr<TypeDeclaration> typeDeclaration)
        : name(variableExpre->name), AddressableExpression(location, expressionType, typeDeclaration)
    {
    }
    const std::string getName() const override
    {
        return name;
    }
    std::shared_ptr<llvm::Value> getAddress() override;
    static bool isClassOf(const ExpressionAST *expressionAst)
    {
        return expressionAst->getExpressionType() >= ExpressionType::TYPE_VARIABLE_EXPRE &&
               expressionAst->getExpressionType() <= ExpressionType::TYPE_LAST_ADDRESSABLE;
    }

  protected:
    std::string name;
};

class ArrayExpression : public AddressableExpression
{
  public:
    ArrayExpression(const Location &loc, std::shared_ptr<ExpressionAST> v,
                    const std::vector<std::shared_ptr<ExpressionAST>> inds,
                    const std::vector<std::shared_ptr<RangeDeclaration>> rangeDeclaration,
                    std::shared_ptr<TypeDeclaration> typeDeclaration);
    std::shared_ptr<llvm::Value> getAddress() override;
    static bool isClassOf(const ExpressionAST *expressionAst)
    {
        return expressionAst->getExpressionType() == ExpressionType::TYPE_ARRAY_EXPRE;
    }

    void accept(ExpressionVisitor &v) override;
    const std::shared_ptr<VariableExpression> &getExpression() const;
    const std::vector<std::shared_ptr<ExpressionAST>> &getIndices() const;
    const std::vector<std::shared_ptr<RangeDeclaration>> &getRanges() const;
    const std::vector<size_t> &getIndexmul() const;

  private:
    std::shared_ptr<VariableExpression> expression;
    std::vector<std::shared_ptr<ExpressionAST>> indices;
    std::vector<std::shared_ptr<RangeDeclaration>> ranges;
    std::vector<size_t> indexmul;
};

class PointerExpression : public AddressableExpression
{
  public:
    PointerExpression(const Location &location, std::shared_ptr<ExpressionAST> variableExpre,
                      std::shared_ptr<TypeDeclaration> typeDeclaration)
        : pointer(variableExpre), AddressableExpression(location, ExpressionType::TYPE_POINTER_EXPRE, typeDeclaration)
    {
    }
    std::shared_ptr<llvm::Value> getAddress() override;
    static bool isClassOf(const ExpressionAST *expressionAst)
    {
        return expressionAst->getExpressionType() == ExpressionType::TYPE_POINTER_EXPRE;
    }
    void accept(ExpressionVisitor &visitor) override;

  private:
    std::shared_ptr<ExpressionAST> pointer;
};

class FilePointerExpression : public AddressableExpression
{
  public:
    FilePointerExpression(const Location &loc, std::shared_ptr<ExpressionAST> varExpre,
                          std::shared_ptr<TypeDeclaration> typeDeclaration)
        : pointer(varExpre), AddressableExpression(loc, ExpressionType::TYPE_FILE_POINTER_EXPRE, typeDeclaration)
    {
    }
    std::shared_ptr<llvm::Value> getAddress() override;
    static bool isClassOf(const ExpressionAST *expressionAst)
    {
        return expressionAst->getExpressionType() == ExpressionType::TYPE_FILE_POINTER_EXPRE;
    }
    void accept(ExpressionVisitor &visitor) override;

  private:
    std::shared_ptr<ExpressionAST> pointer;
};

class FieldExpression : public VariableExpression
{
  public:
    FieldExpression(const Location &loc, std::shared_ptr<VariableExpression> variableExpression, int element,
                    std::shared_ptr<TypeDeclaration> typeDeclaration)
        : element(element), expression(variableExpression),
          VariableExpression(loc, ExpressionType::TYPE_FIELD_EXPRE, variableExpression, typeDeclaration)
    {
    }
    std::shared_ptr<llvm::Value> getAddress() override;
    static bool isClassOf(const ExpressionAST *expressionAst)
    {
        return expressionAst->getExpressionType() == ExpressionType::TYPE_FIELD_EXPRE;
    }
    void accept(ExpressionVisitor &visitor) override;

  private:
    std::shared_ptr<VariableExpression> expression;
    int element;
};

class VariantFieldExpression : public VariableExpression
{
  public:
    VariantFieldExpression(const Location &loc, std::shared_ptr<VariableExpression> variableExpression, int element,
                           std::shared_ptr<TypeDeclaration> typeDeclaration)
        : element(element), expre(variableExpression),
          VariableExpression(loc, ExpressionType::TYPE_VARIANT_EXPRE, variableExpression, typeDeclaration)
    {
    }
    std::shared_ptr<llvm::Value> getAddress() override;
    static bool isClassOf(const ExpressionAST *expressionAst)
    {
        expressionAst->getExpressionType() == ExpressionType::TYPE_VARIANT_EXPRE;
    }

  private:
    std::shared_ptr<VariableExpression> expre;
    int element;
};

class BinaryExpression : public ExpressionAST
{
  public:
    Token &getOper();
    std::shared_ptr<ExpressionAST> &getLhs();
    std::shared_ptr<ExpressionAST> &getRhs();
    BinaryExpression(Token tok, std::shared_ptr<ExpressionAST> lhs, std::shared_ptr<ExpressionAST> rhs)
        : ExpressionAST(tok.getTokenPos(), ExpressionType::TYPE_BINARY_EXPRE), oper(tok), lhs(lhs), rhs(rhs)
    {
    }
    std::shared_ptr<llvm::Value> codeGen() override;
    static bool isClassOf(const ExpressionAST *expressionAst)
    {
        return expressionAst->getExpressionType() == ExpressionType::TYPE_BINARY_EXPRE;
    }
    std::shared_ptr<TypeDeclaration> getTypeDeclaration() const override;
    void accept(ExpressionVisitor &visitor) override
    {
        rhs->accept(visitor);
        lhs->accept(visitor);
        visitor.visit(this);
    }
    static bool classof(const ExpressionAST *expressionAst)
    {
        return isClassOf(expressionAst);
    }

  private:
    std::shared_ptr<llvm::Value> setCodeGen();
    std::shared_ptr<llvm::Value> inlineSetFunc(const std::string &name);
    std::shared_ptr<llvm::Value> callSetFunc(const std::string &name, bool resultTypeIsSet);
    std::shared_ptr<llvm::Value> callStrFunc(const std::string &name);
    std::shared_ptr<llvm::Value> callArrFunc(const std::string &name, size_t size);
    Token oper;
    std::shared_ptr<ExpressionAST> lhs;
    std::shared_ptr<ExpressionAST> rhs;
};

class UnaryExpression : public ExpressionAST
{
  public:
    UnaryExpression(const Location &loc, Token tk, std::shared_ptr<ExpressionAST> expressionAst)
        : ExpressionAST(loc, ExpressionType::TYPE_UNARY_EXPRE), op(tk), rhs(expressionAst){};
    std::shared_ptr<llvm::Value> codeGen() override;
    std::shared_ptr<TypeDeclaration> getTypeDeclaration() const override
    {
        return rhs->getTypeDeclaration();
    }
    static bool isClassOf(const ExpressionAST *expressionAst)
    {
        return expressionAst->getExpressionType() == ExpressionType::TYPE_UNARY_EXPRE;
    }
    void accept(ExpressionVisitor &visitor) override
    {
        rhs->accept(visitor);
        visitor.visit(this);
    }

  private:
    Token op;
    std::shared_ptr<ExpressionAST> rhs;
};

class RangeExpression : public ExpressionAST
{
  public:
    RangeExpression(const Location &loc, std::shared_ptr<ExpressionAST> lhs, std::shared_ptr<ExpressionAST> rhs)
        : ExpressionAST(loc, ExpressionType::TYPE_RANGE_EXPRE), low(lhs), high(rhs)
    {
    }
    std::shared_ptr<TypeDeclaration> getTypeDeclaration() const override
    {
        return low->getTypeDeclaration();
    }
    std::shared_ptr<llvm::Value> getLow();
    std::shared_ptr<llvm::Value> getHigh();
    std::shared_ptr<ExpressionAST> getLowExpression();
    std::shared_ptr<ExpressionAST> getHighExpression();
    static bool isClassOf(const ExpressionAST *expressionAst)
    {
        return expressionAst->getExpressionType() == ExpressionType::TYPE_RANGE_EXPRE;
    }
    static bool classof(const ExpressionAST *expressionAst)
    {
        return isClassOf(expressionAst);
    }
    void accept(ExpressionVisitor &visitor) override
    {
        low->accept(visitor);
        high->accept(visitor);
        visitor.visit(this);
    }

  private:
    std::shared_ptr<ExpressionAST> low;
    std::shared_ptr<ExpressionAST> high;
};

class InitValue : public ExpressionAST
{
  public:
    InitValue(const Location &loc, const std::vector<std::shared_ptr<ExpressionAST>> &v)
        : values(v), ExpressionAST(loc, ExpressionType::TYPE_INIT_VALUE)
    {
    }
    std::shared_ptr<llvm::Value> codeGen() override
    {
        return ExpressionAST::codeGen();
    }
    std::shared_ptr<TypeDeclaration> getTypeDeclaration() const override
    {
        return values[0]->getTypeDeclaration();
    }

  private:
    std::vector<std::shared_ptr<ExpressionAST>> values;
};

class BlockExpression : public ExpressionAST
{
  public:
    BlockExpression(const Location &loc, std::vector<std::shared_ptr<ExpressionAST>> body)
        : ExpressionAST(loc, ExpressionType::TYPE_BLOCK_EXPRE), content(body)
    {
    }
    bool isEmpty()
    {
        return this->content.empty();
    }
    std::shared_ptr<llvm::Value> codeGen() override;
    std::vector<std::shared_ptr<ExpressionAST>> &getBody()
    {
        return this->content;
    }
    static bool isClassOf(ExpressionAST *expressionAst)
    {
        return expressionAst->getExpressionType() == ExpressionType::TYPE_BLOCK_EXPRE;
    }
    void accept(ExpressionVisitor &visitor) override;

  private:
    std::vector<std::shared_ptr<ExpressionAST>> content;
};

class AssignExpression : public ExpressionAST
{
  public:
    AssignExpression(const Location &location, std::shared_ptr<ExpressionAST> &lhs, std::shared_ptr<ExpressionAST> &rhs)
        : ExpressionAST(location, ExpressionType::TYPE_ASSIGN_EXPRE), lhs(lhs), rhs(rhs)
    {
    }
    std::shared_ptr<llvm::Value> codeGen() override;
    static bool isClassOf(const ExpressionAST *expressionAst)
    {
        return expressionAst->getExpressionType() == ExpressionType::TYPE_ASSIGN_EXPRE;
    }
    static bool classof(const ExpressionAST *expressionAst)
    {
        return isClassOf(expressionAst);
    }
    void accept(ExpressionVisitor &visitor) override
    {
        lhs->accept(visitor);
        visitor.visit(this);
        rhs->accept(visitor);
    }
    const std::shared_ptr<ExpressionAST> &getLhs() const;
    const std::shared_ptr<ExpressionAST> &getRhs() const;
    void setLhs(const std::shared_ptr<ExpressionAST> &lhs);
    void setRhs(const std::shared_ptr<ExpressionAST> &rhs);

  private:
    std::shared_ptr<llvm::Value> assignStr();
    std::shared_ptr<llvm::Value> assignSet();
    std::shared_ptr<ExpressionAST> lhs;
    std::shared_ptr<ExpressionAST> rhs;
};

class Function;

class VariableDeclarationExpression : public ExpressionAST
{
  public:
    VariableDeclarationExpression(const Location &loc, std::vector<std::shared_ptr<VariableDefinition>> vars)
        : ExpressionAST(loc, ExpressionType::TYPE_VAR_DECL_EXPRE), vars(vars), function(0)
    {
    }
    std::shared_ptr<llvm::Value> codeGen() override;
    void setFunction(std::shared_ptr<Function> function)
    {
        this->function = function;
    }
    static bool isClassOf(const ExpressionAST *expressionAst)
    {
        return expressionAst->getExpressionType() == ExpressionType::TYPE_VAR_DECL_EXPRE;
    }
    const std::vector<std::shared_ptr<VariableDefinition>> getVars()
    {
        return this->vars;
    }

  private:
    std::vector<std::shared_ptr<VariableDefinition>> vars;
    std::shared_ptr<Function> function;
};

class PrototypeExpression : public ExpressionAST
{
  public:
    PrototypeExpression(const Location &loc, const std::string &name,
                        const std::vector<std::shared_ptr<VariableDefinition>> args,
                        std::shared_ptr<TypeDeclaration> resultType)
        : ExpressionAST(loc, ExpressionType::TYPE_PROTOTYPE, resultType), args(args), name(name), function(0),
          isForward(false), llvmFunction(0)
    {
    }
    std::shared_ptr<llvm::Function> create(const std::string &namePrefix);
    std::shared_ptr<llvm::Function> getLlvmFunction() const
    {
        return llvmFunction;
    }
    void createArgumentsAlloca();
    std::string getName() const
    {
        return this->name;
    }
    const std::vector<std::shared_ptr<VariableDefinition>> getArgs()
    {
        return this->args;
    }
    bool isForwarded() const
    {
        return this->isForward;
    }
    void setIsForward(bool forward)
    {
        this->isForward = forward;
    }
    void setFunction(std::shared_ptr<Function> function)
    {
        this->function = function;
    }
    std::shared_ptr<Function> getFunction() const
    {
        return this->function;
    }
    void addExtraArgsFirst(std::vector<std::shared_ptr<VariableDefinition>> vars);
    bool operator==(const PrototypeExpression &rhs) const;
    bool isMatchWithoutClosure(const PrototypeExpression *rhs) const;
    static bool isClassOf(ExpressionAST *expressionAst)
    {
        return expressionAst->getExpressionType() == ExpressionType::TYPE_PROTOTYPE;
    }

  private:
    std::string name;
    std::vector<std::shared_ptr<VariableDefinition>> args;
    std::shared_ptr<Function> function;
    bool isForward;
    std::shared_ptr<llvm::Function> llvmFunction;
};

class Function : public ExpressionAST
{
  public:
    Function(const Location &loc, std::shared_ptr<PrototypeExpression> prototype,
             const std::vector<std::shared_ptr<VariableDeclarationExpression>> variablesDecl,
             std::shared_ptr<BlockExpression> block);
    std::shared_ptr<llvm::Function> codeGen() const;
    std::shared_ptr<llvm::Function> codeGen(const std::string &namePrefix);
    const std::shared_ptr<PrototypeExpression> getPrototype() const
    {
        return this->prototype;
    }
    void addSubFunctions(const std::vector<std::shared_ptr<Function>> subs)
    {
        subFunctions = subs;
    }
    void setParent(std::shared_ptr<Function> parent)
    {
        this->parent = parent;
    }
    const std::shared_ptr<Function> getParent()
    {
        return this->parent;
    }
    const std::vector<std::shared_ptr<Function>> getSubFunctions()
    {
        return subFunctions;
    }
    void setUsedVars(const std::set<VariableDefinition> &usedVariables)
    {
        this->usedVariables = usedVariables;
    }
    const std::set<VariableDefinition> &getUsedVars()
    {
        return this->usedVariables;
    }
    std::shared_ptr<TypeDeclaration> getReturnType();
    const std::string closureName()
    {
        return "$$CLOSURE";
    }
    static bool isClassOf(const ExpressionAST *expressionAst)
    {
        return expressionAst->getExpressionType() == ExpressionType::TYPE_FUNCTION;
    }
    void accept(ExpressionVisitor &v) override;
    void setEndLoc(Location loc)
    {
        this->endLoc = loc;
    }

  private:
    std::shared_ptr<PrototypeExpression> prototype;
    std::vector<std::shared_ptr<VariableDeclarationExpression>> variablesDecl;
    std::shared_ptr<BlockExpression> body;
    std::vector<std::shared_ptr<Function>> subFunctions;
    std::set<VariableDefinition> usedVariables;
    std::shared_ptr<Function> parent;
    std::shared_ptr<TypeDeclaration> returnType;
    Location endLoc;
};

class FunctionExpression : public VariableExpression
{
  public:
    FunctionExpression(const Location &loc, const std::shared_ptr<PrototypeExpression> p)
        : prototype(p),
          VariableExpression(loc, ExpressionType::TYPE_FUNCTION_EXPRE, p->getName(), p->getTypeDeclaration())
    {
    }
    std::shared_ptr<llvm::Value> codeGen() override;
    const std::shared_ptr<PrototypeExpression> getPrototype() const
    {
        return this->prototype;
    }
    static bool isClassOf(const ExpressionAST *expressionAst)
    {
        return expressionAst->getExpressionType() == ExpressionType::TYPE_FUNCTION_EXPRE;
    }

  private:
    const std::shared_ptr<PrototypeExpression> prototype;
};

class GotoExpression : public ExpressionAST
{
  public:
    GotoExpression(const Location &loc, int pos) : ExpressionAST(loc, ExpressionType::TYPE_GOTO), pos(pos)
    {
    }
    std::shared_ptr<llvm::Value> codeGen() override;
    static bool isClassOf(const ExpressionAST *expressionAst)
    {
        return expressionAst->getExpressionType() == ExpressionType::TYPE_GOTO;
    }

  private:
    int pos;
};

class SizeOfExpression : public ExpressionAST
{
  public:
    SizeOfExpression(const Location &loc, std::shared_ptr<TypeDeclaration> typeDeclaration)
        : ExpressionAST(loc, ExpressionType::TYPE_SIZE_OF_EXPRE, typeDeclaration)
    {
    }
    std::shared_ptr<llvm::Value> codeGen() override;
    std::shared_ptr<TypeDeclaration> getTypeDeclaration() const override
    {
        return getIntegerType();
    }
    static bool isClassOf(const ExpressionAST *expressionAst)
    {
        return expressionAst->getExpressionType() == ExpressionType::TYPE_SIZE_OF_EXPRE;
    }
};

class TypeCastExpression : public AddressableExpression
{
  public:
    TypeCastExpression(const Location &loc, std::shared_ptr<ExpressionAST> expressionAst,
                       std::shared_ptr<TypeDeclaration> typeDeclaration)
        : expr(expressionAst), AddressableExpression(loc, ExpressionType::TYPE_TYPE_CAST_EXPRE, typeDeclaration){};
    std::shared_ptr<llvm::Value> codeGen() override;
    std::shared_ptr<llvm::Value> getAddress() override;
    std::shared_ptr<ExpressionAST> getExpressionn()
    {
        return expr;
    }
    static bool isClassOf(const ExpressionAST *expressionAst)
    {
        return expressionAst->getExpressionType() == ExpressionType::TYPE_TYPE_CAST_EXPRE;
    }
    void accept(ExpressionVisitor &v) override
    {
        expr->accept(v);
        v.visit(this);
    }

  private:
    std::shared_ptr<ExpressionAST> expr;
};

class RangeReduceExpression : public ExpressionAST
{
  public:
    RangeReduceExpression(ExpressionAST *expressionAst, RangeDeclaration *r)
        : ExpressionAST(expressionAst->getLocation(), ExpressionType::TYPE_RANGE_REDUCE_EXPRE,
                        expressionAst->getTypeDeclaration()),
          expr(expressionAst), range(r)
    {
    }
    RangeReduceExpression(ExpressionType expressionType, ExpressionAST *expressionAst, RangeDeclaration *r)
        : ExpressionAST(expressionAst->getLocation(), expressionType, expressionAst->getTypeDeclaration()),
          expr(expressionAst), range(r)
    {
    }
    std::shared_ptr<llvm::Value> codeGen() override;
    void accept(ExpressionVisitor &v) override
    {
        expr->accept(v);
        v.visit(this);
    }
    static bool isClassOf(const ExpressionAST *expressionAst)
    {
        return (expressionAst->getExpressionType() == ExpressionType::TYPE_RANGE_REDUCE_EXPRE) ||
               (expressionAst->getExpressionType() == ExpressionType::TYPE_RANGE_CHECK_EXPRE);
    }

  protected:
    std::shared_ptr<ExpressionAST> expr;
    std::shared_ptr<RangeDeclaration> range;
};

class RangeCheckExpression : public RangeReduceExpression
{
    RangeCheckExpression(ExpressionAST *expressionAst, RangeDeclaration *rangeDecl)
        : RangeReduceExpression(ExpressionType::TYPE_RANGE_CHECK_EXPRE, expressionAst, rangeDecl)
    {
    }
    std::shared_ptr<llvm::Value> codeGen() override;
    static bool isClassOf(const ExpressionAST *expressionAst)
    {
        return expressionAst->getExpressionType() == ExpressionType::TYPE_RANGE_CHECK_EXPRE;
    }
};

class LabelExpression : public ExpressionAST
{
  public:
    LabelExpression(const Location &loc, const std::vector<std::pair<int, int>> &lab,
                    std::shared_ptr<ExpressionAST> stmt)
        : ExpressionAST(loc, ExpressionType::TYPE_LABEL_EXPRE), labelsValues(lab), statement(stmt)
    {
    }
    std::shared_ptr<llvm::Value> codeGen() override;
    std::shared_ptr<llvm::Value> codeGen(std::shared_ptr<llvm::BasicBlock> casebb,
                                         std::shared_ptr<llvm::BasicBlock> afterbb);
    static bool isClassOf(ExpressionAST *expressionAst)
    {
        return expressionAst->getExpressionType() == ExpressionType::TYPE_LABEL_EXPRE;
    }
    void accept(ExpressionVisitor &visitor) override;
    std::vector<std::pair<int, int>> &getLabels()
    {
        return this->labelsValues;
    };

  private:
    std::vector<std::pair<int, int>> labelsValues;
    std::shared_ptr<ExpressionAST> statement;
};

class WithExpression : public ExpressionAST
{
  public:
    WithExpression(const Location &loc, ExpressionAST *body)
        : ExpressionAST(loc, ExpressionType::TYPE_WITH_EXPRE), body(body)
    {
    }
    std::shared_ptr<llvm::Value> codeGen();
    static bool isClassOf(const ExpressionAST *expressionAst)
    {
        return expressionAst->getExpressionType() == ExpressionType::TYPE_WITH_EXPRE;
    }
    void accept(ExpressionVisitor &visitor) override
    {
        body->accept(visitor);
        visitor.visit(this);
    }

  private:
    std::shared_ptr<ExpressionAST> body;
};

class CaseExpression : public ExpressionAST
{
  public:
    CaseExpression(const Location &loc, std::shared_ptr<ExpressionAST> expr,
                   const std::vector<std::shared_ptr<LabelExpression>> &labels, std::shared_ptr<ExpressionAST> other)
        : ExpressionAST(loc, ExpressionType::TYPE_CASE_EXPRE), expre(expr), labels(labels), otherwise(other)
    {
    }
    std::shared_ptr<llvm::Value> codeGen();

    static bool isClassOf(const ExpressionAST *expressionAst)
    {
        return expressionAst->getExpressionType() == ExpressionType::TYPE_CASE_EXPRE;
    }
    static bool classof(const ExpressionAST *expressionAst)
    {
        return isClassOf(expressionAst);
    }
    void accept(ExpressionVisitor &visitor) override;
    const std::shared_ptr<ExpressionAST> &getExpre() const;
    const std::vector<std::shared_ptr<LabelExpression>> &getLabels() const;
    const std::shared_ptr<ExpressionAST> &getOtherwise() const;

  private:
    std::shared_ptr<ExpressionAST> expre;
    std::vector<std::shared_ptr<LabelExpression>> labels;
    std::shared_ptr<ExpressionAST> otherwise;
};

class RepeatExpression : public ExpressionAST
{
  public:
    RepeatExpression(const Location &loc, std::shared_ptr<ExpressionAST> cond, std::shared_ptr<ExpressionAST> body)
        : ExpressionAST(loc, ExpressionType::TYPE_REPEAT_EXPRE), cond(cond), body(body)
    {
    }
    std::shared_ptr<llvm::Value> codeGen() override;
    static bool isClassOf(const ExpressionAST *expressionAst)
    {
        return expressionAst->getExpressionType() == ExpressionType::TYPE_REPEAT_EXPRE;
    }
    void accept(ExpressionVisitor &visitor) override;

  private:
    std::shared_ptr<ExpressionAST> cond;
    std::shared_ptr<ExpressionAST> body;
};

class WhileExpression : public ExpressionAST
{
  public:
    WhileExpression(const Location &loc, std::shared_ptr<ExpressionAST> cond, std::shared_ptr<ExpressionAST> body)
        : ExpressionAST(loc, ExpressionType::TYPE_WHILE_EXPRE), condition(cond), body(body)
    {
    }
    std::shared_ptr<llvm::Value> codeGen() override;
    static bool isClassOf(const ExpressionAST *expressionAst)
    {
        return expressionAst->getExpressionType() == ExpressionType::TYPE_WHILE_EXPRE;
    }
    void accept(ExpressionVisitor &expressionVisitor) override;

  private:
    std::shared_ptr<ExpressionAST> condition;
    std::shared_ptr<ExpressionAST> body;
};

class ForExpression : public ExpressionAST
{
  public:
    ForExpression(const Location &loc, std::shared_ptr<VariableExpression> var, std::shared_ptr<ExpressionAST> start,
                  std::shared_ptr<ExpressionAST> end, bool down, std::shared_ptr<ExpressionAST> body)
        : ExpressionAST(loc, ExpressionType::TYPE_FOR_EXPRE), variable(var), start(start), stepDown(down), end(end),
          body(body)
    {
    }
    ForExpression(const Location &loc, std::shared_ptr<VariableExpression> var, std::shared_ptr<ExpressionAST> start,
                  std::shared_ptr<ExpressionAST> body)
        : ExpressionAST(loc, ExpressionType::TYPE_FOR_EXPRE), variable(var), start(start), stepDown(false),
          end(nullptr), body(body)
    {
    }
    std::shared_ptr<llvm::Value> codeGen() override;
    static bool isClassOf(const ExpressionAST *expressionAst)
    {
        return expressionAst->getExpressionType() == ExpressionType::TYPE_FOR_EXPRE;
    }
    static bool classof(const ExpressionAST *expressionAst)
    {
        return isClassOf(expressionAst);
    }
    void accept(ExpressionVisitor &visitor) override;
    bool isStepDown() const;
    void setStepDown(bool stepDown);
    const std::shared_ptr<VariableExpression> &getVariable() const;
    void setVariable(const std::shared_ptr<VariableExpression> &variable);
    const std::shared_ptr<ExpressionAST> &getStart() const;
    void setStart(const std::shared_ptr<ExpressionAST> &start);
    const std::shared_ptr<ExpressionAST> &getEnd() const;
    void setEnd(const std::shared_ptr<ExpressionAST> &end);
    const std::shared_ptr<ExpressionAST> &getBody() const;
    void setBody(const std::shared_ptr<ExpressionAST> &body);

  private:
    std::shared_ptr<llvm::Value> forInGen();
    bool stepDown;
    std::shared_ptr<VariableExpression> variable;
    std::shared_ptr<ExpressionAST> start;
    std::shared_ptr<ExpressionAST> end;
    std::shared_ptr<ExpressionAST> body;
};

class IfExpression : public ExpressionAST
{
  public:
    IfExpression(const Location &loc, std::shared_ptr<ExpressionAST> cond, std::shared_ptr<ExpressionAST> then,
                 std::shared_ptr<ExpressionAST> other)
        : ExpressionAST(loc, ExpressionType::TYPE_IF_EXPRE), cond(cond), then(then), other(other)
    {
    }
    std::shared_ptr<llvm::Value> codeGen() override;
    static bool isClassOf(const ExpressionAST *expressionAst)
    {
        return expressionAst->getExpressionType() == ExpressionType::TYPE_IF_EXPRE;
    }
    void accept(ExpressionVisitor &visitor) override;

  private:
    std::shared_ptr<ExpressionAST> cond;
    std::shared_ptr<ExpressionAST> then;
    std::shared_ptr<ExpressionAST> other;
};

class ClosureExpression : public ExpressionAST
{
  public:
    ClosureExpression(const Location &loc, std::shared_ptr<TypeDeclaration> ty,
                      std::vector<std::shared_ptr<VariableExpression>> &vf)
        : ExpressionAST(loc, ExpressionType::TYPE_CLOSURE, ty), content(vf)
    {
    }
    std::shared_ptr<llvm::Value> codeGen() override;
    static bool classof(const ExpressionAST *e)
    {
        return e->getExpressionType() == ExpressionType::TYPE_CLOSURE;
    }

  private:
    const std::vector<std::shared_ptr<VariableExpression>> content;
};
class BuiltInExpression : public ExpressionAST
{
  public:
    BuiltInExpression(const Location &loc, std::shared_ptr<FunctionBaseBuiltin> builtin)
        : ExpressionAST(loc, ExpressionType::TYPE_BUILTIN_EXPRE, builtin->getType()), builtin(builtin)
    {
    }
    std::shared_ptr<llvm::Value> codeGen() override;
    static bool isClassOf(const ExpressionAST *expressionAst)
    {
        return expressionAst->getExpressionType() == ExpressionType::TYPE_BUILTIN_EXPRE;
    }
    static bool classof(const ExpressionAST *expressionAst)
    {
        return isClassOf(expressionAst);
    }
    void accept(ExpressionVisitor &visitor) override;

  private:
    std::shared_ptr<FunctionBaseBuiltin> builtin;
};

class CallFunctExpression : public ExpressionAST
{
  public:
    CallFunctExpression(const Location &w, std::shared_ptr<ExpressionAST> calle,
                        std::vector<std::shared_ptr<ExpressionAST>> &args,
                        std::shared_ptr<PrototypeExpression> prototype)
        : ExpressionAST(w, ExpressionType::TYPE_CALL_EXPRE, prototype->getTypeDeclaration()),
          prototypeExpression(prototype), callee(calle), args(args)
    {
    }
    std::shared_ptr<llvm::Value> codeGen() override;
    static bool isClassOf(const ExpressionAST *expressionAst)
    {
        return expressionAst->getExpressionType() == ExpressionType::TYPE_CALL_EXPRE;
    }
    static bool classof(const ExpressionAST *expressionAst)
    {
        return isClassOf(expressionAst);
    }
    const std::shared_ptr<PrototypeExpression> getPrototype()
    {
        return this->prototypeExpression;
    }
    std::shared_ptr<ExpressionAST> getCallee() const
    {
        return callee;
    }
    std::vector<std::shared_ptr<ExpressionAST>> getArgs()
    {
        return this->args;
    }
    void accept(ExpressionVisitor &visitor) override;

  private:
    const std::shared_ptr<PrototypeExpression> prototypeExpression;
    std::shared_ptr<ExpressionAST> callee;
    std::vector<std::shared_ptr<ExpressionAST>> args;
};

class UnitExpression : public ExpressionAST
{
  public:
    UnitExpression(const Location &loc, const std::vector<std::shared_ptr<ExpressionAST>> &c,
                   std::shared_ptr<Function> init)
        : ExpressionAST(loc, ExpressionType::TYPE_UNIT), initFunc(init), code(c){};
    std::shared_ptr<llvm::Value> codeGen() override;
    static bool classof(const ExpressionAST *e)
    {
        return e->getExpressionType() == ExpressionType::TYPE_UNIT;
    }
    void accept(ExpressionVisitor &v) override;

  private:
    std::shared_ptr<Function> initFunc;
    std::vector<std::shared_ptr<ExpressionAST>> code;
};

class WriteExpression : public ExpressionAST
{
  public:
    struct wargs
    {
        wargs() : expr(0), width(0), precision(0)
        {
        }
        std::shared_ptr<ExpressionAST> expr;
        std::shared_ptr<ExpressionAST> width;
        std::shared_ptr<ExpressionAST> precision;
    };

    WriteExpression(const Location &loc, std::shared_ptr<VariableExpression> out, const std::vector<wargs> &args,
                    bool isWriteToLin)
        : ExpressionAST(loc, ExpressionType::TYPE_WRITE), output(out), args(args), isWriteToLn(isWriteToLin)
    {
    }
    std::shared_ptr<llvm::Value> codeGen() override;
    static bool isClassOf(const ExpressionAST *expressionAst)
    {
        return expressionAst->getExpressionType() == ExpressionType::TYPE_WRITE;
    }
    static bool classof(const ExpressionAST *expressionAst)
    {
        return isClassOf(expressionAst);
    }
    void accept(ExpressionVisitor &expressionVisitor) override;

  private:
    std::shared_ptr<VariableExpression> output;
    std::vector<wargs> args;
    bool isWriteToLn;
};

class ReadExpression : public ExpressionAST
{
  public:
    ReadExpression(const Location &w, AddressableExpression *fi, const std::vector<std::shared_ptr<ExpressionAST>> &a,
                   bool isLn)
        : ExpressionAST(w, ExpressionType::TYPE_READ), file(fi), args(a), isReadln(isLn)
    {
    }
    std::shared_ptr<llvm::Value> codeGen() override;
    static bool classof(const ExpressionAST *e)
    {
        return e->getExpressionType() == ExpressionType::TYPE_READ;
    }
    void accept(ExpressionVisitor &v) override;

  private:
    std::shared_ptr<AddressableExpression> file;
    std::vector<std::shared_ptr<ExpressionAST>> args;
    bool isReadln;
};

class TrampolineExpression : public FunctionExpression
{
  public:
    TrampolineExpression(const Location &w, std::shared_ptr<FunctionExpression> fn,
                         std::shared_ptr<ClosureExpression> c, std::shared_ptr<FunctionPointerDeclaration> fnPtrTy)
        : FunctionExpression(w, fn->getPrototype()), func(fn), closure(c), funcPtrTy(fnPtrTy)
    {
    }
    std::shared_ptr<llvm::Value> codeGen() override;
    static bool classof(const ExpressionAST *e)
    {
        return e->getExpressionType() == ExpressionType::TYPE_TRAMPOLINE;
    }
    void accept(ExpressionVisitor &v) override;

  private:
    std::shared_ptr<FunctionExpression> func;
    std::shared_ptr<ClosureExpression> closure;
    std::shared_ptr<FunctionPointerDeclaration> funcPtrTy;
};
} // namespace Pascal
#endif // JAPC_EXPRESSION_H
