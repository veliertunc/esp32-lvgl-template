#include "file_preview.h"
#include "common.h"

#define BUFFER_LENGTH 256

static lv_obj_t *preview_popup = NULL;

static void close_preview_cb(lv_event_t *e) {
  if (preview_popup) {
    lv_obj_del(preview_popup);
    preview_popup = NULL;
  }
}

static void show_file_preview(const char *filepath) {
  if (preview_popup) {
    lv_obj_del(preview_popup);
    preview_popup = NULL;
  }

  preview_popup = lv_obj_create(lv_scr_act());
  lv_obj_set_size(preview_popup, lv_pct(80), lv_pct(60));
  lv_obj_center(preview_popup);

  lv_obj_t *ta = lv_textarea_create(preview_popup);
  lv_obj_set_size(ta, lv_pct(100), lv_pct(100));
  lv_textarea_set_text(ta, "Loading...");

  // Close button
  lv_obj_t *btn_close = lv_btn_create(preview_popup);
  lv_obj_set_size(btn_close, 60, 30);
  lv_obj_align(btn_close, LV_ALIGN_TOP_RIGHT, -5, 5);
  lv_obj_t *label = lv_label_create(btn_close);
  lv_label_set_text(label, "Close");
  lv_obj_center(label);
  lv_obj_add_event_cb(btn_close, close_preview_cb, LV_EVENT_CLICKED, NULL);

  FILE *f = fopen(filepath, "r");
  if (!f) {
    lv_textarea_set_text(ta, "Failed to open file");
    return;
  }

  char buf[BUFFER_LENGTH + 1] = {0};
  size_t r = fread(buf, 1, BUFFER_LENGTH, f);
  fclose(f);

  lv_textarea_set_text(ta, buf);
}
