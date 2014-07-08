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

class Label : public Element
{
private:
    // Data members
    std::string     _key;
    sf::Text        _text;

    bool            _shadow = true;

    ut::Vector      _position;


public:
    // Constructors
                Label();

    explicit    Label(const std::string & string_key);

    // Virtual destructor
    virtual     ~Label() {}

    // Functions
    virtual void draw(gfx::GuiContext & context) const;

    virtual void setPosition(const ut::Vector & position) { _position = position; }

    void        setShadow(bool shadow) { _shadow = shadow; }

    void        loadText(const std::string & string_key);
    void        setTextColor(const sf::Color & color) { _text.setColor(color); }
    void        setCharacterSize(unsigned size) { _text.setCharacterSize(size); }

    virtual void refresh();

};
// class Label

}
// namespace gui

#endif // _IWBAN_LABEL_HPP_
