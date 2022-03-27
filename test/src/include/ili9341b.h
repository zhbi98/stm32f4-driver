
#ifndef __ILI9341B_H__
#define __ILI9341B_H__

#include <stdio.h>
#include <stdarg.h>

#include "stm32f4xx.h"

#include "fonts.h"
#include "time.h"

// sclk
#define ILI9341_D0_GPIO  GPIOC
#define ILI9341_D0_PIN   GPIO_Pin_4
#define ILI9341_D0_CLOCK RCC_AHB1Periph_GPIOC
// mosi
#define ILI9341_D1_GPIO  GPIOA
#define ILI9341_D1_PIN   GPIO_Pin_6
#define ILI9341_D1_CLOCK RCC_AHB1Periph_GPIOA
// rest
#define ILI9341_RS_GPIO  GPIOA
#define ILI9341_RS_PIN   GPIO_Pin_2
#define ILI9341_RS_CLOCK RCC_AHB1Periph_GPIOA
// d::c
#define ILI9341_DC_GPIO  GPIOA
#define ILI9341_DC_PIN   GPIO_Pin_4
#define ILI9341_DC_CLOCK RCC_AHB1Periph_GPIOA
// cs
#define ILI9341_CS_GPIO  GPIOA
#define ILI9341_CS_PIN   GPIO_Pin_0
#define ILI9341_CS_CLOCK RCC_AHB1Periph_GPIOA

#define HORIZONTAL        0

#if (HORIZONTAL)
    #define TFT_WIDTH     (320 + 1)
    #define TFT_HEIGH     (240 + 1)
#else
    #define TFT_WIDTH     (240 + 1)
    #define TFT_HEIGH     (320 + 1)
#endif

enum color {
    white       = 0xffff,
    black       = 0x0000,

    red         = 0xf800,
    green       = 0x07e0,
    blue        = 0x001f,

    purple      = 0x780f,
    yellow      = 0xffe0,
    orange      = 0xfd20,
    orange_red  = 0xfa20,
    sun_orange  = 0xfb80,
    strong_red  = 0xe000,
    hot_pink    = 0xf8b2,
    misty_rose  = 0xff3c,
    sea_green   = 0x2c4a,
    apple_green = 0x8f20,
    tomato      = 0xfb08,
    brown       = 0xa145,
    olive       = 0x8400,
    cyan        = 0x07ff,
    dimcyan     = 0x03ff,
    dimgray     = 0x7bef,
    light_gray  = 0xc618,
    magenta     = 0xf81f
};

static void oled096_gpio_init();

extern void ili9341_send_bit(unsigned char a_bit);
extern void ili9341_send_byte(unsigned char a_byte);
extern void ili9341_write_command(unsigned  char command);
extern void ili9341_write_data(unsigned  char dat_a);
extern void ili9341_reset();
extern void ili9341_direction(unsigned char direction);
extern void ili9341_init();
extern void display_position(unsigned int y, unsigned int x);
extern void ili9341_display_region(unsigned int y, unsigned int x, unsigned int width, unsigned int height);
extern void display_pixel(unsigned int y, unsigned int x, unsigned int color);
extern void ili9341_clear(unsigned int color);
extern void display_character(unsigned int y, unsigned int x, unsigned int bg, unsigned int fg, unsigned char character);
extern void display_character_16x32(unsigned int y, unsigned int x, unsigned int bg, unsigned int fg, unsigned char character);
extern void display_string(unsigned int y, unsigned int x, unsigned int bg, unsigned int fg, unsigned char *string);
extern void display_string_16x32(unsigned int y, unsigned int x, unsigned int bg, unsigned int fg, unsigned char *string);
extern void display_value(unsigned int y, unsigned int x, unsigned int bg, unsigned int fg, unsigned char *format, ...);
extern void display_value_16x32(unsigned int y, unsigned int x, unsigned int bg, unsigned int fg, unsigned char *format, ...);

#endif
