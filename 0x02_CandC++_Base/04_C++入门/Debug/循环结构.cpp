/* #include <iostream.h>
void main()
{
	float cj,sum=0.0,average;
	int i=0;
p10:cin>>cj;
	sum=sum+cj;
	i=i+1;
	if(i<32)goto p10;
	average=sum/32;
	cout<<"sum="<<sum<<"\n";
	cout<<"average="<<average<<"\n";
}*/


#include <iostream.h>
void main()
{
	float a,sum=0.0,average;
	int i=0;
p10:if(i<32){
		cin>>a;
		sum=sum+a;
		i++;
		goto p10;
	}
	average=sum/32.0;
	cout<<"sum="<<sum;
	cout<<"\naverage="<<average<<endl;
}

