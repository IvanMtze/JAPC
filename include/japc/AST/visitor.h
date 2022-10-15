//
// Created by wuser on 12/10/22.
//

#ifndef JAPC_VISITOR_H
#define JAPC_VISITOR_H
namespace Pascal
{
template <typename T> class Visitor
{
  public:
    virtual void visit(T *elem) = 0;
    virtual ~Visitor(){};
};

template <typename T> class Visitable
{
  public:
    virtual void accept(Visitor<T> &visitor) = 0;
    virtual ~Visitable()
    {
    }
};

class ExpressionAST;
using ExpressionVisitor = Visitor<ExpressionAST>;
} // namespace Pascal
#endif // JAPC_VISITOR_H
