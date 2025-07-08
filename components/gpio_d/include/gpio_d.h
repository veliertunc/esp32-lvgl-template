#ifndef GPIO_D_H
#define GPIO_D_H

#include "driver/adc.h"
#include "driver/gpio.h"
#include "esp_err.h"
#include "esp_log.h"
#include "sdkconfig.h"

typedef enum {
  GPIO_D_MODE_ADC,
  GPIO_D_MODE_DIGITAL,
} gpio_d_mode_t;

static const char *TAG = "gpio_d";

#define GPIO32 32
#define GPIO35 35

esp_err_t gpio_d_init(void);
esp_err_t gpio_d_read_adc(int gpio_num, int *adc_out);
esp_err_t gpio_d_set_level(int gpio_num, int level);
int gpio_d_get_level(int gpio_num);

#endif // !GPIO_D_H
