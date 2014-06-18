#ifndef o
#define o
class Desktop
	{
	public:
		Desktop();
		~Desktop();
		static	int get_max_Width()
		{
			static int nScreenWidth = GetSystemMetrics(SM_CXSCREEN);        
		};
		static int get_max_Height()
	{
	static int nScreenHeight = GetSystemMetrics(SM_CYSCREEN);
	}
	static  void CaptureScreen( int x,int y, int w,int h,LPCTSTR lpszFileName);
	
	};

#endif 