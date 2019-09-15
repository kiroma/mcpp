#ifndef MINECRAFT_VECTORS_H
#define MINECRAFT_VECTORS_H

#include <vector>
#include <algorithm>

namespace Vectors
{
    template<typename T>
    void Concat(std::vector<T> &a, std::vector<T> &b)
    {
        a.insert(a.end(), b.begin(), b.end());
    }
}

#endif //MINECRAFT_VECTORS_H
