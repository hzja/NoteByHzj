#include <iostream>
using namespace std;
int main()
{
	int m,n,m1,n1,r;
	cin>>m>>n;
	m1=m;n1=n;
	r=m%n;
	while(r!=0)
	{
		m=n;
		n=r;
		r=m%n;
	}
	cout<<m1/n<<" "<<n1/n;
}