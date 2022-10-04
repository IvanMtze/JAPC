//
// Created by wuser on 17/09/22.
//

#ifndef JAPC_PARSER_H
#define JAPC_PARSER_H
#include "expression.h"
#include "scanner.h"
#include <deque>
#include <list>
#include "function.h"
#include "procedure.h"
#include "type.h"
#include "program.h"
#include <vector>
#include "statement.h"
#include "diagnostics.h"

namespace Pascal
{
enum class SyncParserContext{
    functionDeclaration,
    functionParamList
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
    std::unique_ptr<MainExpression> parseExpression();
    std::unique_ptr<Factor> parseFactor();
    std::unique_ptr<SimpleExpression> parseSimpleExpression();
    std::unique_ptr<Term> parseTerm();
    std::unique_ptr<Primary> parsePrimary();
    bool isSign(TokenType tk);
    bool isRelationalOperator(TokenType tk);
    bool isAddingOperator(TokenType tk);
    bool isExponentiatingOperator(TokenType tk);
    bool isMultiplyingOperator(TokenType tk);
    void parseProgram();
    std::unique_ptr<VariableAccess> parseIdentifier();
    void parseProgramComponent();
    void parseMainProgramDeclaration();
    void parseProgramHeading(Program& program);
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
    std::unique_ptr<Function> parseFunctionDeclaration();
    std::unique_ptr<std::vector<FunctionParameter>> parseFunctionParams();
    AccessModifier parseFunctionAccessModifier();
    FunctionParameter parseFunctionParameter();
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
    std::unique_ptr<Function> parseFunction();
    void parseProcedure();
};

} // namespace Pascal
#endif // JAPC_PARSER_H
