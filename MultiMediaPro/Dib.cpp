// MyDib.cpp: implementation of the CDib class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Dib.h"

#include <math.h>

//在计算图象大小时，采用的公式：biSizeImage = biWidth' * biHeight.
//是biWidth`,而不是biWidth,这里的biWidth`必须是4的整倍数，表示
//大于或等于biWidth的，离4最近的整倍数。WIDTHBYTES就是用来计算
//biWidth`的。
#define WIDTHBYTES(bits) ((bits+31)/32*4)
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/*
* Dib文件头标志（字符串"BM"）
*/
#define DIB_MARKER   ((WORD) ('M' << 8) | 'B') // 用于判断位图的标志宏 
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CDib::CDib()
{
	InitMembers();
}

CDib::~CDib()
{
	ClearMemory();
}

/*************************************************************************
* 函数名称：DrawDib()
* 参数说明:
*   HDC hDC，输出设备DC
*   LPRECT lpDCRect，绘制矩形区域
*   HGLOBAL hDIB ，DIB对象的句柄
*   LPRECT lpDIBRect ，DIB的输出区域
*   CPalette* pPal，调色板的指针
* 函数类型:BOOL
* 函数功能:该函数主要用来绘制DIB对象   
************************************************************************/

BOOL CDib::DrawDib(HDC     hDC,
				   LPRECT  lpDCRect,
				   HGLOBAL    hDIB,
				   LPRECT  lpDIBRect,
				   CPalette* pPal)
{
	BOOL     bSuccess=FALSE;      // 重画成功标志
	HPALETTE hOldPal=NULL;        // 以前的调色板	
	if (hDIB == NULL)// 判断是否是有效的DIB对象
	{		
		return FALSE;// 不是，则返回
	}	
	lpdib  = (LPSTR) ::GlobalLock((HGLOBAL) hDIB);// 锁定DIB	
	lpDIBBits = GetBits(lpdib);// 找到DIB图像像素起始位置	
	if (pPal != NULL)// 获取DIB调色板，并选取到设备环境中
	{
		HPALETTE hPal = (HPALETTE) pPal->m_hObject;	
		hOldPal = ::SelectPalette(hDC, hPal, TRUE);
	}	
	::SetStretchBltMode(hDC, COLORONCOLOR);// 设置显示模式		
	bSuccess = ::StretchDIBits(hDC,								// 设备环境句柄
		lpDCRect->left,                 //目标X坐标
		lpDCRect->top,                  // 目标Y坐标
		GetRectWidth(lpDCRect),            // 目标宽度
		GetRectHeight(lpDCRect),           // 目标高度
		lpDIBRect->left,                // 源X坐标
		lpDIBRect->top,                 // 源Y坐标
		GetRectWidth(lpDIBRect),           // 源宽度
		GetRectHeight(lpDIBRect),          // 源高度
		lpDIBBits,                      // 指向dib像素的指针
		(LPBITMAPINFO)lpdib,         // 指向位图信息结构的指针
		DIB_RGB_COLORS,                 // 使用的颜色数目
		SRCCOPY);                       // 光栅操作类型	
	::GlobalUnlock(hDIB);	// 解除锁定
	if (hOldPal != NULL)// 恢复系统调色板
	{
		::SelectPalette(hDC, hOldPal, TRUE);
	}	
	return bSuccess;
}


/*************************************************************************
* 函数名称：ConstructPalette(HGLOBAL hDIB, CPalette* pPal)
* 函数参数:
*   HGLOBAL hDIB，DIB对象的句柄
*   CPalette* pPal，调色板的指针
* 函数类型:BOOL
* 函数说明:该函数按照DIB创建一个逻辑调色板  
************************************************************************/

BOOL CDib::ConstructPalette(HGLOBAL hDIB, CPalette* pPal)
{	

	HANDLE hLogPal;// 逻辑调色板的句柄	
	int iLoop;// 循环变量	
	BOOL bSuccess = FALSE;// 创建结果	
	if (hDIB == NULL)//判断是否是有效的DIB对象
	{		
		return FALSE;// 返回FALSE
	}	
	lpdib = (LPSTR) ::GlobalLock((HGLOBAL) hDIB);// 锁定DIB
	lpbminfo= (LPBITMAPINFO)lpdib;			
	long wNumColors =GetColorNum(lpdib);// 获取DIB中颜色表中的颜色数目	
	if (wNumColors != 0)
	{		
		hLogPal = ::GlobalAlloc(GHND, sizeof(LOGPALETTE)// 分配为逻辑调色板内存
			+ sizeof(PALETTEENTRY)
			* wNumColors);				
		if (hLogPal == 0)// 如果失败则退出
		{			
			::GlobalUnlock((HGLOBAL) hDIB);// 解除锁定			
			return FALSE;
		}		
		LPLOGPALETTE lpPal = (LPLOGPALETTE) ::GlobalLock((HGLOBAL) hLogPal);		

		lpPal->palVersion = PalVersion;// 设置调色板版本号				
		lpPal->palNumEntries = (WORD)wNumColors;// 设置颜色数目		
		for (iLoop=0; iLoop<(int)wNumColors;iLoop++)// 读取调色板
		{			
			lpPal->palPalEntry[iLoop].peRed =lpbminfo->bmiColors[iLoop].rgbRed;// 读取三原色分量						
			lpPal->palPalEntry[iLoop].peGreen =lpbminfo->bmiColors[iLoop].rgbGreen;							
			lpPal->palPalEntry[iLoop].peBlue =lpbminfo->bmiColors[iLoop].rgbBlue;								
			lpPal->palPalEntry[iLoop].peFlags =0;// 保留位			
		}		
		bSuccess=pPal->CreatePalette(lpPal);// 按照逻辑调色板创建调色板，并返回指针			
		::GlobalUnlock((HGLOBAL) hLogPal);// 解除锁定				
		::GlobalFree((HGLOBAL) hLogPal);// 释放逻辑调色板
	}		
	::GlobalUnlock((HGLOBAL) hDIB);// 解除锁定		
	return bSuccess;// 返回结果
}

/*************************************************************************
* 函数名称：GetBits(LPSTR lpdib)
* 函数参数:
*   LPSTR lpdib，指向DIB对象的指针
* 函数类型:LPSTR 
* 函数功能:计算DIB像素的起始位置，并返回指向它的指针
************************************************************************/

LPSTR CDib::GetBits(LPSTR lpdib)
{
	return (lpdib + ((LPBITMAPINFOHEADER)lpdib)->biSize+GetPalSize(lpdib));
	//	return (lpdib + *(LPDWORD)lpdib+GetPalSize(lpdib));
}

/*************************************************************************
* 函数名称：GetWidth(LPSTR lpdib)
* 函数参数:
*   LPSTR lpdib，指向DIB对象的指针
* 函数类型:DWORD
* 函数功能:该函数返回DIB中图像的宽度
************************************************************************/

DWORD CDib::GetWidth(LPSTR lpdib)
{
	return ((LPBITMAPINFOHEADER)lpdib)->biWidth;//返回DIB宽度
}


/*************************************************************************
* 函数名称：GetHeight(LPSTR lpdib)
* 函数参数:
*   LPSTR lpdib ，指向DIB对象的指针
* 函数类型:DWORD
* 函数功能:该函数返回DIB中图像的高度
************************************************************************/


DWORD CDib::GetHeight(LPSTR lpdib)
{
	return ((LPBITMAPINFOHEADER)lpdib)->biHeight;//返回DIB高度

}
/*************************************************************************
* 函数名称：GetPalSize(LPSTR lpdib)
* 函数参数:
*   LPSTR lpdib，指向DIB对象的指针
* 函数类型:WORD
* 函数功能:该函数返回DIB中调色板的大小
************************************************************************/

WORD CDib::GetPalSize(LPSTR lpdib)
{	
	return (WORD)(GetColorNum(lpdib) * sizeof(RGBQUAD));// 计算DIB中调色板的大小
}

/*************************************************************************
* 函数名称：GetColorNum(LPSTR lpdib)
* 函数参数:
*   LPSTR lpdib，指向DIB对象的指针
* 函数类型:WORD
* 函数功能:该函数返回DIB中调色板的颜色的种数
************************************************************************/
WORD CDib::GetColorNum(LPSTR lpdib)
{				
	long dwClrUsed = ((LPBITMAPINFOHEADER)lpdib)->biClrUsed;	// 读取dwClrUsed值	
	if (dwClrUsed != 0)
	{			
		return (WORD)dwClrUsed;// 如果dwClrUsed不为0，直接返回该值
	}		
	WORD wBitCount = ((LPBITMAPINFOHEADER)lpdib)->biBitCount;// 读取biBitCount值		
	switch (wBitCount)// 按照像素的位数计算颜色数目
	{
	case 1:
		return 2;

	case 4:
		return 16;

	case 8:
		return 256;

	default:
		return 0;
	}
}

/*************************************************************************
* 函数名称：GetBitCount(LPSTR lpdib)
* 函数参数:
*   LPSTR lpdib，指向DIB对象的指针
* 函数类型:WORD 
* 函数功能:该函数返回DIBBitCount
************************************************************************/
WORD CDib::GetBitCount(LPSTR lpdib)
{
	return ((LPBITMAPINFOHEADER)lpdib)->biBitCount;// 返回位宽
}

/*************************************************************************
* 函数名称：CopyObject (HGLOBAL hGlob)
* 函数参数:
*   HGLOBAL hGlob ，要复制的内存区域
* 函数类型:HGLOBAL
* 函数功能:该函数复制指定的内存区域
************************************************************************/

HGLOBAL CDib::CopyObject (HGLOBAL hGlob)
{
	if (hGlob== NULL)
		return NULL;	
	DWORD dwLen = ::GlobalSize((HGLOBAL)hGlob);// 获取指定内存区域大小		
	HGLOBAL hTemp = ::GlobalAlloc(GHND, dwLen);// 分配新内存空间		
	if (hTemp!= NULL)// 判断分配是否成功
	{	
		void* lpTemp = ::GlobalLock((HGLOBAL)hTemp);// 锁定
		void* lp     = ::GlobalLock((HGLOBAL) hGlob);				
		memcpy(lpTemp, lp, dwLen);// 复制		
		::GlobalUnlock(hTemp);// 解除锁定
		::GlobalUnlock(hGlob);
	}
	return hTemp;
}

/*************************************************************************
* 函数名称：SaveFile(HGLOBAL hDib, CFile& file)
* 函数参数:
*   HGLOBAL hDib，要保存的DIB
*   CFile& file，保存文件CFile
* 函数类型:BOOL
* 函数功能:将指定的DIB对象保存到指定的CFile中
*************************************************************************/

BOOL CDib::SaveFile(HGLOBAL hDib, CFile& file)
{
	if (hDib == NULL)
	{		
		return FALSE;// 如果DIB为空，返回FALSE
	}	
	lpbmihrd = (LPBITMAPINFOHEADER) ::GlobalLock((HGLOBAL) hDib);// 读取BITMAPINFO结构，并锁定	
	if (lpbmihrd == NULL)
	{		
		return FALSE;// 为空，返回FALSE
	}		
	bmfHeader.bfType = DIB_MARKER;	// 填充文件头	
	dwDIBSize = *(LPDWORD)lpbmihrd + GetPalSize((LPSTR)lpbmihrd);// 文件头大小＋颜色表大小			
	DWORD dwBmBitsSize;// 像素的大小		
	dwBmBitsSize =GetReqByteWidth((lpbmihrd->biWidth)*((DWORD)lpbmihrd->biBitCount)) * lpbmihrd->biHeight;// 大小为Width * Height		
	dwDIBSize += dwBmBitsSize;// 计算后DIB每行字节数为4的倍数时的大小
	lpbmihrd->biSizeImage = dwBmBitsSize;// 更新biSizeImage	
	bmfHeader.bfSize = dwDIBSize + sizeof(BITMAPFILEHEADER);// 文件大小		
	bmfHeader.bfReserved1 = 0;// 两个保留字
	bmfHeader.bfReserved2 = 0;
	bmfHeader.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + lpbmihrd->biSize// 计算偏移量bfOffBits
		+ GetPalSize((LPSTR)lpbmihrd);
	TRY
	{		
		file.Write(&bmfHeader, sizeof(BITMAPFILEHEADER));// 写文件头				
		file.Write(lpbmihrd, dwDIBSize);// 写DIB头和像素
	}
	CATCH (CFileException, e)
	{		
		::GlobalUnlock((HGLOBAL) hDib);// 解除锁定				
		THROW_LAST();// 抛出异常
	}
	END_CATCH		
		::GlobalUnlock((HGLOBAL) hDib);// 解除锁定	
	return TRUE;// 返回TRUE
}


/*************************************************************************
* 函数名称：LoadFile(CFile& file)
* 函数参数:
*   CFile& file，要读取得文件文件CFile
* 函数类型:HGLOBAL
* 函数功能:将指定的文件中的DIB对象读到指定的内存区域中
*************************************************************************/

HGLOBAL CDib::LoadFile(CFile& file)
{

	DWORD dwFileSize;

	dwFileSize= file.GetLength();// 获取文件大小	
	if (file.Read((LPSTR)&bmfHeader, sizeof(bmfHeader)) != sizeof(bmfHeader))// 读取DIB文件头
	{		
		return NULL;// 大小不一致，返回NULL。
	}
	if (bmfHeader.bfType != DIB_MARKER)// 判断是否是DIB对象
	{		
		return NULL;// 如果不是则返回NULL。
	}	
	m_hDib= (HGLOBAL) ::GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, dwFileSize-sizeof(BITMAPFILEHEADER));// 分配DIB内存
	if (m_hDib== 0)
	{		
		return NULL;// 分配失败，返回NULL
	}
	/////////////////////////////////////////////////////////////////////////
	//给CDib类的成员变量赋值	
	lpdib = (LPSTR) ::GlobalLock((HGLOBAL) m_hDib);// 锁定
	lpbminfo=(BITMAPINFO*)lpdib;
	lpbmihrd=(BITMAPINFOHEADER*)lpdib;
	lpRgbQuag=(RGBQUAD*)(lpdib+lpbmihrd->biSize);
	int m_numberOfColors =GetColorNum((LPSTR)lpbmihrd);
	if (lpbmihrd->biClrUsed == 0)
		lpbmihrd->biClrUsed =m_numberOfColors;
	DWORD colorTableSize = m_numberOfColors *sizeof(RGBQUAD);
	lpDIBBits=lpdib+lpbmihrd->biSize+colorTableSize;
	/////////////////////////////////////////////////////////////////////////

	if (file.Read(lpdib, dwFileSize - sizeof(BITMAPFILEHEADER)) !=// 读像素
		dwFileSize - sizeof(BITMAPFILEHEADER) )//大小不一致
	{					
		::GlobalUnlock((HGLOBAL) m_hDib);	// 解除锁定			
		::GlobalFree((HGLOBAL) m_hDib);	// 释放内存
		return NULL;
	}		
	::GlobalUnlock((HGLOBAL) m_hDib);// 解除锁定		
	return m_hDib;// 返回DIB句柄
}

/*************************************************************************
* 函数名称：GetReqByteWidth(int bits)
* 函数参数:
*   int bits，位数
* 函数类型:int
* 函数功能:获取需要的行字节数，应为4的倍数
*************************************************************************/
int  CDib::GetReqByteWidth(int bits)
{
	int getBytes=(bits + 31) / 32 * 4;
	return  getBytes;
}

/*************************************************************************
* 函数名称：GetRectWidth(LPCRECT lpRect)
* 函数参数:
*   LPCRECT lpRect，指向矩形区域的指针
* 函数类型:long
* 函数功能:获取矩形区域的宽度
*************************************************************************/

long  CDib::GetRectWidth(LPCRECT lpRect)
{
	long nWidth=lpRect->right - lpRect->left;
	return nWidth;
}
/*************************************************************************
* 函数名称：GetRectHeight(LPCRECT lpRect)
* 函数参数:
*   LPCRECT lpRect，指向矩形区域的指针
* 函数类型:long
* 函数功能:获取矩形区域的高度
*************************************************************************/
long  CDib::GetRectHeight(LPCRECT lpRect)
{
	long nHeight=lpRect->bottom - lpRect->top;
	return nHeight;
}

/*************************************************************************
* 函数名称：InitMembers() 
* 函数类型: void
* 函数功能:初始化类的成员变量
*************************************************************************/
void CDib::InitMembers()
{
	m_hDib=NULL;
	lpbmihrd=NULL;	// 指向BITMAPINFO结构的指针
	lpdib=NULL;
	lpDIBBits=NULL;
	dwDIBSize=0;
	lpRgbQuag=NULL;

	/******************以下为新dib的操作*********************/
	// 数据成员初始化
	strcpy_s(m_fileName, "");
	m_lpBmpFileHeader = NULL;
	m_lpDib = NULL;   
	m_lpBmpInfo = NULL;
	m_lpBmpInfoHeader = NULL;
	m_lpRgbQuad = NULL;
	m_lpData = NULL;
	m_hPalette = NULL;
	m_bHasRgbQuad = FALSE;
	m_bValid = FALSE;
}

/*************************************************************************
* 函数名称：ClearMemory() 
* 函数类型: void
* 函数功能:复位类的成员变量
*************************************************************************/
void CDib::ClearMemory()
{
	if(m_hDib!=NULL)
		::GlobalFree(m_hDib);
	lpbmihrd=NULL;	// 指向BITMAPINFO结构的指针
	lpdib=NULL;
	dwDIBSize=0;
	lpRgbQuag=NULL;	
	//dwDIBSize=0;

	/***************************以下是新dib的操作*************************/
	Empty();
}

BOOL CDib::IsEmpty()
{
	if (m_hDib == NULL && lpdib == NULL)
	{
		AfxMessageBox("未加载图片，无法操作!");
		return TRUE;
	}

	return FALSE;
}

/*******************************以下是新dib的操作************************/
//=======================================================
// 函数功能： 从文件加载位图
// 输入参数： LPCTSTR lpszPath-待加载位图文件路径
// 返回值：   BOOL-TRUE 成功；FALSE 失败
//=======================================================
BOOL CDib::LoadFromFile(LPCTSTR lpszPath)
{
	// 记录位图文件名
	strcpy_s(m_fileName, lpszPath);

	// 以读模式打开位图文件
	CFile dibFile;
	if(!dibFile.Open(m_fileName, CFile::modeRead | CFile::shareDenyWrite))
	{
		return FALSE;
	}

	// 清理空间
	Empty(); 

	// 为位图文件头分配空间，并初始化为0
	m_lpBmpFileHeader = (LPBITMAPFILEHEADER)new BYTE[sizeof(BITMAPFILEHEADER)];
	memset(m_lpBmpFileHeader, 0, sizeof(BITMAPFILEHEADER)); 

	// 读取位图文件头
	int nCount = dibFile.Read((void *)m_lpBmpFileHeader, sizeof(BITMAPFILEHEADER));
	if(nCount != sizeof(BITMAPFILEHEADER)) 
	{
		return FALSE;
	} 

	// 判断此文件是不是位图文件（“0x4d42”代表“BM”）
	if(m_lpBmpFileHeader->bfType == 0x4d42)
	{
		// 是位图文件

		// 计算除位图文件头的空间大小，分配空间并初始化为0
		DWORD dwDibSize = dibFile.GetLength() - sizeof(BITMAPFILEHEADER);
		m_lpDib = new BYTE[dwDibSize];
		memset(m_lpDib, 0, dwDibSize);

		// 读取除位图文件头的所有数据
		dibFile.Read(m_lpDib, dwDibSize);

		// 关闭位图文件
		dibFile.Close();

		// 设置位图信息指针
		m_lpBmpInfo = (LPBITMAPINFO)m_lpDib;

		// 设置位图信息头指针
		m_lpBmpInfoHeader = (LPBITMAPINFOHEADER)m_lpDib;

		// 设置位图颜色表指针
		m_lpRgbQuad = (LPRGBQUAD)(m_lpDib + m_lpBmpInfoHeader->biSize);

		// 如果位图没有设置位图使用的颜色数，设置它
		if(m_lpBmpInfoHeader->biClrUsed == 0)
		{
			m_lpBmpInfoHeader->biClrUsed = GetNumOfColor();
		}

		// 计算颜色表长度
		DWORD dwRgbQuadLength = CalcRgbQuadLength();

		// 设置位图数据指针
		m_lpData = m_lpDib + m_lpBmpInfoHeader->biSize + dwRgbQuadLength;

		// 判断是否有颜色表
		if(m_lpRgbQuad == (LPRGBQUAD)m_lpData)
		{
			m_lpRgbQuad = NULL;    // 将位图颜色表指针置空
			m_bHasRgbQuad = FALSE; // 无颜色表
		}
		else
		{
			m_bHasRgbQuad = TRUE;  // 有颜色表
			MakePalette();         // 根据颜色表生成调色板
		}        

		// 设置位图大小（因为很多位图文件都不设置此项）
		m_lpBmpInfoHeader->biSizeImage = GetSize();

		// 位图有效
		m_bValid = TRUE;

		return TRUE;
	}
	else
	{
		// 不是位图文件
		m_bValid = FALSE;

		return FALSE;
	}     

}

//=======================================================
// 函数功能： 将位图保存到文件
// 输入参数： LPCTSTR lpszPath-位图文件保存路径
// 返回值：   BOOL-TRUE 成功；FALSE 失败
//=======================================================
BOOL CDib::SaveToFile(LPCTSTR lpszPath)
{
	// 以写模式打开文件
	CFile dibFile;
	if(!dibFile.Open(lpszPath, CFile::modeCreate | CFile::modeReadWrite 
		| CFile::shareExclusive))
	{
		return FALSE;
	}

	// 记录位图文件名
	strcpy_s(m_fileName, lpszPath);

	// 将文件头结构写进文件
	dibFile.Write(m_lpBmpFileHeader, sizeof(BITMAPFILEHEADER));

	// 将文件信息头结构写进文件
	dibFile.Write(m_lpBmpInfoHeader, sizeof(BITMAPINFOHEADER));

	// 计算颜色表长度
	DWORD dwRgbQuadlength = CalcRgbQuadLength();

	// 如果有颜色表的话，将颜色表写进文件
	if(dwRgbQuadlength != 0)
	{
		dibFile.Write(m_lpRgbQuad, dwRgbQuadlength);
	}                                                        

	// 将位图数据写进文件
	DWORD dwDataSize = GetLineByte() * GetHeight();
	dibFile.Write(m_lpData, dwDataSize);

	// 关闭文件
	dibFile.Close();

	return TRUE;
}

//=======================================================
// 函数功能： 获取位图文件名
// 输入参数： 无
// 返回值：   LPCTSTR-位图文件名
//=======================================================
LPCTSTR CDib::GetFileName()
{
	return m_fileName;
}

//=======================================================
// 函数功能： 获取位图宽度
// 输入参数： 无
// 返回值：   LONG-位图宽度
//=======================================================
LONG CDib::GetWidth()
{
	return m_lpBmpInfoHeader->biWidth;
}

//=======================================================
// 函数功能： 获取位图高度
// 输入参数： 无
// 返回值：   LONG-位图高度
//=======================================================
LONG CDib::GetHeight()
{
	return m_lpBmpInfoHeader->biHeight;
}

//=======================================================
// 函数功能： 获取位图的宽度和高度
// 输入参数： 无
// 返回值：   CSize-位图的宽度和高度
//=======================================================
CSize CDib::GetDimension()
{
	return CSize(GetWidth(), GetHeight());
}

//=======================================================
// 函数功能： 获取位图大小
// 输入参数： 无
// 返回值：   DWORD-位图大小
//=======================================================
DWORD CDib::GetSize()
{
	if(m_lpBmpInfoHeader->biSizeImage != 0)
	{
		return m_lpBmpInfoHeader->biSizeImage;
	}
	else
	{       
		return GetWidth() * GetHeight();
	}
}

//=======================================================
// 函数功能： 获取单个像素所占位数
// 输入参数： 无
// 返回值：   WORD-单个像素所占位数
//=======================================================
WORD CDib::GetBitCount()
{
	return m_lpBmpInfoHeader->biBitCount;
}       

//=======================================================
// 函数功能： 获取每行像素所占字节数
// 输入参数： 无
// 返回值：   UINT-每行像素所占字节数
//=======================================================
UINT CDib::GetLineByte()
{ 
	return (GetWidth() * GetBitCount() / 8 + 3) / 4 * 4;;
}

//=======================================================
// 函数功能： 获取位图颜色数
// 输入参数： 无
// 返回值：   DWORD-位图颜色数
//=======================================================
DWORD CDib::GetNumOfColor()
{
	UINT dwNumOfColor;     

	if ((m_lpBmpInfoHeader->biClrUsed == 0) 
		&& (m_lpBmpInfoHeader->biBitCount < 9))
	{
		switch (m_lpBmpInfoHeader->biBitCount)
		{
		case 1: dwNumOfColor = 2; break;
		case 4: dwNumOfColor = 16; break;
		case 8: dwNumOfColor = 256;
		}
	}
	else
	{
		dwNumOfColor = m_lpBmpInfoHeader->biClrUsed;
	}  		

	return dwNumOfColor; 
}

//=======================================================
// 函数功能： 计算位图颜色表长度
// 输入参数： 无
// 返回值：   DWORD-位图颜色表长度
//=======================================================
DWORD CDib::CalcRgbQuadLength()
{
	DWORD dwNumOfColor = GetNumOfColor();
	if(dwNumOfColor > 256)
	{
		dwNumOfColor = 0;
	}
	return  dwNumOfColor * sizeof(RGBQUAD);
}

//=======================================================
// 函数功能： 获取位图颜色表
// 输入参数： 无
// 返回值：   LPRGBQUAD-位图颜色表指针
//=======================================================
LPRGBQUAD CDib::GetRgbQuad()
{
	return m_lpRgbQuad;
}

//=======================================================
// 函数功能： 获取位图数据
// 输入参数： 无
// 返回值：   LPBYTE-位图数据指针
//=======================================================
LPBYTE CDib::GetData()
{
	return m_lpData;
}

//=======================================================
// 函数功能： 根据颜色表生成调色板
// 输入参数： 无
// 返回值：   BOOL-TRUE 成功；FALSE 失败
//=======================================================
BOOL CDib::MakePalette()
{
	// 计算颜色表长度
	DWORD dwRgbQuadLength = CalcRgbQuadLength();

	// 如果颜色表长度为0，则不生成逻辑调色板
	if(dwRgbQuadLength == 0) 
	{
		return FALSE;
	}

	//删除旧的调色板对象
	if(m_hPalette != NULL) 
	{
		DeleteObject(m_hPalette);
		m_hPalette = NULL;
	}

	// 申请缓冲区，初始化为0
	DWORD dwNumOfColor = GetNumOfColor();
	DWORD dwSize = 2 * sizeof(WORD) + dwNumOfColor * sizeof(PALETTEENTRY);
	LPLOGPALETTE lpLogPalette = (LPLOGPALETTE) new BYTE[dwSize];
	memset(lpLogPalette, 0, dwSize);

	// 生成逻辑调色板
	lpLogPalette->palVersion = 0x300;
	lpLogPalette->palNumEntries = dwNumOfColor;
	LPRGBQUAD lpRgbQuad = (LPRGBQUAD) m_lpRgbQuad;
	for(int i = 0; i < dwNumOfColor; i++) 
	{
		lpLogPalette->palPalEntry[i].peRed = lpRgbQuad->rgbRed;
		lpLogPalette->palPalEntry[i].peGreen = lpRgbQuad->rgbGreen;
		lpLogPalette->palPalEntry[i].peBlue = lpRgbQuad->rgbBlue;
		lpLogPalette->palPalEntry[i].peFlags = 0;
		lpRgbQuad++;
	}

	// 创建逻辑调色板
	m_hPalette = CreatePalette(lpLogPalette);

	// 释放缓冲区
	delete [] lpLogPalette;

	return TRUE;
}

//=======================================================
// 函数功能： 显示位图
// 输入参数：
//            CDC *pDC-设备环境指针
//            CPoint origin-显示矩形区域的左上角
//            CSize size-显示矩形区域的尺寸
// 返回值：
//            BOOL-TRUE 成功；FALSE 失败
//=======================================================
BOOL CDib::Draw(CDC *pDC, CPoint origin, CSize size)
{
	// 位图无效，无法绘制，返回错误
	if(!IsValid())
	{
		return FALSE;
	}

	// 旧的调色板句柄
	HPALETTE hOldPalette = NULL;

	// 如果位图指针为空，则返回FALSE
	if(m_lpDib == NULL) 
	{
		return FALSE;
	}

	// 如果位图有调色板，则选进设备环境中
	if(m_hPalette != NULL) 
	{
		hOldPalette = SelectPalette(pDC->GetSafeHdc(), m_hPalette, TRUE);
	}

	// 设置位图伸缩模式
	pDC->SetStretchBltMode(COLORONCOLOR);

	// 将位图在pDC所指向的设备上进行显示
	StretchDIBits(pDC->GetSafeHdc(), origin.x, origin.y, size.cx, size.cy,
		0, 0, GetWidth(), GetHeight(), m_lpData, m_lpBmpInfo, DIB_RGB_COLORS, SRCCOPY);

	// 恢复旧的调色板
	if(hOldPalette != NULL)
	{
		SelectPalette(pDC->GetSafeHdc(), hOldPalette, TRUE);
	}

	return TRUE;
}

//=======================================================
// 函数功能： 24位彩色位图转8位灰度位图
// 输入参数： 无
// 返回值：   BOOL-TRUE 成功；FALSE 失败
//=======================================================
BOOL CDib::RgbToGrade()
{
	// 位图无效，失败返回
	if(!IsValid())
	{
		return FALSE;
	}

	// 不是24位位图，失败返回
	if(GetBitCount() != 24)
	{
		return FALSE;
	}

	// 是压缩位图，失败返回
	if(m_lpBmpInfoHeader->biCompression != BI_RGB)
	{
		return FALSE;
	}

	// 如果不是灰度位图，才需要转换
	if(!IsGrade())
	{
		// 获取原位图信息
		LONG lHeight = GetHeight();
		LONG lWidth = GetWidth();
		UINT uLineByte = GetLineByte();

		// 计算灰度位图数据所需空间
		UINT uGradeBmpLineByte = (lWidth + 3) / 4 * 4;
		DWORD dwGradeBmpDataSize = uGradeBmpLineByte * lHeight; 

		// 计算灰度位图所需空间
		DWORD dwGradeBmpSize = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256 + dwGradeBmpDataSize;

		// 设置灰度位图文件头
		LPBITMAPFILEHEADER lpGradeBmpFileHeader = (LPBITMAPFILEHEADER)new BYTE[sizeof(BITMAPFILEHEADER)];
		memset(lpGradeBmpFileHeader, 0, sizeof(BITMAPFILEHEADER));
		lpGradeBmpFileHeader->bfType = 0x4d42;
		lpGradeBmpFileHeader->bfSize = sizeof(BITMAPFILEHEADER) + dwGradeBmpSize;
		lpGradeBmpFileHeader->bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)
			+ sizeof(RGBQUAD) * 256;
		lpGradeBmpFileHeader->bfReserved1 = 0;
		lpGradeBmpFileHeader->bfReserved2 = 0;            

		// 为灰度位图分配空间，并初始化为0
		LPBYTE lpGradeBmp = (LPBYTE)new BYTE[dwGradeBmpSize];
		memset(lpGradeBmp, 0, dwGradeBmpSize);

		// 设置灰度位图信息头
		LPBITMAPINFOHEADER lpGradeBmpInfoHeader = (LPBITMAPINFOHEADER)(lpGradeBmp);
		lpGradeBmpInfoHeader->biBitCount = 8;
		lpGradeBmpInfoHeader->biClrImportant = 0;
		lpGradeBmpInfoHeader->biClrUsed = 256;
		lpGradeBmpInfoHeader->biCompression = BI_RGB;
		lpGradeBmpInfoHeader->biHeight = lHeight;
		lpGradeBmpInfoHeader->biPlanes = 1;
		lpGradeBmpInfoHeader->biSize = sizeof(BITMAPINFOHEADER);
		lpGradeBmpInfoHeader->biSizeImage = dwGradeBmpDataSize;
		lpGradeBmpInfoHeader->biWidth = lWidth;
		lpGradeBmpInfoHeader->biXPelsPerMeter = m_lpBmpInfoHeader->biXPelsPerMeter;
		lpGradeBmpInfoHeader->biYPelsPerMeter = m_lpBmpInfoHeader->biYPelsPerMeter;

		// 设置灰度位图颜色表
		LPRGBQUAD lpGradeBmpRgbQuad = (LPRGBQUAD)(lpGradeBmp + sizeof(BITMAPINFOHEADER));

		// 初始化8位灰度图的调色板信息
		LPRGBQUAD lpRgbQuad;               
		for(int k = 0; k < 256; k++)
		{
			lpRgbQuad = (LPRGBQUAD)(lpGradeBmpRgbQuad + k);
			lpRgbQuad->rgbBlue = k; 
			lpRgbQuad->rgbGreen = k;
			lpRgbQuad->rgbRed = k;
			lpRgbQuad->rgbReserved = 0;
		}

		// 灰度位图数据处理
		BYTE r, g, b; 
		LPBYTE lpGradeBmpData = (LPBYTE)(lpGradeBmp + sizeof(BITMAPINFOHEADER) 
			+ sizeof(RGBQUAD) * 256);
		// 进行颜色转换
		for(int i = 0; i < lHeight; i++)
		{
			for(int j = 0; j < lWidth; j++)
			{
				b = m_lpData[i * uLineByte + 3 * j];
				g = m_lpData[i * uLineByte + 3 * j + 1];
				r = m_lpData[i * uLineByte + 3 * j + 2];
				lpGradeBmpData[i * uGradeBmpLineByte + j] = (BYTE)(0.299 * r + 0.587 * g + 0.114 * b); 
			}
		}

		// 释放原有位图空间
		Empty(FALSE);

		// 重新设定原位图指针指向
		m_lpBmpFileHeader = lpGradeBmpFileHeader;
		m_lpDib = lpGradeBmp;
		m_lpBmpInfo = (LPBITMAPINFO)(lpGradeBmp);
		m_lpBmpInfoHeader = lpGradeBmpInfoHeader;
		m_lpRgbQuad = lpGradeBmpRgbQuad;
		m_lpData = lpGradeBmpData;

		// 设置颜色表标志
		m_bHasRgbQuad = TRUE;  
		// 设置位图有效标志
		m_bValid = TRUE;
		// 生成调色板
		MakePalette();
	}

	return TRUE;   
}   

//=======================================================
// 函数功能： 8位灰度位图转24位彩色位图
// 输入参数： 无
// 返回值：   BOOL-TRUE 成功；FALSE 失败
//=======================================================
BOOL CDib::GradeToRgb()
{
	// 位图无效，失败退出
	if(!IsValid())
	{
		return FALSE;
	}

	// 不是8位位图，失败退出
	if(GetBitCount() != 8)
	{
		return FALSE;
	}

	// 是压缩位图，失败返回
	if(m_lpBmpInfoHeader->biCompression != BI_RGB)
	{
		return FALSE;
	}

	// 是灰度图时，才需转换
	if(IsGrade())
	{
		// 获取原位图信息
		LONG lHeight = GetHeight();
		LONG lWidth = GetWidth();
		UINT uLineByte = GetLineByte();

		// 计算彩色位图数据所需空间
		UINT uColorBmpLineByte = (lWidth * 24 / 8 + 3) / 4 * 4;
		DWORD dwColorBmpDataSize = uColorBmpLineByte * lHeight; 

		// 计算彩色位图所需空间
		DWORD dwColorBmpSize = sizeof(BITMAPINFOHEADER) + dwColorBmpDataSize;

		// 设置彩色位图文件头
		LPBITMAPFILEHEADER lpColorBmpFileHeader = (LPBITMAPFILEHEADER)new BYTE[sizeof(BITMAPFILEHEADER)];
		memset(lpColorBmpFileHeader, 0, sizeof(BITMAPFILEHEADER));
		lpColorBmpFileHeader->bfType = 0x4d42;
		lpColorBmpFileHeader->bfSize = sizeof(BITMAPFILEHEADER) + dwColorBmpSize;
		lpColorBmpFileHeader->bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
		lpColorBmpFileHeader->bfReserved1 = 0;
		lpColorBmpFileHeader->bfReserved2 = 0;    

		// 为彩色位图分配空间，并初始化为0
		LPBYTE lpColorBmp = (LPBYTE)new BYTE[dwColorBmpSize];
		memset(lpColorBmp, 0, dwColorBmpSize);

		// 设置彩色位图信息头
		LPBITMAPINFOHEADER lpColorBmpInfoHeader = (LPBITMAPINFOHEADER)(lpColorBmp);
		lpColorBmpInfoHeader->biBitCount = 24;
		lpColorBmpInfoHeader->biClrImportant = 0;
		lpColorBmpInfoHeader->biClrUsed = 0;
		lpColorBmpInfoHeader->biCompression = BI_RGB;
		lpColorBmpInfoHeader->biHeight = lHeight;
		lpColorBmpInfoHeader->biPlanes = 1;
		lpColorBmpInfoHeader->biSize = sizeof(BITMAPINFOHEADER);
		lpColorBmpInfoHeader->biSizeImage = dwColorBmpDataSize;
		lpColorBmpInfoHeader->biWidth = lWidth;
		lpColorBmpInfoHeader->biXPelsPerMeter = m_lpBmpInfoHeader->biXPelsPerMeter;
		lpColorBmpInfoHeader->biYPelsPerMeter = m_lpBmpInfoHeader->biYPelsPerMeter;

		// 彩色位图数据处理
		LPBYTE lpColorBmpData = (LPBYTE)(lpColorBmp + sizeof(BITMAPINFOHEADER));
		// 进行颜色转换
		for(int i = 0; i < lHeight; i++)
		{
			for(int j = 0; j < lWidth; j++)
			{
				BYTE btValue = m_lpData[i * uLineByte + j]; 
				lpColorBmpData[i * uColorBmpLineByte + 3 * j] = btValue;
				lpColorBmpData[i * uColorBmpLineByte + 3 * j + 1] = btValue;
				lpColorBmpData[i * uColorBmpLineByte + 3 * j + 2] = btValue;  
			}
		}

		// 释放原有位图空间
		Empty(FALSE);

		// 重新设定原位图指针指向
		m_lpBmpFileHeader = lpColorBmpFileHeader;
		m_lpDib = lpColorBmp;
		m_lpBmpInfo = (LPBITMAPINFO)(lpColorBmp);
		m_lpBmpInfoHeader = lpColorBmpInfoHeader;
		m_lpRgbQuad = NULL;
		m_lpData = lpColorBmpData;

		// 设置颜色表标志
		m_bHasRgbQuad = FALSE;  
		// 设置位图有效标志
		m_bValid = TRUE;        
	}        

	return TRUE;   
}   

//=======================================================
// 函数功能： 判断是否含有颜色表
// 输入参数： 无
// 返回值：   判断结果：TRUE-含有颜色表；FALSE-不含颜色表
//=======================================================
BOOL CDib::HasRgbQuad()
{
	return m_bHasRgbQuad;
}

//=======================================================
// 函数功能： 判断是否是灰度图
// 输入参数： 无
// 返回值：   判断结果：TRUE-是灰度图；FALSE-是彩色图
//=======================================================
BOOL CDib::IsGrade()
{
	return (GetBitCount() < 9 && GetBitCount() > 0);
}

//=======================================================
// 函数功能： 判断位图是否有效
// 输入参数： 无
// 返回值：   判断结果：TRUE-位图有效；FALSE-位图无效
//=======================================================
BOOL CDib::IsValid()
{
	return m_bValid;
}

//=======================================================
// 函数功能： 清理空间
// 输入参数： BOOL bFlag-TRUE 全部清空；FALSE 部分清空
// 返回值：   无
//=======================================================
void CDib::Empty(BOOL bFlag)
{
	// 文件名清空
	if(bFlag)
	{
		strcpy_s(m_fileName, "");
	}      

	// 释放位图文件头指针空间
	if(m_lpBmpFileHeader != NULL)
	{
		delete [] m_lpBmpFileHeader;
		m_lpBmpFileHeader = NULL;
	}    

	// 释放位图指针空间
	if(m_lpDib != NULL)
	{
		delete [] m_lpDib;
		m_lpDib = NULL;
		m_lpBmpInfo = NULL;
		m_lpBmpInfoHeader = NULL;
		m_lpRgbQuad = NULL;
		m_lpData = NULL;           
	}       

	// 释放调色板
	if(m_hPalette != NULL)
	{
		DeleteObject(m_hPalette);
		m_hPalette = NULL;
	}    

	// 设置不含颜色表
	m_bHasRgbQuad = FALSE;

	// 设置位图无效
	m_bValid = FALSE;

}  

BOOL WINAPI CDib::ChaosEncode(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, double Xstart, double a)
{
	//指向原图的指针
	unsigned char* lpSrc;
	//保存的混沌密码序列
	DWORD chaos, k;
	double x, y;
	x = Xstart;
	//循环变量
	LONG i;
	LONG j;
	LONG h = 1000;
	LONG w;
	//图象每行的字节数
	LONG lLineBytes;
	//中间变量
	//FLOAT fTemp;
	//计算图象每行的字节数
	lLineBytes = WIDTHBYTES(lWidth * 8);
	/**************************************************************
	*本软件采用的是Logistic混沌系统，其定义为:y=a*x*(1-x);y又作为新的x
	*继续生成序列，当3.5699456……<a<=4;-1<x<1，该系统为混沌系统
	**************************************************************/
	//产生1000个Logistic序列
	/*	for(w=0;w<1000;w++)
	{
	y=a*x*(1-x);
	x=y;
	}*/

	//循环次数
	//for(h=0;h<100;h++)
	//{
	//while(h>0)
	//{
	//每行
	for (i = 0; i<lHeight; i++)
	{
		//每列
		for (j = 0; j<lWidth; j++)
		{
			//位图是从下向上扫描的
			//指向DIB第i行,第j个象素的指针
			lpSrc = (unsigned char*)(lpDIBBits + lLineBytes*(lHeight - 1 - i) + j);
			chaos = 0;
			for (w = 0; w<8; w++)
			{  //转换为二进制
				y = a*x*(1 - x);
				if (y >= 0.4)
				{
					k = 1;
				}
				else
				{
					k = 0;
				}
				chaos = ((chaos << 1) | k);                       //或运算|
				// if(chaos>(int)256*y)
				//chaos=256-chaos;
				x = y;
				//可以做个数组，进行
			}
			//修改位图像素值的位
			*lpSrc = (unsigned char)((int)(*lpSrc) ^ (int)(chaos));//异或运算^
			//对二进制位进行编码
			//*lpSrc=255;  
			//chaos = ::LogisticCode(x,a);
			//AfxMessageBox(chaos);

			//fTemp = *lpSrc;
			//判断是否超出范围
			/*if(fTemp >255)
			{
			//直接附值为255
			*lpSrc = 255;
			}
			else if (fTemp<0)
			{
			//直接赋值为0
			*lpSrc = 0;
			}
			else
			{
			//四舍五入
			*lpSrc = (unsigned char)(fTemp+0.5);
			}*/

			h--;
		}
	}
	//}
	//返回
	return TRUE;
}