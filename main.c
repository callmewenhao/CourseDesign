#include "lcd.h"
#include "menu.h"
#include <stdio.h>

#define uint8_t unsigned char
#define uint16_t unsigned int 

/* Declare external variables */
extern struct date my_date;
extern struct beep_time my_beep_time;

/* BEEP bit*/
sbit beep = P2^5;

/* Global variable */
static uint16_t count_times = 0; // count times for 1 sec
static short int hour_beep_times = 5; // 整点报时相关

void delay_ms(uint16_t time_ms);
void Timer1Init(void);
void UartInit(void);
void display_now_date(void);
void change_now_date_time(void);
void update_today(void);

void tm1_interrupt() interrupt 3
{
	TH1 = 0xdc; // 设置定时初值
    TL1 = 0x00;

    if(count_times < 100){
        count_times ++;
    } else if(count_times == 100){
        count_times = 0;
        change_now_date_time(); // 时钟功能
		 if(my_date.minute == 0 && my_date.second == 0){
			// 整点修改星期数
			if(my_date.which_day == 6)
			    my_date.which_day = 0;
			else if(my_date.which_day < 6)
			    my_date.which_day += 1;
            // 整点报时
			my_beep_time.on_the_hour = 1;
		}
        if(my_beep_time.is_beeping == 0){
            if(my_beep_time.hour == my_date.hour && my_beep_time.minute == my_date.minute && my_date.second == 0 ){
                //定时报时检测
                my_beep_time.is_beeping = 1;
            } 
        }
    }
}  

void main()
{
    beep = 1;
    key_init();
    lcd_init();
    Timer1Init();
	printf_str(1, 2, "Welcome!");
	delay(300);
    lcd_wcmd(0x01); //清除LCD的显示内容
	
	ET1 = 1; //enable timer1 interrupt
    EA = 1; //open global interrupt switch

    while(1)
    {
        /* 检测是否跳到菜单界面 */
        menu_list();
		if(count_times == 0){
            /* 如果日期时间有变就刷新显示， 显示效果更好 */
			display_now_date(); 

            /* 定时报时响 */
			if(my_beep_time.is_beeping == 0){
				/* 不响 */
				beep = 1;
				//printf_str(3, 2, "         ");
			} else if(my_beep_time.is_beeping == 1){
				/* 响 */
				beep = 0;
				//printf_str(3, 2, "wake up!");
			} 

            /* 整点报时响 */
            if(my_beep_time.on_the_hour == 1){
                if(hour_beep_times%2 == 1){
                    beep = 1;
                    hour_beep_times --;
                } else if(hour_beep_times == 0){
                    /* 停止响 */
                    my_beep_time.on_the_hour = 0;
                    hour_beep_times = 4;

                } else if(hour_beep_times%2 == 0){
                    beep = 0;
                    hour_beep_times --;
                } 
            }
		}
    }
}
/* Timer1Init */
void Timer1Init(void){
    TMOD = 0x10; // Timer1十六位计数方式

    TH1 = 0xdc; // 设置定时初值
    TL1 = 0x00;

    TR1 = 1; // 启动定时器
    // ET1 = 1;
	// EA = 1;

}
void display_now_date(void){
	char str[10] =" ";
    printf_str(0, 0, "当前日期时间");

    sprintf(str, "%04d", my_date.year);
    printf_str(1, 0, str);
    printf_str(1, 2, "年");
    
    sprintf(str, "%02d", my_date.mounth);
    printf_str(1, 3, str);
    printf_str(1, 4, "月");

    sprintf(str, "%02d", my_date.day);
    printf_str(1, 5, str);
	printf_str(1, 6, "日");

    sprintf(str, "%02d", my_date.hour);
    printf_str(2, 1, str);
    printf_str(2, 2, "时");
    
    
    sprintf(str, "%02d", my_date.minute);
    printf_str(2, 3, str);
    printf_str(2, 4, "分");

    sprintf(str, "%02d", my_date.second);
    printf_str(2, 5, str);
    printf_str(2, 6, "秒");

    sprintf(str, "%2d", my_date.which_day);
    printf_str(3, 6, str);
    printf_str(3, 4, "星期");
    
}
void change_now_date_time(){
    if(my_date.second < 59){
        my_date.second ++;
    } else if(my_date.second == 59) {
        my_date.second = 0;
    }
    if(my_date.second == 0 && my_date.minute < 59){
        my_date.minute ++;
    } else if(my_date.second == 0 && my_date.minute == 59){
        my_date.minute = 0;
    }
    if(my_date.second == 0 && my_date.minute == 0 && my_date.hour < 23){
        my_date.hour ++;
    } else if(my_date.second == 0 && my_date.minute == 0 && my_date.hour == 23){
        my_date.hour = 0;
    }
    if(my_date.second == 0 && my_date.minute == 0 && my_date.hour == 0 && my_date.day < my_date.mounth_days){
        my_date.day ++;
    } else if(my_date.second == 0 && my_date.minute == 0 && my_date.hour == 0 && my_date.day == my_date.mounth_days){
        my_date.day = 1;
    }
    if(my_date.second == 0 && my_date.minute == 0 && my_date.hour == 0 && my_date.day == 1 && my_date.mounth < 12){
        my_date.mounth ++;
    } else if(my_date.second == 0 && my_date.minute == 0 && my_date.hour == 0 && my_date.day == 1 && my_date.mounth == 12){
        my_date.mounth = 1;
    }
    if(my_date.second == 0 && my_date.minute == 0 && my_date.hour == 0 && my_date.day == 1 && my_date.mounth == 1){
        my_date.year ++;
    }
    
}





