#include <string>
#include <cmath>

// --- 1.1 Modularne haszowanie ---
int hash_modular(int key, int tableSize) {
    return key % tableSize;
}

// --- 1.2 Haszowanie przez mno¿enie ---
int hash_multiplicative(int key, int tableSize) {
    const double A = 0.6180339887; // (sqrt(5) - 1)/2
    double frac = fmod(key * A, 1.0); // tylko czêœæ u³amkowa
    return (int)(tableSize * frac);
}

// --- 1.3 Haszowanie XOR + przesuniêcia ---
int hash_xor_shift(int key, int tableSize) {
    key ^= (key >> 16);
    return key % tableSize;
}

// --- 2.1 DJB2 – haszowanie tekstu ---
unsigned long hash_djb2(const std::string& str, int tableSize) {
    unsigned long hash = 5381;
    for (char c : str) {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }
    return hash % tableSize;
}