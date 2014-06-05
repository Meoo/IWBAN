/**
 * @file   PackageConfig.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_PACKAGECONFIG_HPP_
#define _IWBAN_PACKAGECONFIG_HPP_


// File extension
#define IWBAN_PKG_EXTENSION   ".dp"

// Block size is 4kB
#define IWBAN_PKG_BLOCK_SIZE  0x00001000

// Maximum number of files in a package
#define IWBAN_PKG_MAX_FILES   512

// Package magic number 'IwPk' in little endian
#define IWBAN_PKG_MAGIC       0x6B507749


#endif // _IWBAN_PACKAGECONFIG_HPP_
