#pragma once
#include <vector>
#include <list>
#include <functional>
#include "HashUtils.hpp"

/*─────────────────────────────────────────────────────────────
  Hash table – separate chaining
─────────────────────────────────────────────────────────────*/
class HashTableSeparateChaining
{
    std::vector<std::list<int>>        table;
    std::function<int(int, int)>        hashFunc;
    int                                m;

    /* statystyki: licznik porównań przy insert/remove */
    long long probesIns = 0, probesRem = 0;

public:
    HashTableSeparateChaining(int size,
        std::function<int(int, int)> h)
        : table(size), hashFunc(std::move(h)), m(size) {
    }

    bool insert(int key)
    {
        int idx = wrap(hashFunc(key, m), m);
        auto& bucket = table[idx];

        for (int k : bucket) {           // wyszukiwanie duplikatu
            ++probesIns;
            if (k == key) return false;
        }
        bucket.push_front(key);
        return true;
    }

    bool remove(int key)
    {
        int idx = wrap(hashFunc(key, m), m);
        auto& bucket = table[idx];

        for (auto it = bucket.begin(); it != bucket.end(); ++it) {
            ++probesRem;
            if (*it == key) {
                bucket.erase(it);
                return true;
            }
        }
        return false;
    }

    long long getInsertProbes() const { return probesIns; }
    long long getRemoveProbes() const { return probesRem; }
};
