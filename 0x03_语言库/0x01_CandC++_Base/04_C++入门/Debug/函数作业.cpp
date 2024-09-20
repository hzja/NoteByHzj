#include <iostream>
#include <cmath>
using namespace std;
int main()
{
	float ds(int a);
	float s=0.0,a;
	int n,k;
	cin>>n;
	for(k=2;k<=n;k++){
		a=ds(k);
		s=s+a;
	}
	cout<<s;
	return 0;
	
}
float ds(int a)
{
	int n,f=0;
	for(n=2;n<a;n++)
	{
		if(a%n==0)
		{
			f=1;
			break;
		}
	}
	if(f==0){
		return 1.0/a;
	}
	else{
		return 0.0;
	}
}


	
	