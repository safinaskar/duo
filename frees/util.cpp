#include <util.h>

char *itoa(unsigned long long num, int base, char *buf){
	static const char digits[] = "0123456789abcdefghijklmnopqrstuvwxyz";

	if(base < 2 || base > 36){
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
		*buf = digits[num % unsigned(base)];

		num /= unsigned(base);
	}while(num != 0);

	return buf;
}

char *duo_printf_itoa_pad(unsigned long long num, int base, char *buf, char pad, int width){
	if(width > ITOA_SIZE - 1){
		width = ITOA_SIZE - 1;
	}

	char *buf_end = buf + ITOA_SIZE - 1;
	char *cursor = itoa(num, base, buf);

	while(buf_end - cursor < width){
		--cursor;
		*cursor = pad;
	}

	return cursor;
}
