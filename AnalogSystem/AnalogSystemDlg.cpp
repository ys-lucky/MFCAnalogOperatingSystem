
// AnalogSystemDlg.cpp : 实现文件
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


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx,public DataBase
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


// CAnalogSystemDlg 对话框




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
	//释放进程链表
	for(int i=0; i<arrProcess.GetSize(); i++)
	{
		ProcessInfo * PI = arrProcess.GetAt(i);
		delete PI;
	}
	arrProcess.RemoveAll();

	//释放内存链表
	POSITION pos = MemoryList.GetHeadPosition();
	while(pos != NULL)
	{
		MemoryInfo * MI = MemoryList.GetNext(pos);
		delete MI;
	}
	MemoryList.RemoveAll();

	//释放空闲内存数组
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


// CAnalogSystemDlg 消息处理程序

//空闲内存快速排序 从小到大排序
void FreeMemQuickSort(CArray<FreeMem *,FreeMem *> &Array, int low, int high)
{
	if(low > high) // 结束递归的条件
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

//优先级快速排序  从大到小排序
void PriorityQuickSort(CArray<ProcessInfo *,ProcessInfo *> &Array, int low, int high)
{
	if(low > high) // 结束递归的条件
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	arrFreeMem.RemoveAll(); //清空空闲内存数组
	ado.ConnectionDatabase();//连接数据库

	CRect rect;
	m_CourseList.GetClientRect(rect);//获取进程列表大小
	m_CourseList.SetExtendedStyle(m_CourseList.GetExtendedStyle()|LVS_EX_FULLROWSELECT |LVS_EX_GRIDLINES |LVS_EX_CHECKBOXES);
	m_CourseList.InsertColumn(0, _T("进程名"), LVCFMT_CENTER, rect.Width()/4);
	m_CourseList.InsertColumn(1, _T("优先级"), LVCFMT_CENTER,rect.Width()/4);
	m_CourseList.InsertColumn(2, _T("所需内存(k)"), LVCFMT_CENTER,rect.Width()/4);
	m_CourseList.InsertColumn(3, _T("进程状态"), LVCFMT_CENTER,rect.Width()/4);
	
	m_MemoryList.GetClientRect(rect);//获取内存列表大小
	m_MemoryList.SetExtendedStyle(m_MemoryList.GetExtendedStyle()|LVS_EX_FULLROWSELECT |LVS_EX_GRIDLINES);
	m_MemoryList.InsertColumn(0,_T("内存区域"),LVCFMT_CENTER, rect.Width()/3);
	m_MemoryList.InsertColumn(1,_T("内存大小"),LVCFMT_CENTER, rect.Width()/3);
	m_MemoryList.InsertColumn(2,_T("分配状态"),LVCFMT_CENTER, rect.Width()/3);

	EnableButton();
	InitList(_T("ProcessInfo"),_T("Priority DESC"));//初始化进程列表 DESC从大到小排序
	InitList(_T("SystemMemory"),_T("ID"));			//初始化内存列表
	RefreshReadyProList();			//初始化就绪进程列表
	UpdateData(FALSE);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CAnalogSystemDlg::OnPaint()
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
HCURSOR CAnalogSystemDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//初始化列表
void CAnalogSystemDlg::InitList(CString Table,CString sSort)
{
	_RecordsetPtr Rec;
	CString sql = _T("select * from " + Table + " order by " + sSort);//打开进程表
	Rec = ado.GetRecordSet((_bstr_t)sql);	//获取记录集

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

	if (Table == "SystemMemory")//排序空闲内存
		FreeMemQuickSort(arrFreeMem,0,arrFreeMem.GetSize()-1);
	//else if (Table == "ProcessInfo")//排序进程数组
	//	PriorityQuickSort(arrProcess,0,arrProcess.GetSize()-1);

	Rec->Close();//关闭记录集
	SetProcessNum(ProcessCount);//设置进程数
}

//初始化进程列表
void CAnalogSystemDlg::InitProcessList(_RecordsetPtr & Rec, int nRow) 
{
	_variant_t var;

	int size = Rec->Fields->Count;//获取字段总长度

	ProcessInfo *pro = new ProcessInfo; //定义进程类

	m_CourseList.InsertItem(nRow, _T(""));//插入每一行数据

	var = Rec->GetCollect("ProcessName");				//设置进程名

	if (var.vt != VT_NULL)
		pro->ProcessName = (LPCSTR)_bstr_t(var);
	m_CourseList.SetItemText(nRow, 0, pro->ProcessName);
	ProcessMapName[pro->ProcessName] = 1;

	var = Rec->GetCollect("Priority");					//设置优先级
	if (var.vt != VT_NULL)
		pro->Priority = var.lVal;
	CString sPriority;
	sPriority.Format(_T("%d"),pro->Priority);
	m_CourseList.SetItemText(nRow, 1, sPriority);

	var = Rec->GetCollect("ConsumeMemory");				//设置所需空间
	if (var.vt != VT_NULL)
		pro->ConsumeMemory = var.lVal;
	CString sConsumeMemory;
	sConsumeMemory.Format(_T("%d"),pro->ConsumeMemory);
	m_CourseList.SetItemText(nRow, 2, sConsumeMemory);

	var = Rec->GetCollect("ProcessState");				//设置进程状态

	if (var.vt != VT_NULL)
		pro->ProcessState = (LPCSTR)_bstr_t(var);
	m_CourseList.SetItemText(nRow, 3, pro->ProcessState);

	pro->OccupiedMemory = NULL;
	++ProcessCount;
	int nIndex = arrProcess.Add(pro);
	m_CourseList.SetItemData(nRow, nIndex);

	
}

//初始化内存列表
void CAnalogSystemDlg::InitMemoryList(_RecordsetPtr & Rec, int nRow)
{
	_variant_t var;
	int size = Rec->Fields->Count;//获取字段总长度
	MemoryInfo *Mem = new MemoryInfo; //定义进程类

	m_MemoryList.InsertItem(nRow, _T(""));//插入每一行数据
	
	var = Rec->GetCollect("MemoryArea");				//设置内存区域
	if (var.vt != VT_NULL)
		Mem->MemoryArea = (LPCSTR)_bstr_t(var);
	m_MemoryList.SetItemText(nRow, 0, Mem->MemoryArea);

	CString sNumber;
	var = Rec->GetCollect("MemorySize");				//设置内存大小
	if (var.vt != VT_NULL)
		Mem->MemorySize = var.lVal;
	sNumber.Format(_T("%d"),Mem->MemorySize);
	m_MemoryList.SetItemText(nRow, 1, sNumber);


	var = Rec->GetCollect("MemoryState");				//设置内存状态
	if (var.vt != VT_NULL)
		Mem->MemoryState = (LPCSTR)_bstr_t(var);
	m_MemoryList.SetItemText(nRow, 2, Mem->MemoryState);

	++MemoryCount;			
	Mem->MemoryID=MemoryCount;							//设置列索引
	m_MemoryList.SetItemData(nRow, (long)(void*)Mem);
	
	POSITION pos = MemoryList.AddTail(Mem);

	if (Mem->MemoryState == _T("空闲"))//将空闲内存加入数组
	{
		FreeMem *freemem = new FreeMem;//空闲内存
		freemem->MemorySize = Mem->MemorySize;
		freemem->pos = pos;
		arrFreeMem.Add(freemem);
	}
}

//刷新进程列表
void CAnalogSystemDlg::RefreshProcessList(CListCtrl & List, CArray<ProcessInfo *,ProcessInfo *> & arrPro )
{
	List.DeleteAllItems();//清空列表

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

	m_AllSel = FALSE;//刷新列表时,将全选勾去掉
	UpdateData(FALSE);
}

//刷新内存列表
void CAnalogSystemDlg::RefreshMemoryList( CListCtrl & List, CList<MemoryInfo *,MemoryInfo *> & MemList )
{
	List.DeleteAllItems();//清空列表

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

//进程列表双击左键消息处理
void CAnalogSystemDlg::OnNMDblclkCourselist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	if (pNMItemActivate == NULL || pNMItemActivate->iItem < 0)
		return;

	int nIndex = (int)m_CourseList.GetItemData(pNMItemActivate->iItem);

	ProcessDig dlg;
	dlg.pro = arrProcess.GetAt(nIndex);
	dlg.DlgName = _T("查看进程");
	dlg.DoModal();

}

//查找进程
void CAnalogSystemDlg::OnBnClickedButFindpro()
{
	// TODO: 在此添加控件通知处理程序代码
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
			m_CourseList.DeleteAllItems();	//清空列表

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
		AfxMessageBox(_T("未查找到该进程！"));
	}
}

//查找进程
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

//创建进程
void CAnalogSystemDlg::OnBnClickedButcreate()
{
	// TODO: 在此添加控件通知处理程序代码
	ProcessInfo *pro = new ProcessInfo;
	ProcessDig dlg;
	dlg.DlgName = _T("创建进程");
	dlg.pro = pro;
	dlg.CopyMap(dlg.mapProName,ProcessMapName);
	dlg.MaxMemSize = GetMemorySize()-10;//去掉OS所占内存

	if (dlg.DoModal() == IDOK)
	{
		arrProcess.Add(pro);
		++ProcessCount;

		SetProcessNum(ProcessCount);//设置进程数
		ProcessMapName[pro->ProcessName] = 1;

		if (dlg.m_CheckRun)
		{
			AllocationMemory(pro);//分配内存
			EnableButton();
		}

		PriorityQuickSort(arrProcess,0,arrProcess.GetSize()-1);//排序
		RefreshProcessList(m_CourseList, arrProcess);//刷新进程列表

		CString sql;
		sql.Format(_T("INSERT INTO ProcessInfo(ProcessName,Priority,ConsumeMemory,OccupiedMemory,ProcessState) \
			VALUES ('%s',%d,%d,' ','未运行')"),pro->ProcessName,pro->Priority,pro->ConsumeMemory);
		ado.ExecuteSQL(sql);
		AfxMessageBox(_T("创建成功"));
	}
}

//删除进程
void CAnalogSystemDlg::OnBnClickedButDeletepro()
{
	// TODO: 在此添加控件通知处理程序代码
	int nSize = m_CourseList.GetItemCount();
	BOOL bDel = FALSE;
	BOOL bRun = FALSE;
	for (int i=nSize-1; i >= 0; i--)
	{
		if (m_CourseList.GetCheck(i))//获取是否为选择状态
		{
			bDel=TRUE;
			int nIndex = (int)m_CourseList.GetItemData(i);
			ProcessInfo* pro = arrProcess.GetAt(nIndex);
			if (pro->ProcessState != "未运行")
			{
				bRun = TRUE;
				break;
			}
			--ProcessCount;
			ProcessMapName[pro->ProcessName] = 0;

			CString sql;
			sql.Format(_T("delete from ProcessInfo where ProcessName = '%s'"),pro->ProcessName);
			ado.ExecuteSQL(sql);//执行sql语句

			delete pro;//释放进程占用堆空间
			m_CourseList.DeleteItem(i);//删除列表中信息
			arrProcess.RemoveAt(nIndex);//删除该进程节点
		}
	}

	if (bRun)
	{
		AfxMessageBox(_T("当前进程正在运行,不允许删除!"));
		return ;
	}
	if (!bDel)
	{
		AfxMessageBox(_T("请选择需要删除的进程！"));
		return ;
	}
	RefreshProcessList(m_CourseList, arrProcess);//刷新进程列表列表
	SetProcessNum(ProcessCount);//设置进程数
}

//修改进程
void CAnalogSystemDlg::OnBnClickedButtonmodprocess()
{
	// TODO: 在此添加控件通知处理程序代码
	ProcessDig dlg;
	int nSize = m_CourseList.GetItemCount();
	BOOL bMod = FALSE;
	for (int i=0; i<nSize; i++)
	{
		int state = m_CourseList.GetItemState(i,LVIS_SELECTED);//获取是否为选择状态
		if (state)
		{
			bMod=TRUE;
			int nIndex = (int)m_CourseList.GetItemData(i);
			ProcessInfo* pro = arrProcess.GetAt(nIndex);
			dlg.pro = pro;
			dlg.DlgName = "修改进程";
			if (dlg.DoModal() == IDOK)
			{
				CString sql;
				sql.Format(_T("update ProcessInfo set Priority = %d,ConsumeMemory = %d where ProcessName = '%s'"),
					pro->Priority,pro->ConsumeMemory,pro->ProcessName);
				ado.ExecuteSQL(sql);//执行sql语句

				AfxMessageBox(_T("修改成功！"));
			}
			break;
		}
	}

	if (!bMod)
	{
		AfxMessageBox(_T("请选择需要修改的进程！"));
	}
	PriorityQuickSort(arrProcess,0,arrProcess.GetSize()-1);//优先级排序
	RefreshProcessList(m_CourseList, arrProcess);//刷新列表
}

//分配进程
void CAnalogSystemDlg::OnBnClickedButtonAllotpro()
{
	//TODO: 在此添加控件通知处理程序代码
	BOOL bState = FALSE;//是否已分配
	BOOL bSel = FALSE;	//是否选择
	BOOL bAllot = FALSE;//是否有合适内存分配
	for (int i=0; i<m_CourseList.GetItemCount(); i++)
	{
		if (m_CourseList.GetCheck(i))//获取是否为选择状态
		{
			bSel=TRUE;
			int nIndex = (int)m_CourseList.GetItemData(i);
			ProcessInfo* pro = arrProcess.GetAt(nIndex);

			if (pro->ProcessState != "未运行")//已分配
			{
				bState = TRUE;
				break;
			}

			AllocationMemory(pro);//分配内存
		}
	}
	if (bState)
		AfxMessageBox(_T("当前选择进程已分配内存!"));

	if (!bSel)
		AfxMessageBox(_T("请选择需要分配的进程！"));

	else if (bSel && !bState)
	{
		RefreshProcessList(m_CourseList,arrProcess);//刷新进程列表
		EnableButton();
		AfxMessageBox(_T("分配成功！"));
	}
}

//分配内存
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
			sNewMemArea.Format(_T("%d~%dk"),nStart,nNewEnd);//进程占用内存区域

			Pro->ProcessState = "就绪态";
			Mem->MemorySize = Pro->ConsumeMemory;
			Mem->MemoryArea = sNewMemArea;
			Mem->MemoryState = Pro->ProcessName;
			Pro->OccupiedMemory = Mem;

			if (Pro->ConsumeMemory != FM->MemorySize)//创建新的内存节点
			{
				MemoryInfo *NewMem = new MemoryInfo;
				++MemoryCount;
				NewMem->MemoryID = MemoryCount;
				sNewMemArea.Format(_T("%d~%dk"), nNewEnd+1, nEnd);
				NewMem->MemoryArea = sNewMemArea;
				NewMem->MemorySize = GetMemoryAreaSize(sNewMemArea);
				NewMem->MemoryState = _T("空闲");
				MemoryList.InsertAfter(pos,NewMem);
			}
			break;
		}
	}

	if (bAllot)
	{
		ResetFreeMemArr(arrFreeMem, MemoryList);	//重新生成空闲区域数组并排序
		RefreshMemoryList(m_MemoryList,MemoryList);	//刷新内存列表
		RefreshReadyProList();						//刷新就绪列表
	}
	else
	{
		Pro->ProcessState = "阻塞态";
		RefreshBlockProList();						//刷新阻塞列表
	}

	return bAllot;
}

//设置进程数
void CAnalogSystemDlg::SetProcessNum(int proNum)
{
	CString sNum;
	sNum.Format(_T("%d"),proNum);
	GetDlgItem(IDC_processNum)->SetWindowTextW(_T("当前已有进程："+sNum));
}

//设置控件颜色
HBRUSH CAnalogSystemDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	if (pWnd->GetDlgCtrlID()==IDC_processNum)//控件ID
	{
		pDC->SetTextColor(RGB(255,0,0)); //设置颜色
	}
	
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

//获取内存区域大小
int CAnalogSystemDlg::GetMemoryAreaSize( CString sMemArea )
{
	int nStart,nEnd;
	GetMemoryAreaStartAndEnd(sMemArea,nStart,nEnd);
	return nEnd-nStart+1;
}

//获取内存区域起始位和结束位
void CAnalogSystemDlg::GetMemoryAreaStartAndEnd(CString sMemArea, int & start,int & end )
{
	//提取该内存区域起始地址
	int nMark = sMemArea.Find(_T("~"));
	start = _ttoi(sMemArea.Left(nMark));

	//提取该内存区域结束地址
	int nRight = sMemArea.Find(_T("k"));
	int nCount = nRight-nMark;
	end = _ttoi(sMemArea.Mid(nMark+1,nCount-1));
}

//新内存区域
MemoryInfo * CAnalogSystemDlg::NewMemArea( CString proName, int MemSize, CString MemArea )
{
	++MemoryCount;
	MemoryInfo * Mem;
	Mem->MemoryID = MemoryCount;
	Mem->MemorySize = MemSize;
	return Mem;
}

//回收内存
void CAnalogSystemDlg::RecoveryMemory(CList<MemoryInfo *,MemoryInfo *> &MemoryList, MemoryInfo *CurMem)
{
	int nPreStart,nPreEnd;
	int nCurStart,nCurEnd;
	int nNextStart,nNextEnd;
	GetMemoryAreaStartAndEnd(CurMem->MemoryArea, nCurStart, nCurEnd);//获取当前内存区域的起始位和结束位

	BOOL bPrevFreeMem = FALSE;
	BOOL bNextFreeMem = FALSE;
	POSITION pos = MemoryList.Find(CurMem);
	MemoryInfo *PrevMem = NULL;
	MemoryList.GetPrev(pos);
	if (pos != NULL)
	{
		PrevMem = MemoryList.GetAt(pos);
		if (PrevMem->MemoryState == _T("空闲"))
		{
			bPrevFreeMem = TRUE;
			GetMemoryAreaStartAndEnd(PrevMem->MemoryArea, nPreStart, nPreEnd);//获取上一个内存区域的起始位和结束位
		}
	}

	pos = MemoryList.Find(CurMem);
	MemoryInfo *NextMem = NULL;
	MemoryList.GetNext(pos);
	if (pos != NULL)
	{
		NextMem = MemoryList.GetAt(pos);

		if (NextMem->MemoryState == _T("空闲"))
		{
			bNextFreeMem = TRUE;
			GetMemoryAreaStartAndEnd(NextMem->MemoryArea, nNextStart, nNextEnd);//获取下一个内存区域的起始位和结束位
		}
	}
	
	CString nNewMem;
	if (bPrevFreeMem && bNextFreeMem)//上下相邻
	{
		nNewMem.Format(_T("%d~%dk"),nPreStart, nNextEnd);
		CurMem->MemoryArea = nNewMem;
		CurMem->MemorySize = PrevMem->MemorySize + CurMem->MemorySize + NextMem->MemorySize;

		MemoryList.RemoveAt(MemoryList.Find(PrevMem));//删除上一个节点
		MemoryList.RemoveAt(MemoryList.Find(NextMem));//删除下一个节点
	}
	else if (bPrevFreeMem)//上相邻
	{
		nNewMem.Format(_T("%d~%dk"),nPreStart, nCurEnd);
		CurMem->MemoryArea = nNewMem;
		CurMem->MemorySize = PrevMem->MemorySize + CurMem->MemorySize;
		MemoryList.RemoveAt(MemoryList.Find(PrevMem));//删除上一个节点
	}
	else if(bNextFreeMem)//下相邻
	{
		nNewMem.Format(_T("%d~%dk"),nCurStart, nNextEnd);
		CurMem->MemoryArea = nNewMem;
		CurMem->MemorySize = CurMem->MemorySize + NextMem->MemorySize;
		MemoryList.RemoveAt(MemoryList.Find(NextMem));//删除下一个节点
	}

	CurMem->MemoryState = _T("空闲");
}

//重新生成空闲区域数组并排序
void CAnalogSystemDlg::ResetFreeMemArr(CArray<FreeMem *,FreeMem *>  & Array, CList<MemoryInfo *,MemoryInfo *> & MemList)
{
	Array.RemoveAll();//清空数组

	POSITION pos= MemList.GetHeadPosition();

	while(pos != NULL)//查找空闲区域
	{
		MemoryInfo * Mem = MemList.GetNext(pos);
		if (Mem->MemoryState == "空闲")
		{
			FreeMem *FM = new FreeMem;
			FM->pos = MemList.Find(Mem);
			FM->MemorySize = Mem->MemorySize;
			Array.Add(FM);
		}
	}

	if (Array.GetSize() > 1)//排序
		FreeMemQuickSort(Array,0,Array.GetSize()-1);
}

//杀死进程
void CAnalogSystemDlg::OnBnClickedButtonKillpor()
{
	// TODO: 在此添加控件通知处理程序代码
	int nSelIndex = -1;
	int ArrIndex=0;
	if ((nSelIndex = m_ReadyPro.GetCurSel()) != LB_ERR)//就绪进程
	{
		ArrIndex = m_ReadyPro.GetItemData(nSelIndex);
		m_ReadyPro.DeleteString(nSelIndex);
	}
	else if ((nSelIndex = m_BlockPro.GetCurSel()) != LB_ERR)//阻塞进程
	{
		ArrIndex = m_BlockPro.GetItemData(nSelIndex);
		m_BlockPro.DeleteString(nSelIndex);
	}
	else if ((nSelIndex = m_RunPro.GetCurSel()) != LB_ERR)//运行进程
	{
		ArrIndex = m_RunPro.GetItemData(nSelIndex);
		m_RunPro.DeleteString(nSelIndex);
	}

	ProcessInfo *Pro = NULL;
	if (nSelIndex != -1)
	{
		Pro = arrProcess[ArrIndex];
		if (Pro->ProcessState != "阻塞态")
		{
			RecoveryMemory(MemoryList, Pro->OccupiedMemory);//回收内存
			Pro->OccupiedMemory = NULL;
			ResetFreeMemArr(arrFreeMem, MemoryList);		//重新生成空闲区域数组并排序
			RefreshMemoryList(m_MemoryList, MemoryList);	//初始化内存列表
		}
		Pro->ProcessState = "未运行";

		if (m_BlockPro.GetCount() != 0)//从阻塞进程内查找优先级最高的进程，是否有足够内存可分配
		{
			CString proName;
			m_BlockPro.GetText(0,proName);
			ArrIndex = (int)m_BlockPro.GetItemData(0);
			ProcessInfo *Blockpro = arrProcess.GetAt(ArrIndex);
			if (GetMaxFreeMemSize(arrFreeMem) >= Blockpro->ConsumeMemory)
			{
				int ReadyIndex = m_ReadyPro.AddString(Blockpro->ProcessName);
				m_ReadyPro.SetItemData(ReadyIndex,ArrIndex);
				AllocationMemory(Blockpro);//分配内存
				m_BlockPro.DeleteString(0);
				RefreshMemoryList(m_MemoryList,MemoryList);//刷新内存列表
			}
		}

		RefreshProcessList(m_CourseList, arrProcess);	//初始化进程列表
		AfxMessageBox(_T("已杀死")+Pro->ProcessName+_T("进程"));
		EnableButton();
	}
	else
		AfxMessageBox(_T("请选择需要杀死的进程"));
}

//清除全部进程
void CAnalogSystemDlg::OnBnClickedButtonKillallpor()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_ReadyPro.GetCount() == 0 && m_BlockPro.GetCount() == 0 && m_RunPro.GetCount() == 0)
	{
		AfxMessageBox(_T("请先分配进程！"));
		return ;
	}

	int ArrIndex=0;
	while(TRUE)
	{
		int nSelIndex = -1;
		if ((nSelIndex = m_ReadyPro.GetCount()) != 0)//就绪进程
		{
			ArrIndex = m_ReadyPro.GetItemData(0);
			m_ReadyPro.DeleteString(0);
		}
		else if ((nSelIndex = m_BlockPro.GetCount()) != 0)//阻塞进程
		{
			ArrIndex = m_BlockPro.GetItemData(0);
			m_BlockPro.DeleteString(0);
		}
		else if ((nSelIndex = m_RunPro.GetCount()) != 0)//运行进程
		{
			ArrIndex = m_RunPro.GetItemData(0);
			m_RunPro.DeleteString(0);
		}

		ProcessInfo *Pro = NULL;
		if (nSelIndex != 0)
		{
			Pro = arrProcess[ArrIndex];
			if (Pro->ProcessState != "阻塞态")
			{
				RecoveryMemory(MemoryList, Pro->OccupiedMemory);//回收内存
			}
			Pro->OccupiedMemory = NULL;
			Pro->ProcessState = "未运行";
		}
		else
			break;
	}


	RefreshProcessList(m_CourseList, arrProcess);	//初始化进程列表
	ResetFreeMemArr(arrFreeMem, MemoryList);		//重新生成空闲区域数组并排序
	RefreshMemoryList(m_MemoryList, MemoryList);	//初始化内存列表
}

//刷新就绪进程列表
void CAnalogSystemDlg::RefreshReadyProList()
{
	m_ReadyPro.ResetContent();//清空所有信息
	for (int i=0; i<arrProcess.GetSize(); i++)
	{
		ProcessInfo *Pi = arrProcess.GetAt(i);
		if (Pi->ProcessState == "就绪态")
		{
			int nIndex = m_ReadyPro.AddString(Pi->ProcessName);
			m_ReadyPro.SetItemData(nIndex,(long)(void *)i);
		}
	}
}

//刷新阻塞进程列表  
void CAnalogSystemDlg::RefreshBlockProList()
{
	m_BlockPro.ResetContent();//清空所有信息
	for (int i=0; i<arrProcess.GetSize(); i++)
	{
		ProcessInfo *Pi = arrProcess.GetAt(i);
		if (Pi->ProcessState == "阻塞态")
		{
			int nIndex = m_BlockPro.AddString(Pi->ProcessName);
			m_BlockPro.SetItemData(nIndex,(long)(void *)i);
		}
	}
}

//得到总内存大小
int CAnalogSystemDlg::GetMemorySize()
{
	POSITION pos = MemoryList.GetTailPosition();
	MemoryInfo * Mem= MemoryList.GetAt(pos);
	int nStart,nEnd;
	GetMemoryAreaStartAndEnd(Mem->MemoryArea, nStart, nEnd);
	return nEnd;
}

//执行调度
void CAnalogSystemDlg::OnBnClickedButtonRunpro()
{
	// TODO: 在此添加控件通知处理程序代码
	
	//清空所有信息
	for (int i = m_Ddinfo.GetCount(); i>=0; i--)
		m_Ddinfo.DeleteString(i);
	m_Ddinfo.ResetContent();

	while(TRUE)
	{
		if (OperationDispatching())
			break;
	}
}

//单步执行
void CAnalogSystemDlg::OnBnClickedButtonSinglestep()
{
	// TODO: 在此添加控件通知处理程序代码
	//m_Ddinfo.ResetContent();//清空所有信息
	OperationDispatching();
}

//执行调度
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
		str.Format(proName+_T("进程...")+_T("执行完成..."));
		ReferenceMSG(str);
		str.Format(_T("释放")+proName+_T("进程占用内存:")+pro->OccupiedMemory->MemoryArea+_T("区域..."));
		pro->OccupiedMemory->MemoryState = "空闲";
		RecoveryMemory(MemoryList,pro->OccupiedMemory);//回收内存
		ResetFreeMemArr(arrFreeMem,MemoryList);		//重新生成空闲列表
		pro->OccupiedMemory = NULL;
		pro->ProcessState = "未运行";
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
		pro->ProcessState = "执行态";

		str.Format(_T("开始执行")+proName+_T("进程..."));
		ReferenceMSG(str);
		str.Format(_T("优先级:%d 所需内存:%d 所需执行时间:%d "),pro->Priority,pro->ConsumeMemory,pro->ConsumeMemory);
		ReferenceMSG(str);
	}

	if (m_BlockPro.GetCount() != 0)//从阻塞进程内查找优先级最高的进程，是否有足够内存可分配
	{
		m_BlockPro.GetText(0,proName);
		nIndex = (int)m_BlockPro.GetItemData(0);
		pro = arrProcess.GetAt(nIndex);
		if (GetMaxFreeMemSize(arrFreeMem) >= pro->ConsumeMemory)
		{
			int ReadyIndex = m_ReadyPro.AddString(pro->ProcessName);
			m_ReadyPro.SetItemData(ReadyIndex,nIndex);
			AllocationMemory(pro);//分配内存
			m_BlockPro.DeleteString(0);
			RefreshProcessList(m_CourseList,arrProcess);//刷新进程列表

			str.Format(_T("分配进程：%s 占用内存：%s"),pro->ProcessName,pro->OccupiedMemory->MemoryArea);
			ReferenceMSG(str);
		}
	}

	RefreshProcessList(m_CourseList,arrProcess);//刷新进程列表
	RefreshMemoryList(m_MemoryList,MemoryList);//刷新内存列表

	if(m_ReadyPro.GetCount() == 0 && m_BlockPro.GetCount() == 0 && m_RunPro.GetCount() == 0)
	{
		bFinish = TRUE;
		str.Format(_T("执行完成!"));
		ReferenceMSG(str);
		AfxMessageBox(str);
		EnableButton();
	}
	Sleep(500);
	return bFinish;
}

//显示调度信息
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

//得到最大空闲内存大小
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

//初始化Excel服务
void CAnalogSystemDlg::InitExcel( void )
{
	/*************创建Excel服务器(启动Excel)****************/
	if(!App.CreateDispatch(_T("Excel.Application")))
	{
		AfxMessageBox(_T("无法启动Excel服务器！"));
		return ;
	}
}

//导出Excel
void CAnalogSystemDlg::OnBnClickedButtonExportexcel()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog SaveDlg(FALSE,  _T(".xlsx"), _T("调度信息"), OFN_HIDEREADONLY|OFN_READONLY|OFN_OVERWRITEPROMPT ,
		_T("Excel File(*.xlsx)|*.xlsx|all file(*.*)|*.*||"),NULL);//另存为对话框,默认编辑框“调度信息.xlsx”


	INT_PTR Result = SaveDlg.DoModal();//模态显示另存为对话框

	if (Result == IDOK)//判断是否选择确定
	{
		//获取保存路径
		CString sSavePath = SaveDlg.GetPathName();

		CFile File;
		if(PathFileExists(sSavePath))//判断文件是否存在
			File.Remove(sSavePath);//存在则删除文件

		CopyFile(_T("../DataBase/进程调度信息.xlsx"),sSavePath,FALSE);//创建拷贝文件
		CreateExcelFile(sSavePath);//创建Excel
	}
}

//创建Excel
void CAnalogSystemDlg::CreateExcelFile( CString FilePath )
{
	InitExcel();//初始化Excel服务
	Books.AttachDispatch(App.get_Workbooks());//得到工作簿容器
	/*打开工作薄，如果不存在则新增一个工作薄*/
	try
	{
		/*打开已有工作薄*/
		lpDIsp = Books.Open(FilePath, vtMissing, vtMissing, vtMissing, vtMissing, vtMissing,
			vtMissing, vtMissing, vtMissing, vtMissing, vtMissing, vtMissing,
			vtMissing, vtMissing, vtMissing);
		Book.AttachDispatch(lpDIsp);
	}
	catch(...)
	{
		/*增加一个新的工作薄*/
		lpDIsp = Books.Add(vtMissing);
		Book.AttachDispatch(lpDIsp);
	}

	Sheets.AttachDispatch(Book.get_Sheets());//得到工作薄中的sgeet容器


	try
	{
		/*打开已有sheet*/
		lpDIsp = Sheets.get_Item(_variant_t(_T("Sheet1")));
		Sheet.AttachDispatch(lpDIsp);
	}
	catch(...)
	{
		/*创建新的sheet*/
		lpDIsp = Sheets.Add(vtMissing, vtMissing, _variant_t((long)1),vtMissing);
		Sheet.AttachDispatch(lpDIsp);
		Sheet.put_Name(_T("Sheet1"));
	}

	Range.AttachDispatch(Sheet.get_Cells(),TRUE);//加载所有单元格

	for (int i=0,j=2; i<m_Ddinfo.GetCount();i++,j++)
	{
		CString str;
		m_Ddinfo.GetText(i,str);
		Range.put_Item(_variant_t((long)j), _variant_t((long)1),_variant_t(str));
	}

	Range.put_HorizontalAlignment(_variant_t((long)-4108));//设置水平居中
	Range.put_VerticalAlignment(_variant_t((long)-4108));//设置水平垂直

	//选择整列，并设置宽度为自适应
	Range = Range.get_EntireColumn();
	Range.AutoFit();

	/*保存文件*/
	//Book.SaveAs(_variant_t(FilePath), vtMissing, vtMissing, vtMissing, vtMissing,
	//	vtMissing, 0, vtMissing, vtMissing, vtMissing, vtMissing, vtMissing);
	Book.Save();
	ExitExcel();//退出Excel
}

//退出Excel
void CAnalogSystemDlg::ExitExcel( void )
{
	/******************关闭EXCEL***********************/
	Range.ReleaseDispatch();
	Book.ReleaseDispatch();
	Books.ReleaseDispatch();
	Sheet.ReleaseDispatch();
	Sheets.ReleaseDispatch();
	App.Quit();
	App.ReleaseDispatch();
}

//全选
void CAnalogSystemDlg::OnBnClickedCheck2()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	for(int i=0; i<m_CourseList.GetItemCount(); i++)
		m_CourseList.SetCheck(i,m_AllSel);
}

//清空调度信息
void CAnalogSystemDlg::OnBnClickedButtonClear()
{
	// TODO: 在此添加控件通知处理程序代码
	//清空所有信息
	for (int i = m_Ddinfo.GetCount(); i>=0; i--)
		m_Ddinfo.DeleteString(i);
}

//灰掉控件
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
