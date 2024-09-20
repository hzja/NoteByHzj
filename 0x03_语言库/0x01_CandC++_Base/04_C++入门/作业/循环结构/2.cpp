#include <iostream>
using namespace std;
int main()
{
	int i,n,a,min;
	cin>>n;
	cin>>a;
	min=a;
	for(i=1;i<n;i++)
	{
		cin>>a;
		if(a<min)
			min=a;
	}
	cout<<min;
}