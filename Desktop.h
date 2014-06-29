#ifndef ____o_____
#define ____o_____

class Desktop
	{
	public:

		Desktop();
		~Desktop();

		static void change_style(HWND hWnd)
	     {

	       long style = GetWindowLong(hWnd,GWL_STYLE);
	       style = style ^  WS_SIZEBOX;
	       SetWindowLong(hWnd,GWL_STYLE,style);

	     };

		static	int get_max_Width()
		{
			static int nScreenWidth = GetSystemMetrics(SM_CXSCREEN);        
		};

		static int get_max_Height()
		{
	        static int nScreenHeight = GetSystemMetrics(SM_CYSCREEN);
	    };

	   static  void CaptureScreen(
		   int x,
		   int y,
		   int w,
		   int h,
		   bool &
		   );
	
	};

#endif 