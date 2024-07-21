#include <iostream>
using namespace std;
int main()
{
	int n,i;
	float s,p;
	cin>>n;
	s=0;p=1;
	for(i=1;i<=n;i++)
	{
		p*=i;
		s+=p;
	}
	cout<<s;
}