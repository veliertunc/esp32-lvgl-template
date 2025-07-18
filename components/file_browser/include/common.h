#ifndef FILE_BROWSER_COMMON_H
#define FILE_BROWSER_COMMON_H

#include "driver/gpio.h"
#include "driver/spi_common.h"
#include "esp_log.h"
#include "esp_vfs_fat.h"
#include "lvgl.h"
#include "sdcard.h"
#include "sdmmc_cmd.h"
#include <dirent.h>
#include <sys/stat.h>

#define PATH_MAX_LENGTH 256

#define HIDDEN_FILE '.'

static sdmmc_card_t *card;
static char current_path[PATH_MAX_LENGTH] = SD_CARD_MOUNT_BASE;

static lv_obj_t *file_list;

#endif // !FILE_BROWSER_COMMON_H
