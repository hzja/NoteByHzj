#include <iostream>
using namespace std;
int main()
{
	int n,i,m,s;
	cin>>n;
	s=0;
	for(i=1;i<=n;i++)
	{
		if(i%7!=0)
		{
			m=i;
			while(m!=0)
			{
				if(m%10==7)
					break;
				m/=10;
			}
			if(m==0)
				s+=i*i;
		}
	}
	cout<<s;
}

