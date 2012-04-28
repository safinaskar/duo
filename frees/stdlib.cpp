#include <ctype.h>

#include <frees/stdlib.h>

/* TODO: real POSIX strtol */
long strtol(const char *__restrict__ str, char **__restrict__ /* TODO: endptr */, int /* TODO: support other bases and base 0 */){
	while(*str != 0 && isspace(*str)){
		++str;
	}

	long result = 0;
	while('0' <= *str && *str <= '9'){ /* TODO: overflow */
		result = result * 10 + (*str - '0');
		++str;
	}
	return result;
}

int atoi(const char *str){
	return int(strtol(str, 0, 10));
}
