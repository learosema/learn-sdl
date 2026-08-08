#pragma once

#include <algorithm>
#include <array>
#include <cmath>
#include <random>

// Classic Ken Perlin "improved noise" (2002), 2D, plus fractal Brownian motion.
class Perlin {
public:
    explicit Perlin(unsigned seed = 1337) { Reseed(seed); }

    void Reseed(unsigned seed) {
        for (int i = 0; i < 256; ++i) {
            _perm[i] = i;
        }

        std::mt19937 rng(seed);
        std::shuffle(_perm.begin(), _perm.begin() + 256, rng);

        for (int i = 0; i < 256; ++i) {
            _perm[256 + i] = _perm[i];
        }
    }

    // Single octave, range roughly [-1, 1]
    double Noise(double x, double y) const {
        int xi = static_cast<int>(std::floor(x)) & 255;
        int yi = static_cast<int>(std::floor(y)) & 255;
        double xf = x - std::floor(x);
        double yf = y - std::floor(y);

        double u = Fade(xf);
        double v = Fade(yf);

        int aa = _perm[_perm[xi] + yi];
        int ab = _perm[_perm[xi] + yi + 1];
        int ba = _perm[_perm[xi + 1] + yi];
        int bb = _perm[_perm[xi + 1] + yi + 1];

        double x1 = Lerp(Grad(aa, xf, yf), Grad(ba, xf - 1, yf), u);
        double x2 = Lerp(Grad(ab, xf, yf - 1), Grad(bb, xf - 1, yf - 1), u);

        return Lerp(x1, x2, v);
    }

    // Layered octaves for more natural-looking terrain, range roughly [-1, 1]
    double Fbm(double x, double y, int octaves = 4, double persistence = 0.5) const {
        double total = 0.0;
        double amplitude = 1.0;
        double frequency = 1.0;
        double maxValue = 0.0;

        for (int i = 0; i < octaves; ++i) {
            total += Noise(x * frequency, y * frequency) * amplitude;
            maxValue += amplitude;
            amplitude *= persistence;
            frequency *= 2.0;
        }

        return total / maxValue;
    }

private:
    std::array<int, 512> _perm{};

    static double Fade(double t) { return t * t * t * (t * (t * 6 - 15) + 10); }
    static double Lerp(double a, double b, double t) { return a + t * (b - a); }

    static double Grad(int hash, double x, double y) {
        // 8 gradient directions instead of the classic 4 diagonals-only
        // variant, which produces visible axis-aligned streaking.
        switch (hash & 7) {
            case 0: return  x;
            case 1: return  x + y;
            case 2: return  y;
            case 3: return -x + y;
            case 4: return -x;
            case 5: return -x - y;
            case 6: return -y;
            default: return  x - y;
        }
    }
};
