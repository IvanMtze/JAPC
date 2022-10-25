//
// Created by wuser on 3/10/22.
//
#include "japc/basic/diagnostics.h"
using namespace Pascal;
#define LOCATION_ERROR "%s: %d: error: "
#define LOCATION_WARNING "%s: %d: warning: "
void JAPCDiagnostics::japc_error_at(Token tk, const char *error, ...)
{
    this->setHasErrors(HAS_ERROR);
    const char *message = this->getPreformattedMessage(LOCATION_ERROR, error, tk);
    va_list args;
    va_start(args, message);
    vprintf(message, args);
    va_end(args);
}
void JAPCDiagnostics::japc_warning_at(Token tk, const char *error, ...)
{
    if (this->shouldEmitWarningsAsErrors())
    {
        this->japc_error_at(tk, error);
        return;
    }
    const char *message = this->getPreformattedMessage(LOCATION_WARNING, error, tk);
    this->setHasWarnings(HAS_WARNING);
    va_list args;
    va_start(args, message);
    vprintf(message, args);
    va_end(args);
}
int JAPCDiagnostics::shouldEmitWarnings()
{
    return state & OMIT_WARNING;
}
int JAPCDiagnostics::hasErrors()
{
    return state & HAS_ERROR;
}
int JAPCDiagnostics::hasWanings()
{
    return state & HAS_WARNING;
}
int JAPCDiagnostics::shouldEmitWarningsAsErrors()
{
    return state & ERROR_AS_WAR;
}
void JAPCDiagnostics::setEmitWarnings(unsigned int emitWarnings)
{
    this->state |= emitWarnings;
}
void JAPCDiagnostics::setEmitWarningsAsErrors(unsigned int emitWarningsAsErrors)
{
    this->state |= emitWarningsAsErrors;
}
void JAPCDiagnostics::setHasErrors(unsigned int hasErrors)
{
    this->state |= hasErrors;
}
void JAPCDiagnostics::setHasWarnings(unsigned int hasWarnings)
{
    this->state |= hasWarnings;
}
const char *JAPCDiagnostics::getPreformattedMessage(const char *message, const char *error, Token tk)
{
    size_t neededSize = sizeof(error) / sizeof(char) + sizeof(message) / sizeof(char) +
                        sizeof(tk.getTokenPos().getFileName().c_str()) / sizeof(char);
    char buff[neededSize];
    sprintf(buff, message, tk.getTokenPos().getFileName().c_str(), tk.getTokenPos().getLineNo());
    strcat(buff, error);
    return buff;
}
