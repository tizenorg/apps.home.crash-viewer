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



#include "crash-viewer.h"
#include "_util_log.h"
#include "_util_efl.h"


void _launch_popup(Evas_Object *parent, char *text)
{
	Evas_Object *pu;
	char buf[256] = {0, };

	pu = _add_popup(parent);
	retm_if(pu == NULL, "Failed to add popup\n");

	elm_popup_timeout_set(pu, 5.0);
	elm_object_part_text_set(pu, "title,text", "Error!");
	snprintf(buf, sizeof(buf), "Failed to read cs file:<br>%s", text);
	elm_object_text_set(pu, buf);

}
