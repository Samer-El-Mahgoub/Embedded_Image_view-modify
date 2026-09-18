#include <windows.h>
#include <stdio.h>

//Function to handle window messages
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            // 1. Load the bitmap from resources
            // The bitmap resource ID is 101, which we injected earlier using the image_modifier.c program.
            HBITMAP hBitmap = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(101));

            if (hBitmap != NULL) {
                // 2. Create a memory device context and select the bitmap into it
                HDC hdcMem = CreateCompatibleDC(hdc);
                SelectObject(hdcMem, hBitmap);

                // 3. Get the bitmap dimensions
                BITMAP bitmap;
                GetObject(hBitmap, sizeof(BITMAP), &bitmap);

                // 4.Display the bitmap on the window
                BitBlt(hdc, 0, 0, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCCOPY);

                // 5. Clean up
                DeleteDC(hdcMem);
                DeleteObject(hBitmap);
            } else {
                TextOut(hdc, 10, 10, "Failed to load image from resources!", 36);
            }

            EndPaint(hwnd, &ps);
            return 0;
        }
        case WM_DESTROY: {
            PostQuitMessage(0);
            return 0;
        }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

//Instead of using main, we use WinMain for Windows GUI applications
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    const char CLASS_NAME[] = "ImageWindowClass";

    // 1. Register the window class
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    // 2. Create the window
    HWND hwnd = CreateWindowEx(
        0, CLASS_NAME, "Embedded Image Viewer",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
        800, 600,
        NULL, NULL, hInstance, NULL
    );

    if (hwnd == NULL) return 0;

    // 3. Show the window
    ShowWindow(hwnd, nCmdShow);

    // 4. while loop to process messages
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}