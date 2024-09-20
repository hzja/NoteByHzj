/*
【问题描述】输入一个华氏温度，要求输出摄氏温度。公式为：c=5/9*(F-32)。

【样例输入】100

【样例输出】37.7778

【样例说明】100是华氏温度，根据公式计算得到它对应的摄氏温度是37.7778
 */
#include <iostream>
using namespace std;
int main()
{
	float f,c;
	cin>>f;
	c=5/9.0*(f-32);
	cout<<c;
}