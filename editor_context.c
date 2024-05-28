#include <windows.h>
#include "editor_context.h"

Dimensions calculateDimensions(HWND hwnd, int offset) {
    Dimensions dimensions;
    RECT rect;
    GetWindowRect(hwnd, &rect);
    dimensions.width = (rect.right - rect.left) - offset * 4;
    dimensions.height = (rect.bottom - rect.top) - offset * 8;
    return dimensions;
}