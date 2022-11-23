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

Parser::Parser(std::shared_ptr<Scanner> scanner, std::unique_ptr<JAPCDiagnostics> diagnosticsEngine,
               std::shared_ptr<Stack<std::shared_ptr<NamedObject>>> &stack)
{
    this->scanner = scanner;
    this->diagnosticsEngine = std::move(diagnosticsEngine);
    this->objects = stack;
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

std::unique_ptr<Token> Parser::advance()
{
    std::unique_ptr<Token> old = current();
    if (!isAtEnd())
        currentTokenPos++;
    else
    {
        //  TODO: Error unexpected EOF while parsing
        return current();
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
    return _CUR_TOKEN_TYPE_ == TokenType::END_OF_FILE;
}
std::unique_ptr<Token> Parser::lookAhead(const int num)
{
    if (tokenList->size() <= currentTokenPos + num)
    {
        return nullptr;
    }
    return std::make_unique<Token>(tokenList->at(currentTokenPos + num));
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

    std::vector<TokenType> tk = {TokenType::SYMBOL_FUNCTION, TokenType::SYMBOL_BEGIN,     TokenType::SYMBOL_VAR,
                                 TokenType::SYMBOL_TYPE,     TokenType::SYMBOL_PROCEDURE, TokenType::SYMBOL_EXTERNAL,
                                 TokenType::SYMBOL_LABEL,    TokenType::SYMBOL_GOTO};

    while (!ParserUtils::isAnyOf(_CUR_TOKEN_OBJ_, tk))
    {
        advance();
        if (_CUR_TOKEN_TYPE_ == TokenType::END_OF_FILE)
        {
            this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_, "Unexpected EOF while recovering");
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

std::shared_ptr<ExpressionAST> Parser::parseExpression()
{
    //  ISO 10206 -  6.8.1
    //  EXPRESSION = SIMPLE-EXPRESSION [ RELATIONAL-OPERATOR SIMPLE-EXPRESSION ]
    std::shared_ptr<ExpressionAST> leftSideExpr = parseExpressionTerm();
    if (leftSideExpr)
    {
        return parseBinaryOperatorRightSide(0, leftSideExpr);
    }
    return nullptr;
}
std::shared_ptr<ExpressionAST> Parser::parsePrimary()
{

    switch (_CUR_TOKEN_TYPE_)
    {
    case TokenType::IDENTIFIER:
        return parseIdentifierExpression(_CUR_TOKEN_OBJ_);

    case TokenType::SYMBOL_IF:
        return parseIfExpr();

    case TokenType::SYMBOL_FOR:
        return parseForExpr();

    case TokenType::SYMBOL_WHILE:
        return parseWhile();

    case TokenType::SYMBOL_REPEAT:
        return parseRepeat();

    case TokenType::SYMBOL_CASE:
        return parseCaseExpr();

    case TokenType::SYMBOL_WITH:
        return parseWithBlock();
        /*
            case Token::Write:
            case Token::Writeln:
                return ParseWrite();

            case Token::Read:
            case Token::Readln:
                return ParseRead();
        */
    case TokenType::SYMBOL_GOTO:
        return parseGoto();

    default:
        advance();
        this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_, "Syntax error while parsing. Unknown %s",
                                               current()->getValue().c_str());
    }
}
void Parser::parseProgram()
{
    StackWrapper<std::shared_ptr<NamedObject>> wrapper(this->objects);
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
std::shared_ptr<Function> Parser::parseProgramBlock()
{
    if (_CUR_TOKEN_TYPE_ == TokenType::SYMBOL_BEGIN)
    {
        Token tk = _CUR_TOKEN_OBJ_;
        std::shared_ptr<BlockExpression> mainProgramBody = parseBlock();
        if (!mainProgramBody)
        {
            std::shared_ptr<PrototypeExpression> prototype = std::make_shared<PrototypeExpression>(
                tk.getTokenPos(), "_MAIN_", std::vector<std::shared_ptr<VariableDefinition>>(), getVoidType());
            std::shared_ptr<Function> initialFunction = std::make_shared<Function>(
                tk.getTokenPos(), prototype, std::vector<std::shared_ptr<VariableDeclarationExpression>>(),
                mainProgramBody);
            if (_CUR_TOKEN_TYPE_ == TokenType::SYMBOL_DOT)
            {
                advance(); // eat dot
                return initialFunction;
            }
            else
            {
                this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_,
                                                       "Expected '.' after END keyword at main program declaration");
            }
        }
    }
    else
    {
        this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_, "Expected begin keyword at main program declaration");
    }
    return nullptr;
}
std::shared_ptr<ExpressionAST> Parser::parseMainProgramDeclaration()
{
    parseProgramHeading();
    if (_CUR_TOKEN_TYPE_ != TokenType::SYMBOL_SEMICOLON)
    {
        this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_, "Expected semicolon");
    }
    else
    {
        advance();
    }
    if (isAtEnd())
    {
        this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_, "Unexpected EOF");
        this->sync();
    }
    parseImportPart();
    if (isAtEnd())
    {
        this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_, "Unexpected EOF");
        throw this->currentState;
    }
    return parseMainProgramBlock();
}
void Parser::parseProgramHeading()
{
    if (_CUR_TOKEN_TYPE_ != TokenType::SYMBOL_PROGRAM)
    {
        this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_, "Missing program declaration here!");
        this->sync();
    }
    else
    {
        advance(); // eat PROGRAM keyword
        if (_CUR_TOKEN_TYPE_ != TokenType::IDENTIFIER)
        {
            this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_, "Expected program keyword here!");
            this->sync();
        }
        else
        {
            this->moduleName = current()->getValue();
            advance();
            if (_CUR_TOKEN_TYPE_ == TokenType::SYMBOL_PAREN_OPEN)
            {
                parseProgramParameterList();
                if (_CUR_TOKEN_TYPE_ != TokenType::SYMBOL_PAREN_CLOSE)
                {
                    this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_, "Expected ')' after program param list");
                    sync();
                }
                advance();
            }
        }
    }
}
std::shared_ptr<ExpressionAST> Parser::parseMainProgramBlock()
{
    while (!isAtEnd() && _CUR_TOKEN_TYPE_ != TokenType::SYMBOL_BEGIN)
    {
        try
        {
            std::shared_ptr<ExpressionAST> currentExpr;
            switch (_CUR_TOKEN_TYPE_)
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
                currentExpr = parseProcedure();
                break;
            case TokenType::SYMBOL_FUNCTION:
                currentExpr = parseFunction();
                break;
            default:
                //  TODO: ERROR ??
                sync();
                break;
            }
            if (currentExpr)
            {
                ast.push_back(currentExpr);
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
    Token tk = _CUR_TOKEN_OBJ_;
    std::shared_ptr<Function> function = parseProgramBlock();
    if (function)
    {
        return std::make_shared<UnitExpression>(tk.getTokenPos(), this->ast, function);
    }
    else
    {
        return nullptr;
    }
}

void Parser::parseImportPart()
{
    while (_CUR_TOKEN_TYPE_ == TokenType::SYMBOL_IMPORT)
    {
        this->parseImportStatement();
    }
}
void Parser::parseTypeDefinitionPart()
{
    std::vector<std::shared_ptr<PointerDeclaration>> incompletes;
    if (_CUR_TOKEN_TYPE_ != TokenType::SYMBOL_TYPE)
    {
        this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_, "Expected TYPE keyword");
        this->sync();
        return;
    }
    advance();
    do
    {
        if (_CUR_TOKEN_TYPE_ != TokenType::IDENTIFIER)
        {
            this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_, "Expected IDENTIFIER");
            this->sync();
            return;
        }
        std::string name = current()->getValue();
        advance();
        if (_CUR_TOKEN_TYPE_ != TokenType::SYMBOL_EQUAL)
        {
            this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_, "Expected '=' after type name");
            this->sync();
        }
        advance();
        bool restricted = false;
        if (_CUR_TOKEN_TYPE_ == TokenType::SYMBOL_RESTRICTED)
        {
            restricted = true;
            advance();
        }
        std::shared_ptr<TypeDeclaration> typeDeclaration = parseType();
        if (typeDeclaration)
        {
            std::shared_ptr<ExpressionAST> initial;
            if (_CUR_TOKEN_TYPE_ == TokenType::SYMBOL_VALUE)
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
                    this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_, "%s is already declared");
                    return;
                }
                if (typeDeclaration->getTypeKind() == TypeKind::TYPE_POINTER &&
                    llvm::dyn_cast_or_null<PointerDeclaration>(typeDeclaration.get())->isIncomplete())
                {
                    const PointerDeclaration *pointerDeclaration =
                        llvm::dyn_cast_or_null<PointerDeclaration>(typeDeclaration.get());
                    incompletes.push_back(std::make_shared<PointerDeclaration>(*pointerDeclaration));
                }
                if (_CUR_TOKEN_TYPE_ != TokenType::SYMBOL_SEMICOLON)
                {
                    this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_, "Expected ';' ");
                    this->sync();
                    return;
                }
            }
        }
        else
        {
            this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_, "Missing type declaration");
            this->sync();
            return;
        }
    } while (_CUR_TOKEN_TYPE_ == TokenType::IDENTIFIER);
    for (auto incomplete : incompletes)
    {
        std::string name = incomplete->getSubtype()->getName();
        if (std::shared_ptr<TypeDeclaration> type = getTypeDeclaration(name))
        {
            if (type->isIncomplete())
            {
                this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_, "Forwarded type %s is incomplete.",
                                                       name.c_str());
                return;
            }
            incomplete->SetSubType(type);
        }
        else
        {
            this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_,
                                                   "Forwarded declared pointer type %s is not declared.", name.c_str());
        }
    }
}
std::shared_ptr<VariableDeclarationExpression> Parser::parseVariableDeclarationPart()
{
    advance(); // eat VAR keyword
    while (_CUR_TOKEN_TYPE_ == TokenType::IDENTIFIER)
    {
        parseIdentifierList();
        advance();
        if (_CUR_TOKEN_TYPE_ == TokenType::SYMBOL_COLON)
        {
            advance();
            if (_CUR_TOKEN_TYPE_ == TokenType::IDENTIFIER)
            {
                advance();
                if (_CUR_TOKEN_TYPE_ == TokenType::SYMBOL_SEMICOLON)
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

std::shared_ptr<ExpressionAST> Parser::parseIdentifierExpression(Token tk)
{
    std::shared_ptr<ExpressionAST> expre = parseFunctionCallOrVariableExpression(tk);
    if (!expre)
    {
        return nullptr;
    }
    std::shared_ptr<TypeDeclaration> type = expre->getTypeDeclaration();
    while (_CUR_TOKEN_TYPE_ == TokenType::SYMBOL_SQUARE_BRACKET_OPEN || _CUR_TOKEN_TYPE_ == TokenType::SYMBOL_AT ||
           _CUR_TOKEN_TYPE_ == TokenType::SYMBOL_DOT)
    {
        if (!type)
        {
            this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_, "Expression expected to have a type");
        }
        switch (_CUR_TOKEN_TYPE_)
        {
        case TokenType::SYMBOL_SQUARE_BRACKET_OPEN:
            if (!(expre = parseArrayExpression(expre, type)))
            {
                return nullptr;
            }
            break;
        case TokenType::SYMBOL_AT:
            if (!(expre = parsePointerExpression(expre, type)))
            {
                return nullptr;
            }
            break;
        case TokenType::SYMBOL_DOT:
            std::shared_ptr<ExpressionAST> tempExpr;
            if (tempExpr == parseFieldExpression(expre, type))
            {
                if (auto v = llvm::dyn_cast<AddressableExpression>(tempExpr.get()))
                {
                    expre = std::shared_ptr<AddressableExpression>(v);
                }
                else
                {
                    return tempExpr;
                }
            }
            else
            {
                this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_, "Unable to parse token");
                this->sync();
            }
        }
    }
    return expre;
}
void Parser::parseProgramParameterList()
{
    int totalParams = 0;
    std::string inputVariableName;
    std::string outputVariableName;
    if (_CUR_TOKEN_TYPE_ == TokenType::IDENTIFIER)
    {
        totalParams++;
        inputVariableName = current()->getValue();
        advance(); // eat first identifier
        while (_CUR_TOKEN_TYPE_ == TokenType::SYMBOL_COMMA)
        {
            advance(); // eat comma;
            if (_CUR_TOKEN_TYPE_ == TokenType::IDENTIFIER)
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
    if (_CUR_TOKEN_TYPE_ != TokenType::SYMBOL_FUNCTION)
    {
        this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_, "I was expecting to see FUNCTION keyword");
    }
    std::shared_ptr<PrototypeExpression> prototype = parseFunctionHeader();
    // TODO: FIX ME
}
std::shared_ptr<Function> Parser::parseProcedure()
{
    std::string name;
    std::shared_ptr<PrototypeExpression> prototype = parseProcedureHeader();
    if (!prototype || _CUR_TOKEN_TYPE_ != TokenType::SYMBOL_SEMICOLON)
    {
        this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_, "Error??");
        return nullptr;
    }
    advance();
    Location loc = current()->getTokenPos();
    name = prototype->getName();
    std::shared_ptr<NamedObject> namedObject;
    std::shared_ptr<NamedObject> definition = this->objects->find(name);
    FunctionDefinition *functionDef = llvm::dyn_cast_or_null<FunctionDefinition>(definition.get());
    if (!(functionDef && functionDef->getPrototype() && functionDef->getPrototype() == prototype))
    {
        // This seems to be a way to handle with abstract classes using smart ptrs
        std::shared_ptr<FunctionDeclaration> functionDeclaration = std::make_shared<FunctionDeclaration>(prototype);
        std::shared_ptr<TypeDeclaration> typeDeclaration = functionDeclaration;
        namedObject = std::make_shared<FunctionDefinition>(name, typeDeclaration, prototype);
        if (!this->objects->insert(name, namedObject))
        {
            this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_, "%s already exists.", name.c_str());
        }
        // advance();
        if (_CUR_TOKEN_TYPE_ == TokenType::SYMBOL_FORWARD)
        {
            prototype->setIsForward(true);
            advance();
            if (_CUR_TOKEN_TYPE_ != TokenType::SYMBOL_SEMICOLON)
            {
                this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_, "Expected ';' after forward keyword");
                this->sync();
            }
            return std::make_shared<Function>(Function(current()->getTokenPos(), prototype, {}, nullptr));
        }
    }
    StackWrapper<std::shared_ptr<NamedObject>> wrapper(this->objects);
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
            this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_, "Duplicated %s in prototype.",
                                                   v->getName().c_str());
        }
    }
    std::vector<std::shared_ptr<VariableDeclarationExpression>> variableDeclarations;
    std::shared_ptr<BlockExpression> body;
    std::vector<std::shared_ptr<Function>> innerFunctions;
    for (;;)
    {
        switch (_CUR_TOKEN_TYPE_)
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
        case TokenType::SYMBOL_BEGIN: {
            Location endLoc;
            if (body)
            {
                this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_, "Multiple body declaration for a procedure");
                this->sync();
            }
            else
            {
                body = parseBlock();
                if (_CUR_TOKEN_TYPE_ == TokenType::SYMBOL_SEMICOLON)
                {
                    advance(); // eat semicolon
                    if (!body)
                    {
                        this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_, "Expected function body definition");
                        this->sync();
                    }
                    else
                    {
                        std::shared_ptr<Function> function =
                            std::make_shared<Function>(loc, prototype, variableDeclarations, body);
                        if (!prototype->getFunction())
                        {
                            prototype->setFunction(function);
                        }
                        for (auto subF : innerFunctions)
                        {
                            subF->setParent(function);
                        }
                        function->addSubFunctions(innerFunctions);
                        function->setEndLoc(endLoc);
                        return function;
                    }
                }
                else
                {
                    this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_,
                                                           "Expected ';' after procedure body definition");
                    this->sync();
                }
            }
            break;
        }
        default:
            this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_, "Unexpected %s", current()->getValue().c_str());
            this->sync();
        }
    }
}
std::vector<std::shared_ptr<VariableDefinition>> Parser::parseFunctionParams()
{

    std::vector<std::shared_ptr<VariableDefinition>> defs;
    if (_CUR_TOKEN_TYPE_ == TokenType::SYMBOL_PAREN_OPEN)
    {
        std::vector<std::string> paramNames;
        advance();
        VariableDefinitionFlags flags = VariableDefinitionFlags::NONE;
        while (_CUR_TOKEN_TYPE_ != TokenType::SYMBOL_PAREN_CLOSE)
        {
            std::vector<std::shared_ptr<VariableDefinition>> parsedDefs = parseFunctionParameter(flags, paramNames);
            defs.insert(defs.end(), parsedDefs.begin(), parsedDefs.end());
            if (_CUR_TOKEN_TYPE_ == TokenType::END_OF_FILE)
            {
                this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_, "Unexpected EOF");
            }
        }
        advance();
    }
    else
    {
        this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_, "Expected '(' in a function or procedure definition");
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
    if (_CUR_TOKEN_TYPE_ == TokenType::SYMBOL_PROTECTED)
    {
        flags |= VariableDefinitionFlags::PROTECTED;
        advance();
    }
    if (_CUR_TOKEN_TYPE_ == TokenType::SYMBOL_VAR)
    {
        flags |= VariableDefinitionFlags::REFERENCE;
        advance();
    }
    if (_CUR_TOKEN_TYPE_ == TokenType::IDENTIFIER)
    {
        names.push_back(current()->getValue());
        advance();
    }
    else
    {
        this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_, "Expected IDENTIFIER as parameter");
        return varDefs;
    }
    if (_CUR_TOKEN_TYPE_ == TokenType::SYMBOL_COLON)
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
            this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_, "Expecting a type expression after colon.");
            this->sync();
            return varDefs;
        }
    }
    else
    {
        if (_CUR_TOKEN_TYPE_ != TokenType::SYMBOL_COMMA)
        {
            this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_, "Expected IDENTIFIER as parameter");
            this->sync();
            return varDefs;
        }
        else
        {
            if (_CUR_TOKEN_TYPE_ != TokenType::SYMBOL_PAREN_CLOSE)
            {
                this->diagnosticsEngine->japc_error_at(
                    _CUR_TOKEN_OBJ_, "Expected ')' or ',' after type or identifier definition in function params.");
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
        if (_CUR_TOKEN_TYPE_ != TokenType::IDENTIFIER)
        {
            //  TODO: expected identifier error
            return;
        }
        std::string importName = current()->getValue();
        advance();
        if (_CUR_TOKEN_TYPE_ == TokenType::SYMBOL_QUALIFIED)
        {
            //  TODO: Qualified statement
            advance(); // eat qualified;
        }
        if (_CUR_TOKEN_TYPE_ == TokenType::SYMBOL_ONLY)
        {
            advance();
            if (_CUR_TOKEN_TYPE_ == TokenType::SYMBOL_PAREN_OPEN)
            {
                advance();
                parseImportList();
                if (_CUR_TOKEN_TYPE_ == TokenType::SYMBOL_PAREN_CLOSE)
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
        if (_CUR_TOKEN_TYPE_ != TokenType::SYMBOL_SEMICOLON)
        {
            // TODO: Expected semicolon after import identifier
            return;
        }
        else
        {
            advance();
        }
    } while (_CUR_TOKEN_TYPE_ == TokenType::IDENTIFIER);
}
void Parser::parseImportList()
{
    while (true)
    {
        if (_CUR_TOKEN_TYPE_ == TokenType::IDENTIFIER)
        {
            advance();
        }
        else
        {
            //  TODO: Error set missing identifier expected
            return;
        }
        if (_CUR_TOKEN_TYPE_ != TokenType::SYMBOL_COMMA)
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
    if (_CUR_TOKEN_TYPE_ == TokenType::IDENTIFIER)
    {
        advance(); // eat first ocurrence of identifier
        while (_CUR_TOKEN_TYPE_ == TokenType::SYMBOL_COMMA)
        {
            advance(); // eat comma
            if (_CUR_TOKEN_TYPE_ == TokenType::IDENTIFIER)
            {
                current()->getValue();
                advance();
            }
            else
            {
                //  TODO: ERROR expected identifier after comma
                diagnosticsEngine->japc_error_at((_CUR_TOKEN_OBJ_),
                                                 "Expected identifier after comma while parsing identifier list");
            }
        }
    }
    else
    {
        //  TODO: ERROR expected identifier
        diagnosticsEngine->japc_error_at((_CUR_TOKEN_OBJ_), "Expected identifier in identifier list");
    }
}
std::shared_ptr<TypeDeclaration> Parser::parseType()
{
    if (_CUR_TOKEN_TYPE_ == TokenType::SYMBOL_PACKED)
    {
        advance();
        if (_CUR_TOKEN_TYPE_ != TokenType::SYMBOL_ARRAY && _CUR_TOKEN_TYPE_ != TokenType::SYMBOL_RECORD &&
            _CUR_TOKEN_TYPE_ != TokenType::SYMBOL_SET && _CUR_TOKEN_TYPE_ != TokenType::SYMBOL_FILE)
        {
            diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_, "Expected any of array, record, set or file!");
            // TODO Sync
        }
        else
        {
            advance();
        }
    }
    if (_CUR_TOKEN_TYPE_ == TokenType::SYMBOL_BINDABLE)
    {
        diagnosticsEngine->japc_warning_at(_CUR_TOKEN_OBJ_, "Bindable is not supported yet!");
        advance();
    }
    switch (_CUR_TOKEN_TYPE_)
    {
    case TokenType::SYMBOL_TYPE:
        advance();
        if (_CUR_TOKEN_TYPE_ == TokenType::SYMBOL_OF)
        {
            advance();
            if (_CUR_TOKEN_TYPE_ == TokenType::IDENTIFIER)
            {
                std::string varName = current()->getValue();
                const std::shared_ptr<NamedObject> namedObject = this->objects->find(varName);
                if (namedObject)
                {
                    if (!llvm::isa<VariableDefinition>(*namedObject.get()))
                    {
                        diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_, "%s value is not a variable or const",
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
                    diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_, "%s is not declared in this scope.",
                                                     varName.c_str());
                    //  TODO: Sync
                }
            }
            else
            {
                diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_, "Expected identifier after TYPE OF expression.");
                this->sync();
            }
        }
        else
        {
            diagnosticsEngine->japc_error_at(
                _CUR_TOKEN_OBJ_,
                "Expected OF keyword! I could simply add it by my self but I'm tired now, do it by yourself please");
            this->sync();
        }
        break;
    case TokenType::IDENTIFIER: {
        std::string name = current()->getValue();
        if (!llvm::dyn_cast_or_null<EnumDefinition>(this->objects->find(name).get()))
        {
            return parseSimpleType();
        }
    }
    case TokenType::NUMERIC_LITERAL:
    case TokenType::SYMBOL_MINUS:
    case TokenType::STRING_LITERAL: {
        if (_CUR_TOKEN_TYPE_ == TokenType::STRING_LITERAL && current()->getValue().size() > 1)
        {
            //  TODO: parse String const
            this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_, "Expecting char, got a string instead");
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
        this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_,
                                               "Huh ? I was expecting a declaration here... a %s was a complete "
                                               "surprise to me at this point in your program!",
                                               Scanner::tokenTypeToStr(_CUR_TOKEN_TYPE_).c_str());
        this->sync();
    }
    }
}
std::shared_ptr<RangeDeclaration> Parser::parseRangeDeclaration(std::shared_ptr<TypeDeclaration> &type, TokenType end,
                                                                TokenType alternative)
{
    std::shared_ptr<ConstantDeclaration> startConstantDec = parseConstantExpression({TokenType::SYMBOL_DOT_DOT});
    if (!startConstantDec)
    {
        return nullptr;
    }
    if (_CUR_TOKEN_TYPE_ == TokenType::SYMBOL_DOT_DOT)
    {
        advance();
        std::shared_ptr<ConstantDeclaration> endConstantDec = parseConstantExpression({end, alternative});
        if (!endConstantDec)
        {
            return nullptr;
        }
        if (startConstantDec->getType() != endConstantDec->getType())
        {
            this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_,
                                                   "Both expressions in range declaration must be the same type");
        }
        u_int64_t startVal = ParserUtils::constantDeclarationToInteger(startConstantDec.get());
        u_int64_t endVal = ParserUtils::constantDeclarationToInteger(endConstantDec.get());
        if (startVal > endVal)
        {
            this->diagnosticsEngine->japc_error_at(
                _CUR_TOKEN_OBJ_,
                "Start expression must have a lower value than end expression, ISO says so, do not ask me please.");
        }
        else
        {
            if (startVal == endVal)
            {
                this->diagnosticsEngine->japc_warning_at(_CUR_TOKEN_OBJ_,
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
        this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_, "Expected '..' after constant in range declaration.");
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
        if (!ParserUtils::isAnyOf(_CUR_TOKEN_OBJ_, terminator))
        {
            cd = parseConstantRightSide(0, cd);
            if (!cd)
            {
                return nullptr;
            }
        }
    } while (!ParserUtils::isAnyOf(_CUR_TOKEN_OBJ_, terminator));
    return cd;
}
std::shared_ptr<ConstantDeclaration> Parser::parseConstantTerm()
{
    int multiplicative = 1;
    TokenType tk;
    std::shared_ptr<ConstantDeclaration> constantTerm;
    switch (_CUR_TOKEN_TYPE_)
    {
    case TokenType::SYMBOL_MINUS:
        multiplicative = -1;
    case TokenType::SYMBOL_PLUS:
    case TokenType::SYMBOL_NOT:
        tk = _CUR_TOKEN_TYPE_;
        advance();
        break;
    default:
        break;
    }
    switch (_CUR_TOKEN_TYPE_)
    {
    case TokenType::SYMBOL_PAREN_OPEN:
        constantTerm = this->parseConstantExpression({TokenType::SYMBOL_PAREN_CLOSE});
        if (_CUR_TOKEN_TYPE_ != TokenType::SYMBOL_PAREN_CLOSE)
        {
            return nullptr;
        }
        break;
    case TokenType::STRING_LITERAL:
        if (multiplicative != 1 && (tk == TokenType::SYMBOL_PLUS || tk == TokenType::SYMBOL_NOT))
        {
            this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_, "Unary operator are not allowed for strings!");
            // TODO:: return null or find next semicolon
            this->sync();
        }
        if (current()->getValue().size() == 1)
        {
            const char *temp = current()->getValue().c_str();
            constantTerm = std::make_shared<CharConstantDeclaration>(current()->getTokenPos(), temp[0]);
        }
        else
        {
            constantTerm = std::make_shared<StringConstantDeclaration>(current()->getTokenPos(), current()->getValue());
        }
        break;
    case TokenType::NUMERIC_LITERAL:
    case TokenType::BIG_INTEGER:
        if (tk == TokenType::SYMBOL_NOT)
        {
            this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_,
                                                   "Unary operator NOT is not allowed for numeric literal!");
            break; // TODO: FIX ME
        }
        if (ParserUtils::numericLiteralIsInteger(*current()))
        {
            char *endptr;
            errno = 0;
            int64_t val = strtoll(current()->getValue().c_str(), &endptr, 10);
            if ((errno == ERANGE && (val == LONG_MAX || val == LONG_MIN)) || (errno != 0 && val == 0))
            {
                this->diagnosticsEngine->japc_warning_at(_CUR_TOKEN_OBJ_,
                                                         "%s is too large for an integer. It exceed MAX_INTEGER size",
                                                         current()->getValue().c_str());
                val = LONG_MAX - 1;
            }
            constantTerm = std::make_shared<IntConstantDeclaration>(current()->getTokenPos(), val * multiplicative);
        }
        else
        {
            long double val;
            try
            {
                val = std::stod(current()->getValue());
            }
            catch (const std::out_of_range &oor)
            {
                this->diagnosticsEngine->japc_warning_at(_CUR_TOKEN_OBJ_,
                                                         "%s is too large for an real. It exceed LDBL_MAX size",
                                                         current()->getValue().c_str());
                val = LDBL_MAX - 1.0;
            }
            constantTerm = std::make_shared<RealConstantDeclaration>(current()->getTokenPos(), val * multiplicative);
        }
        break;
    case TokenType::IDENTIFIER:
        std::string name = current()->getValue();
        if (EnumDefinition *enumDefinition = llvm::dyn_cast_or_null<EnumDefinition>(this->objects->find(name).get()))
        {
            if (this->objects->find(name) &&
                enumDefinition->getTypeDeclaration()->getTypeKind() == TypeKind::TYPE_BOOLEAN)
            {
                uint64_t val = enumDefinition->getValue();
                if (tk == TokenType::SYMBOL_NOT)
                    val = !val;
                else if (tk == TokenType::SYMBOL_MINUS || tk == TokenType::SYMBOL_PLUS)
                {
                    this->diagnosticsEngine->japc_error_at(
                        _CUR_TOKEN_OBJ_, "+ or - is not allowed for boolean value, did you mean NOT operator?");
                    break;
                }
                constantTerm = std::shared_ptr<BooleanConstantDeclaration>(
                    new BooleanConstantDeclaration(current()->getTokenPos(), val));
            }
            else
            {
                if (tk == TokenType::SYMBOL_MINUS || tk == TokenType::SYMBOL_PLUS)
                {
                    this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_, "+ or - is not allowed for enum values.");
                    break;
                }
                constantTerm = std::shared_ptr<EnumConstantDeclaration>(new EnumConstantDeclaration(
                    enumDefinition->getTypeDeclaration(), current()->getTokenPos(), enumDefinition->getValue()));
            }
        }
        else
        {
            std::shared_ptr<ConstantDeclaration> constantDeclaration;
            if (const ConstantDefinition *constantDefinition =
                    llvm::dyn_cast_or_null<const ConstantDefinition>(this->objects->find(name).get()))
            {
                constantTerm = constantDefinition->getConstValue();
                if (!constantTerm)
                {
                    this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_, "Expected constant name");
                    break;
                }
            }
            if (constantDeclaration.get() && llvm::isa<BooleanConstantDeclaration>(constantDeclaration.get()) &&
                tk == TokenType::SYMBOL_NOT)
            {
                BooleanConstantDeclaration *booleanConstantDeclaration =
                    llvm::dyn_cast_or_null<BooleanConstantDeclaration>(constantDeclaration.get());
                constantTerm = std::shared_ptr<BooleanConstantDeclaration>(
                    new BooleanConstantDeclaration(current()->getTokenPos(), !booleanConstantDeclaration->getValue()));
            }
            if (multiplicative == -1)
            {
                if (constantTerm.get() && llvm::isa<RealConstantDeclaration>(*constantTerm.get()))
                {
                    RealConstantDeclaration *rd = llvm::dyn_cast_or_null<RealConstantDeclaration>(constantTerm.get());
                    constantTerm = std::make_shared<RealConstantDeclaration>(current()->getTokenPos(), -rd->getValue());
                }
                else if (constantTerm.get() && llvm::isa<RealConstantDeclaration>(*constantTerm.get()))
                {
                    IntConstantDeclaration *id = llvm::dyn_cast_or_null<IntConstantDeclaration>(constantTerm.get());
                    constantTerm = std::make_shared<IntConstantDeclaration>(current()->getTokenPos(), -id->getValue());
                }
                else
                {
                    this->diagnosticsEngine->japc_error_at(
                        _CUR_TOKEN_OBJ_, "You can only use - operator with Int or Real types. Did you mean NOT?");
                    break;
                }
            }
        }
    }
    advance();
    return constantTerm;
}
std::shared_ptr<ConstantDeclaration> Parser::parseConstantRightSide(int precedence,
                                                                    std::shared_ptr<ConstantDeclaration> leftSide)
{
    while (true)
    {
        TokenType tkType = _CUR_TOKEN_TYPE_;
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
        int tkNextPrecedence = ParserUtils::getPrecedence(_CUR_TOKEN_TYPE_);
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
    advance(); // eat FUNCTION/PROCEDURE
    if (_CUR_TOKEN_TYPE_ != TokenType::IDENTIFIER)
    {
        this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_, "Missing function identifier");
        this->sync();
    }
    else
    {
        name = current()->getValue();
        FunctionDefinition *functionDefinition;
        advance(); // eat function/procedure identifiers
        if (std::shared_ptr<NamedObject> def = objects->find(name))
        {
            functionDefinition = llvm::dyn_cast_or_null<FunctionDefinition>(def.get());
            if (functionDefinition && functionDefinition->getPrototype() &&
                functionDefinition->getPrototype()->isForwarded())
            {
                functionDefinition->getPrototype()->setIsForward(false);
                if (_CUR_TOKEN_TYPE_ == TokenType::SYMBOL_SEMICOLON)
                    return std::make_shared<PrototypeExpression>(*functionDefinition->getPrototype().get());
            }
        }
        std::vector<std::shared_ptr<VariableDefinition>> vars;
        if (_CUR_TOKEN_TYPE_ == TokenType::SYMBOL_PAREN_OPEN)
        {
            vars = parseFunctionParams();
        }
        advance();
        if (_CUR_TOKEN_TYPE_ == TokenType::SYMBOL_COLON)
        {
            advance();
            std::shared_ptr<TypeDeclaration> typeDeclaration;
            if (_CUR_TOKEN_TYPE_ == TokenType::IDENTIFIER)
            {
                typeDeclaration = getTypeDeclaration(current()->getValue());
                if (functionDefinition)
                {
                    if (functionDefinition->getTypeDeclaration() != typeDeclaration)
                    {
                        this->diagnosticsEngine->japc_error_at(
                            _CUR_TOKEN_OBJ_, "This function was already declared with a different type "
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
                this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_,
                                                       "Expected type identifier as function return type");
                this->sync();
            }
        }
        else
        {
            this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_,
                                                   "Expected ':' after ')' in a function or procedure declaration");
            this->sync();
        }
    }
}

std::shared_ptr<PrototypeExpression> Parser::parseProcedureHeader()
{
    std::string name;
    bool isForwarded = false;
    advance(); // eat FUNCTION/PROCEDURE
    if (_CUR_TOKEN_TYPE_ != TokenType::IDENTIFIER)
    {
        this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_, "Missing function identifier");
        this->sync();
    }
    else
    {
        name = current()->getValue();
        FunctionDefinition *functionDefinition;
        advance(); // eat function/procedure identifiers
        if (std::shared_ptr<NamedObject> def = objects->find(name))
        {
            functionDefinition = llvm::dyn_cast_or_null<FunctionDefinition>(def.get());
            if (functionDefinition && functionDefinition->getPrototype() &&
                functionDefinition->getPrototype()->isForwarded())
            {
                functionDefinition->getPrototype()->setIsForward(false);
                if (_CUR_TOKEN_TYPE_ == TokenType::SYMBOL_SEMICOLON)
                    return std::make_shared<PrototypeExpression>(*functionDefinition->getPrototype().get());
            }
        }
        std::vector<std::shared_ptr<VariableDefinition>> vars;
        if (_CUR_TOKEN_TYPE_ == TokenType::SYMBOL_PAREN_OPEN)
        {
            vars = parseFunctionParams();
            advance();
        }
        PrototypeExpression prototypeExpression =
            PrototypeExpression(current()->getTokenPos(), name, vars, getVoidType());
        return std::make_shared<PrototypeExpression>(prototypeExpression);
    }
}
std::shared_ptr<Function> Parser::parseFunction()
{
    std::string name;
    std::shared_ptr<PrototypeExpression> prototype = parseFunctionHeader();
    if (!prototype || _CUR_TOKEN_TYPE_ != TokenType::SYMBOL_SEMICOLON)
    {
        this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_, "Error??");
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
            this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_, "%s already exists.", name.c_str());
        }
        advance();
        if (_CUR_TOKEN_TYPE_ == TokenType::SYMBOL_FORWARD)
        {
            prototype->setIsForward(true);
            advance();
            if (_CUR_TOKEN_TYPE_ != TokenType::SYMBOL_SEMICOLON)
            {
                this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_, "Expected ';' after forward keyword");
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
            this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_, "Duplicated %s in prototype.",
                                                   v->getName().c_str());
        }
    }
    std::vector<std::shared_ptr<VariableDeclarationExpression>> variableDeclarations;
    std::shared_ptr<BlockExpression> body;
    std::vector<std::shared_ptr<Function>> innerFunctions;
    for (;;)
    {
        switch (_CUR_TOKEN_TYPE_)
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
    if (_CUR_TOKEN_TYPE_ == TokenType::SYMBOL_STRING_KEYWORD)
    {
        unsigned size = 255;
        advance();
        if (_CUR_TOKEN_TYPE_ == TokenType::SYMBOL_SQUARE_BRACKET_OPEN)
        {
            advance();
            if (_CUR_TOKEN_TYPE_ == TokenType::NUMERIC_LITERAL)
            {
                if (ParserUtils::numericLiteralIsInteger(_CUR_TOKEN_OBJ_))
                {
                    size = stoi(current()->getValue());
                    if (_CUR_TOKEN_TYPE_ == TokenType::SYMBOL_SQUARE_BRACKET_CLOSE)
                    {
                        advance();
                    }
                    else
                    {
                        this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_, "Expecting ']'.");
                        this->sync();
                    }
                }
                else
                {
                    this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_,
                                                           "Expecting integer, but got other type instead.");
                }
            }
            else
            {
                this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_,
                                                       "Expecting number, but got other type instead.");
            }
        }
        else
        {
            if (_CUR_TOKEN_TYPE_ == TokenType::SYMBOL_PAREN_OPEN)
            {
                advance();
                if (_CUR_TOKEN_TYPE_ == TokenType::NUMERIC_LITERAL)
                {
                    if (ParserUtils::numericLiteralIsInteger(_CUR_TOKEN_OBJ_))
                    {
                        size = stoi(current()->getValue());
                        if (_CUR_TOKEN_TYPE_ == TokenType::SYMBOL_PAREN_OPEN)
                        {
                            advance();
                        }
                        else
                        {
                            this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_, "Expecting ']'.");
                            this->sync();
                        }
                    }
                    else
                    {
                        this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_,
                                                               "Expecting integer, but got other type instead.");
                    }
                }
                else
                {
                    this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_,
                                                           "Expecting number, but got other type instead.");
                }
            }
        }
        if (size == 0)
        {
            this->diagnosticsEngine->japc_warning_at(_CUR_TOKEN_OBJ_, "Huh? String with size 0 ?");
        }
        return std::make_shared<StringDeclaration>(StringDeclaration(size));
    }
    else
    {
    }
}
std::shared_ptr<ArrayDeclaration> Parser::parseArrayDeclaration()
{
    advance(); // eat array keyword
    if (_CUR_TOKEN_TYPE_ == TokenType::SYMBOL_SQUARE_BRACKET_OPEN)
    {
        advance();
        std::vector<std::shared_ptr<RangeDeclaration>> rangeVector;
        std::shared_ptr<TypeDeclaration> typeDeclaration;
        while (_CUR_TOKEN_TYPE_ != TokenType::SYMBOL_SQUARE_BRACKET_CLOSE)
        {
            std::shared_ptr<RangeDeclaration> rangeDeclaration = this->parseArrayDeclarationRange();
            if (!rangeDeclaration)
            {
                this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_, "I was expecting to see a type here");
                this->sync();
            }
            else
            {
                rangeVector.push_back(rangeDeclaration);
            }
            if (_CUR_TOKEN_TYPE_ == TokenType::SYMBOL_COMMA)
            {
                advance();
            }
        }
        advance();
        if (rangeVector.empty())
        {
            this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_, "Expected size of array now");
            this->sync();
        }
        if (_CUR_TOKEN_TYPE_ == TokenType::SYMBOL_OF)
        {
            advance(); // eat of keyword
            std::shared_ptr<TypeDeclaration> typeDeclaration = this->parseType();
            return std::make_shared<ArrayDeclaration>(ArrayDeclaration(typeDeclaration, rangeVector));
        }
        else
        {
            this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_, "Expected OF keyword");
            this->sync();
        }
    }
    else
    {
        this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_, "Expected '[' after array keyword");
        this->sync();
    }
}
std::shared_ptr<RangeDeclaration> Parser::parseArrayDeclarationRange()
{
    std::shared_ptr<TypeDeclaration> typeDeclaration;
    if (_CUR_TOKEN_TYPE_ == TokenType::IDENTIFIER)
    {
        typeDeclaration = getTypeDeclaration(current()->getValue());
        if (typeDeclaration)
        {
            if (!typeDeclaration->isIntegral())
            {
                this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_,
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
    if (_CUR_TOKEN_TYPE_ == TokenType::SYMBOL_FILE)
    {
        advance();
        if (_CUR_TOKEN_TYPE_ == TokenType::SYMBOL_FILE)
        {
            std::shared_ptr<TypeDeclaration> type = parseType();
            if (type)
            {
                return std::make_shared<FileDeclaration>(FileDeclaration(type));
            }
        }
        else
        {
            this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_, "Expected OF keyword now");
            this->sync();
        }
    }
    else
    {
        this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_, "Expected FILE keyword now");
        this->sync();
    }
    return nullptr;
}
std::shared_ptr<SetDeclaration> Parser::parseSetDeclaration()
{
    if (_CUR_TOKEN_TYPE_ == TokenType::SYMBOL_SET)
    {
        advance();
        if (_CUR_TOKEN_TYPE_ == TokenType::SYMBOL_OF)
        {
            advance();
            std::shared_ptr<TypeDeclaration> typeDeclaration;
            std::shared_ptr<RangeDeclaration> rangeDeclaration;
            if (_CUR_TOKEN_TYPE_ == TokenType::IDENTIFIER)
            {
                typeDeclaration = getTypeDeclaration(current()->getValue());
                if (typeDeclaration)
                {
                    if (!typeDeclaration->isIntegral())
                    {
                        this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_, "Given type is not an integral type");
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
                        this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_, "Set is too large...");
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
            this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_, "Expected OF keyword now");
            this->sync();
        }
    }
    else
    {
        this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_, "Expected SET keyword now");
        this->sync();
    }
    return nullptr;
}
std::shared_ptr<EnumDeclaration> Parser::parseEnumDefinition()
{
    if (_CUR_TOKEN_TYPE_ == TokenType::SYMBOL_PAREN_OPEN)
    {
        advance();
        std::vector<std::string> names;
        do
        {
            if (_CUR_TOKEN_TYPE_ != TokenType::IDENTIFIER)
            {
                names.push_back(current()->getValue());
                advance();
                if (_CUR_TOKEN_TYPE_ == TokenType::SYMBOL_COMMA)
                {
                    advance();
                }
                else if (_CUR_TOKEN_TYPE_ != TokenType::SYMBOL_PAREN_CLOSE)
                {
                    this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_, "Expected ',' of ')' in enum declaration");
                    this->sync();
                }
            }
            else
            {
                this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_, "Expected identifier in enum declaration");
                this->sync();
            }
        } while (_CUR_TOKEN_TYPE_ != TokenType::SYMBOL_PAREN_CLOSE);
        if (names.empty())
        {
            this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_, "Enum declaration cannot be empty");
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
                    this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_,
                                                           ("Value " + value.name + "already declared").c_str());
                    return nullptr;
                }
            }
            return enumDeclaration;
        }
    }
    else
    {
        this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_, "Expected '(' for enum declaration");
        this->sync();
    }
    return nullptr;
}
void Parser::parseConstantDefinition()
{
    if (_CUR_TOKEN_TYPE_ == TokenType::SYMBOL_CONST)
    {
        advance();
        do
        {
            if (_CUR_TOKEN_TYPE_ == TokenType::IDENTIFIER)
            {
                advance();
                std::string name = current()->getValue();
                if (_CUR_TOKEN_TYPE_ == TokenType::SYMBOL_EQUAL) // Constants are defined using = not :=
                {
                    advance();
                    std::shared_ptr<ConstantDeclaration> constantDeclaration =
                        parseConstantExpression({TokenType::SYMBOL_SEMICOLON});
                    if (!constantDeclaration)
                    {
                        this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_, "Constant is not well defined");
                    }
                    else
                    {
                        if (!this->objects->insert(name,
                                                   std::make_shared<ConstantDefinition>(name, constantDeclaration)))
                        {
                            this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_, "%s Is already defined.",
                                                                   name.c_str());
                        }
                        else
                        {
                            if (_CUR_TOKEN_TYPE_ != TokenType::SYMBOL_SEMICOLON)
                            {
                                this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_,
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
                    this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_,
                                                           "Constants must be initialized at definition time.");
                }
            }
            else
            {
                this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_, "Expected identifier in constant definition.");
                this->sync();
            }
        } while (_CUR_TOKEN_TYPE_ == TokenType::IDENTIFIER);
    }
    else
    {
        this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_, "Expected CONST keyword in constant definition.");
        this->sync();
    }
}
void Parser::sync(std::vector<TokenType> tokensToFind)
{
    while (!ParserUtils::isAnyOf(_CUR_TOKEN_OBJ_, tokensToFind))
    {
        advance();
        if (_CUR_TOKEN_TYPE_ == TokenType::END_OF_FILE)
        {
            this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_, "Unexpected EOF while recovering");
            this->sync();
        }
    }
}
void Parser::parseLabel()
{
    std::vector<int64_t> labelsNames;
    if (_CUR_TOKEN_TYPE_ == TokenType::SYMBOL_LABEL)
    {
        advance();
        do
        {
            if (_CUR_TOKEN_TYPE_ == TokenType::NUMERIC_LITERAL)
            {
                if (ParserUtils::numericLiteralIsInteger(*current()))
                {
                    labelsNames.push_back(atoi(current()->getValue().c_str()));
                    advance();
                }
                else
                {
                    this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_,
                                                           "%s is not a integer or cannot be converted to one",
                                                           current()->getValue().c_str());
                    this->sync();
                }
            }
            else
            {
                this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_, "Expected integer label");
                this->sync();
            }
            if (_CUR_TOKEN_TYPE_ == TokenType::SYMBOL_COMMA)
            {
                advance();
            }
        } while (_CUR_TOKEN_TYPE_ != TokenType::SYMBOL_SEMICOLON);
        advance(); // eat semicolon;
        for (auto n : labelsNames)
        {
            if (!this->objects->insert(std::to_string(n), std::make_shared<LabelDefinition>(n)))
            {
                this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_, "%d label was already declared.", n);
            }
        }
    }
    else
    {
        this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_, "Expected LABEL keywword");
        this->sync();
    }
}
std::shared_ptr<VariableDeclarationExpression> Parser::parseVarDeclarations()
{
    if (_CUR_TOKEN_TYPE_ == TokenType::SYMBOL_VAR)
    {
        advance();
        std::vector<std::shared_ptr<VariableDefinition>> varDefs;
        do
        {
            std::vector<std::string> names;
            do
            {
                if (_CUR_TOKEN_TYPE_ == TokenType::IDENTIFIER)
                {
                    names.push_back(current()->getValue());
                    advance();
                }
                else
                {
                    this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_, "Expected identifier in var definition");
                    this->sync();
                }
            } while (_CUR_TOKEN_TYPE_ == TokenType::SYMBOL_COMMA && advance());
            if (_CUR_TOKEN_TYPE_ == TokenType::SYMBOL_COLON)
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
                            this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_, "%s is already defined",
                                                                   varDef->getName().c_str());
                        }
                    }
                    advance();
                    if (_CUR_TOKEN_TYPE_ == TokenType::SYMBOL_VALUE)
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
                                _CUR_TOKEN_OBJ_, "Expected variable initialization after VALUE keyword");
                            this->sync();
                        }
                        return std::make_shared<VariableDeclarationExpression>(current().get()->getTokenPos(), varDefs);
                    }
                    if (_CUR_TOKEN_TYPE_ != TokenType::SYMBOL_SEMICOLON)
                    {
                        this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_,
                                                               "Expected ';' after variable initialization");
                        this->sync();
                    }
                    else
                    {
                        advance();
                        return std::make_shared<VariableDeclarationExpression>(current().get()->getTokenPos(), varDefs);
                    }
                }
                else
                {
                    this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_,
                                                           "Expected type to be defined after variable declaration");
                    this->sync();
                }
            }
            else
            {
                this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_, "Expected ':' in after variable identifier");
                this->sync();
            }
        } while (_CUR_TOKEN_TYPE_ == TokenType::IDENTIFIER);
        if (_CUR_TOKEN_TYPE_ == TokenType::SYMBOL_SEMICOLON)
        {
            advance();
        }
        else
        {
            this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_, "Expected ';'");
            this->sync();
        }
    }
    else
    {
        this->sync();
        return nullptr;
    }
}
std::shared_ptr<InitValue> Parser::parseInitValue(std::shared_ptr<TypeDeclaration> typeOfInitialization)
{
    if (_CUR_TOKEN_TYPE_ == TokenType::SYMBOL_SQUARE_BRACKET_OPEN)
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
        _CUR_TOKEN_OBJ_, "Expected constant integral or real expression or set expression in variable initialization");
    return nullptr;
}
std::shared_ptr<BlockExpression> Parser::parseBlock()
{
    if (_CUR_TOKEN_TYPE_ == TokenType::SYMBOL_BEGIN)
    {
        advance(); // eat begin keyword
        std::vector<std::shared_ptr<ExpressionAST>> content;
        Location loc = current()->getTokenPos();
        while (_CUR_TOKEN_TYPE_ != TokenType::SYMBOL_END)
        {
            if (_CUR_TOKEN_TYPE_ == TokenType::NUMERIC_LITERAL &&
                ParserUtils::numericLiteralIsInteger(_CUR_TOKEN_OBJ_) &&
                lookAhead(1)->getTokenType() == TokenType::SYMBOL_COLON)
            {
                Token cur = _CUR_TOKEN_OBJ_;
                advance(); // eat numeric literal
                advance(); // colon
                int64_t label_val = ParserUtils::convertStringToInteger(cur.getValue());
                std::string labelName = current()->getValue();
                if (!this->objects->findAtTop(labelName))
                {
                    this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_, "Label %s cannot be used in this scope",
                                                           labelName.c_str());
                    return nullptr;
                }
                content.push_back(std::make_shared<LabelExpression>(
                    LabelExpression(cur.getTokenPos(), {{label_val, label_val}}, nullptr)));
            }
            else
            {
                std::shared_ptr<ExpressionAST> expression = parseStatement();
                if (expression)
                {
                    content.push_back(expression);
                    if (_CUR_TOKEN_TYPE_ != TokenType::SYMBOL_SEMICOLON && _CUR_TOKEN_TYPE_ != TokenType::SYMBOL_END)
                    {
                        this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_,
                                                               "Expected ';' or END keyword at this point");
                        this->sync();
                    }
                    else
                    {
                        if (_CUR_TOKEN_TYPE_ == TokenType::SYMBOL_SEMICOLON)
                            advance(); // eat semicolon
                    }
                }
                else
                {
                    this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_,
                                                           "Expected an expression or statement at this point");
                    this->sync();
                }
            }
        }
        advance(); // eat END keyword
        return std::make_shared<BlockExpression>(loc, content);
    }
    else
    {
        this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_, "Expected BEGIN keyword");
        this->sync();
    }
}
std::shared_ptr<ExpressionAST> Parser::parseStatement()
{
    switch (_CUR_TOKEN_TYPE_)
    {
    case TokenType::SYMBOL_BEGIN: {
        std::shared_ptr<BlockExpression> block = parseBlock();
        if (_CUR_TOKEN_TYPE_ == TokenType::SYMBOL_SEMICOLON)
        {
            return block;
        }
        else
        {
            this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_, "Expected ';' at the end of the block");
            this->sync();
        }
        break;
    }
    case TokenType::SYMBOL_SEMICOLON:
    case TokenType::SYMBOL_END:
        this->diagnosticsEngine->japc_warning_at(_CUR_TOKEN_OBJ_, "Is this an empty statement?");
        return std::make_shared<BlockExpression>(BlockExpression(current()->getTokenPos(), {}));
    default:
        std::shared_ptr<ExpressionAST> expre = parsePrimary();
        if (expre)
        {
            if (_CUR_TOKEN_TYPE_ == TokenType::SYMBOL_COLON_EQUAL)
            {              // assign expression
                advance(); // eat assign token
                Location loc = current()->getTokenPos();
                std::shared_ptr<ExpressionAST> rhs = parseExpression();
                if (rhs)
                {
                    expre = std::make_shared<AssignExpression>(AssignExpression(loc, expre, rhs));
                }
                else
                {
                    this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_, "Expected expression in assignment");
                    this->sync();
                }
            }
            return expre;
        }
        else
        {
            this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_, "Expected expression at this point");
            this->sync();
        }
        break;
    }
    return nullptr; // Ideally you should not get here. We do not report anything as we may not be expecting that to
                    // happend.
}
std::shared_ptr<TypeDeclaration> Parser::parseSimpleType()
{
    if (_CUR_TOKEN_TYPE_ == TokenType::IDENTIFIER)
    {
        std::string name = current()->getValue();
        std::shared_ptr<TypeDeclaration> ty;
        if (const TypeDefinition *typeDef =
                llvm::dyn_cast_or_null<const TypeDefinition>(this->objects->find(name).get()))
        {
            return typeDef->getTypeDeclaration();
        }
        else
        {
            this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_, "%s is not a type", name.c_str());
        }
    }
}

std::shared_ptr<ExpressionAST> Parser::parseArrayExpression(std::shared_ptr<ExpressionAST> expression,
                                                            std::shared_ptr<TypeDeclaration> type)
{
    ArrayDeclaration *arrayDeclaration = llvm::dyn_cast_or_null<ArrayDeclaration>(type.get());
    if (!arrayDeclaration)
    {
        this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_, "Unable to access non array identifier");
        this->sync();
    }
    size_t taken = 0;
    std::vector<std::shared_ptr<ExpressionAST>> indexs;
    advance(); // EAT OPEN SQURE BRACKETs
    while (_CUR_TOKEN_TYPE_ != TokenType::SYMBOL_SQUARE_BRACKET_CLOSE)
    {
        std::shared_ptr<ExpressionAST> indexExpr = parseExpression();
        if (_CUR_TOKEN_TYPE_ == TokenType::SYMBOL_COMMA)
        {
            advance();
            if (_CUR_TOKEN_TYPE_ == TokenType::SYMBOL_SQUARE_BRACKET_CLOSE)
            {
                this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_,
                                                       "',' must be followed by a expression in array access");
                this->sync();
            }
        }
        if (!indexExpr)
        {
            this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_, "Expected expression while parsing array access");
            this->sync();
        }
        indexs.push_back(indexExpr);
    }
    advance(); // eat square bracket close
    if (indexs.size() < 1)
    {
        this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_, "Empty array access statement");
    }
    std::vector<std::shared_ptr<ExpressionAST>> indices(indexs);
    while (!indices.empty())
    {
        if (indices.size() >= arrayDeclaration->getRanges().size())
        {
            taken += arrayDeclaration->getRanges().size();
            indices.resize(arrayDeclaration->getRanges().size());
            expression =
                std::make_shared<ArrayExpression>(current()->getTokenPos(), expression, indices,
                                                  arrayDeclaration->getRanges(), arrayDeclaration->getSubtype());
            type = arrayDeclaration->getSubtype();
            indices = indexs;
            indices.erase(indices.begin(), indices.begin() + taken);
            if (!(arrayDeclaration = llvm::dyn_cast_or_null<ArrayDeclaration>(type.get())))
            {
                if (!indices.empty())
                {
                    this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_, "Providing more indices than the expected");
                    this->sync(); // May not require sync
                }
                return expression;
            }
        }
    }
    return expression;
}
std::shared_ptr<ExpressionAST> Parser::parsePointerExpression(std::shared_ptr<ExpressionAST> expression,
                                                              std::shared_ptr<TypeDeclaration> type)
{
    if (_CUR_TOKEN_TYPE_ == TokenType::SYMBOL_AT)
    {
        advance();
        if (llvm::isa<FileDeclaration>(type.get()))
        {
            type = type->getSubtype();
            return std::make_shared<FilePointerExpression>(current()->getTokenPos(), expression, type);
        }
        else if (!llvm::isa<PointerDeclaration>(type.get()))
        {
            this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_, "Expected pointer expression");
            this->sync();
        }
        else
        {
            type = type->getSubtype();
            return std::make_shared<PointerExpression>(current()->getTokenPos(), expression, type);
        }
    }
    else
    {
        this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_, "Expected '@' symbol while parsing pointer expression");
        this->sync();
    }
    return std::shared_ptr<ExpressionAST>(); // You should not hit here
}
std::shared_ptr<ExpressionAST> Parser::parseFieldExpression(std::shared_ptr<ExpressionAST> expression,
                                                            std::shared_ptr<TypeDeclaration> type)
{
    return std::shared_ptr<ExpressionAST>();
}
std::shared_ptr<ExpressionAST> Parser::parseIfExpr()
{
    if (_CUR_TOKEN_TYPE_ == TokenType::SYMBOL_IF)
    {
        advance();
        std::shared_ptr<ExpressionAST> conditionExpression = parseExpression();
        if (_CUR_TOKEN_TYPE_ == TokenType::SYMBOL_THEN && conditionExpression)
        {
            advance(); // eat then
            std::shared_ptr<ExpressionAST> thenStatement = parseStatement();
            if (!thenStatement)
            {
                this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_, "Expected then statement after condition.");
                this->sync();
            }
            else
            {
                std::shared_ptr<ExpressionAST> elseStatement;
                if (_CUR_TOKEN_TYPE_ == TokenType::SYMBOL_ELSE)
                {
                    advance(); // eat else
                    elseStatement = parseStatement();
                    if (elseStatement)
                    {
                        return std::make_shared<IfExpression>(current()->getTokenPos(), conditionExpression,
                                                              thenStatement, elseStatement);
                    }
                    else
                    {
                        this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_,
                                                               "Expected else statement after condition.");
                        this->sync();
                    }
                }
                return std::make_shared<IfExpression>(current()->getTokenPos(), conditionExpression, thenStatement,
                                                      elseStatement);
            }
        }
        else
        {
            this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_, "Expected THEN keyword after condition.");
            this->sync();
        }
    }
    else
    {
        this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_, "Expected 'IF' keyword");
        this->sync();
    }
}
std::shared_ptr<ExpressionAST> Parser::parseForExpr()
{
    if (_CUR_TOKEN_TYPE_ == TokenType::SYMBOL_FOR)
    {
        advance();
        if (_CUR_TOKEN_TYPE_ == TokenType::IDENTIFIER)
        {
            std::string name = current()->getValue();
            std::shared_ptr<NamedObject> def = this->objects->find(name);
            advance(); // eat identifier
            if (def && llvm::isa<VariableDefinition>(def.get()))
            {
                std::shared_ptr<VariableExpression> varExpression =
                    std::make_shared<VariableExpression>(current()->getTokenPos(), name, def->getTypeDeclaration());
                if (_CUR_TOKEN_TYPE_ == TokenType::SYMBOL_COLON_EQUAL)
                {
                    advance();
                    if (std::shared_ptr<ExpressionAST> startExpression = parseExpression())
                    {
                        bool downto = false;
                        if (_CUR_TOKEN_TYPE_ == TokenType::SYMBOL_TO || _CUR_TOKEN_TYPE_ == TokenType::SYMBOL_DOWNTO)
                        {
                            if (_CUR_TOKEN_TYPE_ == TokenType::SYMBOL_DOWNTO)
                                downto = true;
                            advance();
                            if (std::shared_ptr<ExpressionAST> endExpression = parseExpression())
                            {
                                if (_CUR_TOKEN_TYPE_ == TokenType::SYMBOL_DO)
                                {
                                    advance();
                                    if (std::shared_ptr<ExpressionAST> bodyStatement = parseStatement())
                                    {
                                        return std::make_shared<ForExpression>(current()->getTokenPos(), varExpression,
                                                                               startExpression, endExpression, downto,
                                                                               bodyStatement);
                                    }
                                }
                                else
                                {
                                    this->diagnosticsEngine->japc_error_at(
                                        _CUR_TOKEN_OBJ_, "Expected DO after expression in FOR statement");
                                    this->sync();
                                }
                            } // DO not sync
                        }
                        else
                        {
                            this->diagnosticsEngine->japc_error_at(
                                _CUR_TOKEN_OBJ_, "Expected TO or DOWNTO after expression in FOR statement");
                            this->sync();
                        }
                    } // No need to sync. If there is no expression, we will be in sync state
                }
                else if (_CUR_TOKEN_TYPE_ == TokenType::SYMBOL_IN)
                {
                    advance();
                    if (std::shared_ptr<ExpressionAST> startExpression = parseExpression())
                    {
                        if (_CUR_TOKEN_TYPE_ == TokenType::SYMBOL_DO)
                        {
                            advance();
                            if (std::shared_ptr<ExpressionAST> body = parseStatement())
                            {
                                return std::make_shared<ForExpression>(current()->getTokenPos(), varExpression,
                                                                       startExpression, body);
                            }
                        }
                        else
                        {
                            this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_,
                                                                   "Expected DO after expression in FOR statement");
                            this->sync();
                        }
                    }
                }
                else
                {
                    this->diagnosticsEngine->japc_error_at(
                        _CUR_TOKEN_OBJ_, "Expected expression ':=' or IN operator after variable in FOR statement");
                    this->sync();
                }
            }
            else
            {
                this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_, "%s is not defined or does not name a variable",
                                                       name.c_str());
            }
        }
        else
        {
            this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_, "Expected identifier after 'FOR' keyword");
            this->sync();
        }
    }
    else
    {
        this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_, "Expected 'FOR' keyword");
        this->sync();
    }
}
std::shared_ptr<ExpressionAST> Parser::parseWhile()
{
    Location loc = current()->getTokenPos();
    if (_CUR_TOKEN_TYPE_ == TokenType::SYMBOL_WHILE)
    {
        advance();
        std::shared_ptr<ExpressionAST> cond = parseExpression();
        if (cond)
        {
            if (_CUR_TOKEN_TYPE_ == TokenType::SYMBOL_DO)
            {
                advance(); // eat do keyword
                if (std::shared_ptr<ExpressionAST> statement = parseStatement())
                {
                    return std::make_shared<WhileExpression>(loc, cond, statement);
                }
            }
            else
            {
                this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_, "Expected DO keyword");
                this->sync();
            }
        }
    }
    else
    {
        this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_, "Expected WHILE keyword");
        this->sync();
    }
}
std::shared_ptr<ExpressionAST> Parser::parseRepeat()
{
    return std::shared_ptr<ExpressionAST>();
}
std::shared_ptr<ExpressionAST> Parser::parseCaseExpr()
{
    return std::shared_ptr<ExpressionAST>();
}
std::shared_ptr<ExpressionAST> Parser::parseWithBlock()
{
    return std::shared_ptr<ExpressionAST>();
}
std::shared_ptr<ExpressionAST> Parser::parseGoto()
{
    if (_CUR_TOKEN_TYPE_ == TokenType::SYMBOL_GOTO)
    {
        advance();
        if (_CUR_TOKEN_TYPE_ == TokenType::NUMERIC_LITERAL && ParserUtils::numericLiteralIsInteger(_CUR_TOKEN_OBJ_))
        {
            int val = ParserUtils::convertStringToInteger(current()->getValue());
            std::string name = std::to_string(val);
            if (!this->objects->findAtTop(name))
            {
                if (this->objects->find(name))
                {
                    this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_, "Label %s is not defined", name.c_str());
                    this->sync();
                }
                else
                {
                    this->diagnosticsEngine->japc_error_at(
                        _CUR_TOKEN_OBJ_, "Label %s must be defined at same scope of goto expression", name.c_str());
                    this->sync();
                }
            }
            else
            {
                return std::make_shared<GotoExpression>(current()->getTokenPos(), val);
            }
        }
        else
        {
            this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_, "%s does not name a valid label name",
                                                   current()->getValue().c_str());
            this->sync();
        }
    }
    else
    {
        this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_, "Expected GOTO keyword");
        this->sync();
    };
    return std::shared_ptr<ExpressionAST>();
}
std::shared_ptr<ExpressionAST> Parser::parseFunctionCallOrVariableExpression(Token tk)
{
    std::string value = tk.getValue();
    advance(); // eat identifier
    std::shared_ptr<NamedObject> def = this->objects->find(value);
    if (EnumDefinition *enumDef = llvm::dyn_cast_or_null<EnumDefinition>(def.get()))
    {
        return std::make_shared<IntegerExpression>(tk.getTokenPos(), enumDef->getValue(),
                                                   enumDef->getTypeDeclaration());
    }
    if (!def)
    {
        this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_, "%s is not already defined.", value.c_str());
        return nullptr;
    }
    else
    {
        if (!ParserUtils::isFunctionCall(def.get(), *this->lookAhead(1).get()))
        {
            return parseVariableExpression(def);
        }
    }
    std::vector<std::shared_ptr<ExpressionAST>> args;
    if (!parseArgs(def, args))
    {
        this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_,
                                               "Error while parsing function arguments, did you missed ')' or ','?");
        this->sync();
    }
    if (def)
    {
        std::shared_ptr<ExpressionAST> functionCall;
        functionCall = createCallExpression(def, value, args);
        if (functionCall)
        {
            return functionCall;
        }
    }
    return std::shared_ptr<ExpressionAST>();
}
std::shared_ptr<ExpressionAST> Parser::parseExpressionTerm()
{
    Token tk = _CUR_TOKEN_OBJ_;
    if (_CUR_TOKEN_TYPE_ == TokenType::IDENTIFIER)
    {
        std::string name = current()->getValue();
        if (ConstantDefinition *constantDefinition =
                llvm::dyn_cast_or_null<ConstantDefinition>(this->objects->find(name).get()))
        {
            tk = constantDefinition->getConstValue()->toTokenType();
        }
    }
    switch (tk.getTokenType())
    {
    case TokenType::SYMBOL_DEFAULT:
        parseDefaultExpression();
    case TokenType::SYMBOL_NIL:
        advance(); // eat NIL
        return std::make_shared<NullExpression>(current()->getTokenPos());
    case TokenType::NUMERIC_LITERAL:
        if (ParserUtils::numericLiteralIsInteger(tk))
        {
            return parseIntegerExpression(tk);
        }
        else
        {
            advance();
            long double val = 0.0;
            try
            {
                val = std::stod(tk.getValue());
            }
            catch (const std::out_of_range &oor)
            {
                this->diagnosticsEngine->japc_warning_at(_CUR_TOKEN_OBJ_,
                                                         "%s is too large for an real. It exceed LDBL_MAX size",
                                                         current()->getValue().c_str());
            }
            advance(); // EAT Value
            return std::make_shared<RealExpression>(current()->getTokenPos(), val, getRealType());
        }
    case TokenType::STRING_LITERAL:
        if (current()->getValue().size() == 1)
        {
            advance();
            std::shared_ptr<CharExpression> expr =
                std::make_shared<CharExpression>(current()->getTokenPos(), current()->getValue()[0], getCharType());
            return expr;
        }
        else
        {
            return parseStringExpression();
        }
    case TokenType::SYMBOL_PAREN_OPEN:
        return parseParenExpression();
    case TokenType::SYMBOL_SQUARE_BRACKET_OPEN:
        return parseSetExpression();
    case TokenType::SYMBOL_MINUS:
    case TokenType::SYMBOL_PLUS:
    case TokenType::SYMBOL_NOT:
        return parseUnaryOperatorExpression();
    case TokenType::SYMBOL_SIZEOF:
        return parseSizeOfExpression();
    case TokenType::IDENTIFIER:
        return parseIdentifierExpression(tk);
    }
    return nullptr;
}
std::shared_ptr<ExpressionAST> Parser::parseBinaryOperatorRightSide(int expressionPrecedence,
                                                                    std::shared_ptr<ExpressionAST> leftSideExpression)
{
    for (;;)
    {
        std::unique_ptr<Token> binaryOperator = current();
        int tokenPrec = ParserUtils::getPrecedence(binaryOperator->getTokenType());
        if (tokenPrec < expressionPrecedence)
        {
            return leftSideExpression;
        }
        advance();
        std::shared_ptr<ExpressionAST> rightSideExpression = parseExpressionTerm();
        if (!rightSideExpression)
        {
            // TODO: Verify if error is needed
            return nullptr;
        }

        int nextPrecedence = ParserUtils::getPrecedence(current()->getTokenType());
        if (tokenPrec < nextPrecedence)
        {
            if (!(rightSideExpression = parseBinaryOperatorRightSide(tokenPrec + 1, rightSideExpression)))
            {
                return nullptr;
            }
        }
        leftSideExpression =
            std::make_shared<BinaryExpression>(*binaryOperator.get(), leftSideExpression, rightSideExpression);
    }
}
std::shared_ptr<ExpressionAST> Parser::parseDefaultExpression()
{
    if (_CUR_TOKEN_TYPE_ == TokenType::SYMBOL_DEFAULT)
    {
        advance();
        if (_CUR_TOKEN_TYPE_ == TokenType::SYMBOL_PAREN_OPEN)
        {
            advance();
            std::shared_ptr<ExpressionAST> expressionAst;
            std::shared_ptr<TypeDeclaration> typeDeclaration;
            if (_CUR_TOKEN_TYPE_ == TokenType::IDENTIFIER)
            {
                std::string typeName = current()->getValue();
                TypeDefinition *typeDef = llvm::dyn_cast_or_null<TypeDefinition>(this->objects->find(typeName).get());
                if (typeDef)
                {
                    typeDeclaration = typeDef->getTypeDeclaration();
                }
            }
            if (!typeDeclaration)
            {
                if (expressionAst = parseExpression())
                {
                    typeDeclaration = expressionAst->getTypeDeclaration();
                }
            }
            if (typeDeclaration)
            {
                expressionAst = typeDeclaration->getInitial();
            }
            advance();
            if (_CUR_TOKEN_TYPE_ == TokenType::SYMBOL_PAREN_CLOSE)
            {
                advance();
                return expressionAst;
            }
            else
            {
                this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_, "Expected ')' after expression");
                this->sync();
            }
        }
        else
        {
            this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_, "Expected '(' after DEFAULT keyword");
            this->sync();
        }
    }
    else
    {
        return nullptr;
    }
}
std::shared_ptr<ExpressionAST> Parser::parseIntegerExpression(Token tk)
{
    int64_t value = ParserUtils::convertStringToInteger(tk.getValue());
    std::shared_ptr<TypeDeclaration> type;
    if (value < std::numeric_limits<unsigned int>::max())
    {
        type = getIntegerType();
    }
    else
    {
        type = getLongIntType();
    }
    // TODO: Remove this one as this method is not depending in current token
    advance(); // current token
    return std::make_shared<IntegerExpression>(tk.getTokenPos(), value, type);
}
std::shared_ptr<ExpressionAST> Parser::parseStringExpression()
{
    int len = std::max(1, (int)(current()->getValue().length() - 1));
    std::vector<std::shared_ptr<RangeDeclaration>> rv = {
        std::make_shared<RangeDeclaration>(std::make_shared<Range>(0, len), getIntegerType())};
    std::shared_ptr<ArrayDeclaration> arrayDeclaration = std::make_shared<ArrayDeclaration>(getCharType(), rv);

    std::shared_ptr<StringExpression> stringExpression =
        std::make_shared<StringExpression>(current()->getTokenPos(), current()->getValue(), arrayDeclaration);
    advance(); // Eat string
    return stringExpression;
}
std::shared_ptr<ExpressionAST> Parser::parseParenExpression()
{
    if (_CUR_TOKEN_TYPE_ == TokenType::SYMBOL_PAREN_OPEN)
    {
        advance();
        std::shared_ptr<ExpressionAST> expressionAst = parseExpression();
        if (expressionAst && _CUR_TOKEN_TYPE_ == TokenType::SYMBOL_PAREN_CLOSE)
        {
            return expressionAst;
        }
        else
        {
            this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_, "Expected ')' after expression");
            this->sync();
        }
    }
    else
    {
        this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_, "Expected '(' before expression");
        this->sync();
    }
}
std::shared_ptr<ExpressionAST> Parser::parseSetExpression()
{
    return std::shared_ptr<ExpressionAST>();
}
std::shared_ptr<ExpressionAST> Parser::parseUnaryOperatorExpression()
{
    if (_CUR_TOKEN_TYPE_ == TokenType::SYMBOL_MINUS || _CUR_TOKEN_TYPE_ == TokenType::SYMBOL_PLUS ||
        _CUR_TOKEN_TYPE_ == TokenType::SYMBOL_NOT)
    {
        Token operat = *current().get();
        if (std::shared_ptr<ExpressionAST> rightSide = parseExpressionTerm())
        {
            if (operat.getTokenType() == TokenType::SYMBOL_PLUS)
            {
                return rightSide;
            }
            return std::make_shared<UnaryExpression>(operat.getTokenPos(), operat, rightSide);
        } // Do not sync, we are sync as error in expression parsing
    }
    else
    {
        this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_, "Expected unary operator");
        this->sync();
    }
}
std::shared_ptr<ExpressionAST> Parser::parseSizeOfExpression()
{
    if (_CUR_TOKEN_TYPE_ == TokenType::SYMBOL_SIZEOF)
    {
        advance();
        if (_CUR_TOKEN_TYPE_ == TokenType::SYMBOL_PAREN_OPEN)
        {
            advance();
            std::shared_ptr<ExpressionAST> expressionAst;
            if (_CUR_TOKEN_TYPE_ == TokenType::IDENTIFIER)
            {
                std::shared_ptr<TypeDeclaration> type;
                std::string typeName = current()->getValue();
                TypeDefinition *typeDef = llvm::dyn_cast_or_null<TypeDefinition>(this->objects->find(typeName).get());
                if (typeDef)
                {
                    type = typeDef->getTypeDeclaration();
                }
                if (type)
                {
                    expressionAst = std::make_shared<SizeOfExpression>(current()->getTokenPos(), type);
                }
            }
            if (!expressionAst)
            {
                if (std::shared_ptr<ExpressionAST> e = parseExpression())
                {
                    expressionAst =
                        std::make_shared<SizeOfExpression>(current()->getTokenPos(), e->getTypeDeclaration());
                }
            }
            if (_CUR_TOKEN_TYPE_ == TokenType::SYMBOL_PAREN_CLOSE)
            {
                advance();
                return expressionAst;
            }
            else
            {
                this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_, "Expected ')' at the end of sizeof expression");
                this->sync();
            }
        }
        else
        {
            this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_, "Expected '(' after SIZEOF keyword");
            this->sync();
        }
    }
    else
    {
        this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_, "Expected SIZEOF keyword");
        this->sync();
    }
}
std::shared_ptr<ExpressionAST> Parser::parseVariableExpression(std::shared_ptr<NamedObject> namedObject)
{
    std::shared_ptr<AddressableExpression> expression;
    std::shared_ptr<TypeDeclaration> typeDeclaration = namedObject->getTypeDeclaration();
    if (const WithDefinition *w = llvm::dyn_cast<WithDefinition>(namedObject.get()))
    {
        AddressableExpression *expre = llvm::dyn_cast<AddressableExpression>(w->getExpression().get()); // TODO: Review
        expression = std::shared_ptr<AddressableExpression>(expre);
    }
    else
    {
        if (FunctionDeclaration *fd = llvm::dyn_cast<FunctionDeclaration>(typeDeclaration.get()))
        {
            typeDeclaration = fd->getPrototypeExpression()->getTypeDeclaration();
        }
        expression =
            std::make_shared<VariableExpression>(current()->getTokenPos(), namedObject->getName(), typeDeclaration);
    }
    return expression;
}
bool Parser::parseArgs(std::shared_ptr<NamedObject> def, std::vector<std::shared_ptr<ExpressionAST>> &args)
{
    if (_CUR_TOKEN_TYPE_ == TokenType::SYMBOL_PAREN_OPEN)
    {
        advance();
        unsigned argNo = 0;
        std::shared_ptr<PrototypeExpression> proto = 0;
        if (const FunctionDefinition *funcDef = llvm::dyn_cast_or_null<FunctionDefinition>(def.get()))
        {
            proto = funcDef->getPrototype();
        }
        else if (const VariableDefinition *varDef = llvm::dyn_cast_or_null<VariableDefinition>(def.get()))
        {
            if (const FunctionPointerDeclaration *fp =
                    llvm::dyn_cast<FunctionPointerDeclaration>(varDef->getTypeDeclaration().get()))
            {
                proto = fp->getPrototype();
            }
        }
        while (_CUR_TOKEN_TYPE_ != TokenType::SYMBOL_PAREN_CLOSE)
        {
            bool isFuncArg = false;
            if (proto)
            {
                auto &funcArgs = proto->getArgs();
                if (argNo >= funcArgs.size())
                {
                    this->diagnosticsEngine->japc_error_at(
                        _CUR_TOKEN_OBJ_, "Too many arguments. Expected %d but %d were given", funcArgs.size(), argNo);
                    this->sync();
                }
                isFuncArg = llvm::isa<FunctionPointerDeclaration>(funcArgs[argNo]->getTypeDeclaration().get());
            }
            std::shared_ptr<ExpressionAST> arg = 0;
            if (isFuncArg)
            {
                Token token = _CUR_TOKEN_OBJ_;
                if (token.getTokenType() == TokenType::IDENTIFIER)
                {
                    std::string idName = token.getValue();
                    advance();
                    if (const std::shared_ptr<NamedObject> argDef = this->objects->find(idName))
                    {
                        if (const FunctionDefinition *fd = llvm::dyn_cast<FunctionDefinition>(argDef.get()))
                        {
                            arg = std::make_shared<FunctionExpression>(current()->getTokenPos(), fd->getPrototype());
                        }
                        else if (const VariableDefinition *vd = llvm::dyn_cast<VariableDefinition>(argDef.get()))
                        {
                            if (vd->getTypeDeclaration()->getTypeKind() == TypeKind::TYPE_FUNCION_POINTER)
                            {
                                arg = std::make_shared<VariableExpression>(current()->getTokenPos(), idName,
                                                                           argDef->getTypeDeclaration());
                            }
                        }
                    }
                }
                if (!arg)
                {
                    return false;
                }
            }
            else
            {
                arg = parseExpression();
            }
            if (!arg)
            {
                return false;
            }
            args.push_back(arg);
            if (_CUR_TOKEN_TYPE_ == TokenType::SYMBOL_COMMA)
            {
                advance();
                if (_CUR_TOKEN_TYPE_ == TokenType::SYMBOL_PAREN_CLOSE)
                {
                    return false;
                }
            }
            argNo++;
        }
        advance(); // Eat paren close
    }
    return true;
}
std::shared_ptr<ExpressionAST> Parser::createCallExpression(const std::shared_ptr<NamedObject> def,
                                                            const std::string &funcName,
                                                            std::vector<std::shared_ptr<ExpressionAST>> &args)
{
    std::shared_ptr<PrototypeExpression> proto;
    std::shared_ptr<ExpressionAST> expr;
    if (const FunctionDefinition *funcDef = llvm::dyn_cast<const FunctionDefinition>(def.get()))
    {
        proto = funcDef->getPrototype();
        expr = std::make_shared<FunctionExpression>(current()->getTokenPos(), proto);
    }
    else if (llvm::isa<const VariableDefinition>(def.get()))
    {
        if (FunctionPointerDeclaration *fp =
                llvm::dyn_cast<FunctionPointerDeclaration>(def->getTypeDeclaration().get()))
        {
            proto = fp->getPrototype();
            expr = std::make_shared<VariableExpression>(current()->getTokenPos(), funcName, def->getTypeDeclaration());
        }
        else
        {
            this->diagnosticsEngine->japc_error_at(_CUR_TOKEN_OBJ_, "Expected function pointer");
            return nullptr;
        }
    }
    else
    {
        assert(0 && "Huh?");
    }
    if (expr)
    {
        if (std::shared_ptr<Function> function = proto->getFunction())
        {
            if (std::shared_ptr<TypeDeclaration> typeDeclaration = function->getReturnType())
            {
                std::vector<std::shared_ptr<VariableExpression>> varsF;
                for (auto used : function->getUsedVars())
                {
                    varsF.push_back(std::make_shared<VariableExpression>(function->getLocation(), used.getName(),
                                                                         used.getTypeDeclaration()));
                }
                args.insert(args.begin(),
                            std::make_shared<ClosureExpression>(function->getLocation(), typeDeclaration, varsF));
            }
        }
        return std::make_shared<CallFunctExpression>(current()->getTokenPos(), expr, args, proto);
    }
    return nullptr;
}
