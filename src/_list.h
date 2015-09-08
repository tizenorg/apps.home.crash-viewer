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



#ifndef __CRASH_VIEWER_LIST_H__
#define __CRASH_VIEWER_LIST_H__

#include <Elementary.h>

Eina_List *_get_ulist(const char *path, void *data);
int _get_count(void);
void _set_itc(void);
int _set_glist(Eina_List *ulist, Evas_Object *gl);
void _update_glist(void *data);
//void draw_list(void *data);

#endif
/* __CRASH_VIEWER_LIST_H__ */
