#include "public.h"


//矩形相交
bool MyIntersectRect(ME_Rect* rect1,ME_Rect* rect2)
{
	bool zs  = MyPtInRect(rect1,rect2->left,rect2->top);
	bool zx  = MyPtInRect(rect1,rect2->left,rect2->bottom);
	bool ys  = MyPtInRect(rect1,rect2->right,rect2->top);
	bool yx  = MyPtInRect(rect1,rect2->right,rect2->bottom);
	bool zs1 = MyPtInRect(rect2,rect1->left,rect1->top);
	bool zx1 = MyPtInRect(rect2,rect1->left,rect1->bottom);
	bool ys1 = MyPtInRect(rect2,rect1->right,rect1->top);
	bool yx1 = MyPtInRect(rect2,rect1->right,rect1->bottom);

	if( zs || zx || ys || yx || zs1 || zx1 || ys1 || yx1 )
		return true;

	return false;
}
//判断点在矩形
bool MyPtInRect(ME_Rect* rect,int x,int y)
{
	if( x >= rect->left && x <= rect->right &&
		y <= rect->top && y >= rect->bottom )
		return true;

	return false;
}

//中文转换
int GBKToUTF8(std::string &gbkStr,const char* toCode,const char* formCode)
{
     iconv_t iconvH;
     iconvH = iconv_open(formCode,toCode);
     if(iconvH == 0)
	 {
         return -1;
     }
     const char* strChar = gbkStr.c_str();
     const char** pin = &strChar;
     size_t strLength = gbkStr.length();
     char *outbuf = (char*) malloc(strLength*4);
     char *pBuff = outbuf;
     memset(outbuf,0,strLength*4);
     size_t outLength = strLength*4;
     if(-1 == iconv(iconvH,pin,&strLength,&outbuf,&outLength)){
         iconv_close(iconvH);
         return -1;
     }
     gbkStr = pBuff;
     iconv_close(iconvH);
     return 0;
}

//加载一个文件下所有的音乐文件
int LoadDirectory(const char* fn)
{
	int r = 0;
	string s1 = fn;
	s1 += "\\*.*";
	_finddata_t fd;
	intptr_t fr = _findfirst(s1.c_str(),&fd);

	if( -1 != fr )
	{
		do
		{
			//子目录
			if( fd.attrib & _A_SUBDIR )
			{
				if( strcmp(fd.name,".") != 0 && strcmp(fd.name,"..") != 0 )
				{
					string s2 = fn;
					s2 += "\\";
					s2 += fd.name;
					r += LoadDirectory(s2.c_str());
				}
			}
			else//文件
			{
				//检查是否为音乐文件
				char* p = strrchr(fd.name,'.');
				if( p )
				{
					//得到后缀名
					string s3 = p + 1;
					//将所有大小转换为小写
					for( unsigned int i = 0; i < s3.length(); ++i )
					{
						if( s3[i] >= 'A' && s3[i] <= 'Z' )
							s3[i] += ('a' - 'A');
					}
					//确认是音乐文件
					if(    strcmp(s3.c_str(),"mp3") == 0
						|| strcmp(s3.c_str(),"wma") == 0
						|| strcmp(s3.c_str(),"wav") == 0
						|| strcmp(s3.c_str(),"mod") == 0
						|| strcmp(s3.c_str(),"ra") == 0
						|| strcmp(s3.c_str(),"cd") == 0
						|| strcmp(s3.c_str(),"md") == 0
						|| strcmp(s3.c_str(),"asf") == 0
						|| strcmp(s3.c_str(),"aac") == 0
						|| strcmp(s3.c_str(),"mp3pro") == 0
						|| strcmp(s3.c_str(),"vof") == 0
						|| strcmp(s3.c_str(),"flac") == 0
						|| strcmp(s3.c_str(),"ape") == 0
						|| strcmp(s3.c_str(),"mid") == 0
						|| strcmp(s3.c_str(),"ogg") == 0
						|| strcmp(s3.c_str(),"m4a") == 0
						|| strcmp(s3.c_str(),"aac+") == 0
						|| strcmp(s3.c_str(),"aiff") == 0
						|| strcmp(s3.c_str(),"vqf") == 0)
					{
						string s4 = fn;
						s4 += "\\";
						s4 += fd.name;
						SimpleAudioEngine::getInstance()->preloadEffect(s4.c_str());
						r += 1;
					}
				}	
			}
		}while( 0 == _findnext(fr,&fd));
		_findclose(fr);
	}

	return r;
}