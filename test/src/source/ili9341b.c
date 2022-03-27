
#include "ili9341b.h"

static void ili9341_gpio_init()
{
    GPIO_InitTypeDef gpio_init;

    RCC_AHB1PeriphClockCmd(ILI9341_D0_CLOCK, ENABLE);
    RCC_AHB1PeriphClockCmd(ILI9341_D1_CLOCK, ENABLE);
    RCC_AHB1PeriphClockCmd(ILI9341_RS_CLOCK, ENABLE);
    RCC_AHB1PeriphClockCmd(ILI9341_DC_CLOCK, ENABLE);
    RCC_AHB1PeriphClockCmd(ILI9341_CS_CLOCK, ENABLE);

    gpio_init.GPIO_Pin = ILI9341_D0_PIN;
    gpio_init.GPIO_Mode = GPIO_Mode_OUT;
    gpio_init.GPIO_OType = GPIO_OType_PP;
    gpio_init.GPIO_PuPd = GPIO_PuPd_UP;
    gpio_init.GPIO_Speed = GPIO_Speed_25MHz;
    GPIO_Init(ILI9341_D0_GPIO, &gpio_init);

    gpio_init.GPIO_Pin = ILI9341_D1_PIN;
    gpio_init.GPIO_Mode = GPIO_Mode_OUT;
    gpio_init.GPIO_OType = GPIO_OType_PP;
    gpio_init.GPIO_PuPd = GPIO_PuPd_UP;
    gpio_init.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(ILI9341_D1_GPIO, &gpio_init);

    gpio_init.GPIO_Pin = ILI9341_RS_PIN;
    gpio_init.GPIO_Mode = GPIO_Mode_OUT;
    gpio_init.GPIO_OType = GPIO_OType_PP;
    gpio_init.GPIO_PuPd = GPIO_PuPd_UP;
    gpio_init.GPIO_Speed = GPIO_Speed_25MHz;
    GPIO_Init(ILI9341_RS_GPIO, &gpio_init);

    gpio_init.GPIO_Pin = ILI9341_DC_PIN;
    gpio_init.GPIO_Mode = GPIO_Mode_OUT;
    gpio_init.GPIO_OType = GPIO_OType_PP;
    gpio_init.GPIO_PuPd = GPIO_PuPd_UP;
    gpio_init.GPIO_Speed = GPIO_Speed_25MHz;
    GPIO_Init(ILI9341_DC_GPIO, &gpio_init);

    gpio_init.GPIO_Pin = ILI9341_CS_PIN;
    gpio_init.GPIO_Mode = GPIO_Mode_OUT;
    gpio_init.GPIO_OType = GPIO_OType_PP;
    gpio_init.GPIO_PuPd = GPIO_PuPd_UP;
    gpio_init.GPIO_Speed = GPIO_Speed_25MHz;
    GPIO_Init(ILI9341_CS_GPIO, &gpio_init);
}

void ili9341_send_bit(unsigned char a_bit)
{
    if (a_bit)
        GPIO_SetBits(ILI9341_D1_GPIO, ILI9341_D1_PIN);
    else
        GPIO_ResetBits(ILI9341_D1_GPIO, ILI9341_D1_PIN);
}

void ili9341_send_byte(unsigned char a_byte)
{
    unsigned char write;

    GPIO_ResetBits(ILI9341_CS_GPIO, ILI9341_CS_PIN);
    for (write = 0; write < 8; write++) {
        GPIO_ResetBits(ILI9341_D0_GPIO, ILI9341_D0_PIN);
        ili9341_send_bit((a_byte << write) & 0x80);
        // sleep_us(1);
        GPIO_SetBits(ILI9341_D0_GPIO, ILI9341_D0_PIN);
    }
    GPIO_SetBits(ILI9341_CS_GPIO, ILI9341_CS_PIN);
}

void ili9341_write_command(unsigned char command)
{
    GPIO_ResetBits(ILI9341_DC_GPIO, ILI9341_DC_PIN);
    ili9341_send_byte(command);
}

void ili9341_write_data(unsigned char dat_a)
{
    GPIO_SetBits(ILI9341_DC_GPIO, ILI9341_DC_PIN);
    ili9341_send_byte(dat_a);
}

void ili9341_write_datas(unsigned int dat_a)
{
    GPIO_SetBits(ILI9341_DC_GPIO, ILI9341_DC_PIN);
    ili9341_send_byte((dat_a & 0xff00) >> 8);
    ili9341_send_byte(dat_a);
}


void ili9341_reset()
{
    GPIO_SetBits(ILI9341_RS_GPIO, ILI9341_RS_PIN);
    sleep_ms(50);
    GPIO_ResetBits(ILI9341_RS_GPIO, ILI9341_RS_PIN);      
    sleep_ms(50);
    GPIO_SetBits(ILI9341_RS_GPIO, ILI9341_RS_PIN);
    sleep_ms(50);
}

void ili9341_direction(unsigned char direction)
{
    ili9341_write_command(0x36);

    switch (direction) {
        // +----- x 
        // |
        // |
        // y              horizontal
        case 0 : ili9341_write_data(0xa8); break;
        // y -----+
        //        |
        //        |
        //        x       vertical
        case 1 : ili9341_write_data(0x08); break;
        //        y
        //        |
        //        |
        // x -----+ 0x08  horizontal
        case 2 : ili9341_write_data(0x68); break;
        // x
        // |
        // |
        // +----- y       vertical
        case 3 : ili9341_write_data(0xc8); break;
    }
}

void ili9341_init()
{
    ili9341_gpio_init();
    sleep_ms(100);
    ili9341_reset();

    ili9341_write_command(0xcf);
    ili9341_write_data(0x00);
    ili9341_write_data(0x81);
    ili9341_write_data(0x30);
    
    ili9341_write_command(0xed);
    ili9341_write_data(0x64);
    ili9341_write_data(0x03);
    ili9341_write_data(0x12);
    ili9341_write_data(0x81);
    
    ili9341_write_command(0xe8);
    ili9341_write_data(0x85);
    ili9341_write_data(0x10);
    ili9341_write_data(0x78);
    
    ili9341_write_command(0xcb);
    ili9341_write_data(0x39);
    ili9341_write_data(0x2c);
    ili9341_write_data(0x00);
    ili9341_write_data(0x34);
    ili9341_write_data(0x02);
    
    ili9341_write_command(0xf7);
    ili9341_write_data(0x20);
    
    ili9341_write_command(0xea);
    ili9341_write_data(0x00);
    ili9341_write_data(0x00);
    
    ili9341_write_command(0xb1);
    ili9341_write_data(0x00);
    ili9341_write_data(0x1b);
    
    ili9341_write_command(0xb6);
    ili9341_write_data(0x0a);
    ili9341_write_data(0xa2);
    
    ili9341_write_command(0xc0);
    ili9341_write_data(0x35);

    ili9341_write_command(0xc1);
    ili9341_write_data(0x11);

    ili9341_write_command(0xc5);
    ili9341_write_data(0x45);
    ili9341_write_data(0x45);
    
    ili9341_write_command(0xc7);
    ili9341_write_data(0xa2);
    
    ili9341_write_command(0xf2);
    ili9341_write_data(0x00);
    
    ili9341_write_command(0x26);
    ili9341_write_data(0x01);

    ili9341_write_command(0xe0);
    ili9341_write_data(0x0f);
    ili9341_write_data(0x26);
    ili9341_write_data(0x24);
    ili9341_write_data(0x0b);
    ili9341_write_data(0x0e);
    ili9341_write_data(0x09);
    ili9341_write_data(0x54);
    ili9341_write_data(0xA8);
    ili9341_write_data(0x46);
    ili9341_write_data(0x0c);
    ili9341_write_data(0x17);
    ili9341_write_data(0x09);
    ili9341_write_data(0x0f);
    ili9341_write_data(0x07);
    ili9341_write_data(0x00);
    
    ili9341_write_command(0Xe1);
    ili9341_write_data(0x00);
    ili9341_write_data(0x19);
    ili9341_write_data(0x1b);
    ili9341_write_data(0x04);
    ili9341_write_data(0x10);
    ili9341_write_data(0x07);
    ili9341_write_data(0x2a);
    ili9341_write_data(0x47);
    ili9341_write_data(0x39);
    ili9341_write_data(0x03);
    ili9341_write_data(0x06);
    ili9341_write_data(0x06);
    ili9341_write_data(0x30);
    ili9341_write_data(0x38);
    ili9341_write_data(0x0f);
    // direction seting
    // ili9341_write_command(0x36);
    // ili9341_write_data(0xc8);
    if (HORIZONTAL)
        ili9341_direction(2);
    else
        ili9341_direction(3);

    ili9341_write_command(0x2a);
    ili9341_write_data(0x00);
    ili9341_write_data(0x00);
    ili9341_write_data(0x00);
    ili9341_write_data(0xef);
 
    ili9341_write_command(0x2b);
    ili9341_write_data(0x00);
    ili9341_write_data(0x00);
    ili9341_write_data(0x01);
    ili9341_write_data(0x3f);
    
    ili9341_write_command(0x3a);
    ili9341_write_data(0x55);
    
    ili9341_write_command(0x11);
    sleep_ms(120);   
    ili9341_write_command(0x29);
    ili9341_clear(white);
}

void display_position(unsigned int y, unsigned int x)
{
    unsigned char seting_x = 0x2a;
    unsigned char seting_y = 0x2b;

    y = y - 1;
    x = x - 1;

    ili9341_write_command(seting_x);
    ili9341_write_data(x >> 8);
    ili9341_write_data(0x00ff & x);
    ili9341_write_data((x + 1) >> 8);
    ili9341_write_data((x + 1));
    
    ili9341_write_command(seting_y);
    ili9341_write_data(y >> 8);
    ili9341_write_data(0x00ff & y);
    ili9341_write_data((y + 1) >> 8);
    ili9341_write_data((y + 1));
}

void ili9341_display_region(unsigned int y, unsigned int x, unsigned int width, unsigned int height)
{
    ili9341_write_command(0x2a);
    ili9341_write_data(x >> 8);
    ili9341_write_data(x & 0xff);
    ili9341_write_data((x + width - 1) >> 8);
    ili9341_write_data((x + width - 1) & 0xff);

    ili9341_write_command(0x2b);
    ili9341_write_data(y >> 8);
    ili9341_write_data(y & 0xff);
    ili9341_write_data((y + height - 1) >> 8);
    ili9341_write_data((y + height - 1) & 0xff);
}

void display_pixel(unsigned int y, unsigned int x, unsigned int color)
{
    display_position(y, x);
    // display pixel
    ili9341_write_command(0x2c);
    // sleep_us(1);
    ili9341_write_datas(color);
}

void ili9341_clear(unsigned int color)
{
    unsigned int x;
    unsigned int y;

    for (y = 1; y < TFT_HEIGH; y++) {
        for (x = 1; x < TFT_WIDTH; x++) {
            display_pixel(y, x, color);
        }
    }
}

void display_character(unsigned int y, unsigned int x, unsigned int bg, unsigned int fg, unsigned char character)
{
    unsigned char font_h;
    unsigned char font_w;
    unsigned char font_c[16];

    character = character - ' ';

    x = (x * 8) - 8 + 1;
    y = (y * 16) - 16 + 1;

    for (font_h = 0; font_h < 16; font_h++)
        font_c[font_h] = black8x16[16 * character + font_h];

    for (font_h = 0; font_h < 16; font_h++) {
        for (font_w = 0; font_w < 8; font_w++) {
            if (font_c[font_h] & (0x80 >> font_w))
                display_pixel(y + font_h, x + font_w, fg);
            else
                display_pixel(y + font_h, x + font_w, bg);
        }
    }
}

void display_character_16x32(unsigned int y, unsigned int x, unsigned int bg, unsigned int fg, unsigned char character)
{
    unsigned char font_h;
    unsigned char font_w;
    unsigned int byte;
    unsigned char bits = 0;
    unsigned char font_c[64];

    character = character - ' ';

    x = (x * 16) - 16 + 1;
    y = (y * 32) - 32 + 1;

    for (font_h = 0; font_h < 64; font_h++)
        font_c[font_h] = black16x32[64 * character + font_h];

    for (font_h = 0; font_h < 32; font_h++) {

        byte = font_c[bits];
        byte = byte << 8;
        byte = byte | font_c[bits + 1];
        bits = bits + 2;

        for (font_w = 0; font_w < 16; font_w++) {
            if (byte & (0x8000 >> font_w))
                display_pixel(y + font_h, x + font_w, fg);
            else
                display_pixel(y + font_h, x + font_w, bg);
        }
    }
}

void display_string(unsigned int y, unsigned int x, unsigned int bg, unsigned int fg, unsigned char *string)
{
    unsigned char bits = 0;

    while (string[bits] != '\0') {
        display_character(y, x, bg, fg, string[bits]);

        sleep_ms(10);
        bits++;
        x++;
    }
}

void display_string_16x32(unsigned int y, unsigned int x, unsigned int bg, unsigned int fg, unsigned char *string)
{
    unsigned char bits = 0;

    while (string[bits] != '\0') {
        display_character_16x32(y, x, bg, fg, string[bits]);

        sleep_ms(10);
        bits++;
        x++;
    }
}

void display_value(unsigned int y, unsigned int x, unsigned int bg, unsigned int fg, unsigned char *format, ...)
{
    unsigned char value[16];

    va_list parameter_pointer;
    va_start(parameter_pointer, format);

    vsprintf(value, format, parameter_pointer);

    va_end(parameter_pointer);

    display_string(y, x, bg, fg, value);
}

void display_value_16x32(unsigned int y, unsigned int x, unsigned int bg, unsigned int fg, unsigned char *format, ...)
{
    unsigned char value[16];

    va_list parameter_pointer;
    va_start(parameter_pointer, format);

    vsprintf(value, format, parameter_pointer);
    
    va_end(parameter_pointer);

    display_string_16x32(y, x, bg, fg, value);
}
