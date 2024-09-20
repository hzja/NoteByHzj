PythonParamiko<br />Paramiko是用Python语言写的一个模块，远程连接到Linux服务器，查看上面的日志状态，批量配置远程服务器，文件上传，文件下载等<br />paramiko是一个基于SSH用于连接远程服务器并执行相关操作（SSHClient和SFTPClinet，即一个是远程连接，一个是上传下载服务），使用该模块可以对远程服务器进行命令或文件操作，值得一说的是，fabric和ansible内部的远程管理就是使用的paramiko来现实。
<a name="wvnAX"></a>
## 简介
ssh是一个协议，OpenSSH是其中一个开源实现，paramiko是Python的一个库，实现了SSHv2协议(底层使用cryptography)。<br />有了Paramiko以后，就可以在Python代码中直接使用SSH协议对远程服务器执行操作，而不是通过ssh命令对远程服务器进行操作。<br />由于paramiko属于第三方库，所以需要使用如下命令先行安装：
```bash
pip3 install paramiko
```
<a name="snwSp"></a>
## Paramiko介绍
paramiko包含两个核心组件：**SSHClient和SFTPClient**。

- SSHClient的作用类似于Linux的ssh命令，是对SSH会话的封装，该类封装了传输(Transport)，通道(Channel)及SFTPClient建立的方法(open_sftp)，通常用于执行远程命令。
- SFTPClient的作用类似与Linux的sftp命令，是对SFTP客户端的封装，用以实现远程文件操作，如文件上传、下载、修改文件权限等操作。
```python
"""
# Paramiko中的几个基础名词：
1、Channel：是一种类Socket，一种安全的SSH传输通道；
2、Transport：是一种加密的会话，使用时会同步创建了一个加密的Tunnels(通道)，这个Tunnels叫做Channel；
3、Session：是client与Server保持连接的对象，用connect()/start_client()/start_server()开始会话。
"""
```
<a name="WQPYY"></a>
## Paramiko的基本使用
<a name="c6NNN"></a>
### SSHClient常用的方法介绍
`**connect()**`：实现远程服务器的连接与认证，对于该方法只有hostname是必传参数。
```python
"""
常用参数
hostname 连接的目标主机
port=SSH_PORT 指定端口
username=None 验证的用户名
password=None 验证的用户密码
pkey=None 私钥方式用于身份验证
key_filename=None 一个文件名或文件列表，指定私钥文件
timeout=None 可选的tcp连接超时时间
allow_agent=True, 是否允许连接到ssh代理，默认为True 允许
look_for_keys=True 是否在~/.ssh中搜索私钥文件，默认为True 允许
compress=False, 是否打开压缩
"""
```
`**set_missing_host_key_policy()**`：设置远程服务器没有在know_hosts文件中记录时的应对策略。目前支持三种策略：
```python
"""
设置连接的远程主机没有本地主机密钥或HostKeys对象时的策略，目前支持三种：
AutoAddPolicy 自动添加主机名及主机密钥到本地HostKeys对象，不依赖load_system_host_key的配置。即新建立ssh连接时不需要再输入yes或no进行确认
WarningPolicy 用于记录一个未知的主机密钥的python警告。并接受，功能上和AutoAddPolicy类似，但是会提示是新连接
RejectPolicy 自动拒绝未知的主机名和密钥，依赖load_system_host_key的配置。此为默认选项
"""
```
`**exec_command()**`：在远程服务器执行Linux命令的方法。<br />`**open_sftp()**`：在当前ssh会话的基础上创建一个sftp会话。该方法会返回一个SFTPClient对象。
```python
"""
# 利用SSHClient对象的open_sftp()方法，可以直接返回一个基于当前连接的sftp对象，可以进行文件的上传等操作. 
"""
sftp = client.open_sftp()
sftp.put('local.txt','remote.txt')
```
<a name="G7ipb"></a>
### SSHClient常用的方法举例
```python
import paramiko   
# 实例化SSHClient  
ssh_client = paramiko.SSHClient()   
# 自动添加策略，保存服务器的主机名和密钥信息，如果不添加，那么不再本地know_hosts文件中记录的主机将无法连接 ，此方法必须放在connect方法的前面
ssh_client.set_missing_host_key_policy(paramiko.AutoAddPolicy())   
# 连接SSH服务端，以用户名和密码进行认证 ，调用connect方法连接服务器
ssh_client.connect(hostname='192.168.137.105', port=22, username='root', password='123456')   
# 打开一个Channel并执行命令  结果放到stdout中，如果有错误将放到stderr中
stdin, stdout, stderr = ssh_client.exec_command('df -hT ') 
# stdout 为正确输出，stderr为错误输出，同时是有1个变量有值   # 打印执行结果  print(stdout.read().decode('utf-8'))  
# 关闭SSHClient连接 
ssh_client.close()
```
密钥连接方式：
```python
# 配置私人密钥文件位置
private = paramiko.RSAKey.from_private_key_file('/root/.ssh/id_rsa') 
#实例化SSHClient
ssh_client = paramiko.SSHClient() 
#自动添加策略，保存服务器的主机名和密钥信息，如果不添加，那么不再本地know_hosts文件中记录的主机将无法连接
ssh_client.set_missing_host_key_policy(paramiko.AutoAddPolicy()) 
#连接SSH服务端，以用户名和密码进行认证
ssh_client.connect(
	hostname='192.168.137.100',
	port=22,
	username='root',
	pkey=private
)
```
<a name="Aw5xV"></a>
### SFTPClient常用方法介绍
```python
SFTPCLient作为一个sftp的客户端对象，根据ssh传输协议的sftp会话，实现远程文件操作，如上传、下载、权限、状态 
from_transport(cls,t) 创建一个已连通的SFTP客户端通道
put(localpath, remotepath, callback=None, confirm=True) 将本地文件上传到服务器 
参数confirm：是否调用stat()方法检查文件状态，返回ls -l的结果
get(remotepath, localpath, callback=None) 从服务器下载文件到本地
mkdir() 在服务器上创建目录
remove() 在服务器上删除目录
rename() 在服务器上重命名目录
stat() 查看服务器文件状态
listdir() 列出服务器目录下的文件
```
<a name="xiduf"></a>
#### SFTPClient常用方法举例
```python
import paramiko 
# 实例化一个transport对象
tran = paramiko.Transport(('192.168.137.100', 22)) 
# 连接SSH服务端，使用password
tran.connect(username="root", password='123456')
# 或使用
# 配置私人密钥文件位置
private = paramiko.RSAKey.from_private_key_file('/root/.ssh/id_rsa')
# 连接SSH服务端，使用pkey指定私钥
tran.connect(username="root", pkey=private) 
# 获取SFTP实例
sftp = paramiko.SFTPClient.from_transport(tran) 
# 设置上传的本地/远程文件路径
local_path = "/home/1.txt"
remote_path = "/tmp/1.txt" 
# 执行上传动作
sftp.put(local_path, remote_path)
# 执行下载动作
sftp.get(remote_path, local_path) 
# 关闭Transport通道
tran.close()
```
<a name="fhGgJ"></a>
### **完整代码示例**
<a name="lbY53"></a>
#### 操作单台主机
```python
# 单台主机操作
import paramiko
#############################配置信息#####################################
# 登陆参数设置
hostname = "192.168.137.100"  
host_port = 22
username = "root"
password = "123456"
########################################################################

def ssh_client_con():
    """创建ssh连接，并执行shell指令"""
    # 1 创建ssh_client实例
    ssh_client = paramiko.SSHClient()
    # 自动处理第一次连接的yes或者no的问题
    ssh_client.set_missing_host_key_policy(paramiko.AutoAddPolicy)

    # 2 连接服务器
    ssh_client.connect(
        port=host_port,
        hostname=hostname,
        username=username,
        password=password
    )

    # 3 执行shell命令
    # 构造shell指令
    shell_command = "ps aux"
    # 执行shell指令
    stdin, stdout, stderr = ssh_client.exec_command(shell_command)
    # 输出返回信息
    stdout_info = stdout.read().decode('utf8')
    print(stdout_info)

    # 输出返回的错误信息
    stderr_info = stderr.read().decode('utf8')
    print(stderr_info)


def sftp_client_con():
    # 1 创建transport通道
    tran = paramiko.Transport((hostname, host_port))
    tran.connect(username=username, password=password)
    # 2 创建sftp实例
    sftp = paramiko.SFTPClient.from_transport(tran)

    # 3 执行上传功能
    local_path = "米汤.jpg"          # 本地路径
    remote_path = "/home/333.jpg"    # 远程路径
    put_info = sftp.put(local_path, remote_path, confirm=True)
    print(put_info)
    print(f"上传{local_path}完成")
    # 4 执行下载功能
    save_path = "7.jpg"             # 本地保存文件路径
    sftp.get(remotepath=remote_path, localpath=save_path)
    print(f'下载{save_path}完成')

    # 5 关闭通道
    tran.close()


# 调用函数执行功能
ssh_client_con()
sftp_client_con()


# 本例是单个主机操作
```
<a name="iwc1f"></a>
#### 批量操作主机
```python
import paramiko

def ssh_client_con():
    """创建ssh连接，并执行shell指令"""
    # 1 创建ssh_client实例
    ssh_client = paramiko.SSHClient()
    # 自动处理第一次连接的yes或者no的问题
    ssh_client.set_missing_host_key_policy(paramiko.AutoAddPolicy)

    # 2 连接服务器
    ssh_client.connect(
        port=host_port,
        hostname=hostname,
        username=username,
        password=password
    )

    # 3 执行shell命令
    # 构造shell指令
    shell_command = """
    path='/tmp'
    tree ${path}
    # 如果tree命令执行失败，则安装tree后再执行
    if [ $? -ne 0 ]
    then
        yum install -y tree
        tree ${path}
    fi

    """
    # 执行shell指令
    stdin, stdout, stderr = ssh_client.exec_command(shell_command)
    # 输出返回信息
    stdout_info = stdout.read().decode('utf8')
    print(stdout_info)

    # 输出返回的错误信息
    stderr_info = stderr.read().decode('utf8')
    print(stderr_info)


def sftp_client_con():
    # 1 创建transport通道
    tran = paramiko.Transport((hostname, host_port))
    tran.connect(username=username, password=password)
    # 2 创建sftp实例
    sftp = paramiko.SFTPClient.from_transport(tran)

    # 3 执行上传功能
    local_path = "米汤.jpg"
    remote_path = "/home/333.jpg"
    put_info = sftp.put(local_path, remote_path, confirm=True)
    print(put_info)
    print(f"上传{local_path}完成")
    # 4 执行下载功能
    save_path = "7.jpg"
    sftp.get(remotepath=remote_path, localpath=save_path)
    print(f'下载{save_path}完成')

    # 5 关闭通道
    tran.close()


# 读取主机信息
try:
    with open('host_site.txt', 'r', encoding='utf8') as host_file:
        for host_info in host_file:
            line = host_info.strip('\n')
            hostname, host_port, username, password = line.split(',')
            print(f'---------------{hostname}执行结果-----------')
            ssh_client_con()   # 执行command并返回结果
            # 本例适合批量主机处理相同的command
except FileNotFoundError as e:
    print(e)


#  host_site.txt 文件内容格式如下
#  主机地址         端口 用户  密码
#  192.168.137.100,22,root,123456
#  192.168.137.101,22,root,123456
#  192.168.137.102,22,root,123456
#  192.168.137.103,22,root,123456
#  192.168.137.104,22,root,123456
```
