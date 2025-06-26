#pragma once
/*─────────────────────────────────────────────────────────────
  wrap() – wynik % m przenosi do przedziału 0‥m-1  (obsługa <0)
─────────────────────────────────────────────────────────────*/
inline int wrap(int h, int m)
{
    return ((h % m) + m) % m;
}