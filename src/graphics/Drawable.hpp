/**
 * @file   Drawable.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_DRAWABLE_HPP_
#define _IWBAN_DRAWABLE_HPP_

#include <Global.hpp>

#include <boost/intrusive/list.hpp>

namespace gfx
{

class Drawable : public boost::intrusive::list_base_hook<>
{
public:
    class Comparator;
    typedef boost::intrusive::list<Drawable> List;


private:
    // Data members
    int     _render_priority;


public:
    // Virtual destructor
    virtual ~Drawable()     {}

    // Virtual functions
    virtual void draw(gfx::DrawContext & context) = 0;

    // Getters / setters
    int     getPriority() const         { return _render_priority; }
    void    setPriority(int priority)   { _render_priority = priority; }


    class Comparator
    {
    public:
        bool operator () (const Drawable & a, Drawable & b) const
            { return a.getPriority() < b.getPriority(); }
    };
    // class Comparator

};
// class Drawable

}
// namespace gfx

#endif // _IWBAN_DRAWABLE_HPP_
