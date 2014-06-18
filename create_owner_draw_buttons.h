#include "stdafx.h"
#ifndef ___
#define ___
#include "Windows.h"
//

class CreateOwnerDrawButton
	{
	public:
	static void DrawButtons(const DRAWITEMSTRUCT *lpDrawItem,HBITMAP g_load,KDib & bmp)
		{
		HDC hCDC = ::CreateCompatibleDC(lpDrawItem->hDC);
      HBITMAP &hCaptureBitmapp = true ? g_load : g_load;
      HBITMAP hCaptureBitmappOld = (HBITMAP)::SelectObject(hCDC, hCaptureBitmapp);

      ::StretchBlt(
         lpDrawItem->hDC,
         0,0, lpDrawItem->rcItem.right - lpDrawItem->rcItem.left,  lpDrawItem->rcItem.bottom - lpDrawItem->rcItem.top,
         hCDC,
         0,0, bmp.SizeBitmap(hCaptureBitmapp).cx, bmp.SizeBitmap(hCaptureBitmapp).cy,
         SRCCOPY
      );

      ::SelectObject(hCDC, hCaptureBitmappOld);
      ::DeleteDC(hCDC);
	  }
	static void FOCUS(RECT  rect, HWND g_hWndBtn4) 
	{
			if (GetFocus() != g_hWndBtn4) {
				SetFocus(g_hWndBtn4);
				// Рисуем пунктирную рамку вызовом DrawFocusRect
				SetRect(&rect, 3, 3, 250 - 3,
					50 - 3);
				DrawFocusRect(GetDC(g_hWndBtn4), &rect);
			}
		}
	};

#endif 