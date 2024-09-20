#include <iostream>
using namespace std;
int main()
{
	int m,n,min,i;
	cin>>m>>n;
	if(m>n)
		min=n;
	else
		min=m;
	for(i=2;i<=min;i++)
		if(m%i==0&&n%i==0)
		{
			m/=i;
			n/=i;
			i--;
		}
	cout<<m<<" "<<n;
}

