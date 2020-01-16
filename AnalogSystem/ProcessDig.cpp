// ProcessDig.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "AnalogSystem.h"
#include "ProcessDig.h"
#include "afxdialogex.h"
#include "AnalogSystemDlg.h"

// ProcessDig �Ի���

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


// ProcessDig ��Ϣ�������


BOOL ProcessDig::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	UpdateData(TRUE);

	SetWindowText(DlgName);

	if (DlgName == "�鿴����")
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
			GetDlgItem(IDC_STATICMemoryInfo)->SetWindowTextW(_T("δ�����ڴ�"));
		}
	}
	else if(DlgName == "��������")
	{
		GetDlgItem(IDC_STATICMemoryInfo)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDITCConsumeMemory)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATICConsumeMemory1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATICUnit)->ShowWindow(SW_HIDE);
		//����״̬����
		GetDlgItem(IDC_STATIC_ProState1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_ProcessState)->ShowWindow(SW_HIDE);
		
		GetDlgItem(IDC_CHECK1)->ShowWindow(SW_SHOW);
	}
	else if (DlgName == "�޸Ľ���")
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
			GetDlgItem(IDC_STATICMemoryInfo)->SetWindowTextW(_T("δ�����ڴ�"));
		}
	}

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void ProcessDig::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	if(DlgName == "��������")
	{
		if (s_ProcessName == _T(""))
		{
			AfxMessageBox(_T("�����������!"));
			return ;
		}

		if (s_ProcessName.GetLength() > 20)
		{
			AfxMessageBox(_T("���������ȹ���!"));
			return ;
		}

		if (mapProName[s_ProcessName] == 1)
		{
			AfxMessageBox(_T("�����Դ���ͬ������!"));
			return ;
		}

		if (s_ConsumeMemory.SpanIncluding(_T("0123456789 ")) == _T("") || s_Priority.SpanIncluding(_T("0123456789 ")) == _T(""))
		{
			AfxMessageBox(_T("����������!"));
			return ;
		}

		if (_ttoi(s_ConsumeMemory) > MaxMemSize)
		{
			AfxMessageBox(_T("�����ڴ泬�������ڴ�!"));
			return ;
		}

		if (_ttoi(s_ConsumeMemory) <= 0)
		{
			AfxMessageBox(_T("�����ڴ治��С��1!"));
			return ;
		}
		
		if (_ttoi(s_Priority) <= 0 && _ttoi(s_Priority) > 99 || s_Priority.GetLength() > 2)
		{
			AfxMessageBox(_T("���ȼ�ȡֵ��Χ��1~99��"));
			return ;
		}

		pro->ProcessName = s_ProcessName;
		pro->Priority = _ttoi(s_Priority);
		pro->ConsumeMemory = _ttoi(s_ConsumeMemory);
		pro->OccupiedMemory = NULL;
		pro->ProcessState = "δ����";
	}
	else if (DlgName == "�޸Ľ���")
	{
		if (s_ConsumeMemory.SpanIncluding(_T("0123456789 ")) == _T("") || s_Priority.SpanIncluding(_T("0123456789 ")) == _T(""))
		{
			AfxMessageBox(_T("����������!"));
			return ;
		}

		if (_ttoi(s_ConsumeMemory) <= 0)
		{
			AfxMessageBox(_T("�����ڴ治��С��1!"));
			return ;
		}
		
		if (_ttoi(s_Priority) <= 0 && _ttoi(s_Priority) > 99 || s_Priority.GetLength() > 2)
		{
			AfxMessageBox(_T("���ȼ�ȡֵ��Χ��1~99��"));
			return ;
		}

		pro->Priority = _ttoi(s_Priority);
		pro->ConsumeMemory = _ttoi(s_ConsumeMemory);
	}

	CDialogEx::OnOK();
}
