#include "stdio.h" 
#include "windows.h" 
#include <time.h>
#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <cv.h>
#include <highgui.h>
#include <cxcore.h>
#pragma comment (lib,"cv.lib")
#pragma comment (lib,"highgui.lib")
#pragma comment (lib,"cxcore.lib")

using namespace std;

int GetLevel(char a[],char b[])
{
	int length_b=strlen(b);
	char c[MAX_PATH];
	strcpy(c,&a[length_b]);
	return atoi(c);
}

void find_and_enhance(char * lpPath) 
{   
	char szFind[MAX_PATH],szFile[MAX_PATH]; 
    WIN32_FIND_DATA FindFileData; 
	strcpy(szFind,lpPath); 
    strcat(szFind,"\\*.*");
    HANDLE hFind=::FindFirstFile(szFind,&FindFileData); 
    if(INVALID_HANDLE_VALUE == hFind) 
        return;
    while(TRUE) 
	{ 
		if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) 
		{ 
			if(FindFileData.cFileName[0]!='.') 
			{   // 递归找所有文件
				strcpy(szFile,lpPath); 
				strcat(szFile,"\\");  
				strcat(szFile,FindFileData.cFileName); 
				find_and_enhance(szFile); 
			} 
		} 
		else 
		{   // 找到最末级文件夹路径和原图片名

			char *perpath0 = (char *)malloc(MAX_PATH);
			strcpy(perpath0,lpPath);			
			char *pic0 = (char *)malloc(MAX_PATH);
			strcpy(pic0,FindFileData.cFileName); 
			//cout<<perpath0<<"....."<<pic0<<endl;system("PAUSE");

            // 分割，得到原始图片列号（文件夹）、行号（图片名）
            char perpath1[MAX_PATH];// 前段路径,值为".\\10-17\\maps\\层数"
	        char perpath2[MAX_PATH];// 最末文件夹名，列号
            _splitpath(perpath0, NULL, perpath1, perpath2, NULL);
		
            char picname[MAX_PATH];// 原图片名，行号.jpg
	        char ext[MAX_PATH];// 扩展名，.jpg
            _splitpath(pic0, NULL, NULL, picname, ext);	
		

			//图片层数
			int level=GetLevel(perpath1,".\\10-17\\maps\\");

			//源路径
			char srcpath[MAX_PATH];
			strcpy(srcpath,perpath0);
			strcat(srcpath,"\\");
			strcat(srcpath,pic0);
			
			//目标图片层路径
			char dstpath_level[MAX_PATH];
			char level_char[MAX_PATH];
			itoa(level,level_char,10);
			strcpy(dstpath_level,".\\new\\");
			strcat(dstpath_level,level_char);

			//目标图片列路径
			char dstpath_col[MAX_PATH];
			strcpy(dstpath_col,dstpath_level);
			strcat(dstpath_col,"\\");
			strcat(dstpath_col,perpath2);
			
			//目标图片文件路径
			char dstpath[MAX_PATH];
			strcpy(dstpath,dstpath_col);
			strcat(dstpath,"\\");
			strcat(dstpath,pic0);
		
			//cout<<srcpath<<"..."<<dstpath_level<<"..."<<dstpath_col<<"..."<<dstpath<<endl;system("PAUSE");

			WIN32_FIND_DATA fdt;
			//目标图片层路径.\new\level 是否存在，不存在则建立
			HANDLE hFile_level=FindFirstFile(dstpath_level, &fdt);
			if (hFile_level== INVALID_HANDLE_VALUE)
				CreateDirectory(dstpath_level,NULL);
			else
			{};

			//目标图片列路径.\new\level\col 是否存在，不存在则建立
			HANDLE hFile_col=FindFirstFile(dstpath_col,&fdt);
			if(hFile_col==INVALID_HANDLE_VALUE)
				CreateDirectory(dstpath_col,NULL);
			else 
			{};
		
			IplImage* src = NULL;
			IplImage* floatsrc = NULL;
			IplImage* floathsv = NULL;
			IplImage* floatimgH = NULL;
			IplImage* floatimgS = NULL;
			IplImage* floatimgV = NULL;
			IplImage* floatimgS_scale=NULL;
			IplImage* floatdst_hsv=NULL;
			IplImage* floatdst_bgr=NULL;
			IplImage* dst=NULL;

			src = cvLoadImage( srcpath);

			CvSize size=cvGetSize(src);
			//先将图像转换成float型的
			floatsrc = cvCreateImage( size, IPL_DEPTH_32F, 3 );
			floathsv = cvCreateImage( size, IPL_DEPTH_32F, 3 );
			floatimgH = cvCreateImage( size, IPL_DEPTH_32F, 1 );
			floatimgS = cvCreateImage( size, IPL_DEPTH_32F, 1 );
			floatimgV = cvCreateImage( size, IPL_DEPTH_32F, 1 );
			floatimgS_scale=cvCreateImage( size, IPL_DEPTH_32F, 1 );
			floatdst_hsv=cvCreateImage(size,IPL_DEPTH_32F,3);
			floatdst_bgr=cvCreateImage(size,IPL_DEPTH_32F,3);
			dst=cvCreateImage( size, 8, 3 );

			//将src从8位转换到32位的float型
			cvConvertScale( src, floatsrc, 1.0/255.0, 0 );//归一化之后方能够显示
			//cvConvertScale( src, floatsrc, 1, 0 );

			//将float型图像 从BGR转换到HSV  如果需要转换到其他的颜色空间 那么改变CV_BGR2HSV即可
			//cvCvtColor要求两个参数的类型必须完全相同，所以要转为float型
			cvCvtColor( floatsrc, floathsv, CV_BGR2HSV );

			//将三通道图像 分解成3个单通道图像，H对应的通道时0，S、V对应的通道时1和2
			//cvCvtPixToPlane(picHSV, h_plane, s_plane, v_plane, 0);
			cvSplit( floathsv, floatimgH, floatimgS, floatimgV, NULL);
			double scale=2;
			cvScale(floatimgS,floatimgS_scale,scale);
		/*	cvNamedWindow("1",1);cvShowImage("1",floatimgS_scale);
			cvWaitKey(0);cvDestroyWindow("1");
			system("PAUSE");
		*/
			cvMerge(floatimgH,floatimgS_scale,floatimgV,NULL,floatdst_hsv);
			cvCvtColor( floatdst_hsv, floatdst_bgr, CV_HSV2BGR );
			cvConvertScale( floatdst_bgr, dst, 255.0, 0 );
		/*	cvNamedWindow("1",1);cvShowImage("1",dst);
			cvWaitKey(0);cvDestroyWindow("1");
			system("PAUSE");
		*/
			cvSaveImage(dstpath,dst);

			cvReleaseImage( &src );
			cvReleaseImage( &floatsrc );
			cvReleaseImage( &floathsv );
			cvReleaseImage( &floatimgH );
			cvReleaseImage( &floatimgS );
			cvReleaseImage( &floatimgV );
			cvReleaseImage( &floatimgS_scale );
			cvReleaseImage( &floatdst_bgr);
			cvReleaseImage( &floatdst_hsv);
			cvReleaseImage( &dst );
			
			free(perpath0);
			free(pic0);
		} 
		if(!FindNextFile(hFind,&FindFileData)) 
			break; 
	} 
} 

void main() 
{ 
    time_t rawtime; 
    struct tm * timeinfo; 
	// 开始时间
    time ( &rawtime ); 
    timeinfo = localtime ( &rawtime ); 
    printf ( "\007The current date/time is: %s", asctime (timeinfo) ); 

	char filepath[MAX_PATH]=".\\10-17\\maps";
	find_and_enhance(filepath);
	
	// 结束时间
	time ( &rawtime ); 
    timeinfo = localtime ( &rawtime ); 
	printf ( "\007The current date/time is: %s", asctime (timeinfo) );
	
	system("PAUSE");	
}