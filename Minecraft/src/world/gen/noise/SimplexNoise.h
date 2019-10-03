/**
 * WARNING:
 *  This file is taken from https://github.com/SRombauts/SimplexNoise/blob/master/src/SimplexNoise.cpp,
 *  and has been modified and refactored to fit the project.
 *  Extreme credit to the original author for it!
 * */

#include <cstddef>

class SimplexNoise
{
public:
    static float noise(float x);
    static float noise(float x, float y);
    static float noise(float x, float y, float z);

    float fractal(size_t octaves, float x) const;
    float fractal(size_t octaves, float x, float y) const;
    float fractal(size_t octaves, float x, float y, float z) const;

    explicit SimplexNoise(float frequency = 1.0f,
                          float amplitude = 1.0f,
                          float lacunarity = 2.0f,
                          float persistence = 0.5f) :
            freq(frequency),
            amp(amplitude),
            lac(lacunarity),
            per(persistence)
    {}

private:
    float freq, amp, lac, per;
};
