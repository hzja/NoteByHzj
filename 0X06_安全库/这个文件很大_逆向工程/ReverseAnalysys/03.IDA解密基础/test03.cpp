#include<stdio.h>
#include<string.h>

int main()
{
	int i;
	int len;
	char key[20];
	char res[20];
	char *num = "eastmount";     //��Կ 
	char *right = "123456789";   //��ȷֵ 
	
	//��������ȷ������
	printf("please input the key:");
	scanf("%s", &key);
	
	
	//�ж� TS@@XYBVM
	len = strlen(key);
	if(len<6 || len>10) {
		printf("Error, The length of the key is 6~10\n");
	} 
	else {
		//����
		for(i=0; i<len; i++) {
			res[i] = (key[i]^num[i]); //������ 
		}	 
		//printf("%s\n", res);
		if(strcmp(res, right)==0) {
			printf("You are right, Success.\n");
		} else {
			printf("Error, please input the right key.\n");
		}
	}
	
	return 0;
}
