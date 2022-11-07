//
// Created by wuser on 18/07/22.
//

#include "japc/scanner/scanner.h"
#include "japc/scanner/characters.h"
#include "string_utils.h"
#include <codecvt>
#include <list>
#include <locale>

using namespace Pascal;

std::string Scanner::getTextSource()
{
    return Scanner::source;
}
void Scanner::setTextSource(std::string source)
{
    this->source = source;
    end = source.size();
    currentPos = 0;
}
void Scanner::setSkipTrivia(bool skipTrivia)
{
    this->skipTrivia = skipTrivia;
}
TokenType Scanner::scan()
{
    std::pair<TokenType, std::string> res;

    int startPos = currentPos;
    bool commentClosed;
    int lastLineStart;
    char extendedCookedChar, cookedChar;
    tokenPos = -1;
    while (true)
    {
        tokenPos = startPos;
        if (currentPos >= end)
        {
            curToken = TokenType::END_OF_FILE;
            return curToken;
        }
        const wchar_t currentChar = string_utils::getCharAt(startPos, source);
        switch (currentChar)
        {
        case LINEFEED:
        case CARRIAGE_RETURN:
            tokenFlags |= static_cast<int>(TokenFlags::PRECEDING_LINE_BREAK);
            if (skipTrivia)
            {
                currentPos++;
                curLine++;
                startPos = currentPos;
                continue;
            }
            else
            {
                if (currentChar == CARRIAGE_RETURN && currentPos + 1 < end &&
                    string_utils::getCharAt(currentPos + 1, source) == LINEFEED)
                    currentPos += 2;
                else
                {
                    currentPos++;
                    curLine++;
                }
                curToken = TokenType::NEW_LINE;
                return curToken;
            }
        case TAB:
        case VERTICAL_TAB:
        case FORM_FEED:
        case SPACE:
        case NON_BREAKING_SPACE:
        case OGHAM:
        case EN_QUAD:
        case EM_QUAD:
        case EN_SPACE:
        case EM_SPACE:
        case THREE_PER_EM_SPACE:
        case FOUR_PER_EM_SPACE:
        case SIX_PER_EM_SPACE:
        case FIGURE_SPACE:
        case PUNCTUATION_SPACE:
        case THIN_SPACE:
        case HAIR_SPACE:
        case ZERO_WIDTH_SPACE:
        case NARROW_NO_BREAK_SPACE:
        case MATHEMATICAL_SPACE:
        case IDEOGRAPHIC_SPACE:
        case BYTE_ORDER_MARK:
            if (skipTrivia)
            {
                currentPos++;
                startPos = currentPos;
                continue;
            }
            else
            {
                while (isWhiteSpaceSingleLine(string_utils::getCharAt(currentPos, source)))
                    currentPos++;
                curToken = TokenType::SYMBOL_WHITE_SPACE;
                return curToken;
            }
        case OPEN_PAREN:
            if (string_utils::getCharAt(currentPos + 1, source) == ASTERISK)
            {
                currentPos += 2;
                commentClosed = false;
                lastLineStart = currentPos;
                while (currentPos < end)
                {
                    const char ch = string_utils::getCharAt(currentPos, source);
                    if (ch == ASTERISK && string_utils::getCharAt(currentPos + 1, source) == CLOSE_BRACKET)
                    {
                        currentPos += 2;
                        commentClosed = true;
                        break;
                    }
                    currentPos++;
                    if (isLineBreak(ch))
                    {
                        lastLineStart = currentPos;
                        curLine++;
                        tokenFlags |= static_cast<int>(TokenFlags::PRECEDING_LINE_BREAK);
                    }
                }
                if (!commentClosed)
                {
                }
                if (skipTrivia)
                {
                    startPos = currentPos;
                    continue;
                }
                else
                {
                    if (!commentClosed)
                    {
                        tokenFlags |= static_cast<int>(TokenFlags::UNTERMINATED);
                    }
                    curToken = TokenType::COMMENT;
                    return curToken;
                }
            }
            else
            {
                currentPos++;
                curToken = TokenType::SYMBOL_PAREN_OPEN;
                return curToken;
            }
        case CLOSE_PAREN:
            currentPos++;
            curToken = TokenType::SYMBOL_PAREN_CLOSE;
            return curToken;
        case OPEN_BRACKET:
            currentPos++;
            curToken = TokenType::SYMBOL_SQUARE_BRACKET_OPEN;
            return curToken;
        case CLOSE_BRACKET:
            currentPos++;
            curToken = TokenType::SYMBOL_SQUARE_BRACKET_CLOSE;
            return curToken;
        case SLASH:
            currentPos++;
            curToken = TokenType::SYMBOL_SLASH;
            return curToken;
        case BACKSLASH:
            extendedCookedChar = peekExtendedUnicodeEscape();
            if (extendedCookedChar >= 0 && isIdentifierStart(extendedCookedChar))
            {
                currentPos += 3;
                tokenFlags |= static_cast<int>(TokenFlags::EXTENDED_UNICODE_ESCAPE);
                tokenValue = scanExtendedUnicodeEscape() + scanIdentifierParts();
                curToken = getIdentifierToken();
                return curToken;
            }
            cookedChar = peekUnicodeEscape();
            if (cookedChar >= 0 && isIdentifierStart(extendedCookedChar))
            {
                currentPos += 6;
                tokenFlags |= static_cast<int>(TokenFlags::UNICODE_ESCAPE);
                tokenValue = string_utils::fromCharCode(cookedChar) + scanIdentifierParts();
                curToken = getIdentifierToken();
                return curToken;
            }
            // TODO: ERROR
            currentPos++;
            curToken = TokenType::UNKNOWN;
            return curToken;
        case ASTERISK:
            if (string_utils::getCharAt(currentPos + 1, source) == ASTERISK)
            {
                currentPos += 2;
                curToken = TokenType::SYMBOL_STAR_STAR;
                return curToken;
            }
            else
            {
                currentPos++;
                curToken = TokenType::SYMBOL_STAR;
                return curToken;
            }
        case PLUS:
            currentPos++;
            curToken = TokenType::SYMBOL_PLUS;
            return curToken;
        case MINUS:
            currentPos++;
            curToken = TokenType::SYMBOL_MINUS;
            return curToken;
        case EQUALS:
            if (string_utils::getCharAt(currentPos + 1, source) == GREATER_THAN)
            {
                currentPos += 2;
                curToken = TokenType::SYMBOL_EQUAL_GREATER_THAN;
                return curToken;
            }
            else
            {
                currentPos++;
                curToken = TokenType::SYMBOL_EQUAL;
                return curToken;
            }
        case COLON:
            if (string_utils::getCharAt(currentPos + 1, source) == EQUALS)
            {
                currentPos += 2;
                curToken = TokenType::SYMBOL_COLON_EQUAL;
                return curToken;
            }
            else
            {
                currentPos++;
                curToken = TokenType::SYMBOL_COLON;
                return curToken;
            }
        case SEMICOLON:
            currentPos++;
            curToken = TokenType::SYMBOL_SEMICOLON;
            return curToken;
        case COMMA:
            currentPos++;
            curToken = TokenType::SYMBOL_COMMA;
            return curToken;
        case DOUBLE_QUOTE:
        case SINGLE_QUOTE:
            tokenValue = scanString();
            curToken = TokenType::STRING_LITERAL;
            return curToken;
        case OPEN_BRACE:
            currentPos++;
            commentClosed = false;
            lastLineStart = currentPos;
            if (string_utils::getCharAt(currentPos, source) != ASTERISK)
            {
                curToken = TokenType::SYMBOL_BRACE_OPEN;
                return curToken;
            }
            currentPos++;
            while (currentPos < end)
            {
                const char ch = string_utils::getCharAt(currentPos, source);
                if (ch == ASTERISK && string_utils::getCharAt(currentPos + 1, source) == CLOSE_BRACE)
                {
                    currentPos += 2;
                    commentClosed = true;
                    break;
                }
                currentPos++;
                if (isLineBreak(ch))
                {
                    lastLineStart = currentPos;
                    curLine++;
                    tokenFlags |= static_cast<int>(TokenFlags::PRECEDING_LINE_BREAK);
                }
            }
            if (!commentClosed)
            {
                continue;
            }
            if (skipTrivia)
            {
                startPos = currentPos;
                continue;
            }
            else
            {
                if (!commentClosed)
                {
                    tokenFlags |= static_cast<int>(TokenFlags::UNTERMINATED);
                }
                curToken = TokenType::COMMENT;
                return curToken;
            }
            curToken = TokenType::SYMBOL_BRACE_OPEN;
            return curToken;
        case CLOSE_BRACE:
            currentPos++;
            curToken = TokenType::SYMBOL_BRACE_CLOSE;
            return curToken;
        case DOT:
            if (string_utils::getCharAt(currentPos + 1, source) == DOT)
            {
                currentPos += 2;
                curToken = TokenType::SYMBOL_DOT_DOT;
                return curToken;
            }
            else
            {
                currentPos++;
                curToken = TokenType::SYMBOL_DOT;
                return curToken;
            }
        case LESS_THAN:
            if (string_utils::getCharAt(currentPos + 1, source) == EQUALS)
            {
                currentPos += 2;
                curToken = TokenType::SYMBOL_LESS_EQUAL_THAN;
                return curToken;
            }
            else if (string_utils::getCharAt(currentPos + 1, source) == GREATER_THAN)
            {
                currentPos += 2;
                curToken = TokenType::SYMBOL_LESS_GREATER_THAN;
                return curToken;
            }
            else
            {
                currentPos++;
                curToken = TokenType::SYMBOL_LESS_THAN;
                return curToken;
            }
        case GREATER_THAN:
            if (string_utils::getCharAt(currentPos + 1, source) == EQUALS)
            {
                currentPos += 2;
                curToken = TokenType::SYMBOL_GREATER_EQUAL_THAN;
                return curToken;
            }
            else if (string_utils::getCharAt(currentPos + 1, source) == LESS_THAN)
            {
                currentPos += 2;
                curToken = TokenType::SYMBOL_GREATER_LESS_THAN;
                return curToken;
            }
            else
            {
                currentPos++;
                curToken = TokenType::SYMBOL_GREATER_THAN;
                return curToken;
            }
        case LITERAL_0:
            if (string_utils::getCharAt(currentPos + 1, source) == LITERAL_X ||
                string_utils::getCharAt(currentPos + 1, source) == LITERAL_x)
            {
                currentPos += 2;
                tokenValue = scanMinimumNumberOfHexDigits(1, true);
                if (tokenValue.empty())
                {
                    tokenValue = "0";
                }
                tokenValue = "0x" + tokenValue;
                tokenFlags |= static_cast<int>(TokenFlags::HEX_SPECIFIER);
                curToken = checkBigIntSuffix();
                return curToken;
            }
            else if (string_utils::getCharAt(currentPos + 1, source) == LITERAL_B ||
                     string_utils::getCharAt(currentPos + 1, source) == LITERAL_b)
            {
                currentPos += 2;
                tokenValue = scanBinaryOrOctalDigits(2);
                if (tokenValue.empty())
                {
                    tokenValue = "0";
                }
                tokenValue = "0b" + tokenValue;
                tokenFlags |= static_cast<int>(TokenFlags::BINARY_SPECIFIER);
                curToken = checkBigIntSuffix();
                return curToken;
            }
            else if (string_utils::getCharAt(currentPos + 1, source) == LITERAL_O ||
                     string_utils::getCharAt(currentPos + 1, source) == LITERAL_o)
            {
                currentPos += 2;
                tokenValue = scanBinaryOrOctalDigits(8);
                if (tokenValue.empty())
                {
                    tokenValue = "0";
                }
                tokenValue = "0o" + tokenValue;
                tokenFlags |= static_cast<int>(TokenFlags::OCTAL_SPECIFIER);
                curToken = checkBigIntSuffix();
                return curToken;
            }
        case LITERAL_1:
        case LITERAL_2:
        case LITERAL_3:
        case LITERAL_4:
        case LITERAL_5:
        case LITERAL_6:
        case LITERAL_7:
        case LITERAL_8:
        case LITERAL_9:
            res = scanNumber();
            tokenValue = res.second;
            curToken = res.first;
            return curToken;
        default:
            TokenType identifierKind = scanIdentifier(currentChar);
            if (identifierKind != TokenType::null)
            {
                curToken = identifierKind;
                return curToken;
            }
            else if (isWhiteSpaceSingleLine(currentChar))
            {
                tokenFlags = static_cast<int>(TokenFlags::PRECEDING_LINE_BREAK);
                currentPos++;
                continue;
            }
            // TODO: INVALID CHARACTER ERROR
            currentPos++;
            curToken = TokenType::UNKNOWN;
            return curToken;
        }
    }
}
bool Scanner::isWhiteSpaceSingleLine(const int ch)
{
    return (ch == SPACE || ch == TAB || ch == VERTICAL_TAB || ch == FORM_FEED || ch == NON_BREAKING_SPACE ||
            ch == NEXT_LINE || ch == OGHAM || ch >= EN_QUAD && ch <= ZERO_WIDTH_SPACE || ch == NARROW_NO_BREAK_SPACE ||
            ch == MATHEMATICAL_SPACE || ch == IDEOGRAPHIC_SPACE || ch == BYTE_ORDER_MARK);
}
bool Scanner::isDigit(const int ch)
{
    return ch >= LITERAL_0 && ch <= LITERAL_9;
}
bool Scanner::isLineBreak(const int ch)
{
    return ch == LINEFEED || ch == CARRIAGE_RETURN || ch == LINE_SEPARATOR || ch == PARAGRAPH_SEPARATOR;
}
std::pair<TokenType, std::string> Scanner::scanNumber()
{
    std::pair<TokenType, std::string> result;
    int const start = currentPos;
    const std::string mainFragment = scanNumberFragment();
    std::string decimalFragment = "";
    std::string scientificFragment = "";
    int end = currentPos;
    if (string_utils::getCharAt(currentPos, source) == LITERAL_E ||
        string_utils::getCharAt(currentPos, source) == LITERAL_e)
    {
        currentPos++;
        tokenFlags |= static_cast<int>(TokenFlags::SCIENTIFIC);
        if (string_utils::getCharAt(currentPos, source) == PLUS || string_utils::getCharAt(currentPos, source) == MINUS)
            currentPos++;
        int preNumericPart = currentPos;
        const std::string finalFragment = scanNumberFragment();
        if (finalFragment.empty())
        {
            // TODO: ERROR DIGIT EXPECTED
        }
        else
        {
            scientificFragment = source.substr(end, preNumericPart - end) + finalFragment;
            end = currentPos;
        }
    }
    std::string res = "";
    if (tokenFlags & static_cast<int>(TokenFlags::CONTAINS_SEPARATOR))
    {
        res = mainFragment;
        if (!decimalFragment.empty())
        {
            res += "." + decimalFragment;
        }
        if (!scientificFragment.empty())
        {
            res += scientificFragment;
        }
    }
    else
    {
        res = source.substr(start, end - start);
    }
    if (!decimalFragment.empty() || tokenFlags & static_cast<int>(TokenFlags::SCIENTIFIC))
    {
    }
}
std::string Scanner::scanString()
{
    const char quote = string_utils::getCharAt(currentPos, source);
    currentPos++;
    std::string result;
    int start = currentPos;
    while (true)
    {
        if (currentPos >= end)
        {
            result += source.substr(start, currentPos - start);
            tokenFlags |= static_cast<int>(TokenFlags::UNTERMINATED);
            // TODO: Error unterminated string literal
            break;
        }
        const char ch = string_utils::getCharAt(currentPos, source);
        if (ch == quote)
        {
            result += source.substr(start, currentPos - start);
            currentPos++;
            break;
        }
        if (ch == BACKSLASH)
        {
            result += source.substr(start, currentPos - start);
            result += scanEscapeSequence(false);
            start = currentPos;
            continue;
        }
        if (isLineBreak(ch))
        {
            result += source.substr(start, currentPos - start);
            tokenFlags |= static_cast<int>(TokenFlags::UNTERMINATED);
            // TODO: Error unterminated string literal
            break;
        }
        currentPos++;
    }
    return result;
}
/*
 * Current character is backslash. Check for unicode escape of the form \uXXXX
 * and return code point value. Otherwise return -1
 */
int Scanner::peekUnicodeEscape()
{
    if (currentPos + 5 < end && string_utils::getCharAt(currentPos + 1, source) == LITERAL_u)
    {
        const int start = currentPos;
        currentPos += 2; // eat backslash and 'u' \u
        const int value = scanExactNumberOfHexDigits(4, false);
        currentPos = start;
        return value;
    }
    return -1;
}
bool Scanner::isIdentifierStart(char ch)
{
    return ch >= LITERAL_A && ch <= LITERAL_Z || ch >= LITERAL_a && ch <= LITERAL_z;
}
int Scanner::peekExtendedUnicodeEscape()
{
    if (string_utils::codePointAt(source, currentPos + 1) == LITERAL_u &&
        string_utils::codePointAt(source, currentPos + 2) == OPEN_BRACE)
    {
        const int start = currentPos;
        currentPos += 3;
        const std::string escapedValueString = scanMinimumNumberOfHexDigits(2, false);
        const int escapedValue = !escapedValueString.empty() ? std::stoul(escapedValueString, nullptr, 16) : -1;
        currentPos = start;
        return escapedValue;
    }
    return -1;
}
std::string Scanner::scanExtendedUnicodeEscape()
{
    const std::string escapedValueString = scanMinimumNumberOfHexDigits(1, false);
    const int escapedValue = !escapedValueString.empty() ? std::stoul(escapedValueString, nullptr, 16) : -1;
    bool isInvalidExtendedEscape = false;
    if (escapedValue < 0)
    {
        // TODO: error hexadecimal digit expected
        isInvalidExtendedEscape = true;
    }
    else if (escapedValue > 0x10FFFF)
    {
        // TODO: error extended unicode escape value must be between 0x0 and 0x10FFFF inclusive
        isInvalidExtendedEscape = true;
    }
    if (currentPos >= end)
    {
        // TODO: error unexpected end of text
        isInvalidExtendedEscape = true;
    }
    else if (string_utils::getCharAt(currentPos, source) == CLOSE_BRACE)
    {
        currentPos++;
    }
    else
    {
        // TODO: error unterminated unicode escape sequence
        isInvalidExtendedEscape = true;
    }
    if (isInvalidExtendedEscape)
    {
        return "";
    }
    return utf16EncodeAsString(escapedValue);
}
std::string Scanner::scanIdentifierParts()
{
    std::string result = "";
    int start = currentPos;
    while (currentPos < end)
    {
        char ch = string_utils::getCharAt(currentPos, source);
        if (isIdentifierPart(ch))
        {
            currentPos += 1;
        }
        else if (ch == BACKSLASH)
        {
            ch = peekExtendedUnicodeEscape();
            if (ch >= 0 && isIdentifierPart(ch))
            {
                currentPos += 3;
                tokenFlags |= static_cast<int>(TokenFlags::EXTENDED_UNICODE_ESCAPE);
                result += scanExtendedUnicodeEscape();
                start = currentPos;
                continue;
            }
            ch = peekUnicodeEscape();
            if (!(ch >= 0 && isIdentifierPart(ch)))
            {
                break;
            }
            tokenFlags |= static_cast<int>(TokenFlags::UNICODE_ESCAPE);
            result += source.substr(start, currentPos - start);
            result += utf16EncodeAsString(ch);
            currentPos += 6;
            start = currentPos;
        }
        else
        {
            break;
        }
    }
    result += source.substr(start, currentPos - start);
    return result;
}
std::string Scanner::scanBinaryOrOctalDigits(int base)
{
    std::string value = "";
    bool separatorAllowed = false;
    bool isPreviousTokenSeparator = false;
    while (true)
    {
        char ch = string_utils::getCharAt(currentPos, source);
        if (ch == LITERAL__)
        {
            tokenFlags |= static_cast<int>(TokenFlags::CONTAINS_SEPARATOR);
            if (separatorAllowed)
            {
                separatorAllowed = false;
                isPreviousTokenSeparator = true;
            }
            else if (isPreviousTokenSeparator)
            {
                // TODO: multiple consecutive numeric separators are not permitted
            }
            else
            {
                // TODO: numeric separators are not allowed here
            }
            currentPos++;
            continue;
        }
        separatorAllowed = true;
        if (!isDigit(ch) || ch - LITERAL_0 >= base)
        {
            break;
        }
        value += string_utils::getCharAt(currentPos, source);
        currentPos++;
        isPreviousTokenSeparator = false;
    }
    if (string_utils::getCharAt(currentPos - 1, source) == LITERAL__)
    {
        // TODO: numeric separators are not allowed here
    }
    return value;
}
TokenType Scanner::getIdentifierToken()
{
    tokenValue = string_utils::toUpperCase(tokenValue);
    if (keywordsToTokenMap.count(tokenValue))
    {
        return keywordsToTokenMap[tokenValue];
    }
    return TokenType::IDENTIFIER;
}
TokenType Scanner::checkBigIntSuffix()
{
    if (string_utils::getCharAt(currentPos, source) == LITERAL_n)
    {
        tokenValue += "n";
        if (tokenFlags & static_cast<int>(TokenFlags::BINARY_OR_OCTAL_SPECIFIER))
        {
            tokenValue = parsePseudoBigInt(tokenValue) + "n";
        }
        currentPos++;
        return TokenType::BIG_INTEGER;
    }
    else
    {
        const int numericValue = tokenFlags & static_cast<int>(TokenFlags::BINARY_SPECIFIER)
                                     ? parseInt(tokenValue.substr(2, tokenValue.size()))
                                 : tokenFlags & static_cast<int>(TokenFlags::OCTAL_SPECIFIER)
                                     ? parseInt(tokenValue.substr(2, 8))
                                     : std::stoul(tokenValue, nullptr, 10);
        tokenValue = "" + numericValue;
        return TokenType::NUMERIC_LITERAL;
    }
}
std::string Scanner::scanMinimumNumberOfHexDigits(int count, bool canHaveSeparators)
{
    return scanHexDigits(count, true, canHaveSeparators);
}
std::string Scanner::utf16EncodeAsString(char ch)
{
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> convert;
    std::string converted = convert.to_bytes(ch);
    return converted;
}
std::string Scanner::parsePseudoBigInt(std::string number)
{
}
int Scanner::parseInt(std::string number, const int radix)
{
    return std::stoul(number, nullptr, radix);
}
std::string Scanner::scanNumberFragment()
{
    int start = currentPos;
    bool allowSeparator = false;
    bool isPreviousTokenSeparator = false;
    std::string result;
    while (true)
    {
        char ch = string_utils::getCharAt(currentPos, source);
        if (ch == LITERAL__)
        {
            tokenFlags |= static_cast<int>(TokenFlags::CONTAINS_SEPARATOR);
            if (allowSeparator)
            {
                allowSeparator = false;
                isPreviousTokenSeparator = true;
                result += source.substr(start, currentPos - start);
            }
            else if (isPreviousTokenSeparator)
            {
                // TODO: Error Multiple consecutive numeric separators
            }
            else
            {
                // TODO: MNumeric separators are not allowed here
            }
            currentPos++;
            start = currentPos;
            continue;
        }
        if (isDigit(ch))
        {
            allowSeparator = true;
            isPreviousTokenSeparator = false;
            currentPos++;
            continue;
        }
        break;
    }
    if (string_utils::getCharAt(currentPos - 1, source) == LITERAL__)
    {
        // TODO: Numeric separators are not allowed here
    }
    return result + source.substr(start, currentPos - start);
}
bool Scanner::isIdentifierPart(char ch)
{
    return isIdentifierStart(ch) || ch >= LITERAL_0 && ch <= LITERAL_9 || ch == LITERAL__;
}
TokenType Scanner::scanIdentifier(char startCharacter)
{
    char ch = startCharacter;
    if (isIdentifierPart(ch))
    {
        currentPos += 1;
        while (currentPos < end && isIdentifierPart(string_utils::getCharAt(currentPos, source)))
            currentPos++;
        tokenValue = source.substr(tokenPos, currentPos - tokenPos);
        if (ch == BACKSLASH)
        {
            tokenValue += scanIdentifierParts();
        }
        return getIdentifierToken();
    }
    return TokenType::null;
}
std::string Scanner::scanEscapeSequence(bool isTaggedTemplate)
{
    const int start = currentPos;
    currentPos++;
    if (currentPos >= end)
    {
        // TODO: Error unexpected end of text
        return "";
    }
    const wchar_t ch = string_utils::getCharAt(currentPos, source);
    currentPos++;
    switch (ch)
    {
    case LITERAL_0:
        if (isTaggedTemplate && currentPos < end && isDigit(string_utils::getCharAt(currentPos, source)))
        {
            currentPos++;
            tokenFlags |= static_cast<int>(TokenFlags::CONTAINS_INVALID_ESCAPE);
            return source.substr(start, currentPos - start);
        }
        return "\0";
    case LITERAL_b:
        return "\b";
    case LITERAL_t:
        return "\t";
    case LITERAL_n:
        return "\n";
    case LITERAL_v:
        return "\v";
    case LITERAL_f:
        return "\f";
    case LITERAL_r:
        return "\r";
    case SINGLE_QUOTE:
        return "\'";
    case DOUBLE_QUOTE:
        return "\"";
    case LITERAL_u:
        if (isTaggedTemplate)
        {
            // \u or \u0 or \u00 or \u000
            for (int escapePos = currentPos; escapePos < currentPos + 4; escapePos++)
            {
                if (escapePos < end && !isHexDigit(string_utils::getCharAt(escapePos, source)) &&
                    string_utils::getCharAt(escapePos, source) != OPEN_BRACE)
                {
                    currentPos = escapePos;
                    tokenFlags |= static_cast<int>(TokenFlags::CONTAINS_INVALID_ESCAPE);
                    return source.substr(start, currentPos - start);
                }
            }
        }
        // \u{DDDDDDDD}
        if (currentPos < end && string_utils::getCharAt(currentPos, source) == OPEN_BRACE)
        {
            currentPos++;

            if (isTaggedTemplate && !isHexDigit(string_utils::getCharAt(currentPos, source)))
            {
                tokenFlags |= static_cast<int>(TokenFlags::CONTAINS_INVALID_ESCAPE);
                return source.substr(start, currentPos - start);
            }
            if (isTaggedTemplate)
            {
                const int savePos = currentPos;
                const std::string escapeValueString = scanMinimumNumberOfHexDigits(1, false);
                int escapedValue;
                if (!escapeValueString.empty())
                    escapedValue = std::stoul(escapeValueString, nullptr, 16);
                if (!isCodePoint(escapedValue) || string_utils::getCharAt(currentPos, source) != CLOSE_BRACE)
                {
                    tokenFlags |= static_cast<int>(TokenFlags::CONTAINS_INVALID_ESCAPE);
                    return source.substr(start, currentPos - start);
                }
                else
                {
                    currentPos = savePos;
                }
            }
            tokenFlags |= static_cast<int>(TokenFlags::EXTENDED_UNICODE_ESCAPE);
            return scanExtendedUnicodeEscape();
        }
        tokenFlags |= static_cast<int>(TokenFlags::UNICODE_ESCAPE);
        // \uDDDD
        return scanHexadecimalEscape(4);
    case LITERAL_x:
        if (isTaggedTemplate)
        {
            if (!isHexDigit(string_utils::getCharAt(currentPos, source)))
            {
                tokenFlags |= static_cast<int>(TokenFlags::CONTAINS_INVALID_ESCAPE);
                return source.substr(start, currentPos - start);
            }
            else if (!isHexDigit(string_utils::getCharAt(currentPos + 1, source)))
            {
                currentPos++;
                tokenFlags |= static_cast<int>(TokenFlags::CONTAINS_INVALID_ESCAPE);
                return source.substr(start, currentPos - start);
            }
            return scanHexadecimalEscape(2);
        }
    case CARRIAGE_RETURN:
        if (currentPos < end && string_utils::getCharAt(currentPos, source) == LINEFEED)
            currentPos++;
    case LINEFEED:
    case LINE_SEPARATOR:
    case PARAGRAPH_SEPARATOR:
        return "";
    default:
        return string_utils::fromCharCode(ch);
    }
}
std::string Scanner::scanHexadecimalEscape(const int numberOfDigits)
{
    const int escapedValue = scanExactNumberOfHexDigits(numberOfDigits, false);
    if (escapedValue >= 0)
    {
        return string_utils::fromCharCode(escapedValue);
    }
    else
    {
        // TODO: Error hexadecimal digit expected
        return "";
    }
}
int Scanner::scanExactNumberOfHexDigits(const int numberOfDigits, bool canHaveSeparators)
{
    std::string valueString = scanHexDigits(numberOfDigits, false, canHaveSeparators);
    return !valueString.empty() ? std::stoul(valueString, nullptr, 16) : -1;
}
std::string Scanner::scanHexDigits(const int minCount, bool scanAsManyAsPossible, bool canHaveSeparators)
{
    std::list<int> valueChars;
    bool allowSeparator = false;
    bool isPreviousTokenSeparator = false;
    while (valueChars.size() < minCount || scanAsManyAsPossible)
    {
        char ch = string_utils::getCharAt(currentPos, source);
        if (canHaveSeparators && ch == LITERAL__)
        {
            tokenFlags |= static_cast<int>(TokenFlags::CONTAINS_SEPARATOR);
            if (allowSeparator)
            {
                allowSeparator = false;
                isPreviousTokenSeparator = true;
            }
            else if (isPreviousTokenSeparator)
            {
                // TODO: Multiple consecutive numeric separators are not permitted error
            }
            else
            {
                // TODO: Numeric separators are not allowed here
            }
            currentPos++;
            continue;
        }
        allowSeparator = canHaveSeparators;
        if (ch >= LITERAL_A && ch <= LITERAL_F)
        {
            ch += LITERAL_a - LITERAL_A; // Make lower char to case
        }
        else if (!((ch >= LITERAL_0 && ch <= LITERAL_9) || (ch >= LITERAL_a && ch <= LITERAL_f)))
        {
            break;
        }
        valueChars.push_back(ch);
        currentPos++;
        isPreviousTokenSeparator = false;
    }
    if (valueChars.size() < minCount)
    {
        valueChars.clear();
    }
    if (string_utils::getCharAt(currentPos - 1, source) == LITERAL__)
    {
        // TODO: numeric separators are nt allowed here
    }
    return string_utils::fromCharCode(valueChars);
}
bool Scanner::isCodePoint(const char ch)
{
    return ch <= 0x10FFFF;
}
bool Scanner::isHexDigit(const char ch)
{
    return isDigit(ch) || ch >= LITERAL_A && ch <= LITERAL_F || ch >= LITERAL_a && ch <= LITERAL_f;
}

std::string Scanner::tokenTypeToStr(TokenType token)
{
    switch (token)
    {

    case TokenType::null:
        return "TokenType::null";
    case TokenType::SYMBOL_AND:
        return "TokenType::SYMBOL_AND";
    case TokenType::SYMBOL_AND_THEN:
        return "TokenType::SYMBOL_AND_THEN";
    case TokenType::SYMBOL_ARRAY:
        return "TokenType::SYMBOL_ARRAY";
    case TokenType::SYMBOL_BEGIN:
        return "TokenType::SYMBOL_BEGIN";
    case TokenType::SYMBOL_BINDABLE:
        return "TokenType::SYMBOL_BINDABLE";
    case TokenType::SYMBOL_CASE:
        return "TokenType::SYMBOL_CASE";
    case TokenType::SYMBOL_CONST:
        return "TokenType::SYMBOL_CONST";
    case TokenType::SYMBOL_DIV:
        return "TokenType::SYMBOL_DIV";
    case TokenType::SYMBOL_DO:
        return "TokenType::SYMBOL_DO";
    case TokenType::SYMBOL_DOWNTO:
        return "TokenType::SYMBOL_DOWNTO";
    case TokenType::SYMBOL_ELSE:
        return "TokenType::SYMBOL_ELSE";
    case TokenType::SYMBOL_END:
        return "TokenType::SYMBOL_END";
    case TokenType::SYMBOL_EXPORT:
        return "TokenType::SYMBOL_EXPORT";
    case TokenType::SYMBOL_FILE:
        return "TokenType::SYMBOL_FILE";
    case TokenType::SYMBOL_FOR:
        return "TokenType::SYMBOL_FOR";
    case TokenType::SYMBOL_FUNCTION:
        return "TokenType::SYMBOL_FUNCTION";
    case TokenType::SYMBOL_GOTO:
        return "TokenType::SYMBOL_GOTO";
    case TokenType::SYMBOL_IF:
        return "TokenType::SYMBOL_IF";
    case TokenType::SYMBOL_IMPORT:
        return "TokenType::SYMBOL_IMPORT";
    case TokenType::SYMBOL_IN:
        return "TokenType::SYMBOL_IN";
    case TokenType::SYMBOL_LABEL:
        return "TokenType::SYMBOL_LABEL";
    case TokenType::SYMBOL_MOD:
        return "TokenType::SYMBOL_MOD";
    case TokenType::SYMBOL_MODULE:
        return "TokenType::SYMBOL_MODULE";
    case TokenType::SYMBOL_NIL:
        return "TokenType::SYMBOL_NIL";
    case TokenType::SYMBOL_NOT:
        return "TokenType::SYMBOL_NOT";
    case TokenType::SYMBOL_OF:
        return "TokenType::SYMBOL_OF";
    case TokenType::SYMBOL_ONLY:
        return "TokenType::SYMBOL_ONLY";
    case TokenType::SYMBOL_OR:
        return "TokenType::SYMBOL_OR";
    case TokenType::SYMBOL_OR_ELSE:
        return "TokenType::SYMBOL_OR_ELSE";
    case TokenType::SYMBOL_OTHERWISE:
        return "TokenType::SYMBOL_OTHERWISE";
    case TokenType::SYMBOL_PACKED:
        return "TokenType::SYMBOL_PACKED";
    case TokenType::SYMBOL_POW:
        return "TokenType::SYMBOL_POW";
    case TokenType::SYMBOL_PROCEDURE:
        return "TokenType::SYMBOL_PROCEDURE";
    case TokenType::SYMBOL_PROGRAM:
        return "TokenType::SYMBOL_PROGRAM";
    case TokenType::SYMBOL_PROTECTED:
        return "TokenType::SYMBOL_PROTECTED";
    case TokenType::SYMBOL_QUALIFIED:
        return "TokenType::SYMBOL_QUALIFIED";
    case TokenType::SYMBOL_RECORD:
        return "TokenType::SYMBOL_RECORD";
    case TokenType::SYMBOL_REPEAT:
        return "TokenType::SYMBOL_REPEAT";
    case TokenType::SYMBOL_RESTRICTED:
        return "TokenType::SYMBOL_RESTRICTED";
    case TokenType::SYMBOL_SET:
        return "TokenType::SYMBOL_SET";
    case TokenType::SYMBOL_THEN:
        return "TokenType::SYMBOL_THEN";
    case TokenType::SYMBOL_TO:
        return "TokenType::SYMBOL_TO";
    case TokenType::SYMBOL_TYPE:
        return "TokenType::SYMBOL_TYPE";
    case TokenType::SYMBOL_UNTIL:
        return "TokenType::SYMBOL_UNTIL";
    case TokenType::SYMBOL_VALUE:
        return "TokenType::SYMBOL_VALUE";
    case TokenType::SYMBOL_VAR:
        return "TokenType::SYMBOL_VAR";
    case TokenType::SYMBOL_WHILE:
        return "TokenType::SYMBOL_WHILE";
    case TokenType::SYMBOL_WITH:
        return "TokenType::SYMBOL_WITH";
    case TokenType::SYMBOL_PLUS:
        return "TokenType::SYMBOL_PLUS";
    case TokenType::SYMBOL_MINUS:
        return "TokenType::SYMBOL_MINUS";
    case TokenType::SYMBOL_STAR:
        return "TokenType::SYMBOL_STAR";
    case TokenType::SYMBOL_STAR_STAR:
        return "TokenType::SYMBOL_STAR_STAR";
    case TokenType::SYMBOL_SLASH:
        return "TokenType::SYMBOL_SLASH";
    case TokenType::SYMBOL_EQUAL:
        return "TokenType::SYMBOL_EQUAL";
    case TokenType::SYMBOL_LESS_THAN:
        return "TokenType::SYMBOL_LESS_THAN";
    case TokenType::SYMBOL_GREATER_THAN:
        return "TokenType::SYMBOL_GREATER_THAN";
    case TokenType::SYMBOL_SQUARE_BRACKET_OPEN:
        return "TokenType::SYMBOL_SQUARE_BRACKET_OPEN";
    case TokenType::SYMBOL_SQUARE_BRACKET_CLOSE:
        return "TokenType::SYMBOL_SQUARE_BRACKET_CLOSE";
    case TokenType::SYMBOL_DOT:
        return "TokenType::SYMBOL_DOT";
    case TokenType::SYMBOL_COMMA:
        return "TokenType::SYMBOL_COMMA";
    case TokenType::SYMBOL_SEMICOLON:
        return "TokenType::SYMBOL_SEMICOLON";
    case TokenType::SYMBOL_COLON:
        return "TokenType::SYMBOL_COLON";
    case TokenType::SYMBOL_PAREN_OPEN:
        return "TokenType::SYMBOL_PAREN_OPEN";
    case TokenType::SYMBOL_PAREN_CLOSE:
        return "TokenType::SYMBOL_PAREN_CLOSE";
    case TokenType::SYMBOL_BRACE_OPEN:
        return "TokenType::SYMBOL_BRACE_OPEN";
    case TokenType::SYMBOL_BRACE_CLOSE:
        return "TokenType::SYMBOL_BRACE_CLOSE";
    case TokenType::SYMBOL_POWER:
        return "TokenType::SYMBOL_POWER";
    case TokenType::SYMBOL_LESS_GREATER_THAN:
        return "TokenType::SYMBOL_LESS_GREATER_THAN";
    case TokenType::SYMBOL_LESS_EQUAL_THAN:
        return "TokenType::SYMBOL_LESS_EQUAL_THAN";
    case TokenType::SYMBOL_GREATER_EQUAL_THAN:
        return "TokenType::SYMBOL_GREATER_EQUAL_THAN";
    case TokenType::SYMBOL_COLON_EQUAL:
        return "TokenType::SYMBOL_COLON_EQUAL";
    case TokenType::SYMBOL_DOT_DOT:
        return "TokenType::SYMBOL_DOT_DOT";
    case TokenType::SYMBOL_GREATER_LESS_THAN:
        return "TokenType::SYMBOL_GREATER_LESS_THAN";
    case TokenType::SYMBOL_EQUAL_GREATER_THAN:
        return "TokenType::SYMBOL_EQUAL_GREATER_THAN";
    case TokenType::SYMBOL_WHITE_SPACE:
        return "TokenType::SYMBOL_WHITE_SPACE";
    case TokenType::SYMBOL_LETTER:
        return "TokenType::SYMBOL_LETTER";
    case TokenType::SYMBOL_UNDERSCORE:
        return "TokenType::SYMBOL_UNDERSCORE";
    case TokenType::IDENTIFIER:
        return "TokenType::IDENTIFIER";
    case TokenType::STRING_LITERAL:
        return "TokenType::STRING_LITERAL";
    case TokenType::NUMERIC_LITERAL:
        return "TokenType::NUMERIC_LITERAL";
    case TokenType::BIG_INTEGER:
        return "TokenType::BIG_INTEGER";
    case TokenType::NEW_LINE:
        return "TokenType::NEW_LINE";
    case TokenType::COMMENT:
        return "TokenType::COMMENT";
    case TokenType::END_OF_FILE:
        return "TokenType::END_OF_FILE";
    case TokenType::UNKNOWN:
        return "TokenType::UNKNOWN";
    case TokenType::SYMBOL_EXTERNAL:
        return "TokenType::SYMBOL_EXTERNAL";
    default:
        return "UNKNOWN";
    }
}

Token Scanner::getCurrentTokenObject()
{
    const int lastNewLine = source.find_last_of('\n');
    const int lastReturnCar = source.find_last_of('\r');
    const int charLinePos = lastNewLine > lastReturnCar ? currentPos - lastNewLine : currentPos - lastReturnCar;
    Location tkPos = Location(curLine, charLinePos, filename);
    Token token = Token(curToken, tokenValue, tkPos);
    return token;
}
void Scanner::setSkipSpaces(bool skipSpaces)
{
    this->skipSpaces;
}
void Scanner::setSkipComments(bool skipComments)
{
    this->skipComments;
}
void Scanner::setFileName(std::string filename)
{
    this->filename = filename;
}
