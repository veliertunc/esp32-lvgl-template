idf_component_register(
  SRCS
  "src/file_browser.c src/file_preview.c src/folder_ops.c"
  INCLUDE_DIRS
  "include"
  REQUIRES
  lvgl
  driver
  sdcard)
