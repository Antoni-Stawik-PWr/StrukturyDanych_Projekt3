#include <string>
#include <cmath>


int hash_modular(int key, int tableSize) {
    return key % tableSize;
}


int hash_multiplicative(int key, int tableSize) {
    const double A = 0.6180339887; 
    double frac = fmod(key * A, 1.0); 
    return (int)(tableSize * frac);
}


int hash_xor_shift(int key, int tableSize) {
    key ^= (key >> 16);
    return key % tableSize;
}


unsigned long hash_djb2(const std::string& str, int tableSize) {
    unsigned long hash = 5381;
    for (char c : str) {
        hash = ((hash << 5) + hash) + c; 
    }
    return hash % tableSize;
}
