cmake_minimum_required(VERSION 3.14)
project(Minecraft)

add_compile_definitions(GLEW_STATIC)
add_compile_definitions(MINECRAFT_DEBUG)

add_compile_options(-Wall -Wextra -pedantic)

if (WIN32)
    include_directories(../Common/lib/glew-2.1.0/include)
    include_directories(../Common/lib/glm)
    include_directories(../Common/lib/sfml/include)
    include_directories(../Common/lib/stb)
    include_directories(../Common/lib/noise)

    link_directories(../Common/lib/glew-2.1.0/lib/Release/Win32)
    link_directories(../Common/lib/sfml/lib)

    if (CMAKE_BUILD_TYPE MATCHES Debug)
        link_libraries(mingw32 glew32s opengl32 openal32 sfml-graphics-d sfml-window-d sfml-system-d sfml-audio-d)
    elseif (CMAKE_BUILD_TYPE MATCHES Release)
        link_libraries(mingw32 glew32s opengl32 openal32 sfml-graphics sfml-window sfml-system sfml-audio)
    endif ()
else()
    find_package(GLEW REQUIRED)
    find_package(glm REQUIRED)
    find_package(SFML COMPONENTS graphics REQUIRED)
    find_package(OpenGL REQUIRED)
endif (WIN32)

add_executable(Minecraft src/world/Block.cpp
src/world/Chunk.cpp
src/world/FullChunk.cpp
src/world/gen/biome/BiomePlains.cpp
src/world/gen/biome/BiomeExtremeHills.cpp
src/world/gen/OverworldGenerator.cpp
src/world/gen/noise/SimplexNoise.cpp
src/world/World.cpp
src/Minecraft.cpp
src/utils/InputOutput.cpp
src/utils/math/DirectionVectors.cpp
src/utils/math/MathHelper.cpp
src/render/text/TextRenderer.cpp
src/render/text/FontRenderer.cpp
src/render/Texture.cpp
src/render/TextureAtlas.cpp
src/render/RenderMaster.cpp
src/render/shaders/ShaderProgram.cpp
src/render/shaders/Shader.cpp
src/performance/ChunkStatistics.cpp
src/main.cpp
src/gui/GuiDebug.cpp
src/gui/ScaledResolution.cpp
src/entity/Entity.cpp
src/entity/Camera.cpp)

target_link_libraries(Minecraft PRIVATE GLEW::GLEW sfml-graphics OpenGL::GL)
target_compile_features(Minecraft cxx_std_17)
