
// xpgwinDlg.h: 头文件
//

#pragma once


// CxpgwinDlg 对话框
class CxpgwinDlg : public CDialogEx
{
// 构造
public:
	CxpgwinDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_XPGWIN_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	CString m_cstrAccessToken;
	bool  m_isStart;
	HANDLE  m_hLoopThread;
	HICON m_hIcon;

	CRect m_wndRect;//保存窗口尺寸

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedStart();
	afx_msg void OnBnClickedLogin();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClose();
};
