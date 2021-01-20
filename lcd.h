#include <reg52.h>
#include <intrins.h>

#ifndef __LCD_H
#define __LCD_H

#define uchar unsigned char
#define uint  unsigned int

void delay(int ms);	
void lcd_init();
void printf_str(uchar x, uchar y, uchar* p);
void lcd_wcmd(uchar cmd);

#endif

