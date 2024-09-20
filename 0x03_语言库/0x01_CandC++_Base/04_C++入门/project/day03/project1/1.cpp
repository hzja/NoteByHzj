#include <iostream>
using namespace std;
int main()
{
    char s[10];
    int i;
    cin>>s;
    for(i=0;s[i]!='\0';i++)
    {
        s[i]+=4;
        if(s[i]>'z')
        {
            s[i]-=26;
        }
    }
    cout<<s;
}