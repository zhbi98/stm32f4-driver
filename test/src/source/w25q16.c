
#include "w25q16.h"

unsigned char w25q16_data[32] = {0};

static void w25q16_gpio_init()
{
    GPIO_InitTypeDef gpio_init;

    RCC_AHB1PeriphClockCmd(W25Q16_MISO_CLOCK | W25Q16_MOSI_CLOCK, ENABLE);
    RCC_AHB1PeriphClockCmd(W25Q16_SCLK_CLOCK | W25Q16_CS_CLOCK, ENABLE);

    GPIO_PinAFConfig(W25Q16_MISO_GPIO, W25Q16_MISO_SRC, W25Q16_MISO_AF);
    GPIO_PinAFConfig(W25Q16_MOSI_GPIO, W25Q16_MOSI_SRC, W25Q16_MOSI_AF);
    GPIO_PinAFConfig(W25Q16_SCLK_GPIO, W25Q16_SCLK_SRC, W25Q16_SCLK_AF);

    gpio_init.GPIO_Pin = W25Q16_MISO_PIN;
    gpio_init.GPIO_Mode = GPIO_Mode_AF;
    gpio_init.GPIO_OType = GPIO_OType_PP;
    gpio_init.GPIO_PuPd = GPIO_PuPd_NOPULL;
    gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(W25Q16_MISO_GPIO, &gpio_init);

    gpio_init.GPIO_Pin = W25Q16_MOSI_PIN;
    gpio_init.GPIO_Mode = GPIO_Mode_AF;
    gpio_init.GPIO_OType = GPIO_OType_PP;
    gpio_init.GPIO_PuPd = GPIO_PuPd_NOPULL;
    gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(W25Q16_MOSI_GPIO, &gpio_init);

    gpio_init.GPIO_Pin = W25Q16_SCLK_PIN;
    gpio_init.GPIO_Mode = GPIO_Mode_AF;
    gpio_init.GPIO_OType = GPIO_OType_PP;
    gpio_init.GPIO_PuPd = GPIO_PuPd_NOPULL;
    gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(W25Q16_SCLK_GPIO, &gpio_init);

    gpio_init.GPIO_Pin = W25Q16_CS_PIN;
    gpio_init.GPIO_Mode = GPIO_Mode_OUT;
    gpio_init.GPIO_OType = GPIO_OType_PP;
    gpio_init.GPIO_PuPd = GPIO_PuPd_UP;
    gpio_init.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(W25Q16_CS_GPIO, &gpio_init);

    GPIO_SetBits(W25Q16_CS_GPIO, W25Q16_CS_PIN);
}

void w25q16_spibus_init()
{
    SPI_InitTypeDef spi_init;

    w25q16_gpio_init();

    RCC_APB2PeriphClockCmd(SPI_CLOCK, ENABLE);

    spi_init.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    spi_init.SPI_Mode = SPI_Mode_Master;
    spi_init.SPI_DataSize = SPI_DataSize_8b;
    spi_init.SPI_CPOL = SPI_CPOL_High;
    spi_init.SPI_CPHA = SPI_CPHA_2Edge;
    spi_init.SPI_NSS = SPI_NSS_Soft;
    spi_init.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
    spi_init.SPI_FirstBit = SPI_FirstBit_MSB;
    spi_init.SPI_CRCPolynomial = 7;

    SPI_Init(CURRENT_SPI, &spi_init);
    SPI_Cmd(CURRENT_SPI, ENABLE);
}

static unsigned char w25q16_send_byte(unsigned char a_byte)
{
    unsigned char timeout = 10;

    while (SPI_I2S_GetFlagStatus(CURRENT_SPI, SPI_I2S_FLAG_TXE) == RESET) {
        if ((timeout--) == 0)
            return;
    }

    SPI_I2S_SendData(CURRENT_SPI, a_byte);

    timeout = 10;

    while (SPI_I2S_GetFlagStatus(CURRENT_SPI, SPI_I2S_FLAG_RXNE) == RESET) {
        if ((timeout--) == 0) 
            return;
    }

    return SPI_I2S_ReceiveData(CURRENT_SPI);
}

unsigned char w25q16_receive_byte()
{
    return SPI_I2S_ReceiveData(CURRENT_SPI);
}

uint32_t read_w25q16_id()
{
    uint32_t id = 0, id1 = 0, id2 = 0, id3 = 0;

    GPIO_ResetBits(W25Q16_CS_GPIO, W25Q16_CS_PIN);

    w25q16_send_byte(JEDEC_DEVICE_ID);

    id1 = w25q16_send_byte(DUMMY_BYTE);

    id2 = w25q16_send_byte(DUMMY_BYTE);

    id3 = w25q16_send_byte(DUMMY_BYTE);

    GPIO_SetBits(W25Q16_CS_GPIO, W25Q16_CS_PIN);

    id = (id1 << 16) | (id2 << 8) | id3;

    return id;
}

static void w25q16_write_enable()
{
    GPIO_ResetBits(W25Q16_CS_GPIO, W25Q16_CS_PIN);
    w25q16_send_byte(WRITE_ENABLE);
    sleep_us(1);
    GPIO_SetBits(W25Q16_CS_GPIO, W25Q16_CS_PIN);
}

static void tesing_w25q16_busy()
{
    unsigned char w25q16_status = 0;

    GPIO_ResetBits(W25Q16_CS_GPIO, W25Q16_CS_PIN);

    w25q16_send_byte(READ_STATUS);
    do {
        w25q16_status = w25q16_send_byte(DUMMY_BYTE);
    } while ((w25q16_status & 0x01) == SET);

    GPIO_SetBits(W25Q16_CS_GPIO, W25Q16_CS_PIN);
}

void w25q16_power_on()
{
    GPIO_ResetBits(W25Q16_CS_GPIO, W25Q16_CS_PIN);

    w25q16_send_byte(RELEASE_POWEROFF);

    GPIO_SetBits(W25Q16_CS_GPIO, W25Q16_CS_PIN);
}

void w25q16_power_off()
{
    GPIO_ResetBits(W25Q16_CS_GPIO, W25Q16_CS_PIN);

    w25q16_send_byte(POWER_OFF);

    GPIO_SetBits(W25Q16_CS_GPIO, W25Q16_CS_PIN);
}

void w25q16_sector_erase(uint32_t address)
{
    w25q16_write_enable();
    tesing_w25q16_busy();

    GPIO_ResetBits(W25Q16_CS_GPIO, W25Q16_CS_PIN);

    w25q16_send_byte(SECTOR_ERASE);

    w25q16_send_byte((address & 0xff0000) >> 16);
    w25q16_send_byte((address & 0xff00) >> 8);
    w25q16_send_byte(address & 0xff);

    GPIO_SetBits(W25Q16_CS_GPIO, W25Q16_CS_PIN);
    tesing_w25q16_busy();
}

void w25q16_page_write(uint32_t address, unsigned int lenth, unsigned char *string)
{
    unsigned int i = 0;

    w25q16_write_enable();

    GPIO_ResetBits(W25Q16_CS_GPIO, W25Q16_CS_PIN);

    w25q16_send_byte(PAGE_PROGRAM);

    w25q16_send_byte((address & 0xff0000) >> 16);
    w25q16_send_byte((address & 0xff00) >> 8);
    w25q16_send_byte(address & 0xff);

    // This will cause access errors
    // while ((lenth <= 255) && (string[i] != '\0')) {
    //     w25q16_send_byte(string[i]);
    //     i++;
    // }
    while (lenth--) {
        w25q16_send_byte(string[i]);
        i++;
    }

    GPIO_SetBits(W25Q16_CS_GPIO, W25Q16_CS_PIN);
    tesing_w25q16_busy();
}

void w25q16_write_pro(uint32_t address, unsigned int lenth, unsigned char *string)
{
    unsigned char over_address = 0;
    unsigned char less_byte = 0;
    unsigned char pages = 0;
    unsigned char less_page = 0;
    unsigned char less;

    // Note: an address is a byte
    
    over_address = address % 256;
    less_byte    = 256 - over_address;
    pages        = lenth / 256;
    less_page    = lenth % 256;

    if (over_address == 0) {
        if (pages == 0) {
            w25q16_page_write(address, lenth, string);
        } else {
            for (; pages > 0; pages--) {
                w25q16_page_write(address, 256, string);
                address = address + 256;
                string = string + 256;
            }
            w25q16_page_write(address, less_page, string);
        }
    } else {
        if (pages == 0) {
            if (less_byte < less_page) {
                less = less_page - less_byte;
                w25q16_page_write(address, less_byte, string);
                address = address + less_byte;
                string = string + less_byte;
                w25q16_page_write(address, less, string);
            } else {
                w25q16_page_write(address, lenth, string);
            }
        } else {
            lenth = lenth - less_byte;
            pages = lenth / 256;
            less_page = lenth % 256;
            w25q16_page_write(address, less_byte, string);
            address = address + less_byte;
            string = string + less_byte;

            for (; pages > 0; pages--) {
                w25q16_page_write(address, 256, string);
                address = address + 256;
                string = string + 256;
            }

            if (less_page != 0) {
                w25q16_page_write(address, less_page, string);
            }
        }
    }
}

void w25q16_read_data(uint32_t address, unsigned int lenth)
{
    unsigned int read;

    GPIO_ResetBits(W25Q16_CS_GPIO, W25Q16_CS_PIN);

    w25q16_send_byte(READ_DATA);

    w25q16_send_byte((address & 0xff0000) >> 16);
    w25q16_send_byte((address & 0xff00) >> 8);
    w25q16_send_byte(address & 0xff);

    for (read = 0; read < lenth; read++) {
        w25q16_data[read] = w25q16_send_byte(DUMMY_BYTE);
    }

    GPIO_SetBits(W25Q16_CS_GPIO, W25Q16_CS_PIN);
}

void w25q16_read_datas(uint32_t address, unsigned int lenth, unsigned char * w25q16_data)
{
    unsigned int read;

    GPIO_ResetBits(W25Q16_CS_GPIO, W25Q16_CS_PIN);

    w25q16_send_byte(READ_DATA);

    w25q16_send_byte((address & 0xff0000) >> 16);
    w25q16_send_byte((address & 0xff00) >> 8);
    w25q16_send_byte(address & 0xff);

    for (read = 0; read < lenth; read++) {
        *w25q16_data = w25q16_send_byte(DUMMY_BYTE);
        w25q16_data++;
    }

    GPIO_SetBits(W25Q16_CS_GPIO, W25Q16_CS_PIN);
}

