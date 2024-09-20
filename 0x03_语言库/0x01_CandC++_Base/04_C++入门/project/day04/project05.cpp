#include <stdio.h>
int main()
{
    char c[3][80];
    int i,j,d,x,s,k,q;
    d=x=s=k=q=0;
    for(i=0;i<3;i++) 
        gets(c[i]);   
    for(i=0;i<3;i++)  
        puts(c[i]);
    for(i=0;i<3;i++)
        for(j=0;c[i][j]!='\0';j++)
            if(c[i][j]>='A'&&c[i][j]<='Z')d++;
            else if(c[i][j]>='a'&&c[i][j]<='z')x++;
            else if(c[i][j]>='0'&&c[i][j]<='9')s++;
            else if(c[i][j]==' ')k++；
            else q++;
    printf("%d,%d,%d,%d,%d",d,x,s,k,q);
}   
