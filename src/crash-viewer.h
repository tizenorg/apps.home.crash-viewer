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



#ifndef __CRASH_VIEWER_H__
#define __CRASH_VIEWER_H__

#include <Elementary.h>

#if !defined(PACKAGE)
#define PACKAGE "crash-viewer"
#endif

#if !defined(RESDIR)
#  define RESDIR "/usr/apps/org.tizen.crash-viewer/res"
#endif

#if !defined(LOCALEDIR)
#  define LOCALEDIR RESDIR"/locale"
#endif

#define EDJ_FILE	EDJDIR"/"PACKAGE".edj"

#define CRASH_NOTI_DIR      "/opt/share/crash"
#define CRASH_NOTI_FILE     "curbs.log"
#define CRASH_NOTI_PATH CRASH_NOTI_DIR"/"CRASH_NOTI_FILE
#define CRASH_DUMP_PATH   "/opt/usr/share/crash/dump"
#define CRASH_TICKET_PATH   "/opt/usr/share/crash/ticket"
#define CRASH_REPORT_PATH   "/opt/usr/share/crash/report"

#define STR_TITLE_CRASH_VIEWER	gettext("IDS_ST_BODY_CRASH_VIEWER")
#define STR_DELETE_ALL gettext("IDS_COM_BODY_DELETE_ALL")
#define STR_NO_FILES gettext("IDS_COM_BODY_NO_FILES")
#define _EDJ(o) elm_layout_edje_get(o)
#define CRASH_VIEWER_UPDATE "crash_viewer_update"
#define _(str) gettext(str)

struct label_data_t {
	char *file_path;
	char *date;
	char *file_name;
	struct appdata *ad;
};

struct appdata {
	Evas *evas;
	Evas_Object *win_main;
	Evas_Object *bg;
	Evas_Object *indicator_bg;
	Evas_Object *conform;
	Evas_Object *layout_main;
	Evas_Object *nv;
	Evas_Object *popup;
	Evas_Object *gl;
	Evas_Object *tb;
	Elm_Object_Item *eoi, *dbi;
	Ecore_Timer *timer_popup;
	Eina_List *ulist;
	int flag_deleting;
	/* add more variables here */

};

#endif				/* __CRASH_VIEWER_H__ */
