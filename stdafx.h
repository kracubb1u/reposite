// stdafx.h: включаемый файл для стандартных системных включаемых файлов
// или включаемых файлов для конкретного проекта, которые часто используются, но
// не часто изменяются
//

#pragma once

#include "targetver.h"

//#define WIN32_LEAN_AND_MEAN             // Исключите редко используемые компоненты из заголовков Windows
// Файлы заголовков Windows:
#include <windows.h>

//#include "boost/lexical_cast.hpp"
//#include "boost/signals.hpp"
#include <boost/regex.hpp>
#include <boost/foreach.hpp>
#include <boost/thread/condition.hpp>
#include <boost\thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/bind.hpp>


#include "resource2.h"
#include <stdio.h>
#include <WindowsX.h>
#include <TChar.h>
#include "CommDlg.h"
#include <vector>
#include <GdiPlus.h>
#include <objidl.h>

// Файлы заголовков C RunTime
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // некоторые конструкторы CString будут явными

#include <atlbase.h>
#include <atlstr.h>
#include <shlobj.h>
// TODO: Установите здесь ссылки на дополнительные заголовки, требующиеся для программы

#include <iostream> 

#include <fstream>

#define WM_GET_RECTANGLE -923
#define WM_OK -23232
#define WM_CHOICE_marquee 982
#define WM_GAME_START 333
#define WM_SCREEN 322



#include <GdiPlus.h>
#include "create_bitmap_for_bk.h"
using namespace Gdiplus;
#include <Commctrl.h>
#pragma comment(lib,"Comctl32.lib")
#pragma comment(lib, "GdiPlus.lib")