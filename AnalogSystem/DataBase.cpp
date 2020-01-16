#include "StdAfx.h"
#include "DataBase.h"
#include "stdafx.h"

DataBase::DataBase(void)
{
}


DataBase::~DataBase(void)
{
	if(pCommand != NULL)
	{
		pCommand->Cancel();
		pCommand = NULL;
	}
	/******************关闭记录集***********************/
	if(pRecordset->State)
	{
		pRecordset->Close();
		pRecordset.Release();	
	}
	pRecordset = NULL;

	/*************关闭ACCESS数据库连接*****************/
	if(pConnection->State)
	{
		pConnection->Close();
		pConnection.Release();
	}
	pConnection = NULL;
}

//打开数据库
bool DataBase::ConnectionDatabase( void )
{
	try
	{
		if (!AfxOleInit())
		{
			AfxMessageBox(_T("OLE初始化出错!"));
			return false;
		}

		_bstr_t str = "Provider=Microsoft.ACE.OLEDB.12.0;Data Source=..\\DataBase\\SysDic.mdb; Persist Security Info=False";

		pConnection.CreateInstance(__uuidof(Connection));//创建连接
		pConnection->Open(str,"", "", adModeUnknown);//打开数据库
	}
	catch(_com_error& e)//捕捉异常
	{
		ShowErrorInfo(e);
		return false;
	}
	return true;
}

//打印错误信息
void DataBase::ShowErrorInfo( _com_error& e )
{
	CString StrError;
	_bstr_t bstrSource(e.Source());
	_bstr_t bstrDescription(e.Description()); 
	StrError.Format(_T("\nERROR INFO\r\n错误位置:%s\r\n错误描述:%s\r\n错误信息:%s")
		,(LPCTSTR)bstrSource, (LPCTSTR)bstrDescription, e.ErrorMessage());
	AfxMessageBox(StrError);
	return ;
}

_RecordsetPtr & DataBase::GetRecordSet( _bstr_t sql )
{
	pRecordset.CreateInstance(__uuidof(Recordset));
	try
	{
		pRecordset->CursorLocation = adUseClient;
		HRESULT info = 	pRecordset->Open(sql,		// sql语句  
			pConnection.GetInterfacePtr(),				// 获取库接库的IDispatch指针  
			adOpenDynamic,  
			adLockOptimistic,  
			adCmdText);  
	}
	catch(_com_error& e)//捕捉异常
	{
		ShowErrorInfo(e);
	}

	return pRecordset;
}

//执行sql语句
BOOL DataBase::ExecuteSQL( CString sql )
{
	pCommand.CreateInstance(__uuidof(Command));
	pCommand->ActiveConnection = pConnection;
	pCommand->CommandText = _bstr_t((LPCTSTR)sql);
	try
	{
		pCommand = pCommand->Execute(NULL, NULL, adCmdText);
		return true;
	}
	catch(_com_error &e)
	{
		ShowErrorInfo(e);
		return false;
	}
}