#pragma once

#include "DataBase.h"
#include "Resource.h"
#include "DataBase.h"
#include "afxwin.h"
// ProcessDig �Ի���

class ProcessDig : public CDialogEx
{
	DECLARE_DYNAMIC(ProcessDig)

public:
	ProcessDig(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~ProcessDig();

// �Ի�������
	enum { IDD = IDD_DLG_Process };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	ProcessInfo *pro;
	CString DlgName;
	CString s_ConsumeMemory;	//�����ڴ�
	CString s_OccupiedMemory;	//ռ���ڴ�
	CString s_Priority;			//���ȼ�
	CString s_ProcessName;		//������
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();

	CMap<CString, LPCTSTR, int, int&> mapProName;
	//������ӳ��
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
	int MaxMemSize;//�ڴ��С
	BOOL m_CheckRun;//���������н���
};
