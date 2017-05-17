/* Copyright (C) wuchen 2011 ALL RIGHTS RESERVED.
* Author:niewucai
* Email:niewucai@126.com
* Last Update:2011/05/11
* 实现对位图图片添加文字水印功能 -- 头文件
*/
#pragma once
//#include <stdafx.h>
#include <string>
using namespace std;


class CWaterMark{
public:
	/*构造方法
	* @lpszImgFile:要添加水印的位图图片文件
	*/
	CWaterMark(LPCTSTR lpszImgFile);
	/*构造方法
	* @nIDResource:位图资源标识
	*/
	CWaterMark(UINT nIDResource);
	~CWaterMark();

	//水印文字位置枚举
	typedef enum
	{
		/* 表示左上位置。
		*/
		LeftTop = 1,

		/* 表示中上位置。
		*/
		CenterTop,

		/* 表示右上位置。
		*/
		RightTop,

		/* 表示左中位置。
		*/
		LeftMiddle,

		/* 表示中心位置。
		*/
		CenterMiddle,

		/* 表示右中位置。
		*/
		RightMiddle,

		/* 表示左下位置。
		*/
		LeftBottom,

		/* 表示中下位置。
		*/
		CenterBottom,

		/* 表示右下位置。
		*/
		RightBottom
	}Location;

public:
	int m_topSpacing;	//水印文字与图片顶部的间距（像素）
	int m_rightSpacing;	//水印文字与图片右端的间距（像素）
	int m_bottomSpacing;//水印文字与图片底部的间距（像素）
	int m_leftSpacing;	//水印文字与图片左端的间距（像素）
public:
	/*更换当前操作位图
	* @nIDResouce:位图资源标识
	* return:是否更换成功
	*/
	bool ChangeBitmap(UINT nIDResource);
	/*更换当前操作位图
	* @lpszImgFile:位图文件路径
	* return:是否更换成功
	*/
	bool ChangeBitmap(LPCTSTR lpszImgFile);
	/*获取与设备无关的内存设备上下文*/
	inline HDC GetMemoryDC()
	{
		return m_hMemDC;
	}
	/*获取当前处理位图的BITMAP结构*/
	int GetBitmap(BITMAP* pBitMap);
	/*获取水印文字的前景色*/
	COLORREF GetForeColor();
	/*设置水印文字的前景色*/
	void SetForeColor(COLORREF foreColor = 0);
	/*获取水印文字的背景色*/
	COLORREF GetBkColor();
	/*设置水印文字的背景色*/
	void SetBkColor(COLORREF bkColor = RGB(255, 255, 255));
	/*获取当前是否设置水印文字为透明背景*/
	bool IsTransparent();
	/*设置当前是否设置水印文字为透明背景*/
	void SetTransparent(bool transparent = true);
	/*获取当前用于输出水印文字的字体*/
	HFONT GetTextFont();
	/*设置当前用于输出水印文字的字体,返回上一次设置的字体*/
	HFONT SetTextFont(HFONT hFont);
	/*添加水印文字
	* @lpszText:水印文本
	* @x:水印文本相对图片左上角的水平位置(像素)
	* @y:水印文本相对图片左上角的垂直位置(像素)
	*/
	void AddWaterMark(const char * lpszText, int x, int y);
	/*根据选择的方式和上下左右的间距，自动添加水印文本到合适的位置，默认添加到图片右下角*/
	void AddWaterMark(const char * lpszText, Location type = Location::RightBottom);
	/*保存到当前操作位图文件,返回是否保存成功 -  只针对以位图文件加载的情况*/
	bool Save();
	/*保存到指定位图文件,返回是否保存成功*/
	bool Save(LPCTSTR lpszFile);
private:
	/*初始化工作*/
	void Init();
	/*根据BITMAP句柄创建位图文件信息*/
	PBITMAPINFO CreateBitmapInfoStruct(HWND hwnd, HBITMAP hBmp);
	/*创建位图文件*/
	void CreateBMPFile(HWND hwnd, LPCTSTR pszFile, PBITMAPINFO pbi, HBITMAP hBMP, HDC hDC);
private:
	string m_imgFile;		//要添加水印的图片文件
	HFONT m_hFont;			//水印文字的当前字体
	HFONT m_hOldFont;
	HFONT m_hInitFont;		//水印文字的初始字体
	COLORREF m_foreColor;	//水印文字的文本颜色
	COLORREF m_bkColor;		//水印文字的背景颜色
	bool m_isTransparant;	//水印文字是否透明
	HBITMAP m_hBmp;			//HBITMAP对象
	HBITMAP	m_hOldBmp;
	HDC	m_hMemDC;			//内存设备句柄
};
