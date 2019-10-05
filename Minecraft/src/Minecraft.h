#ifndef MINECRAFT_MINECRAFT_H
#define MINECRAFT_MINECRAFT_H

#include <vec2.hpp>
#include <gtc/matrix_transform.hpp>
#include <SFML/Graphics.hpp>

class ScaledResolution;

class GuiScreen;

class RenderMaster;

class World;

class FontRenderer;

namespace Block
{
    class Database;
}

namespace ChunkStats
{
    class Statistics;
}

class Minecraft
{
    int StartGame();
    void DrawSplashScreen() const;

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
    bool HasFocus()
    { return gameFocus; }
    bool IsRunning() const
    { return running; }
    float GetDeltaTime() const
    { return deltaTime; }
    float GetDebugFPS() const
    { return 1.0f / deltaTime; }
    const sf::RenderWindow &GetWindow() const
    { return *window; }
    GuiScreen *GetCurrentGUIScreen() const
    { return currentScreen; }

    RenderMaster &GetMasterRenderer() const
    { return *masterRenderer; }
    World &GetCurrentWorld() const
    { return *world; }
    ScaledResolution &GetScaledResolution() const
    { return *scaledRes; }
    Block::Database &GetBlockDatabase() const
    { return *blockDatabase; }
    ChunkStats::Statistics &GetChunkStatistics() const
    { return *chunkStatistics; }
    FontRenderer &GetFontRenderer() const
    { return *fontRenderer; }

    static Minecraft &GetInstance();

private:
    // Game state
    bool running;
    bool gameFocus;
    float deltaTime;
    std::array<bool, 4096> input;
    glm::vec2 mousePosition;
    GuiScreen *currentScreen = nullptr;

    // Game stuff
    std::unique_ptr<RenderMaster> masterRenderer;
    std::unique_ptr<World> world;
    std::unique_ptr<ScaledResolution> scaledRes;
    std::unique_ptr<Block::Database> blockDatabase;
    std::unique_ptr<ChunkStats::Statistics> chunkStatistics;
    std::unique_ptr<FontRenderer> fontRenderer;

    // SFML window
    sf::RenderWindow *window;
};

#endif //MINECRAFT_MINECRAFT_H
