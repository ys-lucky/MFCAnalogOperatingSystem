#pragma once
class DataBase
{
public:
	DataBase(void);
	virtual ~DataBase(void);

public:
	bool ConnectionDatabase();
	void ShowErrorInfo( _com_error& e );
	_RecordsetPtr & GetRecordSet( _bstr_t sql );

	_RecordsetPtr getRec()
	{
		return pRecordset;
	}
	BOOL ExecuteSQL( CString sql );		//ִ��sql���
private:
	_RecordsetPtr pRecordset;//��¼��
	_ConnectionPtr pConnection;//����һ�����ݿ�� ���Ӷ���
	_CommandPtr pCommand;//������� ����ʹ��SQL���
};

typedef struct Memory		//�ڴ�
{
	long MemoryID;			//�ڴ�ID
	int MemorySize;			//�ڴ��С
	CString MemoryArea;		//�ڴ�����
	CString MemoryState;	//�ڴ�״̬
}MemoryInfo;

typedef struct Process		//����
{
	CString ProcessName;		//��������
	int Priority;				//���ȼ�
	int ConsumeMemory;			//�����ڴ�
	CString ProcessState;		//����״̬
	MemoryInfo* OccupiedMemory;	//ռ���ڴ�

}ProcessInfo;

typedef struct FreeMemory	//�����ڴ�
{
	POSITION pos;			//����pos
	int MemorySize;			//�ڴ��С
}FreeMem;

