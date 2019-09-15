#include <iostream>
#include "Minecraft.h"

// --------------------------------------------------------------
//  Entry point
// --------------------------------------------------------------
int main(int argc, char *argv[])
{
    Minecraft *minecraft = new Minecraft();
#ifdef MINECRAFT_DEBUG
    std::cout << "Minecraft object at memory address 0x" << std::hex << (unsigned long) minecraft << "." << std::endl;
#endif

    int ret = minecraft->Run();
    delete minecraft;
    return ret;
}
