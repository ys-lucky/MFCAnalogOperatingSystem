#pragma once

#include "DataBase.h"
#include "Resource.h"
#include "DataBase.h"
#include "afxwin.h"
// ProcessDig 对话框

class ProcessDig : public CDialogEx
{
	DECLARE_DYNAMIC(ProcessDig)

public:
	ProcessDig(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~ProcessDig();

// 对话框数据
	enum { IDD = IDD_DLG_Process };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	ProcessInfo *pro;
	CString DlgName;
	CString s_ConsumeMemory;	//所需内存
	CString s_OccupiedMemory;	//占用内存
	CString s_Priority;			//优先级
	CString s_ProcessName;		//进程名
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();

	CMap<CString, LPCTSTR, int, int&> mapProName;
	//进程名映射
	void CopyMap(CMap<CString, LPCTSTR, int, int&> &mapName,CMap<CString, LPCTSTR, int, int&> &mapName1)
	{
		mapName.RemoveAll();
		POSITION pos=mapName1.GetStartPosition();
		while(pos != NULL)
		{
			int Num;
			CString Name;
			mapName1.GetNextAssoc(pos,Name,Num);
			mapName[Name] = Num;
		}
	}
	int MaxMemSize;//内存大小
	BOOL m_CheckRun;//创建则运行进程
};
