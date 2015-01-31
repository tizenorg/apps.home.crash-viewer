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



#include <Elementary.h>
#include <Ecore_X.h>
#include "_util_log.h"

Evas_Object *_add_window(const char *name)
{
	Evas_Object *eo;
	int w, h;

	eo = elm_win_add(NULL, name, ELM_WIN_BASIC);
	if (eo) {
		elm_win_title_set(eo, name);
		elm_win_borderless_set(eo, EINA_TRUE);
		ecore_x_window_size_get(
				ecore_x_window_root_first_get(),
				&w, &h);
		evas_object_resize(eo, w, h);
	}

	return eo;
}

Evas_Object *_add_bg(Evas_Object *parent, char *style)
{
	Evas_Object *bg;

	bg = elm_bg_add(parent);
	retvm_if(bg == NULL, NULL, "Failed to add bg\n");
	if (style)	elm_object_style_set(bg, style);
	evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND,
					 EVAS_HINT_EXPAND);
	evas_object_show(bg);
	return bg;
}

Evas_Object *_add_conformant(Evas_Object *parent)
{
	Evas_Object *conform;
	retvm_if(parent == NULL, NULL, "input argument is NULL\n");
	conform = elm_conformant_add(parent);
	retvm_if(conform == NULL, NULL, "Failed to add conform\n");
	evas_object_size_hint_weight_set(conform, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_win_resize_object_add(parent, conform);
	evas_object_show(conform);
	return conform;
}

Evas_Object *_add_naviframe(Evas_Object *parent)
{
	Evas_Object *nv;
	retvm_if(parent == NULL, NULL, "input argument is NULL\n");
	nv = elm_naviframe_add(parent);
	retvm_if(nv == NULL, NULL, "Failed to add navigationbar\n");
	elm_object_part_content_set(parent, "elm.swallow.content", nv);
	evas_object_show(nv);
	return nv;
}

Evas_Object *_add_layout_main(Evas_Object *parent,
		Eina_Bool content, Eina_Bool transparent)
{
	Evas_Object *ly;

	ly = elm_layout_add(parent);
	retvm_if(ly == NULL, NULL, "Failed to add main layout\n");

	elm_layout_theme_set(ly, "layout", "application", "default");
	elm_object_style_set(ly,"dialogue");
	evas_object_size_hint_weight_set(ly, EVAS_HINT_EXPAND,
					 EVAS_HINT_EXPAND);
	if (content)
		elm_object_signal_emit(ly, "elm,state,show,content", "elm");
	if (transparent)
		elm_object_signal_emit(ly, "elm,bg,show,transparent", "elm");
	evas_object_show(ly);

	return ly;
}


Evas_Object *_add_layout(Evas_Object *parent, const char *file,
			     const char *group)
{
	Evas_Object *eo;
	int r;

	retvm_if(parent == NULL, NULL, "Invalid argument: parent is NULL\n");
	retvm_if(file == NULL, NULL, "Invalid argument: file is NULL\n");
	retvm_if(group == NULL, NULL, "Invalid argument: group is NULL\n");

	eo = elm_layout_add(parent);
	retvm_if(eo == NULL, NULL, "Failed to add layout\n");

	r = elm_layout_file_set(eo, file, group);
	if (!r) {
		_E("Failed to set file[%s]\n", file);
		evas_object_del(eo);
		return NULL;
	}

	evas_object_size_hint_weight_set(eo,
			EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	return eo;
}

Evas_Object *_add_controlbar(Evas_Object *parent, const char *style)
{
	Evas_Object *cb;

	cb = elm_toolbar_add(parent);
	retvm_if(cb == NULL, NULL, "Failed to add toolbar\n");
	elm_toolbar_shrink_mode_set(cb, ELM_TOOLBAR_SHRINK_EXPAND);
//	elm_toolbar_select_mode_set(cb, ELM_OBJECT_SELECT_MODE_ALWAYS);
	elm_toolbar_homogeneous_set(cb, EINA_FALSE);
	if (style) elm_object_style_set(cb, style);
	evas_object_show(cb);

	return cb;
}

Evas_Object *_add_scroller(Evas_Object *parent,
		Eina_Bool h_bounce, Eina_Bool v_bounce)
{
	Evas_Object *sc;

	sc = elm_scroller_add(parent);
	evas_object_size_hint_weight_set(sc, EVAS_HINT_EXPAND,
					 EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(sc, EVAS_HINT_FILL, EVAS_HINT_FILL);
	elm_scroller_bounce_set(sc, h_bounce, v_bounce);
	elm_scroller_policy_set(sc,
				h_bounce ? ELM_SCROLLER_POLICY_AUTO :
				ELM_SCROLLER_POLICY_OFF,
				v_bounce ? ELM_SCROLLER_POLICY_AUTO :
				ELM_SCROLLER_POLICY_OFF);
	evas_object_show(sc);
	return sc;
}


Evas_Object *_add_button(Evas_Object *parent, const char *style)
{
	Evas_Object *bt;
	bt = elm_button_add(parent);
	retvm_if(bt == NULL, NULL, "Failed to add button\n");
	elm_object_style_set(bt, style);
	elm_object_focus_set(bt, EINA_FALSE);
	evas_object_show(bt);
	return bt;
}

Evas_Object *_add_popup(Evas_Object *parent)
{
	Evas_Object *pu;

	pu = elm_popup_add(parent);
	if (pu == NULL) {
		fprintf(stderr, "[Error] Failed to add popup\n");
		return NULL;
	}
	evas_object_size_hint_weight_set(pu,
			EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_show(pu);

	return pu;
}

Evas_Object *_add_progressbar(Evas_Object *parent, const char *style)
{
	Evas_Object *pb;

	pb = elm_progressbar_add(parent);
	if(style)	elm_object_style_set(pb, style);
	evas_object_size_hint_weight_set(pb,
			EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_show(pb);

	return pb;
}

Evas_Object *_add_label(Evas_Object *parent, char *text)
{
	Evas_Object *lb;

	lb = elm_label_add(parent);
	retvm_if(lb == NULL, NULL, "Failed to add label\n");
	elm_object_text_set(lb, text);
	evas_object_size_hint_weight_set(lb,
		EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_show(lb);
	return lb;
}

Evas_Object *_add_genlist(Evas_Object *parent)
{
	Evas_Object *gl;

	gl = elm_genlist_add(parent);
	elm_genlist_mode_set(gl, ELM_LIST_COMPRESS);
	retvm_if(gl == NULL, NULL, "Failed to add genlist\n");
	evas_object_size_hint_weight_set(gl,
		EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_show(gl);
	return gl;
}

