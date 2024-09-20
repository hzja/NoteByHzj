#include <iostream>
using namespace std;
int main()
{
	int x,y;
	cin>>x;
	if(x%3==0)
	{
		if(x%7==0)
			y=3;
		else
			y=1;
	}
	else
		if(x%7==0)
			y=2;
		else
			y=4;
	cout<<y;
}