Python
<a name="hTd0m"></a>
## 用Python进行远程登陆服务器
通过使用Paramiko和SCP Python库自动化远程服务器任务。使用Python来SSH到主机，执行任务，传输文件等。<br />paramiko和scp是两个Python库，可以一起使用它们来自动化想要在远程主机上运行的任务，比如重新启动服务、进行更新或获取日志文件。
<a name="p9K3d"></a>
### 设置SSH密钥
要验证SSH连接，需要设置一个私有的RSA SSH密钥(不要与OpenSSH混淆)。可以使用以下命令生成密钥：
```bash
$ ssh-keygen -t rsa
```
这将提示为密钥提供一个名称。随便怎么设置：
```bash
Generating a public/private rsa key pair. Enter the file in which you wish to save they key (i.e., /home/username/.ssh/id_rsa):
```
接下来，系统将提示提供一个密码(不必填写)。<br />现在有了密钥，需要将其复制到远程主机。最简单的方法是使用ssh-copy-id：
```bash
$ ssh-copy-id -i ~/.ssh/mykey username@my_remote_host.org
```
<a name="niKK4"></a>
### 验证SSH密钥
如果想检查已经有哪些密钥，这些可以在系统的.ssh目录中找到：
```bash
$ cd ~/.ssh
```
寻找以以下头文件开头的键：
```
-----BEGIN RSA PRIVATE KEY-----...-----END RSA PRIVATE KEY-----
```
<a name="o5XlD"></a>
### 构造脚本
安装库。
```bash
$ pip3 install paramiko scp
```
在编写一些有意义的Python代码之前，还有一件事要做!创建一个配置文件来保存连接到主机所需的变量。下面是进入服务器所需要的基本内容：

- Host：试图访问的远程主机的IP地址或URL。
- Username：这是用于SSH到服务器的用户名。
- Passphrase(可选)：如果在创建ssh密钥时指定了一个Passphrase，请在这里指定。请记住，SSH密钥密码短语与用户密码不同。
- SSH Key：前面创建的密钥的文件路径。在OSX上，它们存在于系统的~/.ssh文件夹。目标的SSH密钥必须有一个附带的密钥，文件扩展名为.pub。这是公钥;如果遵循前面的步骤，那么应该生成了这个文件。

如果试图从远程主机上传或下载文件，需要包含两个额外的变量：

- Remote Path：文件传输目标的远程目录的路径。可以上传东西到这个文件夹或者下载它的内容。
- Local Path：与上述想法相同，但相反。为了方便起见，将使用的本地路径是简单的/data，并包含可爱的狐狸gif的图片。

现在有了创建一个config.py文件所需的一切：
```python

"""Remote host configuration."""
from os import environ, path
from dotenv import load_dotenv
# Load environment variables from .env
basedir = path.abspath(path.dirname(__file__))
load_dotenv(path.join(basedir, '.env'))
# Read environment variables
host = environ.get('REMOTE_HOST')
user = environ.get('REMOTE_USERNAME')
ssh_key_filepath = environ.get('SSH_KEY')
remote_path = environ.get('REMOTE_PATH')
local_file_directory = 'data'
```
<a name="RT5bc"></a>
### 新建SSH客户端
创建一个名为`RemoteClient`的类来处理与远程主机的交互。先用config.py中创建的变量实例化RemoteClient类：
```python
"""Client to handle connections and actions executed against a remote host."""
class RemoteClient:
    """Client to interact with a remote host via SSH & SCP."""
    def __init__(self, host, user, ssh_key_filepath, remote_path):
        self.host = host
        self.user = user
        self.ssh_key_filepath = ssh_key_filepath
        self.remote_path = remote_path
```
到目前为止只是设置了一些变量，并将它们传递到一个无用的类中。在不离开构造函数的情况下进一步讨论：
```python
"""Client to handle connections and actions executed against a remote host."""
from paramiko import SSHClient, AutoAddPolicy, RSAKey
from paramiko.auth_handler import AuthenticationException, SSHException
class RemoteClient:
    """Client to interact with a remote host via SSH & SCP."""
    def __init__(self, host, user, ssh_key_filepath, remote_path):
        self.host = host
        self.user = user
        self.ssh_key_filepath = ssh_key_filepath
        self.remote_path = remote_path
        self.client = None
        self.scp = None
        self.conn = None
        self._upload_ssh_key()
```
已经添加了三个新东西来实例化类：<br />`self.client = None: self.Client`最终将在类中充当连接对象，类似于处理数据库库中的`conn`等术语。在显式连接到远程主机之前，连接将为`None`。<br />`self.scp = None`与`self.client`相同，但专门处理传输文件的连接。<br />`Self._upload_ssh_key()`不是一个变量，而是一个在客户机实例化时自动运行的函数。调用`_upload_ssh_key()`是`RemoteClient`对象在创建时立即检查本地ssh密钥，以便可以尝试将它们传递到远程主机。否则，根本无法建立联系。
<a name="YjvCC"></a>
### 上传SSH密钥到远程主机
`RemoteClient`将从两个私有方法开始：`_get_ssh_key()`和`_upload_ssh_key()`。前者将获取本地存储的公钥，如果成功，后者将把这个公钥传递给远程主机，作为访问的橄榄枝。一旦本地创建的公钥存在于远程机器上，该机器将永远信任连接请求：不需要密码。将在此过程中包括适当的日志记录，以防遇到任何麻烦：
```python
"""Client to handle connections and actions executed against a remote host."""
from os import system
from paramiko import SSHClient, AutoAddPolicy, RSAKey
from paramiko.auth_handler import AuthenticationException, SSHException
from scp import SCPClient, SCPException
from .log import logger
class RemoteClient:
    """Client to interact with a remote host via SSH & SCP."""
    ...
    def _get_ssh_key(self):
        """
        Fetch locally stored SSH key.
        """
        try:
            self.ssh_key = RSAKey.from_private_key_file(self.ssh_key_filepath)
            logger.info(f'Found SSH key at self {self.ssh_key_filepath}')
        except SSHException as error:
            logger.error(error)
        return self.ssh_key
    def _upload_ssh_key(self):
        try:
            system(f'ssh-copy-id -i {self.ssh_key_filepath} {self.user}@{self.host}>/dev/null 2>&1')
            system(f'ssh-copy-id -i {self.ssh_key_filepath}.pub {self.user}@{self.host}>/dev/null 2>&1')
            logger.info(f'{self.ssh_key_filepath} uploaded to {self.host}')
        except FileNotFoundError as error:
            logger.error(error)
```
`_get_ssh_key()`非常简单：它验证SSH密钥是否存在于在配置中指定的用于连接到主机的路径上。如果该文件确实存在，设置`self.ssh_key`变量，这样客户端就可以上传和使用这个密钥了。Paramiko为提供了一个名为RSAKey的子模块，可以轻松处理所有与RSA密钥相关的事情，比如将一个私钥文件解析为一个可用的连接身份验证。这就是得到的：
```python
RSAKey.from_private_key_file(self.ssh_key_filepath)
```
如果RSA密钥是不可理解的废话，而不是真正的密钥，Paramiko的SSHException会捕捉到这一点，并在解释这一点之前就引发一个异常。正确地利用库的错误处理需要对“哪里出了问题”进行大量猜测，特别是在某些情况下，比如在一个都不会经常搞混的小空间中，可能存在许多未知的情况。
<a name="geE8P"></a>
### 连接到客户端
在客户机中添加一个名为`connect()`的方法来处理到主机的连接：
```python
...
class RemoteClient:
    """Client to interact with a remote host via SSH & SCP."""
    ...
    def _connect(self):
        """Open connection to remote host."""
        if self.conn is None:
            try:
                self.client = SSHClient()
                self.client.load_system_host_keys()
                self.client.set_missing_host_key_policy(
                    AutoAddPolicy()
                )
                self.client.connect(
                    self.host,
                    username=self.user,
                    key_filename=self.ssh_key_filepath,
                    look_for_keys=True,
                    timeout=5000
                )
                self.scp = SCPClient(self.client.get_transport())
            except AuthenticationException as error:
                logger.error(f'Authentication failed: \
                    did you remember to create an SSH key? {error}')
                raise error
        return self.client
```
来分析一下：

- `SSHClient()`为创建代表SSH客户机的对象奠定了基础。以下几行将配置此对象，使其更有用。
- `load_system_host_keys()`指示客户机查找过去连接过的所有主机，方法是查看系统的known_hosts文件并找到主机所期望的SSH密钥。过去从未连接到主机，所以需要显式地指定SSH密钥。
- `set_missing_host_key_policy()`告诉Paramiko在出现未知密钥对时该怎么做。这需要Paramiko内置一个“策略”，将具体到`AutoAddPolicy()`。将策略设置为“自动添加”意味着如果试图连接到一个无法识别的主机，Paramiko将自动在本地添加丢失的密钥。
- `connect()`是SSHClient最重要的方法。终于能够传递主机、用户和SSH密钥来实现一直在等待的东西：到服务器的一个漂亮的SSH连接!`connect()`方法也通过大量可选关键字参数数组提供了极大的灵活性。在这里传递了一些：将`look_for_keys`设置为`True`将允许Paramiko在~/中查看。ssh文件夹发现自己的ssh密钥，设置超时将自动关闭可能忘记关闭的连接。如果选择以这种方式连接到主机，甚至可以传递端口和密码等变量。
<a name="MVQtM"></a>
### 断开连接
在使用完远程主机后，应该关闭与远程主机的连接。不这样做不一定是灾难性的，但是遇到过一些实例，其中足够的挂起连接最终会使端口22的入站流量达到最大。不管用例是否认为重启是一场灾难或轻微的不便，提倡设置一个超时变量(如前所述)。无论如何。
```python
class RemoteClient:
    ...
    def disconnect(self):
        """Close ssh connection."""
        if self.client:
            self.client.close()
        if self.scp:
            self.scp.close()
```
有趣的事实：设置`self.client.close()`实际上设置`self`。将`client`设置为等于`None`，这在检查连接是否已经打开的情况下非常有用。
<a name="PTsLS"></a>
### 执行Unix命令
现在有了一个很棒的Python类，它可以找到RSA密钥、连接和断开连接。它确实缺乏做任何有用的事情的能力。<br />可以修复这个问题，并最终开始使用一个全新的方法来执行命令，将适当地将其命名为`execute_commands()`(正确地说，“命令”可能不止一个，稍后将讨论这个问题)。所有这些工作都是由Paramiko客户端内置的`exec_command()`方法完成的，它接受一个字符串作为命令并执行它：
```python
class RemoteClient:
    ...
    def execute_commands(self, commands):
        """
        Execute multiple commands in succession.
        :param commands: List of unix commands as strings.
        :type commands: List[str]
        """
        self.conn = self._connect()
        for cmd in commands:
            stdin, stdout, stderr = self.client.exec_command(cmd)
            stdout.channel.recv_exit_status()
            response = stdout.readlines()
            for line in response:
                logger.info(f'INPUT: {cmd} | OUTPUT: {line}')
```
刚刚创建的函数`execute_commands()`期望一个字符串列表作为命令执行。这部分是为了方便，但也因为Paramiko不会在命令之间运行任何“状态”更改(比如更改目录)，所以传递给Paramiko的每个命令都应该假定是在服务器的根目录下工作的。冒昧地说出了这样三条命令：
```python
remote.execute_commands(['cd /var/www/ && ls',
                        'tail /var/log/nginx/access.log',
                        'ps aux | grep node'])
```
可以通过将`cd path/`链接到`/dir && ls`来查看一个目录的内容，但是运行`cd path/to/dir`后跟着ls会导致空无，因为ls第二次返回服务器根目录下的文件列表。
<a name="QiPuS"></a>
### 通过SCP上传(下载)文件
SCP既指用于将文件复制到远程计算机的协议(安全复制协议)，也指利用此协议的Python库。已经安装了SCP库，所以请导入它。<br />SCP和Paramiko库相互补充，使得通过SCP上传非常容易。`SCPClient()`创建一个期望Paramiko进行“传输”的对象，通过`self.conn.get_transport()`提供了这个对象。从语法上讲，创建SCP连接依赖于SSH客户机，但这些连接是独立的。关闭SSH连接而保持SCP连接打开是可能的，所以不要这样做。像这样打开一个SCP连接：
```python
self.scp = SCPClient(self.client.get_transport())
```
来上传整个目录的文件。`Bulk_upload()`接受文件路径列表，然后调用`_upload_single_file()`
```python
class RemoteClient:
    ...

    def bulk_upload(self, files):
        """
        Upload multiple files to a remote directory.

        :param files: List of paths to local files.
        :type files: List[str]
        """
        self.conn = self._connect()
        uploads = [self._upload_single_file(file) for file in files]
        logger.info(f'Finished uploading {len(uploads)} files to {self.remote_path} on {self.host}')

    def _upload_single_file(self, file):
        """Upload a single file to a remote directory."""
        upload = None
        try:
            self.scp.put(
                file,
                recursive=True,
                remote_path=self.remote_path
            )
            upload = file
        except SCPException as error:
            logger.error(error)
            raise error
        finally:
            logger.info(f'Uploaded {file} to {self.remote_path}')
            return upload
```
该方法期望接收两个字符串：第一个是文件的本地路径，第二个是想要上传的远程目录的路径。<br />SCP的`put()`方法将把一个本地文件上传到远程主机。如果现有的文件恰好存在于指定的目标上，这将用相同的名称替换它们。这就是所有需要的！
<a name="rjDyD"></a>
#### 下载文件
与SCP的`put()`对应的是`get()`方法：
```python
class RemoteClient:
    ...
    def download_file(self, file):
        """Download file from remote host."""
        self.conn = self._connect()
        self.scp.get(file)
```
