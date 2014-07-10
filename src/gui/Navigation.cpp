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
        Selectable * target = _selected;

        // Loop until it find a suitable entry or reach an end
        do
        {
            switch (action)
            {
            case sys::ACT_UP:
                target = _table[target].up;
                break;

            case sys::ACT_DOWN:
                target = _table[target].down;
                break;

            case sys::ACT_LEFT:
                target = _table[target].left;
                break;

            case sys::ACT_RIGHT:
                target = _table[target].right;
                break;

            default:
                // The action is not navigation, dispatch it and quit
                _selected->dispatchAction(action);
                return;
            }
        }
        while (target && !target->isEnabled());

        if (!target)
        {
            // Cannot move in choosen direction, dispatch the action instead
            _selected->dispatchAction(action);
            return;
        }

        _selected->deselect();
        _selected = target;
        _selected->select();
    }
    else
        reset();
}

void Navigation::dispatchMouseMove(const ut::Vector & position)
{
    for (auto it : _table)
    {
        if (!it.first->isEnabled())
            continue;

        ut::Rectangle bounds = it.first->getBounds();

        if (position.x >= bounds.x
         && position.y >= bounds.y
         && position.x <= bounds.x + bounds.w
         && position.y <= bounds.y + bounds.h)
        {
            if (_selected)
            {
                if (_selected == it.first)
                    return;

                _selected->deselect();
                _selected = it.first;
                _selected->select();
            }
            else
            {
                _selected = it.first;
                _selected->select();
            }
            return;
        }
    }
}

void Navigation::dispatchMouseClick(const ut::Vector & position)
{
    if (_selected)
    {
        ut::Rectangle bounds = _selected->getBounds();
        if (position.x >= bounds.x
         && position.y >= bounds.y
         && position.x <= bounds.x + bounds.w
         && position.y <= bounds.y + bounds.h)
        {
            _selected->dispatchAction(sys::ACT_ACCEPT);
        }
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
    if (_selected == _head) return;

    if (_selected)
        _selected->deselect();

    // TODO head can de deactivated?, error if failed?
    if (_head)
    {
        _selected = _head;
        _head->select();
    }
}

}
// namespace gui
