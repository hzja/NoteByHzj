#include <iostream>
using namespace std;
int main()
{
	int m,i,k,j,s,f;
	cin>>m;
	f=0;
	for(i=1;i<=m/2;i++)
	{
		s=0;k=0;
		while(s<m)
		{
			k++;
			s=(2*i+k)*(k+1)/2;//计算数列i,i+1,i+2,...,i+k的和
		}
		if(s==m)
		{
			for(j=i;j<=i+k;j++)
				cout<<j<<" ";
			cout<<endl;
			f=1;
		}
	}
	if(f==0)
		cout<<"none";
}

