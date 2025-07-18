#include "folder_ops.h"
#include "common.h"
#include "file_ops.h"

static void paste_file_handler(lv_event_t *e) {
  if (clipboard_mode == CLIPBOARD_NONE)
    return;

  // Build destination path
  const char *src = clipboard_path;
  const char *filename = strrchr(src, '/');
  if (!filename)
    return;
  filename++; // skip slash

  char dst[PATH_MAX_LENGTH];
  path_join(dst, sizeof(dst), current_path, filename);

  if (clipboard_mode == CLIPBOARD_COPY) {
    if (copy_file(src, dst)) {
      ESP_LOGI(TAG, "File copied to %s", dst);
    } else {
      ESP_LOGE(TAG, "Copy failed");
    }
  } else if (clipboard_mode == CLIPBOARD_MOVE) {
    if (rename(src, dst) == 0) {
      ESP_LOGI(TAG, "File moved to %s", dst);
    } else {
      ESP_LOGE(TAG, "Move failed");
    }
  }

  clipboard_mode = CLIPBOARD_NONE;
  clipboard_path[0] = '\0';

  go_to_path(current_path); // Refresh list
}

static void new_folder_btn_handler(lv_event_t *e);

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

  lv_obj_t *btn_new_folder =
      lv_list_add_btn(file_list, LV_SYMBOL_PLUS, "New Folder");
  lv_obj_add_event_cb(btn_new_folder, new_folder_btn_handler, LV_EVENT_CLICKED,
                      NULL);
  if (clipboard_mode != CLIPBOARD_NONE) {
    lv_obj_t *btn_paste =
        lv_list_add_btn(file_list, LV_SYMBOL_PASTE, "Paste Here");
    lv_obj_add_event_cb(btn_paste, paste_file_handler, LV_EVENT_CLICKED, NULL);
  }
}

static lv_obj_t *new_folder_popup = NULL;
static lv_obj_t *new_folder_textarea = NULL;

static void new_folder_cancel_cb(lv_event_t *e) {
  if (new_folder_popup) {
    lv_obj_del(new_folder_popup);
    new_folder_popup = NULL;
  }
}

static void new_folder_confirm_cb(lv_event_t *e) {
  if (!new_folder_popup)
    return;

  const char *folder_name = lv_textarea_get_text(new_folder_textarea);
  if (strlen(folder_name) == 0) {
    // Optionally show error
    return;
  }

  char folder_path[PATH_MAX_LENGTH];
  path_join(folder_path, sizeof(folder_path), current_path, folder_name);

  if (mkdir(folder_path, GRANT_ALL) == 0) {
    ESP_LOGI(TAG, "Folder created: %s", folder_path);
  } else {
    ESP_LOGE(TAG, "Failed to create folder: %s", folder_path);
    // Show error dialog if needed
  }

  lv_obj_del(new_folder_popup);
  new_folder_popup = NULL;

  go_to_path(current_path);
}

static void show_new_folder_dialog() {
  if (new_folder_popup)
    return;

  new_folder_popup = lv_obj_create(lv_scr_act());
  lv_obj_set_size(new_folder_popup, 220, 120);
  lv_obj_center(new_folder_popup);

  lv_obj_t *label = lv_label_create(new_folder_popup);
  lv_label_set_text(label, "Enter folder name:");
  lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 10);

  new_folder_textarea = lv_textarea_create(new_folder_popup);
  lv_obj_set_width(new_folder_textarea, 200);
  lv_obj_align(new_folder_textarea, LV_ALIGN_CENTER, 0, -10);
  lv_textarea_set_one_line(new_folder_textarea, true);
  lv_textarea_set_text(new_folder_textarea, "");

  lv_obj_t *btn_create = lv_btn_create(new_folder_popup);
  lv_obj_set_size(btn_create, 90, 30);
  lv_obj_align(btn_create, LV_ALIGN_BOTTOM_LEFT, 10, -10);
  lv_obj_t *label_create = lv_label_create(btn_create);
  lv_label_set_text(label_create, "Create");
  lv_obj_center(label_create);
  lv_obj_add_event_cb(btn_create, new_folder_confirm_cb, LV_EVENT_CLICKED,
                      NULL);

  lv_obj_t *btn_cancel = lv_btn_create(new_folder_popup);
  lv_obj_set_size(btn_cancel, 90, 30);
  lv_obj_align(btn_cancel, LV_ALIGN_BOTTOM_RIGHT, -10, -10);
  lv_obj_t *label_cancel = lv_label_create(btn_cancel);
  lv_label_set_text(label_cancel, "Cancel");
  lv_obj_center(label_cancel);
  lv_obj_add_event_cb(btn_cancel, new_folder_cancel_cb, LV_EVENT_CLICKED, NULL);
}

static void new_folder_btn_handler(lv_event_t *e) { show_new_folder_dialog(); }
