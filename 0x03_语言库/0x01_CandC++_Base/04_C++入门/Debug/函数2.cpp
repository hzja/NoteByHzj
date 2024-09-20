/*#include <iostream>
#include <cmath>
using namespace std;
float TriangleArea (float a,float b,float c)
{
	float area,z;
	z=(a+b+c)/2;
	area=sqrt(z*(z-a)*(z-b)*(z-c));
	if(a+b>c&&a+c>b&&b+c>a)
		return area;
	else
		return -1;
}
int main()
{
	float a,b,c,area;
	cout<<"请输入三角形三条边的值:";
	cin>>a>>b>>c;
	area=TriangleArea(a,b,c);
	if (area==-1)
		cout<<"您所输入的三个数值无法组成三角形的三条边。";
	else 
		cout<<"三角形面积是"<<area;
	return 0;
}*/

/*#include<iostream.h>
#include<iomanip.h>
int palindrome (int);
int main()
{
	int m;
	cout<<setw(10)<<"m"<<setw(20)<<"m*m"<<setw(20)<<"m*m*m"<<endl;
	for(m=11;m<1000;m++){
		if(palindrome(m)&&palindrome(m*m)&&palindrome(m*m*m)){
			cout<<setw(10)<<m<<setw(20)<<m*m<<setw(20)<<m*m*m<<endl;
		}
	}
	return 0;
}

int palindrome (int n)
{
	int m=n,k=0;
	do {
		k=k*10+n%10;
		n/=10;
	}while (n>0);
	return (k==m);
}*/

#include <iostream>
using namespace std;
int prime (int);
void main ()
{
	int m;
	for (m=100;m<=200;m++)
	{
		if(prime(m)){
			cout<<" "<<m;
		}
	}
		cout<<endl;
}
int prime (int m)
{
	int i,yes=1;
	for(i=2;i<=m/2;i++)
		if(m%i==0){
			yes=0;
			break;
		}
		return (yes);
}




