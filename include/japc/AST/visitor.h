//
// Created by wuser on 19/09/22.
//

#ifndef JAPC_VISITOR_H
#define JAPC_VISITOR_H

#include "expression.h"
namespace Pascal{
class Visitor{
  public:
    virtual void visitVarAssignment();
    virtual void visitConstDeclaration();
};
}
#endif // JAPC_VISITOR_H
