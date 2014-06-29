#include "StdAfx.h"
#include "barleyBreak.h"
#include <cstdlib>
#include <ctime>
#include <time.h>





barleyBreak::barleyBreak()
	{

	//эта штука находит лучший результат в файле,
	//в файл пишу время и кол-во перестановок
	boost::thread t(boost::bind(&barleyBreak::GetBestResultFromFile,this ));
	t.detach();
	isLoadGame = false;

	isThreadWork = false;
	singleton11 = true;
	d = L"Кол-во шагов: 0";
	
	int ch = 0;
	LPSYSTEMTIME t1;
	

	/*
	Rectangles = { 
	{0,0,0,0,0,0},
	{0,1,2,3,4,0},
	{0,5,6,7,8,0},
	{0,9,10,11,12,0},
	{0,13,14,15,16,0},
	{0,0,0,0,0,0},
	};
	*/
	for(int i = 1; i < 5; i++)
		for(int j = 1; j < 5; j++)
			Rectangles[i][j] = ( ++ch );
	//эти 2 цикла создают индефикаторы для каждой пятнашки
// при помощи этого массива я проверяю не выиграл ли пользователь
	// и перерисовываю игру  как все это выглядит см выше




		/*
		Rectangles = { 
	{0,0,0,0,0,0},
	{0,1,1,1,1,0},
	{0,1,1,1,1,0},
	{0,1,1,1,1,0},
	{0,1,1,1,0,0},
	{0,0,0,0,0,0},
	}; */
		for(int i = 0; i < 6; i++)
		for(int j = 0; j < 6; j++)
			board[i][j] = true;
/*при помоши этой штуки я буду проверять можно ли делать ход пятнашкой
и, если можно, то куда делать ход. 0 стоит там, куда ход делать можно
1 стоит там, где уже есть пятнашки 
см выше */


		board[4][4] = 0;
		init = single = 0;
	}


barleyBreak::~barleyBreak()
	{

	 for(int i = 0; i < 4; i++)
		for(int j = 0; j < 4; j++)
			delete rects[i][j];		
	
	}


void barleyBreak::create_animation(int pos,bool & false_,bool & true_,char side,HWND main,HWND hbs)
	{
	numberSteep++;


 d.Format(L"Кол-во шагов: %d",numberSteep);
     SendMessage(hbs, SB_SETTEXT ,0,(LPARAM)(LPCWSTR)d);
 
	
	int t = pos;
	false_ = 0;
	true_ = 1;
	Graphics g(GetDC(main));
	Color color(255,255,255);
	SolidBrush brush(color);
		int slide = -1;

/* side принимает слудющие значения: r,l,b,t - right left .. и т.д
например райт озанчает что мы ходим в право и т.д 
*/
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
	RECT rect;
	GetClientRect(main,&rect);
	rect.right -=125;
	rect.bottom -=22;
	UpdateWindow(hbs);
	InvalidateRect(main,&rect,false);
	
	}


void barleyBreak:: GameWin()
	{

	
	//boost::mutex::scoped_lock lock(this->io_mutex);
	//cond.wait(lock);
	//тут короче нужно ждать, пока поток, который находит лучший результат в файле
	//закончит свою работу, условные переменные использовать тут не получится
	//будут блокировки
	//поэтому прийдется тупо использовать цикл вайл...
	while(!isThreadFinish);

	if(!isThreadWork){
		this->isThreadWork = true;

		TCHAR szPath[90];
		SHGetFolderPath(NULL,CSIDL_MYDOCUMENTS, NULL, SHGFP_TYPE_CURRENT, szPath);
		CString path = L"Best_result.game";
		PathAppend(szPath, path);

		ofstream file(szPath,ios::app);		
		file<<numberSteep<<" "<<this->time__<<" ";

		isThreadWork = false;
		file.close();

		CString win;
		if(bst)
		win.Format(	L"Победа! \nвы играли: %s мин \nколичество перестановок - %d \nлучший результат: количество перестановок - %d  время %d мин",
		tend,
		numberSteep,
		(bst <= numberSteep)
		? bst
		:numberSteep,
		(time__ <= st) 
		? time 
		: st
		);

		else
		win.Format(	L"Победа! \nвы играли: %s мин \nколичество перестановок - %d \nлучший результат: количество перестановок - %d  время %d мин",
		tend,
		numberSteep,
		numberSteep,
		time__ 
		);
			
	
	this->isThreadWork = !true;
MessageBox(0,win,L"Поздравляю",MB_OK);
		}
}



bool barleyBreak::isGameWin()
	{
	
	if(Rectangles[4][4] != 16 ) return false; // если 16-й квадрат не белый - игра продолжается 

	//если 16 квадрат белый, нужно проверить все пятнашки 
	//алгоритм очень прост, у каждой пятнашки есть свой порядковый номер
	//получается,что у нас должна быть все время возрастающая последовательность.
	for(int i  = 1; i < 5; i++)
	for(int j = 1; j < 4; j++)
		{
		//вспомни что у каждой пятнашки есть свой индект, индексы я добавлял по возрастанию
		// следовательно если сначало идет 15 а потом 14 - выиграша нет
	    if( ! (Rectangles[i][j+1] == (Rectangles[i][j]) + 1 ))		
		return false;

		}

	return true;

	}


void barleyBreak::step_logic(int r_numb,HWND main,HWND hbs)
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
	//теперь найду её на нашей доске флагов и проверю можно ли делать ход
	for(int i = 1; i < 5; i++)
		for(int j = 1; j < 5; j++)
			{                                                                //          - 0 -
			if(b_number == summ ) { // кнопку нашел теперь провею можно ли делать ход    1 к 1       
				                                   //  1 - позиция занять . 0 - ходи     - 1 -
				if(!board[i][j-1]) { 

					create_animation(r_numb,board[i][j],board[i][j-1],'l',main,hbs);  // делаю ход в l (left) - запуск анимации //изменяю флаги
					//нужно поменять индифекаторы пятнашек местами
					std::swap(Rectangles[i][j],Rectangles[i][j-1]);
				    std::swap(figure[Rectangles[i][j] ].X ,figure[ Rectangles[i][j-1] ].X);
				    std::swap(figure[Rectangles[i][j] ].koefX ,figure[ Rectangles[i][j-1] ].koefX);
					if(!isGameWin() )
					goto l; //выйти из цикла 
					else GameWin();

					}
				if(!board[i][j+1]) { 

					create_animation(r_numb,board[i][j],board[i][j+1],'r',main,hbs);
					std::swap(Rectangles[i][j],Rectangles[i][j+1]);
					 std::swap(figure[Rectangles[i][j] ].X ,figure[ Rectangles[i][j+1] ].X);
					  std::swap(figure[Rectangles[i][j] ].koefX ,figure[ Rectangles[i][j+1] ].koefX);
					if(!isGameWin() )
					goto l;
					else GameWin();

					} 

				if(!board[i-1][j]) { 

					create_animation(r_numb,board[i][j],board[i-1][j],'t',main,hbs) ;
					std::swap(Rectangles[i][j],Rectangles[i-1][j]);
					std::swap(figure[Rectangles[i][j] ] .Y,figure[ Rectangles[i-1][j] ].Y);	
					std::swap(figure[Rectangles[i][j] ] .koefY,figure[ Rectangles[i-1][j] ].koefY);	
					if(!isGameWin() )
					goto l;
					else GameWin();

					}

				if(!board[i+1][j]) { 

					create_animation(r_numb,board[i][j],board[i+1][j],'b',main,hbs); 
				    std::swap(Rectangles[i][j],Rectangles[i+1][j]);
				    std::swap(figure[Rectangles[i][j] ].Y ,figure[ Rectangles[i+1][j] ].Y);
					std::swap(figure[Rectangles[i][j] ].koefY ,figure[ Rectangles[i+1][j] ].koefY);
					if(!isGameWin() )
					goto l;
					else GameWin();

					}
				
				}
			++summ;
			}
		l: ;
	}

bool barleyBreak:: initialize_once(
	LPWSTR name_img,
	Bitmap *& work_img,
	char * img_name_for_bkg_before_start_game,
	KDib& bkgr,
	bool standart_game
	)
	{
	//эта штука инициализирует указатель обьектом имейдж
	//логика такая: пользователь загружал игру - то открывай картинку
	//с папки документов 
	//если пользователь выбрал стандартную игру - открывай картинку из ресурсов
	// если пользователь выбрал свою картинку - открывай его картинку )))
	 if(!single){
		 /// тут я поставил флаг, чтобы эта функция исполнялась только 1 раз
		 TCHAR szPath[90];

		        SHGetFolderPath(
					NULL,
					CSIDL_MYDOCUMENTS,
					NULL,
					SHGFP_TYPE_CURRENT, 
					szPath
					);

				CString img2 = L"img_for_game.png";
			    PathAppend(szPath, img2);

		   if(!standart_game)     {

	 work_img = new Bitmap( ( isLoadGame  ) ? szPath
		 : name_img
		 );
	
			   }

		   else

	 work_img = new Bitmap(
	 this->hinst,
	 MAKEINTRESOURCE(IDB_BITMAP10)
	 );
			   
	  bkgr.LoadFromFile(img_name_for_bkg_before_start_game);

		 single = true;
		 }

	 return single;

	}



 void barleyBreak:: Draw(
	 std::vector<HBITMAP>&gvb,
	 RECT &imgRect,
	 RECT &rect_screen, 
	 bool isStartGameWithPosImage, 
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
	 int piceW ,	
	 int piceH  
      
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

		KDib::DrawBitmap( 
			hDC,
			0,
			0, 
			gvb[gvb.size()-3]
		);
	   
	 }

    else { // если мы уже играем - то логика другая 

   
	
	 


	 barleyBreak:: initialize_once(
	name_img,work_img,
	img_name_for_bkg_before_start_game,
	bkgr,
	standart_game
	);	 

	 

		 
    
	 



     Graphics g(hDC);
	 Color color(255,255,255);
	 Pen pen(color,1); 
	 

	 

	
		//получить размер 1 пятнашки 
	 static int piceW2 =
		work_img->GetWidth()		/ 4;
	static int piceH2  = 
       work_img->GetHeight() 		/ 4;
		static bool _ture = true;

		//если пользватель загружал игру 
		// размер пятнашки берется из файла
	if(rect_scr.left && rect_scr.bottom){ 
		
		piceW2 =
		(rect_scr.right - rect_scr.left)		/ 4;
	piceH2  = 
		(rect_scr.bottom - rect_scr.top) / 4;
	_ture = false;
		}


	
	//если пользователь выбрал свою картнику и выбрал на ней область
	if(isStartGameWithPosImage ){
	;
		
		rect_scr = rect_screen;


		//берем не все изображение а  ту область, которую он выбрал
		//область - rect_screen

		if(!isLoadGame){
						// использовать не все изображение
	piceW2 =
	abs	((rect_screen.right - rect_screen.left)		/ 4);
	piceH2  = 
		abs((rect_screen.bottom - rect_screen.top) / 4);}
		else{
		
			piceW2 =
		(rect_scr.right - rect_scr.left)		/ 4;
	piceH2  = 
		(rect_scr.bottom - rect_scr.top) / 4;}
		load_with_position_on_image  = false;
		}
	
    
	 //эту штука вызывается только 1 раз, по-хорошему ее бы надо закинуть в отдельную функцию
	//и вызывать ее 1 раз. 
	if(singleton11){
		int k = -1;
	   img_name = name_img;
	   //сoord - это массив в котором сохранены все координаты, куда мы будем рисовать 
	   //пятнашки  - также этот массив изменяется при ресайзе
	   // как все это работает ?
	   // у нас есть карта Figure
	   // также есть массив индификаторов Rectangle
	   //вызываем Figure[ Rectangle ] и для каждой пятнашки
	   // по её индификатору - присваиваем её координату для отрисовки на экране
	   //это еше не все
	   //массив coord используется в оконной процедуре главного окна
	   //он нужен, чтобы "ловить" сигналы от левой кнопки мыши
	   // там я сверяю координаты шелчка на вхождение в этот массив
	   //если координат сходятся - отпрвляю сигнал на прорисовку анамации в заданной области

	for(int i = 0; i < 4; i++)
		for(int j = 0; j < 4; j++){
	      coord[++k] = ( j * 100);
          coord[++k] = ( i * 100);
          coord[++k] = (100 * (j+1));
          coord[++k] = (100 * (i+1));	

		  if(singleton11){ 
		  if(!isStartGameWithPosImage && _ture)
			  //изображение "режим на кусочки" и сохраняем координаты в массив
			rects[i][j] = new serializable_rect(j*piceW2,i*piceH2,piceW2,piceH2);
		 
		 else {
			  int x = abs((!_ture) 
				  ? rect_scr.left -  imgRect.left 
				  : rect_screen.left - imgRect.left);
int y = abs((!_ture) 
	? rect_scr.top - imgRect.top
	: rect_screen.top - imgRect.top);

		    rects[i][j] = new serializable_rect(
				x + piceW2 *j,y + piceH2*i,piceW2,piceH2
				);
			 
			  }
		
			  }
		 SetWindowLong(hWnd ,GWL_EXSTYLE,CCS_TOP);				 
                                    GetWindowLong(
										hWnd, GWL_EXSTYLE | CCS_TOP
										);

		}
	
		if(isLoadGame)
			SendMessage(hWnd,WM_SIZING,0,0);
	
		k = 0;

		Points point;

		int i = 0;
		int j = 0;

		//это я делаю для упрошения прорисовки и ресайза
		//вообшем есть карта Figure в ней 16 элементов
		// берем элемент 5 - в нем есть координаты, по которым мы выводим изображение на экран
		// дальше в нем есть еше одни координаты на изображении откуда мы будем рисовать
		//также есть коефициенты, при помоши которых мы изменяем размер доски при ресайзе

		if(!isLoadGame)
		for(int u = 0; u < 61; u+=4 ){
			
			if( j  == 4 ) {j = 0; ++i; }

			point.X = coord[u];
			point.Y = coord[u+1];
			
			point.koefX = j;
			point.koefY = i;
			point.i = i;
			point.j = j++;
			
			figure[      ++k ] = point;

			}

		
		RECT rect;
		GetClientRect(hWnd,&rect);
		InvalidateRect(hWnd,&rect,0);
		
		
		
	   singleton11 = false;
	   isStartGameWithPosImage = false;
		}
		
	
	
	// перерисовать пятнашки
		{ // двойная буферизация 
	 Points point_;
	Graphics gg(hDC);
	 Rect dectR;

	 int max_x = GetSystemMetrics(SM_CXSCREEN);
                  int max_y = GetSystemMetrics(SM_CYSCREEN);

	 Bitmap buffer(
		 max_x, max_y,&gg
		 );

	 Graphics g(&buffer);
	 RECT r_C;
	 Color col(255,165,0);
	 GetClientRect(hWnd,&r_C);
	
	 g.FillRectangle(
		 new SolidBrush(
		 col),
		 r_C.left,
		 r_C.top,
		 r_C.right,
		 r_C.bottom);

	//бегаю по массиву и прорисовываю каждую пятнашку
	// у каждой пятнашки есть свой id
	//у каждого id cвои координаты на приемнике
	 // и своя часть изображения на исходном изображении

	for(int  i = 1; i < 5; i++)
		for(int j = 1; j < 5; j++){

			 point_ = figure [ Rectangles[i][j] ];

			if(Rectangles[i][j] == 16 ) {

				g.FillRectangle(
					new SolidBrush(color),
					point_.X,
					point_.Y,
					coord[2],
					coord[3]
				);
				continue;

				}	 
	
		
			dectR.X =  point_.X ; 
			dectR.Y =  point_.Y ;
			dectR.Width =  
				coord[2];
			dectR.Height =  
				coord[3];
			

			g.DrawImage(
				work_img,
				dectR,
				rects[point_.i][point_.j]->X,
				rects[point_.i][point_.j]->Y,
				rects[point_.i][point_.j]->Width,
				rects[point_.i][point_.j]->Height,
				UnitPixel
				);
			//эта штука рисует белую оводку, чтобы пятнашки не "слипались"
			 g.DrawRectangle(
				 &pen,
				 point_.X,
				 point_.Y,
				 coord[2],
				 coord[3]
			 );
			
			}
		
		 gg.DrawImage(&buffer,0,0,0,0,max_x,max_y,UnitPixel);
		}

		
	
         SendMessage(hbs, SB_SETTEXT ,0,(LPARAM)(LPCWSTR)d);
         EndPaint(hWnd,&ps);
		 DeleteDC(hDC);

	
			
	 } 

	}



//событие к кнопке перемешать
	void barleyBreak::shuffle(HWND hwnd)
	{
	    srand(::time(0));
		//checkout = !checkout;

		//cбить счетчики
		//лень исправлять фразу Steep на Step
 numberSteep = 0;
 time__ = time = 0;
 d = L"Kол-во шагов: 0";
 
		int kl,kk;

		//бегаю по массиву figure  и меняю координаты пятнашек
	    for(int i = 1; i < 44; i++ ){

			kk = 1 + rand() % 4 ;
			kl = (1 + rand() % 4) ;

			if(Rectangles[kk][kl] != 16 && Rectangles[kl][kk] != 16 ){

	std::swap(
		Rectangles[kk][kl],
		Rectangles[kl][kk]
	);

	std::swap(
		figure[Rectangles[kk][kl] ].Y ,
		figure[ Rectangles[kl][kk]].Y
		);

	std::swap(
		figure[Rectangles[kk][kl] ].koefY ,
		figure[ Rectangles[kl][kk]].koefY
		);

	std::swap(
		figure[Rectangles[kk][kl] ].X ,
		figure[ Rectangles[kl][kk]].X
		);

	std::swap(
		figure[Rectangles[kk][kl] ].koefX ,
		figure[ Rectangles[kl][kk]].koefX
		);
				}
	}
	

		RECT rect; 
		init = !false;
		GetClientRect(hwnd,&rect);
		rect.right -= 125;
		rect.bottom -= 22;
		InvalidateRect(hwnd,&rect,true);
	}


/*
эта штука бегает по файлу в отдельном потоке
ишет лучший результат
он используется, когда появляется окно GameWin
*/
	void barleyBreak::GetBestResultFromFile(){

		boost::mutex::scoped_lock lock(this->io_mutex);
	isThreadFinish = false;

		TCHAR szPath[90];
		SHGetFolderPath(NULL,CSIDL_MYDOCUMENTS, NULL, SHGFP_TYPE_CURRENT, szPath);
		CString path = L"Best_result.game";
		PathAppend(szPath, path);

		ifstream file(szPath);
		if(file){
			int step = 0; 
			int time__= 0;
			int maxStep = 0;
			int maxTime = 0;
			bool once = true;

			//обычный алгоритм поиска макс числа только в файле
			//сложность о(n)
			while( ! file.eof() ){
				file>>step;
				file>>time__;

				if(once){
					maxStep = step;
					maxTime = time__;
					once = false;
					}

				if(step <= maxStep && time__ <= maxTime)
					{
					maxTime = time__;
					maxStep = step;
					}
				}
			
			bst = maxStep;
			st = maxTime;
			file.close();

isThreadFinish = ! false;
			//cond.notify_one(); 
			}
		}



	int  barleyBreak::GetGameWidth(){
		int summ = 0;

		for(int i = 1;i < 5; i++)
          summ+=figure[i].X;

		return summ  ;
		}



	//событие для ресайза
	// получаем клиентский квадрат
	//и подгоняем размеры доски под размеры окна
	//пропорции доски остаются

	void barleyBreak::Resize(RECT & rect)
		{
	   // высота - ширина
		coord[2] = coord[3] = abs((rect.right - rect.left - 150) / 4);

		int k = 0, l = 0,p = 0;		

		
	for(int  i = 1; i < 5; i++)
		for(int j = 1; j < 5; j++){		
			
			figure[Rectangles[i][j] ].X = coord[2] * figure[ Rectangles[i][j] ] . koefX;
			figure[Rectangles[i][j] ].Y = coord[2] * figure[ Rectangles[i][j] ] . koefY;
			
			}

		k = l = p = 0;
		for(int u = 0; u < 61; u+=4 ){
			
			if(k == 4) {k = 0; ++l;}
			if(l == 4) l = 0;

			coord[u] = coord[2] * k++ ;
			coord[u+1] = coord[2] *l;
			coord[u+2] = coord[u] + coord[2];
			coord[u+3] = coord[u+1] + coord[2];
		  
				
			}
			
	}



//эта штука возврашает цлсид для формата картинки
	//нужно это для того, чтобы сохранить картинку в выбраном формате

	int barleyBreak::GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
	using namespace Gdiplus;
	UINT  num = 0;         
	UINT  size = 0;        
	ImageCodecInfo* pImageCodecInfo = NULL;
	GetImageEncodersSize(&num, &size);
	if(size == 0)
		return -1; 
	pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
	if(pImageCodecInfo == NULL)
		return -1;  
	GetImageEncoders(num, size, pImageCodecInfo);
	for(UINT j = 0; j < num; ++j)
	{
		if( wcscmp(pImageCodecInfo[j].MimeType, format) == 0 )
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;  
		}    
	}
	free(pImageCodecInfo);
	return 0;
}



		void barleyBreak::Save(long time,bool&exist,Bitmap *& img){

         boost::mutex::scoped_lock lock(io_mutex2);

		 exist = true;
		
			try{
			
			//сохраняю в папку документы
				//сохраняю картинку. имя задаю хардкором
				//сохраняю файл - имя тоже хардкор

				TCHAR szPath[90];

		        SHGetFolderPath(
					NULL,
					CSIDL_MYDOCUMENTS,
					NULL, 
					SHGFP_TYPE_CURRENT,
					szPath

					);

				CString img2 = L"img_for_game.png";
				CString path = L"сохранение игры пятнашки.save";
				CLSID pngClsid ;

				GetEncoderClsid(L"image/png", &pngClsid);
				  PathAppend(szPath, img2);
				    img->Save(szPath,&pngClsid,0);

				   SHGetFolderPath(
					   NULL,
					   CSIDL_MYDOCUMENTS, 
					   NULL, 
					   SHGFP_TYPE_CURRENT,
					   szPath
					   );
				 
				   PathAppend(szPath, path);
				
			std::ofstream file(szPath);
			
			file<<16<<" ";
for(int i = 0; i < figure.size(); i++){
	file<<figure[i].X<<" ";
	file<<figure[i].Y<<" ";
		file<<figure[i].i<<" ";
		file<<figure[i].j<<" ";
		file<<figure[i].koefX<<" ";
		file<<figure[i].koefY<<" ";
	}

file<<"         ";
for(int i = 1; i < 5; i++)
		for(int j = 1; j < 5; j++){
			file<<Rectangles[i][j]<< " " ;
			file<<board[i][j]<<" ";
			
			}
		for(int u = 0; u < 61; u+=4 ){
		file<<coord[u]<<" ";
			file<<coord[u+1]<<" ";
			file<<coord[u+2]<<" ";
			file<<coord[u+3]<<" ";
			}
	static	std::string s;

		if(!s.length())
		while(*img_name)
			s+=*img_name++;
		

	file<<rect_scr.left<<" ";
	file<<rect_scr.bottom<<" ";
	file<<rect_scr.top<<" ";
	file<<rect_scr.right<<" ";

		file<<s;
file.close();
MessageBox(0,L"Игра сохранена... В следующий раз вы можете начать играть с этого места.",L"OK",0);
				}
			
			catch(...)
				{
				MessageBox(0,0,L"Не удалось сохранить игру",0);
				exist = false;
				}
			exist = false;

			}


bool barleyBreak::LoadGame(HWND main)
			{
			isLoadGame = true;
			load_with_position_on_image = true;
			try{

				TCHAR szPath[90];
		        SHGetFolderPath(NULL,CSIDL_MYDOCUMENTS, NULL, SHGFP_TYPE_CURRENT, szPath);
				CString path = L"сохранение игры пятнашки.save";
				 PathAppend(szPath, path);
		 
			std::ifstream  file(szPath); 

			if(!file){
				MessageBox(main,L"Упс... похоже, что сохранений нет. Начните новую игру.",0,MB_OK 
					| 
					MB_ICONEXCLAMATION
 
);  
				isLoadGame = false;
				return 0;

				}

			int size; file>>size;
			for(int i = 0; i <= size; i++)
				{
				file>>figure[i].X;
				file>>figure[i].Y;
				file>>figure[i].i;
				file>>figure[i].j;
				file>>figure[i].koefX;
	         	file>>figure[i].koefY;
				}

			for(int i = 1; i < 5; i++)
		for(int j = 1; j < 5; j++){
			file>>Rectangles[i][j];
			file>>board[i][j];
			}

		for(int u = 0; u < 61; u+=4 ){
		file>>	coord[u];
			file>>coord[u+1];
			file>>coord[u+2];
			file>>coord[u+3];
			}
		file>>rect_scr.left;
	file>>rect_scr.bottom;
	file>>rect_scr.top;
	file>>rect_scr.right;
		std::string s;
		file>>s; std::wstring ss;
		

		
//это можно не читать
		// тут одну строку перевожу в другую
 int len;
 int slength = (int)s.length() + 1;
 len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0); 
 wchar_t* buf = new wchar_t[len];
 MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
 std::wstring r(buf);
 delete[] buf;
 ss = r;	LPWSTR sss;	
  LPWSTR ws = new wchar_t[s.size()+1];
  copy( s.begin(), s.end(), ws );
  ws[s.size()] = 0;
  sss = ws;	 img_name = sss;

			file.close();
				}
			catch(...)
				{
				
				MessageBox(0,0,L"Не удалось открыть файл",0);
				return false;
				}
			
			return true;
			}









	void barleyBreak::TIMER()
		{
		
		}