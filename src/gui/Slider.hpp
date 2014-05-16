/**
 * @file   Slider.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_SLIDER_HPP_
#define _IWBAN_SLIDER_HPP_

#include <Global.hpp>

#include <gui/Element.hpp>

namespace gui
{

// A text in a menu
class Slider : public Element
{
private:
    // Data members
    sf::RectangleShape  _border;
    sf::RectangleShape  _center;

    bool                _shadow = true;

    ut::Vector          _position;
    ut::Vector          _size;

    unsigned            _value;
    unsigned            _value_max = 20;


public:
    // Constructor
    explicit Slider(const ut::Vector & size, unsigned value);

    // Functions
    virtual void draw(gfx::DrawContext & context) const;

    virtual void select();
    virtual void deselect();

    virtual bool isSelectable() const { return true; }

    virtual void dispatchAction(sys::ActionId action);

    // Accessors
    void setShadow(bool shadow) { _shadow = shadow; }

    void setSize(const ut::Vector & size) { _size = size; }
    virtual ut::Vector getSize() const { return _size; }

    virtual void setPosition(const ut::Vector & position);

    unsigned getValue() const { return _value; }


private:
    // Private functions
    void updateValue();

};
// class Slider

}
// namespace gui

#endif // _IWBAN_SLIDER_HPP_
