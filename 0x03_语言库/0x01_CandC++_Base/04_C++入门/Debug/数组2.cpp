/*#include <iostream>
#include <cmath>
using namespace std;
int main()
{
	int i,a[10];
	for(i=0;i<10;i++)
		a[i]=i*10;
	for(i=9;i>=0;i--)
		cout<<a[i]<<",";
	cout<<endl;
	return 0;
}*/

/*#include <iostream>
using namespace std;
void main()
{
	float cj[30];
	int c[6]={0},i,k;
	i=0;
	while(i<30)
	{
		cin>>cj[i];
		i++;
	}
	for(i=0;i<30;i++)
	{
		k=cj[i]/10;
		switch(k)
		{
		case 10:c[5]++;break;
        case 9:c[4]++;break;
		case 8:c[3]++;break;
		case 7:c[2]++;break;
		case 6:c[1]++;break;
		default:c[0]++;break;
		}
	}
	for(i=0;i<6;i++)
		cout<<"分数在第i个段位的人"<<c[i]<<endl;
}*/