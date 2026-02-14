#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdint.h>
#include "graphics.h"
#include "window.h"

static BITMAPINFO bmi  = { sizeof(BITMAPINFOHEADER), FB_WIDTH, -FB_HEIGHT, 1, 32 };
static HWND g_hwnd;
static HDC g_hdc;

void window_draw() {
    MSG msg;
	memset(&msg, 0, sizeof(msg));

	while (PeekMessageA(&msg, 0, 0, 0, PM_REMOVE) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessageA(&msg);
	}

    RECT rect;
    GetClientRect(g_hwnd, &rect);

    int win_w = rect.right - rect.left;
    int win_h = rect.bottom - rect.top;

    float scale_x = (float)win_w / FB_WIDTH;
    float scale_y = (float)win_h / FB_HEIGHT;
    float scale = scale_x < scale_y ? scale_x : scale_y;

    int draw_w = (int)(FB_WIDTH  * scale);
    int draw_h = (int)(FB_HEIGHT * scale);

    int offset_x = (win_w - draw_w) / 2;
    int offset_y = (win_h - draw_h) / 2;

    HBRUSH brush = (HBRUSH)GetStockObject(BLACK_BRUSH);
    FillRect(g_hdc, &rect, brush);

    SetStretchBltMode(g_hdc, COLORONCOLOR);

    StretchDIBits(
        g_hdc,
        offset_x, offset_y,
        draw_w, draw_h,
        0, 0,
        FB_WIDTH, FB_HEIGHT,
        g_framebuffer,
        &bmi,
        DIB_RGB_COLORS,
        SRCCOPY
    );

    SwapBuffers(g_hdc);
}

long window_time() {
    return GetTickCount();
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
        case WM_CLOSE:
            DestroyWindow(hwnd);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            ExitProcess(0);
            break;
        default:
            return DefWindowProcA(hwnd, msg, wParam, lParam);
	}
	return 0;
}

int window_init() {
    const char windowClassName[] = "InscryptionWindowClass";
	const char windowTitle[] = "Inscryption";
	WNDCLASSEXA wc;

	wc.cbSize = sizeof(WNDCLASSEXA);
	wc.style = 0;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = 0;
	wc.hIcon = 0;
	wc.hCursor = 0;
	wc.hbrBackground = 0;
	wc.lpszMenuName = 0;
	wc.lpszClassName = windowClassName;
	wc.hIconSm = 0;

	if (!RegisterClassExA(&wc)) {
		return -1;
	}

	RECT rect;
	rect.left = rect.top = 0;
	rect.right = FB_WIDTH;
	rect.bottom = FB_HEIGHT;
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	HWND hwnd = CreateWindowExA(0, windowClassName, windowTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, rect.right - rect.left, rect.bottom - rect.top, 0, 0, 0, 0);
    
    if (!hwnd) {
        return -1;
    }

    g_hwnd = hwnd;
    g_hdc = GetDC(hwnd);
    ShowWindow(hwnd, SW_SHOW);

    return 0;
}