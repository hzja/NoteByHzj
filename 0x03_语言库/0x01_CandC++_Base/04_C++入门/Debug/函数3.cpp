/*#include <iostream.h>
#include <math.h>
int n=100;
void fun()
{
	n*=2;
}
void main()
{
	n*=2;
	cout<<n<<endl;
	fun();
	cout<<n<<endl;
}

#include <iostream>
#include <cmath>
using namespace std;
void fun()
{
	auto int t=5;
	cout<<"fun()�е�t="<<t<<endl;
}
int main()
{
	float t=3.5;
	cout<<"main�е�t="<<t<<endl;
	fun();
	cout<<"main()�е�t="<<t<<endl;
	return 0;
}

#include <iostream.h>
int b=2;
void sub(int x,int y)
{
	b=x;
	x=y;
	y=b;
}

void main()
{
	int x3=10,x4=20;
	sub(x3,x4);
	cout<<b<<","<<x3<<","<<x4<<endl;
}*/

#include <iostream.h>
int d=1;
void fun(int p)
{
	int d=5;
	d+=p++;
	cout<<d;
}
void main()
{
	int a=3;
	fun(a);
	d+=a++;
	cout<<d<<endl;
}