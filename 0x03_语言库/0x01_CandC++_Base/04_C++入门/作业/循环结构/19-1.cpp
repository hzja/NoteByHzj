#include <iostream>
using namespace std;
int main()
{
	int m,n,min,i;
	cin>>m>>n;
	if(m>n)
		min=n;
	else
		min=m;
	//剔除m和n中共同的因子。若i是m、n共同的因子，要挑至m和n都不能被i整除为止。
	/*例如，m=8,n=12,i=2。剔除m和n中的因子2，得m=6,n=8;
	                      接着剔除2(循环中执行i--，然后执行表达式三的i++，使得下次循环时，i的值仍然为2)，得m=3，n=4。
						  此时2不再是m和n共同的因子，对2的处理结束。接着处理3，即循环中不再执行i--。
	*/
	for(i=2;i<=min;i++)
		if(m%i==0&&n%i==0)
		{
			m/=i;
			n/=i;
			i--;    
		}
	cout<<m<<" "<<n;
}

