#include <iostream>
#include <cmath>
using namespace std;
int main()
{
	float x,y;
	cin>>x;
	if(x<=-1)
		y=2*x+cos(x);
	else if(x<1)
		y=1/pow(x+1,2.0/3);
	else
		y=sin(x)*sin(x);
	cout<<y;
}