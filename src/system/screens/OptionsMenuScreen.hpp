/**
 * @file   OptionsMenuScreen.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_OPTIONSMENUSCREEN_HPP_
#define _IWBAN_OPTIONSMENUSCREEN_HPP_

#include <Global.hpp>

#include <system/Controls.hpp>
#include <system/Screen.hpp>

#include <gui/Menu.hpp>
#include <gui/Choice.hpp>
#include <gui/Frame.hpp>

namespace sys
{

class OptionsMenuScreen : public Screen
{
protected:
    // Callbacks
    virtual void onUpdate()
    {
        for (unsigned i = 0; i < ACT_COUNT; ++i)
            if (getControls().getAction((ActionId) i).isJustActivated())
                menu.dispatchAction((ActionId) i);
    }

    virtual void onRender(gfx::Renderer & renderer) const
    {
        gfx::DrawContext & draw = renderer.openDrawContext();

        draw.fill(sf::Color::White);

        menu.draw(draw);

        draw.close();
    }

    virtual void onShow()
    {
    }

    virtual void onHide()
    {
    }

};
// class IntroScreen

}
// namespace sys

#endif // _IWBAN_OPTIONSMENUSCREEN_HPP_
