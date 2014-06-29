#pragma once

#include "stdafx.h"


HWND hWndD2;

class ToolsWindow
	{
	
	
public:

	   

		static void CreateToolsWindow(HINSTANCE hIns,HWND hWndD,RECT & rect)
			{
			HWND hh = CreateDialog(
				 hIns, 
				 MAKEINTRESOURCE(IDD_DIALOG4),
				 hWndD,
				 ToolsWindow::DlgProc2);

				                    SetWindowLong(hh ,GWL_EXSTYLE,WS_EX_LAYERED);				 
                                    GetWindowLong(hh, GWL_EXSTYLE | WS_EX_LAYERED);
	                                SetLayeredWindowAttributes(hh, 0, 200, LWA_ALPHA);	
	                               	ShowWindow(hh,SW_SHOW);
			                    	SetWindowPos(hh,0,rect.right - 100,rect.top,180,140,SWP_NOMOVE);;
              hWndD2 = hWndD ;
			}




	static	BOOL CALLBACK DlgProc2(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
		
	static MINMAXINFO *pInfo ;

		switch(msg)
		{	

		  case WM_INITDIALOG:     
			  return TRUE;

		  case WM_LBUTTONDOWN: 			 	 
			  break;

	     case WM_GETMINMAXINFO:  {

            pInfo = (MINMAXINFO *)lp;  
		    POINT Min = { 200, 140 };
		    pInfo->ptMinTrackSize = Min;			
		    pInfo->ptMaxTrackSize = Min;
            return 0;

			 }
		
		 case WM_COMMAND : 
				   
				  if(LOWORD(wp) == IDC_BUTTON2){
				ShowWindow(FindWindow(0,L"Пятнашки"),SW_SHOW);
				SendMessage(FindWindow(0,L"Пятнашки"),WM_GAME_START,0,0);
					  }
				
				  if(LOWORD(wp) == 1 )
				SendMessage(hWndD2,9897,0,0);				

				 
				  if(LOWORD(wp) == IDC_BUTTON1 )					 
			   SendMessage(hWndD2, 9890,0,0);
					  
				 
			 break;
			   
		 

		


		

		  case WM_CLOSE: 

			//  delete pInfo; 
			  break;  
		}

	return FALSE;
	}


		~ToolsWindow(void);
	};

