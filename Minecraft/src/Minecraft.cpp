#include "Minecraft.h"

#include "performance/ChunkStatistics.h"
#include "gui/GuiDebug.h"
#include "gui/ScaledResolution.h"
#include "gui/GuiScreen.h"
#include "render/RenderMaster.h"
#include "render/text/FontRenderer.h"
#include "world/gen/OverworldGenerator.h"
#include "world/World.h"
#include "world/Block.h"
#include "utils/exceptions/ReportedException.h"

#include <GL/glew.h>
#include <iostream>
#include <mem.h>

Minecraft *instance = nullptr;

void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id,
                                GLenum severity,
                                GLsizei length, const GLchar *message, const void *userParam)
{
    // Unused parameters
    (void) source;
    (void) type;
    (void) id;
    (void) severity;
    (void) length;
    (void) message;
    (void) userParam;

    // Useful stuff
    switch (type) {
        case GL_DEBUG_TYPE_ERROR:
            std::cout << "Error: \"" << message << "\" from 0x" << std::hex << source << " severity: 0x" << std::hex
                      << severity << std::endl;
            //Minecraft::GetInstance().Shutdown();
            break;
        default:
            //std::cout << "Debug: \"" << message << "\" from 0x" << std::hex << source << " severity: 0x" << std::hex << severity << std::endl;
            break;
    }
}

Minecraft::Minecraft()
        : running(false)
{
    instance = this;
    input.fill(false);
}

Minecraft::~Minecraft()
{ instance = nullptr; }

// --------------------------------------------------------------
//  Run game:
//      Calls StartGame() and enters a loop rendering
//      and waiting for events
// --------------------------------------------------------------
int Minecraft::Run()
{
    // Start game
    try {
        int i = StartGame();
        if (i != 0) return i;
    } catch (ReportedException &e) {
        std::cerr << "Reported exception thrown whilst starting game." << std::endl;
        std::cerr << e.GetMessage() << std::endl;
        std::cerr << "Source: " << e.GetSource() << std::endl;
        std::cerr << "More details: " << std::endl << e.GetDetails() << std::endl;
        return -1;
    } catch (std::exception &e) {
        std::cerr << "Unknown unhandled STL exception whilst starting game." << std::endl;
        std::cerr << e.what() << std::endl;
        return -1;
    }

    running = true;
    gameFocus = true;

    // Game loop
    try {
        sf::Clock clock;
        while (running) {
            // Event handling
            sf::Event event;
            while (window->pollEvent(event)) {
                switch (event.type) {
                    case sf::Event::Closed:
                        Shutdown();
                        return false;
                    case sf::Event::Resized:
                        UpdateProjection();
                        scaledRes->RealResize(event.size.width, event.size.height);
                        std::cout << "Window resized to " << (float) event.size.width << " * "
                                  << (float) event.size.height
                                  << std::endl;
                        break;
                    case sf::Event::KeyPressed:
                        Input(event.key.code, true);
                        currentScreen->KeyDown(event.key);
                        break;
                    case sf::Event::KeyReleased:
                        Input(event.key.code, false);
                        currentScreen->KeyUp(event.key);
                        break;
                    case sf::Event::MouseButtonPressed:
                        gameFocus = true;
                        currentScreen->MouseDown(event.mouseButton);
                        break;
                    case sf::Event::MouseButtonReleased:
                        currentScreen->MouseUp(event.mouseButton);
                        break;
                    case sf::Event::LostFocus:
                        gameFocus = false;
                        break;
                    default:
                        break;
                }
            }

            // Record the time the last tick took
            deltaTime = clock.restart().asSeconds();

            // Player input
            auto mousePos = glm::vec2((float) sf::Mouse::getPosition().x, (float) sf::Mouse::getPosition().y);

            if (GetInput(sf::Keyboard::Key::Escape)) {
                gameFocus = false;
            }

            if (gameFocus) {
                window->setMouseCursorVisible(false);
                sf::Mouse::setPosition(sf::Vector2i(window->getPosition().x + window->getSize().x / 2,
                                                    window->getPosition().y + window->getSize().y / 2));
            } else {
                window->setMouseCursorVisible(true);
            }

            auto mouseNew = glm::vec2((float) sf::Mouse::getPosition().x, (float) sf::Mouse::getPosition().y);
            mousePosition += mousePos - mouseNew;

            // Game logic
            world->GLTick();
            chunkStatistics->Update();

            // Render
            masterRenderer->RenderWorld(*world);
            window->display();
        }
    } catch (ReportedException &e) {
        std::cerr << "Reported exception thrown during game runtime." << std::endl;
        std::cerr << e.GetMessage() << std::endl;
        std::cerr << "Source: " << e.GetSource() << std::endl;
        std::cerr << "More details: " << std::endl << e.GetDetails() << std::endl;
        return -1;
    } catch (std::runtime_error &e) {
        std::cerr << "Unhandled STL runtime exception during game runtime." << std::endl;
        std::cerr << e.what() << std::endl;
        return -1;
    } catch (std::exception &e) {
        std::cerr << "Unknown unhandled STL exception during game runtime." << std::endl;
        std::cerr << e.what() << std::endl;
        return -1;
    }

    delete window;
    return 0;
}

void Minecraft::Shutdown()
{
    std::cout << "Shutting down..." << std::endl;
    running = false;
}

void Minecraft::Input(int vkey, bool state)
{ if (input[vkey] != state) input[vkey] = state; }

bool Minecraft::GetInput(int vkey) const
{ return input[vkey]; }

glm::vec2 Minecraft::GetMousePosition() const
{ return mousePosition; }

void Minecraft::UpdateProjection() const
{
    masterRenderer->SubmitProjection(
            glm::perspective(glm::radians(80.0f), ((float) window->getSize().x) / ((float) window->getSize().y), 0.01f,
                             1000.0f));
    glViewport(0, 0, (float) window->getSize().x, (float) window->getSize().y);
}

void Minecraft::DisplayGuiScreen(GuiScreen *gui_screen)
{
    GuiScreen *last = currentScreen;
    if (last != nullptr) {
        delete last;
    }

    currentScreen = gui_screen;
}

// --------------------------------------------------------------
//  Initialize game window, pixel format,
//  OpenGL and modern OpenGL
// --------------------------------------------------------------
int Minecraft::StartGame()
{
    sf::ContextSettings settings;
    settings.antialiasingLevel = 0;
    settings.majorVersion = 3;
    settings.minorVersion = 3;
    settings.depthBits = 24;
    settings.stencilBits = 8;

    window = new sf::RenderWindow;
    window->create(sf::VideoMode(1440, 720), std::move("Minecraft C++"),
                   sf::Style::Default, settings);   // 2:1 aspect ratio
    window->setFramerateLimit(144);

    GLenum error = glewInit();
    if (error != GLEW_NO_ERROR) {
        delete window;
        throw ReportedException("Initializing OpenGL extensions while starting game.", "Unable to initialize GLEW",
                                std::string((const char *) glewGetErrorString(error)));
    }

    std::cout << "OpenGL " << glGetString(GL_VERSION) << " " << glGetString(GL_RENDERER) << " "
              << glGetString(GL_VENDOR) << " GLSL " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

    scaledRes = std::make_unique<ScaledResolution>(window->getSize().x, window->getSize().y);
    scaledRes->SetScaleFactor(3);

    fontRenderer = std::make_unique<FontRenderer>("../res/textures/font/ascii.png");
    DrawSplashScreen();

#ifdef MINECRAFT_DEBUG
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, nullptr);
#endif

    // Welcome message
    std::cout << "==============================================" << std::endl;
    std::cout << "                MINECRAFT C++                 " << std::endl;
    std::cout << "      https://github.com/jlagarespo/mcpp      " << std::endl;
    std::cout << "==============================================" << std::endl;

    // Initialize stuff
    masterRenderer = std::make_unique<RenderMaster>();
    blockDatabase = std::make_unique<Block::Database>();
    world = std::make_unique<World>(time(0));
    chunkStatistics = std::make_unique<ChunkStats::Statistics>();
    world->LoadGenerator(std::make_unique<OverworldGenerator>());
    chunkStatistics->Update();

    masterRenderer->LoadCamera(std::make_unique<Camera>(glm::vec3(0, world->GetHighestPoint(glm::ivec2(0, 0)) + 4, 0)));
    UpdateProjection();
    DisplayGuiScreen(new GuiDebug());

    return 0;
}

void Minecraft::DrawSplashScreen() const
{
    // Load the splash image
    Texture texture("../res/textures/splash.png");
    ShaderProgram shader;
    unsigned int vaoID, verticesID, textureCoordsID;

    // Prepare our shader
    shader.AttachShader(std::make_unique<Shader>(VERTEX, "../res/shaders/font/vertex.glsl"));
    shader.AttachShader(std::make_unique<Shader>(FRAGMENT, "../res/shaders/font/fragment.glsl"));
    shader.Build();

    // Bind it to texture 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture.GetID());
    glPushMatrix();

    // Draw!
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    shader.Enable();
    glUniformMatrix4fv(shader.UniformLocation("matrix"), 1, false, glm::value_ptr(scaledRes->GenerateMatrix()));
    glUniform1i(shader.UniformLocation("main_texture"), 0);
    glUniform4f(shader.UniformLocation("main_color"), 1, 1, 1, 1);

    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    // OpenGL call wall
    glGenVertexArrays(1, &vaoID);
    glGenBuffers(1, &verticesID);
    glGenBuffers(1, &textureCoordsID);
    glBindBuffer(GL_ARRAY_BUFFER, verticesID);
    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, textureCoordsID);
    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);
    glBindVertexArray(vaoID);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, verticesID);
    glVertexAttribPointer(0, 2, GL_FLOAT, false, 0, nullptr);
    glBindBuffer(GL_ARRAY_BUFFER, textureCoordsID);
    glVertexAttribPointer(1, 2, GL_FLOAT, false, 0, nullptr);

    float w = texture.GetWidth();
    float h = texture.GetHeight();
    float x = scaledRes->GetScaledWidth() / 2 - w / 2;
    float y = scaledRes->GetScaledHeight() / 2 - h / 2;

    const float vertices[] = {
            x, y,
            x + w, y,
            x + w, y + h,
            x, y + h
    };

    const float texturecoordinates[] = {
            0, 0,
            1, 0,
            1, 1,
            0, 1
    };

    glBindBuffer(GL_ARRAY_BUFFER, verticesID);
    glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), vertices, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, textureCoordsID);
    glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), texturecoordinates, GL_DYNAMIC_DRAW);

    glDrawArrays(GL_QUADS, 0, 8);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDeleteBuffers(1, &verticesID);
    glDeleteBuffers(1, &textureCoordsID);
    glDeleteVertexArrays(1, &vaoID);

    // Restore state
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
    glPopMatrix();

    window->display();
}

// --------------------------------------------------------------
//  Get a static reference to the running game
// --------------------------------------------------------------
Minecraft &Minecraft::GetInstance()
{ return *instance; }
