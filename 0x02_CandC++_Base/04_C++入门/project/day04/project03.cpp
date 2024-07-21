#include <iostream>
using namespace std;
int main()
{
	char t1[50],t2[50];
	int i,c;
	cin>>t1;
	cin>>t2;
	for(i=0;t1[i]==t2[i];i++)
		if(t1[i]=='\0')
			break;
	c=t1[i]-t2[i];
	if(c<0)c=-c;
	cout<<c;
}
