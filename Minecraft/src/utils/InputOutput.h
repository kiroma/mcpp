#ifndef MINECRAFT_INPUTOUTPUT_H
#define MINECRAFT_INPUTOUTPUT_H

#include <string>
#include <vector>

namespace InputOutput
{
    std::vector<char> ReadFile(std::string filename);
    bool FileExists(std::string filename);
}

#endif //MINECRAFT_INPUTOUTPUT_H
