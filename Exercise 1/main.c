#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

/* MY CHANGES */
#include <stdbool.h>

// Colors
int fish_color = RGB(0, 100, 255);
int fin_color = RGB(255, 90, 0);

// Mouth
bool mouth_open = false;
/* MY CHANGES END */

// Global variable
HINSTANCE hInst;
UINT  MessageCount = 0;
UINT  Count = 0;
int posX = 0;
int posY = 0;

// Function prototypes.
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);
void paintObject(HWND hWnd, HDC hDC, PAINTSTRUCT ps, int posX, int posY, POINT cursorPosition);
void paintPosition(HWND hWnd, HDC hDC, PAINTSTRUCT ps);

// Application entry point. This is the same as main() in standart C.
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg;
	BOOL bRet;
	WNDCLASS wcx;          // register class
	HWND hWnd;

	hInst = hInstance;     // Save the application-instance handle.
		// Fill in the window class structure with parameters that describe the main window.

	wcx.style = CS_HREDRAW | CS_VREDRAW;              // redraw if size changes
	wcx.lpfnWndProc = (WNDPROC)MainWndProc;          // points to window procedure
	wcx.cbClsExtra = 0;                               // no extra class memory
	wcx.cbWndExtra = 0;                               // no extra window memory
	wcx.hInstance = hInstance;                        // handle to instance
	wcx.hIcon = LoadIcon(NULL, IDI_APPLICATION);      // predefined app. icon
	wcx.hCursor = LoadCursor(NULL, IDC_ARROW);        // predefined arrow
	/* MY CHANGES */
	wcx.hbrBackground = (HBRUSH)(CreateSolidBrush(RGB(70, 140, 255)));
	/* MY CHANGES END */
	wcx.lpszMenuName = (LPCSTR)"MainMenu";          // name of menu resource
	wcx.lpszClassName = (LPCSTR)"MainWClass";        // name of window class

	// Register the window class.

	if (!RegisterClass(&wcx)) return FALSE;

	// create window of registered class

	hWnd = CreateWindow(
		"MainWClass",        // name of window class
		"ITU",               // title-bar string
		WS_OVERLAPPEDWINDOW, // top-level window
		200,                  // default horizontal position
		25,                 // default vertical position
		1000,                // default width
		700,                 // default height
		(HWND)NULL,         // no owner window
		(HMENU)NULL,        // use class menu
		hInstance,           // handle to application instance
		(LPVOID)NULL);      // no window-creation data
	if (!hWnd) return FALSE;

	// Show the window and send a WM_PAINT message to the window procedure.
	// Record the current cursor position.

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// loop of message processing
	while ((bRet = GetMessage(&msg, NULL, 0, 0)) != 0)
	{
		if (bRet == -1)
		{
			// handle the error and possibly exit
		}
		else
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return (int)msg.wParam;
}


LRESULT CALLBACK MainWndProc(
	HWND hWnd,        // handle to window
	UINT uMsg,        // message identifier
	WPARAM wParam,    // first message parameter
	LPARAM lParam)    // second message parameter
{
	HDC         hDC;
	PAINTSTRUCT ps;
	POINT cursorPosition;

	// init cursor position 
	GetCursorPos(&cursorPosition);
	ScreenToClient(hWnd, &cursorPosition);

	switch (uMsg)
	{
	case WM_CREATE:
		break;

		// character input 
	case WM_CHAR:
		switch (wParam) {
		case 0x08:  // backspace
		case 0x0A:  // linefeed
		case 0x1B:  // escape
			break;

		case 0x09:  // tab
			break;

		/* MY CHANGES */
		case 0x62: // B(lue fish)
			fish_color = RGB(0, 100, 255);
			fin_color = RGB(255, 90, 0);
			InvalidateRect(hWnd, NULL, TRUE);
			break;

		case 0x67: // G(reen fish)
			fish_color = RGB(0, 70, 0);
			fin_color = RGB(70, 0, 0);
			InvalidateRect(hWnd, NULL, TRUE);
			break;

		case 0x72: // R(ed fish)
			fish_color = RGB(150, 0, 0);
			fin_color = RGB(255, 220, 0);
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		/* MY CHANGES END */

		default:
			break;
		}
		break;

		// key input
	case WM_KEYDOWN:
		switch (wParam) {
			// update posX and posY in order to move object
		case VK_LEFT: // left arrow
			break;
		case VK_RIGHT: // right arrow
			break;
		case VK_UP: // up arrow
			break;
		case VK_DOWN: // down arrow
			break;

			// react on the other pressed keys 
		case VK_SPACE: // space
			break;
		case VK_BACK: // backspace
			break;
		case VK_TAB: // tab
			break;
			// more virtual codes can be found here: https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
		}
		break;

		// get cursor position 
	case WM_MOUSEMOVE:
		/* MY CHANGES */
		GetCursorPos(&cursorPosition);
		ScreenToClient(hWnd, &cursorPosition);
		InvalidateRect(hWnd, NULL, TRUE);
		break;

		// react on mouse clicks
	case WM_LBUTTONDOWN:
		mouth_open = true;
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_LBUTTONUP:
		mouth_open = false;
		InvalidateRect(hWnd, NULL, TRUE);
		/* MY CHANGES END */
		break;

		// paint objects
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		paintObject(hWnd, hDC, ps, posX, posY, cursorPosition);
		paintPosition(hWnd, hDC, ps);
		// paint other objects
		// paintObject2(hWnd, hDC, ps, posX, posY, cursorPosition);
		// paintObject3(hWnd, hDC, ps, posX, posY, cursorPosition);
		EndPaint(hWnd, &ps);
		DeleteDC(hDC);
		break;

		//
		// Process other messages.
		//

	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
}


void paintObject(HWND hWnd, HDC hDC, PAINTSTRUCT ps, int posX, int posY, POINT cursorPosition)
{
	// Paint rectangles, ellipses, polygons, lines etc.

	/* MY CHANGES */
	// Cursor position
	int x = cursorPosition.x;
	int y = cursorPosition.y;

	// Coloring
	SelectObject(hDC, GetStockObject(DC_BRUSH));

	// Outline
	HPEN pen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
	SelectObject(hDC, pen);

	// Dorsal fin
	SetDCBrushColor(hDC, fin_color);
	POINT dorsal_fin[] = {{x - 35,y - 60}, {x - 35,y}, {x + 5,y}};
	Polygon(hDC, dorsal_fin, 3);

	// Tail fin
	POINT tail_fin[] = {{x - 175,y - 25}, {x - 105,y}, {x - 175,y + 25}};
	Polygon(hDC, tail_fin, 3);

	// Fish body
	SetDCBrushColor(hDC, fish_color);
	POINT fish_body[] = {{x - 5,y - 25}, {x - 155,y}, {x - 5,y + 25}};
	Polygon(hDC, fish_body, 3);

	// Fish head
	Ellipse(hDC, x - 40, y - 25, x + 41, y + 26);

	// Pectoral fin
	SetDCBrushColor(hDC, fin_color);
	POINT pectoral_fin[] = {{x - 70,y - 10}, {x - 45,y}, {x - 70,y + 10}};
	Polygon(hDC, pectoral_fin, 3);

	// Eyeball
	SetDCBrushColor(hDC, RGB(255, 255, 255));
	Ellipse(hDC, x + 10, y - 25, x + 35, y - 5);

	// Pupil
	SetDCBrushColor(hDC, RGB(0, 0, 0));
	Ellipse(hDC, x + 25, y - 17, x + 35, y - 7);

	// Mouth
	if (mouth_open) // Open
	{
		Ellipse(hDC, x + 25, y + 10, x + 40, y);
	}
	else            // Closed
	{
		MoveToEx(hDC, x + 25, y + 5, NULL);
		LineTo(hDC, x + 40, y + 5);
	}
	
	// Pen cleanup
	DeleteObject(pen);
	/* MY CHANGES END */

	return;
}

void paintPosition(HWND hWnd, HDC hDC, PAINTSTRUCT ps)
{
	char        text[40];          // buffer to store an output text
	HFONT       font;              // new large font
	HFONT       oldFont;           // saves the previous font

	font = CreateFont(25, 0, 0, 0, 0, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, 0);
	oldFont = (HFONT)SelectObject(hDC, font);
	sprintf(text, "Position -- x:%d, y:%d", posX, posY);
	TextOut(hDC, 50, 600, text, (int)strlen(text));
	SelectObject(hDC, oldFont);
	DeleteObject(font);
}

