/*
 * drv_uart.c
 *
 *  Created on: 31 mars 2016
 *      Author: kerhoas
 */

#include "drv_uart.h"


uint8_t rec_buf2[2];
uint8_t rec_buf6[2];
uint8_t rec_buf[2];

int put_char(int);

//=================================================================
//	UART 2 INIT (STLINK UART)
//=================================================================

void uart2_Init()
{
	  Uart2Handle.Instance        = USART2;
	  Uart2Handle.Init.BaudRate   = UART_BAUDRATE;
	  Uart2Handle.Init.WordLength = UART_WORDLENGTH_8B;
	  Uart2Handle.Init.StopBits   = UART_STOPBITS_1;
	  Uart2Handle.Init.Parity     = UART_PARITY_NONE;
	  Uart2Handle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
	  Uart2Handle.Init.Mode       = UART_MODE_TX_RX;
	  Uart2Handle.Init.OverSampling = UART_OVERSAMPLING_16;

	  HAL_UART_Init(&Uart2Handle);

	  HAL_UART_Receive_IT(&Uart2Handle, (uint8_t *)rec_buf, 1);
}

//=================================================================
//	UART WRITE
//=================================================================

void uart_Write(char* toprint, int size)
{
	HAL_UART_Transmit(&Uart2Handle, (uint8_t *)toprint, size, 0xFFFF);
}

//=================================================================
//	PUCHAR PROTOTYPE (USED BY PRINTF FUNCTIONS)
//=================================================================



int put_char(int ch)
{
	HAL_UART_Transmit(&Uart2Handle, (uint8_t *)&ch, 1, 0xFFFF);
	return 0;
}

//=================================================================
//	UART RECEIVE CALLBACK5
//=================================================================
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
	 HAL_UART_Receive_IT(&Uart2Handle, (uint8_t *)rec_buf, 1);
}

//================================================================
// 				NUM2STR
//   convert a number 'number' in base 'base' to the string s (the string
//   variable must be large enough)
//   size : (unsigned int) minimal number of digit width for string number
//   sp   : (boolean) complete the width with ' ' or '0'
//=================================================================
static void num2str(char *s, unsigned int number, unsigned int base, unsigned int size, int sp)
{
        static char  hexChars[] = "0123456789ABCDEF";

        char *p=s;
        unsigned int cnt;
        unsigned int i;
        char tmp;

        // get digits
        do {
                *s++=hexChars[number % base];
        } while (number /= base);
        *s='\0';

        // reverse string
        cnt=s-p;
        for (i=0;i<cnt/2;i++) {
                tmp=p[i]; p[i] = p[cnt-i-1]; p[cnt-i-1]=tmp;
        }

        // add extra space
        if (cnt<size) {
                for (i=cnt;i==0;i--)
                		{p[i+size-cnt]=p[i];}
                if (sp) tmp=' '; else tmp='0';
                for (i=0;i<size-cnt;i++) p[i]=tmp;
        }
}
//================================================================
//				STR2NUM
//================================================================
static unsigned int str2num(char *s, unsigned base)
{
	unsigned int u=0, d;
	char ch=*s++;
	while (ch) {
		if ((ch>='0') && (ch<='9')) d=ch-'0';
		else if ((base==16) && (ch>='A') && (ch<='F')) d=ch-'A'+10;
		else if ((base==16) && (ch>='a') && (ch<='f')) d=ch-'a'+10;
		else break;
		u=d+base*u;
		ch=*s++;
	}
	return u;
}

//================================================================
//				FLOAT2STR
//================================================================
void float2str( char* s, float i)
{
    const char *digit = "0123456789ABCDEF";
    unsigned int buf[32];
    float k = i;
    int numb, j, kk;

    j = i;
    numb = 0;

    for (; k*10 != (float) j*10; )
    {
        k = k*10;
        j = k;
        numb++;
    }

    kk = 0;

    do
    {
    	buf[kk] = j % 10;
        j = j / 10;
        kk++;
    }
    while ( j > 0 );

    for (; --kk >= 0; )
    {
        *s++=digit[ buf[kk] ];
        if ( kk == numb )
        {  *s++='.';}
    }
    if ( numb == 0 )
    	{ *s++='0' ;}

    *s='\0';
}

//================================================================
//				PUT STRING
//================================================================
void put_string(char* s)
{
	while(*s != '\0')
	{
		put_char(*s);
		s++;
	}
}



//================================================================
//				TERM_PRINTF
//================================================================

void term_printf(const char* fmt, ...)
{
	va_list        ap;
	char          *p;
	char           ch;
	unsigned long  ul;
	unsigned long  size;
	unsigned int   sp;
	char           s[34];

	va_start(ap, fmt);
	while (*fmt != '\0') {
		if (*fmt =='%') {
			size=0; sp=1;
			if (*++fmt=='0') {fmt++; sp=0;}	// parse %04d --> sp=0
			ch=*fmt;
			if ((ch>'0') && (ch<='9')) {	// parse %4d --> size=4
				char tmp[10];
				int i=0;
				while ((ch>='0') && (ch<='9')) {
					tmp[i++]=ch;
					ch=*++fmt;
				}
				tmp[i]='\0';
				size=str2num(tmp,10);
			}
			switch (ch) {
				case '%':
					put_char('%');
					break;
				case 'c':
					ch = va_arg(ap, int);
					put_char(ch);
					break;
				case 's':
					p = va_arg(ap, char *);
					put_string(p);
					break;
				case 'd':
					ul = va_arg(ap, long);
					if ((long)ul < 0) {
						put_char('-');
						ul = -(long)ul;
						//size--;
					}
					num2str(s, ul, 10, size, sp);
					put_string(s);
					break;
				case 'u':
					ul = va_arg(ap, unsigned int);
					num2str(s, ul, 10, size, sp);
					put_string(s);
					break;
				case 'o':
					ul = va_arg(ap, unsigned int);
					num2str(s, ul, 8, size, sp);
					put_string(s);
					break;
				case 'p':
					put_char('0');
					put_char('x');
					ul = va_arg(ap, unsigned int);
					num2str(s, ul, 16, size, sp);
					put_string(s);
					break;
				case 'x':
					ul = va_arg(ap, unsigned int);
					num2str(s, ul, 16, size, sp);
					put_string(s);
					break;
				case 'f':
					ul = va_arg(ap, double);
					float2str(s, ul);
					put_string(s);
					break;

				default:
					put_char(*fmt);
			}
		} else put_char(*fmt);
		fmt++;
	}
	va_end(ap);
}


