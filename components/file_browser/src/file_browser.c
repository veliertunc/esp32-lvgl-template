#include "file_browser.h"
#include "common.h"
#include "file_ops.h"
#include "file_preview.h"

static const char *TAG = "file_browser";

static void file_browser_event_handler(lv_event_t *e) {
  lv_obj_t *btn = lv_event_get_target(e);
  const char *filename = lv_list_get_btn_text(file_list, btn);

  char new_path[PATH_MAX_LENGTH];
  snprintf(new_path, sizeof(new_path), "%s/%s", current_path, filename);

  struct stat st;

  if (stat(new_path, &st) == 0) {
    if (S_ISDIR(st.st_mode)) {
      // Navigate the directory
      go_to_path(new_path);

      list_files_in_dir(current_path);
    } else {
      // File selected - just print its name and size for demo
      ESP_LOGI(TAG, "File selected: %s Size: %lld bytes", new_path, st.st_size);
      show_file_preview(new_path);
    }
  }
}

void file_browser_init(lv_obj_t *parent) {
  file_list = lv_list_create(parent);
  lv_obj_set_size(file_list, lv_pct(100), lv_pct(100));
  lv_obj_align(file_list, LV_ALIGN_CENTER, 0, 0);

  // List root files
  list_files_in_dir(current_path);
}

static void list_files_in_dir(const char *path) {
  DIR *dir = opendir(path);
  if (!dir) {
    ESP_LOGE(TAG, "Failed to open dir: %s", path);
    return;
  }

  struct dirent *entry;
  while ((entry = readdir(dir)) != NULL) {
    // Skip hidden files (optional)
    if (entry->d_name[0] == HIDDEN_FILE)
      continue;

    lv_obj_t *btn = lv_list_add_btn(file_list, NULL, entry->d_name);
    lv_obj_add_event_cb(btn, file_browser_event_handler, LV_EVENT_CLICKED,
                        NULL);
    lv_obj_add_event_cb(btn, show_context_menu, LV_EVENT_LONG_PRESSED, NULL);
  }
  closedir(dir);
}
