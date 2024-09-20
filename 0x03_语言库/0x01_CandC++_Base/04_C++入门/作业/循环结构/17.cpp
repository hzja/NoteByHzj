#include <iostream>
using namespace std;
int main()
{
	int n,m,s,i,p,g;
	cin>>n;
	s=0;
	m=n;
	while(m!=0)
	{
		g=m%10;
		p=1;
		for(i=1;i<=g;i++)
			p*=i;
		s+=p;
		m/=10;
	}
	cout<<s<<endl;
	if(s==n)
		cout<<"yes";
	else
		cout<<"no";
}
