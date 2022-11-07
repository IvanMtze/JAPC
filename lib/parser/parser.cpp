//
// Created by wuser on 17/09/22.
//
#include "japc/parser/parser.h"
using namespace Pascal;

// NOTE:
// ACCORDING TO ISO 10206 - 3.5 TABLE 1
// THE GRAMMAR DEFINITIONS FOLLOWS THE FOLLOWING SYMBOLOGY DEFINITION.
//
// =            SHALL BE DEFINED TO BE
// >            SHALL HAVE AS AN ALTERNATIVE DEFINITION
// |            ALTERNATIVELY
// .            END OF DEFINITION
// [ X ]        0 OR 1 INSTANCE OF X
// { X }        0 OR MORE INSTANCES OF X
// ( X | Y )    GROUPING: EITHER OF X OR Y
// 'XYZ'        THE TERMNAL SYMBOL XYZ
//

Parser::Parser(std::shared_ptr<Scanner> scanner, std::unique_ptr<JAPCDiagnostics> diagnosticsEngine)
{
    this->scanner = scanner;
    this->diagnosticsEngine = std::move(diagnosticsEngine);
}
void Parser::parseFile()
{
    currentTokenPos = 0;
    std::vector<Token> vector;
    tokenList = std::make_unique<std::vector<Token>>(vector);
    do
    {
        TokenType t = scanner->scan();
        Token tok = scanner->getCurrentTokenObject();
        tokenList->push_back(tok);
    } while (scanner->getCurrentTokenObject().getTokenType() != TokenType::END_OF_FILE);
    parseProgram();
}
bool Parser::compareAhead(const int num, TokenType tokenType)
{
}
std::unique_ptr<Token> Parser::advance()
{
    std::unique_ptr<Token> old = current();
    if (!isAtEnd())
        currentTokenPos++;
    else
    {
        //  TODO: Error unexpected EOF while parsing
    }
    return old;
}
std::unique_ptr<Token> Parser::current()
{
    if (currentTokenPos < tokenList->size())
    {
        return std::make_unique<Token>(tokenList->at(currentTokenPos));
    }
    return nullptr;
}
bool Parser::isAtEnd()
{
    return current()->getTokenType() == TokenType::END_OF_FILE;
}
std::unique_ptr<Token> Parser::lookAhead(const int num)
{
    if (tokenList->size() <= currentTokenPos + num)
    {
        return nullptr;
    }
    return std::make_unique<Token>(tokenList->at(currentTokenPos + num));
}
bool Parser::match(Pascal::TokenType tokenType)
{
}
std::unique_ptr<Token> Parser::previous()
{
    if (currentTokenPos > 0)
    {
        return std::make_unique<Token>(tokenList->at(currentTokenPos - 1));
    }
}
void Parser::sync()
{

    std::vector<TokenType> tk;
    tk.push_back(TokenType::SYMBOL_FUNCTION);
    tk.push_back(TokenType::SYMBOL_BEGIN);
    tk.push_back(TokenType::SYMBOL_VAR);
    tk.push_back(TokenType::SYMBOL_TYPE);
    tk.push_back(TokenType::SYMBOL_PROCEDURE);
    tk.push_back(TokenType::SYMBOL_EXTERNAL);
    tk.push_back(TokenType::SYMBOL_LABEL);
    tk.push_back(TokenType::SYMBOL_GOTO);
    while (!ParserUtils::isAnyOf(*current().get(), tk))
    {
        advance();
        if (current()->getTokenType() == TokenType::END_OF_FILE)
        {
            this->diagnosticsEngine->japc_error_at(*current().get(), "Unexpected EOF");
            this->currentState = ParserState::ERROR_EOF;
            break;
        }
    }
    if (this->currentState != ParserState::ERROR_EOF)
    {
        this->currentState = ParserState::ERROR_RECOVERING;
    }
    throw this->currentState;
}

bool Parser::match(std::vector<TokenType> tokensToMatch)
{
}
std::shared_ptr<ExpressionAST> Parser::parseExpression()
{
    //  ISO 10206 -  6.8.1
    //  EXPRESSION = SIMPLE-EXPRESSION [ RELATIONAL-OPERATOR SIMPLE-EXPRESSION ]
    parseSimpleExpression();
    if (isRelationalOperator(current()->getTokenType()))
    {
        current()->getTokenType();
        advance();
        // TODO: THROW ERROR IF NOT SIMPLE-EXPRESSION
        parseSimpleExpression();
    }
}
std::shared_ptr<ExpressionAST> Parser::parsePrimary()
{
}
void Parser::parseFactor()
{
    //  ISO 10206 -  6.8.1
    //  FACTOR = PRIMARY [ EXPONENTIATING-OPERATOR PRIMARY ]
    parsePrimary();
    if (isExponentiatingOperator(current()->getTokenType()))
    {
        std::make_unique<TokenType>(current()->getTokenType());
        advance();
        //  TODO: THROW ERROR IF NOT PRIMARY AND SYNC
        parsePrimary();
    }
}
void Parser::parseSimpleExpression()
{
    //  ISO 10206 -  6.8.1
    // SIMPLE-EXPRESSION = [ SIGN ] TERM [ ADDING-OPERATOR TERM ]

    if (isSign(current()->getTokenType()))
    {
        current()->getTokenType();
        advance();
    } //  TODO: May assume positive sign?

    parseTerm();
    // PARSE OPTINAL [ADDING-OPERATOR TERM]
    if (isAddingOperator(current()->getTokenType()))
    {
        // Consume addingOperator
        current()->getTokenType();
        advance();
        // SEARCH FOR TERM
        parseTerm();
        //  TODO: Throw error if term is not found and sync
    }
}

void Parser::parseTerm()
{
    //  ISO 10206 -  6.8.1
    // TERM = FACTOR { MULTIPLYING-OPERATOR FACTOR }
    parseFactor();
    while (isMultiplyingOperator(current()->getTokenType()))
    {
        current()->getTokenType();
        advance();
        // TODO: THROW error if no factor is found and sync
        parseFactor();
    }
}
bool Parser::isSign(TokenType tk)
{
    return tk == TokenType::SYMBOL_MINUS || tk == TokenType::SYMBOL_PLUS;
}

bool Parser::isRelationalOperator(TokenType tk)
{
    return tk == TokenType::SYMBOL_EQUAL || tk == TokenType::SYMBOL_LESS_GREATER_THAN ||
           tk == TokenType::SYMBOL_LESS_THAN || tk == TokenType::SYMBOL_GREATER_THAN ||
           tk == TokenType::SYMBOL_LESS_EQUAL_THAN || tk == TokenType::SYMBOL_GREATER_EQUAL_THAN ||
           tk == TokenType::SYMBOL_IN;
}
bool Parser::isAddingOperator(TokenType tk)
{
    return tk == TokenType::SYMBOL_PLUS || tk == TokenType::SYMBOL_MINUS || tk == TokenType::SYMBOL_GREATER_LESS_THAN ||
           tk == TokenType::SYMBOL_OR || tk == TokenType::SYMBOL_OR_ELSE;
}
bool Parser::isExponentiatingOperator(TokenType tk)
{
    return tk == TokenType::SYMBOL_STAR_STAR || tk == TokenType::SYMBOL_POW;
}
bool Parser::isMultiplyingOperator(TokenType tk)
{
    return tk == TokenType::SYMBOL_STAR || tk == TokenType::SYMBOL_SLASH || tk == TokenType::SYMBOL_DIV ||
           tk == TokenType::SYMBOL_MOD || tk == TokenType::SYMBOL_AND || tk == TokenType::SYMBOL_AND_THEN;
}
void Parser::parseProgram()
{
    //  ISO 10206 -  6.12
    try
    {
        parseMainProgramDeclaration();
    }
    catch (ParserState parserState)
    {
        return;
    }
}
void Parser::parseProgramComponent()
{
}
void Parser::parseMainProgramDeclaration()
{
    parseProgramHeading();
    if (current()->getTokenType() != TokenType::SYMBOL_SEMICOLON)
    {
        //  TODO: Sync error missing semicolon after program header
        this->diagnosticsEngine->japc_error_at(*current().get(), "Expected semicolon");
    }
    else
    {
        advance();
    }
    if (isAtEnd())
    {
        // TODO: Error unexpected end of file
        this->diagnosticsEngine->japc_error_at(*current().get(), "Unexpected EOF");
        this->sync();
    }
    parseImportPart();
    if (isAtEnd())
    {
        // TODO: Error unexpected end of file
        this->diagnosticsEngine->japc_error_at(*current().get(), "Unexpected EOF");
        throw this->currentState;
    }
    parseMainProgramBlock();
}
void Parser::parseProgramHeading()
{
    if (current()->getTokenType() != TokenType::SYMBOL_PROGRAM)
    {
        //  TODO: Missing program declaration error, do not sync
        this->diagnosticsEngine->japc_error_at(*current().get(), "Missing program declaration here!");
        this->sync();
    }
    else
    {
        advance(); // eat PROGRAM keyword
        if (current()->getTokenType() != TokenType::IDENTIFIER)
        {
            //  TODO: Missing program identifier and sync
            this->diagnosticsEngine->japc_error_at(*current().get(), "Expected program keyword here!");
            this->sync();
        }
        else
        {
            this->moduleName = current()->getValue();
            advance();
            if (current()->getTokenType() == TokenType::SYMBOL_PAREN_OPEN)
            {
                parseProgramParameterList();
                if (current()->getTokenType() != TokenType::SYMBOL_PAREN_CLOSE)
                {
                    //  TODO: Error missing paren close of program param list
                    this->diagnosticsEngine->japc_error_at(*current().get(), "Expected ')' after program param list");
                    sync();
                }
                advance();
            }
        }
    }
}
void Parser::parseMainProgramBlock()
{
    std::vector<std::shared_ptr<ExpressionAST>> expressions;
    while (!isAtEnd() && current()->getTokenType() != TokenType::SYMBOL_BEGIN)
    {
        try
        {
            std::shared_ptr<ExpressionAST> currentExpr;
            switch (current()->getTokenType())
            {
            case TokenType::SYMBOL_LABEL:
                parseLabel();
            case TokenType::SYMBOL_TYPE:
                parseTypeDefinitionPart();
                break;
            case TokenType::SYMBOL_VAR:
                currentExpr = parseVarDeclarations();
                break;
            case TokenType::SYMBOL_CONST:
                parseConstantDefinition();
                break;
            case TokenType::SYMBOL_PROCEDURE:
                parseProcedure();
                break;
            case TokenType::SYMBOL_FUNCTION:
                parseFunction();
                break;
            default:
                //  TODO: ERROR ??
                sync();
                break;
            }
            if (currentExpr)
            {
                expressions.push_back(currentExpr);
            }
        }
        catch (ParserState state)
        {
            if (state == ParserState::ERROR_EOF)
            {
                throw this->currentState;
            }
            else
            {
                this->currentState = ParserState::OK;
            }
        }
    }
    if (isAtEnd())
    {
        //  TODO: THROW ERROR NO MAIN PROGRAM BLOCK DECLARATION FOUND, END OF FILE REACHED INSTEAD.
    }
}
void Parser::parseImportPart()
{
    while (current()->getTokenType() == TokenType::SYMBOL_IMPORT)
    {
        this->parseImportStatement();
    }
}
void Parser::parseTypeDefinitionPart()
{
    std::vector<std::shared_ptr<PointerDeclaration>> incompletes;
    if (current()->getTokenType() != TokenType::SYMBOL_TYPE)
    {
        this->diagnosticsEngine->japc_error_at(*current().get(), "Expected TYPE keyword");
        this->sync();
        return;
    }
    advance();
    do
    {
        if (current()->getTokenType() != TokenType::IDENTIFIER)
        {
            this->diagnosticsEngine->japc_error_at(*current().get(), "Expected IDENTIFIER");
            this->sync();
            return;
        }
        std::string name = current()->getValue();
        advance();
        if (current()->getTokenType() != TokenType::SYMBOL_EQUAL)
        {
            this->diagnosticsEngine->japc_error_at(*current().get(), "Expected '=' after type name");
            this->sync();
        }
        advance();
        bool restricted = false;
        if (current()->getTokenType() == TokenType::SYMBOL_RESTRICTED)
        {
            restricted = true;
            advance();
        }
        std::shared_ptr<TypeDeclaration> typeDeclaration = parseType();
        if (typeDeclaration)
        {
            std::shared_ptr<ExpressionAST> initial;
            if (current()->getTokenType() == TokenType::SYMBOL_VALUE)
            {
                advance();
                initial = parseInitValue(typeDeclaration);
                if (!initial)
                {
                    this->sync();
                    return;
                }
                typeDeclaration = ParserUtils::copyWithInitialValue(typeDeclaration, initial);
                if (!this->objects->insert(name, std::make_shared<TypeDefinition>(name, typeDeclaration, restricted)))
                {
                    this->diagnosticsEngine->japc_error_at(*current().get(), "%s is already declared");
                    return;
                }
                if (typeDeclaration->getTypeKind() == TypeKind::TYPE_POINTER &&
                    llvm::dyn_cast_or_null<PointerDeclaration>(typeDeclaration.get())->isIncomplete())
                {
                    const PointerDeclaration *pointerDeclaration =
                        llvm::dyn_cast_or_null<PointerDeclaration>(typeDeclaration.get());
                    incompletes.push_back(std::make_shared<PointerDeclaration>(*pointerDeclaration));
                }
                if (current()->getTokenType() != TokenType::SYMBOL_SEMICOLON)
                {
                    this->diagnosticsEngine->japc_error_at(*current().get(), "Expected ';' ");
                    this->sync();
                    return;
                }
            }
        }
        else
        {
            this->diagnosticsEngine->japc_error_at(*current().get(), "Missing type declaration");
            this->sync();
            return;
        }
    } while (current()->getTokenType() == TokenType::IDENTIFIER);
    for (auto incomplete : incompletes)
    {
        std::string name = incomplete->getSubtype()->getName();
        if (std::shared_ptr<TypeDeclaration> type = getTypeDeclaration(name))
        {
            if (type->isIncomplete())
            {
                this->diagnosticsEngine->japc_error_at(*current().get(), "Forwarded type %s is incomplete.",
                                                       name.c_str());
                return;
            }
            incomplete->SetSubType(type);
        }
        else
        {
            this->diagnosticsEngine->japc_error_at(*current().get(),
                                                   "Forwarded declared pointer type %s is not declared.", name.c_str());
        }
    }
}
std::shared_ptr<VariableDeclarationExpression> Parser::parseVariableDeclarationPart()
{
    advance(); // eat VAR keyword
    while (current()->getTokenType() == TokenType::IDENTIFIER)
    {
        parseIdentifierList();
        advance();
        if (current()->getTokenType() == TokenType::SYMBOL_COLON)
        {
            advance();
            if (current()->getTokenType() == TokenType::IDENTIFIER)
            {
                advance();
                if (current()->getTokenType() == TokenType::SYMBOL_SEMICOLON)
                {
                    advance();
                }
                else
                {
                    // TODO: Error expected semicolon after variable declaration;
                    break;
                }
            }
            else
            {
                // TODO: Error expected type after colon
                break;
            }
        }
        else
        {
            // TODO: Error expected colon after identier list
            break;
        }
    }
    advance();
}
void Parser::parseProcedureAndDefinitionPart()
{
}
void Parser::parseProcedureDeclaration()
{
}
void Parser::parseVariableAccess()
{
    //  ISO 10206 -  6.8.1
    //  VARIABLE-ACCESS                 = ENTIRE-VARIABLE | COMPONENT-VARIABLE |
    //                                    IDENTIFIED-VARIABLE | BUFFER-VARIABLE |
    //                                    SUBSTRING-VARIABLE | FUNCTION-IDENTIFIED-VARIABLE
    //  ENTIRE-VARIABLE                 = VARIABLE-NAME
    //  VARIABLE-NAME                   = [ IMPORTED-INTERFACE-IDENTIFIER '.' ] VARIABLE-IDENTIFIER
    //  VARIABLE-IDENTIFIER             = IDENTIFIER
    //  IMPORTED-INTERFACE-IDENTIFIER   = IDENTIFIER
    //  According to this definition ENTIRE-VARIABLE must be a VARIABLE-NAME. Thus ENTIRE-VARIABLE maps
    //  to the following [ IDENTIFIER '.' ] IDENTIFIER
    //  COMPONENT-VARIABLE              = INDEXED-VARIABLE FIELD-DESIGNATOR
    //  INDEXED-VARIABLE                = ARRAY-VARIABLE '[' INDEX-EXPRESSION { ',' INDEX-EXPRESSION } ']'
    //                                    STRING-VARIABLE '[' INDEX-EXPRESSION ']'
    //  ARRAY-VARIABLE                  = VARIABLE-ACCESS
    //  STRING-VARIABLE                 = VARIABLE-ACCESS
    std::deque<char> stack;
    switch (current()->getTokenType())
    {
    case TokenType::IDENTIFIER:
        parseIdentifier();
    case TokenType::END_OF_FILE:
        // TODO: THROW UNEXPECTED END OF FILE
        break;
    default:
        // TODO: THROW UNKNOWN ERROR
        break;
    }
}
void Parser::parseUnsignedConstant()
{
}
void Parser::parseFunctionAccess()
{
}
void Parser::parseParenthesizedExpression()
{
}
void Parser::parseNegatedPrimary()
{
}
void Parser::parseConstantAccess()
{
}
void Parser::parseSchemaDiscriminant()
{
}
void Parser::parseStructuredValueConstructor()
{
}
void Parser::parseDiscriminantIdentifier()
{
}
void Parser::parseIdentifier()
{
    if (current()->getTokenType() == TokenType::END_OF_FILE)
    {
        //  TODO: Error unexpected end of file
    }
    current()->getValue();
    // Eat current TokenType is Identifier
    advance();
    switch (current()->getTokenType())
    {
    case TokenType::SYMBOL_DOT:
        // Is trying to access a member

        break;
    case TokenType::SYMBOL_SQUARE_BRACKET_OPEN:
        // Array access
        advance();
        parseExpression();
        if (current()->getTokenType() != TokenType::SYMBOL_SQUARE_BRACKET_CLOSE)
        {
            // TODO: Throw error missing square bracket close
            parseExpression();
        }
        break;
    }
}
void Parser::parseProgramParameterList()
{
    int totalParams = 0;
    std::string inputVariableName;
    std::string outputVariableName;
    if (current()->getTokenType() == TokenType::IDENTIFIER)
    {
        totalParams++;
        inputVariableName = current()->getValue();
        advance(); // eat first identifier
        while (current()->getTokenType() == TokenType::SYMBOL_COMMA)
        {
            advance(); // eat comma;
            if (current()->getTokenType() == TokenType::IDENTIFIER)
            {
                totalParams++;
                outputVariableName = current()->getValue();
                advance();
            }
            else
            {
                //  TODO: Error Expected identifier after comma in programm parameter list
            }
        }
    }
    else
    {
        VariableDefinition input("input", getTextType(), VariableDefinitionFlags::EXTERNAL);
        VariableDefinition output("output", getTextType(), VariableDefinitionFlags::EXTERNAL);

        this->objects->insert("input", std::make_shared<VariableDefinition>(VariableDefinition(input)));
        this->objects->insert("output", std::make_shared<VariableDefinition>(VariableDefinition(output)));
    }
    if (totalParams > 2)
    {
        //  TODO: Error more than 2 variables, expected 0 or 2 variables.
    }
    else
    {
        VariableDefinition input(inputVariableName, getTextType(), VariableDefinitionFlags::EXTERNAL);
        VariableDefinition output(outputVariableName, getTextType(), VariableDefinitionFlags::EXTERNAL);
        this->objects->insert(inputVariableName, std::make_shared<VariableDefinition>(VariableDefinition(input)));
        this->objects->insert(outputVariableName, std::make_shared<VariableDefinition>(VariableDefinition(output)));
    }
}
std::shared_ptr<FunctionPointerDeclaration> Parser::parseFunctionType()
{
    std::string name;
    if (current()->getTokenType() != TokenType::SYMBOL_FUNCTION)
    {
        this->diagnosticsEngine->japc_error_at(*current().get(), "I was expecting to see FUNCTION keyword");
    }
    std::shared_ptr<PrototypeExpression> prototype = parseFunctionHeader();
    // TODO: FIX ME
}
void Parser::parseProcedure()
{
}
std::vector<std::shared_ptr<VariableDefinition>> Parser::parseFunctionParams()
{

    std::vector<std::shared_ptr<VariableDefinition>> defs;
    if (current()->getTokenType() == TokenType::SYMBOL_PAREN_OPEN)
    {
        std::vector<std::string> paramNames;
        advance();
        VariableDefinitionFlags flags = VariableDefinitionFlags::NONE;
        while (current()->getTokenType() != TokenType::SYMBOL_PAREN_CLOSE)
        {
            std::vector<std::shared_ptr<VariableDefinition>> parsedDefs = parseFunctionParameter(flags, paramNames);
            defs.insert(defs.end(), parsedDefs.begin(), parsedDefs.end());
            if (current()->getTokenType() == TokenType::END_OF_FILE)
            {
                this->diagnosticsEngine->japc_error_at(*current().get(), "Unexpected EOF");
            }
        }
        advance();
    }
    else
    {
        this->diagnosticsEngine->japc_error_at(*current().get(), "Expected '(' in a function or procedure definition");
    }
    return defs;
}
std::vector<std::shared_ptr<VariableDefinition>> Parser::parseFunctionParameter(VariableDefinitionFlags &flags,
                                                                                std::vector<std::string> &names)
{
    //  ISO 10206 -  6.8.1
    //  FORMAL-PARAMETER-LIST           =   "(" FORMAL-PARAMETER-SECTION { ";" FORMAL-PARAMETER-SECTION } ")"
    //  FORMAL-PARAMETER-SECTION        >   VALUE-PARAMETER-SPECIFICATION       |
    //                                      VARIABLE-PARAMETER-SPECIFICATION    |
    //                                      PROCEDURAL-PARAMETER-SPECIFICATION  |
    //                                      FUNCTIONAL-PARAMETER-SPECIFICATION  |
    //                                      CONFORMANT-ARRAY-PARAMETER-SPECIFICATION
    //  VALUE-PARAMETER-SPECIFICATION       =   [ 'PROTECTED' ] IDENTIFIER-LIST ':' PARAMETER-FORM
    //  VARIABLE-PARAMETER-SPECIFICATION    =   [ 'PROTECTED' ] 'VAR' IDENTIFIER-LIST ':' PARAMETER-FORM
    //  PARAMETER-FORM                      =   TYPE-NAME   |   SCHEMA-NAME |   TYPE-INQUIRY
    //  TYPE-NAME                           =   [ IMPORTED-INTERFACE-IDENTIFIER '.' ] TYPE-IDENTIFIER
    //  SCHEMA-NAME                         =   [ IMPORTED-INTERFACE-IDENTIFIER '.' ] SCHEMA-IDENTIFIER
    //  TYPE-INQUIRY                        =   'TYPE' 'OF' TYPE-INQUIRY-OBJECT
    //  TYPE-INQUIRY-OBJECT                 =   VARIABLE-NAME   | PARAMETER-IDENTIFIER

    ;
    // parseFunctionAccessModifier();
    std::vector<std::shared_ptr<VariableDefinition>> varDefs;
    if (current()->getTokenType() == TokenType::SYMBOL_PROTECTED)
    {
        flags |= VariableDefinitionFlags::PROTECTED;
        advance();
    }
    if (current()->getTokenType() == TokenType::SYMBOL_VAR)
    {
        flags |= VariableDefinitionFlags::REFERENCE;
        advance();
    }
    if (current()->getTokenType() == TokenType::IDENTIFIER)
    {
        names.push_back(current()->getValue());
        advance();
    }
    else
    {
        this->diagnosticsEngine->japc_error_at(*current().get(), "Expected IDENTIFIER as parameter");
        return varDefs;
    }
    if (current()->getTokenType() == TokenType::SYMBOL_COLON)
    {
        advance();
        if (std::shared_ptr<TypeDeclaration> type = parseType())
        {
            for (auto name : names)
            {
                VariableDefinition argDef(name, type, flags);
                varDefs.push_back(std::make_shared<VariableDefinition>(argDef));
            }
            flags = VariableDefinitionFlags::NONE;
            names.clear();
            return varDefs;
        }
        else
        {
            this->diagnosticsEngine->japc_error_at(*current().get(), "Expecting a type expression after colon.");
            this->sync();
            return varDefs;
        }
    }
    else
    {
        if (current()->getTokenType() != TokenType::SYMBOL_COMMA)
        {
            this->diagnosticsEngine->japc_error_at(*current().get(), "Expected IDENTIFIER as parameter");
            this->sync();
            return varDefs;
        }
        else
        {
            if (current()->getTokenType() != TokenType::SYMBOL_PAREN_CLOSE)
            {
                this->diagnosticsEngine->japc_error_at(
                    *current().get(), "Expected ')' or ',' after type or identifier definition in function params.");
                this->sync();
            }
        }
    }
    return varDefs;
}
void Parser::parseImportStatement()
{
    advance(); // eat IMPORT keyword
    do
    {
        if (current()->getTokenType() != TokenType::IDENTIFIER)
        {
            //  TODO: expected identifier error
            return;
        }
        std::string importName = current()->getValue();
        advance();
        if (current()->getTokenType() == TokenType::SYMBOL_QUALIFIED)
        {
            //  TODO: Qualified statement
            advance(); // eat qualified;
        }
        if (current()->getTokenType() == TokenType::SYMBOL_ONLY)
        {
            advance();
            if (current()->getTokenType() == TokenType::SYMBOL_PAREN_OPEN)
            {
                advance();
                parseImportList();
                if (current()->getTokenType() == TokenType::SYMBOL_PAREN_CLOSE)
                {
                    advance();
                }
                else
                {
                    //  TODO: Error expected open paren
                    return;
                }
            }
            else
            {
                //  TODO: expected open parentesis after only
                return;
            }
        }
        if (current()->getTokenType() != TokenType::SYMBOL_SEMICOLON)
        {
            // TODO: Expected semicolon after import identifier
            return;
        }
        else
        {
            advance();
        }
    } while (current()->getTokenType() == TokenType::IDENTIFIER);
}
void Parser::parseImportList()
{
    while (true)
    {
        if (current()->getTokenType() == TokenType::IDENTIFIER)
        {
            advance();
        }
        else
        {
            //  TODO: Error set missing identifier expected
            return;
        }
        if (current()->getTokenType() != TokenType::SYMBOL_COMMA)
        {
            return;
        }
        advance();
    }
}
void Parser::parseIdentifierList()
{
    //  ISO 10206
    //  IDENTIFIER-LIST = IDENTIFIER { ',' IDENTIFIER }
    if (current()->getTokenType() == TokenType::IDENTIFIER)
    {
        advance(); // eat first ocurrence of identifier
        while (current()->getTokenType() == TokenType::SYMBOL_COMMA)
        {
            advance(); // eat comma
            if (current()->getTokenType() == TokenType::IDENTIFIER)
            {
                current()->getValue();
                advance();
            }
            else
            {
                //  TODO: ERROR expected identifier after comma
                diagnosticsEngine->japc_error_at((*current().get()),
                                                 "Expected identifier after comma while parsing identifier list");
            }
        }
    }
    else
    {
        //  TODO: ERROR expected identifier
        diagnosticsEngine->japc_error_at((*current().get()), "Expected identifier in identifier list");
    }
}
std::shared_ptr<TypeDeclaration> Parser::parseType()
{
    if (current()->getTokenType() == TokenType::SYMBOL_PACKED)
    {
        advance();
        if (current()->getTokenType() != TokenType::SYMBOL_ARRAY &&
            current()->getTokenType() != TokenType::SYMBOL_RECORD &&
            current()->getTokenType() != TokenType::SYMBOL_SET && current()->getTokenType() != TokenType::SYMBOL_FILE)
        {
            diagnosticsEngine->japc_error_at(*current().get(), "Expected any of array, record, set or file!");
            // TODO Sync
        }
        else
        {
            advance();
        }
    }
    if (current()->getTokenType() == TokenType::SYMBOL_BINDABLE)
    {
        diagnosticsEngine->japc_warning_at(*current().get(), "Bindable is not supported yet!");
        advance();
    }
    switch (current()->getTokenType())
    {
    case TokenType::SYMBOL_TYPE:
        advance();
        if (current()->getTokenType() == TokenType::SYMBOL_OF)
        {
            advance();
            if (current()->getTokenType() == TokenType::IDENTIFIER)
            {
                std::string varName = current()->getValue();
                const std::shared_ptr<NamedObject> namedObject = this->objects->find(varName);
                if (namedObject)
                {
                    if (!llvm::isa<VariableDefinition>(*namedObject.get()))
                    {
                        diagnosticsEngine->japc_error_at(*current().get(), "%s value is not a variable or const",
                                                         varName.c_str());
                    }
                    else
                    {
                        advance();
                        return namedObject->getTypeDeclaration();
                    }
                }
                else
                {
                    diagnosticsEngine->japc_error_at(*current().get(), "%s is not declared in this scope.",
                                                     varName.c_str());
                    //  TODO: Sync
                }
            }
            else
            {
                diagnosticsEngine->japc_error_at(*current().get(), "Expected identifier after TYPE OF expression.");
                this->sync();
            }
        }
        else
        {
            diagnosticsEngine->japc_error_at(
                *current().get(),
                "Expected OF keyword! I could simply add it by my self but I'm tired now, do it by yourself please");
            this->sync();
        }
        break;
    case TokenType::IDENTIFIER:
        break;
    case TokenType::NUMERIC_LITERAL:
    case TokenType::SYMBOL_MINUS:
    case TokenType::STRING_LITERAL: {
        if (current()->getTokenType() == TokenType::STRING_LITERAL && current()->getValue().size() > 1)
        {
            //  TODO: parse String const
            this->diagnosticsEngine->japc_error_at(*current().get(), "Expecting char, got a string instead");
        }
        else
        {
            std::shared_ptr<RangeDeclaration> range;
            std::shared_ptr<TypeDeclaration> type;
            range = this->parseRangeDeclaration(type, TokenType::SYMBOL_SEMICOLON, TokenType::SYMBOL_OF);
            if (range)
            {
                return range;
            }
            return nullptr;
        }
    }
    case TokenType::SYMBOL_STRING_KEYWORD:
        return parseStringDeclaration();
    case TokenType::SYMBOL_SET:
        return parseSetDeclaration();
    case TokenType::SYMBOL_FILE:
        return parseFileDeclaration();
    case TokenType::SYMBOL_ARRAY:
        return parseArrayDeclaration();
    case TokenType::SYMBOL_PAREN_OPEN:
        return parseEnumDefinition();
    case TokenType::SYMBOL_PROCEDURE:
    case TokenType::SYMBOL_FUNCTION: {
        parseFunctionType();
    }
    default: {
        this->diagnosticsEngine->japc_error_at(*current().get(),
                                               "Huh ? I was expecting a declaration here... a %s was a complete "
                                               "surprise to me at this point in your program!",
                                               Scanner::tokenTypeToStr(current()->getTokenType()).c_str());
        this->sync();
    }
    }
}
std::shared_ptr<RangeDeclaration> Parser::parseRangeDeclaration(std::shared_ptr<TypeDeclaration> type, TokenType end,
                                                                TokenType alternative)
{
    std::shared_ptr<ConstantDeclaration> startConstantDec = parseConstantExpression({TokenType::SYMBOL_DOT_DOT});
    if (!startConstantDec)
    {
        return nullptr;
    }
    if (current()->getTokenType() == TokenType::SYMBOL_DOT_DOT)
    {
        advance();
        std::shared_ptr<ConstantDeclaration> endConstantDec = parseConstantExpression({end, alternative});
        if (!endConstantDec)
        {
            return nullptr;
        }
        if (startConstantDec->getType() != endConstantDec->getType())
        {
            this->diagnosticsEngine->japc_error_at(*current().get(),
                                                   "Both expressions in range declaration must be the same type");
        }
        u_int64_t startVal = ParserUtils::constantDeclarationToInteger(startConstantDec.get());
        u_int64_t endVal = ParserUtils::constantDeclarationToInteger(endConstantDec.get());
        if (startVal > endVal)
        {
            this->diagnosticsEngine->japc_error_at(
                *current().get(),
                "Start expression must have a lower value than end expression, ISO says so, do not ask me please.");
        }
        else
        {
            if (startVal == endVal)
            {
                this->diagnosticsEngine->japc_warning_at(*current().get(),
                                                         "Expressions have the same value in range declaration. Call "
                                                         "me paranoid but it may not be what you wan    t as a range.");
            }
            else
            {
                std::shared_ptr<Range> rangeDeclaration = std::make_shared<Range>(Range(startVal, endVal));
                return std::shared_ptr<RangeDeclaration>(new RangeDeclaration(rangeDeclaration, type));
            }
        }
    }
    else
    {
        this->diagnosticsEngine->japc_error_at(*current().get(), "Expected '..' after constant in range declaration.");
        //  TODO: Sync
    }
    return nullptr;
}
std::shared_ptr<ConstantDeclaration> Parser::parseConstantExpression(std::vector<TokenType> terminator)
{
    std::shared_ptr<ConstantDeclaration> cd;
    do
    {
        cd = this->parseConstantTerm();
        if (!cd)
        {
            return nullptr;
        }
        if (!ParserUtils::isAnyOf(*current().get(), terminator))
        {
            cd = parseConstantRightSide(0, cd);
            if (!cd)
            {
                return nullptr;
            }
        }
    } while (!ParserUtils::isAnyOf(*current().get(), terminator));
    return cd;
}
std::shared_ptr<ConstantDeclaration> Parser::parseConstantTerm()
{
    int multiplicative = 1;
    TokenType tk;
    std::shared_ptr<ConstantDeclaration> constantTerm;
    switch (current()->getTokenType())
    {
    case TokenType::SYMBOL_MINUS:
        multiplicative = -1;
    case TokenType::SYMBOL_PLUS:
    case TokenType::SYMBOL_NOT:
        tk = current()->getTokenType();
        advance();
        break;
    default:
        break;
    }
}
std::shared_ptr<ConstantDeclaration> Parser::parseConstantRightSide(int precedence,
                                                                    std::shared_ptr<ConstantDeclaration> leftSide)
{
    while (true)
    {
        TokenType tkType = current()->getTokenType();
        int tkPrecedence = ParserUtils::getPrecedence(tkType);
        if (tkPrecedence < precedence)
        {
            return leftSide;
        }
        advance();
        std::shared_ptr<ConstantDeclaration> rightSide = this->parseConstantTerm();
        if (!rightSide)
        {
            return nullptr;
        }
        int tkNextPrecedence = ParserUtils::getPrecedence(current()->getTokenType());
        if (tkPrecedence < tkNextPrecedence)
        {
            if (!(rightSide = this->parseConstantRightSide(tkPrecedence + 1, rightSide)))
            {
                return nullptr;
            }
        }
        leftSide = ParserUtils::evaluateConstant(leftSide, tkType, rightSide);
    }
}
std::shared_ptr<PrototypeExpression> Parser::parseFunctionHeader()
{
    std::string name;
    bool isForwarded = false;
    advance();
    if (current()->getTokenType() != TokenType::IDENTIFIER)
    {
        this->diagnosticsEngine->japc_error_at(*current().get(), "Missing function identifier");
        this->sync();
    }
    else
    {
        name = current()->getValue();
        FunctionDefinition *functionDefinition;
        advance();
        if (std::shared_ptr<NamedObject> def = objects->find(name))
        {
            functionDefinition = llvm::dyn_cast_or_null<FunctionDefinition>(def.get());
            if (functionDefinition && functionDefinition->getPrototype() &&
                functionDefinition->getPrototype()->isForwarded())
            {
                functionDefinition->getPrototype()->setIsForward(false);
                if (current()->getTokenType() == TokenType::SYMBOL_SEMICOLON)
                    return std::make_shared<PrototypeExpression>(*functionDefinition->getPrototype().get());
            }
        }
        std::vector<std::shared_ptr<VariableDefinition>> vars = parseFunctionParams();
        advance();
        if (current()->getTokenType() == TokenType::SYMBOL_COLON)
        {
            advance();
            std::shared_ptr<TypeDeclaration> typeDeclaration;
            if (current()->getTokenType() == TokenType::IDENTIFIER)
            {
                typeDeclaration = getTypeDeclaration(current()->getValue());
                if (functionDefinition)
                {
                    if (functionDefinition->getTypeDeclaration() != typeDeclaration)
                    {
                        this->diagnosticsEngine->japc_error_at(
                            *current().get(), "This function was already declared with a different type "
                                              "and I will not change my mind about it, so fix it.");
                    }
                    return std::make_shared<PrototypeExpression>(*functionDefinition->getPrototype().get());
                }
                PrototypeExpression prototypeExpression =
                    PrototypeExpression(current()->getTokenPos(), name, vars, typeDeclaration);
                return std::make_shared<PrototypeExpression>(prototypeExpression);
            }
            else
            {
                this->diagnosticsEngine->japc_error_at(*current().get(),
                                                       "Expected type identifier as function return type");
                this->sync();
            }
        }
        else
        {
            this->diagnosticsEngine->japc_error_at(*current().get(),
                                                   "Expected ':' after ')' in a function or procedure declaration");
            this->sync();
        }
    }
}
std::shared_ptr<Function> Parser::parseFunction()
{
    std::string name;
    std::shared_ptr<PrototypeExpression> prototype = parseFunctionHeader();
    if (!prototype || current()->getTokenType() != TokenType::SYMBOL_SEMICOLON)
    {
        this->diagnosticsEngine->japc_error_at(*current().get(), "Error??");
        return nullptr;
    }
    advance();
    Location loc = current()->getTokenPos();
    name = prototype->getName();
    std::shared_ptr<NamedObject> namedObject;
    std::shared_ptr<NamedObject> definition = this->objects->find(name);
    FunctionDefinition *functionDef = llvm::dyn_cast_or_null<FunctionDefinition>(definition.get());
    if (!functionDef && !functionDef->getPrototype() && functionDef->getPrototype() != prototype)
    {
        // This seems to be a way to handle with abstract classes using smart ptrs
        std::shared_ptr<FunctionDeclaration> functionDeclaration = std::make_shared<FunctionDeclaration>(prototype);
        std::shared_ptr<TypeDeclaration> typeDeclaration = functionDeclaration;
        namedObject = std::make_shared<FunctionDefinition>(name, typeDeclaration, prototype);
        if (!this->objects->insert(name, namedObject))
        {
            this->diagnosticsEngine->japc_error_at(*current().get(), "%s already exists.", name.c_str());
        }
        advance();
        if (current()->getTokenType() == TokenType::SYMBOL_FORWARD)
        {
            prototype->setIsForward(true);
            advance();
            if (current()->getTokenType() != TokenType::SYMBOL_SEMICOLON)
            {
                this->diagnosticsEngine->japc_error_at(*current().get(), "Expected ';' after forward keyword");
                this->sync();
            }
            return std::make_shared<Function>(Function(current()->getTokenPos(), prototype, {}, nullptr));
        }
    }
    this->objects->addLevel();
    if (name != "")
    {
        if (namedObject)
        {
            this->objects->insert(name, namedObject);
        }
    }
    for (auto v : prototype->getArgs())
    {
        std::shared_ptr<VariableDefinition> varDefs =
            std::make_shared<VariableDefinition>(v->getName(), v->getTypeDeclaration());
        if (!this->objects->insert(v->getName(), varDefs))
        {
            this->diagnosticsEngine->japc_error_at(*current().get(), "Duplicated %s in prototype.",
                                                   v->getName().c_str());
        }
    }
    std::vector<std::shared_ptr<VariableDeclarationExpression>> variableDeclarations;
    std::shared_ptr<BlockExpression> body;
    std::vector<std::shared_ptr<Function>> innerFunctions;
    for (;;)
    {
        switch (current()->getTokenType())
        {
        case TokenType::SYMBOL_VAR:
            if (std::shared_ptr<VariableDeclarationExpression> vars = parseVarDeclarations())
            {
                variableDeclarations.push_back(vars);
            }
            else
            {
                return nullptr;
            }
            break;
        case TokenType::SYMBOL_LABEL:
            parseLabel();
            break;
        case TokenType::SYMBOL_TYPE:
            parseTypeDefinitionPart();
            break;
        case TokenType::SYMBOL_CONST:
            parseConstantDefinition();
            break;
        case TokenType::SYMBOL_FUNCTION:
        case TokenType::SYMBOL_PROCEDURE:
            if (std::shared_ptr<Function> function = parseFunction())
            {
                innerFunctions.push_back(function);
            }
            break;
        case TokenType::SYMBOL_BEGIN:
            break;
        }
    }
}
std::shared_ptr<StringDeclaration> Parser::parseStringDeclaration()
{
    if (current()->getTokenType() == TokenType::SYMBOL_STRING_KEYWORD)
    {
        unsigned size = 255;
        advance();
        if (current()->getTokenType() == TokenType::SYMBOL_SQUARE_BRACKET_OPEN)
        {
            advance();
            if (current()->getTokenType() == TokenType::NUMERIC_LITERAL)
            {
                if (ParserUtils::numericLiteralIsInteger(*current().get()))
                {
                    size = stoi(current()->getValue());
                    if (current()->getTokenType() == TokenType::SYMBOL_SQUARE_BRACKET_CLOSE)
                    {
                        advance();
                    }
                    else
                    {
                        this->diagnosticsEngine->japc_error_at(*current().get(), "Expecting ']'.");
                        this->sync();
                    }
                }
                else
                {
                    this->diagnosticsEngine->japc_error_at(*current().get(),
                                                           "Expecting integer, but got other type instead.");
                }
            }
            else
            {
                this->diagnosticsEngine->japc_error_at(*current().get(),
                                                       "Expecting number, but got other type instead.");
            }
        }
        else
        {
            if (current()->getTokenType() == TokenType::SYMBOL_PAREN_OPEN)
            {
                advance();
                if (current()->getTokenType() == TokenType::NUMERIC_LITERAL)
                {
                    if (ParserUtils::numericLiteralIsInteger(*current().get()))
                    {
                        size = stoi(current()->getValue());
                        if (current()->getTokenType() == TokenType::SYMBOL_PAREN_OPEN)
                        {
                            advance();
                        }
                        else
                        {
                            this->diagnosticsEngine->japc_error_at(*current().get(), "Expecting ']'.");
                            this->sync();
                        }
                    }
                    else
                    {
                        this->diagnosticsEngine->japc_error_at(*current().get(),
                                                               "Expecting integer, but got other type instead.");
                    }
                }
                else
                {
                    this->diagnosticsEngine->japc_error_at(*current().get(),
                                                           "Expecting number, but got other type instead.");
                }
            }
        }
        if (size == 0)
        {
            this->diagnosticsEngine->japc_warning_at(*current().get(), "Huh? String with size 0 ?");
        }
        return std::make_shared<StringDeclaration>(StringDeclaration(size));
    }
    else
    {
    }
}
std::shared_ptr<ArrayDeclaration> Parser::parseArrayDeclaration()
{
    if (current()->getTokenType() == TokenType::SYMBOL_SQUARE_BRACKET_OPEN)
    {
        advance();
        std::vector<std::shared_ptr<RangeDeclaration>> rangeVector;
        std::shared_ptr<TypeDeclaration> typeDeclaration;
        while (current()->getTokenType() == TokenType::SYMBOL_SQUARE_BRACKET_CLOSE)
        {
            advance();
            std::shared_ptr<RangeDeclaration> rangeDeclaration = this->parseArrayDeclarationRange();
            if (!rangeDeclaration)
            {
                this->diagnosticsEngine->japc_error_at(*current().get(), "I was expecting to see a type here");
                this->sync();
            }
            else
            {
                rangeVector.push_back(rangeDeclaration);
            }
            if (current()->getTokenType() == TokenType::SYMBOL_COMMA)
            {
                advance();
            }
        }
        if (rangeVector.empty())
        {
            this->diagnosticsEngine->japc_error_at(*current().get(), "Expected size of array now");
            this->sync();
        }
        if (current()->getTokenType() == TokenType::SYMBOL_OF)
        {
            std::shared_ptr<TypeDeclaration> typeDeclaration = this->parseType();
            return std::make_shared<ArrayDeclaration>(ArrayDeclaration(typeDeclaration, rangeVector));
        }
        else
        {
            this->diagnosticsEngine->japc_error_at(*current().get(), "Expected OF keyword");
            this->sync();
        }
    }
    else
    {
        this->diagnosticsEngine->japc_error_at(*current().get(), "Expected '[' after array keyword");
        this->sync();
    }
}
std::shared_ptr<RangeDeclaration> Parser::parseArrayDeclarationRange()
{
    std::shared_ptr<TypeDeclaration> typeDeclaration;
    if (current()->getTokenType() == TokenType::IDENTIFIER)
    {
        typeDeclaration = getTypeDeclaration(current()->getValue());
        if (typeDeclaration)
        {
            if (!typeDeclaration->isIntegral())
            {
                this->diagnosticsEngine->japc_error_at(*current().get(),
                                                       "Array declaration must be sized of integral type");
                return nullptr;
            }
            return std::make_shared<RangeDeclaration>(RangeDeclaration(typeDeclaration->getRange(), typeDeclaration));
        }
    }
    return this->parseRangeDeclaration(typeDeclaration, TokenType::SYMBOL_SQUARE_BRACKET_CLOSE,
                                       TokenType::SYMBOL_COMMA);
}
std::shared_ptr<TypeDeclaration> Parser::getTypeDeclaration(std::string nameToSearch)
{
    std::shared_ptr<NamedObject> findedTypeDef = this->objects->find(nameToSearch);
    if (TypeDefinition *typeDef = llvm::dyn_cast_or_null<TypeDefinition>(findedTypeDef.get()))
    {
        return typeDef->getTypeDeclaration();
    }
    return nullptr;
}
std::shared_ptr<FileDeclaration> Parser::parseFileDeclaration()
{
    if (current()->getTokenType() == TokenType::SYMBOL_FILE)
    {
        advance();
        if (current()->getTokenType() == TokenType::SYMBOL_FILE)
        {
            std::shared_ptr<TypeDeclaration> type = parseType();
            if (type)
            {
                return std::make_shared<FileDeclaration>(FileDeclaration(type));
            }
        }
        else
        {
            this->diagnosticsEngine->japc_error_at(*current().get(), "Expected OF keyword now");
            this->sync();
        }
    }
    else
    {
        this->diagnosticsEngine->japc_error_at(*current().get(), "Expected FILE keyword now");
        this->sync();
    }
    return nullptr;
}
std::shared_ptr<SetDeclaration> Parser::parseSetDeclaration()
{
    if (current()->getTokenType() == TokenType::SYMBOL_SET)
    {
        advance();
        if (current()->getTokenType() == TokenType::SYMBOL_OF)
        {
            advance();
            std::shared_ptr<TypeDeclaration> typeDeclaration;
            std::shared_ptr<RangeDeclaration> rangeDeclaration;
            if (current()->getTokenType() == TokenType::IDENTIFIER)
            {
                typeDeclaration = getTypeDeclaration(current()->getValue());
                if (typeDeclaration)
                {
                    if (!typeDeclaration->isIntegral())
                    {
                        this->diagnosticsEngine->japc_error_at(*current().get(), "Given type is not an integral type");
                        return nullptr;
                    }
                    rangeDeclaration = std::make_shared<RangeDeclaration>(
                        RangeDeclaration(typeDeclaration->getRange(), typeDeclaration));
                }
                else
                {
                    rangeDeclaration =
                        parseRangeDeclaration(typeDeclaration, TokenType::SYMBOL_SEMICOLON, TokenType::UNKNOWN);
                }
                if (typeDeclaration)
                {
                    if (typeDeclaration->getRange()->getSize() > __MAX__SET__SIZE__)
                    {
                        this->diagnosticsEngine->japc_error_at(*current().get(), "Set is too large...");
                    }
                    else
                    {
                        return std::make_shared<SetDeclaration>(SetDeclaration(rangeDeclaration, typeDeclaration));
                    }
                }
            }
        }
        else
        {
            this->diagnosticsEngine->japc_error_at(*current().get(), "Expected OF keyword now");
            this->sync();
        }
    }
    else
    {
        this->diagnosticsEngine->japc_error_at(*current().get(), "Expected SET keyword now");
        this->sync();
    }
    return nullptr;
}
std::shared_ptr<EnumDeclaration> Parser::parseEnumDefinition()
{
    if (current()->getTokenType() == TokenType::SYMBOL_PAREN_OPEN)
    {
        advance();
        std::vector<std::string> names;
        do
        {
            if (current()->getTokenType() != TokenType::IDENTIFIER)
            {
                names.push_back(current()->getValue());
                advance();
                if (current()->getTokenType() == TokenType::SYMBOL_COMMA)
                {
                    advance();
                }
                else if (current()->getTokenType() != TokenType::SYMBOL_PAREN_CLOSE)
                {
                    this->diagnosticsEngine->japc_error_at(*current().get(), "Expected ',' of ')' in enum declaration");
                    this->sync();
                }
            }
            else
            {
                this->diagnosticsEngine->japc_error_at(*current().get(), "Expected identifier in enum declaration");
                this->sync();
            }
        } while (current()->getTokenType() != TokenType::SYMBOL_PAREN_CLOSE);
        if (names.empty())
        {
            this->diagnosticsEngine->japc_error_at(*current().get(), "Enum declaration cannot be empty");
            this->sync();
        }
        else
        {
            std::shared_ptr<EnumDeclaration> enumDeclaration =
                std::make_shared<EnumDeclaration>(names, getIntegerType());
            for (auto value : enumDeclaration->getValues())
            {
                if (!this->objects->insert(value.name, std::make_shared<EnumDefinition>(
                                                           EnumDefinition(value.name, enumDeclaration, value.value))))
                {
                    this->diagnosticsEngine->japc_error_at(*current().get(),
                                                           ("Value " + value.name + "already declared").c_str());
                    return nullptr;
                }
            }
            return enumDeclaration;
        }
    }
    else
    {
        this->diagnosticsEngine->japc_error_at(*current().get(), "Expected '(' for enum declaration");
        this->sync();
    }
    return nullptr;
}
void Parser::parseConstantDefinition()
{
    if (current()->getTokenType() == TokenType::SYMBOL_CONST)
    {
        advance();
        do
        {
            if (current()->getTokenType() == TokenType::IDENTIFIER)
            {
                advance();
                std::string name = current()->getValue();
                if (current()->getTokenType() == TokenType::SYMBOL_COLON_EQUAL)
                {
                    advance();
                    std::shared_ptr<ConstantDeclaration> constantDeclaration =
                        parseConstantExpression({TokenType::SYMBOL_SEMICOLON});
                    if (!constantDeclaration)
                    {
                        this->diagnosticsEngine->japc_error_at(*current().get(), "Constant is not well defined");
                    }
                    else
                    {
                        if (!this->objects->insert(name,
                                                   std::make_shared<ConstantDefinition>(name, constantDeclaration)))
                        {
                            this->diagnosticsEngine->japc_error_at(*current().get(), "%s Is already defined.",
                                                                   name.c_str());
                        }
                        else
                        {
                            if (current()->getTokenType() != TokenType::SYMBOL_SEMICOLON)
                            {
                                this->diagnosticsEngine->japc_error_at(*current().get(),
                                                                       "Expected ';' after a constant definition");
                                this->sync();
                            }
                            else
                            {
                                advance();
                            }
                        }
                    }
                }
                else
                {
                    this->diagnosticsEngine->japc_error_at(*current().get(),
                                                           "Constants must be initialized at definition time.");
                }
            }
            else
            {
                this->diagnosticsEngine->japc_error_at(*current().get(), "Expected identifier in constant definition.");
                this->sync();
            }
        } while (current()->getTokenType() == TokenType::IDENTIFIER);
    }
    else
    {
        this->diagnosticsEngine->japc_error_at(*current().get(), "Expected CONST keyword in constant definition.");
        this->sync();
    }
}
void Parser::sync(std::vector<TokenType> tokensToFind)
{
    while (!ParserUtils::isAnyOf(*current().get(), tokensToFind))
    {
        advance();
        if (current()->getTokenType() == TokenType::END_OF_FILE)
        {
            this->diagnosticsEngine->japc_error_at(*current().get(), "Unexpected EOF");
            this->sync();
        }
    }
}
void Parser::parseLabel()
{
    std::vector<int64_t> labelsNames;
    if (current()->getTokenType() == TokenType::SYMBOL_LABEL)
    {
        advance();
        do
        {
            if (current()->getTokenType() == TokenType::NUMERIC_LITERAL)
            {
                if (ParserUtils::numericLiteralIsInteger(*current()))
                {
                    labelsNames.push_back(atoi(current()->getValue().c_str()));
                    advance();
                }
                else
                {
                    this->diagnosticsEngine->japc_error_at(*current().get(),
                                                           "%s is not a integer or cannot be converted to one",
                                                           current()->getValue().c_str());
                    this->sync();
                }
            }
            else
            {
                this->diagnosticsEngine->japc_error_at(*current().get(), "Expected integer label");
                this->sync();
            }
            if (current()->getTokenType() == TokenType::SYMBOL_COMMA)
            {
                advance();
            }
        } while (current()->getTokenType() != TokenType::SYMBOL_SEMICOLON);
        advance(); // eat semicolon;
        for (auto n : labelsNames)
        {
            if (!this->objects->insert(std::to_string(n), std::make_shared<LabelDefinition>(n)))
            {
                this->diagnosticsEngine->japc_error_at(*current().get(), "%d label was already declared.", n);
            }
        }
    }
    else
    {
        this->diagnosticsEngine->japc_error_at(*current().get(), "Expected LABEL keywword");
        this->sync();
    }
}
std::shared_ptr<VariableDeclarationExpression> Parser::parseVarDeclarations()
{
    if (current()->getTokenType() == TokenType::SYMBOL_VAR)
    {
        advance();
        std::vector<std::shared_ptr<VariableDefinition>> varDefs;
        do
        {
            std::vector<std::string> names;
            do
            {
                if (current()->getTokenType() == TokenType::IDENTIFIER)
                {
                    names.push_back(current()->getValue());
                    advance();
                }
                else
                {
                    this->diagnosticsEngine->japc_error_at(*current().get(), "Expected identifier in var definition");
                    this->sync();
                }
            } while (current()->getTokenType() == TokenType::SYMBOL_COMMA);
            advance();
            if (current()->getTokenType() == TokenType::SYMBOL_COLON)
            {
                advance();
                std::shared_ptr<TypeDeclaration> typeDeclaration = parseType();
                if (typeDeclaration)
                {
                    for (auto n : names)
                    {
                        std::shared_ptr<VariableDefinition> varDef =
                            std::make_shared<VariableDefinition>(n, typeDeclaration);
                        varDefs.push_back(varDef);
                        if (!this->objects->insert(n, varDef))
                        {
                            this->diagnosticsEngine->japc_error_at(*current().get(), "%s is already defined",
                                                                   varDef->getName().c_str());
                        }
                    }
                    advance();
                    if (current()->getTokenType() == TokenType::SYMBOL_VALUE)
                    {
                        advance();
                        std::shared_ptr<InitValue> initValue = parseInitValue(typeDeclaration);
                        if (initValue)
                        {
                            varDefs.back()->setInitValue(initValue);
                            advance();
                        }
                        else
                        {
                            this->diagnosticsEngine->japc_error_at(
                                *current().get(), "Expected variable initialization after VALUE keyword");
                            this->sync();
                        }
                        return std::make_shared<VariableDeclarationExpression>(current().get()->getTokenPos(), varDefs);
                    }
                    if (current()->getTokenType() != TokenType::SYMBOL_SEMICOLON)
                    {
                        this->diagnosticsEngine->japc_error_at(*current().get(),
                                                               "Expected ';' after variable initialization");
                        this->sync();
                    }
                }
                else
                {
                    this->diagnosticsEngine->japc_error_at(*current().get(),
                                                           "Expected type to be defined after variable declaration");
                    this->sync();
                }
            }
            else
            {
                this->diagnosticsEngine->japc_error_at(*current().get(), "Expected ':' in after variable identifier");
                this->sync();
            }
        } while (current()->getTokenType() == TokenType::IDENTIFIER);
    }
    else
    {
        this->sync();
        return nullptr;
    }
}
std::shared_ptr<InitValue> Parser::parseInitValue(std::shared_ptr<TypeDeclaration> typeOfInitialization)
{
    if (current()->getTokenType() == TokenType::SYMBOL_SQUARE_BRACKET_OPEN)
    {
    }
    else
    {
        std::shared_ptr<ConstantDeclaration> constantExpression =
            parseConstantExpression({TokenType::SYMBOL_SEMICOLON});
        if (constantExpression)
        {
            std::shared_ptr<ExpressionAST> expre =
                ParserUtils::constantDeclarationToExpression(current()->getTokenPos(), constantExpression.get());
            InitValue value(current()->getTokenPos(), {expre});
            return std::make_shared<InitValue>(value);
        }
    }
    this->diagnosticsEngine->japc_error_at(
        *current().get(), "Expected constant integral or real expression or set expression in variable initialization");
    return nullptr;
}
std::shared_ptr<BlockExpression> Parser::parseBlock()
{
    if (current()->getTokenType() == TokenType::SYMBOL_BEGIN)
    {
        advance(); // eat begin keyword
        std::vector<std::shared_ptr<ExpressionAST>> content;
        Location loc = current()->getTokenPos();
        while (current()->getTokenType() != TokenType::SYMBOL_END)
        {
            if (current()->getTokenType() == TokenType::NUMERIC_LITERAL &&
                ParserUtils::numericLiteralIsInteger(*current().get()) &&
                lookAhead(1)->getTokenType() == TokenType::SYMBOL_COLON)
            {
                Token cur = *current().get();
                advance(); // eat numeric literal
                advance(); // colon
                int64_t label_val = ParserUtils::convertStringToInteger(cur.getValue());
                std::string labelName = current()->getValue();
                if (!this->objects->findAtTop(labelName))
                {
                    this->diagnosticsEngine->japc_error_at(*current().get(), "Label %s cannot be used in this scope",
                                                           labelName.c_str());
                    return nullptr;
                }
                content.push_back(std::make_shared<LabelExpression>(
                    LabelExpression(cur.getTokenPos(), {{label_val, label_val}}, nullptr)));
            }
            else if (std::shared_ptr<ExpressionAST> expression = parseStatement())
            {
                content.push_back(expression);
                if (current()->getTokenType() != TokenType::SYMBOL_SEMICOLON ||
                    current()->getTokenType() != TokenType::SYMBOL_END)
                {
                    this->diagnosticsEngine->japc_error_at(*current().get(),
                                                           "Expected ';' or END keyword at this point");
                    return nullptr; // do not sync, we can just try to parse next stuff to avoid over-sync
                }
            }
            else
            {
                this->diagnosticsEngine->japc_error_at(*current().get(),
                                                       "Expected an expression or statement at this point");
                return nullptr; // Do not sync, we can just try to move to next thing.
            }
        }
        return std::make_shared<BlockExpression>(loc, content);
    }
    else
    {
        this->diagnosticsEngine->japc_error_at(*current().get(), "Expected BEGIN keyword");
        this->sync();
    }
    return std::shared_ptr<BlockExpression>();
}
std::shared_ptr<ExpressionAST> Parser::parseStatement()
{
    switch (current()->getTokenType())
    {
    case TokenType::SYMBOL_BEGIN:
        return parseBlock();
    case TokenType::SYMBOL_SEMICOLON:
    case TokenType::SYMBOL_END:
        this->diagnosticsEngine->japc_warning_at(*current().get(), "Is this an empty statement?");
        return std::make_shared<BlockExpression>(BlockExpression(current()->getTokenPos(), {}));
    default:
        if (std::shared_ptr<ExpressionAST> expre = parsePrimary())
        {
            if (current()->getTokenType() == TokenType::SYMBOL_COLON_EQUAL)
            { // assign expression
                Location loc = current()->getTokenPos();
                std::shared_ptr<ExpressionAST> rhs = parseExpression();
                if (rhs)
                {
                    expre = std::make_shared<AssignExpression>(AssignExpression(loc, expre, rhs));
                }
                else
                {
                    this->diagnosticsEngine->japc_error_at(*current().get(), "Expected expression in assignment");
                    this->sync();
                }
            }
        }
        break;
    }
    return nullptr; // Ideally you should not get here. We do not report anything as we may not be expecting that to
                    // happend.
}