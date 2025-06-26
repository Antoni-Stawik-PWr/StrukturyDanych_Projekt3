#pragma once
#include <vector>
#include <functional>
#include <optional>
#include "HashUtils.hpp"

class HashTableQuadratic
{
    struct Entry { int key = 0; bool deleted = false; };

    std::vector<std::optional<Entry>> table;
    std::function<int(int, int)>       hashFunc;
    int  m;
    int  probesIns = 0, probesRem = 0;

public:
    HashTableQuadratic(int size, std::function<int(int, int)> h)
        : table(size), hashFunc(std::move(h)), m(size) {
    }

    bool insert(int key)
    {
        int start = wrap(hashFunc(key, m), m);

        for (int i = 0; i < m; ++i) {
            ++probesIns;
            long long step = 1LL * i * i;                     // ✔︎ 64-bit
            int idx = static_cast<int>((start + step) % m);
            auto& s = table[idx];

            if (!s) { s = Entry{ key,false }; return true; }
            if (s->deleted) { *s = Entry{ key,false }; return true; }
            if (s->key == key)     return false;                       // dup
        }
        return false;                                               // full
    }

    bool remove(int key)
    {
        int start = wrap(hashFunc(key, m), m);

        for (int i = 0; i < m; ++i) {
            ++probesRem;
            long long step = 1LL * i * i;
            int idx = static_cast<int>((start + step) % m);
            auto& s = table[idx];

            if (!s)                       return false;
            if (!s->deleted && s->key == key) { s->deleted = true; return true; }
        }
        return false;
    }

    int getInsertProbes() const { return probesIns; }
    int getRemoveProbes() const { return probesRem; }
};