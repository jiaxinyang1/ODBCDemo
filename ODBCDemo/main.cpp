#include "ODBC.h"

int main(int argc, char* argv[])
{
	ODBC test;
	test.Connnect();
	test.ExecuteQuery(const_cast<char*>("select *from C_jxy"));
	while (test.next()>0)
	{
		string Cno = test.getString("Cno");
		string Cname = test.getString("Cname");
		string Cpno = test.getString("Cpno");
		string Ccredit = test.getString("Ccredit");
		cout << Cno << "  " << Cname << "  " << Cpno << "  " << Ccredit << endl;
	}
	int n;
	cin >> n;
	
}
