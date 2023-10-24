#ifndef NOISE_HPP
#define NOISE_HPP

#include <cstddef>  // size_t

class SimplexNoise {
public:
    // 1D Perlin simplex noise
    static float noise(float x);
    // 2D Perlin simplex noise
    static float noise(float x, float y);
    // 3D Perlin simplex noise
    static float noise(float x, float y, float z);

    // Fractal/Fractional Brownian Motion (fBm) noise summation
    float fractal(size_t octaves, float x);
    float fractal(size_t octaves, float x, float y);
    float fractal(size_t octaves, float x, float y, float z);

    SimplexNoise(float frequency = 20,
                          float amplitude = 0.20,
                          float lacunarity = 2.0,
                          float persistence = 1) :
        mFrequency(frequency),
        mAmplitude(amplitude),
        mLacunarity(lacunarity),
        mPersistence(persistence) {
    }

private:
    // Parameters of Fractional Brownian Motion (fBm) : sum of N "octaves" of noise
    float mFrequency = 20;   ///< Frequency ("width") of the first octave of noise (default to 1.0)
    float mAmplitude = 0.20;   ///< Amplitude ("height") of the first octave of noise (default to 1.0)
    float mLacunarity = 2.0;  ///< Lacunarity specifies the frequency multiplier between successive octaves (default to 2.0).
    float mPersistence = 1; ///< Persistence is the loss of amplitude between successive octaves (usually 1/lacunarity)
};

#include "SimplexNoise.cpp"

#endif