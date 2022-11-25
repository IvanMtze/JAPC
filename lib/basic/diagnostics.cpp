//
// Created by wuser on 3/10/22.
//
#include "japc/basic/diagnostics.h"
#define LOCATION_ERROR "%s: [%d,%d]: error: %s\n"
#define LOCATION_WARNING "%s: [%d,%d]: warning: %s\n"

using namespace Pascal;
void JAPCDiagnostics::japc_error_at(Token tk, const char *error, ...)
{
    this->setHasErrors(HAS_ERROR);
    std::string fmt = this->getPreformattedMessage(LOCATION_ERROR, error, tk);
    va_list args;
    va_start(args, error);
    vfprintf(stderr, fmt.c_str(), args);
    va_end(args);
}
void JAPCDiagnostics::japc_warning_at(Token tk, const char *error, ...)
{
    if (this->shouldEmitWarningsAsErrors())
    {
        this->japc_error_at(tk, error);
        return;
    }
    if (!this->shouldEmitWarnings())
        return;
    std::string fmt = this->getPreformattedMessage(LOCATION_WARNING, error, tk);
    this->setHasWarnings(HAS_WARNING);
    va_list args;
    va_start(args, error);
    vfprintf(stderr, fmt.c_str(), args);
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
    if(emitWarnings)
        this->state |= OMIT_WARNING;
}
void JAPCDiagnostics::setEmitWarningsAsErrors(unsigned int emitWarningsAsErrors)
{
    if(emitWarningsAsErrors)
        this->state |= ERROR_AS_WAR;
}
void JAPCDiagnostics::setHasErrors(unsigned int hasErrors)
{
    this->state |= hasErrors;
}
void JAPCDiagnostics::setHasWarnings(unsigned int hasWarnings)
{
    this->state |= hasWarnings;
}
std::string JAPCDiagnostics::getPreformattedMessage(const char *message, const char *error, Token tk)
{
    char *buff;
    asprintf(&buff, message, tk.getTokenPos().getFileName().c_str(), tk.getTokenPos().getLineNo(),
             tk.getTokenPos().getCharNo(), error);
    std::string str(buff);
    free(buff);
    return str;
}
JAPCDiagnostics::JAPCDiagnostics()
{
    this->state = 0;
}
