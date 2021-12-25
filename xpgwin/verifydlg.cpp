// verifydlg.cpp: 实现文件
//

#include "pch.h"
#include "xpgwin.h"
#include "verifydlg.h"
#include "afxdialogex.h"


// CVerifyDlg 对话框

IMPLEMENT_DYNAMIC(CVerifyDlg, CDialogEx)

CVerifyDlg::CVerifyDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_VERIFY_DLG, pParent)
{

}

CVerifyDlg::~CVerifyDlg()
{
}

void CVerifyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CVerifyDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CVerifyDlg::OnBnClickedOk)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CVerifyDlg 消息处理程序


void CVerifyDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	// CDialogEx::OnOK();

	// 验证管理员密码是否正确
	CString cstrPasswd;
	GetDlgItem(IDC_EDIT_PASSWD)->GetWindowText(cstrPasswd);
	if (0 == cstrPasswd.Compare(_T("a12345678")))
	{
		EndDialog(IDOK);
	}
	else
	{
		AfxMessageBox(_T("密码错误, 请重试!"));
		return;
	}
}


void CVerifyDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CDialogEx::OnClose();
	//EndDialog(IDCANCEL);
}
