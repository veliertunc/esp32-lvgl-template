#ifndef __SD_CARD_H__
#define __SD_CARD_H__

#include "sdkconfig.h"

#define SD_CARD_MOUNT_BASE "/sdcard"

#define SD_CARD_PIN_NUM_MISO CONFIG_SD_CARD_PIN_NUM_MISO
#define SD_CARD_PIN_NUM_MOSI CONFIG_SD_CARD_PIN_NUM_MOSI
#define SD_CARD_PIN_NUM_CLK CONFIG_SD_CARD_PIN_NUM_CLK
#define SD_CARD_PIN_NUM_CS CONFIG_SD_CARD_PIN_NUM_CS
#define SD_CARD_MAX_FILES CONFIG_SD_CARD_MAX_FILES

#if CONFIG_SD_CARD_ALLOCATION_UNIT_BYTE
#define SD_CARD_SIZE_MULTIPLIER 1
#elif CONFIG_SD_CARD_ALLOCATION_UNIT_KB
#define SD_CARD_SIZE_MULTIPLIER 1024
#elif CONFIG_SD_CARD_ALLOCATION_UNIT_MB
#define SD_CARD_SIZE_MULTIPLIER (1024 * 1024)
#else
#error "No unit selected"
#endif

#define SD_CARD_ALLOCATION_SIZE CONFIG_SD_CARD_ALLOCATION_SIZE

void sd_card_init(void);
void write(const char *path, const char *data);
char *read(const char *path);

#endif // !__SD_CARD_H__
