// ИГРА ПЯТНАШКИ.cpp: определяет точку входа для приложения.
//

#include "stdafx.h"
#include "create_bitmap_for_bk.h"
#include "create_main_window.h"
#include "Desktop.h"
#include "barleyBreak.h"
#include "create_owner_draw_buttons.h"
#include "ToolsWindow.h"

                                                                                                                                                                                                                                                      bool isPushOK = true;bool isExist = false;bool isOneCheck = false;bool isTwoCheck = false;int _top,_left,_right,_bottom;bool Exist = false;MINMAXINFO *pInfo ;bool isStartGameWithPosImage = false;RECT imgRect,rect,rect_p,rectn;bool isStartStandartGame = false;HWND main,hWndD,StartStandartGame,choiceOfimage,g_hWndBtn4,pause,g_hWndBtn7,newGame,buttonLoadGame,save,exit_,hsb,shuffle,screen,hh,hh1;;bool isSartStandartGame = false;bool is_game_start = false;char * img_name_for_bkg = "D:\фон.bmp" ;char * img_name_for_bkg_before_start_game = "D:\фон2.bmp";LPWSTR img_name_for_standart_game = L"D:/1259780126_15.jpg";bool isPause=false,isGameStart=false;long time_ = 0;bool standart_game = 0;LPWSTR name_img;WPARAM g_wp;int g_nCmdShow;HINSTANCE hIns;OPENFILENAME ofn;static char szFile[MAX_PATH];static CHOOSECOLOR cc;static COLORREF acrCustClr[16];bool singleton = true;bool ButtonPush; bool ButtonIsUp = false;


std::vector<HBITMAP>gvb;
Bitmap * work_img;
barleyBreak game;
KDib bmp,bkgr;


//отрисовка для диалогового окна
void OnPaint_for_dialog(HWND h)
{

    Color color(255,0,0); 
    Pen pen(color,5); 
    Image image(name_img);
    HDC hdc = GetDC(h);
	Graphics gg(hdc);	

	//Контроль за тем, чтобы диалоговое окно было не больше размера экрана пользователя.
	static int max_x = GetSystemMetrics(SM_CXSCREEN);
    static int max_y = GetSystemMetrics(SM_CYSCREEN);
	 Bitmap buffer( max_x, max_y,&gg);
	 Graphics g(&buffer);
  //сначала рисую в буфер
	 //потом из буфера на экран
	 // чтобы не мерцало
	gg.DrawImage(
		&image,
		0,
		0,
		(image.GetWidth() > max_x) 
					 ? max_x  
					 : image.GetWidth()
					 ,(image.GetHeight() > max_y)
					 ? max_y - 50 
					 : image.GetHeight()
            );

	//тянуть прямоугольник пока левая кнопка не отжата
	if(ButtonPush || ButtonIsUp ) gg.DrawRectangle(
		&pen,
		_left, // координаты инициализирую в оконной процедуре диалогового окна
		_top,
		_right - _left ,
		_bottom - _top
		); 

	g.DrawImage(&buffer,0,0);
	DeleteDC(hdc);	
}



void OnDestroy(HWND hWnd) {
 
	BOOST_FOREACH( HBITMAP  &i, gvb ) 
		{
	//бегаю по вектору освобождаю память 
		::DeleteObject(i);
		

		}
	
	delete work_img;
	free(pInfo);
	

	if(isPause)
	KillTimer(main,-300);

   ::PostQuitMessage(0);

}




BOOL CALLBACK DlgProc(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
	hWndD = hw;	
	POINT cursorPos;
	RECT rect;
	

 switch (msg) {

case 9890: /* кнопка ок на окне tools */ 

	if(!(_right == _left || _bottom == _top)){
	InvalidateRect(hw,&rect,1);		
	isStartGameWithPosImage = true;
	DestroyWindow(hw);
	ShowWindow(main,SW_SHOW);
	//сохраняем координаты выбранной области и отправляем их на прорисовку
	rectn.bottom = _bottom;
	rectn.left = _left;
	rectn.right = _right;
	rectn.top = _top;  
	
	SendMessage(main,WM_GAME_START,0,0);	
}
	else{
		CString s; s.Format(L" %d %d %d %d",_left,_right,_top,_bottom);
		MessageBox(0,L"Область не выбрана",0,0);
		}
	break;

	
case 9897: /* кнопка выбрать область */ 
	_top = 0;
	_bottom = 0;
	_left = 0;
	_right = 0;
	 ButtonIsUp = false;
	 ButtonPush = false;	
	 GetClientRect(hw,&rect);
	 InvalidateRect(hw,&rect,1);
	break;

	
case WM_INITDIALOG: 
	  GetClientRect(hh1,&rect);
	  ToolsWindow::CreateToolsWindow(hIns,hw,rect);
	  ButtonPush =  ! true;
    return TRUE;


HANDLE_MSG(hw,WM_PAINT,OnPaint_for_dialog);

case WM_LBUTTONUP: 
			ButtonIsUp = true;
			ButtonPush = false;
		break;

case WM_MOUSEMOVE : 

			if(ButtonPush){
			GetCursorPos (&cursorPos);
              ScreenToClient (hWndD, &cursorPos);
			  _right = cursorPos.x;
			  _bottom = cursorPos.y;
			  GetClientRect(hw,&rect);			
			InvalidateRect(hw,&rect,1);
				}

			break;


case WM_LBUTTONDOWN: 
	 
	if(!ButtonPush){
		ButtonIsUp = false;
	ButtonPush = true;
		 GetCursorPos (&cursorPos);
              ScreenToClient (hWndD, &cursorPos);
			  _left = cursorPos.x;
			  _top = cursorPos.y;

		}
			
	 break;

  case WM_CLOSE : 

	  DestroyWindow(hw); 
	  SendMessage(main,WM_CLOSE,0,0);
	  break;

   case WM_ERASEBKGND:
        return TRUE;
 }

  return FALSE;
}

// обработчик сообщения WM_DRAWITEM
void OnDrawItem(HWND hWnd, const DRAWITEMSTRUCT *lpDrawItem) {
   
   if (lpDrawItem->hwndItem == buttonLoadGame)
  CreateOwnerDrawButton::
		 DrawButtons(lpDrawItem,gvb[3],bmp);

    if (lpDrawItem->hwndItem == newGame)
  CreateOwnerDrawButton::
		 DrawButtons(lpDrawItem,gvb[0],bmp);
    
    if (lpDrawItem->hwndItem == g_hWndBtn4)
   CreateOwnerDrawButton::
		 DrawButtons(lpDrawItem,gvb[5],bmp);

	 if (lpDrawItem->hwndItem == choiceOfimage)
   
     CreateOwnerDrawButton::
		 DrawButtons(lpDrawItem,gvb[7],bmp);
   
	 if (lpDrawItem->hwndItem == StartStandartGame)
   
     CreateOwnerDrawButton::
		 DrawButtons(lpDrawItem,gvb[6],bmp);
   
}


void OnClickStartStandartGame( HWND hWnd) 
{
   
//загрузка игры из файла
	if(game.LoadGame(main) )
		{
		 
	      isStartStandartGame = true;
	      name_img =  img_name_for_standart_game;
	      SendMessage(main,WM_GAME_START,0,0);

		}
	
}

void OnClickNewGame() {

   DestroyWindow(newGame);
   DestroyWindow(buttonLoadGame);

   choiceOfimage = ::CreateWindow(
	   TEXT("button"),
	   TEXT("пымпочка 2"),
	   WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
      80, 
	  100,
	  250, 
	  50,
	  main ,
	  (HMENU)NULL,
	  hIns, 
	  NULL
	  );

      StartStandartGame = ::CreateWindow(
	   TEXT("button"),
	   TEXT("пымпочка 2"), 
	   WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
      80, 
	  160, 
	  250, 
	  50, 
	  main , 
	  (HMENU)NULL,
	  hIns, 
	  NULL
	  );

}

void OnClickButton4(HWND hWnd ) {  
     OnDestroy(hWnd);  
}


//кнопка выбора изображения
void OnClickButton5(HWND hWnd)
	{

	//открыть диалог выбора изображения
	 bool success = GetOpenFileName(&ofn);
     ::InvalidateRect(buttonLoadGame, NULL, TRUE);

			if (success){		
		
/* с помощью регулярных выражений, нужно проверить, что выбрана картинка, а не что-то другое . */
/**********************************************************************************************/
 name_img = ofn.lpstrFile; // получить путь к выбранному файлу

//регулярка читается так : "что угодно . без чувствительности к регистру любой формат из списка	"   
boost::regex xRegEx(".*?.(?i)(bmp|gif|png|jpg)");
boost::smatch xResults;

//перевести юникод строку в анси
std::wstring s(name_img);
std::string dest;
std::string tmp;
tmp.resize(s.size());
std::transform(s.begin(),s.end(),tmp.begin(),wctob);
tmp.swap(dest);

if(boost::regex_match(dest,xResults,xRegEx)) {

			   Image image(name_img);
			   if((image.GetWidth() >= 200  && image.GetHeight() >= 200  ) ){

				  int max_x = GetSystemMetrics(SM_CXSCREEN);
                  int max_y = GetSystemMetrics(SM_CYSCREEN);

			 
				 hh1 = CreateDialog(
					 hIns,
					 MAKEINTRESOURCE(IDD_DIALOG3),
					 0,
					 DlgProc);

				 ShowWindow(hh1,SW_SHOW);
				 ShowWindow(main,SW_HIDE);

				 //проверить, чтобы изображение было не больше размера экрана - иначе сжать
				 SetWindowPos(
					 hh1,
					 0,
					 0,
					 0,
					 (image.GetWidth() > max_x) 
					 ? max_x  
					 : image.GetWidth()
					 ,(image.GetHeight() > max_y)
					 ? max_y - 50 
					 : image.GetHeight()
					 ,SWP_NOMOVE);


	CString s = L"Нажмите на определенную область на изображении левой кнопкой мыши и ведите до конечной точки "; 
	int result = MessageBox(0,s,L"Выбор области изображения",MB_ICONQUESTION | 0);
				

	HDC hDC = GetDC(hh1);
	RECT rect;
	GetClientRect(hh1,&rect);
	
	//зарисовать в диалоге выбранное изображение
    
    Graphics sh(hDC); 

	sh.DrawImage(&image,0,0,(image.GetWidth() > max_x) 
					 ? max_x  
					 : image.GetWidth()
					 ,(image.GetHeight() > max_y)
					 ? max_y - 50 
					 : image.GetHeight()
					 );
	DeleteDC(hDC);
	
	KWnd::debug(hh1);
				   }
			   else MessageBox(0,L"Изображние слишком маленькое",0,0);
	}	
		else MessageBox(0,L"Вы выбрали не картинку",0,0);		
				}
			
	}



 
//сюда приходят сообшения от кнопок
void OnCommand(LPARAM lParam, int id, HWND hWndCtl, UINT codeNotify) {
  


   if (hWndCtl == shuffle) {
	   
      if ((codeNotify == BN_CLICKED) || (codeNotify == BN_DOUBLECLICKED)) {
         game.shuffle(main);
      }
	  
   }

    if (hWndCtl == pause) {
	   
      if ((codeNotify == BN_CLICKED) || (codeNotify == BN_DOUBLECLICKED)) {
		  if(isPause){
        SetTimer(main,-300,1000,0);
	    isPause = false;
	     SetWindowText(pause,L"Пауза");
		 isPushOK = true;
			  }

		  else{
	   SetWindowText(pause,L"Продолжить ");
	   KillTimer(main,-300);
	   isPause = true;
	   isPushOK = false;
			  }
      }
	  
   }
	 if (hWndCtl == save) {
	   
		 //асинхронное сохранения с мьютексом и печеньками
      if ((codeNotify == BN_CLICKED) || (codeNotify == BN_DOUBLECLICKED)) {
       
	      if(!isExist){/// для того, чтобы недопустить создания милиона потоков. 
					  //Пул потоков для такой легкой задачи - делать смысла не вижу
		  boost::thread t(

			  boost::bind(
			  &barleyBreak::Save,
			  &game,time_,
			  isExist,
			  work_img
			  )

			);

		  t.detach();
			  }
      }
	  
   }
	  if (hWndCtl == exit_) {
	   
		  
      if ((codeNotify == BN_CLICKED) || (codeNotify == BN_DOUBLECLICKED)) {
		 
         OnDestroy(main);
      }
	  
   }
	   if (hWndCtl == screen) {
	   
		   //асинхронный скриншот
      if ((codeNotify == BN_CLICKED) || (codeNotify == BN_DOUBLECLICKED)) {  
		  
		  RECT rect_p;

		  GetClientRect(
			  main,
			  &rect_p
			  );
		          if(!Exist){ /// для того, чтобы недопустить создания милиона потоков. 
					  //Пул потоков для такой легкой задачи - делать смысла не вижу
		  		  boost::thread t(
					  Desktop:: CaptureScreen,
					  rect_p.left,
					  rect_p.top,
					  rect_p.right-rect_p.left + 10,
					  rect_p.bottom-rect_p.top + 10,
					  Exist
                  );

		  t.detach();
		 }
      }
	  
   }
	    if (hWndCtl == buttonLoadGame) {
	   
      if ((codeNotify == BN_CLICKED) || (codeNotify == BN_DOUBLECLICKED)) {
         OnClickStartStandartGame(main);
      }
	  
   }
   if (hWndCtl == newGame) {
      if ((codeNotify == BN_CLICKED) || (codeNotify == BN_DOUBLECLICKED)) {
         OnClickNewGame();
      }
   }
   
   if (hWndCtl == g_hWndBtn4) {
      if ((codeNotify == BN_CLICKED) || (codeNotify == BN_DOUBLECLICKED)) {
         OnClickButton4(main);
      }
   }
    if (hWndCtl == choiceOfimage) { 
      if ((codeNotify == BN_CLICKED) || (codeNotify == BN_DOUBLECLICKED)) {
         OnClickButton5(main);
      }
   }
	 if (hWndCtl == StartStandartGame) { //standart game
      if ((codeNotify == BN_CLICKED) || (codeNotify == BN_DOUBLECLICKED)) {
		 
		name_img =  img_name_for_standart_game;
		standart_game = true;
        SendMessage(main,WM_GAME_START,lParam,g_wp);
		
      }
   }
}

 //оконная процедура главного окна
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){	 
 	wstring inf;
	POINT cursorPos;
	CString d;
	static int ptWidth[3] = { 110, 300};
	static bool isFileLoaded ;
	HDC hDC;
		
	static RECT rectangle;
	static int dX, dY;
	int i = 0,k=1,m=2,t=3;
	RECT p;

   switch(msg){

	case WM_LBUTTONDOWN:
	/*for(int i = 1; i < game.figure.size(); i++){
	inf += boost::lexical_cast<wstring>(game.figure[i].X) + L" ";
	inf += boost::lexical_cast<wstring>(game.figure[i].Y) + L" ";;
	inf += boost::lexical_cast<wstring>(game.figure[i].j) + L" ";;
	inf += boost::lexical_cast<wstring>(game.figure[i].i) + L" ";
	inf += '\n';
	}

MessageBox(0,inf.c_str(),0,0);*/
	
		if(isPushOK)
	  if(is_game_start) {	
		 GetCursorPos (&cursorPos);
         ScreenToClient (main, &cursorPos);  
		
		 //вот самое интересное
		 //каждая пятнашка это посути самодельная кнопка 
		 // вообшем как это все работает
		 // после сообшения  WM_LBUTTONDOWN
		 //я читаю координаты нажатия мыши
		 //и сравниваю их на прнадлежность к массиву, в котором сохранены координаты
		 //моих пятнашек если все ок -
		 // отправляю сигнал на создание анимации
		 
		 while(t < 65) {
		  if(    ( cursorPos.x >= game.coord[i] ) 		 
			  && ( cursorPos.x <= game.coord[m]) 		
			  && ( cursorPos.y >= game.coord[k]) 		
			  && (cursorPos.y <=  game.coord[t]) ){ 
				 game.step_logic(t,main,hsb); 
				 break;		
			  }
		  i+=4;k+=4;m+=4;t+=4;
			 }		  
		  }
	  break;


case WM_SETCURSOR:  	
	/* можно не читать - это прото фокус для кнопок (начать новую игру и т.д */
		if ((HWND)wParam == newGame) 
			CreateOwnerDrawButton::	FOCUS(rectangle,newGame);

		if ((HWND)wParam == buttonLoadGame) 
			CreateOwnerDrawButton::	FOCUS(rectangle,buttonLoadGame);

		if ((HWND)wParam == choiceOfimage) 
			CreateOwnerDrawButton::	FOCUS(rectangle,choiceOfimage);

		if ((HWND)wParam == StartStandartGame)
			CreateOwnerDrawButton::	FOCUS(rectangle,StartStandartGame);

		if ((HWND)wParam == g_hWndBtn7)
			CreateOwnerDrawButton::	FOCUS(rectangle,g_hWndBtn7);
			
		if ((HWND)wParam == g_hWndBtn4) 
		CreateOwnerDrawButton::	FOCUS(rectangle,g_hWndBtn4);

		break;

   case WM_GAME_START :

	    is_game_start = true; 
		if(!isStartStandartGame){
	   DestroyWindow(g_hWndBtn4);
	   DestroyWindow(choiceOfimage);
	   DestroyWindow(StartStandartGame);
	   DestroyWindow(hWndD);

			} 

		else 			if(!isStartGameWithPosImage){
				DestroyWindow(g_hWndBtn4);
				DestroyWindow(buttonLoadGame);
				DestroyWindow(newGame);
				}
			

	  Desktop:: change_style(hWnd);

		 GetClientRect(hWnd, &rectangle);	

MoveWindow(hWnd, rectangle.left, rectangle.top,
			rectangle.right-rectangle.left +150, rectangle.bottom-rectangle.top +60, 1);

		   GetClientRect(main,&rect); 

		shuffle = CreateWindow(TEXT("button"), TEXT("Перемешать"), WS_CHILD | WS_VISIBLE ,
      rect.right - 120 , 60, 111, 35, main , (HMENU)NULL, hIns ,NULL);

		save = CreateWindow(TEXT("button"), TEXT("Сохранить игру"), WS_CHILD | WS_VISIBLE ,
     rect.right - 120 , 100, 111, 35, main, (HMENU)NULL, hIns ,NULL);

		pause = CreateWindow(TEXT("button"), TEXT("Пауза"), WS_CHILD | WS_VISIBLE ,
     rect.right - 120, 140, 111, 35, main , (HMENU)NULL, hIns ,NULL);

		screen = CreateWindow(TEXT("button"), TEXT("Cкриншот"), WS_CHILD | WS_VISIBLE ,
      rect.right - 120 , 180, 111, 35, main , (HMENU)NULL, hIns ,NULL);

		exit_= CreateWindow(TEXT("button"), TEXT("Выход"), WS_CHILD | WS_VISIBLE ,
      rect.right - 120 , 220, 111, 35, main , (HMENU)NULL, hIns ,NULL);

		 hsb = CreateStatusWindow(WS_CHILD | WS_VISIBLE | SBARS_SIZEGRIP, _T("Welcome to SpyWindows"), main, 9000);
		 SetTimer(main,-300, 1000, NULL);

		 SendMessage(hsb, SB_SETPARTS,2,(LPARAM)ptWidth);

	   InvalidateRect(main,&rect,true);
	   break;

  
	


	   case WM_SIZE:

            SendMessage(hsb, WM_SIZE, 0, 0);
			 SendMessage(hsb, SB_SETPARTS, 2, (LPARAM)ptWidth);	 
 
	   case WM_SIZING :    
/* изменяю размер доски */
	GetClientRect(main,&rect);
	game.set_status(false);	
	game.Resize(rect);
	//смешаю кнопки
	SetWindowPos(shuffle,0,rect.right - 120 , 60, 111, 35,0);
		SetWindowPos(save,0,rect.right - 120 , 100, 111, 35,0);
		SetWindowPos(pause,0,rect.right - 120 , 140, 111, 35,0);
		SetWindowPos(screen,0,rect.right - 120 , 180, 111, 35,0);
		SetWindowPos(exit_,0,rect.right - 120 , 220, 111, 35,0);
	

	break;

   case WM_TIMER : 
//эта штука выводит время игры в статус бар
	   time_++; 
	   game.SetTime(time_);
	   d.Format(L"Вы играете: %d мин.",time_/ 60 );
       SendMessage(hsb, SB_SETTEXT ,1,(LPARAM)(LPCWSTR)d);
      

 break;

case WM_CREATE:
	
	{
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = hWnd;
		ofn.lpstrFile = (LPWSTR)szFile;
		ofn.nMaxFile = sizeof(szFile);
		hDC = GetDC(hWnd);
		RECT rect;
		GetClientRect(0,&rect);
		static int max_x = GetSystemMetrics(SM_CXSCREEN);
        static int max_y = GetSystemMetrics(SM_CYSCREEN);
		 MoveWindow(hWnd, ( max_x) / 2 - 200 , max_y /2 - 200,
			400 , 430 , TRUE);

		ReleaseDC(hWnd, hDC);
	}
		break;
		
case WM_PAINT :

	
		
	 game.Draw(
		  gvb,
		 imgRect,
		 rectn,
		 isStartGameWithPosImage,hsb
		 ,hWnd
		 ,is_game_start
		 ,bmp
		 ,standart_game
		 ,img_name_for_standart_game
		 ,name_img,bkgr
		 ,img_name_for_bkg_before_start_game
		 ,work_img
		 ,100
		 ,100
		 );

	  break;	 

	
 case WM_ERASEBKGND:
       return 0;


   HANDLE_MSG(hWnd, WM_DRAWITEM  , OnDrawItem);
   HANDLE_MSG(hWnd, WM_DESTROY   , OnDestroy );
WM_COMAND:g_wp = wParam;   break;
   HANDLE_MSG(lParam, WM_COMMAND   , OnCommand ); break;
   
   }

   return DefWindowProc(hWnd, msg, wParam, lParam);
}









int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpszCmdLine, int nCmdShow )
{

 _CrtMemState _ms;   // тест на утечку памяти
 _CrtMemCheckpoint(&_ms);


 g_nCmdShow = nCmdShow;
GdiplusStartupInput gdiplusStartupInput; 
ULONG_PTR gdiplusToken; 
GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
   // создаю главное окно проги
   hIns = hInstance;
   game.hinst = hIns;
   KWnd mainWnd(L"Пятнашки", hInstance, nCmdShow, WndProc,NULL, 200, 100, 600, 400);
   // создаю кнопку для битмапы
   main = mainWnd.GetHWnd();
  
   
   newGame = ::CreateWindow(TEXT("button"), TEXT("пымпочка 2"), WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
      80, 100, 250, 50, mainWnd.GetHWnd() , (HMENU)NULL, hInstance ,NULL);
   buttonLoadGame = ::CreateWindow(TEXT("button"), TEXT("пымпочка 2"), WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
      80, 160, 250, 50, mainWnd.GetHWnd() , (HMENU)NULL, hInstance, NULL);  
    g_hWndBtn4 = ::CreateWindow(TEXT("button"), TEXT("пымпочка 2"), WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
     80, 220, 250, 50, mainWnd.GetHWnd() , (HMENU)NULL, hInstance, NULL);
   gvb.push_back( (HBITMAP)::LoadImage(hInstance, MAKEINTRESOURCE(IDB_BITMAP8), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR | LR_DEFAULTSIZE ));
  gvb.push_back( (HBITMAP)::LoadImage(hInstance, MAKEINTRESOURCE(IDB_BITMAP8), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR | LR_DEFAULTSIZE ));
  gvb.push_back( (HBITMAP)::LoadImage(hInstance, MAKEINTRESOURCE(IDB_BITMAP4), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR | LR_DEFAULTSIZE ));
  gvb.push_back( (HBITMAP)::LoadImage(hInstance, MAKEINTRESOURCE(IDB_BITMAP4), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR | LR_DEFAULTSIZE ));
  gvb.push_back( (HBITMAP)::LoadImage(hInstance, MAKEINTRESOURCE(IDB_BITMAP2), IMAGE_BITMAP, 0, 0,LR_DEFAULTCOLOR | LR_DEFAULTSIZE ));
  gvb.push_back( (HBITMAP)::LoadImage(hInstance,MAKEINTRESOURCE(IDB_BITMAP2), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR | LR_DEFAULTSIZE ));
  gvb.push_back((HBITMAP)::LoadImage(hInstance, MAKEINTRESOURCE(IDB_BITMAP6), IMAGE_BITMAP, 0, 0,LR_DEFAULTCOLOR | LR_DEFAULTSIZE ));
  gvb.push_back((HBITMAP)::LoadImage(hInstance, MAKEINTRESOURCE(IDB_BITMAP7), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR | LR_DEFAULTSIZE ));
   gvb.push_back((HBITMAP)::LoadImage(hInstance, MAKEINTRESOURCE(IDB_BITMAP3), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR | LR_DEFAULTSIZE ));
    gvb.push_back((HBITMAP)::LoadImage(hInstance, MAKEINTRESOURCE(IDB_BITMAP5), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR | LR_DEFAULTSIZE ));
	 gvb.push_back((HBITMAP)::LoadImage(hInstance, MAKEINTRESOURCE(IDB_BITMAP10), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR | LR_DEFAULTSIZE ));

   MSG msg;
   while (::GetMessage(&msg, NULL, 0, 0)){
      ::DispatchMessage(&msg);
   }


  
  

   GdiplusShutdown(gdiplusToken);
   _CrtMemDumpAllObjectsSince(&_ms);
   return msg.wParam;
     
}