/*#include <iostream>
#include <cmath>
int st();
int at();
using namespace std;
void main()
{
	int i;
	for(i=0;i<5;i++)
		cout<<at()<<"\t";
	cout<<endl;
	for(i=0;i<5;i++)
		cout<<st()<<'\t';
	cout<<endl;
}
int st()
{
	static int t=100;
	t++;
	return t;
}
int at()
{
	int t=100;
	t++;
	return t;
}*/

#include <iostream>
#include <cmath>
using namespace std;
void fun2();
int n;
void main()
{
	n=1;
	fun2();
	cout<<"n="<<n<<endl;
}
extern int n;//外部变量声明
void fun2()
{
	n=3;
}


