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
	/******************�رռ�¼��***********************/
	if(pRecordset->State)
	{
		pRecordset->Close();
		pRecordset.Release();	
	}
	pRecordset = NULL;

	/*************�ر�ACCESS���ݿ�����*****************/
	if(pConnection->State)
	{
		pConnection->Close();
		pConnection.Release();
	}
	pConnection = NULL;
}

//�����ݿ�
bool DataBase::ConnectionDatabase( void )
{
	try
	{
		if (!AfxOleInit())
		{
			AfxMessageBox(_T("OLE��ʼ������!"));
			return false;
		}

		_bstr_t str = "Provider=Microsoft.ACE.OLEDB.12.0;Data Source=..\\DataBase\\SysDic.mdb; Persist Security Info=False";

		pConnection.CreateInstance(__uuidof(Connection));//��������
		pConnection->Open(str,"", "", adModeUnknown);//�����ݿ�
	}
	catch(_com_error& e)//��׽�쳣
	{
		ShowErrorInfo(e);
		return false;
	}
	return true;
}

//��ӡ������Ϣ
void DataBase::ShowErrorInfo( _com_error& e )
{
	CString StrError;
	_bstr_t bstrSource(e.Source());
	_bstr_t bstrDescription(e.Description()); 
	StrError.Format(_T("\nERROR INFO\r\n����λ��:%s\r\n��������:%s\r\n������Ϣ:%s")
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
		HRESULT info = 	pRecordset->Open(sql,		// sql���  
			pConnection.GetInterfacePtr(),				// ��ȡ��ӿ��IDispatchָ��  
			adOpenDynamic,  
			adLockOptimistic,  
			adCmdText);  
	}
	catch(_com_error& e)//��׽�쳣
	{
		ShowErrorInfo(e);
	}

	return pRecordset;
}

//ִ��sql���
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