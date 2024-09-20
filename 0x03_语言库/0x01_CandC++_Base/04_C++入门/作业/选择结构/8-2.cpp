#include <iostream>
using namespace std;
int main()
{
	char t[10];
	int i;
	cin>>t;
	for(i=0;t[i]!=0;i++)
	{
		t[i]+=4;
		if(t[i]>'z')
			t[i]-=26;
	}
	cout<<t;
}
