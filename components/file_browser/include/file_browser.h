#ifndef FILE_BROWSER_H
#define FILE_BROWSER_H

#include "misc/lv_types.h"

void file_browser_init(lv_obj_t *);
static void list_files_in_dir(const char *);
static void go_to_path(const char *);

#endif // !FILE_BROWSER_H
