
#ifndef __LED_H__
#define __LED_H__

#include "stm32f4xx.h"
#include "time.h"

#define LED1_GPIO  GPIOA
#define LED1_PIN   GPIO_Pin_6
#define LED1_CLOCK RCC_AHB1Periph_GPIOA

#define LED2_GPIO  GPIOA
#define LED2_PIN   GPIO_Pin_7
#define LED2_CLOCK RCC_AHB1Periph_GPIOA

#define LED1_H()   GPIO_SetBits(LED1_GPIO, LED1_PIN)
#define LED1_L()   GPIO_ResetBits(LED1_GPIO, LED1_PIN)

#define LED2_H()   GPIO_SetBits(LED2_GPIO, LED2_PIN)
#define LED2_L()   GPIO_ResetBits(LED1_GPIO, LED1_PIN)

struct led_display_t {
    unsigned int const_high_time;
    unsigned int const_low_time;

    unsigned int display_time;
    unsigned char display_status;
};

extern struct led_display_t led1_display;
extern struct led_display_t led2_display;

extern void led1_gpio_init();
extern void led2_gpio_init();

extern void led1_twink(unsigned int times);
extern void led2_twink(unsigned int times);

static void led1_breath_period(unsigned int period, unsigned int time_l);
extern void led1_breath();
static void led2_breath_period(unsigned int period, unsigned int time_l);
extern void led2_breath();

extern void led1_event_timing();
extern void led1_display_running();
extern void led2_event_timing();
extern void led2_display_running();

#endif
