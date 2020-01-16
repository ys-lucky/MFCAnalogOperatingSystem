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
	BOOL ExecuteSQL( CString sql );		//执行sql语句
private:
	_RecordsetPtr pRecordset;//记录集
	_ConnectionPtr pConnection;//创建一个数据库的 连接对象
	_CommandPtr pCommand;//命令对象 可以使用SQL语句
};

typedef struct Memory		//内存
{
	long MemoryID;			//内存ID
	int MemorySize;			//内存大小
	CString MemoryArea;		//内存区域
	CString MemoryState;	//内存状态
}MemoryInfo;

typedef struct Process		//进程
{
	CString ProcessName;		//进程名字
	int Priority;				//优先级
	int ConsumeMemory;			//所需内存
	CString ProcessState;		//进程状态
	MemoryInfo* OccupiedMemory;	//占用内存

}ProcessInfo;

typedef struct FreeMemory	//空闲内存
{
	POSITION pos;			//链表pos
	int MemorySize;			//内存大小
}FreeMem;

