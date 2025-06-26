// TwoChoiceHashing.hpp
#pragma once
#include <vector>
#include <optional>
#include <functional>
#include "HashUtils.hpp"

/*─────────────────────────────────────────────────────────────
  Two-Choice Hashing  (power of two choices)
  - dla klucza liczymy dwa niezależne hasze: h1, h2
  - wstawiamy do krótszego kubełka; tu każdy kubełek = pojedynczy slot,
    więc „krótszy” = pusty. Jeśli oba zajęte → liniowe dosondowanie.
─────────────────────────────────────────────────────────────*/
class TwoChoiceHashing
{
    struct Slot { int key = 0; bool occupied = false; };

    std::vector<Slot>           table;
    std::function<int(int, int)> h1, h2;
    int                         m;
    long long                   probesIns = 0, probesRem = 0;

public:
    TwoChoiceHashing(int size,
        std::function<int(int, int)> hashA,
        std::function<int(int, int)> hashB)
        : table(size), h1(std::move(hashA)), h2(std::move(hashB)), m(size) {
    }

    bool insert(int key)
    {
        int idx1 = wrap(h1(key, m), m);
        int idx2 = wrap(h2(key, m), m);

        /* krótszy kubełek = pusty slot */
        if (!table[idx1].occupied) {
            table[idx1] = { key, true };
            return true;
        }
        if (!table[idx2].occupied) {
            table[idx2] = { key, true };
            return true;
        }

        /* oba zajęte → fallback: liniowe sondowanie */
        for (int step = 1; step < m; ++step) {
            ++probesIns;
            int idx = (idx1 + step) % m;
            if (!table[idx].occupied) {
                table[idx] = { key, true };
                return true;
            }
            if (table[idx].key == key) return false;   // duplikat
        }
        return false;                                  // pełna tabela
    }

    bool remove(int key)
    {
        int idxs[2] = { wrap(h1(key, m), m), wrap(h2(key, m), m) };

        for (int idx : idxs) {
            ++probesRem;
            if (table[idx].occupied && table[idx].key == key) {
                table[idx].occupied = false;
                return true;
            }
        }
        /* liniowe sondowanie w razie kolizji trzeciego poziomu */
        int idxStart = idxs[0];
        for (int step = 1; step < m; ++step) {
            ++probesRem;
            int idx = (idxStart + step) % m;
            if (!table[idx].occupied) return false;
            if (table[idx].key == key) {
                table[idx].occupied = false;
                return true;
            }
        }
        return false;
    }

    long long getInsertProbes() const { return probesIns; }
    long long getRemoveProbes() const { return probesRem; }
};
