#pragma once
#include <raylib.h>
#include <vector>

// cores que serão possivelmente usadas no jogo
extern const Color darkGray;
extern const Color cyan;
extern const Color magenta;
extern const Color yellow;
extern const Color green;
extern const Color red;
extern const Color purple;
extern const Color blue;
extern const Color orange;
extern const Color menuGray;
extern const Color lightGray;

std::vector<Color> GetCellColors();