/**
 * @file   KeyboardOptionsScreen.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_KEYBOARDOPTIONSSCREEN_HPP_
#define _IWBAN_KEYBOARDOPTIONSSCREEN_HPP_

#include <Global.hpp>

#include <config/KeyboardConfig.hpp>

#include <system/Controls.hpp>
#include <system/Screen.hpp>
#include <system/Time.hpp>
#include <system/inputs/Keyboard.hpp>

#include <gui/Button.hpp>
#include <gui/Label.hpp>
#include <gui/Navigation.hpp>

#include <resources/Locale.hpp>

namespace sys
{

class KeyboardOptionsScreen : public Screen
{
private:
    // Data members
    gui::Navigation _navi;

    gui::Label  _title;

    gui::Button _quit;


public:
    // Constructor
    KeyboardOptionsScreen()
    {
        res::getLocale().loadFile("system/keyboard.txt");

        // Title
        _title.loadText("options.keyboard");
        _title.setCharacterSize(50);
        _title.setPosition({320, 40});

        // Quit
        _quit.loadText("options.return");
        _quit.setCallback([this](){ onQuit(); });
        _quit.setPosition({320, 440});

        // Navigation
        _navi.setHead(&_quit);

/*
        _key_up = new gui::KeyLabel(getKeyboard().getKeyFromAction(ACT_UP));
        _key_up->setAction([this](){ hookKeyboard(_key_up, ACT_UP); });
        _keyboard_menu->add(new gui::Line({
            new gui::Frame(ut::Vector(200, 40), new gui::Choice("actions.up")),
            new gui::Frame(ut::Vector(400, 40), _key_up)
        }));

        _key_left = new gui::KeyLabel(getKeyboard().getKeyFromAction(ACT_LEFT));
        _key_left->setAction([this](){ hookKeyboard(_key_left, ACT_LEFT); });
        _keyboard_menu->add(new gui::Line({
            new gui::Frame(ut::Vector(200, 40), new gui::Choice("actions.left")),
            new gui::Frame(ut::Vector(400, 40), _key_left)
        }));

        _key_down = new gui::KeyLabel(getKeyboard().getKeyFromAction(ACT_DOWN));
        _key_down->setAction([this](){ hookKeyboard(_key_down, ACT_DOWN); });
        _keyboard_menu->add(new gui::Line({
            new gui::Frame(ut::Vector(200, 40), new gui::Choice("actions.down")),
            new gui::Frame(ut::Vector(400, 40), _key_down)
        }));

        _key_right = new gui::KeyLabel(getKeyboard().getKeyFromAction(ACT_RIGHT));
        _key_right->setAction([this](){ hookKeyboard(_key_right, ACT_RIGHT); });
        _keyboard_menu->add(new gui::Line({
            new gui::Frame(ut::Vector(200, 40), new gui::Choice("actions.right")),
            new gui::Frame(ut::Vector(400, 40), _key_right)
        }));

        _key_jump = new gui::KeyLabel(getKeyboard().getKeyFromAction(ACT_JUMP));
        _key_jump->setAction([this](){ hookKeyboard(_key_jump, ACT_JUMP); });
        _keyboard_menu->add(new gui::Line({
            new gui::Frame(ut::Vector(200, 40), new gui::Choice("actions.jump")),
            new gui::Frame(ut::Vector(400, 40), _key_jump)
        }));

        _key_fire = new gui::KeyLabel(getKeyboard().getKeyFromAction(ACT_FIRE));
        _key_fire->setAction([this](){ hookKeyboard(_key_fire, ACT_FIRE); });
        _keyboard_menu->add(new gui::Line({
            new gui::Frame(ut::Vector(200, 40), new gui::Choice("actions.fire")),
            new gui::Frame(ut::Vector(400, 40), _key_fire)
        }));

        _key_retry = new gui::KeyLabel(getKeyboard().getKeyFromAction(ACT_RETRY));
        _key_retry->setAction([this](){ hookKeyboard(_key_retry, ACT_RETRY); });
        _keyboard_menu->add(new gui::Line({
            new gui::Frame(ut::Vector(200, 40), new gui::Choice("actions.retry")),
            new gui::Frame(ut::Vector(400, 40), _key_retry)
        }));

        {
            gui::Choice * reset = new gui::Choice("options.defaults");
            reset->setAction([this](){ getKeyboard().reloadDefaults(); updateKeyboardKeys(); });
            _keyboard_menu->add(new gui::Separator(ut::Vector(400, 10)));
            _keyboard_menu->add(new gui::Frame(ut::Vector(400, 40), reset));
        }

        gui::Choice * kcancel = new gui::Choice("options.return");
        kcancel->setAction([this](){ setState(ST_OPTIONS); });
        _keyboard_menu->add(new gui::Frame(ut::Vector(400, 40), kcancel));
*/
    }

    // Destructor
    virtual ~KeyboardOptionsScreen() {}


protected:
    // Callbacks
    virtual void onUpdate(const sf::Time & update_time)
    {
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
        // TODO Quit
        _quit.setTextColor(sf::Color::Red);
    }

/*    void hookKeyboard(gui::KeyLabel * key_label, ActionId action)
    {
        _waiting_input = true;
        key_label->setText("keyboard.presskey");
        key_label->setTextColor(sf::Color::Red);
        getKeyboard().catchNextKey([this, key_label, action](sf::Keyboard::Key key)
            {
                // Map a key, or unmap if Menu is pressed
                if (key == IWBAN_KEYBOARD_MENU)
                {
                    sf::Keyboard::Key prev_key = getKeyboard().getKeyFromAction(action);
                    if (prev_key != sf::Keyboard::KeyCount)
                        getKeyboard().unmapKey(prev_key);
                }
                else
                    getKeyboard().mapKeyToAction(key, action);

                // Update button text
                key_label->setTextColor(sf::Color::White);
                updateKeyboardKeys();

                // Reset controls, just in case
                getControls().reset();
                _waiting_input = false;
            });
    }

    void updateKeyboardKeys()
    {
        _key_up->setKey(getKeyboard().getKeyFromAction(ACT_UP));
        _key_down->setKey(getKeyboard().getKeyFromAction(ACT_DOWN));
        _key_left->setKey(getKeyboard().getKeyFromAction(ACT_LEFT));
        _key_right->setKey(getKeyboard().getKeyFromAction(ACT_RIGHT));
        _key_jump->setKey(getKeyboard().getKeyFromAction(ACT_JUMP));
        _key_fire->setKey(getKeyboard().getKeyFromAction(ACT_FIRE));
        _key_retry->setKey(getKeyboard().getKeyFromAction(ACT_RETRY));

        _keyboard_menu->updateChilds();
    }*/

};
// class KeyboardOptionsScreen

}
// namespace sys

#endif // _IWBAN_KEYBOARDOPTIONSSCREEN_HPP_
