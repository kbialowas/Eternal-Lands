/**
 * @file
 * @ingroup maps
 * @brief loading, saving and handling of maps
 */
#ifndef	_MAP_H_
#define	_MAP_H_

#include "io/map_io.h"

#ifdef __cplusplus
extern "C" {
#endif

extern Uint32 map_flags;/**< The map flags - basically this will be obsolete with the next map format, but untill then it's good to have*/

extern char dungeon; /**< inidicates whether we are in a dungeon (no sun) or not */

/** @name ambient color values 
 * @{ */
extern float ambient_r;
extern float ambient_g;
extern float ambient_b;
/** @} */

extern int map_type; /**< id of the type of map we are currently using */

/**
 * @ingroup maps
 * @brief Loads an empty map
 *
 * 	Loads an empty map in case no other map file was found
 *
 * @param name the filename of the map that failed to load.
 * @retval int  0 if nomap.elm failed to load, otherwise 1 is returned.
 */
int load_empty_map();

/**
 * @ingroup maps
 * @brief Changes the current map
 *
 * 	Loads the given map, destroys sound objects etc.
 *
 * @param mapname The name of the map
 */
void change_map (const char * mapname);

/**
 * @ingroup maps
 * @brief Loads the given map marks for the given map
 *
 * 	Loads the map marks for the given map
 *
 */
void load_map_marks();

/**
 * @ingroup maps
 * @brief Adds a number of 3d objects to the map
 *
 *	Adds \a nr_objs 3d objects from the server to the current map
 *
 * @param nr_objs The number of objects to add
 * @param data The message from the server with the object list
 * @param len The length of \a data
 * @retval int  0 on error, 1 on success
 * @callgraph
 */
int get_3d_objects_from_server (int nr_objs, const Uint8 *data, int len);

/**
 * @ingroup maps
 * @brief Removes an object from the current map
 *
 *	Removes the object with ID \a id from the map
 *
 * @param id The ID of the object to be removed
 * @callgraph
 */
void remove_3d_object_from_server (int id);

#ifdef __cplusplus
} // extern "C"
#endif

#endif	// _MAP_H_