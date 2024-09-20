#include <stdio.h>
#include <winsock2.h>
#pragma comment (lib, "ws2_32.lib")

int main(){
	
    WSADATA wsaData;//WSDATA是结构体;
    WSAStartup( MAKEWORD(2, 2), &wsaData);
	//原型是int WSAStartup(WORD wVersionRequested,LPWSADATA lpWSAData);
    //wVersionRequested是WinSock规范的版本号,低字节为主版本号,高字节为副版本号;IpWSAData是指向WSAData结构体的指针

    printf("wVersion: %d.%d\n", LOBYTE(wsaData.wVersion), HIBYTE(wsaData.wVersion));//建议使用的版本号
    printf("wHighVersion: %d.%d\n", LOBYTE(wsaData.wHighVersion), HIBYTE(wsaData.wHighVersion));//支持的最高版本号
    printf("szDescription: %s\n", wsaData.szDescription);//以null结尾的字符串,用于说明ws2_32.dll实现以及厂商信息
    printf("szSystemStatus: %s\n", wsaData.szSystemStatus);//以null结尾字符串,用以说明ws2_32.dll状态以及配置信息

    return 0;
}