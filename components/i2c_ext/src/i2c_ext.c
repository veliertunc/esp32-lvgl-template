#include "i2c_ext.h"

void i2c_ext_init(void) {
  i2c_config_t conf = {
      .mode = I2C_MODE_MASTER,
      .sda_io_num = I2C_SDA_PIN,
      .scl_io_num = I2C_SCL_PIN,
      .sda_pullup_en = GPIO_PULLUP_ENABLE,
      .scl_pullup_en = GPIO_PULLUP_ENABLE,
      .master.clk_speed = I2C_FREQ_HZ,
  };

  esp_err_t err = i2c_param_config(I2C_PORT, &conf);
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "i2c_param_config failed: %d", err);
    return;
  }

  err = i2c_driver_install(I2C_PORT, I2C_MODE_MASTER, 0, 0, 0);
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "i2c_driver_install failed: %d", err);
    return;
  }

  ESP_LOGI(
      TAG,
      "External I2C initialized on port %d (SDA GPIO %d, SCL GPIO %d) at %d Hz",
      I2C_PORT, I2C_SDA_PIN, I2C_SCL_PIN, I2C_FREQ_HZ);
}
