#include <stdio.h>
#include "osapi.h"
#include "user_interface.h"
#include "led_btn.h"
#include "uart.h"

void btn_pressed()
{
	led_toggle();
}
void uart_received(char data)
{
  if(data == '0') {
    led_off();
    os_printf("LED off\n");
  } else if(data == '1') {
    led_on();
    os_printf("LED on\n");
  }
}
void app_init()
{
	led_init();
	btn_init(btn_pressed);
    uart_setup(uart_received);
}
void user_init(void)
{
    system_init_done_cb(app_init);
}
