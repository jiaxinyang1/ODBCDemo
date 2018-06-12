#include "ODBC.h"
ODBC::ODBC()
{
		//分配环境句柄
		retCode=SQLAllocHandle(SQL_HANDLE_ENV, nullptr, &hEnv);
		if(retCode!=SQL_SUCCESS &&retCode!=SQL_SUCCESS_WITH_INFO)
			throw InitException();

		//指示函数调用格式为 ODBC 3.x
		retCode = SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, reinterpret_cast<SQLPOINTER>(SQL_OV_ODBC3), SQL_IS_INTEGER);
		if (retCode != SQL_SUCCESS && retCode != SQL_SUCCESS_WITH_INFO)
		{
			SQLFreeHandle(SQL_HANDLE_DBC, hEnv);//失败则释放环境句柄
			throw  InitException();
		}
		//设置连接句柄
		retCode=SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc);
		if (retCode != SQL_SUCCESS && retCode != SQL_SUCCESS_WITH_INFO)
		{
			SQLFreeHandle(SQL_HANDLE_DBC, hEnv);//失败则释放环境句柄
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
	//设置连接
	reCode=SQLConnect(hDbc, 
									reinterpret_cast<SQLCHAR*>(pszDSN), SQL_NTS,
									reinterpret_cast<SQLCHAR*>(pszUserName), SQL_NTS, 
									reinterpret_cast<SQLCHAR*>(pszPassword), SQL_NTS);
	if ((reCode != SQL_SUCCESS) && (reCode != SQL_SUCCESS_WITH_INFO))
	{
		SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
		throw InitException();
	}
	//分配 statement句柄
	SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
	if ((retCode != SQL_SUCCESS) && (retCode != SQL_SUCCESS_WITH_INFO))
	{
	
		SQLDisconnect(hDbc);//失败断开连接
		SQLFreeHandle(SQL_HANDLE_DBC, hDbc);//失败释放句柄
		throw InitException();
	}
	

	return false;
}

int ODBC::ExecuteQuery(const string& sql) const
{
	//执行查询语句
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
	int index = 0;//要查找的列所在的列号
	char sz_buf[256]{};//存放列名
	char pszBuf[50]{};//存放查到的数据
	SQLINTEGER  buflen;//数据长度

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
