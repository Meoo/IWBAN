/**
 * @file   Display.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_DISPLAY_HPP_
#define _IWBAN_DISPLAY_HPP_

#include <Global.hpp>

#include <system/ScreenProjector.hpp>

namespace sys
{

void display_open();

void display_close();

void display_run(sys::ScreenProjector & projector);

}
// namespace sys

#endif // _IWBAN_DISPLAY_HPP_
