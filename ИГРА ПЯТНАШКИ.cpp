// ИГРА ПЯТНАШКИ.cpp: определяет точку входа для приложения.
//

#include "stdafx.h"
#include "resource2.h"
#include "create_bitmap_for_bk.h"
#include "create_main_window.h"
#include <Windows.h>
#include <WindowsX.h>
#include <TChar.h>
#include "CommDlg.h"
#include <GdiPlus.h>
#include <objidl.h>
#include <vector>
#include "Desktop.h"
#include "barleyBreak.h"
#include "create_owner_draw_buttons.h"
#include <stdio.h>

#define WM_GAME_START 333
#define WM_SCREEN 322

char * img_name_for_bkg = "D:\фон.bmp" ;
char * img_name_for_bkg_before_start_game = "фон2.bmp";
LPWSTR img_name_for_standart_game = L"d:\backblue.gif";
Image * work_img;

barleyBreak game;
DWORD dwThreadId, dwThrdParam = 1;
HANDLE hThread;
BOOL is_highlights;
BOOL is_game_start = 0;
bool standart_game = 0;



BOOL is_l_key_pressed;
LPWSTR name_img;
KDib bmp,bkgr;
HWND hWndD;
WPARAM g_wp;
int x1,x2,y11,y2;
bool get_pos,get_pos_2 ;
bool is_load;
int g_nCmdShow;
HWND main;
HINSTANCE hIns;
OPENFILENAME ofn;
static char szFile[MAX_PATH];	
	// Переменные для стандартного диалога "Color"
	static CHOOSECOLOR cc;          // common dialog box structure 
	static COLORREF acrCustClr[16]; // array of custom colors 
HBITMAP g_new_game1 = NULL, g_new_game2 = NULL; // хендлы битмапы
HBITMAP g_load_game1 = NULL, g_load_game2 = NULL; 
HBITMAP g_new_s;
HBITMAP g_load;
HBITMAP g_enter_all;
HBITMAP g_resize;
HBITMAP g_exit1 = NULL, g_exit2 = NULL; 
HWND g_hWndBtn1 = NULL;
HWND g_hWndBtn2 = NULL;
HWND g_hWndBtn7 = 0;
HWND g_hWndBtn4 = NULL; // хендл кнопки с битмапой
HWND g_hWndBtn5 = 0;
HWND g_hWndBtn6 = 0;
bool isWindowMove;
bool button_push ;
bool button_draw_rectangle;
bool button_click;
	 RECT rect;
	 RECT rect_p;
	 int maxW  = rect_p.right - rect_p.left; 
	 int maxH = rect_p.bottom - rect_p.top;
	 POINT  Max = {maxW,maxH } ; 
     bool single_ = 0;


void change_style(HWND hWnd)
	{
	 long style = GetWindowLong(hWnd,GWL_STYLE);
	   style = style ^  WS_OVERLAPPEDWINDOW;	
	 SetWindowLong(hWnd,GWL_STYLE,style);
	}

BOOL CALLBACK DlgProc2(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {

	bool isScreen = false;
	GetClientRect(hWndD,&rect_p);	
	POINT cursorPos;
	MINMAXINFO *pInfo ;
	static int l_x,l_y,l_x2, l_y2;

	switch(msg)
		{		  
		  case WM_INITDIALOG:     
			  return TRUE;

		  case WM_LBUTTONDOWN: 
			  button_click = true;  
			  isWindowMove = true;			 
			  break;

	     case WM_GETMINMAXINFO: {  

            pInfo = (MINMAXINFO *)lp;  
		    POINT Min = { 50, 50 };
		    pInfo->ptMinTrackSize = Min;			
		    maxW  = rect_p.right - rect_p.left; 
	        maxH = rect_p.bottom - rect_p.top;	
          return 0;

		} 
		 case WM_COMMAND : 
				   
				   if(LOWORD(wp) == IDC_BUTTON1) {
				   DestroyWindow(hw);
				   InvalidateRect(hWndD,&rect,1);
				   rect_p = rect;
				   SendMessage(hWndD,WM_SCREEN,0,0);

				   }
			 break;
			   
		  case WM_LBUTTONUP:  button_click = false;
			  isWindowMove = false;
			  break;

			  //это для движения области выделения 
		  case WM_SIZE :  
			  GetClientRect(hw,&rect);
			  break;

		  case WM_MOUSEMOVE :  if(button_click && isWindowMove){
			  single_ = true;
			   GetCursorPos (&cursorPos);
			    ScreenToClient (hWndD, &cursorPos);
				GetWindowRect(hw,&rect);
			    MoveWindow(hw ,cursorPos.x - ((int)abs(rect.right - rect.left ) / 2),cursorPos.y ,
			   (int)abs(rect.right - rect.left ), (int)abs(rect.bottom - rect.top ), 0);

			  }

		break;

		  case WM_CLOSE: 

			  delete pInfo; 
			  break;  
		}
	return FALSE;
	}

void OnPaint_for_dialog(HWND h)
{
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(h,&ps);
	Graphics g(hdc);
	Image image(name_img);
	g.DrawImage(&image,0,0,image.GetWidth(),image.GetHeight());
	EndPaint(h,&ps);
}

void OnDestroy(HWND hWnd) {
 
//   ::DeleteObject(g_hCaptureBitmapp1);
 //  ::DeleteObject(g_hCaptureBitmapp2);
   ::PostQuitMessage(0);
}

BOOL CALLBACK DlgProc(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
	hWndD = hw;	
	POINT cursorPos;
	RECT rect;
	
	Graphics g(GetDC(hWndD));
	 Color color(0,0,0);
	Pen pen(color,1);
	
 switch (msg) {

 case WM_SCREEN :  
	 Desktop::CaptureScreen(rect_p.left,rect_p.top,rect_p.right-rect_p.left,rect_p.bottom-rect_p.top,L"d:\screen2.bmp"); 
	 break;

  case WM_INITDIALOG: 
    return TRUE;

	HANDLE_MSG(hw,WM_PAINT,OnPaint_for_dialog);

	    case WM_LBUTTONUP: 

			if(isWindowMove) isWindowMove = false;
			if(button_draw_rectangle){
		    button_push = false;
		    button_draw_rectangle = false;
			  MessageBox(0,(LPCWSTR)x1,L"",MB_OK);
			  MessageBox(0,(LPCWSTR)x2,L"",MB_OK);
		}
			
		break;

case WM_LBUTTONDOWN: 

	if(button_draw_rectangle){
		button_push = true;
		 GetCursorPos (&cursorPos);
              ScreenToClient (hWndD, &cursorPos);
			  x1 = cursorPos.x;
			  y11 = cursorPos.y;
		}
		 break;

case WM_MOUSEMOVE : // двигаем квадрат (область выделения)

	if(button_draw_rectangle)
	if(button_push){
		      GetCursorPos (&cursorPos);
              ScreenToClient (hWndD, &cursorPos);
			  x2 = cursorPos.x;
			  y2 = cursorPos.y;
	g.DrawRectangle(&pen,x1,y11,x2-x1,y2-y11);
	GetClientRect(hWndD,&rect);
	InvalidateRect(hWndD,&rect,true);

 }
	break;


  case WM_COMMAND: switch LOWORD(wp) {

                                    case IDOK : SendMessage(main,WM_GAME_START,wp,lp);
										break;

                                    case  IDCANCEL : 

	                                HWND hh = CreateDialog(hIns, MAKEINTRESOURCE(IDD_DIALOG2),hWndD,DlgProc2);
				                    SetWindowLong(hh ,GWL_EXSTYLE,WS_EX_LAYERED);				 
                                    GetWindowLong(hh, GWL_EXSTYLE | WS_EX_LAYERED);
	                                SetLayeredWindowAttributes(hh, 0, 80, LWA_ALPHA);	
	                                change_style(hh);
		                      		ShowWindow(hh,SW_SHOW);
			                    	SetWindowPos(hh,hWndD,50,50,100,100,SWP_NOMOVE);;

	                                }  
 break; 
	 
  case WM_CLOSE : DestroyWindow(hw); 
	  break;

 }

  return FALSE;
}

// обработчик сообщения WM_DRAWITEM
void OnDrawItem(HWND hWnd, const DRAWITEMSTRUCT *lpDrawItem) {
   
   if (lpDrawItem->hwndItem == g_hWndBtn2)
  CreateOwnerDrawButton::
		 DrawButtons(lpDrawItem,g_load_game2,bmp);

    if (lpDrawItem->hwndItem == g_hWndBtn1)
  CreateOwnerDrawButton::
		 DrawButtons(lpDrawItem,g_new_game1,bmp);

	if (lpDrawItem->hwndItem == g_hWndBtn7)
   
     CreateOwnerDrawButton::
		 DrawButtons(lpDrawItem,g_load,bmp);
    
    if (lpDrawItem->hwndItem == g_hWndBtn4)
   CreateOwnerDrawButton::
		 DrawButtons(lpDrawItem,g_exit2,bmp);

	 if (lpDrawItem->hwndItem == g_hWndBtn5)
   
     CreateOwnerDrawButton::
		 DrawButtons(lpDrawItem,g_new_s,bmp);
   
	 if (lpDrawItem->hwndItem == g_hWndBtn6)
   
     CreateOwnerDrawButton::
		 DrawButtons(lpDrawItem,g_load,bmp);
   
}

void OnClickButton2( HWND hWnd) {
   
  bool success = GetOpenFileName(&ofn);
 			if (success)
				MessageBox(hWnd, ofn.lpstrFile, L"Открывается файл:", MB_OK);
			else
				MessageBox(hWnd, L"GetOpenFileName",
				L"Отказ от выбора или ошибка", MB_ICONWARNING);	    
}

void OnClickButton1() {

   DestroyWindow(g_hWndBtn1);
   DestroyWindow(g_hWndBtn2);
   g_hWndBtn5 = ::CreateWindow(TEXT("button"), TEXT("пымпочка 2"), WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
      80, 100, 250, 50,main , (HMENU)NULL, hIns, NULL);
   g_hWndBtn6 = ::CreateWindow(TEXT("button"), TEXT("пымпочка 2"), WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
      80, 160, 250, 50, main , (HMENU)NULL, hIns, NULL);
}

void OnClickButton4(HWND hWnd ) {
  
     OnDestroy(hWnd);
   ::InvalidateRect(g_hWndBtn4, NULL, TRUE);
}

void OnClickButton5(HWND hWnd)
	{
	 bool success = GetOpenFileName(&ofn);
     ::InvalidateRect(g_hWndBtn2, NULL, TRUE);
			if (success){	 
			
			   name_img = ofn.lpstrFile;
			   Image image(name_img);
				 HWND hh = CreateDialog(hIns, MAKEINTRESOURCE(IDD_DIALOG1),hWnd,DlgProc);
				 ShowWindow(hh,SW_SHOW);
				 SetWindowPos(hh,hWnd,0,0,image.GetWidth(),image.GetHeight(),SWP_NOMOVE);
	HDC hDC;
	RECT rect;
	GetClientRect(hWnd,&rect);
	PAINTSTRUCT ps;

  hDC = BeginPaint(hh, &ps);
    Graphics s(hDC); 
	s.DrawImage(&image,0,0,image.GetWidth(),image.GetHeight());
	EndPaint(hh, &ps);
	KWnd::debug(hh);

}		
	}

void OnCommand(LPARAM lParam, int id, HWND hWndCtl, UINT codeNotify) {
  
   if (hWndCtl == g_hWndBtn2) {
	   
      if ((codeNotify == BN_CLICKED) || (codeNotify == BN_DOUBLECLICKED)) {
         OnClickButton2(main);
      }
	  
   }
   if (hWndCtl == g_hWndBtn1) {
      if ((codeNotify == BN_CLICKED) || (codeNotify == BN_DOUBLECLICKED)) {
         OnClickButton1();
      }
   }
   
   if (hWndCtl == g_hWndBtn4) {
      if ((codeNotify == BN_CLICKED) || (codeNotify == BN_DOUBLECLICKED)) {
         OnClickButton4(main);
      }
   }
    if (hWndCtl == g_hWndBtn5) {
      if ((codeNotify == BN_CLICKED) || (codeNotify == BN_DOUBLECLICKED)) {
         OnClickButton5(main);
      }
   }
	 if (hWndCtl == g_hWndBtn6) {
      if ((codeNotify == BN_CLICKED) || (codeNotify == BN_DOUBLECLICKED)) {
		 standart_game = true;
        SendMessage(main,WM_GAME_START,lParam,g_wp);
      }
   }
}

 //оконная процедура главного окна
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){	 

	POINT cursorPos;
	static bool isFileLoaded ;
	HDC hDC;
	static RECT rectangle;
	static int dX, dY;
	int i = 0,k=1,m=2,t=3;

   switch(msg){

	case WM_LBUTTONDOWN:

	  if(is_game_start) {	
		 GetCursorPos (&cursorPos);
         ScreenToClient (main, &cursorPos);  
		 
		 while(t < 65) {
		  if(    ( cursorPos.x >= game.coord[i] ) 		 
			  && ( cursorPos.x <= game.coord[m]) 		
			  && ( cursorPos.y >= game.coord[k]) 		
			  && (cursorPos.y <=  game.coord[t]) ){
				 game.step_logic_and_start_create_animation(t,main); 
				 break;		
			  }
		  i+=4;k+=4;m+=4;t+=4;
			 }		  
		  }
	  break;
		
case WM_SETCURSOR:  	

		if ((HWND)wParam == g_hWndBtn1) 
			CreateOwnerDrawButton::	FOCUS(rectangle,g_hWndBtn1);

		if ((HWND)wParam == g_hWndBtn2) 
			CreateOwnerDrawButton::	FOCUS(rectangle,g_hWndBtn2);

		if ((HWND)wParam == g_hWndBtn5) 
			CreateOwnerDrawButton::	FOCUS(rectangle,g_hWndBtn5);

		if ((HWND)wParam == g_hWndBtn6)
			CreateOwnerDrawButton::	FOCUS(rectangle,g_hWndBtn6);

		if ((HWND)wParam == g_hWndBtn7)
			CreateOwnerDrawButton::	FOCUS(rectangle,g_hWndBtn7);
			
		if ((HWND)wParam == g_hWndBtn4) 
		CreateOwnerDrawButton::	FOCUS(rectangle,g_hWndBtn4);

		break;

   case WM_GAME_START :

	   DestroyWindow(g_hWndBtn4);
	   DestroyWindow(g_hWndBtn5);
	   DestroyWindow(g_hWndBtn6);
	   DestroyWindow(hWndD);
	   change_style(hWnd);
	   is_game_start = true; 	  
	   break;

case WM_CREATE:

		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = hWnd;
		ofn.lpstrFile = (LPWSTR)szFile;
		ofn.nMaxFile = sizeof(szFile);
		hDC = GetDC(hWnd);

		isFileLoaded = bmp.LoadFromFile(img_name_for_bkg);
		if (!isFileLoaded) {
			MessageBox(hWnd,L"Файл  не загружен.",L"Error", MB_OK);
			MessageBox(hWnd, (LPCWSTR)bmp.GetError(), L"Error", MB_OK);
			break;
		}
		// Подогнать размеры окна программы под размер растра bmp
		GetClientRect(hWnd, &rectangle);
		dX = bmp.GetWidth() - rectangle.right;
		dY = bmp.GetHeight() - rectangle.bottom; 
		GetWindowRect(hWnd, &rectangle);
		InflateRect(&rectangle, dX/2, dY/2);
		MoveWindow(hWnd, rectangle.left, rectangle.top,
			rectangle.right-rectangle.left, rectangle.bottom-rectangle.top, TRUE);
		ReleaseDC(hWnd, hDC);

		break;
		
case WM_PAINT :

	 game.Draw(hWnd,is_game_start,
	 bmp,standart_game, img_name_for_standart_game,
	 name_img,bkgr,img_name_for_bkg_before_start_game,
	 work_img);

	 break;

   HANDLE_MSG(hWnd, WM_DRAWITEM  , OnDrawItem);
   HANDLE_MSG(hWnd, WM_DESTROY   , OnDestroy );
   HANDLE_MSG(lParam, WM_COMMAND   , OnCommand );
  
   }

   return DefWindowProc(hWnd, msg, wParam, lParam);
}

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpszCmdLine, int nCmdShow )
{
//CaptureScreen(200,200,500,500,L"d:\screen2.bmp");
 g_nCmdShow = nCmdShow;
GdiplusStartupInput gdiplusStartupInput; 
ULONG_PTR gdiplusToken; 
GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
   // создаю главное окно проги
   hIns = hInstance;

   KWnd mainWnd(L"Пятнашки", hInstance, nCmdShow, WndProc,NULL, 100, 100, 350, 400);
   SetWindowPos(mainWnd.GetHWnd(),(HWND)0,0,0,bmp.GetWidth(),bmp.GetHeight(),SWP_NOSIZE);
   // создаю кнопку для битмапы
   main = mainWnd.GetHWnd();
   g_hWndBtn1 = ::CreateWindow(TEXT("button"), TEXT("пымпочка 2"), WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
      80, 100, 250, 50, mainWnd.GetHWnd() , (HMENU)NULL, hInstance, NULL);

   g_hWndBtn2 = ::CreateWindow(TEXT("button"), TEXT("пымпочка 2"), WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
      80, 160, 250, 50, mainWnd.GetHWnd() , (HMENU)NULL, hInstance, NULL);

  
  
    g_hWndBtn4 = ::CreateWindow(TEXT("button"), TEXT("пымпочка 2"), WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
     80, 220, 250, 50, mainWnd.GetHWnd() , (HMENU)NULL, hInstance, NULL);

   g_new_game1  = (HBITMAP)::LoadImage(hInstance, TEXT("d:/начать новую игру.bmp"), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR | LR_DEFAULTSIZE | LR_LOADFROMFILE);
   g_new_game2  = (HBITMAP)::LoadImage(hInstance, TEXT("d:/начать новую игру2.bmp"), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR | LR_DEFAULTSIZE | LR_LOADFROMFILE);

   g_load_game1  = (HBITMAP)::LoadImage(hInstance, TEXT("d:/загрузить.bmp"), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR | LR_DEFAULTSIZE | LR_LOADFROMFILE);
   g_load_game2  = (HBITMAP)::LoadImage(hInstance, TEXT("d:/загрузить2.bmp"), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR | LR_DEFAULTSIZE | LR_LOADFROMFILE);
  
   g_exit1  = (HBITMAP)::LoadImage(hInstance, TEXT("d:/выход.bmp"), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR | LR_DEFAULTSIZE | LR_LOADFROMFILE);
   g_exit2  = (HBITMAP)::LoadImage(hInstance, TEXT("d:/выход2.bmp"), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR | LR_DEFAULTSIZE | LR_LOADFROMFILE);
   g_load =(HBITMAP)::LoadImage(hInstance, TEXT("d:/стандартная.bmp"), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR | LR_DEFAULTSIZE | LR_LOADFROMFILE);
g_new_s = (HBITMAP)::LoadImage(hInstance, TEXT("d:/загрузка картинки.bmp"), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR | LR_DEFAULTSIZE | LR_LOADFROMFILE);

   MSG msg;
   while (::GetMessage(&msg, NULL, 0, 0)){
      ::DispatchMessage(&msg);

   }


   delete work_img;
  

   GdiplusShutdown(gdiplusToken);
   return msg.wParam;
}