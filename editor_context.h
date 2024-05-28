#ifndef EDITOR_CONTEXT_H
#define EDITOR_CONTEXT_H

#include <windows.h>

typedef struct dimensions {
    int width;
    int height;
} Dimensions;

typedef struct stateInfo {
    HWND textArea;
    char* text;
    WNDPROC originalProc;
    WNDPROC textareaProc;
} StateInfo;

Dimensions calculateDimensions(HWND hwnd, int offset);
#endif