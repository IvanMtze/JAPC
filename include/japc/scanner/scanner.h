//
// Created by wuser on 30/06/22.
//

#ifndef JAPC_SCANNER_H
#define JAPC_SCANNER_H

#include "japc/basic/ptr.h"
#include <map>
#include <memory>
#include <regex>

namespace Pascal
{
enum class TokenFlags : int
{
    NONE = 0,
    PRECEDING_LINE_BREAK = 1 << 0,
    PRECEDING_COMMENT = 1 << 1,
    UNTERMINATED = 1 << 2,
    EXTENDED_UNICODE_ESCAPE = 1 << 3,
    SCIENTIFIC = 1 << 4,
    OCTAL = 1 << 5,
    HEX_SPECIFIER = 1 << 6,
    BINARY_SPECIFIER = 1 << 7,
    OCTAL_SPECIFIER = 1 << 8,
    CONTAINS_SEPARATOR = 1 << 9,
    UNICODE_ESCAPE = 1 << 10,
    CONTAINS_INVALID_ESCAPE = 1 << 11,
    BINARY_OR_OCTAL_SPECIFIER = BINARY_SPECIFIER | OCTAL_SPECIFIER,
    NUMERIC_LITERAL_FLAGS = SCIENTIFIC | OCTAL | HEX_SPECIFIER | BINARY_OR_OCTAL_SPECIFIER | CONTAINS_SEPARATOR,
    TEMPLATE_LITERAL_LIKE_FLAGS = CONTAINS_INVALID_ESCAPE,
};

enum class TokenType
{
    null,
    /*
     * WORD-SYMBOLS. See ISO 10206 Page 19
     */
    SYMBOL_AND,
    SYMBOL_AND_THEN,
    SYMBOL_ARRAY,
    SYMBOL_BEGIN,
    SYMBOL_BINDABLE,
    SYMBOL_CASE,
    SYMBOL_CONST,
    SYMBOL_DIV,
    SYMBOL_DO,
    SYMBOL_DOWNTO,
    SYMBOL_ELSE,
    SYMBOL_END,
    SYMBOL_EXPORT,
    SYMBOL_FILE,
    SYMBOL_FOR,
    SYMBOL_FUNCTION,
    SYMBOL_GOTO,
    SYMBOL_IF,
    SYMBOL_IMPORT,
    SYMBOL_IN,
    SYMBOL_LABEL,
    SYMBOL_MOD,
    SYMBOL_MODULE,
    SYMBOL_NIL,
    SYMBOL_NOT,
    SYMBOL_OF,
    SYMBOL_ONLY,
    SYMBOL_OR,
    SYMBOL_OR_ELSE,
    SYMBOL_OTHERWISE,
    SYMBOL_PACKED,
    SYMBOL_POW,
    SYMBOL_PROCEDURE,
    SYMBOL_PROGRAM,
    SYMBOL_PROTECTED,
    SYMBOL_QUALIFIED,
    SYMBOL_RECORD,
    SYMBOL_REPEAT,
    SYMBOL_RESTRICTED,
    SYMBOL_SET,
    SYMBOL_THEN,
    SYMBOL_TO,
    SYMBOL_TYPE,
    SYMBOL_UNTIL,
    SYMBOL_VALUE,
    SYMBOL_VAR,
    SYMBOL_WHILE,
    SYMBOL_WITH,
    SYMBOL_EXTERNAL,

    /*
     * SPECIAL-SYMBOLS. See ISO 10206 Page 19
     */
    SYMBOL_PLUS,
    SYMBOL_MINUS,
    SYMBOL_STAR,
    SYMBOL_STAR_STAR,
    SYMBOL_SLASH,
    SYMBOL_EQUAL,
    SYMBOL_LESS_THAN,
    SYMBOL_GREATER_THAN,
    SYMBOL_SQUARE_BRACKET_OPEN,
    SYMBOL_SQUARE_BRACKET_CLOSE,
    SYMBOL_DOT,
    SYMBOL_COMMA,
    SYMBOL_SEMICOLON,
    SYMBOL_COLON,
    SYMBOL_PAREN_OPEN,
    SYMBOL_PAREN_CLOSE,
    SYMBOL_BRACE_OPEN,
    SYMBOL_BRACE_CLOSE,
    SYMBOL_POWER,
    SYMBOL_STRING_KEYWORD,

    /*
     * One or more characters SPECIAL-SYMBOLS
     */
    SYMBOL_LESS_GREATER_THAN,
    SYMBOL_LESS_EQUAL_THAN,
    SYMBOL_GREATER_EQUAL_THAN,
    SYMBOL_COLON_EQUAL,
    SYMBOL_DOT_DOT,
    SYMBOL_GREATER_LESS_THAN,
    SYMBOL_EQUAL_GREATER_THAN,
    SYMBOL_WHITE_SPACE,
    SYMBOL_LETTER,
    SYMBOL_UNDERSCORE,
    /*
     *
     */
    IDENTIFIER,
    STRING_LITERAL,
    NUMERIC_LITERAL,
    BIG_INTEGER,
    NEW_LINE,
    COMMENT,
    END_OF_FILE,
    UNKNOWN,
};

class Location
{
  public:
    Location(const int lineNumber, const int charNumber, std::string file)
        : lineNo(lineNumber), charNo(charNumber), fileName(file){};
    Location() : lineNo(0), charNo(0){};
    Location(const Location &loc) : charNo(loc.charNo), fileName(loc.fileName), lineNo(loc.lineNo){};
    Location(Location &loc) : charNo(loc.charNo), fileName(loc.fileName), lineNo(loc.lineNo){};
    Location &operator=(const Location &loc)
    {
        return *this;
    }
    const int getLineNo() const
    {
        return lineNo;
    }
    const int getCharNo() const
    {
        return charNo;
    }
    const std::string &getFileName() const
    {
        return fileName;
    }
    void setFileName(const std::string &fileName)
    {
        Location::fileName = fileName;
    }

  private:
    const int lineNo;
    const int charNo;
    std::string fileName;
};

class Token
{
  public:
    Token(TokenType tkType, std::string value, Location pos) : tokenType(tkType), tokenValue(value), tokenPos(pos){};
    TokenType getTokenType()
    {
        return this->tokenType;
    }
    std::string getValue()
    {
        return this->tokenValue;
    }
    Location getTokenPos()
    {
        return this->getTokenPos();
    }

  private:
    TokenType tokenType;
    std::string tokenValue;
    Location tokenPos;
};

class Scanner
{
  public:
    /*
     * CONSTRUCTORS
     */
    Scanner()
    {
    }
    Scanner(std::string &source_, int currentPos_ = 0) : source(source_), currentPos(currentPos_)
    {
        end = source.size();
    };
    /*
     * METHODS
     */
    TokenType scan();
    std::string getTextSource();
    void setTextSource(std::string source);
    void setSkipTrivia(bool skipTrivia);
    static std::string tokenTypeToStr(TokenType token);
    Token getCurrentTokenObject();
    void setSkipSpaces(bool skipSpaces);
    void setSkipComments(bool skipComments);

  private:
    std::string source{0};
    int currentPos = 0;
    int end;
    bool skipTrivia;
    std::string tokenValue;
    int tokenFlags = static_cast<int>(TokenFlags::NONE);
    TokenType curToken;
    int tokenPos;
    int curLine = 1;
    std::string filename;
    bool skipComments;
    bool skipSpaces;
    /*
     * UTILS
     */
    bool isWhiteSpaceSingleLine(const int ch);
    bool isDigit(const int ch);
    std::pair<TokenType, std::string> scanNumber();
    std::string scanNumberFragment();
    bool isLineBreak(const int ch);
    int peekExtendedUnicodeEscape();
    bool isIdentifierStart(char ch);
    bool isIdentifierPart(char ch);
    std::string utf16EncodeAsString(char ch);
    std::string scanExtendedUnicodeEscape();
    std::string scanIdentifierParts();
    TokenType scanIdentifier(char startCharacter);
    std::string scanEscapeSequence(bool isTaggedTemplate);
    std::string scanString();
    std::string scanHexadecimalEscape(const int numberOfDigits);
    int scanExactNumberOfHexDigits(const int numberOfDigits, bool canHaveSeparators);
    bool isCodePoint(const char ch);
    bool isHexDigit(const char ch);
    TokenType getIdentifierToken();
    TokenType checkBigIntSuffix();
    std::string scanMinimumNumberOfHexDigits(int count, bool canHaveSeparators);
    std::string scanBinaryOrOctalDigits(int base);
    std::string scanHexDigits(const int minCount, bool scanAsManyAsPossible, bool canHaveSeparators);
    std::string parsePseudoBigInt(std::string number);
    int parseInt(std::string number, const int radix = 10);
    int peekUnicodeEscape();
    std::map<std::string, TokenType> keywordsToTokenMap{
        {"AND", TokenType::SYMBOL_AND},
        {"AND_THEN", TokenType::SYMBOL_AND_THEN},
        {"ARRAY", TokenType::SYMBOL_ARRAY},
        {"BEGIN", TokenType::SYMBOL_BEGIN},
        {"BINDABLE", TokenType::SYMBOL_BINDABLE},
        {"CASE", TokenType::SYMBOL_CASE},
        {"CONST", TokenType::SYMBOL_CONST},
        {"DIV", TokenType::SYMBOL_DIV},
        {"DO", TokenType::SYMBOL_DO},
        {"DOWNTO", TokenType::SYMBOL_DOWNTO},
        {"ELSE", TokenType::SYMBOL_ELSE},
        {"END", TokenType::SYMBOL_END},
        {"EXPORT", TokenType::SYMBOL_EXPORT},
        {"FILE", TokenType::SYMBOL_FILE},
        {"FOR", TokenType::SYMBOL_FOR},
        {"FUNCTION", TokenType::SYMBOL_FUNCTION},
        {"GOTO", TokenType::SYMBOL_GOTO},
        {"IF", TokenType::SYMBOL_IF},
        {"IMPORT", TokenType::SYMBOL_IMPORT},
        {"IN", TokenType::SYMBOL_IN},
        {"LABEL", TokenType::SYMBOL_LABEL},
        {"MOD", TokenType::SYMBOL_MOD},
        {"MODULE", TokenType::SYMBOL_MODULE},
        {"NIL", TokenType::SYMBOL_NIL},
        {"NOT", TokenType::SYMBOL_NOT},
        {"OF", TokenType::SYMBOL_OF},
        {"ONLY", TokenType::SYMBOL_ONLY},
        {"OR", TokenType::SYMBOL_OR},
        {"OR_ELSE", TokenType::SYMBOL_OR_ELSE},
        {"OTHERWISE", TokenType::SYMBOL_OTHERWISE},
        {"PACKED", TokenType::SYMBOL_PACKED},
        {"POW", TokenType::SYMBOL_POW},
        {"PROCEDURE", TokenType::SYMBOL_PROCEDURE},
        {"PROGRAM", TokenType::SYMBOL_PROGRAM},
        {"PROTECTED", TokenType::SYMBOL_PROTECTED},
        {"QUALIFIED", TokenType::SYMBOL_QUALIFIED},
        {"RECORD", TokenType::SYMBOL_RECORD},
        {"REPEAT", TokenType::SYMBOL_REPEAT},
        {"RESTRICTED", TokenType::SYMBOL_RESTRICTED},
        {"SET", TokenType::SYMBOL_SET},
        {"THEN", TokenType::SYMBOL_THEN},
        {"TO", TokenType::SYMBOL_TO},
        {"TYPE", TokenType::SYMBOL_TYPE},
        {"UNTIL", TokenType::SYMBOL_UNTIL},
        {"VALUE", TokenType::SYMBOL_VALUE},
        {"VAR", TokenType::SYMBOL_VAR},
        {"WHILE", TokenType::SYMBOL_WHILE},
        {"WITH", TokenType::SYMBOL_WITH},
        {"EXTERNAL", TokenType::SYMBOL_EXTERNAL},
        {"STRING", TokenType::SYMBOL_STRING_KEYWORD}
    };
};
} // namespace Pascal
#endif // JAPC_SCANNER_H
