#include <intrins.h>
#include <stdio.h>
#include "lcd.h"
#include "menu.h"

enum my_enum1{
    UP = 2, //
    DOWN = 3 //
};

struct date my_date = {
    /*  
        ���꣺��4�ı���������100�ı��� ����400�ı���
        ����2��29�� ƽ��2��28��
     */
    2020, // 2020Ϊ����
    12,
    31,
	
    23,
    59,
    53,
	
    1, // is_leap_year = 1
    31 // ����2��29��
};
struct beep_time my_beep_time = {
	1,
	0,
    0, // is_beeping = 0;
    0 //����:on_the_hour = 1
};
sbit key_break = P3^5;
sbit value_up = P3^4;
sbit value_down = P3^3;
sbit key_star = P3^2;

char str[10] = " ";
uint8_t star_postion = 1;

void key_init(){
    key_break = 1;
    value_up = 1;
    value_down = 1;
    key_star = 1;
}
uint8_t key_scan(bit key){
    if (key == 0)
    {
        /* code */
        delay(80);
        if(key == 0)
            return 0;
    }
    return 1;
}
void refresh_display(void){
    sprintf(str, "%04d", my_date.year);
    printf_str(1, 1, str);
    sprintf(str, "%02d", my_date.mounth);
    printf_str(2, 1, str);
    sprintf(str, "%02d", my_date.day);
    printf_str(3, 1, str);

    sprintf(str, "%02d", my_date.hour);
    printf_str(1, 5, str);
    sprintf(str, "%02d", my_date.minute);
    printf_str(2, 5, str);
    sprintf(str, "%02d", my_date.second);
    printf_str(3, 5, str);
}
/* �·������ж� */
void set_mounth_days(void){
    /* 
        mounth_flag = 1: ���� 1 3 5 7 8 10 12
        2��2�� 
        3��С�� 4 6 9 11
     */
    uint8_t mounth_flag = 0;
    if(my_date.mounth <= 7){
        if(my_date.mounth % 2 == 1){
            /* ���� */
            mounth_flag = 1;
        } else if(my_date.mounth == 2) {
            /* ���� */
            mounth_flag = 2;
        } else {
            /* С�� */
            mounth_flag = 3;
        }
    } else {
        if(my_date.mounth % 2 == 1){
            /* С�� */
            mounth_flag = 3;
        } else {
            /* ���� */
            mounth_flag = 1;
        }
    }
    switch (mounth_flag)
    {
        case 1:
        /* ���� */
            my_date.mounth_days = 31;
            break;
        case 2:
        /* ���� */
        if(my_date.is_leap_year == 1){ //����2��29�� ƽ��2��28��
            /* ���� */
            my_date.mounth_days = 29;
        } else {
            /* ƽ�� */
            my_date.mounth_days = 28;
        }
            break;
        case 3:
        /* С�� */
            my_date.mounth_days = 30;
            break;
        default:
            break;
    }
}
void set_date(void){
	printf_str(0, 0, "�޸�ʱ��");
    printf_str(1, 0, "��");
    sprintf(str, "%04d", my_date.year);
    printf_str(1, 1, str);
    printf_str(2, 0, "��");
    sprintf(str, "%02d", my_date.mounth);
    printf_str(2, 1, str);
    printf_str(3, 0, "��");
    sprintf(str, "%02d", my_date.day);
    printf_str(3, 1, str);
	
    printf_str(1, 4, "ʱ");
    sprintf(str, "%02d", my_date.hour);
    printf_str(1, 5, str);
    printf_str(2, 4, "��");
    sprintf(str, "%02d", my_date.minute);
    printf_str(2, 5, str);
    printf_str(3, 4, "��");
    sprintf(str, "%02d", my_date.second);
    printf_str(3, 5, str);
	
	printf_str(star_postion, 3, "*");
	
	star_postion = 1;
    while (1)
    {
        /* code */
        /* ��star * ��λ�� */
        if (key_scan(key_star) == 0){
            if(star_postion <= 3){
                printf_str(star_postion, 3, " ");
            } else if(star_postion >= 4){
                printf_str(star_postion-3, 6, " ");
            }
            if (star_postion < 6)
                star_postion ++;
			else if (star_postion == 6)
                star_postion = 1;
            //printf_str(star_postion, 3, "*");
            if(star_postion <= 3){
                printf_str(star_postion, 3, "*");
            } else if(star_postion >= 4){
                printf_str(star_postion-3, 6, "*");
            }
        }
        
        /* �������� ʱ������ֵ */
        if (key_scan(value_up) == 0){
            switch (star_postion)
            {
                /* ������ */
                case 1:
                    /* year */
                    my_date.year ++;
                    /*  
                        ���꣺��4�ı���������100�ı��� ����400�ı���
                        ����2��29�� ƽ��2��28��
                    */
                    if((my_date.year%4 == 0 && my_date.year%100 != 0)|| my_date.year%400 == 0){
                        my_date.is_leap_year = 1;
                    } else {
                        my_date.is_leap_year =0;
                    }
                    break;
                case 2:
                    /* mounth */
                    if(my_date.mounth == 12){
                        my_date.mounth = 1;
                    } else if(my_date.mounth < 12){
                        my_date.mounth ++;
                    }
                    /* ���·ݽ����жϣ�ȷ��ÿ�µ����� */
                    set_mounth_days();
                    break;
                case 3:
                    /* day */
                    
                    if(my_date.day == my_date.mounth_days){
                        my_date.day = 1;
                    } else if(my_date.day < my_date.mounth_days){
                        my_date.day ++;
                    }
                    break;
                /* ʱ���� */
                case 4:
                    /* hour */
                    if(my_date.hour == 23){
                        my_date.hour = 0;
                    } else if(my_date.hour < 23){
                        my_date.hour ++;
                    }
                    break;
                case 5:
                    /* minute */
                    if(my_date.minute == 59){
                        my_date.minute = 0;
                    } else if(my_date.minute < 59){
                        my_date.minute ++;
                    }
                    break;
                case 6:
                    /* second */
                    if(my_date.second == 59){
                        my_date.second = 0;
                    } else if(my_date.second < 59){
                        my_date.second ++;
                    }
                    break;
                default:
                    break;
            }
            refresh_display();
        } else if(key_scan(value_down) == 0){
            switch (star_postion)
            {
                case 1:
                    /* code */
                    if(my_date.year == 0){
                        my_date.year = 2020;
                    }
                    else{
                        my_date.year --;
                    }
                    if((my_date.year%4 == 0 && my_date.year%100 != 0)|| my_date.year%400 == 0){
                        my_date.is_leap_year = 1;
                    } else {
                        my_date.is_leap_year =0;
                    }
                    break;
                case 2:
                    if(my_date.mounth == 1){
                        my_date.mounth = 12;
                    }
                    else{
                        my_date.mounth --;
                    }
                    set_mounth_days();
                    break;
                case 3:
                    if(my_date.day == 1){
                        my_date.day = my_date.mounth_days;
                    }
                    else{
                        my_date.day --;
                    }
                    break;
                    /* ʱ���� */
                case 4:
                    /* hour */
                    if(my_date.hour > 0){
                        my_date.hour --;
                    } else if(my_date.hour == 0){
                        my_date.hour = 23;
                    }
                    break;
                case 5:
                    /* minute */
                    if(my_date.minute > 0){
                        my_date.minute --;
                    }
                    else if(my_date.minute == 0){
                        my_date.minute = 59;
                    } 
                    break;
                case 6:
                    /* second */
                    if(my_date.second > 0){
                        my_date.second --;
                    } else if(my_date.second == 0){
                        my_date.second = 59;
                    }
                    break;
                default:
                    break;
            }
            refresh_display();
        }
       if (key_scan(key_break) == 0){
           break;
       }
    }
	// ET0 = 1; //enable timer1 interrupt
    // EA = 1; //open global interrupt switch
}
void beep_time(){
    printf_str(0, 0, "��ʱ����");
    
    printf_str(1, 2, "ʱ");
    sprintf(str, "%02d", my_beep_time.hour);
    printf_str(1, 5, str);
    printf_str(2, 2, "��");
    sprintf(str, "%02d", my_beep_time.minute);
    printf_str(2, 5, str);
	
	star_postion = 1;
    printf_str(star_postion, 6, "*");
    while (1)
    {
        /* code */
		/* ��star * ��λ�� */
        if (key_scan(key_star) == 0){
            printf_str(star_postion, 6, " ");
            if (star_postion == 1)
                star_postion ++;
			else if (star_postion == 2)
                star_postion = 1;
            printf_str(star_postion, 6, "*");
        }
        /* ��ֵ */
        if (key_scan(value_up) == 0){
            switch (star_postion)
            {
                case 1:
                    /* hour */
                    if(my_beep_time.hour == 23){
                        my_beep_time.hour = 0;
                    } else if(my_beep_time.hour < 23){
                        my_beep_time.hour ++;
                    }
                    break;
                case 2:
                /* minute */
                    if(my_beep_time.minute == 59){
                        my_beep_time.minute = 0;
                    } else if(my_beep_time.minute < 59){
                        my_beep_time.minute ++;
                    }
                    break;
                default:
                    break;
            }
            /*  */
            sprintf(str, "%02d", my_beep_time.hour);
            printf_str(1, 5, str);
            sprintf(str, "%02d", my_beep_time.minute);
            printf_str(2, 5, str);
        } else if(key_scan(value_down) == 0) {
            switch (star_postion)
            {
                case 1:
                    if(my_beep_time.hour > 0){
                        my_beep_time.hour --;
                    } else if(my_beep_time.hour == 0){
                        my_beep_time.hour = 23;
                    }
                    break;
                case 2:
                    /* minute */
                    if(my_beep_time.minute > 0){
                        my_beep_time.minute --;
                    }
                    else if(my_beep_time.minute == 0){
                        my_beep_time.minute = 59;
                    } 
                    break;
                default:
                    break;
            }
            /*  */
            sprintf(str, "%02d", my_beep_time.hour);
            printf_str(1, 5, str);
            sprintf(str, "%02d", my_beep_time.minute);
            printf_str(2, 5, str);
        }
        if (key_scan(key_break) == 0){
           break;
       }
    }
	ET0 = 1; //enable timer1 interrupt
    EA = 1; //open global interrupt switch
}
void menu_list(){
    if(key_scan(key_break) == 0){
            ET0 = 0;        //enable timer1 interrupt
			EA = 0;          //open global interrupt switch
			lcd_wcmd(0x01);      //���LCD����ʾ����
            set_date();
			lcd_wcmd(0x01);      //���LCD����ʾ����
            
            /* ��ʱ���� */
            lcd_wcmd(0x01);      //���LCD����ʾ����
            beep_time();
            lcd_wcmd(0x01);      //���LCD����ʾ����
            //printf_str(0, 2, "��ǰ����ʱ��");
    } else if(key_scan(value_up) == 0){
        /* �ڶ�����������ʱbeepֹͣ */
        my_beep_time.is_beeping = 0;
    }
}



