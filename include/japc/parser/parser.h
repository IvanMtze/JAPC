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
#include "japc/basic/ptr.h"
#include "parser_utils.h"
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
    std::shared_ptr<Stack<std::shared_ptr<NamedObject>>> objects;
    std::unique_ptr<Scanner> scanner;
    int currentTokenPos;
    std::string moduleName;
    std::unique_ptr<JAPCDiagnostics> diagnosticsEngine;
    std::unique_ptr<std::vector<Token>> tokenList;
    std::unique_ptr<Token> lookAhead(const int num);
    bool compareAhead(const int num, TokenType tokenType);
    void sync();
    void sync(std::vector<TokenType> tokensToFind);
    std::unique_ptr<Token> advance();
    std::unique_ptr<Token> current();
    std::unique_ptr<Token> previous();
    bool isAtEnd();
    bool match(TokenType tokenType);
    bool match(std::vector<TokenType> tokensToMatch);
    void parseExpression();
    void parseFactor();
    void parseLabel();
    std::shared_ptr<InitValue> parseInitValue(std::shared_ptr<TypeDeclaration> typeOfInitialization);
    std::shared_ptr<VariableDeclarationExpression> parseVarDeclarations();
    void parseSimpleExpression();
    void parseTerm();
    std::shared_ptr<ArrayDeclaration> parseArrayDeclaration();
    void parsePrimary();
    bool isSign(TokenType tk);
    bool isRelationalOperator(TokenType tk);
    bool isAddingOperator(TokenType tk);
    bool isExponentiatingOperator(TokenType tk);
    std::shared_ptr<ConstantDeclaration> parseConstantExpression(std::vector<TokenType> terminator);
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
    std::shared_ptr<ConstantDeclaration> parseConstantTerm();
    std::shared_ptr<ConstantDeclaration> parseConstantRightSide(int precedence,
                                                                std::shared_ptr<ConstantDeclaration> leftSide);
    void parseTypeDefinitionPart();
    std::shared_ptr<TypeDeclaration> parseType();
    VariableDeclarationExpression *parseVariableDeclarationPart();
    std::shared_ptr<RangeDeclaration> parseRangeDeclaration(std::shared_ptr<TypeDeclaration> type, TokenType end,
                                                            TokenType alternative);
    void parseProcedureAndDefinitionPart();
    void parseProcedureDeclaration();
    std::shared_ptr<PrototypeExpression> parseFunctionDeclaration();
    std::shared_ptr<PrototypeExpression> parseFunctionHeader();
    std::vector<std::shared_ptr<VariableDefinition>> parseFunctionParams();
    std::vector<std::shared_ptr<VariableDefinition>> parseFunctionParameter(VariableDefinitionFlags &flags,
                                                                            std::vector<std::string> &names);
    void parseVariableAccess();
    void parseUnsignedConstant();
    std::shared_ptr<FileDeclaration> parseFileDeclaration();
    void parseFunctionAccess();
    void parseParenthesizedExpression();
    void parseNegatedPrimary();
    std::shared_ptr<EnumDeclaration> parseEnumDefinition();
    std::shared_ptr<SetDeclaration> parseSetDeclaration();
    void parseConstantAccess();
    void parseSchemaDiscriminant();
    std::shared_ptr<RangeDeclaration> parseArrayDeclarationRange();
    std::shared_ptr<TypeDeclaration> getTypeDeclaration(std::string nameToSearch);
    void parseStructuredValueConstructor();
    void parseDiscriminantIdentifier();
    void parseConstantDefinition();
    std::shared_ptr<StringDeclaration> parseStringDeclaration();
    void parseFunction();
    std::shared_ptr<FunctionPointerDeclaration> parseFunctionType();
    void parseProcedure();
};

} // namespace Pascal
#endif // JAPC_PARSER_H
