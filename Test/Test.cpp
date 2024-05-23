// Test.cpp : Defines the entry point for the application.
//


//#include "Test.h"
#include "pch.h"
#include "../RedBlackTree/RedBlackTree.h"

#define MAX_LOADSTRING 100

/** Global Variables */
HWND g_hInsertButton;
HWND g_hDeleteButton;
HWND g_hInsertEditC;
HWND g_hDeleteEditC;
int g_WindowWidth;
int g_WindowHeight;
int g_ButtonWidth = 45;
int g_ButtonHeight = 20;
int g_EditBoxWidth = 100;
int g_EditBoxHeight = g_ButtonHeight;
int g_InsertButtonPosX = 30;
int g_InsertButtonPosY = 20;
int g_DeleteButtonPosX = 30;
int g_DeleteButtonPosY = g_InsertButtonPosY + g_ButtonHeight + 10;
int g_InsertEditPosX = g_InsertButtonPosX + g_ButtonWidth + 5;
int g_InsertEditPosY = g_InsertButtonPosY;
int g_DeleteEditPosX = g_DeleteButtonPosX + g_ButtonWidth + 5;
int g_DeleteEditPosY = g_DeleteButtonPosY;

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_TEST, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TEST));

    MSG msg;

    /** Initialize the window. */
    HWND hWnd = ::GetActiveWindow();
	g_WindowWidth = 1200;
	g_WindowHeight = 800;
	RECT rect = { 0, 0, g_WindowWidth, g_WindowHeight };
	::AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);
	bool tof = ::SetWindowPos(hWnd, 0, 400, 400, g_WindowWidth, g_WindowHeight, 0);

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TEST));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_TEST);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
    {
        /** TODO: Adjust the positions of edit controls with screen width, height. */
		g_hInsertButton = CreateWindowW(
			L"BUTTON",  // Predefined class; Unicode assumed 
			L"Insert",      // Button text 
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
			g_InsertButtonPosX,         // x position 
			g_InsertButtonPosY,         // y position 
            g_ButtonWidth,        // Button width
            g_ButtonHeight,        // Button height
            hWnd,     // Parent window
			NULL,       // No menu.
			(HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
			NULL);
		g_hDeleteButton = CreateWindowW(
			L"BUTTON",  // Predefined class; Unicode assumed 
			L"Delete",      // Button text 
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
			g_DeleteButtonPosX,         // x position 
			g_DeleteButtonPosY,         // y position 
			g_ButtonWidth,        // Button width
			g_ButtonHeight,        // Button height
			hWnd,     // Parent window
			NULL,       // No menu.
			(HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
			NULL);
        g_hInsertEditC = CreateWindowW(L"edit", NULL, WS_BORDER | WS_CHILD | WS_VISIBLE | ES_NUMBER | ES_RIGHT,
            g_InsertEditPosX, g_InsertEditPosY, g_EditBoxWidth, g_EditBoxHeight, hWnd, NULL, hInst, NULL);
        g_hDeleteEditC = CreateWindowW(L"edit", NULL, WS_BORDER | WS_CHILD | WS_VISIBLE | ES_NUMBER | ES_RIGHT,
            g_DeleteEditPosX, g_DeleteEditPosY, g_EditBoxWidth, g_EditBoxHeight, hWnd, NULL, hInst, NULL);
        break;
    }
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            HPEN hBlackPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
            HPEN hRedPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));

            /** TODO: Node의 color에 맞춰 그리기 */
            SelectObject(hdc, hRedPen);
            Ellipse(hdc, 1, 1, 100, 100);
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
