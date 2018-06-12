#pragma once
#include<windows.h>
#include <sql.h>
#include <sqlext.h>
#include <Sqltypes.h>
#include <iostream>
#include<string>
#include <vector>
using namespace  std;

class ODBC
{
	
private:
	SQLHANDLE hEnv= nullptr;
	SQLHANDLE hDbc = nullptr;
	SQLHANDLE hStmt = nullptr;
	SQLRETURN reCode;
	SQLRETURN retCode;
	SQLSMALLINT colCount;
	int rowCount;
	//д╛хо
	char* pszDSN = const_cast<char*>("ODBCDemo");
	char* pszUserName = const_cast<char*>("hakurei");
	char* pszPassword = const_cast<char*>("hakurei");

public:
	ODBC();
	ODBC(char *DNS, char *UserName, char *Password);
	~ODBC();
	bool Connnect();
	int GetRowCount() { return rowCount; }
	int GetColCount() { return colCount; }
	
	int ExecuteQuery(const string& sql) const;
	string getString(string name);

	bool next() const;


};
class InitException :public exception
{
public:
	InitException() :exception(" Error AllocHandle") {}
};