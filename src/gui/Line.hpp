/**
 * @file   Line.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_LINE_HPP_
#define _IWBAN_LINE_HPP_

#include <Global.hpp>

#include <gui/Element.hpp>

#include <vector>

namespace gui
{

// A line of elements aligned horizontally
class Line : public Element
{
private:
    // Data members
    ut::Vector              _position;

    std::vector<Element *>  _content;


public:
    // Constructors
    explicit Line(const std::vector<Element *> & content);

    // Virtual destructor
    virtual ~Line();

    // Functions
    virtual void draw(gfx::DrawContext & context) const;

    virtual ut::Vector getSize() const;

    virtual void setPosition(const ut::Vector & position);

    virtual void select();
    virtual void deselect();

    virtual bool isSelectable() const;

    virtual void dispatchAction(sys::ActionId action);

    void updateContent();

};
// class Frame

}
// namespace gui

#endif // _IWBAN_LINE_HPP_
