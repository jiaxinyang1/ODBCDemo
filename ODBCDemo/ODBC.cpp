#include "ODBC.h"

ODBC::ODBC()
{
		//���价�����
		retCode=SQLAllocHandle(SQL_HANDLE_ENV, nullptr, &hEnv);

		if(retCode!=SQL_SUCCESS &&retCode!=SQL_SUCCESS_WITH_INFO)
			throw InitException();

		//ָʾ�������ø�ʽΪ ODBC 3.x
		retCode = SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, reinterpret_cast<SQLPOINTER>(SQL_OV_ODBC3), SQL_IS_INTEGER);

		if (retCode != SQL_SUCCESS && retCode != SQL_SUCCESS_WITH_INFO)
		{
			SQLFreeHandle(SQL_HANDLE_DBC, hEnv);//ʧ�����ͷŻ������
			throw  InitException();
		}
		//�������Ӿ��
		retCode=SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc);
		if (retCode != SQL_SUCCESS && retCode != SQL_SUCCESS_WITH_INFO)
		{
			SQLFreeHandle(SQL_HANDLE_DBC, hEnv);//ʧ�����ͷŻ������
			throw  InitException();
		}
	
}

ODBC::ODBC(char * DSN, char * UserName, char * Password):ODBC()
{

	this->DSN = DSN;
	this->UserName = UserName;
	this->Password = Password;
}



bool ODBC::Connnect()
{
	//��������
	reCode=SQLConnect(hDbc, 
									reinterpret_cast<SQLCHAR*>(DSN), SQL_NTS,
									reinterpret_cast<SQLCHAR*>(UserName), SQL_NTS, 
									reinterpret_cast<SQLCHAR*>(Password), SQL_NTS);
	if ((reCode != SQL_SUCCESS) && (reCode != SQL_SUCCESS_WITH_INFO))
	{
		SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
		throw InitException("Error Connect");
	}
	//���� statement���
	SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
	if ((retCode != SQL_SUCCESS) && (retCode != SQL_SUCCESS_WITH_INFO))
	{
	
		SQLDisconnect(hDbc);//ʧ�ܶϿ�����
		SQLFreeHandle(SQL_HANDLE_DBC, hDbc);//ʧ���ͷž��
		throw InitException();
	}
	

	return true;
}

void ODBC::ExecuteQuery(const string& sql) const
{
	//ִ�����
	SQLExecDirect(hStmt, reinterpret_cast<SQLCHAR*>(const_cast<char *>(sql.c_str())), SQL_NTS);
	if ((retCode != SQL_SUCCESS) && (retCode != SQL_SUCCESS_WITH_INFO))
	{
		throw InitException();
	}

	
}

string ODBC::getString(string name)
{
	SQLSMALLINT ColCount;
	int index = 0;//Ҫ���ҵ������ڵ��к�
	char sz_buf[256]{};//�������
	char pszBuf[50]{};//��Ų鵽������
	SQLINTEGER  buflen;//���ݳ���

	retCode = SQLNumResultCols(hStmt, &ColCount);
	if ((retCode != SQL_SUCCESS) && (retCode != SQL_SUCCESS_WITH_INFO))
	{
		throw InitException();
	}
	for (int i=0;i<ColCount+1;i++)
	{
		SQLColAttribute(hStmt, i, SQL_DESC_NAME, sz_buf, 256,nullptr, nullptr);
		const string buffer = sz_buf;
		if (buffer == name)
		{
			index = i;
			break;
		}
	}
	SQLGetData(hStmt, index, SQL_C_CHAR, pszBuf, 50, &buflen);
	if (buflen == -1)
	{
		return  string("null");
	}
	return string(pszBuf);
}

bool  ODBC::next() const
{
	return SQLFetch(hStmt) != SQL_NO_DATA;
}

ODBC::~ODBC()
{
	SQLDisconnect(hDbc);
	SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
	SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
}
