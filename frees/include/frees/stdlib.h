#ifndef _DUO_FREES_STDLIB_H
#define _DUO_FREES_STDLIB_H

#ifdef __cplusplus
extern "C" {
#endif

long strtol(const char *__restrict__ str, char **__restrict__ endptr, int base);
int atoi(const char *str);

#ifdef __cplusplus
}
#endif

#endif // ! _DUO_FREES_STDLIB_H
