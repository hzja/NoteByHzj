#include <iostream>
using namespace std;
int main()
{
	int a,n,i,s,t;
	s=0;t=0;
	cin>>a>>n;
	for(i=0;i<n;i++)
	{
		t=t*10+a;
		s+=t;
	}
	cout<<s;
}