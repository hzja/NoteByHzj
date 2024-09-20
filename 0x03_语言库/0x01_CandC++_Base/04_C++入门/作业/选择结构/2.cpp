#include <iostream>
using namespace std;
int main()
{
	float x;
	cin>>x;
	if(x-(int)x>=0.5)
		cout<<(int)x+1;
	else
		cout<<(int)x;
}