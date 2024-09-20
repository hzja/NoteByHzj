#include <iostream>
#include <cmath>
using namespace std;
int main()
{
	int i,m;
	float s,t,e;
	i=1;m=1;
	cin>>e;
	t=1;s=0;
	while(fabs(t)>=e)
	{
		s+=t;
		m=-m;
		i+=4;
		t=1.0/i*m;
	}
	cout<<s;
}