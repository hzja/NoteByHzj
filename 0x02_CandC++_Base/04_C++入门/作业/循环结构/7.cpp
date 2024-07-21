#include <iostream>
using namespace std;
int main()
{
	int s=0;
	char t;
    cin>>t;
	while(t!='0')
	{
		s+=t;
		cin>>t;
	}
	cout<<s;
}