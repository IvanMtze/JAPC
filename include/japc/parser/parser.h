//
// Created by wuser on 17/09/22.
//

#ifndef JAPC_PARSER_H
#define JAPC_PARSER_H
#include "japc/AST/expression.h"
#include "japc/AST/named_object.h"
#include "japc/AST/stack.h"
#include "japc/AST/type.h"
#include "japc/basic/diagnostics.h"
#include "parser_utils.h"
#include <cfloat>
#include <deque>
#include <errno.h>
#include <limits>
#include <list>
#include <stdlib.h>
#include <vector>

#define _CUR_TOKEN_TYPE_ current()->getTokenType()
#define _CUR_TOKEN_OBJ_ *current().get()

namespace Pascal
{
enum class ParserState
{
    ERROR_RECOVERING,
    ERROR_EOF,
    SEMICOLON_EATED,
    OK
};
class Parser
{
  public:
    Parser(std::shared_ptr<Scanner> scanner, std::shared_ptr<JAPCDiagnostics> diagnosticsEngine,
           std::shared_ptr<Stack<std::shared_ptr<NamedObject>>> &stack);
    std::shared_ptr<ExpressionAST> parseFile();

  private:
    ParserState currentState = ParserState::OK;
    std::shared_ptr<Stack<std::shared_ptr<NamedObject>>> objects;
    std::shared_ptr<Scanner> scanner;
    int currentTokenPos;
    std::string moduleName;
    std::shared_ptr<JAPCDiagnostics> diagnosticsEngine;
    std::unique_ptr<std::vector<Token>> tokenList;
    std::vector<std::shared_ptr<ExpressionAST>> ast;
    std::unique_ptr<Token> lookAhead(const int num);
    void sync();
    void sync(std::vector<TokenType> tokensToFind);
    std::unique_ptr<Token> advance();
    std::unique_ptr<Token> current();
    std::unique_ptr<Token> previous();
    bool isAtEnd();
    bool parseArgs(std::shared_ptr<NamedObject> def, std::vector<std::shared_ptr<ExpressionAST>> &args);
    std::shared_ptr<ExpressionAST> parseExpression();
    std::shared_ptr<ExpressionAST> parseExpressionTerm();
    std::shared_ptr<ExpressionAST> parseDefaultExpression();
    std::shared_ptr<ExpressionAST> parseIntegerExpression(Token tk);
    std::shared_ptr<ExpressionAST> parseStringExpression();
    std::shared_ptr<ExpressionAST> parseParenExpression();
    std::shared_ptr<ExpressionAST> parseSetExpression();
    std::shared_ptr<ExpressionAST> parseUnaryOperatorExpression();
    std::shared_ptr<ExpressionAST> parseSizeOfExpression();
    std::shared_ptr<ExpressionAST> parseBinaryOperatorRightSide(int expressionPrecedence,
                                                                std::shared_ptr<ExpressionAST> leftSideExpression);
    void parseLabel();
    std::shared_ptr<InitValue> parseInitValue(std::shared_ptr<TypeDeclaration> typeOfInitialization);
    std::shared_ptr<VariableDeclarationExpression> parseVarDeclarations();
    std::shared_ptr<ArrayDeclaration> parseArrayDeclaration();
    std::shared_ptr<ExpressionAST> parsePrimary();
    std::shared_ptr<ConstantDeclaration> parseConstantExpression(std::vector<TokenType> terminator);
    std::shared_ptr<ExpressionAST> parseProgram();
    std::shared_ptr<ExpressionAST> parseIdentifierExpression(Token tk);
    std::shared_ptr<ExpressionAST> parseIfExpr();
    std::shared_ptr<ExpressionAST> parseForExpr();
    std::shared_ptr<ExpressionAST> parseWhile();
    std::shared_ptr<ExpressionAST> parseRepeat();
    std::shared_ptr<ExpressionAST> parseCaseExpr();
    std::shared_ptr<ExpressionAST> parseWithBlock();
    std::shared_ptr<ExpressionAST> parseGoto();
    std::shared_ptr<TypeDeclaration> parseSimpleType();
    std::shared_ptr<ExpressionAST> parseFunctionCallOrVariableExpression(Token tk);
    std::shared_ptr<Function> parseProgramBlock();
    std::shared_ptr<ExpressionAST> parseMainProgramDeclaration();
    void parseProgramHeading();
    std::shared_ptr<ExpressionAST> parseMainProgramBlock();
    void parseProgramParameterList();
    void parseImportPart();
    void parseImportStatement();
    std::shared_ptr<ExpressionAST> createCallExpression(const std::shared_ptr<NamedObject> def,
                                                        const std::string &funcName,
                                                        std::vector<std::shared_ptr<ExpressionAST>> &args);
    void parseImportList();
    void parseIdentifierList();
    std::shared_ptr<ConstantDeclaration> parseConstantTerm();
    std::shared_ptr<ConstantDeclaration> parseConstantRightSide(int precedence,
                                                                std::shared_ptr<ConstantDeclaration> leftSide);
    void parseTypeDefinitionPart();
    std::shared_ptr<TypeDeclaration> parseType();
    std::shared_ptr<VariableDeclarationExpression> parseVariableDeclarationPart();
    std::shared_ptr<RangeDeclaration> parseRangeDeclaration(std::shared_ptr<TypeDeclaration> &type, TokenType end,
                                                            TokenType alternative);
    std::shared_ptr<PrototypeExpression> parseFunctionHeader();
    std::shared_ptr<PrototypeExpression> parseProcedureHeader();
    std::vector<std::shared_ptr<VariableDefinition>> parseFunctionParams();
    std::vector<std::shared_ptr<VariableDefinition>> parseFunctionParameter(VariableDefinitionFlags &flags,
                                                                            std::vector<std::string> &names);
    std::shared_ptr<ExpressionAST> parseArrayExpression(std::shared_ptr<ExpressionAST> expression,
                                                        std::shared_ptr<TypeDeclaration> type);
    std::shared_ptr<ExpressionAST> parsePointerExpression(std::shared_ptr<ExpressionAST> expression,
                                                          std::shared_ptr<TypeDeclaration> type);
    std::shared_ptr<ExpressionAST> parseFieldExpression(std::shared_ptr<ExpressionAST> expression,
                                                        std::shared_ptr<TypeDeclaration> type);
    std::shared_ptr<FileDeclaration> parseFileDeclaration();
    std::shared_ptr<BlockExpression> parseBlock();
    std::shared_ptr<EnumDeclaration> parseEnumDefinition();
    std::shared_ptr<SetDeclaration> parseSetDeclaration();
    std::shared_ptr<RangeDeclaration> parseArrayDeclarationRange();
    std::shared_ptr<TypeDeclaration> getTypeDeclaration(std::string nameToSearch);
    std::shared_ptr<ExpressionAST> parseStatement();
    void parseConstantDefinition();
    std::shared_ptr<StringDeclaration> parseStringDeclaration();
    std::shared_ptr<Function> parseFunction();
    std::shared_ptr<FunctionPointerDeclaration> parseFunctionType();
    std::shared_ptr<Function> parseProcedure();
    std::shared_ptr<ExpressionAST> parseVariableExpression(std::shared_ptr<NamedObject> namedObject);
};

} // namespace Pascal
#endif // JAPC_PARSER_H
