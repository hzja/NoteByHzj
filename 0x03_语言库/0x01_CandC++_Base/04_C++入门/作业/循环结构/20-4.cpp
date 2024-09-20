#include <iostream>
using namespace std;
int main()
{	int n,s,i,j,m,f,a[5010];
	cin>>n;
	f=0;
	for(i=1;i<=n/2+1;i++)
		a[i]=i;
	for(i=1;i<=n/2+1;i++)
	{	
		s=0;
		for(j=i;s<n;j++)
			s+=a[j];
		if(s==n)
		{
			f=1;
			for(m=i;m<j;m++)
				cout<<a[m]<<" ";
			cout<<endl;
		}			
	}
	if(f==0)
		cout<<"none";
}