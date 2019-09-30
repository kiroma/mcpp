#ifndef MINECRAFT_GUIDEBUG_H
#define MINECRAFT_GUIDEBUG_H

#include "GuiScreen.h"

class GuiDebug : public GuiScreen
{
public:
    void DrawScreen() override;
    void MouseDown(sf::Mouse::Button button) override;
    void MouseUp(sf::Mouse::Button button) override;
    void KeyDown(sf::Keyboard::Key button) override;
    void KeyUp(sf::Keyboard::Key button) override;
};

#endif //MINECRAFT_GUIDEBUG_H
