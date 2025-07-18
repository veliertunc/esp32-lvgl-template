#include "file_ops.h"
#include "common.h"

static lv_obj_t *context_menu = NULL;
static char selected_file_path[PATH_MAX_LENGTH];

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

  // You can add a LVGL text input dialog here for new name and then rename()
}

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
}
