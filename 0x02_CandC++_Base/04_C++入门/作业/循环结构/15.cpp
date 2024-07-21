#include <iostream>
using namespace std;
int main()
{
	int n,i,j,gs;
	float s;
	cin>>n;
	s=0;
	for(i=2;i<=n;i++)
	{
		gs=0;
		for(j=1;j<=i;j++)
			if(i%j==0)
				gs++;
		if(gs==2)
			s+=1.0/i;
	}
	cout<<s;
}

