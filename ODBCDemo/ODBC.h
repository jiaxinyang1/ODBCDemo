#pragma once
#include<windows.h>
#include <sql.h>
#include <sqlext.h>
#include <Sqltypes.h>
#include <iostream>
#include<string>
using namespace  std;

class ODBC
{
	
private:
	SQLHANDLE hEnv= nullptr;
	SQLHANDLE hDbc = nullptr;
	SQLHANDLE hStmt = nullptr;
	SQLRETURN reCode;
	SQLRETURN retCode;

	//д╛хо
	char* DSN = const_cast<char*>("ODBCDemo");
	char* UserName = const_cast<char*>("hakurei");
	char* Password = const_cast<char*>("hakurei");

public:
	ODBC();
	ODBC(char *DSN, char *UserName, char *Password);
	~ODBC();
	bool Connnect();
	void ExecuteQuery(const string& sql) const;
	string getString(string name);
	bool next() const;
};
class InitException :public exception
{
public:
	InitException() :exception(" Error AllocHandle") {}
	InitException(const char *m):exception(m){}
};