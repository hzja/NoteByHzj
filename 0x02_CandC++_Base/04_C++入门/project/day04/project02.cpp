#include <iostream>
#include <cstring>
using namespace std;
int main()
{
	char t1[50],t2[20];
	int d,s;
	cin>>t1;
	cin>>t2;
	d=strlen(t1);
	s=0;
	while(t2[s]!='\0')
	{
		t1[d]=t2[s];
		d++;s++;
	}
	t1[d]='\0';
	cout<<t1;
}
