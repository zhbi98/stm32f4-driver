
#include "key.h"

void key_gpio_init()
{
    GPIO_InitTypeDef gpio_init;

    RCC_AHB1PeriphClockCmd(KEY1_CLOCK, ENABLE);
    RCC_AHB1PeriphClockCmd(KEY2_CLOCK, ENABLE);

    gpio_init.GPIO_Pin = KEY1_PIN;
    gpio_init.GPIO_Mode = GPIO_Mode_IN;
    gpio_init.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(KEY1_GPIO, &gpio_init);

    gpio_init.GPIO_Pin = KEY2_PIN;
    gpio_init.GPIO_Mode = GPIO_Mode_IN;
    gpio_init.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(KEY2_GPIO, &gpio_init);
}

unsigned char read_key1()
{
    if (!KEY1_STATUS())
        return 0;
    else
        return 1;
}

unsigned char read_key2()
{
    if (!KEY2_STATUS())
        return 0;
    else
        return 1;
}

struct key_event_t key_evt = {
    .press_long = false,
    .press_duration = 500,
    .active_time = 50,
};

void key_event_timing()
{
    if (key_evt.press_duration != 0)
        key_evt.press_duration--;

    if (key_evt.active_time != 0)
        key_evt.active_time--;
}

unsigned char read_key_event()
{
    /**
     * [0011, 1111], <k1, k2, 1, 1, 1, 1, 1, 1>
     * Key follow the code:
     * Key <1> <0x7f>
     * Key <2> <0xbf>
     */

    unsigned char t = 0;
    unsigned char k = 0;
    static unsigned char plast = RELEASE;

    t = (t << 1) | read_key1();
    t = (t << 1) | read_key2();
    t = (t << 1) | (0x01);
    t = (t << 1) | (0x01);
    t = (t << 1) | (0x01);
    t = (t << 1) | (0x01);
    t = (t << 1) | (0x01);
    t = (t << 1) | (0x01);

    switch (t) {
        case 0x7f:
            k = KEY1_EVT;
            break;
        case 0xbf:
            k = KEY2_EVT;
            break;
        default:
            k = RELEASE;
            break;
    }

    if (k == RELEASE) {
        key_evt.press_duration = 1000;
    }

    // key 2 unsupport long press auto trigger active  
    if ((key_evt.press_duration <= 0)/* && (k != KEY2_EVT)*/) {
        key_evt.press_long = true;
    } else {
        key_evt.press_long = false;
    }

    key_event_timing();

    if (key_evt.press_long == true) {
        if (key_evt.active_time <= 0) {
            key_evt.active_time = 100;
            return k;
        } else {
            return RELEASE;
        }
    }

    // Pressed event
    // if ((k != RELEASE) && (plast == RELEASE)) {
    //     plast = k;
    //     return k;
    // } else {
    //     plast = k;
    //     return RELEASE;
    // }

    // Released event
    if ((k == RELEASE) && (plast != RELEASE)) {
        t = plast;
        plast = RELEASE;
        return t;
    } else {
        plast = k;
        return RELEASE;
    }
}

void key_event_doing()
{
    unsigned char event = 0x00;

    event = read_key_event();

    switch (event) {
        case KEY1_EVT:
            break;
        case KEY2_EVT:
            break;
    }
}
