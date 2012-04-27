#include <frees/string.h>

extern "C" void *memcpy(void *__restrict__ s1, const void *__restrict__ s2, size_t n){
	char *c1 = (char *)s1; // TIME: я правильно работаю с restrict?
	const char *c2 = (const char *)s2;

	for(size_t i = 0; i != n; ++i){
		c1[i] = c2[i];
	}

	return s1;
}

extern "C" void *memset(void *s, int c, size_t n){
	char *cs = (char *)s;

	for(size_t i = 0; i != n; ++i){
		cs[i] = char(c);
	}

	return s;
}

/*
 * Функцию strcmp можно было бы написать быстрее и проще, вот так:
 * for(;;){
 *   if(*s1 < *s2){
 *     return -1;
 *   }
 *   if(*s1 > *s2){
 *     return 1;
 *   }
 *   if(*s1 == 0){
 *     // Здесь мы точно знаем, что *s1 == *s2, а потому *s2 тоже равно нулю. Значит, строки равны
 *     return 0;
 *   }
 *   ++s1;
 *   ++s2;
 * }
 * Но такой код не будет работать с символами за пределами ASCII на платформах, где char имеет знак (например, i386)
 * Теоритически, здесь нужно поставить compile-time проверку на знаковость char'а, чтобы оптимизировать код на платформах с беззнаковым char'ом, но я не буду этого делать в таком простом ядре
 */

extern "C" int strcmp(const char *s1, const char *s2){
	for(;;){
		if(*s1 == 0){
			if(*s2 == 0){
				return 0;
			}else{
				return -1;
			}
		}

		if(*s2 == 0){
			return 1;
		}

		if(*s1 < *s2){
			return -1;
		}

		if(*s1 > *s2){
			return 1;
		}

		++s1;
		++s2;
	}
}

extern "C" char *strcpy(char *__restrict__ s1, const char *__restrict__ s2){
	for(;;){
		*s1 = *s2;
		if(*s2 == 0){
			return s1;
		}
		++s1;
		++s2;
	}
}

extern "C" size_t strlen(const char *s){
	size_t result = 0;
	for(; s[result] != 0; ++result);
	return result;
}
