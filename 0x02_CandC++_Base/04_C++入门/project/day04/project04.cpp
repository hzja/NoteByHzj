#include  <stdio.h>
int main ( )
{ 
    int c[10]={0},i;
    char a[50];
    gets(a);
    for(i=0;a[i]!='\0';i++)
        c[a[i] - 48]++;
    for(i=0;i<10;i++)
        printf(" %d,%d\n",i,c[i]); 
 }
