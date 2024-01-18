

#include "esp_log.h"
#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "lvgl.h"
#include "ILI9486.h"
#include "lcd_com.h"
#include "stdlib.h"
#include "esp_timer.h"

#define HORI_RES 480
#define VER_RES 320
#define DISPLAY_BUFFER_SIZE (HORI_RES * 32)
#define LV_TICK_PERIOD_MS 1000

static SemaphoreHandle_t lvgl_task_handler_semaphore;

static lv_disp_draw_buf_t draw_buf;
static lv_disp_drv_t disp_drv;

static void lv_tick_task(void *arg);
static void lcd_project_task(void *params);
static void create_application();

void app_main()
{
    xTaskCreatePinnedToCore(lcd_project_task, "lcd_project_task", 4096 * 2, NULL, 3, NULL, 0); // The task is pinned to core 0, if used BLE and wifi, please change it to the core 1.
}

static void lcd_project_task(void *params)
{

    // Create the task handler semaphoer and initialize the LVGL graphics library.
    lvgl_task_handler_semaphore = xSemaphoreCreateMutex();
    lv_init();

    // Now initialize the TFT library. It includes all the interfaces.
    TFT_t *ili9486_device = (TFT_t *)malloc(sizeof(TFT_t));
    memset(ili9486_device, 0, sizeof(TFT_t));
    lcd_interface_cfg(ili9486_device, 0);
    ILI9486_lcdInit(ili9486_device);

    // Create a buffer, the display buffer size generally can be up tp 10% of the vertical resolution, however, it can either be increaded or decreased according to the requirements and memory available.
    // Create at least 2 buffers.
    lv_color_t *buffer1 = malloc(DISPLAY_BUFFER_SIZE * sizeof(lv_color_t));
    lv_color_t *buffer2 = malloc(DISPLAY_BUFFER_SIZE * sizeof(lv_color_t));
    // Check if buffers are available.
    assert(buffer1 != NULL);
    assert(buffer2 != NULL);

    // Initialize the display buffer, display driver and populate the respective fields in display driver.
    lv_disp_draw_buf_init(&draw_buf, buffer1, buffer2, DISPLAY_BUFFER_SIZE);
    lv_disp_drv_init(&disp_drv);
    disp_drv.flush_cb = ILI9486_flush;
    disp_drv.wait_cb = ili9486_wait_cb;
    disp_drv.user_data = ili9486_device;
    disp_drv.rotated = LV_DISP_ROT_NONE;
    disp_drv.draw_buf = &draw_buf;
    disp_drv.hor_res = 480;
    disp_drv.ver_res = 320;

    // Register the display driver.
    lv_disp_drv_register(&disp_drv);

    /* Create and start a periodic timer interrupt to call lv_tick_inc */
    const esp_timer_create_args_t periodic_timer_args = {
        .callback = &lv_tick_task,
        .name = "periodic_gui"};
    esp_timer_handle_t periodic_timer;
    ESP_ERROR_CHECK(esp_timer_create(&periodic_timer_args, &periodic_timer));
    ESP_ERROR_CHECK(esp_timer_start_periodic(periodic_timer, LV_TICK_PERIOD_MS * 1000));

    create_application();

    while (1)
    {
        /* Delay 1 tick (assumes FreeRTOS tick is 10ms */
        vTaskDelay(pdMS_TO_TICKS(10));

        /* Try to take the semaphore, call lvgl related function on success */
        if (pdTRUE == xSemaphoreTake(lvgl_task_handler_semaphore, portMAX_DELAY))
        {
            lv_task_handler();
            xSemaphoreGive(lvgl_task_handler_semaphore);
        }
    }
}

static void create_application()
{
    lv_obj_t *screen = lv_scr_act();

    static lv_style_t screen_style;
    lv_style_init(&screen_style);
    lv_style_set_bg_color(&screen_style, lv_color_make(255, 250, 250)); // Snow white color is set as background color.
    lv_style_set_pad_all(&screen_style, 5);
    lv_obj_add_style(screen, &screen_style, LV_PART_MAIN);

    lv_obj_t *chart = lv_chart_create(screen);
    lv_obj_set_size(chart, 350, 310);
    lv_chart_set_type(chart, LV_CHART_TYPE_LINE); /*Show lines and points too*/

    /*Add two data series*/
    lv_chart_series_t *ser1 = lv_chart_add_series(chart, lv_palette_main(LV_PALETTE_RED), LV_CHART_AXIS_PRIMARY_Y);
    lv_chart_series_t *ser2 = lv_chart_add_series(chart, lv_palette_main(LV_PALETTE_GREEN), LV_CHART_AXIS_SECONDARY_Y);

    /*Set the next points on 'ser1'*/
    lv_chart_set_next_value(chart, ser1, 10);
    lv_chart_set_next_value(chart, ser1, 10);
    lv_chart_set_next_value(chart, ser1, 10);
    lv_chart_set_next_value(chart, ser1, 10);
    lv_chart_set_next_value(chart, ser1, 10);
    lv_chart_set_next_value(chart, ser1, 10);
    lv_chart_set_next_value(chart, ser1, 10);
    lv_chart_set_next_value(chart, ser1, 30);
    lv_chart_set_next_value(chart, ser1, 70);
    lv_chart_set_next_value(chart, ser1, 90);

    /*Directly set points on 'ser2'*/
    ser2->y_points[0] = 90;
    ser2->y_points[1] = 70;
    ser2->y_points[2] = 65;
    ser2->y_points[3] = 65;
    ser2->y_points[4] = 65;
    ser2->y_points[5] = 65;
    ser2->y_points[6] = 65;
    ser2->y_points[7] = 65;
    ser2->y_points[8] = 65;
    ser2->y_points[9] = 65;


    lv_chart_refresh(chart); /*Required after direct set*/
}

static void lv_tick_task(void *arg)
{
    (void)arg;
    lv_tick_inc(LV_TICK_PERIOD_MS);
}
