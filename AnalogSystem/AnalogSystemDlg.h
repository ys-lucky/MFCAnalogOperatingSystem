// AnalogSystemDlg.h : ͷ�ļ�
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

// CAnalogSystemDlg �Ի���
class CAnalogSystemDlg : public CDialogEx
{
// ����
public:
	CAnalogSystemDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CAnalogSystemDlg();
// �Ի�������
	enum { IDD = IDD_ANALOGSYSTEM_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);

	virtual BOOL OnInitDialog();
	void InitList(CString Table,CString sSort);							//��ʼ���б�
	void InitProcessList(_RecordsetPtr & Rec, int nRow);	//��ʼ�������б�
	void InitMemoryList(_RecordsetPtr & Rec, int nRow);		//��ʼ���ڴ��б�
	DECLARE_MESSAGE_MAP()
public:
	int RunTime;				//����ʱ��
	BOOL m_AllSel;				//ȫѡ
	DataBase ado;				//���ݿ�
	CString s_ProcessName;		//����

	CListBox m_RunPro;			//ִ�н���
	CListBox m_ReadyPro;		//��������
	CListBox m_BlockPro;		//��������
	CListBox m_Ddinfo;			//������Ϣ

	long ProcessCount;			//��¼������
	long MemoryCount;			//��¼�ڴ���
	CListCtrl m_CourseList;		//�����б�
	CListCtrl m_MemoryList;		//�ڴ��б�
	CProgressCtrl m_Progress;	//������

	CMap<CString, LPCTSTR, int, int&> ProcessMapName;	//������ӳ��
	CArray<ProcessInfo *,ProcessInfo *> arrProcess;		//��������
	CList<MemoryInfo *,MemoryInfo *> MemoryList;		//�ڴ��б�
	CArray<FreeMem *,FreeMem *> arrFreeMem;				//�����ڴ�����

	void RefreshReadyProList();//ˢ�¾��������б�
	void RefreshBlockProList();//ˢ�����������б�

	//���ҽ���
	afx_msg void OnBnClickedButFindpro();	
	ProcessInfo * FindProcess( CString Proname ,int &nIndex);

	void EnableButton();						//������رտռ�
	afx_msg void OnBnClickedCheck2();			//����ȫѡ
	afx_msg void OnBnClickedButcreate();		//��������
	afx_msg void OnBnClickedButDeletepro();		//ɾ������
	afx_msg void OnBnClickedButtonKillpor();	//ɱ������
	afx_msg void OnBnClickedButtonAllotpro();	//�������
	BOOL AllocationMemory(ProcessInfo *Pro);	//�����ڴ�
	afx_msg void OnBnClickedButtonmodprocess();	//�޸Ľ���

	//���̵���
	BOOL OperationDispatching();				//ִ�е���
	afx_msg void OnBnClickedButtonRunpro();		//ִ�е���
	afx_msg void OnBnClickedButtonSinglestep();	//����ִ��
	void ReferenceMSG(CString &slog);			//��ʾ������Ϣ

	afx_msg void OnBnClickedButtonClear();		//��յ�����Ϣ
	afx_msg void OnBnClickedButtonKillallpor();	//���ȫ������
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);		//���ÿռ���ɫ
	afx_msg void OnNMDblclkCourselist(NMHDR *pNMHDR, LRESULT *pResult);		//˫��������
	
	int GetMemorySize();													//�õ����ڴ��С
	void SetProcessNum(int proNum);											//���ý�����
	int GetMemoryAreaSize(CString sMemArea);								//��ȡ�ڴ������С
	int GetMaxFreeMemSize(CArray<FreeMem *,FreeMem *>  & Array);			//�õ��������ڴ��С
	MemoryInfo * NewMemArea(CString proName, int MemSize, CString MemArea);	//���ڴ�����
	void GetMemoryAreaStartAndEnd(CString sMemArea,int & start,int & end);	//��ȡ�ڴ�������ʼλ�ͽ���λ
	void RecoveryMemory(CList<MemoryInfo *,MemoryInfo *> &MemoryList, MemoryInfo *CurMem);					//�����ڴ�
	void RefreshMemoryList(CListCtrl & List, CList<MemoryInfo *,MemoryInfo *> & MemList );					//ˢ���ڴ��б�
	void RefreshProcessList(CListCtrl & List, CArray<ProcessInfo *,ProcessInfo *> & arrPro );				//ˢ�½����б�
	void ResetFreeMemArr(CArray<FreeMem *,FreeMem *>  & Array, CList<MemoryInfo *,MemoryInfo *> & MemList);	//�������ɿ����������鲢����
	

	//Excel ����
	CApplication App;
	CWorkbook Book;
	CWorkbooks Books;
	CWorksheet Sheet;
	CWorksheets Sheets;
	CRange Range;
	LPDISPATCH lpDIsp;
	void CAnalogSystemDlg::InitExcel( void );		//��ʼ��Excel����
	afx_msg void OnBnClickedButtonExportexcel();	//����excel
	void CreateExcelFile( CString FilePath );		//����Excel
	void ExitExcel( void );							//�˳�Excel
	

};