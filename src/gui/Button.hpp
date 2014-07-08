/**
 * @file   Button.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_BUTTON_HPP_
#define _IWBAN_BUTTON_HPP_

#include <Global.hpp>

#include <gui/Selectable.hpp>

#include <functional>

namespace gui
{

class Button : public Selectable
{
public:
    typedef std::function<void ()> ActionCallback;


private:
    // Data members
    std::string     _key;
    sf::Text        _text;

    bool            _shadow = true;

    ut::Vector      _position;

    bool            _enabled = true;
    bool            _selected = false;
    ActionCallback  _action;

    sf::Color       _idle_color     = sf::Color(152, 152, 152);
    sf::Color       _selected_color = sf::Color::White;
    sf::Color       _disabled_color = sf::Color(96, 96, 96);


public:
    // Constructors
                Button();

    explicit    Button(const std::string & string_key);

    // Virtual destructor
    virtual     ~Button() {}

    // Functions
    virtual void    draw(gfx::GuiContext & context) const;

    virtual void    setPosition(const ut::Vector & position) { _position = position; }

    void        enable();
    void        disable();

    virtual bool    isEnabled() const { return _enabled; }

    virtual void    select();
    virtual void    deselect();

    virtual ut::Rectangle getBounds() const;

    virtual void    dispatchAction(sys::ActionId action);

    void        setCallback(const ActionCallback & func) { _action = func; }

    void        setShadow(bool shadow) { _shadow = shadow; }

    void        loadText(const std::string & string_key);
    void        setTextColor(const sf::Color & color) { _text.setColor(color); }
    void        setCharacterSize(unsigned size) { _text.setCharacterSize(size); }

    void        setIdleColor(const sf::Color & color);
    void        setSelectedColor(const sf::Color & color);
    void        setDisabledColor(const sf::Color & color);

    virtual void    refresh();

};
// class Button

}
// namespace gui

#endif // _IWBAN_BUTTON_HPP_
