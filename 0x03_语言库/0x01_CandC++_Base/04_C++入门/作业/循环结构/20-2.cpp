#include <iostream>
using namespace std;
#define M 100
int main()
{
	int n,c,s,z,i,m,k;
	int a[M];
	cin>>n;
	c=0;
	for(i=1;i<=n/2;i++)
	{
		s=0;s+=i;
		a[0]=i;
		z=0;
		for(m=i+1;m<=n;m++)
		{
			s+=m;
			z++;
			k=z;
			a[z]=m;
			if(s==n)
			{
				c++;
				for(z=0;z<=k;z++)
				{
					cout<<a[z]<<" ";
				}
				cout<<endl;
				break;
			}
			else if(s>n)
				break;
		}
	}
	if(c==0)
		cout<<"none";
}