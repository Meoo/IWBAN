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
    sf::Text _text;

    ut::Vector _margin;


public:
    // Constructor
    explicit Label(const std::string & string_key);

    // Functions
    virtual void draw(gfx::DrawContext & context, const ut::Vector & origin);

    virtual ut::Vector getSize() const;

    void setTextColor(const sf::Color & color) { _text.setColor(color); }

};
// class Choice

}
// namespace gui

#endif // _IWBAN_CHOICE_HPP_
