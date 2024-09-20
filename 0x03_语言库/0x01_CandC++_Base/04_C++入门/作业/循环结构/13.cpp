#include <iostream>
#include <cmath>
using namespace std;
int main()
{
	int i,j,m,n,gs,s,k;
	gs=0;s=0;
	cin>>m>>n;
	for(i=m;i<=n;i++)
	{
		k=sqrt(i);
		for(j=2;j<=k;j++)
			if(i%j==0)break;
		if(j==k+1&&i!=1)
		{
			s+=i;
			gs++;
		}
	}
	if(gs==0)
		cout<<"no";
	else
		cout<<gs<<" "<<s*1.0/gs;
}

