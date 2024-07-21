#include <iostream>
using namespace std;
int main()
{
	int n,i,a=1,fw,ms;
    cin>>n;
	fw=0;ms=0;
	for(i=0;i<n;i++)
	{
		fw+=100000;
		ms+=a;
		a*=2;
		
	}
	cout<<fw<<endl<<ms;
}