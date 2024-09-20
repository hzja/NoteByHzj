#include <iostream>
using namespace std;
int main()
{
	int i,n,a,b,t;
	float s;
	cin>>n;
	a=2;b=1;s=0;
	for(i=1;i<=n;i++)
	{
		s+=a*1.0/b;
		t=a;
		a=a+b;
		b=t;
	}
	cout<<s;
}