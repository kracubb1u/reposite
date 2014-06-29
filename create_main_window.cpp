#include "Stdafx.h"
#include "create_main_window.h"

KWnd::KWnd(
	LPCTSTR windowName,
	HINSTANCE hInst, 
	int cmdShow,
	LRESULT (WINAPI *pWndProc)(HWND,UINT,WPARAM,LPARAM),
	LPCTSTR menuName,
	int x, 
	int y, 
	int width,
	int height,
	UINT classStyle, 
	DWORD windowStyle, 
	HWND hParent)
{
	LPCWSTR szClassName =L"KWndClass";

	wc.cbSize        = sizeof(wc);
	wc.style         = classStyle;
	wc.lpfnWndProc   = pWndProc;
	wc.cbClsExtra	 = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = hInst;
	wc.hIcon		 = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = 0;
	wc.lpszMenuName  = menuName;
	wc.lpszClassName = szClassName;
	wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

	// Регистрируем класс окна
	if (!RegisterClassEx(&wc)) {
		LPCWSTR msg = L"Cannot register class: ";
		
		MessageBox(NULL, msg, L"Error", MB_OK);
		return;
	}
	
	// Создаем окно
	hWnd = CreateWindow(szClassName, windowName, windowStyle,
		x, y, width, height, hParent, (HMENU)NULL, hInst, NULL);       
	
	if (!hWnd) {
		LPCWSTR text = L"Cannot create window: ";
	
		MessageBox(NULL, text, L"Error", MB_OK);
		return;
	}

	// Показываем  окно
	ShowWindow(hWnd, cmdShow); 
}

void KWnd::debug(HWND hh)
	{
	if (!hh) {
	static LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR) &lpMsgBuf,
		0,
		NULL );

	MessageBox(0, (LPCWSTR)lpMsgBuf, L"Error", MB_ICONEXCLAMATION | MB_OK);
	LocalFree( lpMsgBuf );
}
 }	