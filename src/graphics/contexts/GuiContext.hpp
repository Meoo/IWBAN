/**
 * @file   GuiContext.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_GUICONTEXT_HPP_
#define _IWBAN_GUICONTEXT_HPP_

#include <Global.hpp>

#include <graphics/Context.hpp>

#include <SFML/Graphics.hpp>

namespace gfx
{

class Renderer;

class GuiContext : public Context
{
private:
    // Data members
    sf::RenderTarget &  _render_target;

    bool                _open   = false;


public:
    GuiContext(sf::RenderTarget & target);

    void draw(const sf::Drawable & drawable,
              const sf::RenderStates & states = sf::RenderStates::Default);

    bool isOpen() const override { return _open; }
    void close() override;


private:
    friend class Renderer;

    void open();

};
// class GuiContext

}
// namespace gfx

#endif // _IWBAN_GUICONTEXT_HPP_
