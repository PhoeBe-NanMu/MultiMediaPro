
// MultiMediaProDlg.cpp : 实现文件
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

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CMultiMediaProDlg 对话框



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


// CMultiMediaProDlg 消息处理程序

BOOL CMultiMediaProDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码

	m_ShelListCtrl.ModifyStyle(LVS_TYPEMASK, LVS_REPORT);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMultiMediaProDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMultiMediaProDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//
//void CMultiMediaProDlg::OnBnClickedButton1()
//{
//	// TODO:  在此添加控件通知处理程序代码
//	// TODO: Add your control notification handler code here
//	CFileDialog dlg(TRUE, _T("bmp"), _T("*.bmp"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
//		_T("bmp文件(*.bmp)|*.bmp|JPEG文件(*.jpg)|*.jpg|GIF文件(*.gif)|*.gif|"), NULL);
//	if (dlg.DoModal() == IDOK)
//	{
//		SetDlgItemText(IDC_TXT_PATH, dlg.GetPathName());
//
//		//设置静态控件的样式，使其可以使用位图，并使位图显示居中
//		((CStatic*)GetDlgItem(IDC_STATIC_IMG))->ModifyStyle(0xF, SS_BITMAP | SS_CENTERIMAGE);
//
//		CDC *pDC = NULL;
//		pDC = GetDlgItem(IDC_STATIC_IMG)->GetDC();
//		//ShowJpgGif(pDC,dlg.GetPathName(),0,0);
//		ShowImage(pDC, dlg.GetPathName(), 0, 0);
//
//		ReleaseDC(pDC);	// 记得释放资源，不然会导致内存泄露
//	}
//
//	return;
//}
////************************************
//// 方法说明:    显示JPG和GIF、BMP图片
//// 参数说明:    CDC * pDC           设备环境对象
//// 参数说明:    CString strPath     要显示的图片路径 
//// 参数说明:    int x               要显示的X位置
//// 参数说明:    int y               要显示的Y位置
//// 返回值:      BOOL                成功返回TRUE,否则返回FALSE
////************************************
//BOOL CMultiMediaProDlg::ShowJpgGif(CDC* pDC, CString strPath, int x, int y)
//
//{
//	CFileStatus fstatus;
//	CFile file;
//	ULONGLONG cb;
//
//	// 打开文件并检测文件的有效性
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
//	// 根据文件大小分配内存空间,记得释放内存
//	HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, (unsigned int)cb);
//	if (hGlobal == NULL)
//	{
//		return FALSE;
//	}
//
//	// 锁定刚才分配的内存空间
//	LPVOID pvData = NULL;
//	pvData = GlobalLock(hGlobal);
//	if (pvData == NULL)
//	{
//		GlobalFree(hGlobal);  // 记得释放内存
//		return FALSE;
//	}
//
//	// 将文件放到流中
//	IStream *pStm;
//	file.Read(pvData, (unsigned int)cb);
//	GlobalUnlock(hGlobal);
//	CreateStreamOnHGlobal(hGlobal, TRUE, &pStm);
//
//	// 从流中加载图片
//	// 显示JPEG和GIF格式的图片，GIF只能显示一帧，还不能显示动画，
//	// 要显示动画GIF请使用ACTIVE控件。
//	IPicture *pPic;
//	if (OleLoadPicture(pStm, (LONG)fstatus.m_size, TRUE, IID_IPicture, (LPVOID*)&pPic) != S_OK)
//	{
//		GlobalFree(hGlobal);  // 记得释放内存
//		return FALSE;
//	}
//
//	//获取图像宽和高,注意这里的宽和高不是图像的分辨率
//	OLE_XSIZE_HIMETRIC hmWidth;
//	OLE_YSIZE_HIMETRIC hmHeight;
//	pPic->get_Width(&hmWidth);
//	pPic->get_Height(&hmHeight);
//
//	// 将图像宽度和高度单位转化为像素单位
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
//		GlobalFree(hGlobal);  // 记得释放内存
//		return false;
//	}
//
//	pPic->Release();
//	GlobalFree(hGlobal);  // 记得释放内存
//	return true;
//}
//
////************************************
//// 方法说明:    显示JPG和GIF、BMP图片
//// 参数说明:    CDC * pDC           设备环境对象
//// 参数说明:    CString strPath     要显示的图片路径 
//// 参数说明:    int x               要显示的X位置
//// 参数说明:    int y               要显示的Y位置
//// 返回值:      BOOL                成功返回TRUE,否则返回FALSE
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
//	// 获取图像宽和高,注意这里的宽和高不是图像的分辨率
//	OLE_XSIZE_HIMETRIC hmWidth;
//	OLE_YSIZE_HIMETRIC hmHeight;
//	pPic->get_Width(&hmWidth);
//	pPic->get_Height(&hmHeight);
//
//	// 将图像宽度和高度单位转化为像素单位
//	//#define HIMETRIC_PER_INCH 2540
//	//int nPicWidth =  MulDiv(hmWidth, GetDeviceCaps(GetDC()->m_hDC, LOGPIXELSX),2540);
//	//int nPicHeight = MulDiv(hmHeight, GetDeviceCaps(GetDC()->m_hDC, LOGPIXELSY),2540);
//
//	// 获取显示图片窗口的宽度和高度
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
//	//记得释放资源，不然会导致内存泄露
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
		// 如果是"." 则不扫描
		if (find.IsDots())
			continue;
		// 如果是是目录,继续扫描此目录
		else if (find.IsDirectory())
		{
			strFile = find.GetFileName();
			strTemp = strTemp + _T("\\") + strFile;
			ScanDiskFile(strTemp);
		}
		// 文件
		else
		{
			strFile = strPath + _T("\\") + find.GetFileName();

			AddWaterMark(strFile);
			// 此处也可以根据相应的扩展名来设置

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
		// 如果是"." 则不扫描
		if (find.IsDots())
			continue;
		// 如果是是目录,继续扫描此目录
		else if (find.IsDirectory())
		{
			strFile = find.GetFileName();
			strTemp = strTemp + _T("\\") + strFile;
			ScanDiskFile(strTemp);
		}
		// 文件
		else
		{
			strFile = strPath + _T("\\") + find.GetFileName();
		
			m_PhotoImage.LoadFromFile(strFile);
			if (m_PhotoImage.GetBitCount() != 24)
			{
				AfxMessageBox("所打开的图片不是彩色图!");
				return;
			}

			CPoint point;
			CSize size;
			point.x = 0;
			point.y = 0;
			LONG lWidth = m_PhotoImage.GetWidth();    //获得灰度图像的宽度
			LONG lHeight = m_PhotoImage.GetHeight();  //获得灰度图像的高度
			size.cx = lWidth;
			size.cy = lHeight;
			CDC *pDC = GetDC();
			//m_PhotoImage.Draw(pDC, point, size);//绘制处理前的图像

			m_PhotoImage.RgbToGrade();

			CPoint point1;
			point1.x = lWidth + 20;  //+20是为了让两个图像显示时有个间隙
			point1.y = 0;
			//m_PhotoImage.Draw(pDC, point1, size);//绘制处理后的图像
			m_PhotoImage.SaveToFile(strFile);
			// 此处也可以根据相应的扩展名来设置

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

	//创建一个字体GDI对象

	HFONT hFont = CreateFontA(50, 20, 0, 0, FW_BOLD, 1, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, _T("华文楷体"));

	//设置水印文字文本字体

	mark.SetTextFont(hFont);

	//设置水印文字文本颜色

	mark.SetForeColor(RGB(204, 0, 0));

	mark.SetBkColor(RGB(0, 0, 222));

	mark.SetTransparent(TRUE);

	//添加一条水印文本到位图的右下角，也可以添加多条

	//自动添加水印时，文字与图片上下左右顶端的间距由

	//mark.m_leftSpacing, mark.m_rightSpacing, mark.m_topSpacing, mark.m_bottomSpacing控制

	mark.AddWaterMark(m_watermark, CWaterMark::Location::RightBottom);

	//BITMAP bmp;

	//mark.GetBitmap(&bmp);

	//CDC* pDC = GetDC();

	////拷贝结果到客户区视图DC

	//pDC->BitBlt(0, 0, bmp.bmWidth, bmp.bmHeight, CDC::FromHandle(mark.GetMemoryDC()), 0, 0, SRCCOPY);

	//ReleaseDC(pDC);

	//保存结果到指定位图文件
	mark.Save();
	/*if (mark.Save() == FALSE)
	{
		AfxMessageBox("保存失败!!!");
	}*/

}

void CMultiMediaProDlg::OnBnClickedButton2()
{


	ScanDiskFile(m_PicturePathName);
	
}


void CMultiMediaProDlg::OnBnClickedButton5()
{
	// TODO:  在此添加控件通知处理程序代码
	char szPath[MAX_PATH];     //存放选择的目录路径   
	CString str;

	ZeroMemory(szPath, sizeof(szPath));

	BROWSEINFO bi;
	bi.hwndOwner = m_hWnd;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = szPath;
	bi.lpszTitle = "请选择需要打包的目录：";
	bi.ulFlags = 0;
	bi.lpfn = NULL;
	bi.lParam = 0;
	bi.iImage = 0;
	//弹出选择目录对话框  
	LPITEMIDLIST lp = SHBrowseForFolder(&bi);

	if (lp && SHGetPathFromIDList(lp, szPath))
	{
		/*str.Format("选择的目录为 %s", szPath);
		AfxMessageBox(str);*/
		;

	}
	else
		AfxMessageBox("无效的目录，请重新选择");

	m_ShelListCtrl.DisplayFolder(szPath);
	m_PicturePathName = szPath;
	UpdateData(FALSE);
	//m_ShelListCtrl.ModifyStyle(LVS_TYPEMASK, LVS_REPORT);

	return;
}


void CMultiMediaProDlg::OnBnClickedButton3()
{
	// TODO:  在此添加控件通知处理程序代码

	//打开照片图像
	ScanDiskFile1(m_PicturePathName);
}


void CMultiMediaProDlg::OnBnClickedButton4()
{
	// TODO:  在此添加控件通知处理程序代码

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
		// 如果是"." 则不扫描
		if (find.IsDots())
			continue;
		// 如果是是目录,继续扫描此目录
		else if (find.IsDirectory())
		{
			strFile = find.GetFileName();
			strTemp = strTemp + _T("\\") + strFile;
			ScanDiskFile(strTemp);
		}
		// 文件
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

