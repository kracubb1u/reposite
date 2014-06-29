#include "StdAfx.h"
#include "Desktop.h"

boost::mutex io_mutex;

Desktop::Desktop(void)
	{
	}


Desktop::~Desktop(void)
	{
	}


//эта штука делает скриншот любой область
//скриншот сохраняем в документы
//и выбиваем окно 

	void Desktop::CaptureScreen( int x,int y, int w,int h,bool & exist)
	{

	 boost::mutex::scoped_lock lock(io_mutex);

	 exist = true;

    static int mdf = 0;
	TCHAR szPath[90];

	//эта штука получает путь к папке документы
		  SHGetFolderPath(NULL,CSIDL_MYDOCUMENTS, NULL, SHGFP_TYPE_CURRENT, szPath);

		  SYSTEMTIME tt;

		  GetSystemTime(&tt);

		 //эта штука готовит уникальный id для картинки
		  CString path; path.Format(
			L"%d.%d.%d.%d.%d.%d - screen.bmp",
			(int)tt.wDay,
			(int)tt.wHour,
			(int) tt.wSecond,
			(int)tt.wMinute,
			(int)tt.wMilliseconds,
			mdf
			);

		  PathAppend(szPath, path);
		 
		  ++mdf;

	 	 HWND hDesktopWnd = GetDesktopWindow();
	 	 HDC hDesktopDC = GetDC(hDesktopWnd);
	     HDC hCaptureDC = CreateCompatibleDC(hDesktopDC);
	 	 HBITMAP hCaptureBitmap =CreateCompatibleBitmap(hDesktopDC, 
								w, h);
		SelectObject(hCaptureDC,hCaptureBitmap); 
		BitBlt(hCaptureDC,0,0,w,h,
		hDesktopDC,x,y,SRCCOPY|CAPTUREBLT); 
	    BITMAP BM; 

	 BITMAPFILEHEADER BFH; 
	 LPBITMAPINFO BIP; 
	 HDC DC; 
	 LPBYTE Buf; 
     DWORD ColorSize,DataSize; 
	 WORD BitCount; 
	 HANDLE FP; 
	 DWORD dwTemp; 


	GetObject(hCaptureBitmap, sizeof(BITMAP), (LPSTR)&BM); 


	BitCount = (WORD)BM.bmPlanes * BM.bmBitsPixel; 
	switch (BitCount) 
	{ 
	case 1: 
	case 4: 
	case 8: 
	case 32: 
	ColorSize = sizeof(RGBQUAD) * (1 << BitCount); 
	case 24: 
	ColorSize = 0; 
	} 


	DataSize = ((BM.bmWidth*BitCount+31) & ~31)/8*BM.bmHeight; 

	BIP=(LPBITMAPINFO)HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,sizeof(BITMAPINFO)+ColorSize); 

	BIP->bmiHeader.biSize = sizeof(BITMAPINFOHEADER); 
	BIP->bmiHeader.biWidth = BM.bmWidth; 
	BIP->bmiHeader.biHeight = BM.bmHeight; 
	BIP->bmiHeader.biPlanes = 1; 
	BIP->bmiHeader.biBitCount = BitCount; 
	BIP->bmiHeader.biCompression = 0; 
	BIP->bmiHeader.biSizeImage = DataSize; 
	BIP->bmiHeader.biXPelsPerMeter = 0; 
	BIP->bmiHeader.biYPelsPerMeter = 0; 
	if (BitCount < 24) 
	BIP->bmiHeader.biClrUsed = (1<<BitCount); 
	BIP->bmiHeader.biClrImportant = 0; 



	BFH.bfType = 0x4d42; 
	BFH.bfOffBits=sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+ BIP->bmiHeader.biClrUsed * sizeof(RGBQUAD); 
	BFH.bfReserved1 = 0; 
	BFH.bfReserved2 = 0; 
	BFH.bfSize = BFH.bfOffBits + DataSize; 


	Buf = (LPBYTE)GlobalAlloc(GMEM_FIXED, DataSize); 


	DC = GetDC(0); 
	GetDIBits(DC, hCaptureBitmap, 0,(WORD)BM.bmHeight, Buf, BIP, DIB_RGB_COLORS); 
	ReleaseDC(0, DC); 

	FP=CreateFile(szPath,GENERIC_READ | GENERIC_WRITE, 0, NULL, 
	CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL); 
	WriteFile(FP,&BFH,sizeof(BITMAPFILEHEADER),&dwTemp,NULL); 
	WriteFile(FP,BIP,sizeof(BITMAPINFOHEADER) + BIP->bmiHeader.biClrUsed * sizeof(RGBQUAD),&dwTemp,NULL); 
	WriteFile(FP,Buf,DataSize,&dwTemp,NULL); 

	CloseHandle(FP); 
	GlobalFree((HGLOBAL)Buf); 
	HeapFree(GetProcessHeap(),0,(LPVOID)BIP); 
		ReleaseDC(hDesktopWnd,hDesktopDC);
		DeleteDC(hCaptureDC);
		DeleteObject(hCaptureBitmap);





		CString d; d.Format(	L"Cкриншот %s",path);
		d+='\n';
		d += L"сохранен в папку документы текущего пользователя ";
	    MessageBox(0,d ,L"ОК",0);

		exist = false;
	}
