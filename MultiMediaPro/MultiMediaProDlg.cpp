
// MultiMediaProDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MultiMediaPro.h"
#include "MultiMediaProDlg.h"
#include "afxdialogex.h"
#include "WaterMark.h"
#include "function.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WIDTHBYTES(bits)    (((bits) + 31) / 32 * 4)

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMultiMediaProDlg �Ի���



CMultiMediaProDlg::CMultiMediaProDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMultiMediaProDlg::IDD, pParent)
	, m_PicturePathName(_T(""))
	, m_watermark(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMultiMediaProDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TXT_PATH, m_PicturePathName);
	DDX_Control(pDX, IDC_MFCSHELLLIST1, m_ShelListCtrl);
	DDX_Text(pDX, IDC_EDIT2, m_watermark);
}

BEGIN_MESSAGE_MAP(CMultiMediaProDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()/*
	ON_BN_CLICKED(IDC_BUTTON1, &CMultiMediaProDlg::OnBnClickedButton1)*/
	ON_BN_CLICKED(IDC_BUTTON2, &CMultiMediaProDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON5, &CMultiMediaProDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON3, &CMultiMediaProDlg::OnBnClickedButton3)
END_MESSAGE_MAP()


// CMultiMediaProDlg ��Ϣ�������

BOOL CMultiMediaProDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������

	m_ShelListCtrl.ModifyStyle(LVS_TYPEMASK, LVS_REPORT);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CMultiMediaProDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMultiMediaProDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMultiMediaProDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//
//void CMultiMediaProDlg::OnBnClickedButton1()
//{
//	// TODO:  �ڴ���ӿؼ�֪ͨ����������
//	// TODO: Add your control notification handler code here
//	CFileDialog dlg(TRUE, _T("bmp"), _T("*.bmp"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
//		_T("bmp�ļ�(*.bmp)|*.bmp|JPEG�ļ�(*.jpg)|*.jpg|GIF�ļ�(*.gif)|*.gif|"), NULL);
//	if (dlg.DoModal() == IDOK)
//	{
//		SetDlgItemText(IDC_TXT_PATH, dlg.GetPathName());
//
//		//���þ�̬�ؼ�����ʽ��ʹ�����ʹ��λͼ����ʹλͼ��ʾ����
//		((CStatic*)GetDlgItem(IDC_STATIC_IMG))->ModifyStyle(0xF, SS_BITMAP | SS_CENTERIMAGE);
//
//		CDC *pDC = NULL;
//		pDC = GetDlgItem(IDC_STATIC_IMG)->GetDC();
//		//ShowJpgGif(pDC,dlg.GetPathName(),0,0);
//		ShowImage(pDC, dlg.GetPathName(), 0, 0);
//
//		ReleaseDC(pDC);	// �ǵ��ͷ���Դ����Ȼ�ᵼ���ڴ�й¶
//	}
//
//	return;
//}
////************************************
//// ����˵��:    ��ʾJPG��GIF��BMPͼƬ
//// ����˵��:    CDC * pDC           �豸��������
//// ����˵��:    CString strPath     Ҫ��ʾ��ͼƬ·�� 
//// ����˵��:    int x               Ҫ��ʾ��Xλ��
//// ����˵��:    int y               Ҫ��ʾ��Yλ��
//// ����ֵ:      BOOL                �ɹ�����TRUE,���򷵻�FALSE
////************************************
//BOOL CMultiMediaProDlg::ShowJpgGif(CDC* pDC, CString strPath, int x, int y)
//
//{
//	CFileStatus fstatus;
//	CFile file;
//	ULONGLONG cb;
//
//	// ���ļ�������ļ�����Ч��
//	if (!file.Open(strPath, CFile::modeRead))
//	{
//		return FALSE;
//	}
//	if (!file.GetStatus(strPath, fstatus))
//	{
//		return FALSE;
//	}
//	if ((cb = fstatus.m_size) <= 0)
//	{
//		return FALSE;
//	}
//
//	// �����ļ���С�����ڴ�ռ�,�ǵ��ͷ��ڴ�
//	HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, (unsigned int)cb);
//	if (hGlobal == NULL)
//	{
//		return FALSE;
//	}
//
//	// �����ղŷ�����ڴ�ռ�
//	LPVOID pvData = NULL;
//	pvData = GlobalLock(hGlobal);
//	if (pvData == NULL)
//	{
//		GlobalFree(hGlobal);  // �ǵ��ͷ��ڴ�
//		return FALSE;
//	}
//
//	// ���ļ��ŵ�����
//	IStream *pStm;
//	file.Read(pvData, (unsigned int)cb);
//	GlobalUnlock(hGlobal);
//	CreateStreamOnHGlobal(hGlobal, TRUE, &pStm);
//
//	// �����м���ͼƬ
//	// ��ʾJPEG��GIF��ʽ��ͼƬ��GIFֻ����ʾһ֡����������ʾ������
//	// Ҫ��ʾ����GIF��ʹ��ACTIVE�ؼ���
//	IPicture *pPic;
//	if (OleLoadPicture(pStm, (LONG)fstatus.m_size, TRUE, IID_IPicture, (LPVOID*)&pPic) != S_OK)
//	{
//		GlobalFree(hGlobal);  // �ǵ��ͷ��ڴ�
//		return FALSE;
//	}
//
//	//��ȡͼ���͸�,ע������Ŀ�͸߲���ͼ��ķֱ���
//	OLE_XSIZE_HIMETRIC hmWidth;
//	OLE_YSIZE_HIMETRIC hmHeight;
//	pPic->get_Width(&hmWidth);
//	pPic->get_Height(&hmHeight);
//
//	// ��ͼ���Ⱥ͸߶ȵ�λת��Ϊ���ص�λ
//	//#define HIMETRIC_PER_INCH 2540
//	//int nPicWidth =  MulDiv(hmWidth, GetDeviceCaps(GetDC()->m_hDC, LOGPIXELSX),2540);
//	//int nPicHeight = MulDiv(hmHeight, GetDeviceCaps(GetDC()->m_hDC, LOGPIXELSY),2540);
//
//
//	//HRESULT Render(
//	//    HDC hdc, //Handle of device context on which to render the image
//	//    long x,  //Horizontal position of image in hdc
//	//    long y,  //Vertical position of image in hdc
//	//    long cx, //Horizontal dimension of destination rectangle
//	//    long cy, //Vertical dimension of destination rectangle
//	//    OLE_XPOS_HIMETRIC xSrc,      //Horizontal offset in source picture
//	//    OLE_YPOS_HIMETRIC ySrc,      //Vertical offset in source picture
//	//    OLE_XSIZE_HIMETRIC cxSrc,    //Amount to copy horizontally in source picture
//	//    OLE_YSIZE_HIMETRIC cySrc,    //Amount to copy vertically in source picture
//	//    LPCRECT prcWBounds           //Pointer to position of destination for a metafile hdc
//	//    );
//
//	//use render function display image
//	RECT rtWnd;
//	pDC->GetWindow()->GetWindowRect(&rtWnd);
//	int iWndWidth = rtWnd.right - rtWnd.left;
//	int iWndHeight = rtWnd.bottom - rtWnd.top;
//
//	if (FAILED(pPic->Render(*pDC, x, y, iWndWidth, iWndHeight, 0, hmHeight, hmWidth, -hmHeight, NULL)))
//	{
//		pPic->Release();
//		GlobalFree(hGlobal);  // �ǵ��ͷ��ڴ�
//		return false;
//	}
//
//	pPic->Release();
//	GlobalFree(hGlobal);  // �ǵ��ͷ��ڴ�
//	return true;
//}
//
////************************************
//// ����˵��:    ��ʾJPG��GIF��BMPͼƬ
//// ����˵��:    CDC * pDC           �豸��������
//// ����˵��:    CString strPath     Ҫ��ʾ��ͼƬ·�� 
//// ����˵��:    int x               Ҫ��ʾ��Xλ��
//// ����˵��:    int y               Ҫ��ʾ��Yλ��
//// ����ֵ:      BOOL                �ɹ�����TRUE,���򷵻�FALSE
////************************************
//BOOL CMultiMediaProDlg::ShowImage(CDC* pDC, CString strPath, int x, int y)
//{
//
//	IPicture *pPic = NULL;
//	OleLoadPicturePath(CComBSTR(strPath.GetBuffer()), (LPUNKNOWN)NULL, 0, 0, IID_IPicture, (LPVOID*)&pPic);
//	if (NULL == pPic)
//	{
//		return FALSE;
//	}
//
//	// ��ȡͼ���͸�,ע������Ŀ�͸߲���ͼ��ķֱ���
//	OLE_XSIZE_HIMETRIC hmWidth;
//	OLE_YSIZE_HIMETRIC hmHeight;
//	pPic->get_Width(&hmWidth);
//	pPic->get_Height(&hmHeight);
//
//	// ��ͼ���Ⱥ͸߶ȵ�λת��Ϊ���ص�λ
//	//#define HIMETRIC_PER_INCH 2540
//	//int nPicWidth =  MulDiv(hmWidth, GetDeviceCaps(GetDC()->m_hDC, LOGPIXELSX),2540);
//	//int nPicHeight = MulDiv(hmHeight, GetDeviceCaps(GetDC()->m_hDC, LOGPIXELSY),2540);
//
//	// ��ȡ��ʾͼƬ���ڵĿ�Ⱥ͸߶�
//	RECT rtWnd;
//	pDC->GetWindow()->GetWindowRect(&rtWnd);
//	int iWndWidth = rtWnd.right - rtWnd.left;
//	int iWndHeight = rtWnd.bottom - rtWnd.top;
//
//	if (FAILED(pPic->Render(*pDC, x, y, iWndWidth, iWndHeight, 0, hmHeight, hmWidth, -hmHeight, NULL)))
//	{
//		pPic->Release();
//		return false;
//	}
//
//	//�ǵ��ͷ���Դ����Ȼ�ᵼ���ڴ�й¶
//	pPic->Release();
//
//	return true;
//}

void CMultiMediaProDlg::ScanDiskFile(const CString& strPath)
{
	CFileFind find;
	CString strTemp = strPath;
	CString strDirectory = strPath + _T("\\") + _T("\\*.*");
	CString strFile;

	BOOL IsFind = find.FindFile(strDirectory);
	while (IsFind)
	{


		IsFind = find.FindNextFile();
		// �����"." ��ɨ��
		if (find.IsDots())
			continue;
		// �������Ŀ¼,����ɨ���Ŀ¼
		else if (find.IsDirectory())
		{
			strFile = find.GetFileName();
			strTemp = strTemp + _T("\\") + strFile;
			ScanDiskFile(strTemp);
		}
		// �ļ�
		else
		{
			strFile = strPath + _T("\\") + find.GetFileName();

			AddWaterMark(strFile);
			// �˴�Ҳ���Ը�����Ӧ����չ��������

			// ......
			;
			//m_vStrAllFiles.push_back(strFile);
		}

	}

	find.Close();
}

void CMultiMediaProDlg::ScanDiskFile1(const CString& strPath)
{
	CFileFind find;
	CString strTemp = strPath;
	CString strDirectory = strPath + _T("\\") + _T("\\*.*");
	CString strFile;

	BOOL IsFind = find.FindFile(strDirectory);
	while (IsFind)
	{


		IsFind = find.FindNextFile();
		// �����"." ��ɨ��
		if (find.IsDots())
			continue;
		// �������Ŀ¼,����ɨ���Ŀ¼
		else if (find.IsDirectory())
		{
			strFile = find.GetFileName();
			strTemp = strTemp + _T("\\") + strFile;
			ScanDiskFile(strTemp);
		}
		// �ļ�
		else
		{
			strFile = strPath + _T("\\") + find.GetFileName();
		
			m_PhotoImage.LoadFromFile(strFile);
			if (m_PhotoImage.GetBitCount() != 24)
			{
				AfxMessageBox("���򿪵�ͼƬ���ǲ�ɫͼ!");
				return;
			}

			CPoint point;
			CSize size;
			point.x = 0;
			point.y = 0;
			LONG lWidth = m_PhotoImage.GetWidth();    //��ûҶ�ͼ��Ŀ��
			LONG lHeight = m_PhotoImage.GetHeight();  //��ûҶ�ͼ��ĸ߶�
			size.cx = lWidth;
			size.cy = lHeight;
			CDC *pDC = GetDC();
			//m_PhotoImage.Draw(pDC, point, size);//���ƴ���ǰ��ͼ��

			m_PhotoImage.RgbToGrade();

			CPoint point1;
			point1.x = lWidth + 20;  //+20��Ϊ��������ͼ����ʾʱ�и���϶
			point1.y = 0;
			//m_PhotoImage.Draw(pDC, point1, size);//���ƴ�����ͼ��
			m_PhotoImage.SaveToFile(strFile);
			// �˴�Ҳ���Ը�����Ӧ����չ��������

			// ......
			;
			//m_vStrAllFiles.push_back(strFile);
		}

	}

	find.Close();
}


void CMultiMediaProDlg::AddWaterMark(CString path)
{

	UpdateData(TRUE);

	CWaterMark mark(path);

	//����һ������GDI����

	HFONT hFont = CreateFontA(50, 20, 0, 0, FW_BOLD, 1, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, _T("���Ŀ���"));

	//����ˮӡ�����ı�����

	mark.SetTextFont(hFont);

	//����ˮӡ�����ı���ɫ

	mark.SetForeColor(RGB(204, 0, 0));

	mark.SetBkColor(RGB(0, 0, 222));

	mark.SetTransparent(TRUE);

	//���һ��ˮӡ�ı���λͼ�����½ǣ�Ҳ������Ӷ���

	//�Զ����ˮӡʱ��������ͼƬ�������Ҷ��˵ļ����

	//mark.m_leftSpacing, mark.m_rightSpacing, mark.m_topSpacing, mark.m_bottomSpacing����

	mark.AddWaterMark(m_watermark, CWaterMark::Location::RightBottom);

	//BITMAP bmp;

	//mark.GetBitmap(&bmp);

	//CDC* pDC = GetDC();

	////����������ͻ�����ͼDC

	//pDC->BitBlt(0, 0, bmp.bmWidth, bmp.bmHeight, CDC::FromHandle(mark.GetMemoryDC()), 0, 0, SRCCOPY);

	//ReleaseDC(pDC);

	//��������ָ��λͼ�ļ�
	mark.Save();
	/*if (mark.Save() == FALSE)
	{
		AfxMessageBox("����ʧ��!!!");
	}*/

}

void CMultiMediaProDlg::OnBnClickedButton2()
{


	ScanDiskFile(m_PicturePathName);
	
}


void CMultiMediaProDlg::OnBnClickedButton5()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	char szPath[MAX_PATH];     //���ѡ���Ŀ¼·��   
	CString str;

	ZeroMemory(szPath, sizeof(szPath));

	BROWSEINFO bi;
	bi.hwndOwner = m_hWnd;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = szPath;
	bi.lpszTitle = "��ѡ����Ҫ�����Ŀ¼��";
	bi.ulFlags = 0;
	bi.lpfn = NULL;
	bi.lParam = 0;
	bi.iImage = 0;
	//����ѡ��Ŀ¼�Ի���  
	LPITEMIDLIST lp = SHBrowseForFolder(&bi);

	if (lp && SHGetPathFromIDList(lp, szPath))
	{
		/*str.Format("ѡ���Ŀ¼Ϊ %s", szPath);
		AfxMessageBox(str);*/
		;

	}
	else
		AfxMessageBox("��Ч��Ŀ¼��������ѡ��");

	m_ShelListCtrl.DisplayFolder(szPath);
	m_PicturePathName = szPath;
	UpdateData(FALSE);
	//m_ShelListCtrl.ModifyStyle(LVS_TYPEMASK, LVS_REPORT);

	return;
}


void CMultiMediaProDlg::OnBnClickedButton3()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	//����Ƭͼ��
	ScanDiskFile1(m_PicturePathName);
}


void CMultiMediaProDlg::OnBnClickedButton4()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	ScanDiskFile2(m_PicturePathName);
}

void CMultiMediaProDlg::ScanDiskFile2(const CString& strPath)
{
	CFileFind find;
	CString strTemp = strPath;
	CString strDirectory = strPath + _T("\\") + _T("\\*.*");
	CString strFile;

	BOOL IsFind = find.FindFile(strDirectory);
	while (IsFind)
	{


		IsFind = find.FindNextFile();
		// �����"." ��ɨ��
		if (find.IsDots())
			continue;
		// �������Ŀ¼,����ɨ���Ŀ¼
		else if (find.IsDirectory())
		{
			strFile = find.GetFileName();
			strTemp = strTemp + _T("\\") + strFile;
			ScanDiskFile(strTemp);
		}
		// �ļ�
		else
		{
			strFile = strPath + _T("\\") + find.GetFileName();
			 
			CDib cbmp;
			CFile file;
			file.Open(strFile, CFile::modeReadWrite);
			cbmp.LoadFile(file);
			//cbmp.LoadFromFile(strFile);
			//Rotate(cbmp.lpdib, cbmp.lpDIBBits, cbmp.GetWidth(), cbmp.GetHeight(), WIDTHBYTES(cbmp.GetWidth() * 8), cbmp.GetPalSize(cbmp.lpdib), cbmp.GetHeight(), cbmp.GetWidth(), WIDTHBYTES(cbmp.GetHeight() * 8), 0, 1);
			//cbmp.SaveToFile(strFile);
			//// ......
			RotateDIB2(cbmp.lpdib, 90, cbmp.lpDIBBits, cbmp.GetWidth(), cbmp.GetHeight(), cbmp.GetPalSize(cbmp.lpdib));
			cbmp.SaveToFile(strFile);
			;
			//m_vStrAllFiles.push_back(strFile);
		}

	}

	find.Close();
}

