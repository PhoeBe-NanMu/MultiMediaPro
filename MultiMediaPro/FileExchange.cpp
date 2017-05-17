#include "stdio.h" 
#include "windows.h" 
#include <time.h>
#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <cv.h>
#include <highgui.h>
#pragma comment (lib,"cv.lib")
#pragma comment (lib,"highgui.lib")
#pragma comment (lib,"cxcore.lib")


using namespace std;

int LoadConfig(char *fname,char src_path[],char dst_path[],int& jump,double& scale_S)
{
	FILE *fp=fopen(fname,"r");
	if(fp)
	{
		char jump_char[MAX_PATH];
		char scale_char[MAX_PATH];
		char buf[256];
		fscanf(fp,"%s\n",buf);
		fscanf(fp,"%s\n",src_path);
		fscanf(fp,"%s\n",buf);
		fscanf(fp,"%s\n",dst_path);
		fscanf(fp,"%s\n",buf);
		fscanf(fp,"%d\n",&jump);
		fscanf(fp,"%s\n",buf);
		fscanf(fp,"%lf\n",&scale_S);

		fclose(fp);
		return 1;
	}
	else
	{
		//添加配置文件不存在的操作
		return 0;
	}

}

int GetLevel(char *lpPath,char * src_path)
{
	char temp[MAX_PATH];
	strcpy(temp,src_path);
	strcat(temp,"\\");
	int length_src=strlen(temp);
	char level[MAX_PATH];
	strcpy(level,&lpPath[length_src]);
	return atoi(level);
}

void find_and_process(char * lpPath, char * src_path, char * dst_path,double scale_S) 
{ 
	// 图片大小
	CvSize size;size.height=256;size.width=256;
	//默认颜色赋为绿色b-g-r(0,255,0)
	//CvScalar scalar=cvScalar(0,255,0);
	// 定义仿射变换矩阵
	CvMat mat1;	float vals1[]={0.5,0,0,0,0.5,0};
	cvInitMatHeader(&mat1,2,3,CV_32FC1,vals1);
	CvMat mat2;	float vals2[]={0.5,0,0,0,0.5,128};
	cvInitMatHeader(&mat2,2,3,CV_32FC1,vals2);
	CvMat mat3;	float vals3[]={0.5,0,128,0,0.5,0};
	cvInitMatHeader(&mat3,2,3,CV_32FC1,vals3);
	CvMat mat4;	float vals4[]={0.5,0,128,0,0.5,128};
	cvInitMatHeader(&mat4,2,3,CV_32FC1,vals4);
    
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
				find_and_process(szFile,src_path,dst_path,scale_S); 
			} 
		} 
		else 
		{   // 找到最末级文件夹路径和原图片名

			char *perpath0 = (char *)malloc(MAX_PATH);
			strcpy(perpath0,lpPath);			
			char *pic0 = (char *)malloc(MAX_PATH);
			strcpy(pic0,FindFileData.cFileName); 

            // 分割，得到原始图片列号（文件夹）、行号（图片名）
            char perpath1[MAX_PATH];// 前段路径,值为".\\10-17\\maps\\层数"
	        char perpath2[MAX_PATH];// 最末文件夹名，列号
            _splitpath(perpath0, NULL, perpath1, perpath2, NULL);
		
            char picname[MAX_PATH];// 原图片名，行号.jpg
	        char ext[MAX_PATH];// 扩展名，.jpg
            _splitpath(pic0, NULL, NULL, picname, ext);	
		
			WIN32_FIND_DATA fdt;

			//目标图片层数
			int level=GetLevel(perpath1,src_path)-1;
			//此处应添加判断层数的代码，若层数为最高层，则不做操作，继续遍历文件夹//
		//	cout<<level<<endl;system("PAUSE");
			
			//目标图片路径：.\new\层数
			char nowpath_level[MAX_PATH];itoa(level,nowpath_level,10);
			char nowpath1[MAX_PATH];
			strcpy(nowpath1,dst_path);
			strcat(nowpath1,nowpath_level);
		
			//.\new\层数 是否存在，不存在则建立
			HANDLE hFile_level=FindFirstFile(nowpath1, &fdt);
			if (hFile_level== INVALID_HANDLE_VALUE)
				CreateDirectory(nowpath1,NULL);
			else
			{};

			//目标图片所在列号（文件夹）
			int c=atoi(perpath2);int r=atoi(picname);

			char nowpath_file[MAX_PATH];
			char nowpath_2[MAX_PATH];
			char nowpath_pic[MAX_PATH];
			strcpy(nowpath_file,nowpath1);
			strcat(nowpath_file,"\\");
			strcat(nowpath_file,itoa(c/2,nowpath_2,10));
			HANDLE hFile1=FindFirstFile(nowpath_file,&fdt);
			//目标图片所在列（文件夹）不存在则创建
			if(hFile1==INVALID_HANDLE_VALUE)
				CreateDirectory(nowpath_file,NULL);
			else 
			{};

			//目标图片行号（文件名）
			strcat(nowpath_file,"\\");
			strcat(nowpath_file,itoa(r/2,nowpath_pic,10));
			strcat(nowpath_file,ext);
			//cout<<nowpath_file<<endl;system("PAUSE");
			HANDLE hFile_dst=FindFirstFile(nowpath_file,&fdt);

			char perpath[MAX_PATH];
			strcpy(perpath,".\\10-17\\maps\\");
			strcat(perpath,nowpath_level);
			strcat(perpath,"\\");
			strcat(perpath,itoa(c/2,nowpath_2,10));
			strcat(perpath,"\\");
			strcat(perpath,itoa(r/2,nowpath_pic,10));
			strcat(perpath,ext);

			if(level==15 && c/2==26756 && r/2==14276)
			{ int yy=0;
			}

			HANDLE hFile_src=FindFirstFile(perpath,&fdt);		
			//目标图片不存在则自行创建
			IplImage* img_aff;
			if((hFile_dst==INVALID_HANDLE_VALUE)&&(hFile_src!=INVALID_HANDLE_VALUE))
			{
				img_aff=cvLoadImage(perpath);
				//cvSet(img_aff,scalar,NULL);
			}
			else if((hFile_dst==INVALID_HANDLE_VALUE)&&(hFile_src==INVALID_HANDLE_VALUE))
				img_aff=cvCreateImage(size,8,3);
			else
				img_aff=cvLoadImage(nowpath_file);
			
			char perpath_file[MAX_PATH];
			strcpy(perpath_file,perpath0);
			strcat(perpath_file,"\\");
			strcat(perpath_file,pic0);
			IplImage* img=cvLoadImage(perpath_file);

/*			IplImage* floatimg = NULL;
			IplImage* floathsv = NULL;
			IplImage* floatimgH = NULL;
			IplImage* floatimgS = NULL;
			IplImage* floatimgV = NULL;
			IplImage* floatimgS_scale=NULL;
			IplImage* floatdst_hsv=NULL;
			IplImage* floatdst_bgr=NULL;
			IplImage* dst=NULL;

			//先将图像转换成float型的
			floatimg = cvCreateImage( size, IPL_DEPTH_32F, 3 );
			floathsv = cvCreateImage( size, IPL_DEPTH_32F, 3 );
			floatimgH = cvCreateImage( size, IPL_DEPTH_32F, 1 );
			floatimgS = cvCreateImage( size, IPL_DEPTH_32F, 1 );
			floatimgV = cvCreateImage( size, IPL_DEPTH_32F, 1 );
			floatimgS_scale=cvCreateImage( size, IPL_DEPTH_32F, 1 );
			floatdst_hsv=cvCreateImage(size,IPL_DEPTH_32F,3);
			floatdst_bgr=cvCreateImage(size,IPL_DEPTH_32F,3);
			dst=cvCreateImage( size, 8, 3 );

			//将src从8位转换到32位的float型
			cvConvertScale( img, floatimg, 1.0/255.0, 0 );//归一化之后方能够显示
			//cvConvertScale( src, floatsrc, 1, 0 );

			//将float型图像 从BGR转换到HSV  如果需要转换到其他的颜色空间 那么改变CV_BGR2HSV即可
			//cvCvtColor要求两个参数的类型必须完全相同，所以要转为float型
			cvCvtColor( floatimg, floathsv, CV_BGR2HSV );

			//将三通道图像 分解成3个单通道图像，H对应的通道时0，S、V对应的通道时1和2
			//cvCvtPixToPlane(picHSV, h_plane, s_plane, v_plane, 0);
			cvSplit( floathsv, floatimgH, floatimgS, floatimgV, NULL);
			cvScale(floatimgS,floatimgS_scale,scale_S);
		//	cvNamedWindow("1",1);cvShowImage("1",floatimgS_scale);
		//	cvWaitKey(0);cvDestroyWindow("1");
		//	system("PAUSE");
		
			cvMerge(floatimgH,floatimgS_scale,floatimgV,NULL,floatdst_hsv);
			cvCvtColor( floatdst_hsv, floatdst_bgr, CV_HSV2BGR );
			cvConvertScale( floatdst_bgr, dst, 255.0, 0 );
		//	cvNamedWindow("1",1);cvShowImage("1",dst);
		//	cvWaitKey(0);cvDestroyWindow("1");
		//	system("PAUSE");
		
			cvReleaseImage( &img );
			cvReleaseImage( &floatimg );
			cvReleaseImage( &floathsv );
			cvReleaseImage( &floatimgH );
			cvReleaseImage( &floatimgS );
			cvReleaseImage( &floatimgV );
			cvReleaseImage( &floatimgS_scale );
			cvReleaseImage( &floatdst_bgr);
			cvReleaseImage( &floatdst_hsv);
*/
	
			//拼合1/4角
			if(c==c/2*2&&r==r/2*2)
				cvWarpAffine(img,img_aff,&mat1,CV_INTER_LINEAR);
			if(c==c/2*2&&r==r/2*2+1)
				cvWarpAffine(img,img_aff,&mat2,CV_INTER_LINEAR);
			if(c==c/2*2+1&&r==r/2*2)
				cvWarpAffine(img,img_aff,&mat3,CV_INTER_LINEAR);
			if(c==c/2*2+1&&r==r/2*2+1)
				cvWarpAffine(img,img_aff,&mat4,CV_INTER_LINEAR);
		//	cvSaveImage(nowpath_file,img_aff);

			cvSaveImage(nowpath_file,img_aff);

			cvReleaseImage( &img );
			cvReleaseImage(&img_aff);


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

	char src_path[MAX_PATH];
	char dst_path[MAX_PATH];
	int jump;
	double  scale_S;
	char config_path[MAX_PATH]=".\\config.txt";
	int flag=LoadConfig(config_path,src_path,dst_path,jump,scale_S);
	cout<<src_path<<endl;
	cout<<dst_path<<endl;
	cout<<jump<<endl;
	cout<<scale_S<<endl;
	system("PAUSE");

	find_and_process(src_path,src_path,dst_path,scale_S);
	
	// 结束时间
	time ( &rawtime ); 
    timeinfo = localtime ( &rawtime ); 
	printf ( "\007The current date/time is: %s", asctime (timeinfo) );
	
	system("PAUSE");	
}
