
#ifndef __KEY_H__
#define __KEY_H__

#include <stdbool.h>

#include "stm32f4xx.h"

#define KEY1_GPIO     GPIOA
#define KEY1_PIN      GPIO_Pin_1
#define KEY1_CLOCK    RCC_AHB1Periph_GPIOA

#define KEY2_GPIO     GPIOA
#define KEY2_PIN      GPIO_Pin_2
#define KEY2_CLOCK    RCC_AHB1Periph_GPIOA

#define KEY1_STATUS() GPIO_ReadInputDataBit(KEY1_GPIO, KEY1_PIN)
#define KEY2_STATUS() GPIO_ReadInputDataBit(KEY2_GPIO, KEY2_PIN)

#define KEY1_EVT ('1')
#define KEY2_EVT ('2')
#define RELEASE  ('0')

struct key_event_t {
    // false: short pressed event
    // true : long pressed event
    bool press_long;

    // duration pressed time
    unsigned int press_duration;

    // pressed active speed
    unsigned int active_time;
};

extern struct key_event_t key_evt;

extern void key_gpio_init();
extern unsigned char read_key1();
extern unsigned char read_key2();
extern void key_event_timing();
extern unsigned char read_key_event();
extern void key_event_doing();

#endif
