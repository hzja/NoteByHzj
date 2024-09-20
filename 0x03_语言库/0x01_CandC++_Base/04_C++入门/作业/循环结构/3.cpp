#include <iostream>
using namespace std;
int main()
{
	int n,g,max;
	cin>>n;
	max=0;
	while(n!=0)
	{
		g=n%10;
		if(g>max)
			max=g;
		n/=10;
	}
	cout<<max;
}