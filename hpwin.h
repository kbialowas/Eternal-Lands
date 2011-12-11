#ifndef __HPWIN_H__
#define __HPWIN_H__

#include "elwindows.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * Display health/etheral points window
 */ 
void display_hp_window();

/*!
 * Health/ethereal points window ID
 */
extern int hp_win;


extern int hp_win_x;
extern int hp_win_y;

#ifdef __cplusplus
} // extern "C"
#endif

#endif //__HPWIN_H__
