#include "StdAfx.h"
#include "Desktop.h"


Desktop::Desktop(void)
	{
	}


Desktop::~Desktop(void)
	{
	}

	void Desktop::CaptureScreen( int x,int y, int w,int h,LPCTSTR lpszFileName)
	{
    
	static 	 HWND hDesktopWnd = GetDesktopWindow();
	static 	 HDC hDesktopDC = GetDC(hDesktopWnd);
	static 	 HDC hCaptureDC = CreateCompatibleDC(hDesktopDC);
	static 	 HBITMAP hCaptureBitmap =CreateCompatibleBitmap(hDesktopDC, 
								w, h);
		SelectObject(hCaptureDC,hCaptureBitmap); 
		BitBlt(hCaptureDC,0,0,w,h,
			   hDesktopDC,x,y,SRCCOPY|CAPTUREBLT); 
		static BITMAP BM; 
	static  BITMAPFILEHEADER BFH; 
	static LPBITMAPINFO BIP; 
	static HDC DC; 
	static LPBYTE Buf; 
    static 	DWORD ColorSize,DataSize; 
	static WORD BitCount; 
	static HANDLE FP; 
	static DWORD dwTemp; 


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

	FP=CreateFile(lpszFileName,GENERIC_READ | GENERIC_WRITE, 0, NULL, 
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
	}
