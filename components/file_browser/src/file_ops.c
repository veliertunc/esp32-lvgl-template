#include "file_ops.h"
#include "folder_ops.h"

/*********** Cut/Copy Operations ***********/
static void cut_file_cb(lv_event_t *e) {
  clipboard_mode = CLIPBOARD_MOVE;
  strcpy(clipboard_path, selected_file_path);
  hide_context_menu();
  go_to_path(current_path); // triggers paste button
}

static void copy_file_cb(lv_event_t *e) {
  clipboard_mode = CLIPBOARD_COPY;
  strcpy(clipboard_path, selected_file_path);
  hide_context_menu();
  go_to_path(current_path); // triggers paste button
}

/*********** Rename Popup ***********/
static void rename_cancel_cb(lv_event_t *e) {
  if (rename_popup) {
    lv_obj_del(rename_popup);
    rename_popup = NULL;
  }
}

static void rename_confirm_cb(lv_event_t *e) {
  if (!rename_popup)
    return;

  const char *new_name = lv_textarea_get_text(rename_textarea);
  if (strlen(new_name) == 0) {
    // Show error or ignore
    return;
  }

  // Build new path
  char new_path[PATH_MAX_LENGTH];
  path_join(new_path, sizeof(new_path), current_path, new_name);

  // Rename file
  if (rename(selected_file_path, new_path) == 0) {
    ESP_LOGI("", "Renamed to %s", new_path);
  } else {
    ESP_LOGE("", "Rename failed");
    // You can show an error dialog here
  }

  lv_obj_del(rename_popup);
  rename_popup = NULL;

  // Refresh file list
  go_to_path(current_path);
}

static void show_rename_dialog() {
  if (rename_popup)
    return;

  rename_popup = lv_obj_create(lv_scr_act());
  lv_obj_set_size(rename_popup, 200, 120);
  lv_obj_center(rename_popup);

  lv_obj_t *label = lv_label_create(rename_popup);
  lv_label_set_text(label, "Enter new name:");
  lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 10);

  rename_textarea = lv_textarea_create(rename_popup);
  lv_obj_set_width(rename_textarea, 180);
  lv_obj_align(rename_textarea, LV_ALIGN_CENTER, 0, -10);
  lv_textarea_set_one_line(rename_textarea, true);
  lv_textarea_set_text(rename_textarea, "");

  lv_obj_t *btn_rename = lv_btn_create(rename_popup);
  lv_obj_set_size(btn_rename, 80, 30);
  lv_obj_align(btn_rename, LV_ALIGN_BOTTOM_LEFT, 10, -10);
  lv_obj_t *label_rename = lv_label_create(btn_rename);
  lv_label_set_text(label_rename, "Rename");
  lv_obj_center(label_rename);
  lv_obj_add_event_cb(btn_rename, rename_confirm_cb, LV_EVENT_CLICKED, NULL);

  lv_obj_t *btn_cancel = lv_btn_create(rename_popup);
  lv_obj_set_size(btn_cancel, 80, 30);
  lv_obj_align(btn_cancel, LV_ALIGN_BOTTOM_RIGHT, -10, -10);
  lv_obj_t *label_cancel = lv_label_create(btn_cancel);
  lv_label_set_text(label_cancel, "Cancel");
  lv_obj_center(label_cancel);
  lv_obj_add_event_cb(btn_cancel, rename_cancel_cb, LV_EVENT_CLICKED, NULL);
}

/*********** Context Menu Callbacks ***********/
static void delete_file_cb(lv_event_t *e) {
  if (remove(selected_file_path) == 0) {
    ESP_LOGI(TAG, "Deleted: %s", selected_file_path);
    hide_context_menu();
    go_to_path(current_path);
  } else {
    ESP_LOGE(TAG, "Failed to delete: %s", selected_file_path);
  }
}

static void rename_file_cb(lv_event_t *e) {
  // For brevity, let's just log the rename request
  ESP_LOGI(TAG, "Rename requested for: %s", selected_file_path);
  hide_context_menu();
  show_rename_dialog();
}

/*********** Context Menu Operations ***********/
static void hide_context_menu() {
  if (context_menu) {
    lv_obj_del(context_menu);
    context_menu = NULL;
  }
}

static void show_context_menu(lv_event_t *e) {
  lv_obj_t *btn = lv_event_get_target(e);
  const char *filename = lv_list_get_btn_text(file_list, btn);
  snprintf(selected_file_path, sizeof(selected_file_path), "%s/%s",
           current_path, filename);

  context_menu = lv_obj_create(lv_scr_act());
  lv_obj_set_size(context_menu, 120, 90);
  lv_obj_center(context_menu);

  lv_obj_t *del_btn = lv_btn_create(context_menu);
  lv_obj_set_size(del_btn, 110, 30);
  lv_obj_align(del_btn, LV_ALIGN_TOP_MID, 0, 5);
  lv_obj_t *del_label = lv_label_create(del_btn);
  lv_label_set_text(del_label, "Delete");
  lv_obj_center(del_label);
  lv_obj_add_event_cb(del_btn, delete_file_cb, LV_EVENT_CLICKED, NULL);

  lv_obj_t *ren_btn = lv_btn_create(context_menu);
  lv_obj_set_size(ren_btn, 110, 30);
  lv_obj_align_to(ren_btn, del_btn, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);
  lv_obj_t *ren_label = lv_label_create(ren_btn);
  lv_label_set_text(ren_label, "Rename");
  lv_obj_center(ren_label);
  lv_obj_add_event_cb(ren_btn, rename_file_cb, LV_EVENT_CLICKED, NULL);

  // Cut
  lv_obj_t *cut_btn = lv_btn_create(context_menu);
  lv_obj_set_size(cut_btn, 110, 30);
  lv_obj_align_to(cut_btn, ren_btn, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);
  lv_obj_t *cut_label = lv_label_create(cut_btn);
  lv_label_set_text(cut_label, "Cut");
  lv_obj_center(cut_label);
  lv_obj_add_event_cb(cut_btn, cut_file_cb, LV_EVENT_CLICKED, NULL);

  // Copy
  lv_obj_t *copy_btn = lv_btn_create(context_menu);
  lv_obj_set_size(copy_btn, 110, 30);
  lv_obj_align_to(copy_btn, cut_btn, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);
  lv_obj_t *copy_label = lv_label_create(copy_btn);
  lv_label_set_text(copy_label, "Copy");
  lv_obj_center(copy_label);
  lv_obj_add_event_cb(copy_btn, copy_file_cb, LV_EVENT_CLICKED, NULL);
}
