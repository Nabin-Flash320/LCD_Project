#ifndef __ILI9486_H__
#define __ILI9486_H__

#include "lcd_com.h"
#include "lvgl.h"


void ILI9486_lcdInit(TFT_t * dev);
void ILI9486_flush(lv_disp_drv_t * drv, const lv_area_t * area, lv_color_t * color_map);
void ili9486_wait_cb(struct _lv_disp_drv_t * disp_drv);


#endif

