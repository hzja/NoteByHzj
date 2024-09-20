Python 标准库
<a name="KOpaJ"></a>
## 文本
string：通用字符串操作<br />re：正则表达式操作<br />difflib：差异计算工具<br />textwrap：文本填充<br />unicodedata：Unicode字符数据库<br />stringprep：互联网字符串准备工具<br />readline：GNU按行读取接口<br />rlcompleter：GNU按行读取的实现函数<br />struct：将字节解析为打包的二进制数据<br />codecs：注册表与基类的编解码器
<a name="eiViA"></a>
## 数据类型
datetime：基于日期与时间工具<br />calendar：通用月份函数<br />collections：容器数据类型<br />collections.abc：容器虚基类<br />heapq：堆队列算法<br />bisect：数组二分算法<br />array：高效数值数组<br />weakref：弱引用<br />types：内置类型的动态创建与命名<br />copy：浅拷贝与深拷贝<br />reprlib：交替repr()的实现
<a name="b5gwW"></a>
## 数学
numbers：数值的虚基类<br />math：数学函数<br />cmath：复数的数学函数<br />decimal：定点数与浮点数计算<br />fractions：有理数<br />random：生成伪随机数
<a name="9uTFN"></a>
## 函数式编程
itertools：为高效循环生成迭代器<br />functools：可调用对象上的高阶函数与操作<br />operator：针对函数的标准操作
<a name="08Gx2"></a>
## 文件与目录
os.path：通用路径名控制<br />fileinput：从多输入流中遍历行<br />stat：解释stat()的结果<br />filecmp：文件与目录的比较函数<br />tempfile：生成临时文件与目录<br />glob：Unix风格路径名格式的扩展<br />fnmatch：Unix风格路径名格式的比对<br />linecache：文本行的随机存储<br />shutil：高级文件操作<br />macpath：MacOS 9路径控制函数
<a name="l994u"></a>
## 持久化
pickle：Python对象序列化<br />copyreg：注册机对pickle的支持函数<br />shelve：Python对象持久化<br />marshal：内部Python对象序列化<br />dbm：Unix“数据库”接口<br />sqlite3：针对SQLite数据库的API2.0
<a name="EJWPy"></a>
## 压缩
zlib：兼容gzip的压缩<br />gzip：对gzip文件的支持<br />bz2：对bzip2压缩的支持<br />lzma：使用LZMA算法的压缩<br />zipfile：操作ZIP存档<br />tarfile：读写tar存档文件
<a name="aoLi6"></a>
## 加密
hashlib：安全散列与消息摘要<br />hmac：针对消息认证的键散列
<a name="b2NDZ"></a>
## 操作系统工具
os：多方面的操作系统接口<br />io：流核心工具<br />time：时间的查询与转化<br />argparser：命令行选项、参数和子命令的解析器<br />optparser：命令行选项解析器<br />getopt：C风格的命令行选项解析器<br />logging：Python日志工具<br />logging.config：日志配置<br />logging.handlers：日志处理器<br />getpass：简易密码输入<br />curses：字符显示的终端处理<br />curses.textpad：curses程序的文本输入域<br />curses.ascii：ASCII字符集工具<br />curses.panel：curses的控件栈扩展<br />platform：访问底层平台认证数据<br />errno：标准错误记号<br />ctypes：Python外部函数库
<a name="6mIzx"></a>
## 并发
threading：基于线程的并行<br />multiprocessing：基于进程的并行<br />concurrent：并发包<br />concurrent.futures：启动并行任务<br />subprocess：子进程管理<br />sched：事件调度<br />queue：同步队列<br />select：等待I / O完成<br />dummy_threading：threading模块的替代（当_thread不可用时）<br />_thread：底层的线程API（threading基于其上）<br />_dummy_thread：_thread模块的替代（当_thread不可用时）
<a name="f4Wl4"></a>
## 进程间通信
socket：底层网络接口<br />ssl：socket对象的TLS / SSL填充器<br />asyncore：异步套接字处理器<br />asynchat：异步套接字命令 / 响应处理器<br />signal：异步事务信号处理器<br />mmap：内存映射文件支持
<a name="KliXI"></a>
## 互联网
email：邮件与MIME处理包<br />json：JSON编码与解码<br />mailcap：mailcap文件处理<br />mailbox：多种格式控制邮箱<br />mimetypes：文件名与MIME类型映射<br />base64：RFC3548：Base16、Base32、Base64编码<br />binhex：binhex4文件编码与解码<br />binascii：二进制码与ASCII码间的转化<br />quopri：MIMEquoted - printable数据的编码与解码<br />uu：uuencode文件的编码与解码
<a name="jGJ32"></a>
## 互联网协议与支持
webbrowser：简易Web浏览器控制器<br />cgi：CGI支持<br />cgitb：CGI脚本反向追踪管理器<br />wsgiref：WSGI工具与引用实现<br />urllib：URL处理模块<br />urllib.request：打开URL连接的扩展库<br />urllib.response：urllib模块的响应类<br />urllib.parse：将URL解析成组件<br />urllib.error：urllib.request引发的异常类<br />urllib.robotparser：robots.txt的解析器<br />http：HTTP模块<br />http.client：HTTP协议客户端<br />ftplib：FTP协议客户端<br />poplib：POP协议客户端<br />imaplib：IMAP4协议客户端<br />nntplib：NNTP协议客户端<br />smtplib：SMTP协议客户端<br />smtpd：SMTP服务器<br />telnetlib：Telnet客户端<br />uuid：RFC4122的UUID对象<br />socketserver：网络服务器框架<br />http.server：HTTP服务器<br />http.cookies：HTTPCookie状态管理器<br />http.cookiejar：HTTP客户端的Cookie处理<br />xmlrpc：XML - RPC服务器和客户端模块<br />xmlrpc.client：XML - RPC客户端访问<br />xmlrpc.server：XML - RPC服务器基础<br />ipaddress：IPv4 / IPv6控制库
<a name="H63UX"></a>
## 多媒体
audioop：处理原始音频数据<br />aifc：读写AIFF和AIFC文件<br />sunau：读写Sun AU文件<br />wave：读写WAV文件<br />chunk：读取IFF大文件<br />colorsys：颜色系统间转化<br />imghdr：指定图像类型<br />sndhdr：指定声音文件类型<br />ossaudiodev：访问兼容OSS的音频设备
<a name="9fpOY"></a>
## 国际化
gettext：多语言的国际化服务<br />locale：国际化服务
<a name="HavTj"></a>
## 编程框架
turtle：Turtle图形库<br />cmd：基于行的命令解释器支持<br />shlex：简单词典分析
<a name="DMf3u"></a>
## Tk图形用户接口
tkinter：Tcl / Tk接口<br />tkinter.ttk：Tk主题控件<br />tkinter.tix：Tk扩展控件<br />tkinter.scrolledtext：滚轴文本控件
<a name="mc9VR"></a>
## 开发工具
pydoc：文档生成器和在线帮助系统<br />doctest：交互式Python示例<br />unittest：单元测试框架<br />unittest.mock：模拟对象库<br />test：Python回归测试包<br />test.support：Python测试工具套件<br />venv：虚拟环境搭建
<a name="ZfN7q"></a>
## 调试
bdb：调试框架<br />faulthandler：Python反向追踪库<br />pdb：Python调试器<br />timeit：小段代码执行时间测算<br />trace：Python执行状态追踪
<a name="BKBAN"></a>
## 运行时
sys：系统相关的参数与函数<br />sysconfig：访问Python配置信息<br />builtins：内置对象<br />main：顶层脚本环境<br />warnings：警告控制<br />contextlib：with状态的上下文工具<br />abc：虚基类<br />atexit：出口处理器<br />traceback：打印或读取一条栈的反向追踪<br />future：未来状态定义<br />gc：垃圾回收接口<br />inspect：检查存活的对象<br />site：址相关的配置钩子（hook）<br />fpectl：浮点数异常控制<br />distutils：生成和安装Python模块
<a name="svyly"></a>
## 解释器
code：基类解释器<br />codeop：编译Python代码
<a name="K7Poj"></a>
## 导入模块
imp：访问import模块的内部<br />zipimport：从ZIP归档中导入模块<br />pkgutil：包扩展工具<br />modulefinder：通过脚本查找模块<br />runpy：定位并执行Python模块<br />importlib：import的一种实施
<a name="zhBzx"></a>
## Python语言
parser：访问Python解析树<br />ast：抽象句法树<br />symtable：访问编译器符号表<br />symbol：Python解析树中的常量<br />token：Python解析树中的常量<br />keyword：Python关键字测试<br />tokenize：Python源文件分词<br />tabnany：模糊缩进检测<br />pyclbr：Python类浏览支持<br />py_compile：编译Python源文件<br />compileall：按字节编译Python库<br />dis：Python字节码的反汇编器<br />pickletools：序列化开发工具
<a name="cY8QE"></a>
## 其他
formatter：通用格式化输出
<a name="3R3TI"></a>
## Windows相关
msilib：读写Windows的Installer文件<br />msvcrt：MS VC + + Runtime的有用程序<br />winreg：Windows注册表访问<br />winsound：Windows声音播放接口
<a name="Nn41R"></a>
## Unix相关
posix：最常用的POSIX调用<br />pwd：密码数据库<br />spwd：影子密码数据库<br />grp：组数据库<br />crypt：Unix密码验证<br />termios：POSIX风格的tty控制<br />tty：终端控制函数<br />pty：伪终端工具<br />fcntl：系统调用fcntl()和ioctl()<br />pipes：shell管道接口<br />resource：资源可用信息<br />nis：Sun的NIS的接口<br />syslog：Unix 日志服务
