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
	cout<<"����������ʵ����";
	cin>>x>>y;
	cout<<"����ʵ���Ƚϴ����"<<max(x,y)<<endl;
	return 0;
}




