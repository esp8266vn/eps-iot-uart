#include "osapi.h"
#include "user_interface.h"
#include "gpio.h"
#define BTN_PIN 0 //GPIO0
static uint32_t led_value = 0;
void (*pressed_callback)(void) = NULL;
void key_intr_handler(void *args)
{
  uint32 gpio_status = GPIO_REG_READ(GPIO_STATUS_ADDRESS);
  if (gpio_status & BIT(BTN_PIN)) {
    if (pressed_callback)
      pressed_callback();
  }
  GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, BIT(BTN_PIN)); //clear interrupt status
}
void led_init()
{
  WRITE_PERI_REG(PAD_XPD_DCDC_CONF,
                 (READ_PERI_REG(PAD_XPD_DCDC_CONF) & 0xffffffbc) | (uint32_t)0x1);
  WRITE_PERI_REG(RTC_GPIO_CONF,
                 (READ_PERI_REG(RTC_GPIO_CONF) & (uint32_t)0xfffffffe) | (uint32_t)0x0);
  WRITE_PERI_REG(RTC_GPIO_ENABLE,
                 (READ_PERI_REG(RTC_GPIO_ENABLE) & (uint32_t)0xfffffffe) | (uint32_t)0x1);
}
void led_on()
{
  WRITE_PERI_REG(RTC_GPIO_OUT, (READ_PERI_REG(RTC_GPIO_OUT) & (uint32_t)0xfffffffe));
}
void led_off()
{
  WRITE_PERI_REG(RTC_GPIO_OUT, (READ_PERI_REG(RTC_GPIO_OUT) & (uint32_t)0xfffffffe) | 1);
}
void led_toggle()
{
  led_value ^= 1;
  if (led_value) led_off();
  else led_on();
}
void btn_init(void (*on_pressed)(void))
{
  ETS_GPIO_INTR_ATTACH(key_intr_handler, NULL);
  ETS_GPIO_INTR_DISABLE();
  PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO0_U, FUNC_GPIO0);
  gpio_output_set(0, 0, 0, GPIO_ID_PIN(BTN_PIN));
  gpio_register_set(GPIO_PIN_ADDR(BTN_PIN), GPIO_PIN_INT_TYPE_SET(GPIO_PIN_INTR_DISABLE)
                    | GPIO_PIN_PAD_DRIVER_SET(GPIO_PAD_DRIVER_DISABLE)
                    | GPIO_PIN_SOURCE_SET(GPIO_AS_PIN_SOURCE));
  GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, BIT(BTN_PIN));//clear interrupt status
  gpio_pin_intr_state_set(GPIO_ID_PIN(BTN_PIN), GPIO_PIN_INTR_NEGEDGE); //enable interrupt
  ETS_GPIO_INTR_ENABLE();
  pressed_callback = on_pressed;
}
