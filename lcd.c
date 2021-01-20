#include "lcd.h"

#define delayNOP(); {_nop_();_nop_();_nop_();_nop_();};	
#define LCD_data  P0 

sbit LCD_RS  =  P2^0;            
sbit LCD_RW  =  P2^1;            
sbit LCD_EN  =  P2^2;           
sbit LCD_PSB =  P2^3;        

void delay(int ms)
{
    while(ms--)
	{
      uchar i;
	  for(i=0;i<250;i++)  
	   {
	    _nop_();			   
		_nop_();
		_nop_();
		_nop_();
	   }
	}
}		

bit lcd_busy()
 {                          
    bit result;
    LCD_RS = 0;
    LCD_RW = 1;
    LCD_EN = 1;
    delayNOP();
    result = (bit)(P0&0x80);
    LCD_EN = 0;
    return(result); 
 }

void lcd_wcmd(uchar cmd)
{                          
   while(lcd_busy());
    LCD_RS = 0;
    LCD_RW = 0;
    LCD_EN = 0;
    _nop_();
    _nop_(); 
    P0 = cmd;
    delayNOP();
    LCD_EN = 1;
    delayNOP();
    LCD_EN = 0;  
}

void lcd_wdat(uchar dat)
{                          
   while(lcd_busy());
    LCD_RS = 1;
    LCD_RW = 0;
    LCD_EN = 0;
    P0 = dat;
    delayNOP();
    LCD_EN = 1;
    delayNOP();
    LCD_EN = 0; 
}

void lcd_init()
{ 

    LCD_PSB = 1;        
    
    lcd_wcmd(0x34);   
    delay(5);
    lcd_wcmd(0x30);      
    delay(5);
    lcd_wcmd(0x0C);     
    delay(5);
    lcd_wcmd(0x01);      
    delay(5);
}

void lcd_pos(uchar X,uchar Y)
{                          
   uchar  pos;
   if (X==0)
     {X=0x80;}
   else if (X==1)
     {X=0x90;}
   else if (X==2)
     {X=0x88;}
   else if (X==3)
     {X=0x98;}
   pos = X+Y ;  
   lcd_wcmd(pos);     
}
void printf_str(uchar x, uchar y, uchar* p){
	lcd_pos(x, y);
	while(*p != '\0')
	{
		lcd_wdat(*p);     
	    p++;
	}
}



