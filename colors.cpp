#include "colors.h"

const Color darkGray{16, 16, 16, 255};
const Color cyan{0, 255, 255, 255};
const Color magenta{255, 0, 255, 255};
const Color yellow{234, 239, 135, 255};
const Color green{41, 239, 135, 255};
const Color red{255, 72, 70, 255};
const Color purple{160, 74, 245, 255};
const Color orange{255, 102, 0, 255};
const Color menuGray{18, 18, 18, 255};
const Color lightGray{40, 40, 40, 255};

std::vector<Color> GetCellColors()
{
    return {darkGray, cyan, magenta, yellow, green, red, purple, orange};
}