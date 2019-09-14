#include <iostream>
#include "Minecraft.h"

// --------------------------------------------------------------
//  Entry point
// --------------------------------------------------------------
int main(int argc, char *argv[]) {
    Minecraft *minecraft = new Minecraft();
    std::cout << "Minecraft object at memory address 0x" << std::hex << (unsigned long) minecraft << "." << std::endl;
    int ret = minecraft->Run();
    delete minecraft;
    return ret;
}
