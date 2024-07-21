/*#include <iostream>
#include <cmath>
using namespace std;
void main ()
{
	int a,b;
	cout<<"输入两个整数:"<<endl;
	cin>>a>>b;
	cout<<"a="<<a<<"\t"<<"b="<<b<<endl;
    if(b>a)
	{
		int t;
		t=a;
		a=b;
		b=t;
	}
	cout<<"a="<<a<<"\t"<<"b="<<b<<endl;
}*/

#include <iostream>
using namespace std;
int n=100;
void main()
{
	int i=200,j=300;
	cout<<n<<"\t"<<i<<"\t"<<j<<endl;
    {
		int i=500,j=600,n;
		n=i+j;
		cout<<n<<"\t"<<i<<"\t"<<j<<endl;
        cout<<::n<<endl;
	}
	n=i+j;
	cout<<n<<"\t"<<i<<"\t"<<j<<endl;
}
		
