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

                Selectable * slave = _table[_selected].slave;
                if (slave)
                    slave->dispatchAction(action);

                return;
            }
        }
        while (target && !target->isEnabled());

        if (!target)
        {
            // Cannot move in choosen direction, dispatch the action instead
            _selected->dispatchAction(action);

            Selectable * slave = _table[_selected].slave;
            if (slave)
                slave->dispatchAction(action);

            return;
        }

        select(target);
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

        if (bounds.isContaining(position))
        {
            if (_selected && _selected == it.first)
                return;

            select(it.first);
            return;
        }

        Selectable * slave = _table[it.first].slave;
        if (slave)
        {
            ut::Rectangle bounds = slave->getBounds();
            if (bounds.isContaining(position))
            {
                if (_selected && _selected == it.first)
                    return;

                select(it.first);
                return;
            }
        }
    }
}

void Navigation::dispatchMouseClick(const ut::Vector & position)
{
    if (_selected)
    {
        ut::Rectangle bounds = _selected->getBounds();
        if (bounds.isContaining(position))
        {
            _selected->dispatchMouseClick(ut::Vector(
                    position.x - bounds.top,
                    position.y - bounds.left
            ));
        }

        Selectable * slave = _table[_selected].slave;
        if (slave)
        {
            ut::Rectangle bounds = slave->getBounds();
            if (bounds.isContaining(position))
            {
                slave->dispatchMouseClick(ut::Vector(
                        position.x - bounds.top,
                        position.y - bounds.left
                ));
            }
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
}

void Navigation::addSlave(Selectable * master, Selectable * slave)
{
    IWBAN_PRE_PTR(master);
    IWBAN_PRE_PTR(slave);
    IWBAN_PRE(master != slave);

    NavigationEntry & master_entry = _table[master];
    master_entry.slave = slave;

    if (_selected == master)
        slave->select();
}

void Navigation::setHead(Selectable * head)
{
    IWBAN_PRE_PTR(head);

    _head = head;

    // Ensure that the entry exists
    _table[_head];

    if (!_selected && _head->isEnabled())
        select(_head);
}

void Navigation::reset()
{
    if (_selected == _head) return;

    deselect();

    _selected = _head;

    // Find the first aviable option
    while (_selected && !_selected->isEnabled())
        _selected = _table[_selected].up;

    if (_selected)
        select(_selected);
}

void Navigation::select(Selectable * elem)
{
    if (_selected)
        deselect();

    elem->select();

    Selectable * slave = _table[elem].slave;
    if (slave)
        slave->select();

    _selected = elem;
}

void Navigation::deselect()
{
    if (!_selected)
        return;

    _selected->deselect();

    Selectable * slave = _table[_selected].slave;
    if (slave)
        slave->deselect();

    _selected = nullptr;
}

}
// namespace gui
