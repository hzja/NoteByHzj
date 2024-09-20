#include <iostream>
using namespace std;
int main()
{
	int c,a=1,f=1,n,d,j;
	cin>>n;
	while(a<=n/2)
	{
		c=a;d=a;
		while(c<n)
		{
			d++;
			c+=d;
		}
		if(c==n)
		{
			for(j=a;j<=d;j++)
				cout<<j<<" ";
			cout<<endl;
			f=0;
		}
		a++;
	}
	if(f==1)
		cout<<"none";
}


