#ifndef _LED_BTN_H_
#define _LED_BTN_H_
void led_init();
void led_on();
void led_off();
void led_toggle();
void btn_init(void (*on_pressed)(void));
#endif