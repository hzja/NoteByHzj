#include <iostream>
#include <cstdio>
#include <stack>
using namespace std;
const int maxn=1000;

int target[maxn];
int n;

int main()
{
    while(cin>>n)
    {
        stack<int>s;
        int A=1;
        int B=1;
        for(int i=1;i<=n;i++)
        {
            cin>>target[i];//该目标序列就是进入B车站的序列
        }
        int flag=1;
        while(B<=n)
        {
            //入栈顺序和出栈顺序相同，此语句执行到底
            if(A==target[B]){A++;B++;}

            //入栈顺序与出栈顺序相反(逆序)，则此语句执行到底
            //当栈为空返回true
            else if(!s.empty()&&s.top()==target[B]){s.pop();B++;}

            //入栈操作，将可进栈元素进栈，还有一个作用是调整车厢为逆序出中转站
            else if(A<=n){s.push(A);A++;}

            //即不顺序也不逆序，就比如3,4,1,2这种是不存在的
            else{flag=0;break;}
        }
        if(flag)
        {
            cout<<"YES\n";
        }
        else
        {
            cout<<"NO\n";
        }
    }
    return 0;
}
