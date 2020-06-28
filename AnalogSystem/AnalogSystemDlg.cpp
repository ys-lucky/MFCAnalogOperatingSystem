
// AnalogSystemDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include <stdlib.h>
#include "DataBase.h"
#include "ProcessDig.h"
#include "afxdialogex.h"
#include "AnalogSystem.h"
#include "AnalogSystemDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx,public DataBase
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


// CAnalogSystemDlg �Ի���




CAnalogSystemDlg::CAnalogSystemDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAnalogSystemDlg::IDD, pParent)
	, s_ProcessName(_T(""))
	, m_AllSel(FALSE)
{
	MemoryCount = 0;
	ProcessCount = 0;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}


CAnalogSystemDlg::~CAnalogSystemDlg()
{
	//�ͷŽ�������
	for(int i=0; i<arrProcess.GetSize(); i++)
	{
		ProcessInfo * PI = arrProcess.GetAt(i);
		delete PI;
	}
	arrProcess.RemoveAll();

	//�ͷ��ڴ�����
	POSITION pos = MemoryList.GetHeadPosition();
	while(pos != NULL)
	{
		MemoryInfo * MI = MemoryList.GetNext(pos);
		delete MI;
	}
	MemoryList.RemoveAll();

	//�ͷſ����ڴ�����
	for (int i=0; i<arrFreeMem.GetSize(); i++)
		delete arrFreeMem[i];
	arrFreeMem.RemoveAll();
}


void CAnalogSystemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CourseList, m_CourseList);
	DDX_Control(pDX, IDC_MemoryList, m_MemoryList);
	DDX_Text(pDX, IDC_EDIT1, s_ProcessName);
	DDX_Control(pDX, IDC_LIST_RunPro, m_RunPro);
	DDX_Control(pDX, IDC_LIST_ReadyPro, m_ReadyPro);
	DDX_Control(pDX, IDC_LIST_BlockPro, m_BlockPro);
	DDX_Control(pDX, IDC_LIST_DdInfo, m_Ddinfo);
	DDX_Check(pDX, IDC_CHECK2, m_AllSel);
	DDX_Control(pDX, IDC_PROGRESS1, m_Progress);
}

BEGIN_MESSAGE_MAP(CAnalogSystemDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(NM_DBLCLK, IDC_CourseList, &CAnalogSystemDlg::OnNMDblclkCourselist)
	ON_BN_CLICKED(IDC_BUTCreate, &CAnalogSystemDlg::OnBnClickedButcreate)
	ON_BN_CLICKED(IDC_BUT_FindPro, &CAnalogSystemDlg::OnBnClickedButFindpro)
	ON_BN_CLICKED(IDC_BUT_DeletePro, &CAnalogSystemDlg::OnBnClickedButDeletepro)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTONModProcess, &CAnalogSystemDlg::OnBnClickedButtonmodprocess)
	ON_BN_CLICKED(IDC_BUTTON_AllotPro, &CAnalogSystemDlg::OnBnClickedButtonAllotpro)
	ON_BN_CLICKED(IDC_BUTTON_KillPor, &CAnalogSystemDlg::OnBnClickedButtonKillpor)
	ON_BN_CLICKED(IDC_BUTTON_RunPro, &CAnalogSystemDlg::OnBnClickedButtonRunpro)
	ON_BN_CLICKED(IDC_BUTTON_ExportExcel, &CAnalogSystemDlg::OnBnClickedButtonExportexcel)
	ON_BN_CLICKED(IDC_BUTTON_SingleStep, &CAnalogSystemDlg::OnBnClickedButtonSinglestep)
	ON_BN_CLICKED(IDC_CHECK2, &CAnalogSystemDlg::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_BUTTON_Clear, &CAnalogSystemDlg::OnBnClickedButtonClear)
	ON_BN_CLICKED(IDC_BUTTON_KillAllPor, &CAnalogSystemDlg::OnBnClickedButtonKillallpor)
END_MESSAGE_MAP()


// CAnalogSystemDlg ��Ϣ�������

//�����ڴ�������� ��С��������
void FreeMemQuickSort(CArray<FreeMem *,FreeMem *> &Array, int low, int high)
{
	if(low > high) // �����ݹ������
		return ;

	FreeMem * FM = Array[low];
	int refer = FM->MemorySize;

	int left = low;
	int right = high;

	while(left<right)
	{
		while(left<right && refer <= Array[right]->MemorySize )
			right--;

		if(left<right)
			Array[left] = Array[right];

		while(left<right && refer >= Array[left]->MemorySize )
			left++;

		if(left<right)
			Array[right] = Array[left];	

	}
	Array[left] = FM ;

	FreeMemQuickSort(Array , low , left-1 );
	FreeMemQuickSort(Array , right+1 , high );
}

//���ȼ���������  �Ӵ�С����
void PriorityQuickSort(CArray<ProcessInfo *,ProcessInfo *> &Array, int low, int high)
{
	if(low > high) // �����ݹ������
		return ;

	ProcessInfo * Pi = Array[low];
	int refer = Pi->Priority;

	int left = low;
	int right = high;

	while(left<right)
	{
		while(left<right && refer >= Array[right]->Priority )
			right--;

		if(left<right)
			Array[left] = Array[right];

		while(left<right && refer <= Array[left]->Priority )
			left++;

		if(left<right)
			Array[right] = Array[left];	

	}
	Array[left] = Pi ;

	PriorityQuickSort(Array , low , left-1 );
	PriorityQuickSort(Array , right+1 , high );
}

BOOL CAnalogSystemDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	UpdateData(TRUE);
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	arrFreeMem.RemoveAll(); //��տ����ڴ�����
	ado.ConnectionDatabase();//�������ݿ�

	CRect rect;
	m_CourseList.GetClientRect(rect);//��ȡ�����б��С
	m_CourseList.SetExtendedStyle(m_CourseList.GetExtendedStyle()|LVS_EX_FULLROWSELECT |LVS_EX_GRIDLINES |LVS_EX_CHECKBOXES);
	m_CourseList.InsertColumn(0, _T("������"), LVCFMT_CENTER, rect.Width()/4);
	m_CourseList.InsertColumn(1, _T("���ȼ�"), LVCFMT_CENTER,rect.Width()/4);
	m_CourseList.InsertColumn(2, _T("�����ڴ�(k)"), LVCFMT_CENTER,rect.Width()/4);
	m_CourseList.InsertColumn(3, _T("����״̬"), LVCFMT_CENTER,rect.Width()/4);
	
	m_MemoryList.GetClientRect(rect);//��ȡ�ڴ��б��С
	m_MemoryList.SetExtendedStyle(m_MemoryList.GetExtendedStyle()|LVS_EX_FULLROWSELECT |LVS_EX_GRIDLINES);
	m_MemoryList.InsertColumn(0,_T("�ڴ�����"),LVCFMT_CENTER, rect.Width()/3);
	m_MemoryList.InsertColumn(1,_T("�ڴ��С"),LVCFMT_CENTER, rect.Width()/3);
	m_MemoryList.InsertColumn(2,_T("����״̬"),LVCFMT_CENTER, rect.Width()/3);

	EnableButton();
	InitList(_T("ProcessInfo"),_T("Priority DESC"));//��ʼ�������б� DESC�Ӵ�С����
	InitList(_T("SystemMemory"),_T("ID"));			//��ʼ���ڴ��б�
	RefreshReadyProList();			//��ʼ�����������б�
	UpdateData(FALSE);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CAnalogSystemDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CAnalogSystemDlg::OnPaint()
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
HCURSOR CAnalogSystemDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//��ʼ���б�
void CAnalogSystemDlg::InitList(CString Table,CString sSort)
{
	_RecordsetPtr Rec;
	CString sql = _T("select * from " + Table + " order by " + sSort);//�򿪽��̱�
	Rec = ado.GetRecordSet((_bstr_t)sql);	//��ȡ��¼��

	int nRow=0;
	while(!Rec->adoEOF)
	{
		if (Table == "ProcessInfo")
			InitProcessList(Rec, nRow);
		else if (Table == "SystemMemory")
			InitMemoryList(Rec, nRow);

		nRow++;
		Rec->MoveNext();
	}

	if (Table == "SystemMemory")//��������ڴ�
		FreeMemQuickSort(arrFreeMem,0,arrFreeMem.GetSize()-1);
	//else if (Table == "ProcessInfo")//�����������
	//	PriorityQuickSort(arrProcess,0,arrProcess.GetSize()-1);

	Rec->Close();//�رռ�¼��
	SetProcessNum(ProcessCount);//���ý�����
}

//��ʼ�������б�
void CAnalogSystemDlg::InitProcessList(_RecordsetPtr & Rec, int nRow) 
{
	_variant_t var;

	int size = Rec->Fields->Count;//��ȡ�ֶ��ܳ���

	ProcessInfo *pro = new ProcessInfo; //���������

	m_CourseList.InsertItem(nRow, _T(""));//����ÿһ������

	var = Rec->GetCollect("ProcessName");				//���ý�����

	if (var.vt != VT_NULL)
		pro->ProcessName = (LPCSTR)_bstr_t(var);
	m_CourseList.SetItemText(nRow, 0, pro->ProcessName);
	ProcessMapName[pro->ProcessName] = 1;

	var = Rec->GetCollect("Priority");					//�������ȼ�
	if (var.vt != VT_NULL)
		pro->Priority = var.lVal;
	CString sPriority;
	sPriority.Format(_T("%d"),pro->Priority);
	m_CourseList.SetItemText(nRow, 1, sPriority);

	var = Rec->GetCollect("ConsumeMemory");				//��������ռ�
	if (var.vt != VT_NULL)
		pro->ConsumeMemory = var.lVal;
	CString sConsumeMemory;
	sConsumeMemory.Format(_T("%d"),pro->ConsumeMemory);
	m_CourseList.SetItemText(nRow, 2, sConsumeMemory);

	var = Rec->GetCollect("ProcessState");				//���ý���״̬

	if (var.vt != VT_NULL)
		pro->ProcessState = (LPCSTR)_bstr_t(var);
	m_CourseList.SetItemText(nRow, 3, pro->ProcessState);

	pro->OccupiedMemory = NULL;
	++ProcessCount;
	int nIndex = arrProcess.Add(pro);
	m_CourseList.SetItemData(nRow, nIndex);

	
}

//��ʼ���ڴ��б�
void CAnalogSystemDlg::InitMemoryList(_RecordsetPtr & Rec, int nRow)
{
	_variant_t var;
	int size = Rec->Fields->Count;//��ȡ�ֶ��ܳ���
	MemoryInfo *Mem = new MemoryInfo; //���������

	m_MemoryList.InsertItem(nRow, _T(""));//����ÿһ������
	
	var = Rec->GetCollect("MemoryArea");				//�����ڴ�����
	if (var.vt != VT_NULL)
		Mem->MemoryArea = (LPCSTR)_bstr_t(var);
	m_MemoryList.SetItemText(nRow, 0, Mem->MemoryArea);

	CString sNumber;
	var = Rec->GetCollect("MemorySize");				//�����ڴ��С
	if (var.vt != VT_NULL)
		Mem->MemorySize = var.lVal;
	sNumber.Format(_T("%d"),Mem->MemorySize);
	m_MemoryList.SetItemText(nRow, 1, sNumber);


	var = Rec->GetCollect("MemoryState");				//�����ڴ�״̬
	if (var.vt != VT_NULL)
		Mem->MemoryState = (LPCSTR)_bstr_t(var);
	m_MemoryList.SetItemText(nRow, 2, Mem->MemoryState);

	++MemoryCount;			
	Mem->MemoryID=MemoryCount;							//����������
	m_MemoryList.SetItemData(nRow, (long)(void*)Mem);
	
	POSITION pos = MemoryList.AddTail(Mem);

	if (Mem->MemoryState == _T("����"))//�������ڴ��������
	{
		FreeMem *freemem = new FreeMem;//�����ڴ�
		freemem->MemorySize = Mem->MemorySize;
		freemem->pos = pos;
		arrFreeMem.Add(freemem);
	}
}

//ˢ�½����б�
void CAnalogSystemDlg::RefreshProcessList(CListCtrl & List, CArray<ProcessInfo *,ProcessInfo *> & arrPro )
{
	List.DeleteAllItems();//����б�

	CString str;
	for (int i=0; i<arrPro.GetSize(); i++)
	{
		ProcessInfo * pro = arrPro.GetAt(i);
		List.InsertItem(i, _T(""));
		List.SetItemText(i, 0, pro->ProcessName);

		str.Format(_T("%d"),pro->Priority);
		List.SetItemText(i, 1, str);

		str.Format(_T("%d"),pro->ConsumeMemory);
		List.SetItemText(i, 2, str);

		List.SetItemText(i, 3, pro->ProcessState);

		List.SetItemData(i, (long)(void*)i);
	}

	m_AllSel = FALSE;//ˢ���б�ʱ,��ȫѡ��ȥ��
	UpdateData(FALSE);
}

//ˢ���ڴ��б�
void CAnalogSystemDlg::RefreshMemoryList( CListCtrl & List, CList<MemoryInfo *,MemoryInfo *> & MemList )
{
	List.DeleteAllItems();//����б�

	POSITION pos = MemList.GetHeadPosition();
	int nRow = 0;
	CString str;
	while (pos != NULL)
	{
		MemoryInfo * Mem = MemList.GetNext(pos);
		List.InsertItem(nRow, _T(""));
		List.SetItemText(nRow, 0, Mem->MemoryArea);

		str.Format(_T("%d"),Mem->MemorySize);
		List.SetItemText(nRow, 1, str);

		List.SetItemText(nRow, 2, Mem->MemoryState);

		List.SetItemData(nRow, (long)(void*)Mem);
		nRow++;
	}
}

//�����б�˫�������Ϣ����
void CAnalogSystemDlg::OnNMDblclkCourselist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	if (pNMItemActivate == NULL || pNMItemActivate->iItem < 0)
		return;

	int nIndex = (int)m_CourseList.GetItemData(pNMItemActivate->iItem);

	ProcessDig dlg;
	dlg.pro = arrProcess.GetAt(nIndex);
	dlg.DlgName = _T("�鿴����");
	dlg.DoModal();

}

//���ҽ���
void CAnalogSystemDlg::OnBnClickedButFindpro()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	BOOL bFind = FALSE;
	if (s_ProcessName == "")
	{
		bFind = TRUE;
		RefreshProcessList(m_CourseList, arrProcess);
	}
	else
	{
		int nIndex;
		CString str;
		ProcessInfo *pro = FindProcess(s_ProcessName,nIndex);
		if(pro != NULL)
		{
			bFind = TRUE;
			m_CourseList.DeleteAllItems();	//����б�

			m_CourseList.InsertItem(0, _T(""));
			m_CourseList.SetItemText(0, 0, pro->ProcessName);
			str.Format(_T("%d"),pro->Priority);
			m_CourseList.SetItemText(0, 1, str);
			str.Format(_T("%d"),pro->ConsumeMemory);
			m_CourseList.SetItemText(0, 2, str);
			m_CourseList.SetItemText(0, 3, pro->ProcessState);

			m_CourseList.SetItemData(0, (long)(void*)nIndex);
		}

	}

	if (!bFind)
	{
		AfxMessageBox(_T("δ���ҵ��ý��̣�"));
	}
}

//���ҽ���
ProcessInfo * CAnalogSystemDlg::FindProcess( CString Proname ,int &nIndex)
{
	for(nIndex=0; nIndex<arrProcess.GetSize(); nIndex++)
	{
		ProcessInfo *pro = arrProcess.GetAt(nIndex);
		if (Proname == pro->ProcessName)
			return pro;
	}
	return NULL;
}

//��������
void CAnalogSystemDlg::OnBnClickedButcreate()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ProcessInfo *pro = new ProcessInfo;
	ProcessDig dlg;
	dlg.DlgName = _T("��������");
	dlg.pro = pro;
	dlg.CopyMap(dlg.mapProName,ProcessMapName);
	dlg.MaxMemSize = GetMemorySize()-10;//ȥ��OS��ռ�ڴ�

	if (dlg.DoModal() == IDOK)
	{
		arrProcess.Add(pro);
		++ProcessCount;

		SetProcessNum(ProcessCount);//���ý�����
		ProcessMapName[pro->ProcessName] = 1;

		if (dlg.m_CheckRun)
		{
			AllocationMemory(pro);//�����ڴ�
			EnableButton();
		}

		PriorityQuickSort(arrProcess,0,arrProcess.GetSize()-1);//����
		RefreshProcessList(m_CourseList, arrProcess);//ˢ�½����б�

		CString sql;
		sql.Format(_T("INSERT INTO ProcessInfo(ProcessName,Priority,ConsumeMemory,OccupiedMemory,ProcessState) \
			VALUES ('%s',%d,%d,' ','δ����')"),pro->ProcessName,pro->Priority,pro->ConsumeMemory);
		ado.ExecuteSQL(sql);
		AfxMessageBox(_T("�����ɹ�"));
	}
}

//ɾ������
void CAnalogSystemDlg::OnBnClickedButDeletepro()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nSize = m_CourseList.GetItemCount();
	BOOL bDel = FALSE;
	BOOL bRun = FALSE;
	for (int i=nSize-1; i >= 0; i--)
	{
		if (m_CourseList.GetCheck(i))//��ȡ�Ƿ�Ϊѡ��״̬
		{
			bDel=TRUE;
			int nIndex = (int)m_CourseList.GetItemData(i);
			ProcessInfo* pro = arrProcess.GetAt(nIndex);
			if (pro->ProcessState != "δ����")
			{
				bRun = TRUE;
				break;
			}
			--ProcessCount;
			ProcessMapName[pro->ProcessName] = 0;

			CString sql;
			sql.Format(_T("delete from ProcessInfo where ProcessName = '%s'"),pro->ProcessName);
			ado.ExecuteSQL(sql);//ִ��sql���

			delete pro;//�ͷŽ���ռ�öѿռ�
			m_CourseList.DeleteItem(i);//ɾ���б�����Ϣ
			arrProcess.RemoveAt(nIndex);//ɾ���ý��̽ڵ�
		}
	}

	if (bRun)
	{
		AfxMessageBox(_T("��ǰ������������,������ɾ��!"));
		return ;
	}
	if (!bDel)
	{
		AfxMessageBox(_T("��ѡ����Ҫɾ���Ľ��̣�"));
		return ;
	}
	RefreshProcessList(m_CourseList, arrProcess);//ˢ�½����б��б�
	SetProcessNum(ProcessCount);//���ý�����
}

//�޸Ľ���
void CAnalogSystemDlg::OnBnClickedButtonmodprocess()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ProcessDig dlg;
	int nSize = m_CourseList.GetItemCount();
	BOOL bMod = FALSE;
	for (int i=0; i<nSize; i++)
	{
		int state = m_CourseList.GetItemState(i,LVIS_SELECTED);//��ȡ�Ƿ�Ϊѡ��״̬
		if (state)
		{
			bMod=TRUE;
			int nIndex = (int)m_CourseList.GetItemData(i);
			ProcessInfo* pro = arrProcess.GetAt(nIndex);
			dlg.pro = pro;
			dlg.DlgName = "�޸Ľ���";
			if (dlg.DoModal() == IDOK)
			{
				CString sql;
				sql.Format(_T("update ProcessInfo set Priority = %d,ConsumeMemory = %d where ProcessName = '%s'"),
					pro->Priority,pro->ConsumeMemory,pro->ProcessName);
				ado.ExecuteSQL(sql);//ִ��sql���

				AfxMessageBox(_T("�޸ĳɹ���"));
			}
			break;
		}
	}

	if (!bMod)
	{
		AfxMessageBox(_T("��ѡ����Ҫ�޸ĵĽ��̣�"));
	}
	PriorityQuickSort(arrProcess,0,arrProcess.GetSize()-1);//���ȼ�����
	RefreshProcessList(m_CourseList, arrProcess);//ˢ���б�
}

//�������
void CAnalogSystemDlg::OnBnClickedButtonAllotpro()
{
	//TODO: �ڴ���ӿؼ�֪ͨ����������
	BOOL bState = FALSE;//�Ƿ��ѷ���
	BOOL bSel = FALSE;	//�Ƿ�ѡ��
	BOOL bAllot = FALSE;//�Ƿ��к����ڴ����
	for (int i=0; i<m_CourseList.GetItemCount(); i++)
	{
		if (m_CourseList.GetCheck(i))//��ȡ�Ƿ�Ϊѡ��״̬
		{
			bSel=TRUE;
			int nIndex = (int)m_CourseList.GetItemData(i);
			ProcessInfo* pro = arrProcess.GetAt(nIndex);

			if (pro->ProcessState != "δ����")//�ѷ���
			{
				bState = TRUE;
				break;
			}

			AllocationMemory(pro);//�����ڴ�
		}
	}
	if (bState)
		AfxMessageBox(_T("��ǰѡ������ѷ����ڴ�!"));

	if (!bSel)
		AfxMessageBox(_T("��ѡ����Ҫ����Ľ��̣�"));

	else if (bSel && !bState)
	{
		RefreshProcessList(m_CourseList,arrProcess);//ˢ�½����б�
		EnableButton();
		AfxMessageBox(_T("����ɹ���"));
	}
}

//�����ڴ�
BOOL CAnalogSystemDlg::AllocationMemory( ProcessInfo *Pro )
{
	BOOL bAllot = FALSE;
	for (int i=0; i < arrFreeMem.GetSize(); i++)
	{
		FreeMem * FM = arrFreeMem.GetAt(i);
		if (Pro->ConsumeMemory <= FM->MemorySize)
		{
			bAllot = TRUE;
			POSITION pos = FM->pos;
			MemoryInfo * Mem = MemoryList.GetAt(pos);

			int nStart,nEnd;
			GetMemoryAreaStartAndEnd(Mem->MemoryArea,nStart,nEnd);
			int nNewEnd = nStart+Pro->ConsumeMemory-1;

			CString sNewMemArea;
			sNewMemArea.Format(_T("%d~%dk"),nStart,nNewEnd);//����ռ���ڴ�����

			Pro->ProcessState = "����̬";
			Mem->MemorySize = Pro->ConsumeMemory;
			Mem->MemoryArea = sNewMemArea;
			Mem->MemoryState = Pro->ProcessName;
			Pro->OccupiedMemory = Mem;

			if (Pro->ConsumeMemory != FM->MemorySize)//�����µ��ڴ�ڵ�
			{
				MemoryInfo *NewMem = new MemoryInfo;
				++MemoryCount;
				NewMem->MemoryID = MemoryCount;
				sNewMemArea.Format(_T("%d~%dk"), nNewEnd+1, nEnd);
				NewMem->MemoryArea = sNewMemArea;
				NewMem->MemorySize = GetMemoryAreaSize(sNewMemArea);
				NewMem->MemoryState = _T("����");
				MemoryList.InsertAfter(pos,NewMem);
			}
			break;
		}
	}

	if (bAllot)
	{
		ResetFreeMemArr(arrFreeMem, MemoryList);	//�������ɿ����������鲢����
		RefreshMemoryList(m_MemoryList,MemoryList);	//ˢ���ڴ��б�
		RefreshReadyProList();						//ˢ�¾����б�
	}
	else
	{
		Pro->ProcessState = "����̬";
		RefreshBlockProList();						//ˢ�������б�
	}

	return bAllot;
}

//���ý�����
void CAnalogSystemDlg::SetProcessNum(int proNum)
{
	CString sNum;
	sNum.Format(_T("%d"),proNum);
	GetDlgItem(IDC_processNum)->SetWindowTextW(_T("��ǰ���н��̣�"+sNum));
}

//���ÿؼ���ɫ
HBRUSH CAnalogSystemDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	if (pWnd->GetDlgCtrlID()==IDC_processNum)//�ؼ�ID
	{
		pDC->SetTextColor(RGB(255,0,0)); //������ɫ
	}
	
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}

//��ȡ�ڴ������С
int CAnalogSystemDlg::GetMemoryAreaSize( CString sMemArea )
{
	int nStart,nEnd;
	GetMemoryAreaStartAndEnd(sMemArea,nStart,nEnd);
	return nEnd-nStart+1;
}

//��ȡ�ڴ�������ʼλ�ͽ���λ
void CAnalogSystemDlg::GetMemoryAreaStartAndEnd(CString sMemArea, int & start,int & end )
{
	//��ȡ���ڴ�������ʼ��ַ
	int nMark = sMemArea.Find(_T("~"));
	start = _ttoi(sMemArea.Left(nMark));

	//��ȡ���ڴ����������ַ
	int nRight = sMemArea.Find(_T("k"));
	int nCount = nRight-nMark;
	end = _ttoi(sMemArea.Mid(nMark+1,nCount-1));
}

//���ڴ�����
MemoryInfo * CAnalogSystemDlg::NewMemArea( CString proName, int MemSize, CString MemArea )
{
	++MemoryCount;
	MemoryInfo * Mem;
	Mem->MemoryID = MemoryCount;
	Mem->MemorySize = MemSize;
	return Mem;
}

//�����ڴ�
void CAnalogSystemDlg::RecoveryMemory(CList<MemoryInfo *,MemoryInfo *> &MemoryList, MemoryInfo *CurMem)
{
	int nPreStart,nPreEnd;
	int nCurStart,nCurEnd;
	int nNextStart,nNextEnd;
	GetMemoryAreaStartAndEnd(CurMem->MemoryArea, nCurStart, nCurEnd);//��ȡ��ǰ�ڴ��������ʼλ�ͽ���λ

	BOOL bPrevFreeMem = FALSE;
	BOOL bNextFreeMem = FALSE;
	POSITION pos = MemoryList.Find(CurMem);
	MemoryInfo *PrevMem = NULL;
	MemoryList.GetPrev(pos);
	if (pos != NULL)
	{
		PrevMem = MemoryList.GetAt(pos);
		if (PrevMem->MemoryState == _T("����"))
		{
			bPrevFreeMem = TRUE;
			GetMemoryAreaStartAndEnd(PrevMem->MemoryArea, nPreStart, nPreEnd);//��ȡ��һ���ڴ��������ʼλ�ͽ���λ
		}
	}

	pos = MemoryList.Find(CurMem);
	MemoryInfo *NextMem = NULL;
	MemoryList.GetNext(pos);
	if (pos != NULL)
	{
		NextMem = MemoryList.GetAt(pos);

		if (NextMem->MemoryState == _T("����"))
		{
			bNextFreeMem = TRUE;
			GetMemoryAreaStartAndEnd(NextMem->MemoryArea, nNextStart, nNextEnd);//��ȡ��һ���ڴ��������ʼλ�ͽ���λ
		}
	}
	
	CString nNewMem;
	if (bPrevFreeMem && bNextFreeMem)//��������
	{
		nNewMem.Format(_T("%d~%dk"),nPreStart, nNextEnd);
		CurMem->MemoryArea = nNewMem;
		CurMem->MemorySize = PrevMem->MemorySize + CurMem->MemorySize + NextMem->MemorySize;

		MemoryList.RemoveAt(MemoryList.Find(PrevMem));//ɾ����һ���ڵ�
		MemoryList.RemoveAt(MemoryList.Find(NextMem));//ɾ����һ���ڵ�
	}
	else if (bPrevFreeMem)//������
	{
		nNewMem.Format(_T("%d~%dk"),nPreStart, nCurEnd);
		CurMem->MemoryArea = nNewMem;
		CurMem->MemorySize = PrevMem->MemorySize + CurMem->MemorySize;
		MemoryList.RemoveAt(MemoryList.Find(PrevMem));//ɾ����һ���ڵ�
	}
	else if(bNextFreeMem)//������
	{
		nNewMem.Format(_T("%d~%dk"),nCurStart, nNextEnd);
		CurMem->MemoryArea = nNewMem;
		CurMem->MemorySize = CurMem->MemorySize + NextMem->MemorySize;
		MemoryList.RemoveAt(MemoryList.Find(NextMem));//ɾ����һ���ڵ�
	}

	CurMem->MemoryState = _T("����");
}

//�������ɿ����������鲢����
void CAnalogSystemDlg::ResetFreeMemArr(CArray<FreeMem *,FreeMem *>  & Array, CList<MemoryInfo *,MemoryInfo *> & MemList)
{
	Array.RemoveAll();//�������

	POSITION pos= MemList.GetHeadPosition();

	while(pos != NULL)//���ҿ�������
	{
		MemoryInfo * Mem = MemList.GetNext(pos);
		if (Mem->MemoryState == "����")
		{
			FreeMem *FM = new FreeMem;
			FM->pos = MemList.Find(Mem);
			FM->MemorySize = Mem->MemorySize;
			Array.Add(FM);
		}
	}

	if (Array.GetSize() > 1)//����
		FreeMemQuickSort(Array,0,Array.GetSize()-1);
}

//ɱ������
void CAnalogSystemDlg::OnBnClickedButtonKillpor()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nSelIndex = -1;
	int ArrIndex=0;
	if ((nSelIndex = m_ReadyPro.GetCurSel()) != LB_ERR)//��������
	{
		ArrIndex = m_ReadyPro.GetItemData(nSelIndex);
		m_ReadyPro.DeleteString(nSelIndex);
	}
	else if ((nSelIndex = m_BlockPro.GetCurSel()) != LB_ERR)//��������
	{
		ArrIndex = m_BlockPro.GetItemData(nSelIndex);
		m_BlockPro.DeleteString(nSelIndex);
	}
	else if ((nSelIndex = m_RunPro.GetCurSel()) != LB_ERR)//���н���
	{
		ArrIndex = m_RunPro.GetItemData(nSelIndex);
		m_RunPro.DeleteString(nSelIndex);
	}

	ProcessInfo *Pro = NULL;
	if (nSelIndex != -1)
	{
		Pro = arrProcess[ArrIndex];
		if (Pro->ProcessState != "����̬")
		{
			RecoveryMemory(MemoryList, Pro->OccupiedMemory);//�����ڴ�
			Pro->OccupiedMemory = NULL;
			ResetFreeMemArr(arrFreeMem, MemoryList);		//�������ɿ����������鲢����
			RefreshMemoryList(m_MemoryList, MemoryList);	//��ʼ���ڴ��б�
		}
		Pro->ProcessState = "δ����";

		if (m_BlockPro.GetCount() != 0)//�����������ڲ������ȼ���ߵĽ��̣��Ƿ����㹻�ڴ�ɷ���
		{
			CString proName;
			m_BlockPro.GetText(0,proName);
			ArrIndex = (int)m_BlockPro.GetItemData(0);
			ProcessInfo *Blockpro = arrProcess.GetAt(ArrIndex);
			if (GetMaxFreeMemSize(arrFreeMem) >= Blockpro->ConsumeMemory)
			{
				int ReadyIndex = m_ReadyPro.AddString(Blockpro->ProcessName);
				m_ReadyPro.SetItemData(ReadyIndex,ArrIndex);
				AllocationMemory(Blockpro);//�����ڴ�
				m_BlockPro.DeleteString(0);
				RefreshMemoryList(m_MemoryList,MemoryList);//ˢ���ڴ��б�
			}
		}

		RefreshProcessList(m_CourseList, arrProcess);	//��ʼ�������б�
		AfxMessageBox(_T("��ɱ��")+Pro->ProcessName+_T("����"));
		EnableButton();
	}
	else
		AfxMessageBox(_T("��ѡ����Ҫɱ���Ľ���"));
}

//���ȫ������
void CAnalogSystemDlg::OnBnClickedButtonKillallpor()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_ReadyPro.GetCount() == 0 && m_BlockPro.GetCount() == 0 && m_RunPro.GetCount() == 0)
	{
		AfxMessageBox(_T("���ȷ�����̣�"));
		return ;
	}

	int ArrIndex=0;
	while(TRUE)
	{
		int nSelIndex = -1;
		if ((nSelIndex = m_ReadyPro.GetCount()) != 0)//��������
		{
			ArrIndex = m_ReadyPro.GetItemData(0);
			m_ReadyPro.DeleteString(0);
		}
		else if ((nSelIndex = m_BlockPro.GetCount()) != 0)//��������
		{
			ArrIndex = m_BlockPro.GetItemData(0);
			m_BlockPro.DeleteString(0);
		}
		else if ((nSelIndex = m_RunPro.GetCount()) != 0)//���н���
		{
			ArrIndex = m_RunPro.GetItemData(0);
			m_RunPro.DeleteString(0);
		}

		ProcessInfo *Pro = NULL;
		if (nSelIndex != 0)
		{
			Pro = arrProcess[ArrIndex];
			if (Pro->ProcessState != "����̬")
			{
				RecoveryMemory(MemoryList, Pro->OccupiedMemory);//�����ڴ�
			}
			Pro->OccupiedMemory = NULL;
			Pro->ProcessState = "δ����";
		}
		else
			break;
	}


	RefreshProcessList(m_CourseList, arrProcess);	//��ʼ�������б�
	ResetFreeMemArr(arrFreeMem, MemoryList);		//�������ɿ����������鲢����
	RefreshMemoryList(m_MemoryList, MemoryList);	//��ʼ���ڴ��б�
}

//ˢ�¾��������б�
void CAnalogSystemDlg::RefreshReadyProList()
{
	m_ReadyPro.ResetContent();//���������Ϣ
	for (int i=0; i<arrProcess.GetSize(); i++)
	{
		ProcessInfo *Pi = arrProcess.GetAt(i);
		if (Pi->ProcessState == "����̬")
		{
			int nIndex = m_ReadyPro.AddString(Pi->ProcessName);
			m_ReadyPro.SetItemData(nIndex,(long)(void *)i);
		}
	}
}

//ˢ�����������б�  
void CAnalogSystemDlg::RefreshBlockProList()
{
	m_BlockPro.ResetContent();//���������Ϣ
	for (int i=0; i<arrProcess.GetSize(); i++)
	{
		ProcessInfo *Pi = arrProcess.GetAt(i);
		if (Pi->ProcessState == "����̬")
		{
			int nIndex = m_BlockPro.AddString(Pi->ProcessName);
			m_BlockPro.SetItemData(nIndex,(long)(void *)i);
		}
	}
}

//�õ����ڴ��С
int CAnalogSystemDlg::GetMemorySize()
{
	POSITION pos = MemoryList.GetTailPosition();
	MemoryInfo * Mem= MemoryList.GetAt(pos);
	int nStart,nEnd;
	GetMemoryAreaStartAndEnd(Mem->MemoryArea, nStart, nEnd);
	return nEnd;
}

//ִ�е���
void CAnalogSystemDlg::OnBnClickedButtonRunpro()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
	//���������Ϣ
	for (int i = m_Ddinfo.GetCount(); i>=0; i--)
		m_Ddinfo.DeleteString(i);
	m_Ddinfo.ResetContent();

	while(TRUE)
	{
		if (OperationDispatching())
			break;
	}
}

//����ִ��
void CAnalogSystemDlg::OnBnClickedButtonSinglestep()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//m_Ddinfo.ResetContent();//���������Ϣ
	OperationDispatching();
}

//ִ�е���
BOOL CAnalogSystemDlg::OperationDispatching()
{
	BOOL bFinish = FALSE;
	CString str;
	ProcessInfo* pro = NULL;

	CString proName;
	int nIndex;
	if (m_RunPro.GetCount() != 0)
	{
		m_RunPro.GetText(0,proName);
		nIndex = (int)m_RunPro.GetItemData(0);
		pro = arrProcess.GetAt(nIndex);
		str.Format(proName+_T("����...")+_T("ִ�����..."));
		ReferenceMSG(str);
		str.Format(_T("�ͷ�")+proName+_T("����ռ���ڴ�:")+pro->OccupiedMemory->MemoryArea+_T("����..."));
		pro->OccupiedMemory->MemoryState = "����";
		RecoveryMemory(MemoryList,pro->OccupiedMemory);//�����ڴ�
		ResetFreeMemArr(arrFreeMem,MemoryList);		//�������ɿ����б�
		pro->OccupiedMemory = NULL;
		pro->ProcessState = "δ����";
		ReferenceMSG(str);

		m_RunPro.DeleteString(0);
	}

	if (m_ReadyPro.GetCount() != 0)
	{
		m_ReadyPro.GetText(0,proName);
		nIndex = (int)m_ReadyPro.GetItemData(0);
		m_ReadyPro.DeleteString(0);

		int RunIndex = m_RunPro.AddString(proName);
		m_RunPro.SetItemData(RunIndex,nIndex);
		pro = arrProcess.GetAt(nIndex);
		pro->ProcessState = "ִ��̬";

		str.Format(_T("��ʼִ��")+proName+_T("����..."));
		ReferenceMSG(str);
		str.Format(_T("���ȼ�:%d �����ڴ�:%d ����ִ��ʱ��:%d "),pro->Priority,pro->ConsumeMemory,pro->ConsumeMemory);
		ReferenceMSG(str);
	}

	if (m_BlockPro.GetCount() != 0)//�����������ڲ������ȼ���ߵĽ��̣��Ƿ����㹻�ڴ�ɷ���
	{
		m_BlockPro.GetText(0,proName);
		nIndex = (int)m_BlockPro.GetItemData(0);
		pro = arrProcess.GetAt(nIndex);
		if (GetMaxFreeMemSize(arrFreeMem) >= pro->ConsumeMemory)
		{
			int ReadyIndex = m_ReadyPro.AddString(pro->ProcessName);
			m_ReadyPro.SetItemData(ReadyIndex,nIndex);
			AllocationMemory(pro);//�����ڴ�
			m_BlockPro.DeleteString(0);
			RefreshProcessList(m_CourseList,arrProcess);//ˢ�½����б�

			str.Format(_T("������̣�%s ռ���ڴ棺%s"),pro->ProcessName,pro->OccupiedMemory->MemoryArea);
			ReferenceMSG(str);
		}
	}

	RefreshProcessList(m_CourseList,arrProcess);//ˢ�½����б�
	RefreshMemoryList(m_MemoryList,MemoryList);//ˢ���ڴ��б�

	if(m_ReadyPro.GetCount() == 0 && m_BlockPro.GetCount() == 0 && m_RunPro.GetCount() == 0)
	{
		bFinish = TRUE;
		str.Format(_T("ִ�����!"));
		ReferenceMSG(str);
		AfxMessageBox(str);
		EnableButton();
	}
	Sleep(500);
	return bFinish;
}

//��ʾ������Ϣ
void CAnalogSystemDlg::ReferenceMSG(CString &slog)
{
	m_Ddinfo.AddString(slog);
	m_Ddinfo.SetCurSel (m_Ddinfo.GetCount() - 1);
	MSG   pMsg;
	while (PeekMessage(&pMsg,NULL,0,0,PM_REMOVE))     
	{
		TranslateMessage(&pMsg);
		DispatchMessage(&pMsg);
	}
}

//�õ��������ڴ��С
int CAnalogSystemDlg::GetMaxFreeMemSize( CArray<FreeMem *,FreeMem *> & Array )
{
	int nFreeMemSize = 0;
	for (int i=0; i< Array.GetSize(); i++)
	{
		if (nFreeMemSize < Array.GetAt(i)->MemorySize)
			nFreeMemSize = Array.GetAt(i)->MemorySize;
	}
	return nFreeMemSize;
}

//��ʼ��Excel����
void CAnalogSystemDlg::InitExcel( void )
{
	/*************����Excel������(����Excel)****************/
	if(!App.CreateDispatch(_T("Excel.Application")))
	{
		AfxMessageBox(_T("�޷�����Excel��������"));
		return ;
	}
}

//����Excel
void CAnalogSystemDlg::OnBnClickedButtonExportexcel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog SaveDlg(FALSE,  _T(".xlsx"), _T("������Ϣ"), OFN_HIDEREADONLY|OFN_READONLY|OFN_OVERWRITEPROMPT ,
		_T("Excel File(*.xlsx)|*.xlsx|all file(*.*)|*.*||"),NULL);//���Ϊ�Ի���,Ĭ�ϱ༭�򡰵�����Ϣ.xlsx��


	INT_PTR Result = SaveDlg.DoModal();//ģ̬��ʾ���Ϊ�Ի���

	if (Result == IDOK)//�ж��Ƿ�ѡ��ȷ��
	{
		//��ȡ����·��
		CString sSavePath = SaveDlg.GetPathName();

		CFile File;
		if(PathFileExists(sSavePath))//�ж��ļ��Ƿ����
			File.Remove(sSavePath);//������ɾ���ļ�

		CopyFile(_T("../DataBase/���̵�����Ϣ.xlsx"),sSavePath,FALSE);//���������ļ�
		CreateExcelFile(sSavePath);//����Excel
	}
}

//����Excel
void CAnalogSystemDlg::CreateExcelFile( CString FilePath )
{
	InitExcel();//��ʼ��Excel����
	Books.AttachDispatch(App.get_Workbooks());//�õ�����������
	/*�򿪹����������������������һ��������*/
	try
	{
		/*�����й�����*/
		lpDIsp = Books.Open(FilePath, vtMissing, vtMissing, vtMissing, vtMissing, vtMissing,
			vtMissing, vtMissing, vtMissing, vtMissing, vtMissing, vtMissing,
			vtMissing, vtMissing, vtMissing);
		Book.AttachDispatch(lpDIsp);
	}
	catch(...)
	{
		/*����һ���µĹ�����*/
		lpDIsp = Books.Add(vtMissing);
		Book.AttachDispatch(lpDIsp);
	}

	Sheets.AttachDispatch(Book.get_Sheets());//�õ��������е�sgeet����


	try
	{
		/*������sheet*/
		lpDIsp = Sheets.get_Item(_variant_t(_T("Sheet1")));
		Sheet.AttachDispatch(lpDIsp);
	}
	catch(...)
	{
		/*�����µ�sheet*/
		lpDIsp = Sheets.Add(vtMissing, vtMissing, _variant_t((long)1),vtMissing);
		Sheet.AttachDispatch(lpDIsp);
		Sheet.put_Name(_T("Sheet1"));
	}

	Range.AttachDispatch(Sheet.get_Cells(),TRUE);//�������е�Ԫ��

	for (int i=0,j=2; i<m_Ddinfo.GetCount();i++,j++)
	{
		CString str;
		m_Ddinfo.GetText(i,str);
		Range.put_Item(_variant_t((long)j), _variant_t((long)1),_variant_t(str));
	}

	Range.put_HorizontalAlignment(_variant_t((long)-4108));//����ˮƽ����
	Range.put_VerticalAlignment(_variant_t((long)-4108));//����ˮƽ��ֱ

	//ѡ�����У������ÿ��Ϊ����Ӧ
	Range = Range.get_EntireColumn();
	Range.AutoFit();

	/*�����ļ�*/
	//Book.SaveAs(_variant_t(FilePath), vtMissing, vtMissing, vtMissing, vtMissing,
	//	vtMissing, 0, vtMissing, vtMissing, vtMissing, vtMissing, vtMissing);
	Book.Save();
	ExitExcel();//�˳�Excel
}

//�˳�Excel
void CAnalogSystemDlg::ExitExcel( void )
{
	/******************�ر�EXCEL***********************/
	Range.ReleaseDispatch();
	Book.ReleaseDispatch();
	Books.ReleaseDispatch();
	Sheet.ReleaseDispatch();
	Sheets.ReleaseDispatch();
	App.Quit();
	App.ReleaseDispatch();
}

//ȫѡ
void CAnalogSystemDlg::OnBnClickedCheck2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	for(int i=0; i<m_CourseList.GetItemCount(); i++)
		m_CourseList.SetCheck(i,m_AllSel);
}

//��յ�����Ϣ
void CAnalogSystemDlg::OnBnClickedButtonClear()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//���������Ϣ
	for (int i = m_Ddinfo.GetCount(); i>=0; i--)
		m_Ddinfo.DeleteString(i);
}

//�ҵ��ؼ�
void CAnalogSystemDlg::EnableButton()
{
	BOOL bEnable = FALSE;
	if(m_ReadyPro.GetCount() == 0 && m_BlockPro.GetCount() == 0 && m_RunPro.GetCount() == 0)
		bEnable = FALSE;
	else
		bEnable = TRUE;

	GetDlgItem(IDC_BUTTON_RunPro)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON_SingleStep)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON_KillAllPor)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON_KillPor)->EnableWindow(bEnable);
}
