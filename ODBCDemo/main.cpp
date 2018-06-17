#include "ODBC.h"


int main(int argc, char* argv[])
{
	
	try
	{
		ODBC test;
	
		test.Connnect();
		test.ExecuteQuery("delete from C_jxy where Cno=15");
		//test.ExecuteQuery("insert into C_jxy values('15','ÌåÓý','5','4')");
		test.ExecuteQuery("select *from C_jxy");
		while (test.next())
		{
			string Cno = test.getString("Cno");
			string Cname = test.getString("Cname");
			string Cpno = test.getString("Cpno");
			string Ccredit = test.getString("Ccredit");
			cout << Cno << "  " << Cname << "  " << Cpno << "  " << Ccredit << endl;
		}

		// ReSharper disable once CppExpressionWithoutSideEffects
	
	}
	catch (InitException e)
	{
		cout << e.what() << endl;
	}
	int n;
	cin >> n;
	
}
