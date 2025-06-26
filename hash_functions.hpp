#pragma once
#include <string>
#include <cmath>
#include <random>
#include "HashUtils.hpp"

/* 1.1  Modularne haszowanie */
inline int hash_modular(int key, int m)
{
    return wrap(key, m);
}

/* 1.2  Metoda mnożenia (Knuth) */
inline int hash_multiplicative(int key, int m)
{
    const double A = 0.618033988749894848;          // (√5−1)/2
    double frac = std::fmod(key * A, 1.0);
    return static_cast<int>(m * frac);
}

/* 1.3  XOR-shift */
inline int hash_xor_shift(int key, int m)
{
    key ^= (key >> 16);
    return wrap(key, m);
}

/* 1.4  Uniwersalny (Carter-Wegman) */
inline int hash_universal(int key, int m)
{
    static const int P = 2'147'483'647;             // 2^31−1 (pierwsza)

    /* losujemy a, b tylko raz (statyczne inicjalizatory lambda) */
    static const int a = [] {
        std::mt19937 gen{ std::random_device{}() };
        std::uniform_int_distribution<int> d(1, P - 1);
        return d(gen);
        }();
    static const int b = [] {
        std::mt19937 gen{ std::random_device{}() };
        std::uniform_int_distribution<int> d(0, P - 1);
        return d(gen);
        }();

    long long h = (1LL * a * key + b) % P;
    return static_cast<int>(h % m);
}

/* 2.1  DJB2 (napisy) */
inline unsigned long hash_djb2(const std::string& s, int m)
{
    unsigned long h = 5381;
    for (unsigned char c : s) h = ((h << 5) + h) + c;
    return h % m;
}