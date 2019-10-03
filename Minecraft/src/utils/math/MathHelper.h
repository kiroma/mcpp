#ifndef MINECRAFT_MATHHELPER_H
#define MINECRAFT_MATHHELPER_H

namespace MathHelper
{
    float Lerp(float s, float e, float t);
    float Blerp(float c00, float c10, float c01, float c11, float x, float y);
}

#endif //MINECRAFT_MATHHELPER_H
