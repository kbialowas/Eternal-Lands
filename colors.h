/*!
 * \file
 * \ingroup other
 * \brief color handling
 */
#ifndef __COLORS_H__
#define __COLORS_H__

#include <SDL_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * A color structure for RGB color values.
 */
typedef struct
{
	Uint8 r1;
	Uint8 g1;
	Uint8 b1;
	Uint8 r2;
	Uint8 g2;
	Uint8 b2;
	Uint8 r3;
	Uint8 g3;
	Uint8 b3;
	Uint8 r4;
	Uint8 g4;
	Uint8 b4;

} color_rgb;

extern color_rgb colors_list[30]; /*!< the global list of colors we use */

/*!
 * \ingroup other
 * \brief initializes the colors_list color table of EL.
 *
 *      Initializes the colors_list color table of EL.
 *
 * \sa init_stuff
 */
void init_colors();

#ifdef __cplusplus
} // extern "C"
#endif

#endif
