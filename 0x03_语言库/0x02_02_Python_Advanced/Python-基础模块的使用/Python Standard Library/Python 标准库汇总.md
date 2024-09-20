Python
<a name="Bc7q3"></a>
### 文本
```python
string：通用字符串操作
re：正则表达式操作
difflib：差异计算工具
textwrap：文本填充
unicodedata：Unicode字符数据库
stringprep：互联网字符串准备工具
readline：GNU按行读取接口
rlcompleter：GNU按行读取的实现函数
```
<a name="NwB35"></a>
### 二进制数据
```python
struct：将字节解析为打包的二进制数据
codecs：注册表与基类的编解码器
```
<a name="cQ5xT"></a>
### 数据类型
```python
datetime：基于日期与时间工具
calendar：通用月份函数
collections：容器数据类型
collections.abc：容器虚基类
heapq：堆队列算法
bisect：数组二分算法
array：高效数值数组
weakref：弱引用
types：内置类型的动态创建与命名
copy：浅拷贝与深拷贝
pprint：格式化输出
reprlib：交替repr()的实现
```
<a name="vXIjB"></a>
### 数学
```python
numbers：数值的虚基类
math：数学函数
cmath：复数的数学函数
decimal：定点数与浮点数计算
fractions：有理数
random：生成伪随机数
```
<a name="i36Xp"></a>
### 函数式编程
```python
itertools：为高效循环生成迭代器
functools：可调用对象上的高阶函数与操作
operator：针对函数的标准操作
```
<a name="KA08r"></a>
### 文件与目录
```python
os.path：通用路径名控制
fileinput：从多输入流中遍历行
stat：解释stat()的结果
filecmp：文件与目录的比较函数
tempfile：生成临时文件与目录
glob：Unix风格路径名格式的扩展
fnmatch：Unix风格路径名格式的比对
linecache：文本行的随机存储
shutil：高级文件操作
macpath：MacOS 9路径控制函数
```
<a name="c7QZT"></a>
### 持久化
```python
pickle：Python对象序列化
copyreg：注册机对pickle的支持函数
shelve：Python对象持久化
marshal：内部Python对象序列化
dbm：Unix“数据库”接口
sqlite3：针对SQLite数据库的API2.0
```
<a name="YWdLB"></a>
### 压缩
```python
zlib：兼容gzip的压缩
gzip：对gzip文件的支持
bz2：对bzip2压缩的支持
lzma：使用LZMA算法的压缩
zipfile：操作ZIP存档
tarfile：读写tar存档文件
```
<a name="w7EoN"></a>
### 文件格式化
```python
csv：读写CSV文件
configparser：配置文件解析器
netrc：netrc文件处理器
xdrlib：XDR数据编码与解码
plistlib：生成和解析Mac OS X.plist文件
```
<a name="CV395"></a>
### 加密
```python
hashlib：安全散列与消息摘要
hmac：针对消息认证的键散列
```
<a name="XApJc"></a>
### 操作系统工具
```python
os：多方面的操作系统接口
io：流核心工具
time：时间的查询与转化
argparser：命令行选项、参数和子命令的解析器
optparser：命令行选项解析器
getopt：C风格的命令行选项解析器
logging：Python日志工具
logging.config：日志配置
logging.handlers：日志处理器
getpass：简易密码输入
curses：字符显示的终端处理
curses.textpad：curses程序的文本输入域
curses.ascii：ASCII字符集工具
curses.panel：curses的控件栈扩展
platform：访问底层平台认证数据
errno：标准错误记号
ctypes：Python外部函数库
```
<a name="UUnEU"></a>
### 并发
```python
threading：基于线程的并行
multiprocessing：基于进程的并行
concurrent：并发包
concurrent.futures：启动并行任务
subprocess：子进程管理
sched：事件调度
queue：同步队列
select：等待I / O完成
dummy_threading：threading模块的替代（当_thread不可用时）
_thread：底层的线程API（threading基于其上）
_dummy_thread：_thread模块的替代（当_thread不可用时）
```
<a name="d0MTl"></a>
### 进程间通信
```python
socket：底层网络接口
ssl：socket对象的TLS / SSL填充器
asyncore：异步套接字处理器
asynchat：异步套接字命令 / 响应处理器
signal：异步事务信号处理器
mmap：内存映射文件支持
```
