#ifndef FB_FILE_OP_H
#define FB_FILE_OP_H
#include "common.h"

typedef enum {
  CLIPBOARD_NONE,
  CLIPBOARD_COPY,
  CLIPBOARD_MOVE
} clipboard_mode_t;

static lv_obj_t *context_menu = NULL;
static char selected_file_path[PATH_MAX_LENGTH];

static lv_obj_t *rename_popup = NULL;
static lv_obj_t *rename_textarea = NULL;

static clipboard_mode_t clipboard_mode = CLIPBOARD_NONE;
static char clipboard_path[PATH_MAX_LENGTH] = {0};

static void show_context_menu(lv_event_t *e);
static void hide_context_menu();
static void show_rename_dialog();
static bool copy_file(const char *src, const char *dst);

#endif // !FB_FILE_OP_H
