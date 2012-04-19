#ifndef _DUO_FREES_STRING_H
#define _DUO_FREES_STRING_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef __SIZE_TYPE__ size_t;

void *memcpy(void *__restrict__ s1, const void *__restrict__ s2, size_t n);
void *memset(void *s, int c, size_t n);
int strcmp(const char *s1, const char *s2);
char *strcpy(char *__restrict__ s1, const char *__restrict__ s2);
size_t strlen(const char *s);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // ! _DUO_FREES_STRING_H
