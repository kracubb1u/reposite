#include "StdAfx.h"
#include "barleyBreak.h"

#include <cstdlib>
#include <ctime>

barleyBreak::barleyBreak()
	{
	/*
	Rectangles = { 
	{0,0,0,0,0,0},
	{0,1,2,3,4,0},
	{0,5,6,7,8,0},
	{0,9,10,11,12,0},
	{0,13,14,15,16,0},
	{0,0,0,0,0,0},
	};
		board = {
	                   {1,1,1,1,1,1},

					{1,   1,1,1,1,   1},
					{1,   1,1,1,1,   1},
					{1,   1,1,1,1,   1},
					{1,   1,1,1,0,   1},

					   {1,1,1,1,1,1},
					};
	*/
	int ch = 0;
	for(int i = 1; i < 5; i++)
		for(int j = 1; j < 5; j++){
			Rectangles[i][j] = ( ++ch );
			//board[i][j] = true;
			}
		for(int i = 0; i < 6; i++)
		for(int j = 0; j < 6; j++)
			board[i][j] = true;

		board[4][4] = 0;
		init = single = 0;
	}


barleyBreak::~barleyBreak(void)
	{
	 for(int i = 0; i < 4; i++)
		for(int j = 0; j < 4; j++){
			delete brush[i][j] ; 
			delete rects[i][j];
		
	 }
	}


void barleyBreak::create_animation(int pos,bool & false_,bool & true_,char side,HWND main)
	{
	int t = pos;
	false_ = 0;
	true_ = 1;
	Graphics g(GetDC(main));
	Color color(255,255,255);
	SolidBrush brush(color);
		int slide = -1;
	switch(side){
	case 'r' :  
		
		
			
			StretchBlt(GetDC(main),coord[t+1],coord[t+2],coord[2],coord[3],
					GetDC(main),coord[t-3],coord[t-2] ,coord[2],coord[3],SRCCOPY);
		g.FillRectangle(&brush,
					coord[t-3],coord[t-2],coord[2],coord[3]);
		
		
			
		break;
	case 'l' :  
		StretchBlt(GetDC(main),coord[t-7],coord[t-6],coord[2],coord[3],
					GetDC(main),
					coord[t-3],coord[t-2],coord[2],coord[3],SRCCOPY);
	
		g.FillRectangle(&brush,
					coord[t-3],coord[t-2],coord[2],coord[3]);
		
		break;
	case 't' :  StretchBlt(GetDC(main),coord[t-19],coord[t-18],coord[2],coord[3],
					GetDC(main),coord[t-3],coord[t-2],coord[2],coord[3],SRCCOPY); 
		g.FillRectangle(&brush, 
					coord[t-3],coord[t-2],coord[2],coord[3]);
		
		break;
	case 'b' :  StretchBlt(GetDC(main),coord[t+13],coord[t+14],coord[2],coord[3],
					GetDC(main),coord[t-3],coord[t-2],coord[2],coord[3],SRCCOPY);
		g.FillRectangle(&brush, 
					coord[t-3],coord[t-2],coord[2],coord[3]);
		
		break;
}
	}
void barleyBreak:: GameWin()
	{
	MessageBox(0,L"game is win",L"",MB_OK);
	}

bool barleyBreak::isGameWin()
	{
	if(Rectangles[4][4] != 16 ) return false; // если 16-й квадрат не белый - игра продолжаетс€ 
	for(int i  = 1; i < 5; i++)
	for(int j = 1; j < 4; j++)
		{
	    if(Rectangles[i][j+1] == (Rectangles[i][j]) + 1 )	;	
		else return false;
		}
	return true;
	}


void barleyBreak::step_logic_and_start_create_animation(int r_numb,HWND main)
	{
	int b_number = 1;
	int summ = 1;

	for(int i = 3; i < 65;i+=4){
		if(r_numb == i){
			b_number = summ;
			break;
			}
		++summ;
		}
	summ = 1;

	// b - номер кнопки от 1 до 16
	//теперь найду еЄ на нашей доске флагов и проверю можно ли делать ход
	for(int i = 1; i < 5; i++)
		for(int j = 1; j < 5; j++)
			{                                                                //          - 0 -
			if(b_number == summ ) { // кнопку нашел теперь провею можно ли делать ход    1 к 1       
				                                   //  1 - позици€ зан€ть . 0 - ходи     - 1 -
				if(!board[i][j-1]) { 

					create_animation(r_numb,board[i][j],board[i][j-1],'l',main);  // делаю ход в l (left) - запуск анимации //измен€ю флаги
					std::swap(Rectangles[i][j],Rectangles[i][j-1]);
				    std::swap(figure[Rectangles[i][j] ].X ,figure[ Rectangles[i][j-1] ].X);
					if(!isGameWin() )
					goto l; //выйти из цикла 
					else GameWin();

					}
				if(!board[i][j+1]) { 

					create_animation(r_numb,board[i][j],board[i][j+1],'r',main);
					std::swap(Rectangles[i][j],Rectangles[i][j+1]);
					 std::swap(figure[Rectangles[i][j] ].X ,figure[ Rectangles[i][j+1] ].X);
					if(!isGameWin() )
					goto l;
					else GameWin();

					} 
				if(!board[i-1][j]) { 

					create_animation(r_numb,board[i][j],board[i-1][j],'t',main) ;
					std::swap(Rectangles[i][j],Rectangles[i-1][j]);
					std::swap(figure[Rectangles[i][j] ] .Y,figure[ Rectangles[i-1][j] ].Y);				
					if(!isGameWin() )
					goto l;
					else GameWin();

					}
				if(!board[i+1][j]) { 

					create_animation(r_numb,board[i][j],board[i+1][j],'b',main); 
				    std::swap(Rectangles[i][j],Rectangles[i+1][j]);
				    std::swap(figure[Rectangles[i][j] ].Y ,figure[ Rectangles[i+1][j] ].Y);
					if(!isGameWin() )
					goto l;
					else GameWin();

					}
				
				}
			++summ;
			}
		l: ;//     // как бы не хотелось но из 2х циклов лахнуть только так)
	}

bool barleyBreak:: initialize_once(LPWSTR name_img,Image *& work_img)
	{

	 if(!single){
	 work_img = new Image(name_img);
		 single = true;
		 }

	 return single;

	}

 void barleyBreak:: Draw(HWND hWnd,
	 bool is_game_start,
	 KDib & bmp,
	 bool standart_game,
	 LPWSTR img_name_for_standart_game,
	 LPWSTR &name_img ,
	 KDib& bkgr,
	 char * img_name_for_bkg_before_start_game,
	 Image *& work_img
	  )
	 {
	 HDC hDC;
	
	 PAINTSTRUCT ps;
	static int ws, hs, wd, hd;
	 static int pieceW;
	static int pieceH;
     hDC = BeginPaint(hWnd, &ps);

   if(!is_game_start){ // если игра не началась - рисуй так 

		wd = ws = bmp.GetWidth();
		hd = hs = bmp.GetHeight();
	    bmp.Draw(hDC, 0, 0, wd, hd,0, 0, ws, hs, SRCCOPY);

	 }

    else { // если мы уже играем - то логика друга€ 

	 if(standart_game) name_img = img_name_for_standart_game;    
	 ///////////////////////////
	 


	 barleyBreak:: initialize_once(name_img,work_img);	 

     bkgr.LoadFromFile(img_name_for_bkg_before_start_game);
	 bkgr.Draw(hDC, 0, 0,bkgr.GetWidth(),bkgr.GetHeight(),0, 0, bkgr.GetWidth(), bkgr.GetHeight(), SRCCOPY);
           Graphics g(hDC);
	 Color color(255,255,255);
	 Pen pen(color,1); 
	 Color color_two(255,0,0);
	 Pen pen_two(color_two,2);

	 

	 static int piceW =
		work_img->GetWidth()		/ 4;
	static int piceH =
        work_img->GetHeight() 		/ 4;
		
      int k = -1;

   if(!init){
	for(int i = 0; i < 4; i++)
		for(int j = 0; j < 4; j++){
	      coord[++k] = ( j * piceW);
          coord[++k] = ( i * piceH);
          coord[++k] = (piceW * (j+1));
          coord[++k] = (piceH * (i+1));				
			rects[i][j] = new Rect(j*piceW,i*piceH,piceW,piceH);
			brush[i][j] = new TextureBrush( work_img,*rects[i][j]);
		}
		 
		//g.FillRectangle(&white_brush,*rects[3][3]);
		
		
		init = true;
		k = 0;

		// номер п€тнашки - еЄ координаты 
		Points point;

		int i = 0;
		int j = 0;

		for(int u = 0; u < 61; u+=4 ){
			
			if( j  == 4 ) {j = 0; ++i; }

			point.X = coord[u];
			point.Y = coord[u+1];
			point.i = i;
			point.j = j++;

			figure[++k] = point;

			}
		
	   }

	
	
	// перерисовать п€тнашки

	 Points point;
	 
	for(int  i = 1; i < 5; i++)
		for(int j = 1; j < 5; j++){
			 point = figure [ Rectangles[i][j] ];
			if(Rectangles[i][j] == 16 ) {
				g.FillRectangle(new SolidBrush(color),point.X,point.Y,coord[2],coord[3]);
				continue;
				}
	    g.DrawRectangle(&pen,point.X,point.Y,coord[2],coord[3]);			
		g.FillRectangle(brush[point.i][point.j] ,point.X +2 ,point.Y +2 ,coord[2],coord[3]);	
		}
		RECT p;
		GetClientRect(hWnd,&p);
		
		if(first) {

		for(int i = 0; i < time; 
			i++){
           Sleep(1000000);
		   char time_ = i + 1;
		   TextOut(GetDC(hWnd),( p.right - p.left ) / 2  , (p.bottom - p.top ) / 2,
			(LPCWSTR)time_,0);
};
		first = !false;
		shuffle();
		InvalidateRect(hWnd,&p,true);	

			}

		EndPaint(hWnd, &ps);

			
	 } 
	}

	void barleyBreak::shuffle()
	{
	    srand(::time(0));
	    for(int i = 0; i < 25; i++ )
		std::swap(barleyBreak::figure[ 1 + rand() % 15] ,
		barleyBreak::figure[1 + rand() % 15]);	
	}

	void barleyBreak::TIMER()
		{
		
		}