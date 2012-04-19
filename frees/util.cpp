#include <util.h>

extern "C" char *itoa(unsigned long long num, unsigned base, char *buf){
	static const char digits[] = "0123456789abcdefghijklmnopqrstuvwxyz";

	if(base < 2 || base > 10 + 26){
		base = 10;
	}

	buf += ITOA_SIZE - 1;
	*buf = 0;

	if(num == 0){
		--buf;
		*buf = '0';
		return buf;
	}

	do{
		--buf;
		*buf = digits[num % base];

		num /= base;
	}while(num != 0);

	return buf;
}

/* REAL: Хм, хм, а тут случайно ли не может произойти переполнение? */
extern "C" char *duo_printf_itoa_pad(unsigned long long num, unsigned base, char *buf, char pad, int width){
	char *buf_end = buf + ITOA_SIZE - 1;
	char *cursor = itoa(num, base, buf);

	while(buf_end - cursor < width){
		--cursor;
		*cursor = pad;
	}

	return cursor;
}
