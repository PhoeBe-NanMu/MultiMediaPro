
// MultiMediaProDlg.h : ͷ�ļ�
//

#pragma once
#include "afxshelllistctrl.h"
#include"Dib.h"
#define WIDTHBYTES(bits)    (((bits) + 31) / 32 * 4)
// CMultiMediaProDlg �Ի���
class CMultiMediaProDlg : public CDialogEx
{
// ����
public:
	CMultiMediaProDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MULTIMEDIAPRO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	CDib m_PhotoImage;//��Ƭͼ��
	
	BOOL flag;//����Ƭͼ���Ǳ���
	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	BOOL ShowJpgGif(CDC* pDC, CString strPath, int x, int y);
	BOOL ShowImage(CDC* pDC, CString strPath, int x, int y);
	void OnBnClickedBtnBrowse();

	void AddWaterMark(CString path);
	void ScanDiskFile(const CString& strPath);
	void ScanDiskFile1(const CString& strPath);
	void ScanDiskFile2(const CString& strPath);
	afx_msg void OnBnClickedButton2();
	CString m_PicturePathName;
	afx_msg void OnBnClickedButton5();
	CMFCShellListCtrl m_ShelListCtrl;
	CString m_watermark;
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
};
