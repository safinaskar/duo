#ifndef _DUO_STRING_H
#define _DUO_STRING_H

typedef __SIZE_TYPE__ size_t;

static inline void *memcpy(void *__restrict__ s1, const void *__restrict__ s2, size_t n){
	return __builtin_memcpy(s1, s2, n);
}

static inline void *memset(void *s, int c, size_t n){
	return __builtin_memset(s, c, n);
}

static inline int strcmp(const char *s1, const char *s2){
	return __builtin_strcmp(s1, s2);
}

static inline char *strcpy(char *__restrict__ s1, const char *__restrict__ s2){
	return __builtin_strcpy(s1, s2);
}

static inline size_t strlen(const char *s){
	return __builtin_strlen(s);
}

#endif // ! _DUO_STRING_H
