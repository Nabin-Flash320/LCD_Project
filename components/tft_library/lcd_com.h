#ifndef   __LCD_COM_H__
#define   __LCD_COM_H__

#include "driver/adc.h"

#define TFTLCD_DELAY	0xFFFF
#define TFTLCD_DELAY16	0xFFFF
#define TFTLCD_DELAY8	0x7F

typedef enum {INTERFACE_I2S, INTERFACE_GPIO, INTERFACE_REG} INTERFACE;


typedef enum {MODE_RESET, MODE_OUTPUT, MODE_INPUT} MODE;

#define GPIO_PORT_NUM  0
#define NUM_SAMPLES    2 // Number of samples
#define COMP_TOLERANCE 2 // Coordinate tolerance
#define AVERAGE_TIME   4 // Number of samples when averaging

typedef struct {
	int16_t _rd;
	int16_t _wr;
	int16_t _rs;
	int16_t _cs;
	int16_t _d0;
	int16_t _d1;
	int16_t _d2;
	int16_t _d3;
	int16_t _d4;
	int16_t _d5;
	int16_t _d6;
	int16_t _d7;
	int16_t _delay;
	int16_t _interface;
} TFT_t;

void gpio_digital_write(int GPIO_PIN, uint8_t data);
void gpio_lcd_write_data(int dummy1, unsigned char *data, size_t size);
void reg_lcd_write_data(int dummy1, unsigned char *data, size_t size);

void lcd_write_table(TFT_t * dev, const void *table, int16_t size);
void lcd_write_table16(TFT_t * dev, const void *table, int16_t size);
void lcd_write_comm_byte(TFT_t * dev, uint8_t cmd);
void lcd_write_comm_word(TFT_t * dev, uint16_t cmd);
void lcd_write_data_byte(TFT_t * dev, uint8_t data);
void lcd_write_data_word(TFT_t * dev, uint16_t data);
void lcd_write_addr(TFT_t * dev, uint16_t addr1, uint16_t addr2);
void lcd_write_color(TFT_t * dev, uint16_t color, uint16_t size);
void lcd_write_colors(TFT_t * dev, uint16_t * colors, uint16_t size);
void lcd_delay_ms(int delay_time);
void lcd_write_register_word(TFT_t * dev, uint16_t addr, uint16_t data);
void lcd_write_register_byte(TFT_t * dev, uint8_t addr, uint16_t data);
esp_err_t lcd_interface_cfg(TFT_t * dev, int interface);


#endif
