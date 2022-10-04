//
// Created by wuser on 3/10/22.
//
#include "diagnostics.h";
void JAPCDiagnostics::japc_error_at(std::unique_ptr<Token> tk, const char *error, ...)
{
    this->setHasErrors(HAS_ERROR);
    va_list args;
    va_start(args, error);
    vprintf(error, args);
    va_end(args);
}
void JAPCDiagnostics::japc_warning_at(std::unique_ptr<Token> tk, const char *error, ...)
{
    this->setHasWarnings(HAS_WARNING);
    va_list args;
    va_start(args, error);
    vprintf(error, args);
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
