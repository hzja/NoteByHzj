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
            cin>>target[i];//��Ŀ�����о��ǽ���B��վ������
        }
        int flag=1;
        while(B<=n)
        {
            //��ջ˳��ͳ�ջ˳����ͬ�������ִ�е���
            if(A==target[B]){A++;B++;}

            //��ջ˳�����ջ˳���෴(����)��������ִ�е���
            //��ջΪ�շ���true
            else if(!s.empty()&&s.top()==target[B]){s.pop();B++;}

            //��ջ���������ɽ�ջԪ�ؽ�ջ������һ�������ǵ�������Ϊ�������תվ
            else if(A<=n){s.push(A);A++;}

            //����˳��Ҳ�����򣬾ͱ���3,4,1,2�����ǲ����ڵ�
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
