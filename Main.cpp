#include <iostream>
#include <vector>
#include <functional>
#include <string>
#include <chrono>
#include <locale>
#include <fstream>
#include <iomanip>

#include "hashTable_Linear.hpp"
#include "hashTable_Quadratic.hpp"
#include "hashTable_RobinHood.hpp"
#include "SeparateChaining.hpp"    // HashTableSeparateChaining
#include "TwoChoiceHashing.hpp"    // TwoChoiceHashing
#include "Hash_functions.hpp"

/* --- generatory danych ----------------------------------------------*/
std::vector<int> genOptimistic(int n, int) {
    std::vector<int> v; v.reserve(n);
    for (int i = 0; i < n; ++i) v.push_back(i * 1'000);
    return v;
}
std::vector<int> genAverage(int n, int) {
    std::vector<int> v; v.reserve(n);
    for (int i = 0; i < n; ++i) v.push_back(i);
    return v;
}
std::vector<int> genPessimistic(int n, int m) {
    std::vector<int> v; v.reserve(n);
    for (int i = 0; i < n; ++i) v.push_back(i * m);
    return v;
}

int main()
{
    std::locale::global(std::locale::classic());                 // liczby z kropką
    using namespace std::chrono;

    /* --- plik wynikowy ----------------------------------------------*/
    std::ofstream file("testy.csv");
    file.imbue(std::locale::classic());
    file << "Przypadek;Rozmiar;Wypełnienie;Funkcja;Strategia;"
        "CzasInsert[ms];AvgInsertProbes;CzasRemove[ms];AvgRemoveProbes\n";

    /* --- parametry testu --------------------------------------------*/
    std::vector<int> tableSizes;
    for (int s = 10'000; s <= 100'000; s += 10'000) tableSizes.push_back(s);

    std::vector<double> fillFactors{ 0.25, 0.50, 0.75, 0.90, 0.99 };

    std::vector<std::pair<std::string, std::function<int(int, int)>>> hashFuncs = {
        { "Modular",         hash_modular        },
        { "Multiplicative",  hash_multiplicative },
        { "XOR+Shift",       hash_xor_shift      },
        { "Universal",       hash_universal      }
    };

    std::vector<std::pair<std::string, std::vector<int>(*)(int, int)>> dataCases = {
        { "Optymistyczny", genOptimistic  },
        { "Średni",        genAverage     },
        { "Pesymistyczny", genPessimistic }
    };

    const int totalSteps =
        tableSizes.size() * fillFactors.size() *
        hashFuncs.size() * dataCases.size() * 5;        // 5 strategii
    int step = 0;

    /* --- główne pętle ------------------------------------------------*/
    for (auto [caseName, gen] : dataCases)
    {
        for (int m : tableSizes)
        {
            for (double fill : fillFactors)
            {
                int n = static_cast<int>(m * fill);
                std::vector<int> keys = gen(n, m);

                for (auto [hashName, h] : hashFuncs)
                {
                    auto test = [&](auto& ht, const std::string& strat)
                        {
                            auto t0 = high_resolution_clock::now();
                            for (int k : keys) ht.insert(k);
                            auto t1 = high_resolution_clock::now();

                            auto t2 = high_resolution_clock::now();
                            for (int k : keys) ht.remove(k);
                            auto t3 = high_resolution_clock::now();

                            file << caseName << ';' << m << ';' << int(fill * 100) << ';'
                                << hashName << ';' << strat << ';'
                                << std::fixed << std::setprecision(3)
                                << duration_cast<microseconds>(t1 - t0).count() / 1000.0 << ';'
                                << double(ht.getInsertProbes()) / n << ';'
                                << duration_cast<microseconds>(t3 - t2).count() / 1000.0 << ';'
                                << double(ht.getRemoveProbes()) / n << '\n';

                            std::cout << '[' << ++step << '/' << totalSteps << "] "
                                << std::left << std::setw(16) << strat << "\r";
                        };

                    /* pięć strategii */
                    { HashTableLinear             ht(m, h);                   test(ht, "Linear"); }
                    { HashTableQuadratic          ht(m, h);                   test(ht, "Quadratic"); }
                    { HashTableRobinHood          ht(m, h);                   test(ht, "RobinHood"); }
                    { HashTableSeparateChaining   ht(m, h);                   test(ht, "SeparateChain"); }
                    { TwoChoiceHashing            ht(m, h, hash_xor_shift);   test(ht, "TwoChoice"); }
                }
            }
        }
    }

    std::cout << "\n✅ Testy zakończone. Wyniki zapisano do pliku testy.csv\n";
    return 0;
}
