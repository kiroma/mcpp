#ifndef MINECRAFT_MINECRAFT_H
#define MINECRAFT_MINECRAFT_H

#include <vec2.hpp>
#include <gtc/matrix_transform.hpp>
#include <SFML/Graphics.hpp>

#include "gui/ScaledResolution.h"
#include "gui/GuiScreen.h"

class RenderMaster;

class World;

class Minecraft
{
public:
    Minecraft();
    ~Minecraft();

    int Run();
    void Shutdown();

    void Input(int vkey, bool state);
    bool GetInput(int vkey) const;
    glm::vec2 GetMousePosition() const;

    void UpdateProjection() const;
    void DisplayGuiScreen(GuiScreen *gui_screen);

    // Getters
    static Minecraft &GetInstance();
    bool HasFocus()
    { return gameFocus; }
    bool IsRunning() const volatile
    { return running; }
    const RenderMaster &GetMasterRenderer() const
    { return *masterRenderer; }
    const World &GetCurrentWorld() const
    { return *world; }
    float GetDeltaTime() const
    { return deltaTime; }
    const sf::RenderWindow &GetWindow() const
    { return *window; }
    float GetDebugFPS() const
    { return 1.0f / deltaTime; }
    ScaledResolution &GetScaledResolution() const
    { return *scaledRes; }
    GuiScreen *GetCurrentGUIScreen() const
    { return currentScreen; }

private:
    int StartGame();

    bool running;
    bool input[4096];
    bool gameFocus;
    glm::vec2 mousePosition;
    float deltaTime;

    RenderMaster *masterRenderer;
    World *world;
    ScaledResolution *scaledRes;

    sf::RenderWindow *window;

    GuiScreen *currentScreen;
};

#endif //MINECRAFT_MINECRAFT_H
