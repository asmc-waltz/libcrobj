/**
 * @file crobj_core.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
// #define LOG_LEVEL LOG_LEVEL_TRACE
#if defined(LOG_LEVEL)
#warning "LOG_LEVEL defined locally will override the global setting in this file"
#endif
#include "../include/log.h"

#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>

#include <lvgl.h>
#include "../include/crobj_core.h"
#include "../include/crobj_scr.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  GLOBAL VARIABLES
 **********************/
static gui_ctx_t *ui_rdata = NULL;

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   STATIC FUNCTIONS
 **********************/
static lv_display_t *sf_init_drm_display(const char *file, \
                                         int64_t connector_id)
{
    lv_display_t *disp = NULL;
    int32_t scr_width = 0;
    int32_t scr_height = 0;

    scr_width = get_scr_width();
    scr_height = get_scr_height();
    if (scr_width <= 0 || scr_height <= 0) {
        LOG_FATAL("Display width or height resolution not available");
        return NULL;
    }

    disp = lv_linux_drm_create();
    if (disp == NULL) {
        LOG_FATAL("Failed to initialize the display");
        return NULL;
    }

    lv_display_set_default(disp);
    lv_linux_drm_set_file(disp, file, connector_id);
    lv_display_set_resolution(disp, scr_width, scr_height);

    return disp;
}

static lv_indev_t *sf_init_touch_screen(const char *dev_path, \
                                        lv_display_t *disp)
{
    lv_indev_t *indev = NULL;

    indev = lv_evdev_create(LV_INDEV_TYPE_POINTER, dev_path);
    if (!indev) {
        LOG_FATAL("Failed to initialize touch input device");
        return NULL;
    }

    lv_indev_set_display(indev, disp);

    return indev;
}

static void gtimer_handler(lv_timer_t *timer)
{
    gui_ctx_t *g_ctx = lv_timer_get_user_data(timer);
    lv_tick_inc(g_ctx->interval);
}

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
int32_t set_ui_ctx(gui_ctx_t *g_ctx)
{
    if (!g_ctx)
        return -EINVAL;

    ui_rdata = g_ctx;
    return 0;
}

gui_ctx_t *get_ui_ctx()
{
    return ui_rdata? ui_rdata : NULL;
}

int32_t ui_main_init(gui_ctx_t **rg_ctx)
{
    gui_ctx_t *g_ctx = NULL;
    lv_obj_t *com_scr = NULL;
    obj_meta_t *meta = NULL;
    int32_t ret;

    if (!rg_ctx)
        return -EINVAL;

    g_ctx = (gui_ctx_t *)calloc(1, sizeof(gui_ctx_t));
    if (!g_ctx)
        return -ENOMEM;
    else {
        *rg_ctx = g_ctx;
        ret = set_ui_ctx(g_ctx);
        if (ret)
            goto ctx_err;
    }

    ret = init_ui_object_ctx(&g_ctx->objs);
    if (ret) {
        LOG_FATAL("Unable to init ui object list head");
        goto ctx_err;
    }

    g_ctx->objs.next_id = 1;

    set_scr_size(DISP_WIDTH, DISP_HEIGHT);

    // Initialize LVGL and the associated UI hardware
    lv_init();
    g_ctx->scr.drm_disp = sf_init_drm_display(DRM_CARD, DRM_CONNECTOR_ID);
    if (g_ctx->scr.drm_disp == NULL) {
        ret = -EIO;
        goto init_err;
    }

    g_ctx->scr.touch_event = sf_init_touch_screen(TOUCH_EVENT_FILE, \
                                                g_ctx->scr.drm_disp);
    if (g_ctx->scr.touch_event == NULL) {
        ret = -EIO;
        goto init_err;
    }

    g_ctx->interval = UI_LVGL_TIMER_MS;
    g_ctx->timer = lv_timer_create(gtimer_handler, g_ctx->interval, g_ctx);
    if (g_ctx->timer == NULL) {
        LOG_FATAL("Failed to create timer for LVGL task handler");
        ret = -ENOMEM;
        goto init_err;
    }

    // Make lv_timer ready. It will not wait its period.
    lv_timer_ready(g_ctx->timer);

    // Initialize LVGL layers as base components
    meta = register_obj(NULL, lv_layer_sys(), NULL);
    if (meta) {
        meta->theme.level = 0;
    } else {
        LOG_FATAL("Register system layout metadata failed");
        ret = -ENOMEM;
        goto init_err;
    }

    meta = register_obj(NULL, lv_layer_top(), NULL);
    if (meta) {
        meta->theme.level = 0;
    } else {
        LOG_FATAL("Register top layer metadata failed");
        ret = -ENOMEM;
        goto init_err;
    }

    meta = register_obj(NULL, lv_screen_active(), NULL);
    if (meta) {
        meta->theme.level = 0;
    } else {
        LOG_FATAL("Register screen active metadata failed");
        ret = -ENOMEM;
        goto init_err;
    }

    meta = register_obj(NULL, lv_layer_bottom(), NULL);
    if (meta) {
        meta->theme.level = 0;
    } else {
        LOG_FATAL("Register bottom layer metadata failed");
        ret = -ENOMEM;
        goto init_err;
    }

    com_scr = create_common_screen(g_ctx, lv_screen_active(), LAYOUT_SETTING);
    if (!com_scr) {
        LOG_ERROR("Failed to create main screen, no content can be displayed");
        ret = -EIO;
        goto init_err;
    }

    LOG_DEBUG("size of obj_meta_t: %d", sizeof(obj_meta_t));
    LOG_DEBUG("size of gui_ctx_t: %d", sizeof(gui_ctx_t));

    return 0;

init_err:
    destroy_ui_object_ctx(g_ctx);
ctx_err:
    free(g_ctx);
    return ret;
}

void ui_main_deinit(gui_ctx_t *g_ctx)
{
    destroy_ui_object_ctx(g_ctx);
    // TODO: Free gui context data
}
