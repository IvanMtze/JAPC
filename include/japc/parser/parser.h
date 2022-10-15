//
// Created by wuser on 17/09/22.
//

#ifndef JAPC_PARSER_H
#define JAPC_PARSER_H
#include "japc/AST/expression.h"
#include "japc/AST/type.h"
#include "japc/basic/diagnostics.h"
#include "japc/basic/ptr.h"
#include <deque>
#include <list>
#include <vector>

namespace Pascal
{
enum class SyncParserContext
{
    functionDeclaration,
    functionParamList,
};
class Parser
{
  public:
    Parser(std::unique_ptr<Scanner> scanner, std::unique_ptr<JAPCDiagnostics> diagnosticsEngine);
    void parseFile();

  private:
    std::unique_ptr<Scanner> scanner;
    int currentTokenPos;
    std::unique_ptr<JAPCDiagnostics> diagnosticsEngine;
    std::unique_ptr<std::vector<Token>> tokenList;
    std::unique_ptr<Token> lookAhead(const int num);
    bool compareAhead(const int num, TokenType tokenType);
    void sync();
    std::unique_ptr<Token> advance();
    std::unique_ptr<Token> current();
    std::unique_ptr<Token> previous();
    bool isAtEnd();
    bool match(TokenType tokenType);
    bool match(std::vector<TokenType> tokensToMatch);
    void parseExpression();
    void parseFactor();
    void parseSimpleExpression();
    void parseTerm();
    void parsePrimary();
    bool isSign(TokenType tk);
    bool isRelationalOperator(TokenType tk);
    bool isAddingOperator(TokenType tk);
    bool isExponentiatingOperator(TokenType tk);
    bool isMultiplyingOperator(TokenType tk);
    void parseProgram();
    void parseIdentifier();
    void parseProgramComponent();
    void parseMainProgramDeclaration();
    void parseProgramHeading();
    void parseMainProgramBlock();
    void parseProgramParameterList();
    void parseImportPart();
    void parseImportStatement();
    void parseImportList();
    void parseIdentifierList();
    void parseConstantDefinitionPart();
    void parseTypeDefinitionPart();
    void parseVariableDeclarationPart();
    void parseProcedureAndDefinitionPart();
    void parseProcedureDeclaration();
    void parseFunctionDeclaration();
    void parseFunctionParams();
    void parseFunctionAccessModifier();
    void parseFunctionParameter();
    void parseVariableAccess();
    void parseUnsignedConstant();
    void parseSetConstructor();
    void parseFunctionAccess();
    void parseParenthesizedExpression();
    void parseNegatedPrimary();
    void parseConstantAccess();
    void parseSchemaDiscriminant();
    void parseStructuredValueConstructor();
    void parseDiscriminantIdentifier();
    void parseFunction();
    void parseProcedure();
};

} // namespace Pascal
#endif // JAPC_PARSER_H
