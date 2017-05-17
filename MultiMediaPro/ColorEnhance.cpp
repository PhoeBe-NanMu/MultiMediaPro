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
			{   // �ݹ��������ļ�
				strcpy(szFile,lpPath); 
				strcat(szFile,"\\");  
				strcat(szFile,FindFileData.cFileName); 
				find_and_enhance(szFile); 
			} 
		} 
		else 
		{   // �ҵ���ĩ���ļ���·����ԭͼƬ��

			char *perpath0 = (char *)malloc(MAX_PATH);
			strcpy(perpath0,lpPath);			
			char *pic0 = (char *)malloc(MAX_PATH);
			strcpy(pic0,FindFileData.cFileName); 
			//cout<<perpath0<<"....."<<pic0<<endl;system("PAUSE");

            // �ָ�õ�ԭʼͼƬ�кţ��ļ��У����кţ�ͼƬ����
            char perpath1[MAX_PATH];// ǰ��·��,ֵΪ".\\10-17\\maps\\����"
	        char perpath2[MAX_PATH];// ��ĩ�ļ��������к�
            _splitpath(perpath0, NULL, perpath1, perpath2, NULL);
		
            char picname[MAX_PATH];// ԭͼƬ�����к�.jpg
	        char ext[MAX_PATH];// ��չ����.jpg
            _splitpath(pic0, NULL, NULL, picname, ext);	
		

			//ͼƬ����
			int level=GetLevel(perpath1,".\\10-17\\maps\\");

			//Դ·��
			char srcpath[MAX_PATH];
			strcpy(srcpath,perpath0);
			strcat(srcpath,"\\");
			strcat(srcpath,pic0);
			
			//Ŀ��ͼƬ��·��
			char dstpath_level[MAX_PATH];
			char level_char[MAX_PATH];
			itoa(level,level_char,10);
			strcpy(dstpath_level,".\\new\\");
			strcat(dstpath_level,level_char);

			//Ŀ��ͼƬ��·��
			char dstpath_col[MAX_PATH];
			strcpy(dstpath_col,dstpath_level);
			strcat(dstpath_col,"\\");
			strcat(dstpath_col,perpath2);
			
			//Ŀ��ͼƬ�ļ�·��
			char dstpath[MAX_PATH];
			strcpy(dstpath,dstpath_col);
			strcat(dstpath,"\\");
			strcat(dstpath,pic0);
		
			//cout<<srcpath<<"..."<<dstpath_level<<"..."<<dstpath_col<<"..."<<dstpath<<endl;system("PAUSE");

			WIN32_FIND_DATA fdt;
			//Ŀ��ͼƬ��·��.\new\level �Ƿ���ڣ�����������
			HANDLE hFile_level=FindFirstFile(dstpath_level, &fdt);
			if (hFile_level== INVALID_HANDLE_VALUE)
				CreateDirectory(dstpath_level,NULL);
			else
			{};

			//Ŀ��ͼƬ��·��.\new\level\col �Ƿ���ڣ�����������
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
			//�Ƚ�ͼ��ת����float�͵�
			floatsrc = cvCreateImage( size, IPL_DEPTH_32F, 3 );
			floathsv = cvCreateImage( size, IPL_DEPTH_32F, 3 );
			floatimgH = cvCreateImage( size, IPL_DEPTH_32F, 1 );
			floatimgS = cvCreateImage( size, IPL_DEPTH_32F, 1 );
			floatimgV = cvCreateImage( size, IPL_DEPTH_32F, 1 );
			floatimgS_scale=cvCreateImage( size, IPL_DEPTH_32F, 1 );
			floatdst_hsv=cvCreateImage(size,IPL_DEPTH_32F,3);
			floatdst_bgr=cvCreateImage(size,IPL_DEPTH_32F,3);
			dst=cvCreateImage( size, 8, 3 );

			//��src��8λת����32λ��float��
			cvConvertScale( src, floatsrc, 1.0/255.0, 0 );//��һ��֮���ܹ���ʾ
			//cvConvertScale( src, floatsrc, 1, 0 );

			//��float��ͼ�� ��BGRת����HSV  �����Ҫת������������ɫ�ռ� ��ô�ı�CV_BGR2HSV����
			//cvCvtColorҪ���������������ͱ�����ȫ��ͬ������ҪתΪfloat��
			cvCvtColor( floatsrc, floathsv, CV_BGR2HSV );

			//����ͨ��ͼ�� �ֽ��3����ͨ��ͼ��H��Ӧ��ͨ��ʱ0��S��V��Ӧ��ͨ��ʱ1��2
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
	// ��ʼʱ��
    time ( &rawtime ); 
    timeinfo = localtime ( &rawtime ); 
    printf ( "\007The current date/time is: %s", asctime (timeinfo) ); 

	char filepath[MAX_PATH]=".\\10-17\\maps";
	find_and_enhance(filepath);
	
	// ����ʱ��
	time ( &rawtime ); 
    timeinfo = localtime ( &rawtime ); 
	printf ( "\007The current date/time is: %s", asctime (timeinfo) );
	
	system("PAUSE");	
}