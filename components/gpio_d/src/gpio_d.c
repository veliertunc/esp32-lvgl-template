#include "gpio_d.h"

static bool gpio32_is_adc = false;
static bool gpio35_is_adc = false;

esp_err_t gpio_d_init(void) {
  esp_err_t ret = ESP_OK;

#if CONFIG_GPIO32_MODE_ADC
  gpio32_is_adc = true;
  adc1_config_width(ADC_WIDTH_BIT_12);
  ret = adc1_config_channel_atten(ADC1_CHANNEL_4, ADC_ATTEN_DB_11);
  if (ret != ESP_OK) {
    ESP_LOGE(TAG, "Failed to config ADC channel GPIO32");
    return ret;
  }
#else
  gpio32_is_adc = false;
  gpio_reset_pin(GPIO32);
  gpio_set_direction(GPIO32, GPIO_MODE_OUTPUT);
#endif

#if CONFIG_GPIO35_MODE_ADC
  gpio35_is_adc = true;
  adc1_config_width(ADC_WIDTH_BIT_12);
  ret = adc1_config_channel_atten(ADC1_CHANNEL_7, ADC_ATTEN_DB_11);
  if (ret != ESP_OK) {
    ESP_LOGE(TAG, "Failed to config ADC channel GPIO35");
    return ret;
  }
#else
  gpio35_is_adc = false;
  gpio_reset_pin(GPIO35);
  gpio_set_direction(GPIO35, GPIO_MODE_OUTPUT);
#endif

  ESP_LOGI(TAG, "GPIO32 mode: %s", gpio32_is_adc ? "ADC" : "Digital");
  ESP_LOGI(TAG, "GPIO35 mode: %s", gpio35_is_adc ? "ADC" : "Digital");

  return ESP_OK;
}

esp_err_t gpio_d_read_adc(int gpio_num, int *adc_out) {
  if (adc_out == NULL)
    return ESP_ERR_INVALID_ARG;

  if (gpio_num == GPIO32) {
    if (!gpio32_is_adc)
      return ESP_ERR_INVALID_STATE;
    int val = adc1_get_raw(ADC1_CHANNEL_4);
    *adc_out = val;
    return ESP_OK;
  } else if (gpio_num == GPIO35) {
    if (!gpio35_is_adc)
      return ESP_ERR_INVALID_STATE;
    int val = adc1_get_raw(ADC1_CHANNEL_7);
    *adc_out = val;
    return ESP_OK;
  } else {
    return ESP_ERR_INVALID_ARG;
  }
}

esp_err_t gpio_d_set_level(int gpio_num, int level) {
  if (gpio_num == GPIO32) {
    if (gpio32_is_adc)
      return ESP_ERR_INVALID_STATE;
    gpio_set_level(GPIO32, level);
    return ESP_OK;
  } else if (gpio_num == GPIO35) {
    if (gpio35_is_adc)
      return ESP_ERR_INVALID_STATE;
    gpio_set_level(GPIO35, level);
    return ESP_OK;
  }
  return ESP_ERR_INVALID_ARG;
}

int gpio_d_get_level(int gpio_num) {
  if (gpio_num == GPIO32) {
    if (gpio32_is_adc)
      return -1;
    return gpio_get_level(GPIO32);
  } else if (gpio_num == GPIO35) {
    if (gpio35_is_adc)
      return -1;
    return gpio_get_level(GPIO35);
  }
  return -1;
}
