#ifndef FILE_BROWSER_H
#define FILE_BROWSER_H

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

void file_browser_init(lv_obj_t *);
static void list_files_in_dir(const char *);
static void go_to_path(const char *);

#endif // !FILE_BROWSER_H
