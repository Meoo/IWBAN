/**
 * @file   Drawable.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_DRAWABLE_HPP_
#define _IWBAN_DRAWABLE_HPP_

#include <Global.hpp>

#include <boost/intrusive/list.hpp>

#include <SFML/Graphics.hpp>

namespace gfx
{

class Drawable : public sf::Drawable,
                 public boost::intrusive::list_base_hook<>
{
public:
    class Comparator;
    typedef boost::intrusive::list<Drawable> List;


private:
    // Data members
    int     _depth;


public:
    // Virtual destructor
    virtual ~Drawable()     {}

    // Getters / setters
    int     getDepth() const    { return _depth; }
    void    setDepth(int depth) { _depth = depth; }


    class Comparator
    {
    public:
        bool operator () (const Drawable & a, const Drawable & b) const
            { return a.getDepth() < b.getDepth(); }
    };
    // class Comparator

};
// class Drawable

}
// namespace gfx

#endif // _IWBAN_DRAWABLE_HPP_
