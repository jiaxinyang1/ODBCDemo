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

ODBC::ODBC(char * DNS, char * UserName, char * Password):ODBC()
{

	pszDSN = DNS;
	pszUserName = UserName;
	pszPassword = Password;
}


ODBC::~ODBC()
{
	SQLDisconnect(hDbc);
	SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
	SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
}

bool ODBC::Connnect()
{
	//��������
	reCode=SQLConnect(hDbc, 
									reinterpret_cast<SQLCHAR*>(pszDSN), SQL_NTS,
									reinterpret_cast<SQLCHAR*>(pszUserName), SQL_NTS, 
									reinterpret_cast<SQLCHAR*>(pszPassword), SQL_NTS);
	if ((reCode != SQL_SUCCESS) && (reCode != SQL_SUCCESS_WITH_INFO))
	{
		SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
		throw InitException();
	}
	//���� statement���
	SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
	if ((retCode != SQL_SUCCESS) && (retCode != SQL_SUCCESS_WITH_INFO))
	{
	
		SQLDisconnect(hDbc);//ʧ�ܶϿ�����
		SQLFreeHandle(SQL_HANDLE_DBC, hDbc);//ʧ���ͷž��
		throw InitException();
	}
	

	return false;
}

int ODBC::ExecuteQuery(const string& sql) const
{
	//ִ�в�ѯ���
	SQLExecDirect(hStmt, reinterpret_cast<SQLCHAR*>(const_cast<char *>(sql.c_str())), SQL_NTS);
	//retCode = SQLExecDirect(hStmt, reinterpret_cast<SQLCHAR*>(sql), SQL_NTS);
	if ((retCode != SQL_SUCCESS) && (retCode != SQL_SUCCESS_WITH_INFO))
	{
		throw InitException();
	}

	return 0;
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
