/**
 * @file   Selector.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_SELECTOR_HPP_
#define _IWBAN_SELECTOR_HPP_

#include <Global.hpp>

#include <gui/Label.hpp>

#include <functional>
#include <vector>

namespace gui
{

class Selector : public Label
{
public:
    typedef std::function<void (int)> ValueCallback;

    class Entry
    {
        int value;
        std::string string_key;
    };

    typedef std::vector<Entry> EntryList;


private:
    // Data members
    EntryList       _entries;

    ValueCallback   _action;

    sf::Color       _idle_color     = sf::Color(152, 152, 152);
    sf::Color       _selected_color = sf::Color::White;


public:
    // Constructors
    Selector() { setTextColor(_idle_color); }

    explicit Selector(const std::string & string_key)
        : Label(string_key) { setTextColor(_idle_color); }

    // Functions
    void addEntry(int value, const std::string & string_key);

    virtual void select();
    virtual void deselect();

    virtual bool isSelectable() const { return true; }

    void setAction(const ValueCallback & func) { _action = func; }

    virtual void dispatchAction(sys::ActionId action);

    void setIdleColor(const sf::Color & color);
    void setSelectedColor(const sf::Color & color);

    void updateColor();

};
// class Selector

}
// namespace gui

#endif // _IWBAN_SELECTOR_HPP_
