/*#include <iostream>
#include <cmath>
using namespace std;
int main()
{
	int t=0,i=0;
	while(t<10000){
		t=t+pow(2,i);
		i++;
	}
	cout<<"总和为"<<t<<endl;
	cout<<i--;
	return 0;
}*/

/*#include <iostream>
#include <cmath>
using namespace std;
int main()
{

	int i=1,n;
	cin>>n;
	float sum=0.0;
	while(i<=n){
		sum=sum+1.0/i;
		i=i+1;
	}
	cout<<sum<<endl;
	return 0;
}*/

/*#include <iostream>
#include <cmath>
using namespace std;
int main()
{
	int i,n;
	cin>>n;
	float sum=0.0;
	for (i=1;i<=n;i++){
		sum=sum+1.0/i;
	}
	cout<<sum<<endl;
	return 0;
}*/

/*#include <iostream.h>
int main()
{
	int i,k,sum;
	sum=0;
	for(i=1;i<=9;i++){
		k=i*i;
		sum=sum+i;
		cout<<"i="<<i<<"　"<<"k="<<k<<endl;
	}
	cout<<"总和为"<<sum<<endl;
	return 0;
}*/

#include <iostream.h>
int main()
{
	int a,b,c;
	for(a=1;a<=9;a++){
		for(b=1;b<=9;b++){
			c=a*b;
			cout<<a<<"*"<<b<<"="<<c<<" ";
		}
	}
	return 0;
}




