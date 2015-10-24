/*
 * CRASH_VIEWER
 * Copyright (c) 2012-2013 Samsung Electronics Co., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the License);
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */



#include <sys/types.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <efl_extension.h>

#include "crash-viewer.h"
#include "_list.h"
#include "_util_log.h"
#include "_util_efl.h"

static int _delete_dir_contents(DIR *d, const char *ignore)
{
	int result = 0;
	struct dirent *de;
	int dfd;
	dfd = dirfd(d);
	if (dfd < 0) return -1;
	while ((de = readdir(d))) {
		const char *name = de->d_name;
		/* skip the ignore name if provided */
		if (ignore && !strcmp(name, ignore)) continue;
		if (de->d_type == DT_DIR) {
			int subfd;
			DIR *subdir;
			/* always skip "." and ".." */
			if (name[0] == '.') {
				if (name[1] == 0) continue;
				if ((name[1] == '.') && (name[2] == 0)) continue;
			}
			subfd = openat(dfd, name, O_RDONLY | O_DIRECTORY);
			if (subfd < 0) {
				_E("Couldn't openat %s: %s\n", name, strerror(errno));
				result = -1;
				continue;
			}
			subdir = fdopendir(subfd);
			if (subdir == NULL) {
				_E("Couldn't fdopendir %s: %s\n", name, strerror(errno));
				close(subfd);
				result = -1;
				continue;
			}
			if (_delete_dir_contents(subdir, 0)) {
				result = -1;
			}
			closedir(subdir);
			if (unlinkat(dfd, name, AT_REMOVEDIR) < 0) {
				_E("Couldn't unlinkat %s: %s\n", name, strerror(errno));
				result = -1;
			}
		} else {
			if (unlinkat(dfd, name, 0) < 0) {
				LOGE("Couldn't unlinkat %s: %s\n", name, strerror(errno));
				result = -1;
			}
		}
	}
	return result;
}

int delete_dir_contents(const char *pathname,
		int also_delete_dir,
		const char *ignore)
{
	int res = 0;
	DIR *d;

	d = opendir(pathname);
	if (d == NULL) {
		_E("Couldn't opendir %s: %s\n", pathname, strerror(errno));
		return -errno;
	}
	res = _delete_dir_contents(d, ignore);
	closedir(d);
	if (also_delete_dir) {
		if (rmdir(pathname)) {
			_E("Couldn't rmdir %s: %s\n", pathname, strerror(errno));
			res = -1;
		}
	}
	return res;
}
static void _cancel_cb(void *data, Evas_Object *obj, void *event_info)
{
	struct appdata *ad = (struct appdata *)data;

	retm_if(ad == NULL, "Invalid argument: appdata is NULL\n");
	evas_object_del(ad->popup);
	ad->popup = NULL;
}

static void _delete_cb(void *data, Evas_Object *obj, void *event_info)
{
	struct appdata *ad = (struct appdata *)data;

	retm_if(ad == NULL, "Invalid argument: appdata is NULL\n");
	ad->flag_deleting = EINA_TRUE;
	delete_dir_contents(CRASH_DUMP_PATH, 0, NULL);
	delete_dir_contents(CRASH_TICKET_PATH, 0, NULL);
	delete_dir_contents(CRASH_REPORT_PATH, 0, NULL);
	_update_glist(data);
	ad->flag_deleting = EINA_FALSE;
	evas_object_del(ad->popup);
	ad->popup = NULL;
}

void _delete_all_cb(void *data, Evas_Object *obj, void *event_info)
{
	Evas_Object *btn_delete = NULL;
	Evas_Object *btn_cancel = NULL;
	Evas_Object *popup = NULL;

	struct appdata *ad = (struct appdata *)data;
	retm_if(ad == NULL, "Invalid argument: appdata is NULL\n");

	popup = elm_popup_add(ad->nv);
	evas_object_size_hint_weight_set(popup, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	eext_object_event_callback_add(popup, EEXT_CALLBACK_BACK, eext_popup_back_cb, NULL);
	elm_object_part_text_set(popup, "title,text",  STR_DELETE_ALL);
	elm_object_text_set(popup, STR_DELETE_ALL);

	btn_cancel = elm_button_add(popup);
	elm_object_domain_translatable_text_set(btn_cancel, "sys_string", _("IDS_COM_SK_CANCEL"));
	elm_object_part_content_set(popup, "button1", btn_cancel);
	elm_object_style_set(btn_cancel, "popup_button/default");
	evas_object_smart_callback_add(btn_cancel, "clicked", _cancel_cb, ad);

	btn_delete = elm_button_add(popup);
	elm_object_domain_translatable_text_set(btn_delete, "sys_string", _("IDS_COM_SK_DELETE"));
	elm_object_part_content_set(popup, "button2", btn_delete);
	elm_object_style_set(btn_delete, "popup_button/default");
	evas_object_smart_callback_add(btn_delete, "clicked", _delete_cb, ad);

	evas_object_show(popup);
	ad->popup = popup;
}
