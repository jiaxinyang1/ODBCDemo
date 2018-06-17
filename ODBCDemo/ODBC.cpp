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

ODBC::ODBC(char * DSN, char * UserName, char * Password):ODBC()
{

	this->DSN = DSN;
	this->UserName = UserName;
	this->Password = Password;
}



bool ODBC::Connnect()
{
	//设置连接
	reCode=SQLConnect(hDbc, 
									reinterpret_cast<SQLCHAR*>(DSN), SQL_NTS,
									reinterpret_cast<SQLCHAR*>(UserName), SQL_NTS, 
									reinterpret_cast<SQLCHAR*>(Password), SQL_NTS);
	if ((reCode != SQL_SUCCESS) && (reCode != SQL_SUCCESS_WITH_INFO))
	{
		SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
		throw InitException("Error Connect");
	}
	//分配 statement句柄
	SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
	if ((retCode != SQL_SUCCESS) && (retCode != SQL_SUCCESS_WITH_INFO))
	{
	
		SQLDisconnect(hDbc);//失败断开连接
		SQLFreeHandle(SQL_HANDLE_DBC, hDbc);//失败释放句柄
		throw InitException();
	}
	

	return true;
}

void ODBC::ExecuteQuery(const string& sql) const
{
	//执行语句
	SQLExecDirect(hStmt, reinterpret_cast<SQLCHAR*>(const_cast<char *>(sql.c_str())), SQL_NTS);
	if ((retCode != SQL_SUCCESS) && (retCode != SQL_SUCCESS_WITH_INFO))
	{
		throw InitException();
	}

	
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

ODBC::~ODBC()
{
	SQLDisconnect(hDbc);
	SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
	SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
}
