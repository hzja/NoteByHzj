#include <iostream>
using namespace std;
int main()
{
	int n,i,j,s;
	cin>>n;
	for(i=1;i<n;i++)
	{
		s=0;
		for(j=1;j<=i/2;j++)
			if(i%j==0)
				s+=j;
		if(s==i)
			cout<<i<<" ";
	}
}

