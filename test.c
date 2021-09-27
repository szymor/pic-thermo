#include <stdio.h>
#include <stdint.h>

void temp2str(int16_t temp, char *str);

int main()
{
	char string[16] = "S              ";
	char *eos = string + 14;
	temp2str(0x07d0, eos);
	printf("+125     -> %s\n", string);
	temp2str(0x0550, eos);
	printf("+85      -> %s\n", string);
	temp2str(0x0191, eos);
	printf("+25.0625 -> %s\n", string);
	temp2str(0x00a2, eos);
	printf("+10.125  -> %s\n", string);
	temp2str(0x0008, eos);
	printf("+0.5     -> %s\n", string);
	temp2str(0x0000, eos);
	printf("0        -> %s\n", string);
	temp2str(0xfff8, eos);
	printf("-0.5     -> %s\n", string);
	temp2str(0xff5e, eos);
	printf("-10.125  -> %s\n", string);
	temp2str(0xfe6f, eos);
	printf("-25.0625 -> %s\n", string);
	temp2str(0xfc90, eos);
	printf("-55      -> %s\n", string);
	return 0;
}

void temp2str(int16_t temp, char *eos)
{
	temp *= 10;
	temp >>= 4;
	int8_t chars_left = 8;
	char minus = temp < 0;
	if (minus)
	{
		temp = -temp;
	}
	char dot = 1;
	do
	{
		int8_t digit = temp % 10;
		*eos = digit + 0x30;
		--eos;
		--chars_left;
		temp /= 10;
		if (dot && (temp || digit))
		{
			*eos = '.';
			--eos;
			--chars_left;
			dot = 0;
		}
	} while (0 != temp);
	if (minus)
	{
		*eos = '-';
		--eos;
		--chars_left;
	}
	while (chars_left--)
	{
		*eos = ' ';
		--eos;
	}
}
