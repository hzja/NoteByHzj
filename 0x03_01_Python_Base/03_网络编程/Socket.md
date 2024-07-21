# Socket

## 什么是socket

socket(简称 **套接字**) 是进程间通信的一种方式，它与其他进程间通信的一个主要不同是：能实现不同主机间的进程间通信；

我们网络上各种各样的服务大多都基于Socket完成通信，例如我们每天浏览网页、QQ 聊天、收发 email 等等；

## 多进程实现

~~~ PYHTON
import socket
import re
import multiprocessing


def service_client(new_socket):
    """为这个客户端返回数据"""
    ### 这段代码是一个简单的HTTP服务器，用于接收浏览器发送的HTTP请求，返回相应的HTTP响应。具体实现是通过socket模块建立TCP连		接，接收浏览器发送的HTTP请求，解析请求中的文件名，读取相应的文件内容，构造HTTP响应头和响应体，发送给浏览器。如果请求的			文件不存在，则返回404错误。

    # 1. 接收浏览器发送过来的请求 ，即http请求  
    # GET / HTTP/1.1
    # .....
    request = new_socket.recv(1024).decode("utf-8")
    ###这段代码是用于从新建立的套接字（socket）中接收数据，最多接收1024字节的数据，并将其解码为utf-8编码的字符串。通常情况下，		这段代码会放在一个循环中，以便不断地接收来自客户端的数据。
    # print(">>>"*50)
    # print(request)

    request_lines = request.splitlines()
    print("")
    print(">"*20)
    print(request_lines)

    # GET /index.html HTTP/1.1
    # get post put del
    # 这段代码的作用是从HTTP请求的第一行中提取出请求的文件名，如果请求的是根目录，则默认返回index.html文件。
    file_name = ""
    ret = re.match(r"[^/]+(/[^ ]*)", request_lines[0])
    if ret:
        file_name = ret.group(1)
        # print("*"*50, file_name)
        if file_name == "/":
            file_name = "/index.html"

    # 2. 返回http格式的数据，给浏览器 
    try:
        f = open("./html" + file_name, "rb")
        ### 这行代码是在Python中打开一个文件，文件路径是当前目录下的html文件夹中的一个特定文件，文件名由变量file_name指定。			打开文件的模式是二进制模式（"rb"），这意味着文件将以二进制格式读取。
    
    except:
        response = "HTTP/1.1 404 NOT FOUND\r\n"
        response += "\r\n"
        response += "------file not found-----"
        new_socket.send(response.encode("utf-8"))
        ###这段代码是一个简单的HTTP响应，表示请求的文件未找到。在HTTP响应中，第一行是状态行，包括HTTP版本号和状态码。这里的状			态码是404，表示请求的资源未找到。接下来的空行表示响应头结束，然后是响应体，这里是一个简单的字符串，表示文件未找到。最		  后，将整个响应编码为UTF-8格式，并通过新的套接字发送给客户端。
        
    else:
        html_content = f.read()
        f.close()
        # 上面两行是准备发送给浏览器的数据---header
        
        response = "HTTP/1.1 200 OK\r\n"
        response += "\r\n"
        # 上面两行是准备发送给浏览器的数据---boy
       
        # 下面一行是将response header发送给浏览器
        new_socket.send(response.encode("utf-8"))
        
        # 下面一行是将response body发送给浏览器
        new_socket.send(html_content)

    # 关闭套接
    new_socket.close()
    

def main():
    """用来完成整体的控制"""
    # 1. 创建套接字
    
    tcp_server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    ### 这段代码创建了一个TCP服务器端的socket对象，使用了IPv4地址族(AF_INET)和流式传输协议(SOCK_STREAM)
    ### 这个socket对象可以用来监听客户端的连接请求，并在连接建立后与客户端进行通信。
    
    tcp_server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
	### 这段代码是用来设置socket选项具体来说是设SO_REUSEADDR选项。
	### SO_REUSEADDR选项的作用是在socket关闭后允许立即重新绑定相同的端口号。
	### 这个选项在服务器程序中比较常用，因为服务器程序可能需要频繁地启动和关闭，如果不设置SO_REUSEADDR选项，那么在服务器程序关		 闭后，该端口号会被操作系统保留一段时间，这段时间内其他程序无法使用该端口号，从而导致服务器程序无法立即重新启动。设置		    SO_REUSEADDR选项可以避免这个问题。
	
    # 2. 绑定
    tcp_server_socket.bind(("", 7890))
	###这段代码是在创建一个TCP服务器的socket对象，并将其绑定到本地IP地址的7890端口上。其中，双引号中的空字符串表示绑定到本地所		有可用的IP地址上。如果想要指定绑定到特定的IP地址上，可以将双引号中的空字符串替换为相应的IP地址。绑定成功后，该socket对象		就可以监听来自客户端的连接请求，并接受客户端发送的数据。
	
    # 3. 变为监听套接字
    tcp_server_socket.listen(128)
	###这行代码是用于启动TCP服务器的监听功能，其中参数128表示服务器最多可以同时处理128个客户端连接请求。当TCP服务器启动后，它		会一直处于监听状态，等待客户端的连接请求。当有客户端连接请求到达时，服务器会接受该请求并创建一个新的套接字来处理该客户端		的请求。
	
    while True:
        # 4. 等待新客户端的链接
        new_socket, client_addr = tcp_server_socket.accept()
			###
			这段代码是在使用 Python 的 socket 模块创建一个 TCP 服务器，并等待客户端连接。当有客户端连接时，accept() 方法会			  返回一个新的 socket 对象和客户端的地址信息，可以通过这个 socket 对象与客户端进行通信。具体解释如下：
			- tcp_server_socket 是一个已经创建好的 TCP 服务器 socket 对象。
			- accept() 方法会阻塞程序，直到有客户端连接进来。
			- 当有客户端连接进来时，accept() 方法会返回一个新的 socket 对象和客户端的地址信息。
			- new_socket 是一个新的 socket 对象，可以使用它与客户端进行通信。
			- client_addr 是一个元组，包含客户端的 IP 地址和端口号。
		
        # 5. 为这个客户端服务
        p = multiprocessing.Process(target=service_client, args=(new_socket,))
        p.start()
			###
			   这段代码使用了Python的multiprocessing模块创建了一个新的进程，并将service_client函数作为进程的目标函数，传			    入了一个新的套接字new_socket作为参数。然后通过调用start()方法启动了这个进程，使其开始执行目标函数。这样可以实			      现多进程并发处理客户端请求，提高服务器的性能和并发处理能力。
			
        # 开启多进程，子进程会复制主进程的socket资源，类似硬链接，所以此处必须再次关闭，和多线程唯一的区别
        new_socket.close()
			###这是Python中关闭网络套接字的方法。当你使用网络套接字完成通信后，需要关闭套接字以释放资源。在Python中，可以使用`				new_socket.close()`方法来关闭套接字。这个方法会释放套接字占用的资源，并且将套接字从操作系统中移除。
			
    # 关闭监听套接字
    tcp_server_socket.close()


if __name__ == "__main__":
    main()
~~~



## 多线程实现

~~~ PYTHON
import socket
import re
import threading


def service_client(new_socket):
    """为这个客户端返回数据"""

    # 1. 接收浏览器发送过来的请求 ，即http请求  
    # GET / HTTP/1.1
    # .....
    request = new_socket.recv(1024).decode("utf-8")
    # print(">>>"*50)
    # print(request)

    request_lines = request.splitlines()
    print("")
    print(">"*20)
    print(request_lines)

    # GET /index.html HTTP/1.1
    # get post put del
    file_name = ""
    ret = re.match(r"[^/]+(/[^ ]*)", request_lines[0])
    if ret:
        file_name = ret.group(1)
        # print("*"*50, file_name)
        if file_name == "/":
            file_name = "/index.html"

    # 2. 返回http格式的数据，给浏览器 
    try:
        f = open("./html" + file_name, "rb")
    except:
        response = "HTTP/1.1 404 NOT FOUND\r\n"
        response += "\r\n"
        response += "------file not found-----"
        new_socket.send(response.encode("utf-8"))
    else:
        html_content = f.read()
        f.close()
        # 2.1 准备发送给浏览器的数据---header
        response = "HTTP/1.1 200 OK\r\n"
        response += "\r\n"
        # 2.2 准备发送给浏览器的数据---boy
        # response += "hahahhah"

        # 将response header发送给浏览器
        new_socket.send(response.encode("utf-8"))
        # 将response body发送给浏览器
        new_socket.send(html_content)

    # 关闭套接
    new_socket.close()
    

def main():
    """用来完成整体的控制"""
    # 1. 创建套接字
    tcp_server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    tcp_server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

    # 2. 绑定
    tcp_server_socket.bind(("", 7890))

    # 3. 变为监听套接字
    tcp_server_socket.listen(128)

    while True:
        # 4. 等待新客户端的链接
        new_socket, client_addr = tcp_server_socket.accept()

        # 5. 为这个客户端服务
        p = threading.Thread(target=service_client, args=(new_socket,))
        p.start()
		###这段代码是创建一个新的线程来为客户端提供服务。其中，`target`参数指定了线程要执行的函数，即`service_client`函数；			  `args`参数则指定了传递给`service_client`函数的参数，即`new_socket`，即客户端的socket对象。通过创建新的线程，			可以同时为多个客户端提供服务，提高服务器的并发处理能力。
        
    # 关闭监听套接字
    tcp_server_socket.close()


if __name__ == "__main__":
    main()
~~~

