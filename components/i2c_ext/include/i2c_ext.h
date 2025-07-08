#ifndef I2C_EXT_H
#define I2C_EXT_H

#include "driver/i2c.h"
#include "esp_log.h"
#include "sdkconfig.h"

static const char *TAG = "i2c_ext";

#define I2C_PORT CONFIG_I2C_EXT_PORT_NUM
#define I2C_SDA_PIN CONFIG_I2C_EXT_SDA_PIN
#define I2C_SCL_PIN CONFIG_I2C_EXT_SCL_PIN
#define I2C_FREQ_HZ CONFIG_I2C_EXT_FREQ_HZ

void i2c_ext_init(void);

#endif // !I2C_EXT_H
