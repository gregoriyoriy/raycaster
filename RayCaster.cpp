#include <Windows.h>
#include <iostream>
#include <ctime>


RGBQUAD edgeColor = { 255,255,255,0 };
const wchar_t* szClassName = L"MainWinClass";
const int nWidth = 1440;
const int nHeight = 810;
MSG msg;

RGBQUAD FrameBuffer[nHeight][nWidth];

void PutPixel(int x, int y, RGBQUAD cl = edgeColor)
{
    FrameBuffer[y][x] = cl;
}

void PresentFrame(HDC hdc, void* pixels)
{
    HDC hMemDC = CreateCompatibleDC(hdc);
    HBITMAP hMemBitmap = CreateBitmap(nWidth, nHeight, 1, 8 * 4, pixels);
    HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, hMemBitmap);

    BitBlt(
        hdc,
        0, 0,
        nWidth, nHeight,
        hMemDC,
        0, 0,
        SRCCOPY
    );

    SelectObject(hMemDC, hOldBitmap);
    DeleteObject(hMemBitmap);
    DeleteObject(hMemDC);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;
    switch (message)
    {
    case WM_CREATE:
        SetTimer(hWnd, 1, 10, NULL);
        break;
    case WM_TIMER:
        InvalidateRect(hWnd, NULL, FALSE);
        break;
    case WM_PAINT:
    {
        hdc = BeginPaint(hWnd, &ps);
        int time = clock();

        //Graphics pipeline

        PutPixel(100, 100);

        PresentFrame(hdc, FrameBuffer);

        std::cout << "FPS: " << 1000 / (clock() - time) << std::endl;
        EndPaint(hWnd, &ps);
        break;
    }
    case WM_QUIT:
        PostQuitMessage(0);
        break;
    default:
        return (DefWindowProcW(hWnd, message, wParam, lParam));
        break;
    }
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpszCmdParam, int nCmdShow)
{
    WNDCLASS wc;
    memset(&wc, 0, sizeof(wc));
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = 0;
    wc.lpszMenuName = 0;
    wc.lpszClassName = TEXT("MyWindowClass");

    if (!RegisterClass(&wc))
    {
        MessageBox(NULL, TEXT("Window Registration Failed!"), TEXT("Error!"), MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    HWND hWnd = CreateWindowEx(0L, wc.lpszClassName, TEXT("MyWindowName"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, nWidth, nHeight, NULL, NULL, hInstance, NULL);
    if (hWnd == NULL)
    {
        MessageBox(NULL, TEXT("Window Creation Failed!"), TEXT("Error"), MB_OK);
        return 0;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    MSG msg;
    while (GetMessageW(&msg, 0, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    return msg.wParam;
}