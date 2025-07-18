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
#include <stdbool.h>
#include <stdint.h>
#include <sys/stat.h>

#define PATH_MAX_LENGTH 256

#define HIDDEN_FILE '.'

static sdmmc_card_t *card;
static char current_path[PATH_MAX_LENGTH] = SD_CARD_MOUNT_BASE;

static lv_obj_t *file_list;

static void path_join(char *dest, size_t size, const char *base,
                      const char *name) {
  if (base[strlen(base) - 1] == '/') {
    snprintf(dest, size, "%s%s", base, name);
  } else {
    snprintf(dest, size, "%s/%s", base, name);
  }
}

#define READ_PERMISSION (1 << 2)
#define WRITE_PERMISSON (1 << 1)
#define EXEC_PERMISSON (1 << 0)
#define RWX_PERMISSON (READ_PERMISSION | WRITE_PERMISSON | EXEC_PERMISSON)

// Unix 0777 equivalent
#define GRANT_ALL                                                              \
  ((RWX_PERMISSON << 2) | (RWX_PERMISSON << 1) | (RWX_PERMISSON << 0))

#endif // !FILE_BROWSER_COMMON_H
