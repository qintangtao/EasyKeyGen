
// EasyKeyGenDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "EasyKeyGen.h"
#include "EasyKeyGenDlg.h"
#include "afxdialogex.h"

#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//字节数组转16进制字符串
std::string bytesToHexString(const BYTE* bytes, const int length)
{
	if (bytes == NULL) {
		return "";
	}
	std::string buff;
	const int len = length;
	for (int j = 0; j < len; j++) {
		int high = bytes[j] / 16, low = bytes[j] % 16;
		buff += (high < 10) ? ('0' + high) : ('A' + high - 10);
		buff += (low < 10) ? ('0' + low) : ('A' + low - 10);
	}
	return buff;
}

static const char base64Char[] =
"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

char* base64Encode(char const* origSigned, unsigned origLength) {
	unsigned char const* orig = (unsigned char const*)origSigned; // in case any input bytes have the MSB set
	if (orig == NULL) return NULL;

	unsigned const numOrig24BitValues = origLength / 3;
	bool havePadding = origLength > numOrig24BitValues * 3;
	bool havePadding2 = origLength == numOrig24BitValues * 3 + 2;
	unsigned const numResultBytes = 4 * (numOrig24BitValues + havePadding);
	char* result = new char[numResultBytes + 1]; // allow for trailing '\0'

	// Map each full group of 3 input bytes into 4 output base-64 characters:
	unsigned i;
	for (i = 0; i < numOrig24BitValues; ++i) {
		result[4 * i + 0] = base64Char[(orig[3 * i] >> 2) & 0x3F];
		result[4 * i + 1] = base64Char[(((orig[3 * i] & 0x3) << 4) | (orig[3 * i + 1] >> 4)) & 0x3F];
		result[4 * i + 2] = base64Char[((orig[3 * i + 1] << 2) | (orig[3 * i + 2] >> 6)) & 0x3F];
		result[4 * i + 3] = base64Char[orig[3 * i + 2] & 0x3F];
	}

	// Now, take padding into account.  (Note: i == numOrig24BitValues)
	if (havePadding) {
		result[4 * i + 0] = base64Char[(orig[3 * i] >> 2) & 0x3F];
		if (havePadding2) {
			result[4 * i + 1] = base64Char[(((orig[3 * i] & 0x3) << 4) | (orig[3 * i + 1] >> 4)) & 0x3F];
			result[4 * i + 2] = base64Char[(orig[3 * i + 1] << 2) & 0x3F];
		}
		else {
			result[4 * i + 1] = base64Char[((orig[3 * i] & 0x3) << 4) & 0x3F];
			result[4 * i + 2] = '=';
		}
		result[4 * i + 3] = '=';
	}

	result[numResultBytes] = '\0';
	return result;
}


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CEasyKeyGenDlg 对话框



CEasyKeyGenDlg::CEasyKeyGenDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_EASYKEYGEN_DIALOG, pParent)
{
	//m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON_KEY);
}

void CEasyKeyGenDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_SYSTEM_TYPE, m_combo_system_type);
	DDX_Control(pDX, IDC_DATETIMEPICKER_CODE_TIME, m_date_code);
	DDX_Control(pDX, IDC_DATETIMEPICKER_START_TIME, m_date_start);
	DDX_Control(pDX, IDC_EDIT_KEY, m_edit_key);
	DDX_Control(pDX, IDC_EDIT_NAME, m_edit_name);
	DDX_Control(pDX, IDC_EDIT_EMAIL, m_edit_email);
	DDX_Control(pDX, IDC_EDIT_DAYS, m_edit_days);
	DDX_Control(pDX, IDC_STATIC_HEAD, m_pic_head);
}

BEGIN_MESSAGE_MAP(CEasyKeyGenDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CEasyKeyGenDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CEasyKeyGenDlg 消息处理程序


void CEasyKeyGenDlg::ReadIniFile()
{
	TCHAR exeFullPath[MAX_PATH]; // Full path   
	GetModuleFileName(NULL, exeFullPath, MAX_PATH);
	std::wstring strFullPath = (std::wstring)(exeFullPath);
	int nStart = strFullPath.find_last_of(TEXT("\\"));

	CString iniPath = exeFullPath;
	iniPath = iniPath.Mid(0, nStart + 1) + _T("settings.ini");

	CString str;
	CTime time;
	CTime now = CTime::GetCurrentTime();
	int ret;

	ret = GetPrivateProfileInt(_T("sys"), _T("os"), 4, iniPath);
	m_combo_system_type.SetCurSel(ret);

	GetPrivateProfileString(_T("sys"), _T("name"), _T(""), str.GetBuffer(MAX_PATH), MAX_PATH, iniPath);
	str.ReleaseBuffer();
	m_edit_name.SetWindowText(str);

	GetPrivateProfileString(_T("sys"), _T("email"), _T(""), str.GetBuffer(MAX_PATH), MAX_PATH, iniPath);
	str.ReleaseBuffer();
	m_edit_email.SetWindowText(str);

	time = GetPrivateProfileInt(_T("sys"), _T("date_code"), now.GetTime(), iniPath);
	m_date_code.SetTime(&time);

	time = GetPrivateProfileInt(_T("sys"), _T("date_start"), now.GetTime(), iniPath);
	m_date_start.GetTime(time);

	ret = GetPrivateProfileInt(_T("sys"), _T("days"), 9999, iniPath);
	str.Format(_T("%d"), ret);
	m_edit_days.SetWindowText(str);
}

void CEasyKeyGenDlg::WriteIniFile()
{
	TCHAR exeFullPath[MAX_PATH]; // Full path   
	GetModuleFileName(NULL, exeFullPath, MAX_PATH);
	std::wstring strFullPath = (std::wstring)(exeFullPath);
	int nStart = strFullPath.find_last_of(TEXT("\\"));

	CString iniPath = exeFullPath;
	iniPath = iniPath.Mid(0, nStart + 1) + _T("settings.ini");

	CString str;
	CTime time;

	str.Format(_T("%d"), m_combo_system_type.GetCurSel());
	WritePrivateProfileString(_T("sys"), _T("os"), str, iniPath);

	m_edit_name.GetWindowText(str);
	WritePrivateProfileString(_T("sys"), _T("name"), str, iniPath);

	m_edit_email.GetWindowText(str);
	WritePrivateProfileString(_T("sys"), _T("email"), str, iniPath);

	m_date_code.GetTime(time);
	str.Format(_T("%I64d"), time.GetTime());
	WritePrivateProfileString(_T("sys"), _T("date_code"), str, iniPath);

	m_date_start.GetTime(time);
	str.Format(_T("%I64d"), time.GetTime());
	WritePrivateProfileString(_T("sys"), _T("date_start"), str, iniPath);

	m_edit_days.GetWindowText(str);
	WritePrivateProfileString(_T("sys"), _T("days"), str, iniPath);
}

BOOL CEasyKeyGenDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// TODO: 在此添加额外的初始化代码
	SetWindowText(_T("EasyKeyGen"));
	ModifyStyle(WS_THICKFRAME, 0, NULL);

	HBITMAP hBitmap;
	// 显示资源中的图像（仅限位图BMP）
	hBitmap = (HBITMAP)LoadImage(AfxGetInstanceHandle(),
		MAKEINTRESOURCE(IDB_BITMAP_HEAD),
		IMAGE_BITMAP, 0, 0,
		LR_LOADMAP3DCOLORS);
	m_pic_head.SetBitmap(hBitmap);

	m_combo_system_type.AddString(_T("linux"));
	m_combo_system_type.AddString(_T("arm"));
	m_combo_system_type.AddString(_T("android"));
	m_combo_system_type.AddString(_T("ios"));
	m_combo_system_type.AddString(_T("windows"));
	m_combo_system_type.SetItemHeight(0, 18);
	
	m_date_code.SetFormat(_T("yyyy-MM-dd HH:mm:ss"));
	m_date_start.SetFormat(_T("yyyy-MM-dd HH:mm:ss"));

	ReadIniFile();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CEasyKeyGenDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	WriteIniFile();

	CDialogEx::OnCancel();
}

void CEasyKeyGenDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CEasyKeyGenDlg::OnPaint()
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
HCURSOR CEasyKeyGenDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CEasyKeyGenDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	//CDialogEx::OnOK();

/*

9A BB 8A CC																						(头标志(FFFFFFFF - 65447533:eDu3)
88 96 91 9B																						(系统类型(0x77696E64:wind,
																										 0x6C696E75:linu,
																										 0x41524D:ARM,
																										 0x616E6472:andr,
																										 0x494F53:IOS))
80 DD B1 59  																					(Start Time (秒) 0x59B1DD80)
A6 4D FB 70 																					(End Time (秒)   0xFFFFFFFF-0xA64DFB70=0x59B2048F)
EA																								(程序名长度(FF-EA=0x15 -> 21))
45 61 73 79 49 50 43 61 6D 65 72 61 5F 52 54 53 50 2E 65 78 65  								(程序名(EasyIPCamera_RTSP.exe))
57 0C 56 A0  																					(Code Time (秒) 570C56A0)
D8 F0 																							(失效时间(天数) FFFF-D8F0=270F -> 9999 永不失效)
E0                 																				(邮箱名长度(FF-E0=0x1F -> 31))
47 61 76 69 6E 26 42 61 62 6F 73 61 40 32 30 31 36 45 61 73 79 44 61 72 77 69 6E 54 65 61 6D   （邮箱(Gavin&Babosa@2016EasyDarwinTeam)）
65 61 73 79                                         			 								(结尾标志(easy))

dword1 = 0x9ABB8ACC
head1 = 0xFFFFFFFF - 0x9ABB8ACC = 0x65447533(eDu3)

dword2 = 0x8896919B
system = 0xFFFFFFFF - 0x8896919B = 0x77696E64(wind)
								 = 0x41524D(ARM)
								 = 0x616E6472(andr)
								 = 0x494F53(IOS)
								 = 0x6C696E75(linu)

Start Time
					  0x59B1DD80  2017-09-08 08:00:00
End Time
0xFFFFFFFF-0xA64DFB70=0x59B2048F  2017-09-08 10:46:39

Code Time
					  0x570C56A0  2016-04-12 10:00:00

D8 F0  (失效时间(天数) FFFF-D8F0=270F -> 9999 永不失效)
End Time - Start Time =
0x59B2048F - 0x59B1DD80 = 0x270F (FFFF-0x270F=D8F0)
*/
	CByteArray data;
	CString str;
	CString str2;
	CTime time;

	//(头标志(0xFFFFFFFF - 0x65447533:eDu3)
	data.Add(0xFF - 'e');
	data.Add(0xFF - 'D');
	data.Add(0xFF - 'u');
	data.Add(0xFF - '3');

	//m_combo_system_type.GetLBText(m_combo_system_type.GetCurSel(), str2);
	m_combo_system_type.GetWindowText(str2);

	/*
	系统类型
		0x77696E64:wind
		0x6C696E75:linu
		0x41524D:ARM
		0x616E6472:andr
		0x494F53:IOS
	*/
	if (_T("windows") == str2)
	{
		data.Add(0xFF - 'w');
		data.Add(0xFF - 'i');
		data.Add(0xFF - 'n');
		data.Add(0xFF - 'd');
	}
	else if (_T("linux") == str2)
	{
		data.Add(0xFF - 'l');
		data.Add(0xFF - 'i');
		data.Add(0xFF - 'n');
		data.Add(0xFF - 'u');
	}
	else if (_T("arm") == str2)
	{
		data.Add(0xFF - 'A');
		data.Add(0xFF - 'R');
		data.Add(0xFF - 'M');
	}
	else if (_T("android") == str2)
	{
		data.Add(0xFF - 'a');
		data.Add(0xFF - 'n');
		data.Add(0xFF - 'd');
		data.Add(0xFF - 'r');
	}
	else if (_T("ios") == str2)
	{
		data.Add(0xFF - 'I');
		data.Add(0xFF - 'O');
		data.Add(0xFF - 'S');
	}
	else 
	{
		m_edit_key.SetWindowText(_T("无效的系统类型!"));
		return;
	}
		 
	m_date_start.GetTime(time);

	//(Start Time(秒) 59B1DD80)
	//0x59B1DD80  2017-09-08 08:00:00
	DWORD dwTimeStart = (DWORD)time.GetTime();
	data.Add(BYTE(dwTimeStart));
	data.Add(BYTE(dwTimeStart >> 8));
	data.Add(BYTE(dwTimeStart >> 16));
	data.Add(BYTE(dwTimeStart >> 24));
	
	m_edit_days.GetWindowText(str2);
	WORD wTimeDays = _ttoi(str2);
	if (0 == wTimeDays)
	{
		m_edit_key.SetWindowText(_T("无效的有效时间!"));
		return;
	}

	//(End Time (秒)   59B2048F) A6 4D FB 70 
	//0xFFFFFFFF-0xA64DFB70=0x59B2048F  2017-09-08 10:46:39
	DWORD dwTimeEnd = dwTimeStart + wTimeDays;
	DWORD dwTime = 0xFFFFFFFF - dwTimeEnd;
	data.Add(BYTE(dwTime >> 24));
	data.Add(BYTE(dwTime >> 16));
	data.Add(BYTE(dwTime >> 8));
	data.Add(BYTE(dwTime));

	m_edit_name.GetWindowText(str2);

	if (str2.GetLength() >= 0xFF)
	{
		m_edit_key.SetWindowText(_T("名称长度小于255!"));
		return;
	}

	if (str2.GetLength() % 2 == 0)
	{
		m_edit_key.SetWindowText(_T("名称长度需奇数!"));
		return;
	}

	// 程序名长度
	// FF-0xEA=0x15 -> 21
	data.Add(BYTE(0xFF - str2.GetLength()));
	for (int i = 0; i < str2.GetLength(); i++)
	{
		data.Add(BYTE(str2.GetAt(i)));
	}

	m_date_code.GetTime(time);

	//57 0C 56 A0 (Code Time (秒) 570C56A0)
	//0x570C56A0  2016-04-12 10:00:00
	DWORD dwTimeCode = (DWORD)time.GetTime();
	data.Add(BYTE(dwTimeCode >> 24));
	data.Add(BYTE(dwTimeCode >> 16));
	data.Add(BYTE(dwTimeCode >> 8));
	data.Add(BYTE(dwTimeCode));

	//D8 F0 (失效时间(天数) FFFF-D8F0=270F -> 9999 永不失效)
	//End Time - Start Time =
	//0x59B2048F - 0x59B1DD80 = 0x270F (FFFF - 0x270F = D8F0)
	
	WORD dwTimeLife = 0xFFFF - wTimeDays;
	data.Add(BYTE(dwTimeLife >> 8));
	data.Add(BYTE(dwTimeLife));

	m_edit_email.GetWindowText(str2);
	if (str2.GetLength() >= 0xFF)
	{
		m_edit_key.SetWindowText(_T("邮件长度小于255!"));
		return;
	}

	//(邮箱名长度(FF-E0=0x1F -> 31))
	//（邮箱(Gavin&Babosa@2016EasyDarwinTeam)）
	data.Add(BYTE(0xFF - str2.GetLength()));
	for (int i = 0; i < str2.GetLength(); i++)
	{
		data.Add(BYTE(str2.GetAt(i)));
	}

	//(结尾标志(easy))
	data.Add('e');
	data.Add('a');
	data.Add('s');
	data.Add('y');

	//base64 encode
	char* strBase64 = base64Encode((const char *)data.GetData(), data.GetCount());
	if (strBase64)
	{
		std::string str4 = bytesToHexString((const BYTE*)strBase64, strlen(strBase64));

		str = _T("");
		str += str4.c_str();

		m_edit_key.SetWindowText(str);

		delete strBase64;
	}
	else
	{
		m_edit_key.SetWindowText(_T("编码错误!"));
	}
}