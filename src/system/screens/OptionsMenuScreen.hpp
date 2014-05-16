/**
 * @file   OptionsMenuScreen.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_OPTIONSMENUSCREEN_HPP_
#define _IWBAN_OPTIONSMENUSCREEN_HPP_

#include <Global.hpp>

#include <system/Clock.hpp>
#include <system/Controls.hpp>
#include <system/Screen.hpp>

#include <gui/Choice.hpp>
#include <gui/Frame.hpp>
#include <gui/Line.hpp>
#include <gui/Menu.hpp>
#include <gui/Separator.hpp>
#include <gui/Slider.hpp>

namespace sys
{

class OptionsMenuScreen : public Screen
{
private:
    // Data members
    gui::Menu * _menu;
    gui::Menu * _keyboard_menu;
    gui::Menu * _gamepad_menu;

    sf::Time     _gamepad_state_timer;
    gui::Label * _gamepad_state;

    enum State
    {
        ST_OPTIONS,
        ST_KEYBOARD,
        ST_GAMEPAD

    } _state = ST_OPTIONS;


public:
    // Constructor
    OptionsMenuScreen()
    {
        const ut::Vector choice_box(300, 40);


        // Options menu

        _menu = new gui::Menu();
        _menu->setPosition(ut::Vector(0, 20));
        _menu->setSize(ut::Vector(640, 400));
        _menu->setCentered(true);

        {
            gui::Label * title = new gui::Label("menu.options");
            title->setCharacterSize(40);
            _menu->add(new gui::Frame(ut::Vector(400, 70), title));
            _menu->add(new gui::Separator(ut::Vector(400, 10)));
        }

        _menu->add(new gui::Line({
            new gui::Frame(ut::Vector(200, 40), new gui::Choice("options.language")),
            new gui::Frame(ut::Vector(400, 40), new gui::Slider(ut::Vector(250, 22), 10))
        }));

        _menu->add(new gui::Line({
            new gui::Frame(ut::Vector(200, 40), new gui::Choice("options.audio")),
            new gui::Frame(ut::Vector(400, 40), new gui::Slider(ut::Vector(250, 22), 15))
        }));

        _menu->add(new gui::Line({
            new gui::Frame(ut::Vector(200, 40), new gui::Choice("options.video")),
            new gui::Separator(ut::Vector(400, 40))
        }));

        gui::Choice * keyboard = new gui::Choice("options.keyboard");
        keyboard->setAction([this](){ setState(ST_KEYBOARD); });
        _menu->add(new gui::Line({
            new gui::Frame(ut::Vector(200, 40), keyboard),
            new gui::Separator(ut::Vector(400, 40))
        }));

        gui::Choice * gamepad = new gui::Choice("options.gamepad");
        gamepad->setAction([this](){ setState(ST_GAMEPAD); });
        _gamepad_state = new gui::Label();
        _gamepad_state->setCharacterSize(24);
        _menu->add(new gui::Line({
            new gui::Frame(ut::Vector(200, 40), gamepad),
            new gui::Frame(ut::Vector(400, 40), _gamepad_state)
        }));

        _menu->add(new gui::Separator(ut::Vector(400, 40)));
        _menu->add(new gui::Frame(ut::Vector(400, 40), new gui::Choice("options.save")));
        _menu->add(new gui::Frame(ut::Vector(400, 40), new gui::Choice("options.cancel")));


        // Keyboard menu

        _keyboard_menu = new gui::Menu();
        _keyboard_menu->setPosition(ut::Vector(0, 20));
        _keyboard_menu->setSize(ut::Vector(640, 400));
        _keyboard_menu->setCentered(true);

        {
            gui::Label * title = new gui::Label("options.keyboard");
            title->setCharacterSize(40);
            _keyboard_menu->add(new gui::Frame(ut::Vector(400, 70), title));
        }

        _keyboard_menu->add(new gui::Line({
            new gui::Frame(ut::Vector(200, 40), new gui::Choice("actions.up")),
            new gui::Separator(ut::Vector(100, 40)),
            new gui::Separator(ut::Vector(150, 40)),
            new gui::Separator(ut::Vector(150, 40))
        }));

        _keyboard_menu->add(new gui::Line({
            new gui::Frame(ut::Vector(200, 40), new gui::Choice("actions.left")),
            new gui::Separator(ut::Vector(100, 40)),
            new gui::Separator(ut::Vector(150, 40)),
            new gui::Separator(ut::Vector(150, 40))
        }));

        _keyboard_menu->add(new gui::Line({
            new gui::Frame(ut::Vector(200, 40), new gui::Choice("actions.down")),
            new gui::Separator(ut::Vector(100, 40)),
            new gui::Separator(ut::Vector(150, 40)),
            new gui::Separator(ut::Vector(150, 40))
        }));

        _keyboard_menu->add(new gui::Line({
            new gui::Frame(ut::Vector(200, 40), new gui::Choice("actions.right")),
            new gui::Separator(ut::Vector(100, 40)),
            new gui::Separator(ut::Vector(150, 40)),
            new gui::Separator(ut::Vector(150, 40))
        }));

        _keyboard_menu->add(new gui::Line({
            new gui::Frame(ut::Vector(200, 40), new gui::Choice("actions.jump")),
            new gui::Separator(ut::Vector(100, 40)),
            new gui::Separator(ut::Vector(150, 40)),
            new gui::Separator(ut::Vector(150, 40))
        }));

        _keyboard_menu->add(new gui::Line({
            new gui::Frame(ut::Vector(200, 40), new gui::Choice("actions.fire")),
            new gui::Separator(ut::Vector(100, 40)),
            new gui::Separator(ut::Vector(150, 40)),
            new gui::Separator(ut::Vector(150, 40))
        }));

        _keyboard_menu->add(new gui::Line({
            new gui::Frame(ut::Vector(200, 40), new gui::Choice("actions.retry")),
            new gui::Separator(ut::Vector(100, 40)),
            new gui::Separator(ut::Vector(150, 40)),
            new gui::Separator(ut::Vector(150, 40))
        }));

        _keyboard_menu->add(new gui::Separator(ut::Vector(400, 10)));
        _keyboard_menu->add(new gui::Frame(ut::Vector(400, 40), new gui::Choice("options.save")));

        gui::Choice * kcancel = new gui::Choice("options.cancel");
        kcancel->setAction([this](){ setState(ST_OPTIONS); });
        _keyboard_menu->add(new gui::Frame(ut::Vector(400, 40), kcancel));


        // Gamepad menu

        _gamepad_menu = new gui::Menu();
        _gamepad_menu->setPosition(ut::Vector(0, 20));
        _gamepad_menu->setSize(ut::Vector(640, 400));
        _gamepad_menu->setCentered(true);

        {
            gui::Label * title = new gui::Label("options.gamepad");
            title->setCharacterSize(40);
            _gamepad_menu->add(new gui::Frame(ut::Vector(400, 70), title));
        }

        _gamepad_menu->add(new gui::Line({
            new gui::Frame(ut::Vector(200, 40), new gui::Choice("actions.jump")),
            new gui::Separator(ut::Vector(250, 40)),
            new gui::Separator(ut::Vector(150, 40))
        }));

        _gamepad_menu->add(new gui::Line({
            new gui::Frame(ut::Vector(200, 40), new gui::Choice("actions.fire")),
            new gui::Separator(ut::Vector(250, 40)),
            new gui::Separator(ut::Vector(150, 40))
        }));

        _gamepad_menu->add(new gui::Line({
            new gui::Frame(ut::Vector(200, 40), new gui::Choice("actions.retry")),
            new gui::Separator(ut::Vector(250, 40)),
            new gui::Separator(ut::Vector(150, 40))
        }));

        _gamepad_menu->add(new gui::Separator(ut::Vector(400, 10)));
        _gamepad_menu->add(new gui::Frame(ut::Vector(400, 40), new gui::Choice("options.save")));

        gui::Choice * gcancel = new gui::Choice("options.cancel");
        gcancel->setAction([this](){ setState(ST_OPTIONS); });
        _gamepad_menu->add(new gui::Frame(ut::Vector(400, 40), gcancel));

    }

    // Destructor
    virtual ~OptionsMenuScreen()
    {
        // Choices are released by _menu
        delete _menu;
        delete _keyboard_menu;
        delete _gamepad_menu;
    }


protected:
    // Callbacks
    virtual void onUpdate()
    {
        if (_state == ST_OPTIONS && _gamepad_state_timer < getUpdateTime())
        {
            if (sf::Joystick::isConnected(0))
            {
                _gamepad_state->setText("gamepad.connected");
                _gamepad_state->setTextColor(sf::Color(64, 128, 64));
            }
            else
            {
                _gamepad_state->setText("gamepad.disconnected");
                _gamepad_state->setTextColor(sf::Color(128, 64, 64));
            }

            _gamepad_state_timer = getUpdateTime() + sf::seconds(1);
            _menu->updateChilds();
        }

        if (getControls().getAction(ACT_MENU).isJustActivated())
        {
            switch (_state)
            {
            case ST_OPTIONS:
                // TODO Go back to main menu
                break;

            case ST_KEYBOARD:
                setState(ST_OPTIONS);
                break;

            case ST_GAMEPAD:
                setState(ST_OPTIONS);
                break;
            }
            return;
        }

        for (unsigned i = 0; i < ACT_COUNT; ++i)
            if (getControls().getAction((ActionId) i).isJustActivated())
            {
                switch (_state)
                {
                case ST_OPTIONS:
                    _menu->dispatchAction((ActionId) i);
                    break;

                case ST_KEYBOARD:
                    _keyboard_menu->dispatchAction((ActionId) i);
                    break;

                case ST_GAMEPAD:
                    _gamepad_menu->dispatchAction((ActionId) i);
                    break;
                }
            }
    }

    virtual void onRender(gfx::Renderer & renderer) const
    {
        gfx::DrawContext & draw = renderer.openDrawContext();

        draw.fill(sf::Color(16, 16, 16));

        switch (_state)
        {
        case ST_OPTIONS:
            _menu->draw(draw);
            break;

        case ST_KEYBOARD:
            _keyboard_menu->draw(draw);
            break;

        case ST_GAMEPAD:
            _gamepad_menu->draw(draw);
            break;
        }

        draw.close();
    }

    virtual void onShow()
    {
    }

    virtual void onHide()
    {
    }

    void setState(State state)
    {
        _state = state;
    }

};
// class OptionsMenuScreen

}
// namespace sys

#endif // _IWBAN_OPTIONSMENUSCREEN_HPP_
