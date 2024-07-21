/*#include <iostream>
#include <cmath>
using namespace std;
int sum (int a,int b)
{
	return a+b;
}
double sum(double a,double b)
{
	return a+b;
}
float sum(float a,float b,float c){
	return a+b+c;
}
void main()
{
	cout<<"3+5="<<sum(3,5)<<endl;
	cout<<"2.2+5.6="<<sum(2.2,5.6)<<endl;
	cout<<"3.4+4+8="<<sum(3.4,4,8)<<endl;
}*/

/*#include <iostream>
#include <cmath>
using namespace std;
void delay(int loop=1000)
{
	for(;loop>0;loop--);
}
void main()
{
	delay(100);
	cout<<"延时100个时间单位"<<endl;
	delay();
	cout<<"延时1000个时间单位"<<endl;
}*/

#include <iostream>
#include <cmath>
using namespace std;
int IsEven (int n){
	return n%2?0:1;
}
void main ()
{
	int x,i;
    for (i=0;i<5;i++)
	{
		cin>>x;
		if(IsEven(x))cout<<"是偶数"<<endl;
		else cout<<"不是偶数"<<endl;
	}
}