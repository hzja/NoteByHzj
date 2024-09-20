#include <iostream>
#include <cmath>
using namespace std;
int main()
{
	float x,y;
	cin>>x;
	if(x<=-1)
		y=3*sqrt(fabs(x)+2)+1;
	else if(x<=10)
		y=x*x*x*x*x+1;
	else
		y=sqrt((x+1)/(2*x*x));
	cout<<y;
}