#include <stdio.h>
#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib")  //加载 ws2_32.dll

int main(){
    //初始化 DLL
    WSADATA wsaData;
    WSAStartup( MAKEWORD(2, 2), &wsaData);

    //创建套接字
    SOCKET servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    //绑定套接字
    sockaddr_in sockAddr;
    memset(&sockAddr, 0, sizeof(sockAddr));  //每个字节都用0填充
    sockAddr.sin_family = PF_INET;  //使用IPv4地址
    sockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");  //具体的IP地址
    sockAddr.sin_port = htons(1234);  //端口
    bind(servSock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));//servSock为socket文件描述符;(SOCKADDR)&sockAddr为sockaddr结构体变量指针;sizeof(SOCKADDR)是addr变量大小;
    
    //进入监听状态
    listen(servSock, 20);//servSock为需要进入监听状态的套接字;20为请求队列最大长度;

    //接收客户端请求
    SOCKADDR clntAddr;
    int nSize = sizeof(SOCKADDR);
    SOCKET clntSock = accept(servSock, (SOCKADDR*)&clntAddr, &nSize);//clntSock为一个新的套接字和客户端通信;servSock为服务器端套接字;(SOCKADDR)&clntAddr为sockaddr_in结构体变量;&nSize为参数addr长度;

    //向客户端发送数据
    char* str = (char*)"Hello World!";
    send(clntSock, str, strlen(str)+sizeof(char), NULL);//clntSock为要发送数据套接字;str为要发送数据的缓冲区地址;strlen(str)+sizeof(char)为要发送数据的字节数;NULL为发送数据时选项;
    //关闭套接字
    closesocket(clntSock);
    closesocket(servSock);

    //终止 DLL 的使用
    WSACleanup();

    return 0;
}