/**
 * @file   Frame.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_FRAME_HPP_
#define _IWBAN_FRAME_HPP_

#include <Global.hpp>

#include <gui/Element.hpp>

namespace gui
{

// A Frame containing another Element
// The contained Element is centered in the Frame
class Frame : public Element
{
private:
    // Data members
    ut::Vector          _position;
    ut::Vector          _size;

    Element *           _content;


public:
    // Constructor
    Frame(const ut::Vector & size, Element * content);

    // Virtual destructor
    virtual ~Frame();

    // Functions
    virtual void draw(gfx::DrawContext & context) const;

    virtual ut::Vector getSize() const      { return _size; }
    void setSize(const ut::Vector & size);

    virtual void setPosition(const ut::Vector & position);

    virtual void select()   { Element::select(); _content->select(); }
    virtual void deselect() { Element::deselect(); _content->deselect(); }

    virtual bool isSelectable() const { return _content->isSelectable(); }

    virtual void dispatchAction(sys::ActionId action);


private:
    // Private function
    void updateContent();

};
// class Frame

}
// namespace gui

#endif // _IWBAN_FRAME_HPP_
