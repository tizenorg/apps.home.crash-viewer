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
#include <efl_extension.h>
#include "crash-viewer.h"
#include "_util_log.h"
#include "_util_efl.h"
#include "_list.h"
#include "_delete.h"

static Eina_Bool _back_cb(void *data, Elm_Object_Item *it)
{
	elm_exit();
	return EINA_TRUE;
}

int _app_create(struct appdata *ad)
{
	retvm_if(ad == NULL, -1, "Invalid argument: appdata is NULL\n");
	retvm_if(ad->win_main == NULL, -1, "win_main is NULL\n");

	elm_win_indicator_mode_set(ad->win_main, ELM_WIN_INDICATOR_SHOW);
	elm_win_indicator_opacity_set(ad->win_main, ELM_WIN_INDICATOR_OPAQUE);

	ad->conform = _add_conformant(ad->win_main);
	retvm_if(ad->conform == NULL, -1, "Failed to add conformant\n");

	ad->layout_main = _add_layout_main(ad->conform, EINA_TRUE, EINA_FALSE);
	retvm_if(ad->layout_main == NULL, -1, "Failed to add layout main\n");

	ad->bg = _add_bg(ad->layout_main, "group_list");
	retvm_if(ad->bg == NULL, -1, "Failed to add bg\n");
	elm_object_part_content_set(ad->layout_main, "elm.swallow.bg",ad->bg);

	elm_win_resize_object_add(ad->win_main, ad->bg);
	elm_win_resize_object_add(ad->win_main, ad->conform);

	elm_object_content_set(ad->conform, ad->layout_main);

	ad->indicator_bg = elm_bg_add(ad->conform);
	elm_object_style_set(ad->indicator_bg, "indicator/headerbg");
	elm_object_part_content_set(ad->conform, "elm.swallow.indicator_bg", ad->indicator_bg);
	evas_object_show(ad->indicator_bg);

	ad->nv = _add_naviframe(ad->layout_main);
	retvm_if(ad->nv == NULL, -1, "Failed to add naviframe\n");

	eext_object_event_callback_add(ad->nv, EEXT_CALLBACK_BACK, eext_naviframe_back_cb, NULL);

	ad->gl = _add_genlist(ad->nv);
	retvm_if(ad->gl == NULL, -1, "Failed to add genlist\n");
	evas_object_data_set(ad->gl, "window", ad->layout_main);

	/* create toolbar */
	ad->tb = elm_toolbar_add(ad->nv);
	retvm_if(ad->tb == NULL, -1, "Failed to add toolbar\n");
	elm_object_style_set(ad->tb, "default");
	elm_toolbar_shrink_mode_set(ad->tb, ELM_TOOLBAR_SHRINK_EXPAND);
	elm_toolbar_transverse_expanded_set(ad->tb, EINA_TRUE);
	elm_toolbar_select_mode_set(ad->tb, ELM_OBJECT_SELECT_MODE_NONE);
	ad->dbi = elm_toolbar_item_append(ad->tb, NULL, STR_DELETE_ALL, _delete_all_cb, ad);
	ad->eoi = elm_naviframe_item_push(ad->nv, STR_TITLE_CRASH_VIEWER, NULL, NULL, ad->gl, NULL);
	elm_naviframe_item_pop_cb_set(ad->eoi, _back_cb, ad);
	elm_object_item_part_content_set(ad->eoi, "toolbar", ad->tb);

	return 0;
}

