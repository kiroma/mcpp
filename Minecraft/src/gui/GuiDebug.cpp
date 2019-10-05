#include "GuiDebug.h"

#include "../render/text/FontRenderer.h"
#include "../render/RenderMaster.h"
#include "../render/text/TextRenderer.h"
#include "../gui/ScaledResolution.h"
#include "../performance/ChunkStatistics.h"
#include "../entity/Camera.h"
#include "../Minecraft.h"

#include <iostream>

void GuiDebug::DrawScreen()
{
    const Camera &camera = minecraft.GetMasterRenderer().GetViewCamera();

    TextRenderer text(true, 1, 1);
    if (!extended) {
        text.Print("Minecraft C++ %d FPS", (int) minecraft.GetDebugFPS());
        text.Print("(Press [F3] for more details)");
    } else {
        text.Print("Minecraft C++ %d FPS (DT %.4f)", (int) minecraft.GetDebugFPS(),
                   Minecraft::GetInstance().GetDeltaTime());
        text.Print("%d chunk updates (C %d D %d L %d R %d)", chunkStats.GetChunkUpdates(ChunkStats::ALL),
                   chunkStats.GetChunkUpdates(ChunkStats::CREATE),
                   chunkStats.GetChunkUpdates(ChunkStats::DESTROY),
                   chunkStats.GetChunkUpdates(ChunkStats::LOAD),
                   chunkStats.GetChunkUpdates(ChunkStats::REMESH));
        text.NewLine();
        text.Print("X %.5f", camera.GetPosition().x);
        text.Print("Y %.3f", camera.GetPosition().y);
        text.Print("Z %.5f", camera.GetPosition().z);
        text.Print("F %.2f // %.2f", camera.GetRotation().y, camera.GetRotation().x);

        text.Reset(true, minecraft.GetScaledResolution().GetScaledWidth() - 1, 1);
        text.PrintRight("MC @ 0x%x", &minecraft);
    }
}

void GuiDebug::MouseDown(sf::Event::MouseButtonEvent button)
{ (void) button; }
void GuiDebug::MouseUp(sf::Event::MouseButtonEvent button)
{ (void) button; }
void GuiDebug::KeyDown(sf::Event::KeyEvent key)
{
    if (key.code == sf::Keyboard::Key::F3) {
        extended = !extended;
    }
}

void GuiDebug::KeyUp(sf::Event::KeyEvent key)
{ (void) key; }
