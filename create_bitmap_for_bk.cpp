#include "StdAfx.h"
#include "create_bitmap_for_bk.h"

KDib::KDib() {
	fileHeadSize = sizeof(BITMAPFILEHEADER);
	fileHead.bfType = 0x4d42; 
	aBitmapBits = NULL;
}

KDib::~KDib() {
	if (pInfoHead)  delete [] pInfoHead;
	if (aBitmapBits)  delete [] aBitmapBits;
	if (outFile)  outFile.close();
}


SIZE KDib::SizeBitmap(HBITMAP hCaptureBitmapp) {
   SIZE resSize = {0,0};
   BITMAP bmp = {0,0,0,0,0,0,NULL};
   int iRes = ::GetObject(hCaptureBitmapp, sizeof(BITMAP), &bmp);
   if (iRes == sizeof(BITMAP)) {
      resSize.cx = bmp.bmWidth;
      resSize.cy = bmp.bmHeight;
   }
   return resSize;
}


BOOL KDib::CreateDib24(int w, int h, const char* fileName) 
	{

	width = w;
	height = h;
	bytesPerLine = ((width * 24 + 31) / 32) * 4;
	imageSize = bytesPerLine * height;

	infoHeadSize = sizeof(BITMAPINFOHEADER);
	fileHead.bfSize = fileHeadSize + infoHeadSize + bytesPerLine * height;
	fileHead.bfOffBits = fileHeadSize + infoHeadSize;

	infoHead.biSize = infoHeadSize;
	infoHead.biWidth =  width;
	infoHead.biHeight = height;
	infoHead.biPlanes = 1;
	infoHead.biBitCount = 24;
	infoHead.biCompression = BI_RGB;
	infoHead.biSizeImage = imageSize;

	aBitmapBits = new BYTE[imageSize];
	memset(aBitmapBits, 0, imageSize);

	outFile.open(fileName, ios::out | ios::binary | ios::trunc);
	if (!outFile) return FALSE;
	else return TRUE;
}

BOOL KDib::LoadFromFile(const char* fileName) {
	inpFile.open(fileName, ios::in | ios::binary);

	if (!inpFile) {
		error = "Неверное имя файла или каталога.";
		return FALSE;
	}

	inpFile.read((char*)&fileHead, fileHeadSize);
	if (fileHead.bfType != 0x4d42) {
		error = "Это не BMP-файл";
		return FALSE;
	}

	infoHeadSize = fileHead.bfOffBits - fileHeadSize;
	int fileSize = fileHead.bfSize;
	imageSize = fileSize - (fileHeadSize + infoHeadSize);

	pInfoHead = (BITMAPINFOHEADER*)(new BYTE [infoHeadSize]);
	inpFile.read((char*)pInfoHead, infoHeadSize);

	width =	pInfoHead->biWidth;
	height = pInfoHead->biHeight;

	aBitmapBits = new BYTE[imageSize];
	inpFile.read((char*)aBitmapBits, imageSize);
	return true;
}

int KDib::Draw(
	HDC hdc,
	int xDst,
	int yDst,
	int wDst,
	int hDst,
	int xSrc,
	int ySrc, 
	int wSrc, 
	int hSrc,
	DWORD rop
	) {

	return StretchDIBits(
		hdc,
		xDst, 
		yDst, 
		wDst,
		hDst,
		xSrc, 
		ySrc,
		wSrc, 
		hSrc,
		aBitmapBits, 
		(CONST BITMAPINFO*)pInfoHead, 
		DIB_RGB_COLORS,
		rop);
}

void KDib::SetPixel(int x, int y, COLORREF color) {

	int row = y;
	int col = 3 * x;
 
	aBitmapBits[row*bytesPerLine + col]   = GetBValue(color);
	aBitmapBits[row*bytesPerLine + col+1] = GetGValue(color);
	aBitmapBits[row*bytesPerLine + col+2] = GetRValue(color);
}

void KDib::StoreDib24() {
	// Запись заголовка BMP-файла
	outFile.write((char*)&fileHead, fileHeadSize);
	outFile.write((char*)&infoHead, infoHeadSize);
	// Запись массива пикселей
	outFile.write((char*)aBitmapBits, imageSize);
}


void KDib::DrawBitmap(HDC hdc, int x, int y, HBITMAP & hBitmap)
{
        HBITMAP hbm, hOldbm;
        HDC hMemDC;
        BITMAP bm;
        POINT ptSize, ptOrg;
 
        // Создаем контекст памяти, совместимый
        // с контекстом отображения
        hMemDC = CreateCompatibleDC(hdc);
 
        // Выбираем изображение bitmap в контекст памяти
        hOldbm = (HBITMAP)SelectObject(hMemDC, hBitmap);
 
        // Если не было ошибок, продолжаем работу
        if (hOldbm)
        {
                // Для контекста памяти устанавливаем тот же
                // режим отображения, что используется в
                // контексте отображения
                SetMapMode(hMemDC, GetMapMode(hdc));
 
                // Определяем размеры изображения
                GetObject(hBitmap, sizeof(BITMAP), (LPSTR) &bm);
 
                ptSize.x = bm.bmWidth;  // ширина
                ptSize.y = bm.bmHeight; // высота
 
                // Преобразуем координаты устройства в логические
                // для устройства вывода
                DPtoLP(hdc, &ptSize, 1);
 
                ptOrg.x = 0;
                ptOrg.y = 0;
 
                // Преобразуем координаты устройства в логические
                // для контекста памяти
                DPtoLP(hMemDC, &ptOrg, 1);
 
                // Рисуем изображение bitmap
                BitBlt(hdc, x, y, ptSize.x, ptSize.y,
                        hMemDC, ptOrg.x, ptOrg.y, SRCCOPY);
 
                // Восстанавливаем контекст памяти
                SelectObject(hMemDC, hOldbm);
        }
 
        // Удаляем контекст памяти
        DeleteDC(hMemDC);
}