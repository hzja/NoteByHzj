#include <iostream>
#include <cmath>
using namespace std;
int main()
{
    float x,y;
	cin>>x;
    if(x<=-1.0){
	y=3*sqrt(-x+2)+1;
	}
	else{
		if(-1<x<=10){
			y=x*x*x*x*x+1.0;
		}
		else{
			y=sqrt((x+1.0)/(2x*x));
		}
	}
	cout<<y;
	return 0;
	}
