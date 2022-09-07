
// EasyKeyGenDlg.h: 头文件
//

#pragma once


// CEasyKeyGenDlg 对话框
class CEasyKeyGenDlg : public CDialogEx
{
// 构造
public:
	CEasyKeyGenDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EASYKEYGEN_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	void ReadIniFile();
	void WriteIniFile();

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedOk();
	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_combo_system_type;
	CDateTimeCtrl m_date_code;
	CDateTimeCtrl m_date_start;
	CEdit m_edit_key;
	CEdit m_edit_name;
	CEdit m_edit_email;
	CEdit m_edit_days;
	CStatic m_pic_head;
};
