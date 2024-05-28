#ifndef MAIN_H
#define MAIN_H
#include "editor_context.h"

const int TEXTAREA_OFFSET = 10;
void messageLoop();
LRESULT CALLBACK windowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HWND createMainWindow(HINSTANCE hInstance, StateInfo *state);
HWND repaintMainWindow(HWND hwnd, HDC *hdc, PAINTSTRUCT *ps);
void initWindow(HWND hwnd, StateInfo* state);
void draw(HWND hwnd, HDC *hdc, PAINTSTRUCT *ps);
void createTextArea(HWND hwnd, StateInfo* state);
void resizeTextArea(HWND hwnd);
StateInfo* getStateInfoFromLParam(LPARAM lParam);
StateInfo* getAppState(HWND hwnd);
void setAppState(HWND hwnd, StateInfo* state);

#endif 