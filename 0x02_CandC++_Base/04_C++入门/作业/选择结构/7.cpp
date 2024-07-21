#include <iostream>
#include <cmath>
using namespace std;
int main()
{
	char t;
	int n;
	cin>>t;
	if(t>='a'&&t<='z')
	{
		t-=32;
		cout<<t;
	}
	else if(t>='A'&&t<='Z')
	{
		t+=32;
		cout<<t;
	}
	else if(t>='0'&&t<='9')
	{
		n=t-'0';
		cout<<n*n;
	}
	else
		cout<<t;
}
