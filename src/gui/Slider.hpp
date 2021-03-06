/**
 * @file   Slider.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_SLIDER_HPP_
#define _IWBAN_SLIDER_HPP_

#include <Global.hpp>

#include <gui/Selectable.hpp>

#include <functional>

namespace gui
{

class Slider : public Selectable
{
public:
    typedef std::function<void (unsigned)> ValueCallback;


private:
    // Data members
    ValueCallback       _action;

    sf::RectangleShape  _border;
    sf::RectangleShape  _center;

    bool                _shadow = true;

    ut::Vector          _position;
    ut::Vector          _size;

    int                 _value     = 50;
    int                 _value_min = 0;
    int                 _value_max = 100;
    int                 _step      = 10;


public:
    // Constructor
    Slider();

    Slider(const ut::Vector & size,
           int min, int max, int step, int value);

    // Functions
    virtual void draw(gfx::GuiContext & context) const;

    virtual void select();
    virtual void deselect();

    virtual ut::Rectangle getBounds() const;

    void        setCallback(const ValueCallback & func) { _action = func; }
    virtual void dispatchAction(sys::ActionId action);
    virtual void dispatchMouseClick(const ut::Vector & local_position);

    void        setShadow(bool shadow) { _shadow = shadow; }

    void        setSize(const ut::Vector & size);
    virtual void setPosition(const ut::Vector & position);

    int         getValue() const { return _value; }
    void        setValue(int value) { _value = value; updateValue(); }

    void        setRange(int min, int max, int step) { _value_min = min; _value_max = max; _step = step; updateValue(); }

    virtual void    refresh();

private:
    // Private functions
    void updateValue();

};
// class Slider

}
// namespace gui

#endif // _IWBAN_SLIDER_HPP_
