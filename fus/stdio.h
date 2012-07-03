#ifndef _DUO_FUS_STDIO_H
#define _DUO_FUS_STDIO_H

extern "C" {

#include <frees/stdarg.h>

struct FILE{
};

extern FILE *stdin;
extern FILE *stdout;
extern FILE *stderr;

int putchar(int c);
int printf(const char *format, ...);
int vprintf(const char *format, va_list ap);
int fprintf(FILE *fp, const char *format, ...);
int fputs(const char *s, FILE *stream);
char *fgets(char *s, int, FILE *);

}

#endif // ! _DUO_FUS_STDIO_H
