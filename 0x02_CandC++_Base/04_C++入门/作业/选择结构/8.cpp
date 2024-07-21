#include <iostream>
#include <cmath>
using namespace std;
int main()
{
	char t1,t2,t3,t4,t5;
	cin>>t1>>t2>>t3>>t4>>t5;
	t1+=4;t2+=4;t3+=4;t4+=4;t5+=4;
	if(t1>'z')
		t1-=26;
	if(t2>'z')
		t2-=26;
	if(t3>'z')
		t3-=26;
	if(t4>'z')
		t4-=26;
	if(t5>'z')
		t5-=26;
	cout<<t1<<t2<<t3<<t4<<t5;
}
