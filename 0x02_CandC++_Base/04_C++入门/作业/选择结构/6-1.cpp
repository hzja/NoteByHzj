#include <iostream>
using namespace std;
int main()
{ int year,month,day;
  cin>>year>>month;
  if(month==2)
  {    
        if((year%4==0&&year%100!=0)||year%400==0) 
			day=29;
        else
			day=28;
  }
  else if(month<=7)
        if(month%2==1)
			day=31;
        else
			day=30;
  else
        if(month%2==1)
			day=30;
        else		
			day=31;
  cout<<day;
}
