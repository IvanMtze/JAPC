//
// Created by wuser on 3/10/22.
//

#ifndef JAPC_DIAGNOSTICS_H
#define JAPC_DIAGNOSTICS_H
#include "japc/scanner/scanner.h"
#include <stdarg.h>
#include <stdio.h>
#include <cstdio>
#include <cstdarg>

#define HAS_WARNING     0b00000001
#define HAS_ERROR       0b00000010
#define OMIT_WARNING    0b00000100
#define ERROR_AS_WAR    0b00001000

namespace Pascal{
class Token;
class JAPCDiagnostics{
    //  TODO: there should be a better way to do it.
  public:
    JAPCDiagnostics();
    void japc_error_at(Token tk, const char* error, ...);
    void japc_warning_at(Token tk, const char* error, ...);
    int shouldEmitWarnings();
    int hasErrors();
    int hasWanings();
    int shouldEmitWarningsAsErrors();
    void setEmitWarnings(unsigned int emitWarnings);
    void setEmitWarningsAsErrors(unsigned int emitWarningsAsErrors);
  private:
    unsigned int state : 4;
    std::string getPreformattedMessage(const char* message, const char *error,Token tk);
    void setHasErrors(unsigned int hasErrors);
    void setHasWarnings(unsigned int hasWarnings);
};
}
#endif // JAPC_DIAGNOSTICS_H
