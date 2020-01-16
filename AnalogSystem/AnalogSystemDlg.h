// AnalogSystemDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "AnalogSystem.h"
#include "DataBase.h"
#include "Excel//CApplication.h"
#include "Excel//CFont0.h"
#include "Excel//CRange.h"
#include "Excel//CWorkbook.h"
#include "Excel//CWorkbooks.h"
#include "Excel//CWorksheet.h"
#include "Excel//CWorksheets.h"

// CAnalogSystemDlg 对话框
class CAnalogSystemDlg : public CDialogEx
{
// 构造
public:
	CAnalogSystemDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CAnalogSystemDlg();
// 对话框数据
	enum { IDD = IDD_ANALOGSYSTEM_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);

	virtual BOOL OnInitDialog();
	void InitList(CString Table,CString sSort);							//初始化列表
	void InitProcessList(_RecordsetPtr & Rec, int nRow);	//初始化进程列表
	void InitMemoryList(_RecordsetPtr & Rec, int nRow);		//初始化内存列表
	DECLARE_MESSAGE_MAP()
public:
	int RunTime;				//运行时间
	BOOL m_AllSel;				//全选
	DataBase ado;				//数据库
	CString s_ProcessName;		//查找

	CListBox m_RunPro;			//执行进程
	CListBox m_ReadyPro;		//就绪进程
	CListBox m_BlockPro;		//阻塞进程
	CListBox m_Ddinfo;			//调度信息

	long ProcessCount;			//记录进程数
	long MemoryCount;			//记录内存数
	CListCtrl m_CourseList;		//进程列表
	CListCtrl m_MemoryList;		//内存列表
	CProgressCtrl m_Progress;	//进度条

	CMap<CString, LPCTSTR, int, int&> ProcessMapName;	//进程名映射
	CArray<ProcessInfo *,ProcessInfo *> arrProcess;		//进程数组
	CList<MemoryInfo *,MemoryInfo *> MemoryList;		//内存列表
	CArray<FreeMem *,FreeMem *> arrFreeMem;				//空闲内存数组

	void RefreshReadyProList();//刷新就绪进程列表
	void RefreshBlockProList();//刷新阻塞进程列表

	//查找进程
	afx_msg void OnBnClickedButFindpro();	
	ProcessInfo * FindProcess( CString Proname ,int &nIndex);

	void EnableButton();						//启动或关闭空间
	afx_msg void OnBnClickedCheck2();			//进程全选
	afx_msg void OnBnClickedButcreate();		//创建进程
	afx_msg void OnBnClickedButDeletepro();		//删除进程
	afx_msg void OnBnClickedButtonKillpor();	//杀死进程
	afx_msg void OnBnClickedButtonAllotpro();	//分配进程
	BOOL AllocationMemory(ProcessInfo *Pro);	//分配内存
	afx_msg void OnBnClickedButtonmodprocess();	//修改进程

	//进程调度
	BOOL OperationDispatching();				//执行调度
	afx_msg void OnBnClickedButtonRunpro();		//执行调度
	afx_msg void OnBnClickedButtonSinglestep();	//单步执行
	void ReferenceMSG(CString &slog);			//显示调度信息

	afx_msg void OnBnClickedButtonClear();		//清空调度信息
	afx_msg void OnBnClickedButtonKillallpor();	//清除全部进程
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);		//设置空间颜色
	afx_msg void OnNMDblclkCourselist(NMHDR *pNMHDR, LRESULT *pResult);		//双击鼠标左键
	
	int GetMemorySize();													//得到总内存大小
	void SetProcessNum(int proNum);											//设置进程数
	int GetMemoryAreaSize(CString sMemArea);								//获取内存区域大小
	int GetMaxFreeMemSize(CArray<FreeMem *,FreeMem *>  & Array);			//得到最大空闲内存大小
	MemoryInfo * NewMemArea(CString proName, int MemSize, CString MemArea);	//新内存区域
	void GetMemoryAreaStartAndEnd(CString sMemArea,int & start,int & end);	//获取内存区域起始位和结束位
	void RecoveryMemory(CList<MemoryInfo *,MemoryInfo *> &MemoryList, MemoryInfo *CurMem);					//回收内存
	void RefreshMemoryList(CListCtrl & List, CList<MemoryInfo *,MemoryInfo *> & MemList );					//刷新内存列表
	void RefreshProcessList(CListCtrl & List, CArray<ProcessInfo *,ProcessInfo *> & arrPro );				//刷新进程列表
	void ResetFreeMemArr(CArray<FreeMem *,FreeMem *>  & Array, CList<MemoryInfo *,MemoryInfo *> & MemList);	//重新生成空闲区域数组并排序
	

	//Excel 操作
	CApplication App;
	CWorkbook Book;
	CWorkbooks Books;
	CWorksheet Sheet;
	CWorksheets Sheets;
	CRange Range;
	LPDISPATCH lpDIsp;
	void CAnalogSystemDlg::InitExcel( void );		//初始化Excel服务
	afx_msg void OnBnClickedButtonExportexcel();	//导出excel
	void CreateExcelFile( CString FilePath );		//创建Excel
	void ExitExcel( void );							//退出Excel
	

};