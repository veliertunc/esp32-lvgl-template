#include "display.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "gpio_d.h"
#include "i2c_ext.h"
#include "lvgl.h"
#include "uart1.h"
#include <stdio.h>

static const char *TAG = "main";

static void lv_tick_cb(void *arg) { lv_tick_inc(1); }

void app_main(void) {
  ESP_LOGI(TAG, "Initializing UART1...");
  uart1_init();

  ESP_LOGI(TAG, "Initializing external I2C...");
  i2c_ext_init();

  ESP_LOGI(TAG, "Initializing GPIO_D...");
  esp_err_t err = gpio_d_init();
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "gpio_d_init failed: %d", err);
  }

  // Example usage GPIO_D
  int adc_val;
  if (gpio_d_read_adc(32, &adc_val) == ESP_OK) {
    ESP_LOGI(TAG, "GPIO32 ADC value: %d", adc_val);
  } else {
    ESP_LOGI(TAG, "GPIO32 is digital output, setting HIGH");
    gpio_d_set_level(32, 1);
  }

  if (gpio_d_read_adc(35, &adc_val) == ESP_OK) {
    ESP_LOGI(TAG, "GPIO35 ADC value: %d", adc_val);
  } else {
    ESP_LOGI(TAG, "GPIO35 is digital output, setting LOW");
    gpio_d_set_level(35, 0);
  }

  ESP_LOGI(TAG, "Initializing display...");
  display_init();

  const esp_timer_create_args_t periodic_timer_args = {.callback = &lv_tick_cb,
                                                       .name = "lv_tick"};
  esp_timer_handle_t periodic_timer;
  esp_timer_create(&periodic_timer_args, &periodic_timer);
  esp_timer_start_periodic(periodic_timer, 1000); // 1ms tick

  lv_obj_t *label = lv_label_create(lv_scr_act());
  lv_label_set_text(label, "Hello Elecrow!");
  lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

  while (1) {
    lv_task_handler();
    vTaskDelay(pdMS_TO_TICKS(10));
  }
}
