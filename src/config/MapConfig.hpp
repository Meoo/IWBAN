/**
 * @file   MapConfig.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_MAPCONFIG_HPP_
#define _IWBAN_MAPCONFIG_HPP_


// File extension
#define IWBAN_MAP_EXTENSION     ".map"

// Map magic number 'IwMp' in little endian
#define IWBAN_MAP_MAGIC         0x704D7749

#define IWBAN_MAP_VERSION       1

#define IWBAN_MAP_MAX_WIDTH     0xFFFFu
#define IWBAN_MAP_MAX_HEIGHT    0xFFFFu

#define IWBAN_MAP_MAX_TEXTURES  0xFFu
#define IWBAN_MAP_MAX_LAYERS    0xFFu

#define IWBAN_MAP_MAX_CHUNKS_PER_LAYER      0xFFu
#define IWBAN_MAP_MAX_VERTICES_PER_LAYER    0xFFFFu


#endif // _IWBAN_MAPCONFIG_HPP_
