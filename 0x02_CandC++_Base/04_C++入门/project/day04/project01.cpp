#include <iostream>
using namespace std;
int main()
{
    char s[50];
    int i;
    cin>>s;
    for(i=0;s[i]!='\0';i++)
    {
        if(s[i]>='a'&&s[i]<='z')
        {
            s[i]+=4;
            if(s[i]>'z')
            {
                s[i]-=26;
            }
            else if(s[i]>='A'&&s[i]<='Z')
            {
                s[i]+=4;
                if(s[i]>'Z')
                {
                    s[i]-=26;
                }
            }
            else
            {
                s[i]=s[i];
            }
        }
    }
    cout<<s;
}