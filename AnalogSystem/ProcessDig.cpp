// ProcessDig.cpp : 实现文件
//

#include "stdafx.h"
#include "AnalogSystem.h"
#include "ProcessDig.h"
#include "afxdialogex.h"
#include "AnalogSystemDlg.h"

// ProcessDig 对话框

IMPLEMENT_DYNAMIC(ProcessDig, CDialogEx)

ProcessDig::ProcessDig(CWnd* pParent /*=NULL*/)
	: CDialogEx(ProcessDig::IDD, pParent)
	, s_OccupiedMemory(_T(""))
	, m_CheckRun(FALSE)
{
	pro = new ProcessInfo;
	s_ConsumeMemory = _T("");
	s_ProcessName = _T("");
	s_Priority = _T("");
	m_CheckRun = FALSE;
}

ProcessDig::~ProcessDig()
{
}

void ProcessDig::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDITMemory, s_ConsumeMemory);
	DDX_Text(pDX, IDC_EDITName, s_ProcessName);
	DDX_Text(pDX, IDC_EDITPriority, s_Priority);
	DDX_Text(pDX, IDC_EDITCConsumeMemory, s_OccupiedMemory);
	DDX_Check(pDX, IDC_CHECK1, m_CheckRun);
}


BEGIN_MESSAGE_MAP(ProcessDig, CDialogEx)
	ON_BN_CLICKED(IDOK, &ProcessDig::OnBnClickedOk)
END_MESSAGE_MAP()


// ProcessDig 消息处理程序


BOOL ProcessDig::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	UpdateData(TRUE);

	SetWindowText(DlgName);

	if (DlgName == "查看进程")
	{
		GetDlgItem(IDC_EDITName)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDITPriority)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDITMemory)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDITCConsumeMemory)->EnableWindow(FALSE);
		s_ProcessName = pro->ProcessName;
		s_Priority.Format(_T("%d"),pro->Priority);
		s_ConsumeMemory.Format(_T("%d"),pro->ConsumeMemory);
		if (pro->OccupiedMemory !=  NULL)
			s_OccupiedMemory = pro->OccupiedMemory->MemoryArea;

		GetDlgItem(IDC_STATIC_ProcessState)->SetWindowTextW(pro->ProcessState);
		if (pro->OccupiedMemory ==  NULL)
		{
			GetDlgItem(IDC_STATICMemoryInfo)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDITCConsumeMemory)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATICUnit)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATICMemoryInfo)->SetWindowTextW(_T("未分配内存"));
		}
	}
	else if(DlgName == "创建进程")
	{
		GetDlgItem(IDC_STATICMemoryInfo)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDITCConsumeMemory)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATICConsumeMemory1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATICUnit)->ShowWindow(SW_HIDE);
		//进程状态隐藏
		GetDlgItem(IDC_STATIC_ProState1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_ProcessState)->ShowWindow(SW_HIDE);
		
		GetDlgItem(IDC_CHECK1)->ShowWindow(SW_SHOW);
	}
	else if (DlgName == "修改进程")
	{
		GetDlgItem(IDC_EDITName)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDITCConsumeMemory)->EnableWindow(FALSE);
		//GetDlgItem(IDC_STATICConsumeMemory1)->ShowWindow(SW_HIDE);
		//GetDlgItem(IDC_STATICUnit)->ShowWindow(SW_HIDE);

		s_ProcessName = pro->ProcessName;
		s_Priority.Format(_T("%d"),pro->Priority);
		s_ConsumeMemory.Format(_T("%d"),pro->ConsumeMemory);
		if (pro->OccupiedMemory !=  NULL)
			s_OccupiedMemory = pro->OccupiedMemory->MemoryArea;

		GetDlgItem(IDC_STATIC_ProcessState)->SetWindowTextW(pro->ProcessState);
		if (s_OccupiedMemory == "")
		{
			GetDlgItem(IDC_STATICMemoryInfo)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDITCConsumeMemory)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATICUnit)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATICMemoryInfo)->SetWindowTextW(_T("未分配内存"));
		}
	}

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void ProcessDig::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if(DlgName == "创建进程")
	{
		if (s_ProcessName == _T(""))
		{
			AfxMessageBox(_T("请输入进程名!"));
			return ;
		}

		if (s_ProcessName.GetLength() > 20)
		{
			AfxMessageBox(_T("进程名长度过长!"));
			return ;
		}

		if (mapProName[s_ProcessName] == 1)
		{
			AfxMessageBox(_T("不可以创建同名进程!"));
			return ;
		}

		if (s_ConsumeMemory.SpanIncluding(_T("0123456789 ")) == _T("") || s_Priority.SpanIncluding(_T("0123456789 ")) == _T(""))
		{
			AfxMessageBox(_T("请输入整数!"));
			return ;
		}

		if (_ttoi(s_ConsumeMemory) > MaxMemSize)
		{
			AfxMessageBox(_T("所需内存超过现有内存!"));
			return ;
		}

		if (_ttoi(s_ConsumeMemory) <= 0)
		{
			AfxMessageBox(_T("所需内存不能小于1!"));
			return ;
		}
		
		if (_ttoi(s_Priority) <= 0 && _ttoi(s_Priority) > 99 || s_Priority.GetLength() > 2)
		{
			AfxMessageBox(_T("优先级取值范围是1~99！"));
			return ;
		}

		pro->ProcessName = s_ProcessName;
		pro->Priority = _ttoi(s_Priority);
		pro->ConsumeMemory = _ttoi(s_ConsumeMemory);
		pro->OccupiedMemory = NULL;
		pro->ProcessState = "未运行";
	}
	else if (DlgName == "修改进程")
	{
		if (s_ConsumeMemory.SpanIncluding(_T("0123456789 ")) == _T("") || s_Priority.SpanIncluding(_T("0123456789 ")) == _T(""))
		{
			AfxMessageBox(_T("请输入整数!"));
			return ;
		}

		if (_ttoi(s_ConsumeMemory) <= 0)
		{
			AfxMessageBox(_T("所需内存不能小于1!"));
			return ;
		}
		
		if (_ttoi(s_Priority) <= 0 && _ttoi(s_Priority) > 99 || s_Priority.GetLength() > 2)
		{
			AfxMessageBox(_T("优先级取值范围是1~99！"));
			return ;
		}

		pro->Priority = _ttoi(s_Priority);
		pro->ConsumeMemory = _ttoi(s_ConsumeMemory);
	}

	CDialogEx::OnOK();
}
