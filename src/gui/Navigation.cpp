/**
 * @file   Navigation.cpp
 * @author Bastien Brunnenstein
 */

#include <Global.hpp>

#include <gui/Navigation.hpp>

namespace gui
{

void Navigation::dispatchAction(sys::ActionId action)
{
    if (_selected)
    {
        NavigationEntry & entry = _table[_selected];

        if (action == sys::ACT_UP && entry.up)
        {
            _selected->deselect();
            _selected = entry.up;
            _selected->select();
        }
        else if (action == sys::ACT_DOWN && entry.down)
        {
            _selected->deselect();
            _selected = entry.down;
            _selected->select();
        }
        else if (action == sys::ACT_LEFT && entry.left)
        {
            _selected->deselect();
            _selected = entry.left;
            _selected->select();
        }
        else if (action == sys::ACT_RIGHT && entry.right)
        {
            _selected->deselect();
            _selected = entry.right;
            _selected->select();
        }
        else
            _selected->dispatchAction(action);
    }
}

void Navigation::addVertical(Selectable * up, Selectable * down)
{
    IWBAN_PRE_PTR(up);
    IWBAN_PRE_PTR(down);
    IWBAN_PRE(up != down);

    NavigationEntry & up_entry = _table[up];
    NavigationEntry & down_entry = _table[down];

    up_entry.down = down;
    down_entry.up = up;

    if (!_selected)
    {
        if (up->isEnabled())
        {
            _selected = up;
            up->select();
        }
        else if (down->isEnabled())
        {
            _selected = down;
            down->select();
        }
    }
}

void Navigation::addHorizontal(Selectable * left, Selectable * right)
{
    IWBAN_PRE_PTR(left);
    IWBAN_PRE_PTR(right);
    IWBAN_PRE(left != right);

    NavigationEntry & left_entry = _table[left];
    NavigationEntry & right_entry = _table[right];

    left_entry.right = right;
    right_entry.left = left;

    if (!_selected)
    {
        if (left->isEnabled())
        {
            _selected = left;
            left->select();
        }
        else if (right->isEnabled())
        {
            _selected = right;
            right->select();
        }
    }
}

void Navigation::setHead(Selectable * head)
{
    IWBAN_PRE_PTR(head);

    _head = head;

    // Ensure that the entry exists
    _table[_head];

    if (!_selected && _head->isEnabled())
    {
        _selected = _head;
        _head->select();
    }
}

void Navigation::reset()
{
    if (_selected)
        _selected->deselect();

    // TODO head can de deactivated, error if failed?
    if (_head)
        _head->select();
}

}
// namespace gui