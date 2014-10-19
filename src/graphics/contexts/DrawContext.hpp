/**
 * @file   DrawContext.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_DRAWCONTEXT_HPP_
#define _IWBAN_DRAWCONTEXT_HPP_

#include <Global.hpp>

#include <graphics/Context.hpp>

#include <SFML/Graphics.hpp>

namespace gfx
{

class Drawable;
class Renderer;

class DrawContext : public Context
{
private:
    // Data members
    sf::RenderTexture   _render_scene;

    bool                _open   = false;


public:
    DrawContext();

    void draw(const Drawable & drawable);

    // TODO Remove sf::Drawable functions
    void draw(const sf::Drawable & drawable,
              const sf::RenderStates & states = sf::RenderStates::Default);

    void fill(const sf::Color & color,
              const sf::RenderStates & states = sf::RenderStates::Default);

    bool isOpen() const override { return _open; }
    void close() override;


private:
    friend class Renderer;

    void open(const sf::View & view);
    const sf::Texture & getTexture() const;

};
// class DrawContext

}
// namespace gfx

#endif // _IWBAN_DRAWCONTEXT_HPP_
