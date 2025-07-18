#include "folder_ops.h"
#include "common.h"

extern void list_files_in_dir(const char *path);

static void back_button_handler(lv_event_t *e) {
  if (strcmp(current_path, SD_CARD_MOUNT_BASE) == 0)
    return; // Already root

  // Remove last path component
  char *last_slash = strrchr(current_path, '/');
  if (last_slash && last_slash != current_path) {
    *last_slash = '\0'; // cut off
  } else {
    // Go to root
    strcpy(current_path, SD_CARD_MOUNT_BASE);
  }
  go_to_path(current_path);
}

static void root_button_handler(lv_event_t *e) {
  strcpy(current_path, SD_CARD_MOUNT_BASE);
  go_to_path(current_path);
}

static void go_to_path(const char *new_path) {
  strncpy(current_path, new_path, sizeof(current_path));
  lv_list_clean(file_list);

  // Add navigation buttons
  lv_obj_t *btn_back = lv_list_add_btn(file_list, NULL, ".. (Back)");
  lv_obj_add_event_cb(btn_back, back_button_handler, LV_EVENT_CLICKED, NULL);

  lv_obj_t *btn_root = lv_list_add_btn(file_list, NULL, "/ (Root)");
  lv_obj_add_event_cb(btn_root, root_button_handler, LV_EVENT_CLICKED, NULL);
}
