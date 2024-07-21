/*#include <iostream>
#include <cmath>
using namespace std;
int fac(int n)
{
	int y;
	cout<<n<<"\t";
	if(n==0||n==1)
		y=1;
	else
		y=n*fac(n-1);
	cout<<y<<"\t";
	return y;
}
void main()
{
	cout<<"\n4!="<<fac(4)<<endl;
}*/

/*#include <iostream>
#include <cmath>
using namespace std;
void move(char,char);
void hanoi(int,char,char,char);
void main()
{
	int n;
	cout<<"输入盘子数:"<<endl;
	cin>>n;
	hanoi(n,'A','B','C');
	cout<<endl;
}
void hanoi(int n,char source,char temp,char target)
{
	if(n==1)
		move(source,target);
	else
	{
		hanoi(n-1,source,target,temp);
			move(source,target);
		hanoi(n-1,temp,source,target);
	}
}
void move(char source,char target)
{
	cout<<source<<'-'<<target<<"\t";
}*/
//汉诺塔问题

/*#include <iostream>
#include <iomanip>
using namespace std;
int fib(int n)
{
	if(n==0)
		return 0;
	else if(n==1)
		return 1;
	else return fib (n-1)+fib(n-1);
}
void main ()
{
	for (int i=0;i<=19;i++)
	{
		if(i%5==0)
			cout<<endl;
		cout<<setw(6)<<fib(i);
	}
	cout<<endl;
}*/

#include <iostream>
using namespace std;
void backward(int);
void main()
{
	int n;
	cout<<"输入整数:"<<endl;
	cin>>n;
	cout<<"反向数:";
	backward(n);
	cout<<endl;
}
void backward(int n)
{
	cout<<n%10;
	if(n<10)
		return ;
	else 
		backward(n/10);
}





















































































































