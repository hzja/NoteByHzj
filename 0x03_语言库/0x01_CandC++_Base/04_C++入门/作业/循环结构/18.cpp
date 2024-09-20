#include <iostream>
using namespace std;
int main()
{
	int n,a,b,f;
	f=0;
	cin>>n;
	for(a=1;a<=8;a++)
		for(b=a+1;b<=9;b++)
			if((a*10+b)*(b*10+a)==n)
			{
				f=1;
				cout<<a<<b<<endl;
			}
	if(f==0)
		cout<<"no";
}

