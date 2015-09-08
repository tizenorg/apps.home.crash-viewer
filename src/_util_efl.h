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



#ifndef __CRASH_VIEWER_UTIL_EFL_H__
#define __CRASH_VIEWER_UTIL_EFL_H__

Evas_Object *_add_window(const char *name);
Evas_Object *_add_bg(Evas_Object *parent, char *style);
Evas_Object *_add_conformant(Evas_Object *parent);
Evas_Object *_add_naviframe(Evas_Object *parent);
Evas_Object *_add_layout_main(Evas_Object *parent,
		Eina_Bool content, Eina_Bool transparent);
Evas_Object *_add_layout(Evas_Object *parent,
		const char *file, const char *group);
Evas_Object *_add_controlbar(Evas_Object *parent, const char *style);
Evas_Object *_add_scroller(Evas_Object *parent,
		Eina_Bool h_bounce, Eina_Bool v_bounce);
Evas_Object *_add_button(Evas_Object *parent, const char *style);
Evas_Object *_add_popup(Evas_Object *parent);
Evas_Object *_add_progressbar(Evas_Object *parent, const char *style);
Evas_Object *_add_label(Evas_Object *parent, char *text);
Evas_Object *_add_genlist(Evas_Object *parent);

#endif
/* __CRASH_VIEWER_UTIL_EFL_H__ */

