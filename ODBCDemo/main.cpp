#include "ODBC.h"

int main(int argc, char* argv[])
{
	ODBC test;
	test.Connnect();
	// ReSharper disable once CppExpressionWithoutSideEffects
	test.ExecuteQuery("select *from C_jxy");
	while (test.next())
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
