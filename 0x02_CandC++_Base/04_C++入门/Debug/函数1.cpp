#include <iostream>
#include <cmath>
using namespace std;
float max(float x,float y)
{
	if(x>y)
		return x;
	else
		return y;
}
int main()
{
	float x,y;
	cout<<"请输入两个实数：";
	cin>>x>>y;
	cout<<"两个实数比较大的是"<<max(x,y)<<endl;
	return 0;
}




