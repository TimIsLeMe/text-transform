#include <stdio.h>
#include <unistd.h>
#include <windows.h>
#include "main.h"
#include "editor_context.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASS wc = {0};
    wc.lpfnWndProc = windowProc;       // Pointer to the window procedure
    wc.hInstance = hInstance;          // Handle to the application instance
    wc.lpszClassName = "MyWindowClass";// Name of the window class
    RegisterClass(&wc);
    StateInfo* state = malloc(sizeof(StateInfo));
    state->text = "";
    HWND hwnd = createMainWindow(hInstance, state);
    setAppState(hwnd, state);
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);
    messageLoop();
    free(state);
    return 0;
}

void messageLoop() {
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

LRESULT CALLBACK windowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    StateInfo* state = getStateInfoFromLParam(lParam);
    switch (uMsg) {
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            draw(hwnd, &hdc, &ps);
            EndPaint(hwnd, &ps);
            break;
        }
        case WM_CREATE: {
            initWindow(hwnd, state);
            break;
        }
        case WM_SIZE: {
            resizeTextArea(hwnd);
            break;
        }
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    state->originalProc = DefWindowProc(hwnd, uMsg, wParam, lParam);
    return state->originalProc;
}

HWND createMainWindow(HINSTANCE hInstance, StateInfo *state) {
    return CreateWindow(
        "MyWindowClass",               // Window class name
        "Text Transform (TT)",         // Window title
        WS_OVERLAPPEDWINDOW,           // Window style
        CW_USEDEFAULT, CW_USEDEFAULT,  // Position
        CW_USEDEFAULT, CW_USEDEFAULT,  // Size
        NULL,                          // Parent window
        NULL,                          // Menu
        hInstance,                     // Application instance handle
        state                       // Additional application data
    );
}

HWND repaintMainWindow(HWND hwnd, HDC *hdc, PAINTSTRUCT *ps) {
    LPCRECT rectPointer = &(ps->rcPaint);
    FillRect(*hdc, rectPointer, (HBRUSH) (COLOR_WINDOW+1));
}

void initWindow(HWND hwnd, StateInfo *state) {
    createTextArea(hwnd, state);
}

void draw(HWND hwnd, HDC *hdc, PAINTSTRUCT *ps) {
    repaintMainWindow(hwnd, hdc, ps);
    return;
}

LRESULT CALLBACK textAreaProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    StateInfo* state = getAppState(hwnd);
    switch (uMsg) {
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            draw(hwnd, &hdc, &ps);
            EndPaint(hwnd, &ps);
            break;
        }
        case WM_SETTEXT: {
            state->text = (char*) wParam;
            break;
        }
        case WM_MBUTTONUP: {
            printf("text: %", state->text);
        }
        default: {
            return CallWindowProc(state->originalProc, hwnd, uMsg, wParam, lParam);
        }
    }
    printf("callwindowproc\n");
    return CallWindowProc(state->textareaProc, hwnd, uMsg, wParam, lParam);
}

void createTextArea(HWND hwnd, StateInfo *state) {
    Dimensions dimensions = calculateDimensions(hwnd, TEXTAREA_OFFSET);
    state->textArea = CreateWindowEx(0,
        "EDIT",                   // Predefined class; Unicode assumed
        state->text,              // Window Content (Title)
        WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL, // Styles
        TEXTAREA_OFFSET, TEXTAREA_OFFSET,                   // Position
        dimensions.width, dimensions.height,                // Size
        hwnd,                     // Parent window
        NULL,                     // No menu
        GetModuleHandle(NULL), // Handle Instance
        NULL                    //data
    );
    printf("creattingTextArea\n");
    state->textareaProc = (WNDPROC) SetWindowLongPtr(state->textArea, GWLP_WNDPROC, (LONG_PTR)textAreaProc);
    printf("createdProc\n");
}

void resizeTextArea(HWND hwnd) {
    StateInfo* state = getAppState(hwnd);
    if (state && state->textArea) {
        Dimensions dimensions = calculateDimensions(hwnd, TEXTAREA_OFFSET);
        MoveWindow(state->textArea, 
        TEXTAREA_OFFSET, TEXTAREA_OFFSET,
        dimensions.width, dimensions.height,
        TRUE);
        InvalidateRect(state->textArea, NULL, TRUE);
    }
}

StateInfo* getStateInfoFromLParam(LPARAM lParam) {
    CREATESTRUCT *pCreate = (CREATESTRUCT*) lParam;
    return (StateInfo*) pCreate->lpCreateParams;
}

StateInfo* getAppState(HWND hwnd) {
    LONG_PTR ptr = GetWindowLongPtr(hwnd, GWLP_USERDATA);
    return (StateInfo*) ptr;
}

void setAppState(HWND hwnd, StateInfo *state) {
    SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR) state);
}