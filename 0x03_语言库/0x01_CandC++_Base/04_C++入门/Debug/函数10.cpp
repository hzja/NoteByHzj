/*#include <iostream>
#include <cmath>
using namespace std;
#define N 2r//没有分号
#define M N+1
#define NUM (M+1)*M/2
void main()
{
	int i;
	for (i=1;i<=NUM;i++);
		cout<<i<<endl;
}*/
#include "iostream.h"
#define P 4
#define F(x) P*x*x
void main()
{
	int m=2,n=4;
	cout<<F(m+n)<<endl;//输出时宏展开
}