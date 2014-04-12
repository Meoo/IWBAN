/**
 * @file   PackagerConfig.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _PKG_PACKAGERCONFIG_HPP_
#define _PKG_PACKAGERCONFIG_HPP_


// File extension
#define PKG_EXTENSION ".dp"

// Block size is 4kB
#define PKG_BLOCK_SIZE  0x00001000

// Maximum number of files in a package
#define PKG_MAX_FILES   512

// Package magic number 'IwPk'
#define PKG_MAGIC       0x4977506B


#endif // _PKG_PACKAGERCONFIG_HPP_
