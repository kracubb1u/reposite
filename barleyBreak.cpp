#include "StdAfx.h"
#include "barleyBreak.h"
#include <cstdlib>
#include <ctime>
#include <time.h>





barleyBreak::barleyBreak()
	{

	//��� ����� ������� ������ ��������� � �����,
	//� ���� ���� ����� � ���-�� ������������
	boost::thread t(boost::bind(&barleyBreak::GetBestResultFromFile,this ));
	t.detach();
	isLoadGame = false;

	isThreadWork = false;
	singleton11 = true;
	d = L"���-�� �����: 0";
	
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
	//��� 2 ����� ������� ������������ ��� ������ ��������
// ��� ������ ����� ������� � �������� �� ������� �� ������������
	// � ������������� ����  ��� ��� ��� �������� �� ����




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
/*��� ������ ���� ����� � ���� ��������� ����� �� ������ ��� ���������
�, ���� �����, �� ���� ������ ���. 0 ����� ���, ���� ��� ������ �����
1 ����� ���, ��� ��� ���� �������� 
�� ���� */


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


 d.Format(L"���-�� �����: %d",numberSteep);
     SendMessage(hbs, SB_SETTEXT ,0,(LPARAM)(LPCWSTR)d);
 
	
	int t = pos;
	false_ = 0;
	true_ = 1;
	Graphics g(GetDC(main));
	Color color(255,255,255);
	SolidBrush brush(color);
		int slide = -1;

/* side ��������� �������� ��������: r,l,b,t - right left .. � �.�
�������� ���� �������� ��� �� ����� � ����� � �.� 
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
	//��� ������ ����� �����, ���� �����, ������� ������� ������ ��������� � �����
	//�������� ���� ������, �������� ���������� ������������ ��� �� ���������
	//����� ����������
	//������� ��������� ���� ������������ ���� ����...
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
		win.Format(	L"������! \n�� ������: %s ��� \n���������� ������������ - %d \n������ ���������: ���������� ������������ - %d  ����� %d ���",
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
		win.Format(	L"������! \n�� ������: %s ��� \n���������� ������������ - %d \n������ ���������: ���������� ������������ - %d  ����� %d ���",
		tend,
		numberSteep,
		numberSteep,
		time__ 
		);
			
	
	this->isThreadWork = !true;
MessageBox(0,win,L"����������",MB_OK);
		}
}



bool barleyBreak::isGameWin()
	{
	
	if(Rectangles[4][4] != 16 ) return false; // ���� 16-� ������� �� ����� - ���� ������������ 

	//���� 16 ������� �����, ����� ��������� ��� �������� 
	//�������� ����� �����, � ������ �������� ���� ���� ���������� �����
	//����������,��� � ��� ������ ���� ��� ����� ������������ ������������������.
	for(int i  = 1; i < 5; i++)
	for(int j = 1; j < 4; j++)
		{
		//������� ��� � ������ �������� ���� ���� ������, ������� � �������� �� �����������
		// ������������� ���� ������� ���� 15 � ����� 14 - �������� ���
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

	// b - ����� ������ �� 1 �� 16
	//������ ����� � �� ����� ����� ������ � ������� ����� �� ������ ���
	for(int i = 1; i < 5; i++)
		for(int j = 1; j < 5; j++)
			{                                                                //          - 0 -
			if(b_number == summ ) { // ������ ����� ������ ������ ����� �� ������ ���    1 � 1       
				                                   //  1 - ������� ������ . 0 - ����     - 1 -
				if(!board[i][j-1]) { 

					create_animation(r_numb,board[i][j],board[i][j-1],'l',main,hbs);  // ����� ��� � l (left) - ������ �������� //������� �����
					//����� �������� ������������ �������� �������
					std::swap(Rectangles[i][j],Rectangles[i][j-1]);
				    std::swap(figure[Rectangles[i][j] ].X ,figure[ Rectangles[i][j-1] ].X);
				    std::swap(figure[Rectangles[i][j] ].koefX ,figure[ Rectangles[i][j-1] ].koefX);
					if(!isGameWin() )
					goto l; //����� �� ����� 
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
	//��� ����� �������������� ��������� �������� ������
	//������ �����: ������������ �������� ���� - �� �������� ��������
	//� ����� ���������� 
	//���� ������������ ������ ����������� ���� - �������� �������� �� ��������
	// ���� ������������ ������ ���� �������� - �������� ��� �������� )))
	 if(!single){
		 /// ��� � �������� ����, ����� ��� ������� ����������� ������ 1 ���
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

   if(!is_game_start){ // ���� ���� �� �������� - ����� ��� 

		wd = ws = bmp.GetWidth();
		hd = hs = bmp.GetHeight();

		KDib::DrawBitmap( 
			hDC,
			0,
			0, 
			gvb[gvb.size()-3]
		);
	   
	 }

    else { // ���� �� ��� ������ - �� ������ ������ 

   
	
	 


	 barleyBreak:: initialize_once(
	name_img,work_img,
	img_name_for_bkg_before_start_game,
	bkgr,
	standart_game
	);	 

	 

		 
    
	 



     Graphics g(hDC);
	 Color color(255,255,255);
	 Pen pen(color,1); 
	 

	 

	
		//�������� ������ 1 �������� 
	 static int piceW2 =
		work_img->GetWidth()		/ 4;
	static int piceH2  = 
       work_img->GetHeight() 		/ 4;
		static bool _ture = true;

		//���� ����������� �������� ���� 
		// ������ �������� ������� �� �����
	if(rect_scr.left && rect_scr.bottom){ 
		
		piceW2 =
		(rect_scr.right - rect_scr.left)		/ 4;
	piceH2  = 
		(rect_scr.bottom - rect_scr.top) / 4;
	_ture = false;
		}


	
	//���� ������������ ������ ���� �������� � ������ �� ��� �������
	if(isStartGameWithPosImage ){
	;
		
		rect_scr = rect_screen;


		//����� �� ��� ����������� �  �� �������, ������� �� ������
		//������� - rect_screen

		if(!isLoadGame){
						// ������������ �� ��� �����������
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
	
    
	 //��� ����� ���������� ������ 1 ���, ��-�������� �� �� ���� �������� � ��������� �������
	//� �������� �� 1 ���. 
	if(singleton11){
		int k = -1;
	   img_name = name_img;
	   //�oord - ��� ������ � ������� ��������� ��� ����������, ���� �� ����� �������� 
	   //��������  - ����� ���� ������ ���������� ��� �������
	   // ��� ��� ��� �������� ?
	   // � ��� ���� ����� Figure
	   // ����� ���� ������ ������������� Rectangle
	   //�������� Figure[ Rectangle ] � ��� ������ ��������
	   // �� � ������������ - ����������� � ���������� ��� ��������� �� ������
	   //��� ��� �� ���
	   //������ coord ������������ � ������� ��������� �������� ����
	   //�� �����, ����� "������" ������� �� ����� ������ ����
	   // ��� � ������ ���������� ������ �� ��������� � ���� ������
	   //���� ��������� �������� - �������� ������ �� ���������� �������� � �������� �������

	for(int i = 0; i < 4; i++)
		for(int j = 0; j < 4; j++){
	      coord[++k] = ( j * 100);
          coord[++k] = ( i * 100);
          coord[++k] = (100 * (j+1));
          coord[++k] = (100 * (i+1));	

		  if(singleton11){ 
		  if(!isStartGameWithPosImage && _ture)
			  //����������� "����� �� �������" � ��������� ���������� � ������
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

		//��� � ����� ��� ��������� ���������� � �������
		//������� ���� ����� Figure � ��� 16 ���������
		// ����� ������� 5 - � ��� ���� ����������, �� ������� �� ������� ����������� �� �����
		// ������ � ��� ���� ��� ���� ���������� �� ����������� ������ �� ����� ��������
		//����� ���� �����������, ��� ������ ������� �� �������� ������ ����� ��� �������

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
		
	
	
	// ������������ ��������
		{ // ������� ����������� 
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

	//����� �� ������� � ������������ ������ ��������
	// � ������ �������� ���� ���� id
	//� ������� id c��� ���������� �� ���������
	 // � ���� ����� ����������� �� �������� �����������

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
			//��� ����� ������ ����� ������, ����� �������� �� "���������"
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



//������� � ������ ����������
	void barleyBreak::shuffle(HWND hwnd)
	{
	    srand(::time(0));
		//checkout = !checkout;

		//c���� ��������
		//���� ���������� ����� Steep �� Step
 numberSteep = 0;
 time__ = time = 0;
 d = L"K��-�� �����: 0";
 
		int kl,kk;

		//����� �� ������� figure  � ����� ���������� ��������
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
��� ����� ������ �� ����� � ��������� ������
���� ������ ���������
�� ������������, ����� ���������� ���� GameWin
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

			//������� �������� ������ ���� ����� ������ � �����
			//��������� �(n)
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



	//������� ��� �������
	// �������� ���������� �������
	//� ��������� ������� ����� ��� ������� ����
	//��������� ����� ��������

	void barleyBreak::Resize(RECT & rect)
		{
	   // ������ - ������
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



//��� ����� ���������� ����� ��� ������� ��������
	//����� ��� ��� ����, ����� ��������� �������� � �������� �������

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
			
			//�������� � ����� ���������
				//�������� ��������. ��� ����� ���������
				//�������� ���� - ��� ���� �������

				TCHAR szPath[90];

		        SHGetFolderPath(
					NULL,
					CSIDL_MYDOCUMENTS,
					NULL, 
					SHGFP_TYPE_CURRENT,
					szPath

					);

				CString img2 = L"img_for_game.png";
				CString path = L"���������� ���� ��������.save";
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
MessageBox(0,L"���� ���������... � ��������� ��� �� ������ ������ ������ � ����� �����.",L"OK",0);
				}
			
			catch(...)
				{
				MessageBox(0,0,L"�� ������� ��������� ����",0);
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
				CString path = L"���������� ���� ��������.save";
				 PathAppend(szPath, path);
		 
			std::ifstream  file(szPath); 

			if(!file){
				MessageBox(main,L"���... ������, ��� ���������� ���. ������� ����� ����.",0,MB_OK 
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
		

		
//��� ����� �� ������
		// ��� ���� ������ �������� � ������
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
				
				MessageBox(0,0,L"�� ������� ������� ����",0);
				return false;
				}
			
			return true;
			}









	void barleyBreak::TIMER()
		{
		
		}