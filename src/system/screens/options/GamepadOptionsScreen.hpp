/**
 * @file   GamepadOptionsScreen.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_GAMEPADOPTIONSSCREEN_HPP_
#define _IWBAN_GAMEPADOPTIONSSCREEN_HPP_

#include <Global.hpp>

#include <config/GamepadConfig.hpp>

#include <data/Locale.hpp>

#include <system/Controls.hpp>
#include <system/Screen.hpp>
#include <system/Time.hpp>
#include <system/inputs/Gamepad.hpp>

#include <gui/Button.hpp>
#include <gui/Label.hpp>
#include <gui/Navigation.hpp>
#include <gui/Slider.hpp>

namespace sys
{

class GamepadOptionsScreen : public Screen
{
private:
    // Data members
    gui::Navigation _navi;

    gui::Label  _title;

    gui::Button _quit;

    gui::Slider _sensibility;

    Screen * _previous_menu;


public:
    // Constructor
    GamepadOptionsScreen(Screen * previous_menu)
        : _previous_menu(previous_menu)
    {
        data::getLocale().loadFile("system/gamepad.txt");

        // Title
        _title.loadText("options.gamepad");
        _title.setCharacterSize(50);
        _title.setPosition({320, 50});

        // Quit
        _quit.loadText("options.return");
        _quit.setCallback([this](){ onQuit(); });
        _quit.setPosition({320, 440});

        // Navigation
        _navi.setHead(&_quit);

/*
        _thresh = new gui::Slider(ut::Vector(250, 22), 5, 95, 9,
                                    100 - getGamepad().getAxisThreshold());
        _thresh->setAction([this](unsigned value){ updateThresh(value); });
        _gamepad_menu->add(new gui::Line({
            new gui::Frame(ut::Vector(200, 40), new gui::Choice("gamepad.sensibility")),
            new gui::Frame(ut::Vector(400, 40), _thresh)
        }));

        _btn_jump = new gui::ButtonLabel(getGamepad().getButtonFromAction(ACT_JUMP));
        _btn_jump->setAction([this](){ hookGamepad(_btn_jump, ACT_JUMP); });
        _gamepad_menu->add(new gui::Line({
            new gui::Frame(ut::Vector(200, 40), new gui::Choice("actions.jump")),
            new gui::Frame(ut::Vector(400, 40), _btn_jump)
        }));

        _btn_fire = new gui::ButtonLabel(getGamepad().getButtonFromAction(ACT_FIRE));
        _btn_fire->setAction([this](){ hookGamepad(_btn_fire, ACT_FIRE); });
        _gamepad_menu->add(new gui::Line({
            new gui::Frame(ut::Vector(200, 40), new gui::Choice("actions.fire")),
            new gui::Frame(ut::Vector(400, 40), _btn_fire)
        }));

        _btn_retry = new gui::ButtonLabel(getGamepad().getButtonFromAction(ACT_RETRY));
        _btn_retry->setAction([this](){ hookGamepad(_btn_retry, ACT_RETRY); });
        _gamepad_menu->add(new gui::Line({
            new gui::Frame(ut::Vector(200, 40), new gui::Choice("actions.retry")),
            new gui::Frame(ut::Vector(400, 40), _btn_retry)
        }));

        {
            gui::Choice * reset = new gui::Choice("options.defaults");
            reset->setAction([this](){ getGamepad().reloadDefaults(); updateGamepadButtons(); });
            _gamepad_menu->add(new gui::Separator(ut::Vector(400, 10)));
            _gamepad_menu->add(new gui::Frame(ut::Vector(400, 40), reset));
        }


        gui::Choice * gcancel = new gui::Choice("options.return");
        gcancel->setAction([this](){ setState(ST_OPTIONS); });
        _gamepad_menu->add(new gui::Frame(ut::Vector(400, 40), gcancel));
*/
    }

    // Destructor
    virtual ~GamepadOptionsScreen() {}


protected:
    // Callbacks
    virtual void onUpdate(const sf::Time & update_time)
    {
        // If menu is pressed, return to previous menu
        if (getControls().getAction(ACT_MENU).isJustActivated())
        {
            onQuit();
            return;
        }

        for (unsigned i = 0; i < ACT_COUNT; ++i)
            if (getControls().getAction((ActionId) i).isJustActivated())
                _navi.dispatchAction((ActionId) i);

/*        if (_state == ST_OPTIONS && _gamepad_state_timer < update_time)
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

            _gamepad_state_timer = update_time + sf::seconds(2);
            _menu->updateChilds();
        }

        if (!_waiting_input)
        {
            // If menu is pressed, return to previous menu
            if (getControls().getAction(ACT_MENU).isJustActivated())
            {
                switch (_state)
                {
                case ST_OPTIONS:
                    // TODO Go back to main menu
                    break;

                case ST_VIDEO:
                    setState(ST_OPTIONS);
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

            // Send actions to active menu
            for (unsigned i = 0; i < ACT_COUNT; ++i)
            {
                if (getControls().getAction((ActionId) i).isJustActivated())
                {
                    switch (_state)
                    {
                    case ST_OPTIONS:
                        _menu->dispatchAction((ActionId) i);
                        break;

                    case ST_VIDEO:
                        _video_menu->dispatchAction((ActionId) i);
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
        } // if (!_waiting_input)
        else
        {
            // If we catch an action while a hook is active, it means
            // another device sent it.
            if (getControls().getAction(ACT_MENU).isJustActivated())
            {
                if (_state == ST_GAMEPAD)
                {
                    // If we are waiting for a gamepad input and the user presses escape
                    // we need to handle it properly
                    getGamepad().catchNextButton(nullptr);
                    _waiting_input = false;
                    _btn_jump->updateColor();
                    _btn_fire->updateColor();
                    _btn_retry->updateColor();
                    updateGamepadButtons();
                }
                else if (_state == ST_KEYBOARD)
                {
                    // Same thing if we are waiting for keyboard input and the user presses
                    // start on his gamepad
                    getKeyboard().catchNextKey(nullptr);
                    _waiting_input = false;
                    _key_up->updateColor();
                    _key_down->updateColor();
                    _key_left->updateColor();
                    _key_right->updateColor();
                    _key_jump->updateColor();
                    _key_fire->updateColor();
                    _key_retry->updateColor();
                    updateKeyboardKeys();
                }
            }
        } // else if (!_waiting_input)*/
    }

    virtual void onRender(gfx::Renderer & renderer, const sf::Time & render_time) const
    {
        gfx::DrawContext & draw = renderer.openDrawContext();

        draw.fill(sf::Color(16, 16, 16));

        draw.close();


        gfx::GuiContext & gui = renderer.openGuiContext();

        _title.draw(gui);

        _quit.draw(gui);

        gui.close();
    }

    virtual void onMouseMove(const ut::Vector & position)
    {
        _navi.dispatchMouseMove(position);
    }

    virtual void onMouseClick(const ut::Vector & position)
    {
        _navi.dispatchMouseClick(position);
    }

    void onQuit()
    {
        setNextScreen(_previous_menu);
        _navi.reset();
    }

/*    void updateThresh(unsigned value)
    {
        getGamepad().setAxisThreshold(100 - value);
    }

    void hookGamepad(gui::ButtonLabel * button_label, ActionId action)
    {
        _waiting_input = true;
        button_label->setText("gamepad.pressbutton");
        button_label->setTextColor(sf::Color::Red);
        getGamepad().catchNextButton([this, button_label, action](Gamepad::Button button)
            {
                // Map a button
                getGamepad().mapButtonToAction(button, action);

                // Update button text
                button_label->setTextColor(sf::Color::White);
                updateGamepadButtons();

                // Reset controls, just in case
                getControls().reset();
                _waiting_input = false;
            });
    }

    void updateGamepadButtons()
    {
        _thresh->setValue(100 - getGamepad().getAxisThreshold());
        _btn_jump->setButton(getGamepad().getButtonFromAction(ACT_JUMP));
        _btn_fire->setButton(getGamepad().getButtonFromAction(ACT_FIRE));
        _btn_retry->setButton(getGamepad().getButtonFromAction(ACT_RETRY));

        _gamepad_menu->updateChilds();
    }*/

};
// class GamepadOptionsScreen

}
// namespace sys

#endif // _IWBAN_GAMEPADOPTIONSSCREEN_HPP_
