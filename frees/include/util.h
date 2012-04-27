#ifndef _DUO_UTIL_H
#define _DUO_UTIL_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#define ITOA_SIZE 65

// buf should have size ITOA_SIZE (binary uint64_t + NUL) (I386)
char *itoa(unsigned long long num, int base, char *buf);

char *duo_printf_itoa_pad(unsigned long long num, int base, char *buf, char pad, int width);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // ! _DUO_UTIL_H
