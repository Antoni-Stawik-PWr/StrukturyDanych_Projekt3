#pragma once

inline int wrap(int h, int m)
{
    return ((h % m) + m) % m;
}
