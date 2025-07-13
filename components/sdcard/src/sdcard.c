#include "sdcard.h"
#include "driver/gpio.h"
#include "driver/spi_common.h"
#include "esp_log.h"
#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

static const char *TAG = "SD_CARD";

void sd_card_init(void) {
  esp_err_t ret;
  // Mount config
  esp_vfs_fat_sdmmc_mount_config_t mount_config = {
      .format_if_mount_failed = false,
      .max_files = SD_CARD_MAX_FILES,
      .allocation_unit_size =
          SD_CARD_ALLOCATION_SIZE * SD_CARD_SIZE_MULTIPLIER};

  // SPI Bus config
  spi_bus_config_t bus_cfg = {.mosi_io_num = SD_CARD_PIN_NUM_MOSI,
                              .miso_io_num = SD_CARD_PIN_NUM_MISO,
                              .sclk_io_num = SD_CARD_PIN_NUM_CLK,
                              .quadwp_io_num = -1,
                              .quadhd_io_num = -1};
  ret = spi_bus_initialize(SPI2_HOST, &bus_cfg, SPI_DMA_CH_AUTO);
  ESP_ERROR_CHECK(ret);

  // SD SPI device config
  sdspi_device_config_t slot_config = SDSPI_DEVICE_CONFIG_DEFAULT();
  slot_config.gpio_cs = SD_CARD_PIN_NUM_CS;
  slot_config.host_id = SPI2_HOST;
  file_list = lv_list_create(parent);
  lv_obj_set_size(file_list, lv_pct(100), lv_pct(100));
  lv_obj_align(file_list, LV_ALIGN_CENTER, 0, 0);

  // List root files
  list_files_in_dir(current_path);
  sdmmc_card_t *card;
  ret = esp_vfs_fat_sdspi_mount(SD_CARD_MOUNT_BASE, &slot_config, &mount_config,
                                &card);
  if (ret != ESP_OK) {
    ESP_LOGE(TAG, "Failed to mount filesystem. (%s)", esp_err_to_name(ret));
    return;
  }

  // Print card info
  sdmmc_card_print_info(stdout, card);
}

void write(const char *path, const char *data) {
  FILE *f = fopen(path, "w");
  if (f == NULL) {
    ESP_LOGE(TAG, "Failed to open file for writing");
    return;
  }

  fprintf(f, "%s\n", data);
  fclose(f);
}

char *read(const char *path) {
  char line[64];
  FILE *f = fopen(path, "r");
  if (f == NULL) {
    ESP_LOGE(TAG, "Failed to open file for reading");
    return NULL;
  }

  fgets(line, sizeof(line), f);
  fclose(f);
  ESP_LOGI(TAG, "Read from file: '%s'", line);

  return line;
}
