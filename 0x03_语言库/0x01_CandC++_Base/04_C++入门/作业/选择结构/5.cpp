#include <iostream>
#include <cmath>
using namespace std;
int main()
{
	int m,n,g,s,b;
	cin>>m;
	if(m>=100&&m<=999)
	{
		g=m%10;
		s=m/10%10;
		b=m/100;
		n=g*100+s*10+b;
	}
	else
		n=-1;
	cout<<n;
}