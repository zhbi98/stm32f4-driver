
#include "tm1637.h"

unsigned char num_code[] = {0x3f, 0x06, 0x5b, 0x4f, 0x66,
                            0x6d, 0x7d, 0x07, 0x7f, 0x6f};

unsigned char twinkle_code[] = {0xbf, 0x86, 0xdb, 0xcf, 0xe6,
                                0xed, 0xfd, 0x87, 0xff, 0xef};

unsigned char twinkle = 0;

static void tm1637_sda_input()
{
    GPIO_InitTypeDef gpio_init;

    gpio_init.GPIO_Pin = TM1637_SDA_PIN;
    gpio_init.GPIO_Mode = GPIO_Mode_IN;
    gpio_init.GPIO_OType = GPIO_OType_OD;
    gpio_init.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(TM1637_SDA_GPIO, &gpio_init);
}

static void tm1637_sda_output()
{
    GPIO_InitTypeDef gpio_init;

    gpio_init.GPIO_Pin = TM1637_SDA_PIN;
    gpio_init.GPIO_Mode = GPIO_Mode_OUT;
    gpio_init.GPIO_OType = GPIO_OType_OD;
    gpio_init.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(TM1637_SDA_GPIO, &gpio_init);
}

void tm1637_gpio_init()
{
    GPIO_InitTypeDef gpio_init;

    RCC_AHB1PeriphClockCmd(TM1637_SCL_CLOCK, ENABLE);
    RCC_AHB1PeriphClockCmd(TM1637_SDA_CLOCK, ENABLE);

    gpio_init.GPIO_Pin = TM1637_SCL_PIN;
    gpio_init.GPIO_Mode = GPIO_Mode_OUT;
    gpio_init.GPIO_OType = GPIO_OType_PP;
    gpio_init.GPIO_PuPd = GPIO_PuPd_NOPULL;
    gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(TM1637_SCL_GPIO, &gpio_init);

    gpio_init.GPIO_Pin = TM1637_SDA_PIN;
    gpio_init.GPIO_Mode = GPIO_Mode_OUT;
    gpio_init.GPIO_OType = GPIO_OType_PP;
    gpio_init.GPIO_PuPd = GPIO_PuPd_NOPULL;
    gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(TM1637_SDA_GPIO, &gpio_init);
}

void tm1637_onoff(unsigned char state)
{
    if (state)
        GPIO_SetBits(TM1637_SDA_GPIO, TM1637_SDA_PIN);
    else
        GPIO_ResetBits(TM1637_SDA_GPIO, TM1637_SDA_PIN);

    GPIO_SetBits(TM1637_SCL_GPIO, TM1637_SCL_PIN);
    sleep_us(20);
    
    if (state)
        GPIO_ResetBits(TM1637_SDA_GPIO, TM1637_SDA_PIN);
    else
        GPIO_SetBits(TM1637_SDA_GPIO, TM1637_SDA_PIN);
    // sleep_us(20);
    GPIO_ResetBits(TM1637_SCL_GPIO, TM1637_SCL_PIN);
}

void check_tm1637_ack()
{
    unsigned char ack = 0;

    tm1637_sda_input();

    GPIO_SetBits(TM1637_SCL_GPIO, TM1637_SCL_PIN);
    sleep_us(20);
    while ((GPIO_ReadInputDataBit(TM1637_SDA_GPIO, TM1637_SDA_PIN) == 1) && (ack < 255)) {
        ack++;
    }
    GPIO_ResetBits(TM1637_SCL_GPIO, TM1637_SCL_PIN);
    sleep_us(20);

    tm1637_sda_output();
}

void tm1637_send_bit(unsigned char a_bit)
{
    GPIO_ResetBits(TM1637_SCL_GPIO, TM1637_SCL_PIN);
    sleep_us(20);

    if (a_bit)
        GPIO_SetBits(TM1637_SDA_GPIO, TM1637_SDA_PIN);
    else
        GPIO_ResetBits(TM1637_SDA_GPIO, TM1637_SDA_PIN);
    sleep_us(20);
    GPIO_SetBits(TM1637_SCL_GPIO, TM1637_SCL_PIN);
    sleep_us(20);
}

void tm1637_send_byte(unsigned char a_byte)
{
    unsigned char send;

    for (send = 0; send < 8; send++) {
        tm1637_send_bit((a_byte >> send) & 0x01);
    }
    GPIO_ResetBits(TM1637_SCL_GPIO, TM1637_SCL_PIN);
    sleep_us(20);
    GPIO_SetBits(TM1637_SDA_GPIO, TM1637_SDA_PIN);
    sleep_us(20);
    GPIO_SetBits(TM1637_SCL_GPIO, TM1637_SCL_PIN);
    sleep_us(20);
}

void write_command(unsigned char command)
{
	tm1637_onoff(TM1637_START);
	tm1637_send_byte(command);
	check_tm1637_ack();
	tm1637_onoff(TM1637_STOP);
}

void write_data(unsigned char address, unsigned char dat_a)
{
	tm1637_onoff(TM1637_START);
	tm1637_send_byte(address);
	check_tm1637_ack();
	tm1637_send_byte(dat_a);
	check_tm1637_ack();
	tm1637_onoff(TM1637_STOP);	
} 

void display_value(unsigned int value)
{
	unsigned char l;
	unsigned char bits[4];
	
	for (l = 0; l < 4; l++) {
		bits[l] = value % 10;
		value = value / 10;
	}
	write_command(0x44);
	write_data(0xc0, num_code[bits[3]]);
	write_data(0xc1, num_code[bits[2]]);
	write_data(0xc2, num_code[bits[1]]);
	write_data(0xc3, num_code[bits[0]]);	
	write_command(0x8a);	
}

void display_time(unsigned char h, unsigned m)
{
	hour.ten = h / 10;
	hour.bits = h % 10;

	minute.ten = m / 10;
	minute.bits = m % 10;

	write_command(0x44);
	write_data(0xc0, num_code[hour.ten]);
	if (twinkle) {
		write_data(0xc1, twinkle_code[hour.bits]);
	} else {
		write_data(0xc1, num_code[hour.bits]);
	}
	
	write_data(0xc200, num_code[minute.ten]);
	write_data(0xc3, num_code[minute.bits]);
	write_command(0x8a);
}

void display_off()
{
	write_command(0x44);
	write_data(0xc0, 0);
	write_data(0xc1, 0);
	write_data(0xc200, 0);
	write_data(0xc3, 0);
	write_command(0x8a);
}
