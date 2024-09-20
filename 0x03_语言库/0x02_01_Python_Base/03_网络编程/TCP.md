# TCP

## 注意点

1. tcp服务器一般情况下都需要绑定，否则客户端找不到这个服务器
2. tcp客户端一般不绑定，因为是主动链接服务器，所以只要确定好服务器的ip、port等信息就好，本地客户端可以随机
3. tcp服务器中通过listen可以将socket创建出来的主动套接字变为被动的，这是做tcp服务器时必须要做的
4. 当客户端需要链接服务器时，就需要使用connect进行链接，udp是不需要链接的而是直接发送，但是tcp必须先链接，只有链接成功才能通信
5. 当一个tcp客户端连接服务器时，服务器端会有1个新的套接字，这个套接字用来标记这个客户端，单独为这个客户端服务
6. listen后的套接字是被动套接字，用来接收新的客户端的链接请求的，而accept返回的新套接字是标记这个新客户端的
7. 关闭listen后的套接字意味着被动套接字关闭了，会导致新的客户端不能够链接服务器，但是之前已经链接成功的客户端正常通信。
8. 关闭accept返回的套接字意味着这个客户端已经服务完毕
9. 当客户端的套接字调用close后，服务器端会recv解堵塞，并且返回的长度为0，因此服务器可以通过返回数据的长度来区别客户端是否已经下线



## 流程

### 服务器

1. 创建监听套接字（socket）

​		socket.socket(socket.AF_INET, socket.SOCK_STREAM)

​	2.绑定本地信息（bind）

​	3.监听端口，让默认的套接字由主动变为被动（listen）

​	4.创建服务套接字，等待客户端请求（accept）

​	5.接收数据/发送数据（recv、send）

​	6.关闭服务套接字（close）

​	7.关闭监听套接字（close） 



~~~ python
import socket


def main():
    # 1. 买个手机(创建监听套接字 socket)
    tcp_server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    # 2. 插入手机卡(绑定本地信息 bind)
    tcp_server_socket.bind(("", 7890))

    # 3. 将手机设置为正常的响铃模式(监听端口，让默认的套接字由主动变为被动listen)128代表并发
    tcp_server_socket.listen(128)

    # 循环目的：调用多次accept,从而为多个客户端服务
    while True:
        print("等待一个新的客户端的到来...")
        # 4. 等待别人的电话到来(创建服务套接字，等待客户端的链接 accept)
        new_client_socket, client_addr = tcp_server_socket.accept()

        print("一个新的客户端已经到来%s" % str(client_addr))
        
        # 循环目的: 为同一个客户端服务多次
        while True:
            # 5. 接收客户端发送过来的请求（recv、send）
            recv_data = new_client_socket.recv(1024)
            print("客户端福送过来的请求是:%s" % recv_data.decode("utf-8"))

            # 如果recv解堵塞，那么有2种方式：
            # 1. 客户端发送过来数据
            # 2. 客户端调用close导致而了 这里 recv解堵塞
            if recv_data:
                # 回送一部分数据给客户端
                new_client_socket.send("hahahghai-----ok-----".encode("utf-8"))
            else:
                break

        # 6. 关闭服务套接字
        # 关闭accept返回的套接字 意味着 不会在为这个客户端服务
        new_client_socket.close()
        print("已经为这个客户端服务完毕。。。。")

    # 7. 关闭监听套接字（close）关闭监听套接字如果将监听套接字 关闭了，那么会导致 不能再次等待新客户端的到来，即xxxx.accept就会失败
    tcp_server_socket.close()


if __name__ == "__main__":
    main()

~~~

### 客户端

1. 创建tcp的套接字（socket）

​		socket.socket(socket.AF_INET, socket.SOCK_STREAM)

​	2.链接服务器（connect）

​	3.发送数据/接收数据（send）

​	4.关闭套接字（close）

~~~ python
import socket


def main():
    # 1. 创建tcp的套接字
    tcp_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    # 2. 链接服务器
    # tcp_socket.connect(("192.168.33.11", 7890))
    server_ip = input("请输入要链接的服务器的ip:")
    server_port = int(input("请输入要链接的服务器的port:"))
    server_addr = (server_ip, server_port)
    tcp_socket.connect(server_addr)

    # 3. 发送数据/接收数据
    send_data = input("请输入要发送的数据:")
    tcp_socket.send(send_data.encode("utf-8"))

    # 4. 关闭套接字
    tcp_socket.close()


if __name__ == "__main__":
    main()

~~~



## 文件下载器

### 服务器 参考代码如下

~~~ python
from socket import *
import sys


def get_file_content(file_name):
    """获取文件的内容"""
    try:
        with open(file_name, "rb") as f:
            content = f.read()
        return content
    except:
        print("没有下载的文件:%s" % file_name)


def main():

    if len(sys.argv) != 2:
        print("请按照如下方式运行：python3 xxx.py 7890")
        return
    else:
        # 运行方式为python3 xxx.py 7890
        port = int(sys.argv[1])


    # 创建socket
    tcp_server_socket = socket(AF_INET, SOCK_STREAM)
    # 本地信息
    address = ('', port)
    # 绑定本地信息
    tcp_server_socket.bind(address)
    # 将主动套接字变为被动套接字
    tcp_server_socket.listen(128)

    while True:
        # 等待客户端的链接，即为这个客户端发送文件
        client_socket, clientAddr = tcp_server_socket.accept()
        # 接收对方发送过来的数据
        recv_data = client_socket.recv(1024)  # 接收1024个字节
        file_name = recv_data.decode("utf-8")
        print("对方请求下载的文件名为:%s" % file_name)
        file_content = get_file_content(file_name)
        # 发送文件的数据给客户端
        # 因为获取打开文件时是以rb方式打开，所以file_content中的数据已经是二进制的格式，因此不需要encode编码
        if file_content:
            client_socket.send(file_content)
        # 关闭这个套接字
        client_socket.close()

    # 关闭监听套接字
    tcp_server_socket.close()


if __name__ == "__main__":
    main()

~~~

### 客户端 参考代码如下

~~~ python
from socket import *


def main():

    # 创建socket
    tcp_client_socket = socket(AF_INET, SOCK_STREAM)

    # 目的信息
    server_ip = input("请输入服务器ip:")
    server_port = int(input("请输入服务器port:"))

    # 链接服务器
    tcp_client_socket.connect((server_ip, server_port))

    # 输入需要下载的文件名
    file_name = input("请输入要下载的文件名：")

    # 发送文件下载请求
    tcp_client_socket.send(file_name.encode("utf-8"))

    # 接收对方发送过来的数据，最大接收1024个字节（1K）
    recv_data = tcp_client_socket.recv(1024)
    # print('接收到的数据为:', recv_data.decode('utf-8'))
    # 如果接收到数据再创建文件，否则不创建
    if recv_data:
        with open("[接收]"+file_name, "wb") as f:
            f.write(recv_data)

    # 关闭套接字
    tcp_client_socket.close()


if __name__ == "__main__":
    main()
~~~

