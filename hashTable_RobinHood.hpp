#pragma once
#include <vector>
#include <functional>
#include "HashUtils.hpp"

class HashTableRobinHood
{
    struct Slot { int key = 0, probe = 0; bool occupied = false, deleted = false; };

    std::vector<Slot>           table;
    std::function<int(int, int)> hashFunc;
    int m;
    int probesIns = 0, probesRem = 0;

public:
    HashTableRobinHood(int size, std::function<int(int, int)> h)
        : table(size), hashFunc(std::move(h)), m(size) {
    }

    bool insert(int key)
    {
        int start = wrap(hashFunc(key, m), m);
        Slot cur{ key, 0, true, false };

        for (int d = 0; d < m; ++d, ++cur.probe) {
            ++probesIns;
            int idx = (start + d) % m;
            auto& s = table[idx];

            if (!s.occupied || s.deleted) { s = cur; return true; }
            if (s.key == key)             return false;

            int sStart = wrap(hashFunc(s.key, m), m);
            int sDist = (idx - sStart + m) % m;

            if (sDist < cur.probe) std::swap(cur, s);
        }
        return false;
    }

    bool remove(int key)
    {
        int start = wrap(hashFunc(key, m), m);
        for (int d = 0; d < m; ++d) {
            ++probesRem;
            int idx = (start + d) % m;
            auto& s = table[idx];

            if (!s.occupied)               return false;
            if (!s.deleted && s.key == key) { s.deleted = true; return true; }
        }
        return false;
    }

    int getInsertProbes() const { return probesIns; }
    int getRemoveProbes() const { return probesRem; }
};
