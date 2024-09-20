#include <iostream>
using namespace std;
int main()
{
	int n,g,s,b,i;
	cin>>n;
	if(n>=100&&n<=999)
	{
		for(i=100;i<=n;i++)
		{
			g=i%10;
			s=i/10%10;
			b=i/100;
			if(g*g*g+s*s*s+b*b*b==i)
				cout<<endl<<i;
		}
	}
}