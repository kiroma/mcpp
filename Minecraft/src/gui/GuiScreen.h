#ifndef MINECRAFT_GUISCREEN_H
#define MINECRAFT_GUISCREEN_H

#include "Gui.h"

#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Keyboard.hpp>

class GuiScreen : public Gui
{
public:
    // Virtual destructor stub
    virtual ~GuiScreen()
    {}

    // Update
    virtual void DrawScreen() = 0;

    // Events
    virtual void MouseDown(sf::Event::MouseButtonEvent button) = 0;
    virtual void MouseUp(sf::Event::MouseButtonEvent button) = 0;
    virtual void KeyDown(sf::Event::KeyEvent key) = 0;
    virtual void KeyUp(sf::Event::KeyEvent key) = 0;
};

#endif //MINECRAFT_GUISCREEN_H
