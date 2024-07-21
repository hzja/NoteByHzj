#include <iostream>
using namespace std;
int main()
{
	int i=1;
	float s,q;
    cin>>q;
	s=0;
	while(s<=q)
	{
		s+=(i+1)*1.0/i;
		i++;
	}
	cout<<s;
}