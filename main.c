#include <xc.h>
#include <stdint.h>

#define _XTAL_FREQ		(4000000)

#define OW_PIN		RB5
#define OW_TRIS		TRISB5

#define RST_MASK	_PORTA_RA2_MASK
#define CE_MASK		_PORTA_RA3_MASK
#define DC_MASK		_PORTA_RA1_MASK
#define DIN_MASK	_PORTA_RA0_MASK
#define CLK_MASK	_PORTA_RA7_MASK
#define BL_PIN		RA6
#define BL_TRIS		TRISA6

#define MAX_STRLEN	(17)
#define HISTORY_LEN	(12)
#define SAMPLE_DELAY	(5000)
// number of sample delays forming one hours
// time correction included
#define HISTORY_PERIOD	(720-58)

#pragma config CP=OFF
#pragma config CPD=OFF
#pragma config LVP=OFF
#pragma config BOREN=OFF
#pragma config MCLRE=ON
#pragma config PWRTE=ON
#pragma config WDTE=OFF
#pragma config FOSC=INTOSCIO

const uint8_t charset[][5] = {
  { 0x00, 0x00, 0x00, 0x00, 0x00 },  // 20 space
  { 0x00, 0x00, 0x5f, 0x00, 0x00 },  // 21 !
  { 0x00, 0x07, 0x00, 0x07, 0x00 },  // 22 "
  { 0x14, 0x7f, 0x14, 0x7f, 0x14 },  // 23 #
  { 0x24, 0x2a, 0x7f, 0x2a, 0x12 },  // 24 $
  { 0x23, 0x13, 0x08, 0x64, 0x62 },  // 25 %
  { 0x36, 0x49, 0x55, 0x22, 0x50 },  // 26 &
  { 0x00, 0x05, 0x03, 0x00, 0x00 },  // 27 '
  { 0x00, 0x1c, 0x22, 0x41, 0x00 },  // 28 (
  { 0x00, 0x41, 0x22, 0x1c, 0x00 },  // 29 )
  { 0x14, 0x08, 0x3e, 0x08, 0x14 },  // 2a *
  { 0x08, 0x08, 0x3e, 0x08, 0x08 },  // 2b +
  { 0x00, 0x50, 0x30, 0x00, 0x00 },  // 2c ,
  { 0x08, 0x08, 0x08, 0x08, 0x08 },  // 2d -
  { 0x00, 0x60, 0x60, 0x00, 0x00 },  // 2e .
  { 0x20, 0x10, 0x08, 0x04, 0x02 },  // 2f /
  { 0x3e, 0x51, 0x49, 0x45, 0x3e },  // 30 0
  { 0x00, 0x42, 0x7f, 0x40, 0x00 },  // 31 1
  { 0x42, 0x61, 0x51, 0x49, 0x46 },  // 32 2
  { 0x21, 0x41, 0x45, 0x4b, 0x31 },  // 33 3
  { 0x18, 0x14, 0x12, 0x7f, 0x10 },  // 34 4
  { 0x27, 0x45, 0x45, 0x45, 0x39 },  // 35 5
  { 0x3c, 0x4a, 0x49, 0x49, 0x30 },  // 36 6
  { 0x01, 0x71, 0x09, 0x05, 0x03 },  // 37 7
  { 0x36, 0x49, 0x49, 0x49, 0x36 },  // 38 8
  { 0x06, 0x49, 0x49, 0x29, 0x1e },  // 39 9
  { 0x00, 0x36, 0x36, 0x00, 0x00 },  // 3a :
  { 0x00, 0x56, 0x36, 0x00, 0x00 },  // 3b ;
  { 0x08, 0x14, 0x22, 0x41, 0x00 },  // 3c <
  { 0x14, 0x14, 0x14, 0x14, 0x14 },  // 3d =
  { 0x00, 0x41, 0x22, 0x14, 0x08 },  // 3e >
  { 0x02, 0x01, 0x51, 0x09, 0x06 },  // 3f ?
  { 0x32, 0x49, 0x79, 0x41, 0x3e },  // 40 @
  { 0x7e, 0x11, 0x11, 0x11, 0x7e },  // 41 A
  { 0x7f, 0x49, 0x49, 0x49, 0x36 },  // 42 B
  { 0x3e, 0x41, 0x41, 0x41, 0x22 },  // 43 C
  { 0x7f, 0x41, 0x41, 0x22, 0x1c },  // 44 D
  { 0x7f, 0x49, 0x49, 0x49, 0x41 },  // 45 E
  { 0x7f, 0x09, 0x09, 0x09, 0x01 },  // 46 F
  { 0x3e, 0x41, 0x49, 0x49, 0x7a },  // 47 G
  { 0x7f, 0x08, 0x08, 0x08, 0x7f },  // 48 H
  { 0x00, 0x41, 0x7f, 0x41, 0x00 },  // 49 I
  { 0x20, 0x40, 0x41, 0x3f, 0x01 },  // 4a J
  { 0x7f, 0x08, 0x14, 0x22, 0x41 },  // 4b K
  { 0x7f, 0x40, 0x40, 0x40, 0x40 },  // 4c L
  { 0x7f, 0x02, 0x0c, 0x02, 0x7f },  // 4d M
  { 0x7f, 0x04, 0x08, 0x10, 0x7f },  // 4e N
  { 0x3e, 0x41, 0x41, 0x41, 0x3e },  // 4f O
  { 0x7f, 0x09, 0x09, 0x09, 0x06 },  // 50 P
  { 0x3e, 0x41, 0x51, 0x21, 0x5e },  // 51 Q
  { 0x7f, 0x09, 0x19, 0x29, 0x46 },  // 52 R
  { 0x46, 0x49, 0x49, 0x49, 0x31 },  // 53 S
  { 0x01, 0x01, 0x7f, 0x01, 0x01 },  // 54 T
  { 0x3f, 0x40, 0x40, 0x40, 0x3f },  // 55 U
  { 0x1f, 0x20, 0x40, 0x20, 0x1f },  // 56 V
  { 0x3f, 0x40, 0x38, 0x40, 0x3f },  // 57 W
  { 0x63, 0x14, 0x08, 0x14, 0x63 },  // 58 X
  { 0x07, 0x08, 0x70, 0x08, 0x07 },  // 59 Y
  { 0x61, 0x51, 0x49, 0x45, 0x43 },  // 5a Z
  { 0x00, 0x7f, 0x41, 0x41, 0x00 },  // 5b [
  { 0x02, 0x04, 0x08, 0x10, 0x20 },  // 5c backslash
  { 0x00, 0x41, 0x41, 0x7f, 0x00 },  // 5d ]
//  { 0x04, 0x02, 0x01, 0x02, 0x04 },  // 5e ^
  { 0x00, 0x06, 0x09, 0x09, 0x06 },  // 5e degree character
  { 0x40, 0x40, 0x40, 0x40, 0x40 },  // 5f _
  { 0x00, 0x01, 0x02, 0x04, 0x00 },  // 60 `
  { 0x20, 0x54, 0x54, 0x54, 0x78 },  // 61 a
  { 0x7f, 0x48, 0x44, 0x44, 0x38 },  // 62 b
  { 0x38, 0x44, 0x44, 0x44, 0x20 },  // 63 c
  { 0x38, 0x44, 0x44, 0x48, 0x7f },  // 64 d
  { 0x38, 0x54, 0x54, 0x54, 0x18 },  // 65 e
  { 0x08, 0x7e, 0x09, 0x01, 0x02 },  // 66 f
  { 0x0c, 0x52, 0x52, 0x52, 0x3e },  // 67 g
  { 0x7f, 0x08, 0x04, 0x04, 0x78 },  // 68 h
  { 0x00, 0x44, 0x7d, 0x40, 0x00 },  // 69 i
  { 0x20, 0x40, 0x44, 0x3d, 0x00 },  // 6a j
  { 0x7f, 0x10, 0x28, 0x44, 0x00 },  // 6b k
  { 0x00, 0x41, 0x7f, 0x40, 0x00 },  // 6c l
  { 0x7c, 0x04, 0x18, 0x04, 0x78 },  // 6d m
  { 0x7c, 0x08, 0x04, 0x04, 0x78 },  // 6e n
  { 0x38, 0x44, 0x44, 0x44, 0x38 },  // 6f o
  { 0x7c, 0x14, 0x14, 0x14, 0x08 },  // 70 p
  { 0x08, 0x14, 0x14, 0x18, 0x7c },  // 71 q
  { 0x7c, 0x08, 0x04, 0x04, 0x08 },  // 72 r
  { 0x48, 0x54, 0x54, 0x54, 0x20 },  // 73 s
  { 0x04, 0x3f, 0x44, 0x40, 0x20 },  // 74 t
  { 0x3c, 0x40, 0x40, 0x20, 0x7c },  // 75 u
  { 0x1c, 0x20, 0x40, 0x20, 0x1c },  // 76 v
  { 0x3c, 0x40, 0x30, 0x40, 0x3c },  // 77 w
  { 0x44, 0x28, 0x10, 0x28, 0x44 },  // 78 x
  { 0x0c, 0x50, 0x50, 0x50, 0x3c },  // 79 y
  { 0x44, 0x64, 0x54, 0x4c, 0x44 },  // 7a z
  { 0x00, 0x08, 0x36, 0x41, 0x00 },  // 7b {
  { 0x00, 0x00, 0x7f, 0x00, 0x00 },  // 7c |
  { 0x00, 0x41, 0x36, 0x08, 0x00 },  // 7d }
  { 0x10, 0x08, 0x08, 0x10, 0x08 },  // 7e ~
  { 0x00, 0x00, 0x00, 0x00, 0x00 }   // 7f
};

const char strings[][17] = {
	" 1 hour ago     ",
	" 2 hours ago    ",
	" 3 hours ago    ",
	" 4 hours ago    ",
	" 5 hours ago    ",
	" 6 hours ago    ",
	" 7 hours ago    ",
	" 8 hours ago    ",
	" 9 hours ago    ",
	" 10 hours ago   ",
	" 11 hours ago   ",
	" 12 hours ago   "
};

int16_t history[HISTORY_LEN] = { 0 };
uint8_t state = 0;
uint8_t shadowA = 0;
uint16_t counter = 0;

void send_byte(uint8_t byte);
void setb(uint8_t msk) { shadowA |= msk; PORTA = shadowA; }
void clrb(uint8_t msk) { shadowA &= ~msk; PORTA = shadowA; }
void write(int x, int y, const uint8_t string[]);

int ow_reset(void);
void ow_write0(void);
void ow_write1(void);
int ow_read(void);
void ow_send_byte(uint8_t byte);
uint8_t ow_recv_byte(void);

void temp2str(int16_t temp, char *eos);
int16_t ctof(int16_t tempc);

void main(void)
{
	// CPU set to 4 MHz
	PCONbits.OSCF = 1;

	// display pin init
	TRISA = 0;
	setb(CE_MASK);
	__delay_ms(1);
	setb(RST_MASK);
	__delay_ms(1);

	// display init
	clrb(CE_MASK);
	clrb(DC_MASK);
	send_byte(0x21);
	send_byte(0xb2);
	send_byte(0x13);
	send_byte(0x20);
	send_byte(0x09);
	send_byte(0x0c);
	send_byte(0x40);
	send_byte(0x80);
	setb(CE_MASK);

	// one wire pin init
	OW_TRIS = 0;
	OW_PIN = 1;
	__delay_ms(1);

	while (1)
	{
		// ds18b20 - reset
		while (ow_reset())
		{
			write(0, 0, "DS18B20         ");
			write(0, 1, "not found       ");
			__delay_ms(1000);
		}
		// ds18b20 - skip ROM
		ow_send_byte(0xcc);
		// ds18b20 - convert T
		ow_send_byte(0x44);
		__delay_ms(SAMPLE_DELAY);
		// ds18b20 - reset
		ow_reset();
		// ds18b20 - skip ROM
		ow_send_byte(0xcc);
		// ds18b20 - read scratchpad
		ow_send_byte(0xbe);
		uint8_t tlsb = ow_recv_byte();
		uint8_t tmsb = ow_recv_byte();
		int16_t temp = (tmsb << 8) | tlsb;
		// update history if needed
		if (0 == counter)
		{
			for (int i = HISTORY_LEN - 1; i > 0; --i)
			{
				history[i] = history[i - 1];
			}
			history[0] = temp;
		}
		// display things
		uint8_t offset1 = state * 2;
		uint8_t offset2 = offset1 + 1;
		const char *text1 = strings[offset1];
		const char *text2 = strings[offset2];
		int16_t history1 = history[offset1];
		int16_t history2 = history[offset2];
		char buff[17] = "                ";
		char *eos;
		// ---
		write(0, 0, " Now:     ^C/^F ");
		eos = buff + 6;
		temp2str(temp, eos);
		eos = buff + 14;
		temp2str(ctof(temp), eos);
		write(0, 1, buff);
		// ---
		write(0, 2, text1);
		eos = buff + 6;
		temp2str(history1, eos);
		eos = buff + 14;
		temp2str(ctof(history1), eos);
		write(0, 3, buff);
		// ---
		write(0, 4, text2);
		eos = buff + 6;
		temp2str(history2, eos);
		eos = buff + 14;
		temp2str(ctof(history2), eos);
		write(0, 5, buff);
		// counter update
		++counter;
		if (HISTORY_PERIOD == counter)
		{
			counter = 0;
		}
		// state update
		state = (state + 1) % 6;
	}
}

void send_byte(uint8_t byte)
{
	for (int i = 0; i < 8; ++i)
	{
		if (byte >> 7)
			setb(DIN_MASK);
		else
			clrb(DIN_MASK);
		setb(CLK_MASK);
		clrb(CLK_MASK);
		byte <<= 1;
	}
}

void write(int x, int y, const uint8_t string[])
{
	const uint8_t *ptr = string;
	clrb(CE_MASK);
	clrb(DC_MASK);
	send_byte(0x40 | y & 0x07);
	send_byte(0x80 | x & 0x7f);
	setb(DC_MASK);
	while (*ptr != 0)
	{
		// send char
		uint8_t ch = *ptr;
		if ('\n' == ch)
		{
			clrb(DC_MASK);
			++y;
			send_byte(0x40 | y & 0x07);
			send_byte(0x80 | x & 0x7f);
			setb(DC_MASK);
		}
		else
		{
			if (ch < 0x20 || ch > 0x7f)
				ch = 0x20;	// space
			ch -= 0x20;
			for (int i = 0; i < 5; ++i)
			{
				send_byte(charset[ch][i]);
			}
		}
		++ptr;
	}
	setb(CE_MASK);
}

int ow_reset(void)
{
	OW_PIN = 0;
	__delay_us(500);
	OW_PIN = 1;
	__delay_us(120);
	int ret = OW_PIN;
	__delay_us(500);
	return ret;
}

void ow_write0(void)
{
	OW_PIN = 0;
	__delay_us(80);
	OW_PIN = 1;
	__delay_us(10);
}

void ow_write1(void)
{
	OW_PIN = 0;
	__delay_us(5);
	OW_PIN = 1;
	__delay_us(85);
}

int ow_read(void)
{
	OW_PIN = 0;
	__delay_us(2);
	OW_PIN = 1;
	__delay_us(10);
	return OW_PIN;
}

void ow_send_byte(uint8_t byte)
{
	for (int i = 0; i < 8; ++i)
	{
		if (byte & 0x01)
			ow_write1();
		else
			ow_write0();
		byte >>= 1;
	}
}

uint8_t ow_recv_byte(void)
{
	uint8_t ret = 0;
	for (int i = 0; i < 8; ++i)
	{
		ret >>= 1;
		ret |= ow_read() ? 0x80 : 0;
	}
	return ret;
}

void temp2str(int16_t temp, char *eos)
{
	temp *= 10;
	temp >>= 4;
	int8_t chars_left = 6;
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

int16_t ctof(int16_t tempc)
{
	return (tempc * 9 / 5 + 32*16);
}
