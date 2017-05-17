/* Copyright (C) wuchen 2011 ALL RIGHTS RESERVED.
* Author:niewucai
* Email:niewucai@126.com
* Last Update:2011/05/11
* ʵ�ֶ�λͼͼƬ�������ˮӡ���� -- ͷ�ļ�
*/
#pragma once
//#include <stdafx.h>
#include <string>
using namespace std;


class CWaterMark{
public:
	/*���췽��
	* @lpszImgFile:Ҫ���ˮӡ��λͼͼƬ�ļ�
	*/
	CWaterMark(LPCTSTR lpszImgFile);
	/*���췽��
	* @nIDResource:λͼ��Դ��ʶ
	*/
	CWaterMark(UINT nIDResource);
	~CWaterMark();

	//ˮӡ����λ��ö��
	typedef enum
	{
		/* ��ʾ����λ�á�
		*/
		LeftTop = 1,

		/* ��ʾ����λ�á�
		*/
		CenterTop,

		/* ��ʾ����λ�á�
		*/
		RightTop,

		/* ��ʾ����λ�á�
		*/
		LeftMiddle,

		/* ��ʾ����λ�á�
		*/
		CenterMiddle,

		/* ��ʾ����λ�á�
		*/
		RightMiddle,

		/* ��ʾ����λ�á�
		*/
		LeftBottom,

		/* ��ʾ����λ�á�
		*/
		CenterBottom,

		/* ��ʾ����λ�á�
		*/
		RightBottom
	}Location;

public:
	int m_topSpacing;	//ˮӡ������ͼƬ�����ļ�ࣨ���أ�
	int m_rightSpacing;	//ˮӡ������ͼƬ�Ҷ˵ļ�ࣨ���أ�
	int m_bottomSpacing;//ˮӡ������ͼƬ�ײ��ļ�ࣨ���أ�
	int m_leftSpacing;	//ˮӡ������ͼƬ��˵ļ�ࣨ���أ�
public:
	/*������ǰ����λͼ
	* @nIDResouce:λͼ��Դ��ʶ
	* return:�Ƿ�����ɹ�
	*/
	bool ChangeBitmap(UINT nIDResource);
	/*������ǰ����λͼ
	* @lpszImgFile:λͼ�ļ�·��
	* return:�Ƿ�����ɹ�
	*/
	bool ChangeBitmap(LPCTSTR lpszImgFile);
	/*��ȡ���豸�޹ص��ڴ��豸������*/
	inline HDC GetMemoryDC()
	{
		return m_hMemDC;
	}
	/*��ȡ��ǰ����λͼ��BITMAP�ṹ*/
	int GetBitmap(BITMAP* pBitMap);
	/*��ȡˮӡ���ֵ�ǰ��ɫ*/
	COLORREF GetForeColor();
	/*����ˮӡ���ֵ�ǰ��ɫ*/
	void SetForeColor(COLORREF foreColor = 0);
	/*��ȡˮӡ���ֵı���ɫ*/
	COLORREF GetBkColor();
	/*����ˮӡ���ֵı���ɫ*/
	void SetBkColor(COLORREF bkColor = RGB(255, 255, 255));
	/*��ȡ��ǰ�Ƿ�����ˮӡ����Ϊ͸������*/
	bool IsTransparent();
	/*���õ�ǰ�Ƿ�����ˮӡ����Ϊ͸������*/
	void SetTransparent(bool transparent = true);
	/*��ȡ��ǰ�������ˮӡ���ֵ�����*/
	HFONT GetTextFont();
	/*���õ�ǰ�������ˮӡ���ֵ�����,������һ�����õ�����*/
	HFONT SetTextFont(HFONT hFont);
	/*���ˮӡ����
	* @lpszText:ˮӡ�ı�
	* @x:ˮӡ�ı����ͼƬ���Ͻǵ�ˮƽλ��(����)
	* @y:ˮӡ�ı����ͼƬ���ϽǵĴ�ֱλ��(����)
	*/
	void AddWaterMark(const char * lpszText, int x, int y);
	/*����ѡ��ķ�ʽ���������ҵļ�࣬�Զ����ˮӡ�ı������ʵ�λ�ã�Ĭ����ӵ�ͼƬ���½�*/
	void AddWaterMark(const char * lpszText, Location type = Location::RightBottom);
	/*���浽��ǰ����λͼ�ļ�,�����Ƿ񱣴�ɹ� -  ֻ�����λͼ�ļ����ص����*/
	bool Save();
	/*���浽ָ��λͼ�ļ�,�����Ƿ񱣴�ɹ�*/
	bool Save(LPCTSTR lpszFile);
private:
	/*��ʼ������*/
	void Init();
	/*����BITMAP�������λͼ�ļ���Ϣ*/
	PBITMAPINFO CreateBitmapInfoStruct(HWND hwnd, HBITMAP hBmp);
	/*����λͼ�ļ�*/
	void CreateBMPFile(HWND hwnd, LPCTSTR pszFile, PBITMAPINFO pbi, HBITMAP hBMP, HDC hDC);
private:
	string m_imgFile;		//Ҫ���ˮӡ��ͼƬ�ļ�
	HFONT m_hFont;			//ˮӡ���ֵĵ�ǰ����
	HFONT m_hOldFont;
	HFONT m_hInitFont;		//ˮӡ���ֵĳ�ʼ����
	COLORREF m_foreColor;	//ˮӡ���ֵ��ı���ɫ
	COLORREF m_bkColor;		//ˮӡ���ֵı�����ɫ
	bool m_isTransparant;	//ˮӡ�����Ƿ�͸��
	HBITMAP m_hBmp;			//HBITMAP����
	HBITMAP	m_hOldBmp;
	HDC	m_hMemDC;			//�ڴ��豸���
};
