
#include "display.h"

void lvgl_driver_flush(lv_disp_drv_t *drv, const lv_area_t *area,
                       lv_color_t *color_map) {
  esp_lcd_panel_draw_bitmap(panel_handle, area->x1, area->y1, area->x2 + 1,
                            area->y2 + 1, color_map);
  lv_disp_flush_ready(drv);
}

void lvgl_touchpad_read(lv_indev_drv_t *drv, lv_indev_data_t *data) {
  bool touched = esp_lcd_touch_get_coordinates(touch_handle, &data->point.x,
                                               &data->point.y, NULL, NULL);
  data->state = touched ? LV_INDEV_STATE_PRESSED : LV_INDEV_STATE_RELEASED;
}

void display_init(void) {
  spi_bus_config_t buscfg = {
      .miso_io_num = PIN_NUM_MISO,
      .mosi_io_num = PIN_NUM_MOSI,
      .sclk_io_num = PIN_NUM_CLK,
      .quadwp_io_num = -1,
      .quadhd_io_num = -1,
      .max_transfer_sz = DISP_HOR_RES * DISP_VER_RES * sizeof(lv_color_t),
  };
  ESP_ERROR_CHECK(spi_bus_initialize(LCD_HOST, &buscfg, SPI_DMA_CH_AUTO));

  esp_lcd_panel_io_spi_config_t io_config = {
      .cs_gpio_num = PIN_NUM_CS,
      .dc_gpio_num = PIN_NUM_DC,
      .spi_mode = 0,
      .pclk_hz = 20 * 1000 * 1000,
      .trans_queue_depth = 10,
      .on_color_trans_done = NULL,
      .user_ctx = NULL,
  };

  esp_lcd_panel_io_handle_t io_handle = NULL;
  ESP_ERROR_CHECK(esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)LCD_HOST,
                                           &io_config, &io_handle));

  esp_lcd_panel_dev_config_t panel_config = {
      .reset_gpio_num = PIN_NUM_RST,
      .color_space = ESP_LCD_COLOR_SPACE_RGB,
      .bits_per_pixel = DISP_COLOR_DEPTH,
  };

  ESP_ERROR_CHECK(
      esp_lcd_new_panel_ili9341(io_handle, &panel_config, &panel_handle));
  ESP_ERROR_CHECK(esp_lcd_panel_reset(panel_handle));
  ESP_ERROR_CHECK(esp_lcd_panel_init(panel_handle));
  ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(panel_handle, true));

  gpio_set_direction(PIN_NUM_BL, GPIO_MODE_OUTPUT);
  gpio_set_level(PIN_NUM_BL, 1);

  esp_lcd_touch_config_t tp_cfg = {
      .x_max = DISP_HOR_RES,
      .y_max = DISP_VER_RES,
      .rst_gpio_num = -1,
      .int_gpio_num = PIN_NUM_TOUCH_IRQ,
      .levels =
          {
              .reset = 0,
              .interrupt = 0,
          },
      .flags =
          {
              .swap_xy = 1,
              .mirror_x = 0,
              .mirror_y = 0,
          },
  };

  esp_lcd_panel_io_spi_config_t tp_io_cfg = {
      .cs_gpio_num = PIN_NUM_TOUCH_CS,
      .dc_gpio_num = -1,
      .spi_mode = 0,
      .pclk_hz = 2 * 1000 * 1000,
      .trans_queue_depth = 5,
      .user_ctx = NULL,
      .on_color_trans_done = NULL,
  };

  esp_lcd_panel_io_handle_t tp_io_handle = NULL;
  ESP_ERROR_CHECK(esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)LCD_HOST,
                                           &tp_io_cfg, &tp_io_handle));

  ESP_ERROR_CHECK(
      esp_lcd_touch_new_xpt2046(tp_io_handle, &tp_cfg, &touch_handle));

  lv_disp_draw_buf_init(&draw_buf, buf1, buf2, sizeof(buf1) / sizeof(buf1[0]));

  lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);
  disp_drv.hor_res = DISP_HOR_RES;
  disp_drv.ver_res = DISP_VER_RES;
  disp_drv.flush_cb = lvgl_driver_flush;
  disp_drv.draw_buf = &draw_buf;
  disp_drv.user_data = panel_handle;
  lv_disp_drv_register(&disp_drv);

  lv_indev_drv_t indev_drv;
  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = lvgl_touchpad_read;
  indev_drv.user_data = touch_handle;
  lv_indev_drv_register(&indev_drv);

  ESP_LOGI(TAG, "Display initialized");
}
