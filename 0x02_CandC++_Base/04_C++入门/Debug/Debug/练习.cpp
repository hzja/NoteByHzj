#include <iostream>
#include <cmath>
using namespace std;
int main()
{/*
	float s,a,b,c,d;
	cin>>a>>b>>c;
	d=(a+b+c)/2;
	s=sqrt(d*(d-a)*(d-b)*(d-c));
	cout<<s;
	return 0;
}
	int F;
	float c;
	cin>>F;
	c=5.0/9*(F-32);
	cout<<c;
	return 0;
}
	int a,b,c,d;
	cin>>d;
	a=d/10;
	b=d%10/5;
	c=d%5/2;
	d=d%5%2;
	cout<<a<<" "<<b<<" "<<c<<" "<<d;
	return 0;
}
    char a,b,c;
	cin>>b;
	a=b-1;
	c=b+1;
	cout<<a<<" "<<b<<" "<<c<<endl;
	cout<<(int)a<<" "<<(int)b<<" "<<(int)c;
	return 0;
}*/
	float x;
	cin>>x;
	if(x-(int)x>=0.5){
		cout<<(int)x+1;}
	else{
		cout<<(int)x;
	}
	return 0;
}


 