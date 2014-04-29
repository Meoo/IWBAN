/**
 * @file   DebugContext.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_DEBUGCONTEXT_HPP_
#define _IWBAN_DEBUGCONTEXT_HPP_

#ifdef NDEBUG
#   error "Cannot use DebugContext.hpp with NDEBUG"
#endif

#include <Global.hpp>

#include <graphics/Context.hpp>

#include <SFML/Graphics.hpp>

namespace gfx
{

class Renderer;

class DebugContext : public Context
{
public:
    friend class Renderer;


private:
    // Data members
    sf::RenderTexture   _render_scene;

    bool                _open;


public:
    DebugContext();

    void draw(const sf::Drawable & drawable,
              const sf::RenderStates & states = sf::RenderStates::Default);

    void fill(const sf::Color & color,
              const sf::RenderStates & states = sf::RenderStates::Default);

    virtual bool isOpen() const { return _open; }

    virtual void close();


protected:
    // Protected functions for Renderer
    void open();

    const sf::Texture & getTexture() const;

};
// class DebugContext

}
// namespace gfx

#endif // _IWBAN_DEBUGCONTEXT_HPP_
