#pragma once



//этот класс отвечает за всю логику в игре

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



  
	class serializable_rect : public Rect{
	public:
		serializable_rect() : Rect() {}
		serializable_rect(int x,int y, int x2,int y2): Rect(x,y,x2,y2) {}
    
};



class barleyBreak
	{
	public:
		bool LoadGame(HWND main);
		void SetTime(long &t) { 
			tend.Format(L"%d",t/60); time__ = t/60;			}
		void step_logic(int r_numb,HWND main,HWND);
		bool isGameWin();
		void GameWin();
		LPWSTR GetImgName();
		void create_animation(int pos,bool & false_,bool & true_,char side,HWND main,HWND);
		barleyBreak();
		~barleyBreak();
		void Save(long time,bool&,Bitmap *&);
		void shuffle(HWND hwnd);
		void Resize(RECT & rect);
		int GetGameWidth();
		void TIMER();
		void set_time(double a = 5.0) { time = a; };
		bool barleyBreak:: initialize_once(
	    LPWSTR name_img,
	    Bitmap *& work_img,
	    char * img_name_for_bkg_before_start_game,
	    KDib& bkgr,
		bool
	    );
		int barleyBreak::GetEncoderClsid(const WCHAR* format, CLSID* pClsid);
	    void set_status(bool a) { init = a; };
		int coord[65];
		CString GetNumStep() { return d; }
		void barleyBreak::GetBestResultFromFile();
		Bitmap* barleyBreak::BitmapFromResource(HINSTANCE hInstance,
               LPCTSTR szResName, LPCTSTR szResType);
		
void Draw(	
	std::vector<HBITMAP>&,
	RECT &,
	 RECT& , 
	 bool, 
	 HWND hbs,
	 HWND hWnd,
	 bool is_game_start,
	 KDib & bmp,
	 bool standart_game,
	 LPWSTR img_name_for_standart_game,
	 LPWSTR &name_img ,
	 KDib& bkgr,
	 char * img_name_for_bkg_before_start_game,
	Bitmap *& work_img,
	 int piceW = 100,	
	 int piceH  = 100
  );
		

	private:

	boost::mutex io_mutex;
	boost::mutex io_mutex2;
	boost::condition cond;

long bst;
long st;
bool checkout;
bool singleton11 ;
long numberSteep;
long time__;
		CString d;
		CString tbegin;
		CString tend;
		 bool isThreadFinish;
	int Rectangles[6][6];
	bool board[6][6];
	serializable_rect * rects[4][4];
   // TextureBrush *brush[4][4];
    bool init;
	bool single ;
	double time;
	bool first;
	LPWSTR img_name;
	RECT rect_scr;
	struct Points {	
		int X;
		int Y;
		int i;
		int j;
		int koefX;
		int koefY;
		};

	public:HINSTANCE hinst;
   bool load_with_position_on_image ;
	bool isThreadWork;
	bool isLoadGame;
	public:std::map<int,Points> figure;
	};

#endif