#include <iostream>
using namespace std;
int main()
{
	int a,b,gb,r;
	cin>>a>>b;
	gb=a*b;
	r=a%b;
	while(r!=0)
	{
		a=b;b=r;r=a%b;
	}
	cout<<b<<" "<<gb/b;
}