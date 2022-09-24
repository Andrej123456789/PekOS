#ifndef _VSPRINTF_H
#define _VSPRINTF_H

#include <stdarg.h>

int vsprintf(char *buf, const char *fmt, va_list args);

#endif