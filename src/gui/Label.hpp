/**
 * @file   Label.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_LABEL_HPP_
#define _IWBAN_LABEL_HPP_

#include <Global.hpp>

#include <gui/Element.hpp>

namespace gui
{

// A text in a menu
class Label : public Element
{
private:
    // Data members
    sf::Text            _text;

    bool                _shadow = false;

    ut::Vector          _position;
    ut::Vector          _margin;


public:
    // Constructor
    Label();

    explicit Label(const std::string & string_key);

    // Functions
    virtual void draw(gfx::DrawContext & context) const;

    void setShadow(bool shadow) { _shadow = shadow; }

    virtual ut::Vector getSize() const;

    virtual void setPosition(const ut::Vector & position) { _position = position; }

    void setText(const std::string & string_key);

    void setTextColor(const sf::Color & color) { _text.setColor(color); }

    void setCharacterSize(unsigned size) { _text.setCharacterSize(size); }

};
// class Label

}
// namespace gui

#endif // _IWBAN_CHOICE_HPP_
