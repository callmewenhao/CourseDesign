#include <reg52.h>

#ifndef USER_MENU_H_
#define USER_MENU_H_

#define uint8_t unsigned char
#define uint16_t unsigned int 

struct date
{
    /* data */
    uint16_t year;
    uint16_t mounth;
    uint16_t day;
    
    uint16_t hour;
    uint16_t minute;
    uint16_t second;

    uint8_t is_leap_year; /* yes:1, no:0 */
    uint16_t mounth_days;
};
struct beep_time
{
    /* data */
    uint16_t hour;
    uint16_t minute;
    uint8_t is_beeping;
    uint8_t on_the_hour;
};
void key_init();
void set_date(void);
uint8_t key_scan(bit key);
void menu_list();
void beep_time();
#endif /* USER_MENU_H_ */

