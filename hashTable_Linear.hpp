#pragma once
#include <vector>
#include <functional>
#include <optional>
#include "HashUtils.hpp"

class HashTableLinear
{
    struct Entry { int key = 0; bool deleted = false; };

    std::vector<std::optional<Entry>> table;
    std::function<int(int, int)>       hashFunc;
    int  m;
    int  probesIns = 0, probesRem = 0;

public:
    HashTableLinear(int size, std::function<int(int, int)> h)
        : table(size), hashFunc(std::move(h)), m(size) {
    }

    bool insert(int key)
    {
        int start = wrap(hashFunc(key, m), m);

        for (int i = 0; i < m; ++i) {
            ++probesIns;
            int idx = (start + i) % m;
            auto& s = table[idx];

            if (!s) {                         // całkiem puste
                s = Entry{ key,false };
                return true;
            }
            if (s->deleted) {                 // „nagrobek”
                *s = Entry{ key,false };
                return true;
            }
            if (s->key == key)                // duplikat
                return false;
        }
        return false;                         // pełna tablica
    }

    bool remove(int key)
    {
        int start = wrap(hashFunc(key, m), m);

        for (int i = 0; i < m; ++i) {
            ++probesRem;
            int idx = (start + i) % m;
            auto& s = table[idx];

            if (!s)                 return false;
            if (!s->deleted && s->key == key) { s->deleted = true; return true; }
        }
        return false;
    }

    int getInsertProbes() const { return probesIns; }
    int getRemoveProbes() const { return probesRem; }
    void resetProbes() { probesIns = probesRem = 0; }
};