/**
 * @file   InitialScreen.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_INITIALSCREEN_HPP_
#define _IWBAN_INITIALSCREEN_HPP_

#include <Global.hpp>

#include <system/Screen.hpp>

namespace game
{

sys::Screen * createInitialScreen(int argc, char ** argv);

}
// namespace game

#endif // _IWBAN_INITIALSCREEN_HPP_
