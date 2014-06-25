/**
 * @file   Slider.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_SLIDER_HPP_
#define _IWBAN_SLIDER_HPP_

#include <Global.hpp>

#include <gui/Element.hpp>

#include <functional>

namespace gui
{

// A text in a menu
class Slider : public Element
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

    int                 _value;
    int                 _value_min;
    int                 _value_max;
    int                 _step;


public:
    // Constructor
    explicit Slider(const ut::Vector & size,
                    int min, int max, int step, int value);

    // Functions
    virtual void draw(gfx::GuiContext & context) const;

    virtual void select();
    virtual void deselect();

    virtual bool isSelectable() const { return true; }

    void setAction(const ValueCallback & func) { _action = func; }
    virtual void dispatchAction(sys::ActionId action);

    // Accessors
    void setShadow(bool shadow) { _shadow = shadow; }

    void setSize(const ut::Vector & size) { _size = size; }
    virtual ut::Vector getSize() const { return _size; }

    virtual void setPosition(const ut::Vector & position);

    int getValue() const { return _value; }
    void setValue(int value) { _value = value; updateValue(); }


private:
    // Private functions
    void updateValue();

};
// class Slider

}
// namespace gui

#endif // _IWBAN_SLIDER_HPP_
