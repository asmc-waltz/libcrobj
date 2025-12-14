/**
 * @file crobj_widget_keyboard.h
 *
 */

#ifndef G_CROBJ_WIDGET_KEYBOARD_H
#define G_CROBJ_WIDGET_KEYBOARD_H
/*********************
 *      INCLUDES
 *********************/
#include <stdint.h>

#include <lvgl.h>
#include "../crobj_core.h"

/*********************
 *      DEFINES
 *********************/
#define KEYBOARD_CHAR_FONTS             &lv_font_montserrat_24

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
lv_obj_t *create_keyboard(lv_obj_t *par, const char *name);
int32_t remove_keyboard(void);

/**********************
 *      MACROS
 **********************/

#endif /* G_CROBJ_WIDGET_KEYBOARD_H */
