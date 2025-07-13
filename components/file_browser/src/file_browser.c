#include "file_browser.h"
#include "sdcard.h"

static const char *TAG = "file_browser";

static sdmmc_card_t *card;
static char current_path[PATH_MAX_LENGTH] = SD_CARD_MOUNT_BASE;

static lv_obj_t *file_list;

static void file_browser_event_handler(lv_event_t *e) {
  lv_obj_t *btn = lv_event_get_target(e);
  const char *filename = lv_list_get_btn_text(file_list, btn);

  char new_path[PATH_MAX_LENGTH];
  snprintf(new_path, sizeof(new_path), "%s/%s", current_path, filename);

  struct stat st;

  if (stat(new_path, &st) == 0) {
    if (S_ISDIR(st.st_mode)) {
      // Change directory
      strncpy(current_path, new_path, sizeof(current_path));
      ESP_LOGI(TAG, "Changed directory: %s", current_path);

      // Refresh list
      lv_list_clean(file_list);
      // Recurse to list new dir
      file_browser_event_handler(e); // Not best but simple: refresh
    } else {
      // File selected - just print its name and size for demo
      ESP_LOGI(TAG, "File selected: %s Size: %lld bytes", new_path, st.st_size);
    }
  }
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
  }
  closedir(dir);
}

void file_browser_init(lv_obj_t *parent) {
  file_list = lv_list_create(parent);
  lv_obj_set_size(file_list, lv_pct(100), lv_pct(100));
  lv_obj_align(file_list, LV_ALIGN_CENTER, 0, 0);

  // List root files
  list_files_in_dir(current_path);
}

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

  list_files_in_dir(current_path);
}
