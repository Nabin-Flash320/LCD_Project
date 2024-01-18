#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "lcd_com.h"
// #include "lcd_lib.h"
#include "ILI9486.h"

#define TAG "ILI9486"

void ILI9486_lcdInit(TFT_t *dev)
{
	dev->_delay = 0;
	// Initailize TFT
	static const uint8_t reset_off[] = {
		0x01, 0, // Soft Reset
		TFTLCD_DELAY8, 150,
		0x28, 0,	   // Display Off
		0x3A, 1, 0x55, // Pixel read=565, write=565.
	};

	static const uint8_t wake_on[] = {
		0x11, 0, // Sleep Out
		TFTLCD_DELAY8, 150,
		0x29, 0, // Display On
	};

	static const uint8_t regValues[] = {
		0xB0, 1, 0x00,					 // unlocks E0, F0 0100 0000
		0xB3, 4, 0x02, 0x00, 0x00, 0x00, // Frame Memory, interface [02 00 00 00]
		0xB4, 1, 0x00,					 // Frame mode [00]
		// 0xB6, 3, 0x02, 0x02, 0x3B,  // Display Function Control [02 02 3B]
		0xB6, 3, 0x02, 0x42, 0x3B, // Display Function Control [02 02 3B]
		0xD0, 3, 0x07, 0x42, 0x18,
		0xD1, 3, 0x00, 0x07, 0x18,
		0xD2, 2, 0x01, 0x02,
		0xD3, 2, 0x01, 0x02, // Set Power for Partial Mode [01 22]
		0xD4, 2, 0x01, 0x02, // Set Power for Idle Mode [01 22]
		// 0xC0, 5, 0x10, 0x3B, 0x00, 0x02, 0x11,
		0xC0, 5, 0x14, 0x3B, 0x00, 0x02, 0x11,
		0xC1, 3, 0x10, 0x10, 0x88, // Display Timing Normal [10 10 88]
		0xC5, 1, 0x03,			   // Frame Rate [03]
		0xC6, 1, 0x02,			   // Interface Control [02]
		0xC8, 12, 0x00, 0x32, 0x36, 0x45, 0x06, 0x16, 0x37, 0x75, 0x77, 0x54, 0x0C, 0x00,
		0xCC, 1, 0x00, // Panel Control [00]
		0x36, 1, 0x68, // https://github.com/ImpulseAdventure/Waveshare_ILI9486/blob/master/src/Waveshare_ILI9486.cpp and line number 473 currently in potrait
	};
	lcd_write_table(dev, reset_off, sizeof(reset_off));
	lcd_write_table(dev, regValues, sizeof(regValues));
	lcd_write_table(dev, wake_on, sizeof(wake_on));
}

static void ILI9486_send_data(TFT_t *ili9486_device, uint8_t *data, size_t length)
{
	for (int i = 0; i < length; i++)
	{
		lcd_write_data_byte(ili9486_device, data[i]);
	}
}

void ILI9486_flush(lv_disp_drv_t *drv, const lv_area_t *area, lv_color_t *color_map)
{

	TFT_t *ili9486_device = (TFT_t *)drv->user_data;
	uint32_t size = 0;
	if (ili9486_device)
	{
		uint8_t data[4] = {0};

		// /*Column addresses*/
		lcd_write_comm_byte(ili9486_device, 0x2A);
		data[0] = (area->x1 >> 8) & 0xFF;
		data[1] = area->x1 & 0xFF;
		data[2] = (area->x2 >> 8) & 0xFF;
		data[3] = area->x2 & 0xFF;

		ILI9486_send_data(ili9486_device, data, 4);

		// /*Page addresses*/
		lcd_write_comm_byte(ili9486_device, 0x2B);
		data[0] = (area->y1 >> 8) & 0xFF;
		data[1] = area->y1 & 0xFF;
		data[2] = (area->y2 >> 8) & 0xFF;
		data[3] = area->y2 & 0xFF;

		ILI9486_send_data(ili9486_device, data, 4);

		// /*Memory write*/
		lcd_write_comm_byte(ili9486_device, 0x2C);

		size = lv_area_get_width(area) * lv_area_get_height(area);
		lcd_write_colors(ili9486_device, (uint16_t *)color_map, size);
		lv_disp_flush_ready(drv);
	}
}

void ili9486_wait_cb(struct _lv_disp_drv_t *disp_drv)
{
	vTaskDelay(10 / portTICK_PERIOD_MS);
}
