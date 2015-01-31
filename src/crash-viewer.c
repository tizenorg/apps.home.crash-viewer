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



#include <stdio.h>
#include <errno.h>
#include <appcore-efl.h>
#include <Ecore_X.h>

#include "crash-viewer.h"
#include "_util_log.h"
#include "_util_efl.h"
#include "_list.h"
#include "_logic.h"

static int app_create(void *data)
{
	int r;
	struct appdata *ad = data;

	retvm_if(ad == NULL, -1, "Invalid argument: appdata is NULL\n");

	ad->gl = NULL;
	_D("PROF: measure start: app_init");

	/* create window */
	ad->win_main = _add_window(PACKAGE);
	retvm_if(ad->win_main == NULL, -1, "Failed to create window\n");

	/* init internationalization */
	r = appcore_set_i18n(PACKAGE, LOCALEDIR);
	retvm_if(r < 0, -1, "Failed to set i18\n");

	/* set the rotation property */
	if (elm_win_wm_rotation_supported_get(ad->win_main)) {
		int rots[4] = { 0, 90, 180, 270 };
		elm_win_wm_rotation_available_rotations_set(ad->win_main, (const int*)(&rots), 4);
	}

	evas_object_show(ad->win_main);

	_app_create(ad);

	_set_itc();

	_update_glist(ad);

	/* appcore measure time example */
	_D("from AUL to %s(): %d msec\n", __func__,
			appcore_measure_time_from("APP_START_TIME"));

	appcore_measure_start();
	return 0;
}

static int app_pause(void *data)
{
	elm_exit();
	return 0;
}

static int app_resume(void *data)
{
	return 0;
}

static int app_reset(bundle *b, void *data)
{
	struct appdata *ad = data;
	retvm_if(ad == NULL, -1, "Invalid argument: appdata is NULL\n");
	if (ad->win_main) {
		elm_win_raise(ad->win_main);
	}
	return 0;
}

int app_terminate(void *data)
{
	struct appdata *ad = data;

	evas_object_del(ad->gl);

	exit(0);
	return 0;
}

int main(int argc, char *argv[])
{
	struct appdata ad;
	struct appcore_ops ops = {
		.create = app_create,
		.terminate = app_terminate,
		.pause = app_pause,
		.resume = app_resume,
		.reset = app_reset,
	};
	if (!ecore_x_init(NULL)) {
		_E("Cannot connect to X11 display\n");
		return -1;
	}
	_D("PROF: link & load time: %d msec\n", appcore_measure_time_from("APP_START_TIME"));

	memset(&ad, 0x0, sizeof(struct appdata));

	ops.data = &ad;

	return appcore_efl_main(PACKAGE, &argc, &argv, &ops);
}
