/**
 * @file crobj_widget_common.h
 *
 */

#ifndef G_CROBJ_WIDGET_COMMON_H
#define G_CROBJ_WIDGET_COMMON_H
/*********************
 *      INCLUDES
 *********************/
#include <stdint.h>

#include <lvgl.h>
#include "../crobj_core.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  GLOBAL VARIABLES
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/
/*=====================
 * Setter functions
 *====================*/

/*=====================
 * Getter functions
 *====================*/

/*=====================
 * Other functions
 *====================*/
lv_obj_t *create_vertical_flex_group(lv_obj_t *par, const char *name);
lv_obj_t *create_vscroll_flex_group(lv_obj_t *par, const char *name);
lv_obj_t *create_horizontal_flex_group(lv_obj_t *par, const char *name);
lv_obj_t *create_hscroll_flex_group(lv_obj_t *par, const char *name);

/**********************
 *      MACROS
 **********************/

#endif /* G_CROBJ_WIDGET_COMMON_H */
