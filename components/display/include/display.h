#ifndef DISPLAY_H
#define DISPLAY_H

#include "driver/gpio.h"
#include "driver/spi_master.h"
#include "esp_lcd_ili9341.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_touch.h"
#include "esp_lcd_touch_xpt2046.h"
#include "esp_log.h"
#include "lvgl.h"
#include "sdkconfig.h"

#if CONFIG_DISPLAY_SPI1_HOST
#define LCD_HOST SPI1_HOST
#elif CONFIG_DISPLAY_SPI2_HOST
#define LCD_HOST SPI2_HOST
#elif CONFIG_DISPLAY_SPI3_HOST
#define LCD_HOST SPI3_HOST
#else
#error "No SPI host selected for LCD!"
#endif

#define PIN_NUM_MISO CONFIG_PIN_NUM_MISO
#define PIN_NUM_MOSI CONFIG_PIN_NUM_MOSI
#define PIN_NUM_CLK CONFIG_PIN_NUM_CLK
#define PIN_NUM_CS CONFIG_PIN_NUM_CS
#define PIN_NUM_DC CONFIG_PIN_NUM_DC
#define PIN_NUM_RST CONFIG_PIN_NUM_RST
#define PIN_NUM_BL CONFIG_PIN_NUM_BL
#define PIN_NUM_TOUCH_CS CONFIG_PIN_NUM_TOUCH_CS
#define PIN_NUM_TOUCH_IRQ CONFIG_PIN_NUM_TOUCH_IRQ

#define DISP_HOR_RES CONFIG_LVGL_HOR_RES
#define DISP_VER_RES CONFIG_LVGL_VER_RES

#if CONFIG_LVGL_COLOR_DEPTH_8
#define DISP_COLOR_DEPTH 8
#elif CONFIG_LVGL_COLOR_DEPTH_16
#define DISP_COLOR_DEPTH 16
#elif CONFIG_LVGL_COLOR_DEPTH_32
#define DISP_COLOR_DEPTH 32
#else
#define DISP_COLOR_DEPTH 16
#endif

static const char *TAG = "display";

static esp_lcd_panel_handle_t panel_handle;
static esp_lcd_touch_handle_t touch_handle;

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf1[320 * 40];
static lv_color_t buf2[320 * 40];

void lvgl_driver_flush(lv_disp_drv_t *drv, const lv_area_t *area,
                       lv_color_t *color_map);

void lvgl_touchpad_read(lv_indev_drv_t *drv, lv_indev_data_t *data);

void display_init(void);

#endif
