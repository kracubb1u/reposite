#pragma once

#ifndef _
#define _

#include <algorithm>
#include <map>

//Этот массив -  координаты каждой пятнашки для перерисовки
//int coord[65];
//Этот массив - координаты каждой пятнашки для проверки на выиграш
//int Rectangles[6][6];

//Этот массив - для проверки можно ли делать ход
//bool board[6][6];

class barleyBreak
	{
	public:
		void step_logic_and_start_create_animation(int r_numb,HWND main);
		bool isGameWin();
		void GameWin();
		void create_animation(int pos,bool & false_,bool & true_,char side,HWND main);
		barleyBreak();
		~barleyBreak();
		
		void Draw(HWND hWnd,
	 bool is_game_start,
	 KDib & bmp,
	 bool standart_game,
	 LPWSTR img_name_for_standart_game,
	 LPWSTR &name_img ,
	 KDib& bkgr,
	 char * img_name_for_bkg_before_start_game,
	 Image *& work_img
	  );
		void shuffle();
		void TIMER();
		void set_time(double a = 5.0) { time = a; };
    bool barleyBreak:: initialize_once(LPWSTR name_img,Image *& work_img);
		//координаты пятнашек используются как глобальная переменная
		 int coord[65];
		  
	private:
		int Rectangles[6][6];
		bool board[6][6];
		Rect * rects[4][4];
        TextureBrush *brush[4][4];
        bool init;
		bool single ;
		double time;
		bool first;
		struct Points {
			int X;
			int Y;
			int i;
			int j;
			};
		std::map<int,Points> figure;
	};

#endif