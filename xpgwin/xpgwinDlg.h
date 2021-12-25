
// xpgwinDlg.h: 头文件
//

#pragma once

#include <atomic>

#define WM_USER_NOTIFYICON WM_USER+1

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

private:
	NOTIFYICONDATA m_notify;
	int m_nCountdownSecs; // 倒计时（s）

// 实现
protected:
	CString m_cstrAccessToken;
	std::atomic<bool> m_isStart;
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
	afx_msg LRESULT OnNotifyMsg(WPARAM wparam, LPARAM lparam);
	LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedStart();
	afx_msg void OnBnClickedLogin();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
