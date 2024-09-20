#include <iostream>
using namespace std;
int main()
{
	int n,i,m;
	float p,s,x,q;
	cin>>n>>x;
	s=1;p=1;q=1;m=-1;
	for(i=1;i<=n;i++)
	{
		p*=i;
		q*=x;
		m=-m;
		s+=q/p*m;
	}
	cout<<s;
}