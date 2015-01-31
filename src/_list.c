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



#include <Eina.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#include "crash-viewer.h"
#include "_util_log.h"
#include "_util_efl.h"
#include "_popup.h"
#include "_delete.h"

#define LINEBUF 256
#define LINEMAX 400

static Elm_Genlist_Item_Class itc;
static Elm_Genlist_Item_Class itc_0;
const Elm_Genlist_Item_Class seperator = {
	.item_style = "dialogue/separator",
	.func.text_get = NULL,
	.func.content_get = NULL,
	.func.state_get = NULL,
	.func.del = NULL,
};

struct u_data {
	char name[256];
	char time[256];
	Elm_Object_Item *egi;
	void *data;
};

int sort_cb(const void *d1, const void *d2)
{
	struct u_data *data1 = (struct u_data *)d1;
	struct u_data *data2 = (struct u_data *)d2;
	if (!data1) return 1;
	if (!data2) return -1;
	return (strcmp(data1->time, data2->time));
}

void _update_glist(void *data);

static char *gl_0_label_get(void *data, Evas_Object *obj, const char *part)
{
	if (!strcmp(part, "elm.text")) {
		return strdup(STR_NO_FILES);

	}
	return NULL;
}

static void gl_0_sel(void *data, Evas_Object *obj, void *event_info)
{
	retm_if(event_info == NULL, "Invalid argument: event_info is NULL\n");
	Elm_Object_Item *it = (Elm_Object_Item *)event_info;
	if (it != NULL) {
		elm_genlist_item_selected_set(it, EINA_FALSE);
	}

}

int _mod_string(char *str)
{
	int i, len;

	retvm_if(str == NULL, -1, "Invalid argument: str is NULL\n");
	len = strlen(str);
	for (i = 0; i < len; i++) {
		if (str[i] == '{') {
			break;
		}
	}
	snprintf(str, len - i, "%s%c", &str[i], '\0');
	return 0;
}

static char *gl_label_get(void *data, Evas_Object *obj, const char *part)
{
	struct u_data *ud = (struct u_data *)data;;
	retvm_if(ud == NULL, NULL, "Invlaid argument: u_data is NULL\n");

	if (!strcmp(part, "elm.text.main.left.top")) {
		_D("%s\n", ud->name);
		return strdup(ud->name ? ud->name : "Failed to get name");

	} else  if (!strcmp(part, "elm.text.sub.left.bottom")) {
		_D("%s\n", ud->time);
		return strdup(ud->time ? ud->time : "Failed to get time");

	}
	return NULL;
}

void _clicked_cb(void* data, Evas_Object* obj, void* event_info)
{
	struct u_data *ud = (struct u_data *)data;;
	retm_if(ud == NULL, "Invlaid argument: u_data is NULL\n");

	elm_genlist_item_update(ud->egi);
}

static Eina_Bool _unselect_item(void *data)
{
	Elm_Object_Item *it = (Elm_Object_Item *)data;
	retvm_if(it == NULL, ECORE_CALLBACK_CANCEL,
			"Invalid arugument: Elm_Object_Item is NULL\n");
	elm_genlist_item_selected_set(it, EINA_FALSE);

	return ECORE_CALLBACK_CANCEL;
}
#define CRASH_MAPSINFO_TITLE "Maps Information"
#define CRASH_MAPSINFO_TITLE_E "End of Maps Information"
char *_get_screen_text(char *file)
{
	FILE *fp;
	char buf[LINEBUF] = {0, };
	int fsize, flag_skip = 0, copying_size = 0, i;
	int count = 0;
	int size_prop, size_br;
	char *fbuf;
	char *prop = "<font_size=25><font color=#000011>";
	retvm_if(file == NULL, NULL, "_get_screen_text input file is null \n");
	size_prop = strlen(prop);
	size_br = strlen("<br> ");

	fp = fopen(file, "rb");
	retvm_if(fp == NULL, NULL, "Failed to fopen (%s)\n", file);
	fseek(fp, 0, SEEK_END);
	fsize = ftell(fp);
	_D("fsize(%d)\n", fsize);
	fbuf = calloc(fsize + 2048, 1);
	if (fbuf == NULL) {
		fclose(fp);
		_D("can't calloc\n");
		return NULL;
	}
	snprintf(fbuf, fsize + 2048, "%s", prop);
	fseek(fp, 0, SEEK_SET);

	while (fgets(buf, sizeof(buf), fp)) {
		char *p = strchr(buf, '\n');
		if (p)
			*p = '\0';
		else
			buf[sizeof(buf) - 1] = '\0';
		copying_size = strlen(buf);
		if (!copying_size) continue;

		if (!strncmp(CRASH_MAPSINFO_TITLE, buf, strlen(CRASH_MAPSINFO_TITLE))) {
			flag_skip = 1;
		} else if (!strncmp(CRASH_MAPSINFO_TITLE_E, buf, strlen(CRASH_MAPSINFO_TITLE_E))) {
			flag_skip = 0;
			continue;
		}

		if (flag_skip == 1) {
			continue;
		}

		/* delete unprintable value */
		for (i = 0; i < copying_size; i++) {
			if (!isprint(buf[i])) {
				buf[i] = 0;
				copying_size = i;
				break;
			}
		}

		sprintf(fbuf + size_prop, "<br> %s", buf);
		size_prop += copying_size + size_br;
		count++;

		if (count > LINEMAX) {
			break;
		}

	}
	fclose(fp);

	return fbuf;
}

Evas_Object *_get_screen(Evas_Object *parent, char *text)
{
	Evas_Object *ly, *sc, *lb;

	retvm_if(parent == NULL, NULL, "Invalid argument: parent is NULL\n");
	retvm_if(text == NULL, NULL, "Invalid argument: text is NULL\n");

	ly = _add_layout(parent, EDJ_FILE, "main");
	retvm_if(ly == NULL, NULL, "Failed to add layout(%s)\n", EDJ_FILE);

	sc = _add_scroller(ly, EINA_FALSE, EINA_TRUE);
	retvm_if(sc == NULL, NULL, "Failed to add scroller\n");
	elm_object_part_content_set(ly, "swallow/text", sc);

	lb = _add_label(sc, text);
	retvm_if(lb == NULL, NULL, "Failed to add label\n");
	elm_object_content_set(sc, lb);

	return ly;
}

static void gl_sel(void *data, Evas_Object *obj, void *event_info)
{
	char buf[256];
	char *text;
	struct u_data *ud = (struct u_data *)data;
	struct appdata *ad = (struct appdata *)ud->data;
	Evas_Object *ly;

	retm_if(ud == NULL, "Invlaid argument: u_data is NULL\n");
	retm_if(ad == NULL, "Invlaid argument: appdata is NULL\n");

	ecore_timer_add(0.5, _unselect_item, event_info);

	snprintf(buf, sizeof(buf), "%s/%s",
			CRASH_REPORT_PATH, ud->name);
	_D("cs file (%s)\n", buf);

	text = _get_screen_text(buf);
	ly = _get_screen(obj, text);
	elm_naviframe_item_push(ad->nv, ud->name, NULL, NULL, ly, NULL);

	if (text) {
		free(text);
	}
}

Eina_Bool gl_state_get(void *data, Evas_Object *obj, const char *part)
{
	return EINA_FALSE;
}

void _set_itc(void)
{
	/* create genlist */
	itc_0.item_style = "default";
	itc_0.func.text_get = gl_0_label_get;
	itc.item_style = "2line.top";
	itc.func.text_get = gl_label_get;
	itc.func.content_get = NULL;
	itc.func.state_get = NULL;
	itc.func.del = NULL;
}

int _check_is_csfile(const char *file)
{
	retvm_if(file == NULL, -1, "Invalid argument: path is NULL\n:");
	if (strstr(file, ".cs") == NULL)
		return 0;
	return 1;
}

Eina_List *_get_ulist(const char *path, void *data)
{
	Eina_List *ulist = NULL;

	DIR *dp;
	struct dirent *de;
	struct u_data *ud = NULL;
	int len;

	dp = opendir(path);
	retvm_if(dp == NULL, NULL, "Failed to open dir %s\n", path);

	while ((de = readdir(dp))) {
		if (!strcmp(de->d_name, ".") || !strcmp(de->d_name, ".."))
			continue;
		if (_check_is_csfile(de->d_name) == 0)
			continue;
		len = strlen(de->d_name);
		ud = calloc(1, sizeof(struct u_data));
		if (ud == NULL) {
			_E("Failed to calloc\n");
			break;
		}
		/* get name */
		snprintf(ud->name, sizeof(ud->name), "%s", de->d_name);
		_D("name[%s]\n", de->d_name);

		/* get time */
		snprintf(ud->time, sizeof(ud->time), "%c%c/%c%c/%c%c %c%c:%c%c:%c%c",
				de->d_name[len-15], de->d_name[len-14],	/* year */
				de->d_name[len-13], de->d_name[len-12],	/* month */
				de->d_name[len-11], de->d_name[len-10],	/* day */
				de->d_name[len-9], de->d_name[len-8],	/* hour */
				de->d_name[len-7], de->d_name[len-6],	/* min */
				de->d_name[len-5], de->d_name[len-4]);	/* sec */
		_D("time:[%s]\n", ud->time);

		ud->data = data;
		ulist = eina_list_sorted_insert(ulist, sort_cb ,ud);
	}
	closedir(dp);
	return ulist;
}

int _set_glist(Eina_List *ulist, Evas_Object *gl)
{
	Eina_List *l, *l_next;
	struct u_data *ud = NULL;
	int cnt = 0;
	Elm_Object_Item *egi, *item;

	retvm_if(gl == NULL, -1, "Invalid argument: gl is NULL\n");

	item = elm_genlist_item_append(gl, &seperator,
			(void *)0, NULL, ELM_GENLIST_ITEM_NONE, NULL, NULL);
	elm_genlist_item_select_mode_set(item,
			ELM_OBJECT_SELECT_MODE_DISPLAY_ONLY);

	if (ulist == NULL || eina_list_count(ulist) == 0) {
		egi = elm_genlist_item_append(gl, &itc_0, NULL,
				NULL, ELM_GENLIST_ITEM_NONE,
				gl_0_sel, NULL);
		elm_object_item_disabled_set(egi, EINA_TRUE);
		return cnt;
	}

	ulist = eina_list_nth_list(ulist, 0);
	EINA_LIST_FOREACH_SAFE(ulist, l, l_next, ud) {
		if (ud != NULL) {
			_D("name[%s]\n", ud->name);
		egi = elm_genlist_item_append(gl,
				&itc, (void *)ud,
				NULL, ELM_GENLIST_ITEM_NONE,
				gl_sel, (void *)ud);
				ud->egi = egi;
		cnt++;
		}
	}
	return cnt;
}

int _get_count(void)
{
	DIR *dp;
	struct dirent *de;
	char buf[256] = {0, };
	int count = 0;

	dp = opendir(CRASH_REPORT_PATH);
	retvm_if(dp == NULL, 0, "Failed to open dir %s\n", CRASH_REPORT_PATH);

	while ((de = readdir(dp))) {
		if (!strcmp(de->d_name, ".") || !strcmp(de->d_name, ".."))
			continue;
		snprintf(buf, sizeof(buf), "%s/%s", CRASH_REPORT_PATH, de->d_name);
		if (access(buf,F_OK) == 0)
			count++;
	}
	closedir(dp);
	return count;
}

void _update_glist(void *data)
{
	Eina_List *l;
	struct u_data *ud = NULL;
	int cnt = 0;
	int all_count;
	char buf[256] = {0, };
	struct appdata *ad = (struct appdata *)data;
	if (ad == NULL) {
		_D("Invalid argument: appdata is NULL\n");
		return;
	}

	if (ad->ulist) {
		EINA_LIST_FOREACH(ad->ulist, l, ud) {
			if (ud) {
				_D("free\n");
				free(ud);
			}
		}
		ad->ulist = eina_list_free(ad->ulist);
		ad->ulist = NULL;
	}
	ad->ulist = _get_ulist(CRASH_REPORT_PATH, ad);
	elm_genlist_clear(ad->gl);
	cnt = _set_glist(ad->ulist, ad->gl);
	if (cnt < 0)
		cnt = 0;
	/* update title & delete button*/
	all_count = _get_count();
	if (all_count < 0)
		all_count = 0;
	snprintf(buf, sizeof(buf), "%s (%d)", STR_TITLE_CRASH_VIEWER, cnt);
	elm_object_item_text_set(ad->eoi, buf);
	elm_object_item_disabled_set(ad->dbi, all_count ? EINA_FALSE:EINA_TRUE);
}
