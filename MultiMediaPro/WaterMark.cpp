/* Copyright (C) wuchen 2011 ALL RIGHTS RESERVED.
* Author:niewucai
* Email:niewucai@126.com
* Last Update:2011/05/11
* 实现对位图图片添加文字水印功能 -- 实现文件
*/
#include "stdafx.h"
#include "WaterMark.h"

CWaterMark::CWaterMark(LPCTSTR lpszImgFile)
: m_topSpacing(10),
m_rightSpacing(10),
m_bottomSpacing(10),
m_leftSpacing(10),
m_isTransparant(true),
m_foreColor(0),
m_bkColor(RGB(255, 255, 255))
{
	this->m_imgFile = lpszImgFile;
	//创建默认字体
	m_hFont = CreateFontA(0, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, _T("宋体"));
	m_hInitFont = m_hFont;

	m_hBmp = (HBITMAP)LoadImage(AfxGetInstanceHandle(), lpszImgFile, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	Init();
}
CWaterMark::CWaterMark(UINT nIDResource)
: m_topSpacing(10),
m_rightSpacing(10),
m_bottomSpacing(10),
m_leftSpacing(10),
m_isTransparant(true),
m_foreColor(0),
m_bkColor(RGB(255, 255, 255))
{
	//创建默认字体
	m_hFont = CreateFontA(0, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, _T("宋体"));
	m_hFont = CreateFontA(100, 100, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, _T("宋体"));
	m_hInitFont = m_hFont;

	m_hBmp = (HBITMAP)LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(nIDResource));

	Init();
}
CWaterMark::~CWaterMark()
{
	SelectObject(m_hMemDC, m_hOldBmp);
	SelectObject(m_hMemDC, m_hOldFont);
	DeleteObject(m_hInitFont);
}
void CWaterMark::Init()
{
	m_hMemDC = CreateCompatibleDC(NULL);	//创建一个与屏幕内存设备兼容的内存设备
	m_hOldFont = (HFONT)SelectObject(m_hMemDC, (HFONT)m_hFont);//选择默认字体
	m_hOldBmp = (HBITMAP)SelectObject(m_hMemDC, m_hBmp);//选择位图到内存设备
	SetBkMode(m_hMemDC, m_isTransparant ? TRANSPARENT : OPAQUE);//是否背景透明
	SetTextColor(m_hMemDC, m_foreColor);//前景色
	::SetBkColor(m_hMemDC, m_bkColor);//背景色
}
/*更换当前操作位图
* @nIDResouce:位图资源标识
* return:是否更换成功
*/
bool CWaterMark::ChangeBitmap(UINT nIDResource)
{
	m_hBmp = (HBITMAP)LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(nIDResource));
	if (!m_hBmp)
	{
		return false;
	}
	m_imgFile.clear();
	m_hOldBmp = (HBITMAP)SelectObject(m_hMemDC, m_hBmp);//选择位图到内存设备
	return true;
}
/*更换当前操作位图
* @lpszImgFile:位图文件路径
* return:是否更换成功
*/
bool CWaterMark::ChangeBitmap(LPCTSTR lpszImgFile)
{
	m_hBmp = (HBITMAP)LoadImage(AfxGetInstanceHandle(), lpszImgFile, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	if (!m_hBmp)
	{
		return false;
	}
	this->m_imgFile = lpszImgFile;
	m_hOldBmp = (HBITMAP)SelectObject(m_hMemDC, m_hBmp);//选择位图到内存设备
	return true;
}
/*获取当前处理位图的BITMAP结构*/
int CWaterMark::GetBitmap(BITMAP* pBitMap)
{
	if (!m_hBmp)
	{
		return 0;
	}
	return CBitmap::FromHandle(m_hBmp)->GetBitmap(pBitMap);
}
/*获取水印文字的前景色*/
COLORREF CWaterMark::GetForeColor()
{
	return m_foreColor;
}
/*设置水印文字的前景色*/
void CWaterMark::SetForeColor(COLORREF foreColor/* = 0*/)
{
	m_foreColor = foreColor;
	SetTextColor(m_hMemDC, m_foreColor);//设置前景色
}
/*获取水印文字的背景色*/
COLORREF CWaterMark::GetBkColor()
{
	return m_bkColor;
}
/*设置水印文字的背景色*/
void CWaterMark::SetBkColor(COLORREF bkColor/* = RGB(255, 255, 255)*/)
{
	m_bkColor = bkColor;
	::SetBkColor(m_hMemDC, m_bkColor);//背景色
}
/*获取当前是否设置水印文字为透明背景*/
bool CWaterMark::IsTransparent()
{
	return m_isTransparant;
}
/*设置当前是否设置水印文字为透明背景*/
void CWaterMark::SetTransparent(bool transparent/* = true*/)
{
	m_isTransparant = transparent;
	SetBkMode(m_hMemDC, m_isTransparant ? TRANSPARENT : OPAQUE);//是否背景透明
}
/*获取当前用于输出水印文字的字体*/
HFONT CWaterMark::GetTextFont()
{
	return m_hFont;
}
/*设置当前用于输出水印文字的字体,返回上一次设置的字体*/
HFONT CWaterMark::SetTextFont(HFONT hFont)
{
	m_hFont = hFont;
	m_hOldFont = (HFONT)SelectObject(m_hMemDC, (HFONT)m_hFont);//设置水印文字字体
	return m_hOldFont;
}
/*添加水印文字
* @lpszText:水印文本
* @x:水印文本相对图片左上角的水平位置(像素)
* @y:水印文本相对图片左上角的垂直位置(像素)
*/
void CWaterMark::AddWaterMark(const char *  lpszText, int x, int y)
{
	TextOut(m_hMemDC, x, y, lpszText, strlen(lpszText));
}
/*根据选择的方式和上下左右的间距，自动添加水印文本到合适的位置，默认添加到图片右下角*/
void CWaterMark::AddWaterMark(LPCTSTR lpszText, Location type/* = Location::RightBottom*/)
//void CWaterMark::AddWaterMark(const char * lpszText, Location type/* = Location::RightBottom*/)
{
	//获取字体宽度
	TEXTMETRIC txtMetric = { 0 };
	::GetTextMetrics(m_hMemDC, &txtMetric);
	//水印文字的总高度和宽度
	SIZE size;
	::GetTextExtentPoint(m_hMemDC, lpszText, strlen(lpszText), &size);
	int height = size.cy;
	int width = size.cx;
	//水印文本相对图片左上角的水平位置(像素)
	int x = 0;
	//水印文本相对图片左上角的垂直位置(像素)
	int y = 0;
	BITMAP bmp;
	this->GetBitmap(&bmp);
	//位图的高度和宽度
	int nBmpWidth = bmp.bmWidth;
	int nBmpHeight = bmp.bmHeight;
	switch (type)
	{
	case Location::LeftTop:
		x = m_leftSpacing;
		y = m_topSpacing;
		break;
	case Location::CenterTop:
		x = (nBmpWidth - width) / 2;
		y = m_topSpacing;
		break;
	case Location::RightTop:
		x = nBmpWidth - width - m_rightSpacing;
		y = m_topSpacing;
		break;
	case Location::LeftMiddle:
		x = m_leftSpacing;
		y = (nBmpHeight - height) / 2;
		break;
	case Location::CenterMiddle:
		x = (nBmpWidth - width) / 2;
		y = (nBmpHeight - height) / 2;
		break;
	case Location::RightMiddle:
		x = nBmpWidth - width - m_rightSpacing;
		y = (nBmpHeight - height) / 2;
		break;
	case Location::LeftBottom:
		x = m_leftSpacing;
		y = nBmpHeight - height - m_bottomSpacing;
		break;
	case Location::CenterBottom:
		x = (nBmpWidth - width) / 2;
		y = nBmpHeight - height - m_bottomSpacing;
		break;
	case Location::RightBottom:
		x = nBmpWidth - width - m_rightSpacing;
		y = nBmpHeight - height - m_bottomSpacing;
		break;
	default:
		break;
	}
	TextOut(m_hMemDC, x, y, lpszText, strlen(lpszText));
}
/*保存到当前操作位图文件,返回是否保存成功 -  只针对以位图文件加载的情况*/
bool CWaterMark::Save()
{
	if (m_imgFile.empty())
	{
		return false;
	}
	if (m_hBmp)
	{
		PBITMAPINFO bmpInfo = CreateBitmapInfoStruct(NULL, m_hBmp);
		CreateBMPFile(NULL, m_imgFile.c_str(), bmpInfo, m_hBmp, m_hMemDC);
		return true;
	}
	else
	{
		return false;
	}
}
/*保存到指定位图文件,返回是否保存成功*/
bool CWaterMark::Save(LPCTSTR lpszFile)
{
	if (m_hBmp)
	{
		PBITMAPINFO bmpInfo = CreateBitmapInfoStruct(NULL, m_hBmp);
		CreateBMPFile(NULL, lpszFile, bmpInfo, m_hBmp, m_hMemDC);
		return true;
	}
	else
	{
		return false;
	}
}
PBITMAPINFO CWaterMark::CreateBitmapInfoStruct(HWND hwnd, HBITMAP hBmp)
{
	BITMAP bmp;
	PBITMAPINFO pbmi;
	WORD    cClrBits;

	// Retrieve the bitmap color format, width, and height.  
	if (!GetObject(hBmp, sizeof(BITMAP), (LPSTR)&bmp))
		return NULL;

	// Convert the color format to a count of bits.  
	cClrBits = (WORD)(bmp.bmPlanes * bmp.bmBitsPixel);
	if (cClrBits == 1)
		cClrBits = 1;
	else if (cClrBits <= 4)
		cClrBits = 4;
	else if (cClrBits <= 8)
		cClrBits = 8;
	else if (cClrBits <= 16)
		cClrBits = 16;
	else if (cClrBits <= 24)
		cClrBits = 24;
	else cClrBits = 32;

	// Allocate memory for the BITMAPINFO structure. (This structure  
	// contains a BITMAPINFOHEADER structure and an array of RGBQUAD  
	// data structures.)  

	if (cClrBits < 24)
		pbmi = (PBITMAPINFO)LocalAlloc(LPTR,
		sizeof(BITMAPINFOHEADER)+
		sizeof(RGBQUAD)* (1 << cClrBits));

	// There is no RGBQUAD array for these formats: 24-bit-per-pixel or 32-bit-per-pixel 

	else
		pbmi = (PBITMAPINFO)LocalAlloc(LPTR,
		sizeof(BITMAPINFOHEADER));

	// Initialize the fields in the BITMAPINFO structure.  

	pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	pbmi->bmiHeader.biWidth = bmp.bmWidth;
	pbmi->bmiHeader.biHeight = bmp.bmHeight;
	pbmi->bmiHeader.biPlanes = bmp.bmPlanes;
	pbmi->bmiHeader.biBitCount = bmp.bmBitsPixel;
	if (cClrBits < 24)
		pbmi->bmiHeader.biClrUsed = (1 << cClrBits);

	// If the bitmap is not compressed, set the BI_RGB flag.  
	pbmi->bmiHeader.biCompression = BI_RGB;

	// Compute the number of bytes in the array of color  
	// indices and store the result in biSizeImage.  
	// The width must be DWORD aligned unless the bitmap is RLE 
	// compressed. 
	pbmi->bmiHeader.biSizeImage = ((pbmi->bmiHeader.biWidth * cClrBits + 31) & ~31) / 8
		* pbmi->bmiHeader.biHeight;
	// Set biClrImportant to 0, indicating that all of the  
	// device colors are important.  
	pbmi->bmiHeader.biClrImportant = 0;
	return pbmi;
}

void CWaterMark::CreateBMPFile(HWND hwnd, LPCTSTR pszFile, PBITMAPINFO pbi,
	HBITMAP hBMP, HDC hDC)
{
	HANDLE hf;                 // file handle  
	BITMAPFILEHEADER hdr;       // bitmap file-header  
	PBITMAPINFOHEADER pbih;     // bitmap info-header  
	LPBYTE lpBits;              // memory pointer  
	DWORD dwTotal;              // total count of bytes  
	DWORD cb;                   // incremental count of bytes  
	BYTE *hp;                   // byte pointer  
	DWORD dwTmp;

	pbih = (PBITMAPINFOHEADER)pbi;
	lpBits = (LPBYTE)GlobalAlloc(GMEM_FIXED, pbih->biSizeImage);

	if (!lpBits)
		return;

	// Retrieve the color table (RGBQUAD array) and the bits  
	// (array of palette indices) from the DIB.  
	if (!GetDIBits(hDC, hBMP, 0, (WORD)pbih->biHeight, lpBits, pbi,
		DIB_RGB_COLORS))
	{
		return;
	}

	// Create the .BMP file.  
	hf = CreateFile(pszFile,
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (hf == INVALID_HANDLE_VALUE)
		return;
	hdr.bfType = 0x4d42;        // 0x42 = "B" 0x4d = "M"  
	// Compute the size of the entire file.  
	hdr.bfSize = (DWORD)(sizeof(BITMAPFILEHEADER)+
		pbih->biSize + pbih->biClrUsed
		* sizeof(RGBQUAD)+pbih->biSizeImage);
	hdr.bfReserved1 = 0;
	hdr.bfReserved2 = 0;

	// Compute the offset to the array of color indices.  
	hdr.bfOffBits = (DWORD) sizeof(BITMAPFILEHEADER)+
		pbih->biSize + pbih->biClrUsed
		* sizeof (RGBQUAD);

	// Copy the BITMAPFILEHEADER into the .BMP file.  
	if (!WriteFile(hf, (LPVOID)&hdr, sizeof(BITMAPFILEHEADER),
		(LPDWORD)&dwTmp, NULL))
	{
		return;
	}

	// Copy the BITMAPINFOHEADER and RGBQUAD array into the file.  
	if (!WriteFile(hf, (LPVOID)pbih, sizeof(BITMAPINFOHEADER)
		+pbih->biClrUsed * sizeof (RGBQUAD),
		(LPDWORD)&dwTmp, (NULL)))
		return;

	// Copy the array of color indices into the .BMP file.  
	dwTotal = cb = pbih->biSizeImage;
	hp = lpBits;
	if (!WriteFile(hf, (LPSTR)hp, (int)cb, (LPDWORD)&dwTmp, NULL))
		return;

	// Close the .BMP file.  
	if (!CloseHandle(hf))
		return;

	// Free memory.  
	GlobalFree((HGLOBAL)lpBits);
}

