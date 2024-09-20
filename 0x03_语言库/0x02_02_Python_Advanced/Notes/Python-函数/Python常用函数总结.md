Python 常用函数
<a name="HZSwj"></a>
## 1、常用内置函数：(不用`import`就可以直接使用)
```python
help(obj) 在线帮助, obj可是任何类型
callable(obj) 查看一个obj是不是可以像函数一样调用
repr(obj) 得到obj的表示字符串,可以利用这个字符串eval重建该对象的一个拷贝
eval_r(str) 表示合法的python表达式,返回这个表达式
dir(obj) 查看obj的name space中可见的name
hasattr(obj,name) 查看一个obj的name space中是否有name
getattr(obj,name) 得到一个obj的name space中的一个name
setattr(obj,name,value) 为一个obj的name space中的一个name指向vale这个object
delattr(obj,name) 从obj的name space中删除一个name
vars(obj) 返回一个object的name space。用dictionary表示
locals() 返回一个局部name space,用dictionary表示
globals() 返回一个全局name space,用dictionary表示
type(obj) 查看一个obj的类型
isinstance(obj,cls) 查看obj是不是cls的instance
issubclass(subcls,supcls) 查看subcls是不是supcls的子类
Process finished with exit code 0
```
<a name="A8KGo"></a>
### 类型转换函数
```python
chr(i) 把一个ASCII数值,变成字符
ord(i) 把一个字符或者unicode字符,变成ASCII数值
oct(x) 把整数x变成八进制表示的字符串
hex(x) 把整数x变成十六进制表示的字符串
str(obj) 得到obj的字符串描述
list(seq) 把一个sequence转换成一个list
tuple(seq) 把一个sequence转换成一个tuple
dict(),dict(list) 转换成一个dictionary
int(x) 转换成一个integer
long(x) 转换成一个long interger
float(x) 转换成一个浮点数
complex(x) 转换成复数
max(...) 求最大值
min(...) 求最小值
```
<a name="HdaNu"></a>
### 用于执行程序的内置函数
```python
complie 如果一段代码经常要使用,那么先编译,再运行会更快。
```
<a name="qhIO5"></a>
## 2、和操作系统相关的调用
<a name="q0TKg"></a>
### 系统相关的信息模块 `import sys`
```python
sys.argv是一个list,包含所有的命令行参数.
sys.stdout sys.stdin sys.stderr 分别表示标准输入输出,错误输出的文件对象.
sys.stdin.readline() 从标准输入读一行 sys.stdout.write("a") 屏幕输出a
sys.exit(exit_code) 退出程序
sys.modules 是一个dictionary,表示系统中所有可用的module
sys.platform 得到运行的操作系统环境
sys.path 是一个list,指明所有查找module,package的路径.
```
<a name="Hnzp3"></a>
### 操作系统相关的调用和操作 `import os`
```python
os.environ 一个dictionary 包含环境变量的映射关系 os.environ["HOME"] 可以得到环境变量HOME的值
os.chdir(dir) 改变当前目录 os.chdir('d:\outlook') 注意windows下用到转义
os.getcwd() 得到当前目录
os.getegid() 得到有效组id os.getgid() 得到组id
os.getuid() 得到用户id os.geteuid() 得到有效用户id
os.setegid os.setegid() os.seteuid() os.setuid()
os.getgruops() 得到用户组名称列表
os.getlogin() 得到用户登录名称
os.getenv 得到环境变量
os.putenv 设置环境变量
os.umask 设置umask
os.system(cmd) 利用系统调用,运行cmd命令
```
操作举例：
```python
os.mkdir('/tmp/xx') os.system("echo 'hello' > /tmp/xx/a.txt") os.listdir('/tmp/xx')
os.rename('/tmp/xx/a.txt','/tmp/xx/b.txt') os.remove('/tmp/xx/b.txt') os.rmdir('/tmp/xx')
```
<a name="iXa7h"></a>
### 用python编写一个简单的shell
```python
#!/usr/bin/python
import os, sys
cmd = sys.stdin.readline()
while cmd:
    os.system(cmd)
    cmd = sys.stdin.readline()
```
<a name="mjjRy"></a>
### 用`os.path`编写平台无关的程序
```python
os.path.abspath("1.txt") == os.path.join(os.getcwd(), "1.txt")
os.path.split(os.getcwd()) 用于分开一个目录名称中的目录部分和文件名称部分。
os.path.join(os.getcwd(), os.pardir, 'a', 'a.doc') 全成路径名称.
os.pardir 表示当前平台下上一级目录的字符 ..
os.path.getctime("/root/1.txt") 返回1.txt的ctime(创建时间)时间戳
os.path.exists(os.getcwd()) 判断文件是否存在
os.path.expanduser('~/dir') 把~扩展成用户根目录
os.path.expandvars('$PATH') 扩展环境变量PATH
os.path.isfile(os.getcwd()) 判断是否是文件名,1是0否
os.path.isdir('c:Python26emp') 判断是否是目录,1是0否
os.path.islink('/home/huaying/111.sql') 是否是符号连接 windows下不可用
os.path.ismout(os.getcwd()) 是否是文件系统安装点 windows下不可用
os.path.samefile(os.getcwd(), '/home/huaying') 看看两个文件名是不是指的是同一个文件
os.path.walk('/home/huaying', test_fun, "a.c")
```
遍历/home/huaying下所有子目录包括本目录,对于每个目录都会调用函数test_fun。<br />例：在某个目录中,和他所有的子目录中查找名称是a.c的文件或目录。
```python
def test_fun(filename, dirname, names): //filename即是walk中的a.c dirname是访问的目录名称
    if filename in names: //names是一个list,包含dirname目录下的所有内容
        print os.path.join(dirname, filename)
        os.path.walk('/home/huaying', test_fun, "a.c")
```
<a name="M0vW5"></a>
#### 打开文件
```python
f = open("filename", "r") r只读 w写 rw读写 rb读二进制 wb写二进制 w+写追加
```
<a name="FYE0v"></a>
#### 读写文件
```python
f.write("a") f.write(str) 写一字符串 f.writeline() f.readlines() 与下read类同
f.read() 全读出来 f.read(size) 表示从文件中读取size个字符
f.readline() 读一行,到文件结尾,返回空串. f.readlines() 读取全部,返回一个list. list每个元素表示一行，包含" "
f.tell() 返回当前文件读取位置
f.seek(off, where) 定位文件读写位置. off表示偏移量,正数向文件尾移动,负数表示向开头移动。where为0表示从开始算起,1表示从当前位置算,2表示从结尾算.
f.flush() 刷新缓存
```
<a name="R38bx"></a>
#### 关闭文件
```python
f.close()
```
<a name="bmyTW"></a>
### regular expression 正则表达式 `import re`
简单的regexp
```python
p = re.compile("abc") if p.match("abc") : print "match"
```
上例中首先生成一个pattern(模式),如果和某个字符串匹配,就返回一个match object<br />除某些特殊字符metacharacter元字符,大多数字符都和自身匹配。<br />这些特殊字符是: `^` `$` `*` `+` `?` `{` `[` `]` `|` `(` `)`<br />字符集合(用`[]`表示)<br />列出字符,如`[abc]`表示匹配a或b或c,大多数metacharacter在`[]`中只表示和本身匹配。例：
```python
a = ".^$*+?{\|()" 大多数metachar在[]中都和本身匹配,但"^[]"不同
p = re.compile("["+a+"]")
for i in a:
    if p.match(i):
    	print "[%s] is match" %i
    else:
    	print "[%s] is not match" %i
```
在`[]`中包含`[]`本身,表示"`[`"或者"`]`"匹配.用`[`和`]`表示。<br />`^`出现在`[]`的开头,表示取反。`[^abc]`表示除了a,b,c之外的所有字符。`^`没有出现在开头,即于身身匹配。<br />`-`可表示范围.`[a-zA-Z]`匹配任何一个英文字母。`[0-9]`匹配任何数字。<br />在`[]`中的妙用。
```python
d [0-9]
D [^0-9]
s [
]
S [^
]
w [a-zA-Z0-9_]
W [^a-zA-Z0-9_]
```
表示和tab匹配, 其他的都和字符串的表示法一致<br />表示和十六进制ascii 0x20匹配<br />有了,可以在`[]`中表示任何字符。注：单独的一个"`.`"如果没有出现`[]`中，表示出了换行 以外的匹配任何字符,类似`[^ ]`.
<a name="o7Wft"></a>
#### regexp的重复
`{m,n}`表示出现`m`个以上(含`m`个),`n`个以下(含`n`个)。 如`ab{1,3}c`和`abc`,`abbc`,`abbbc`匹配，不会与`ac`,`abbbc`匹配。<br />`m`是下界,`n`是上界。`m`省略表下界是`0`,`n`省略,表上界无限大。<br />`*`表示`{,} +`表示`{1,} ?`表示`{0,1}`<br />最大匹配和最小匹配 python都是最大匹配,如果要最小匹配，在*,+,?,{m,n}后面加一个?。<br />match object的`end`可以得到匹配的最后一个字符的位置。
```python
re.compile("a*").match('aaaa').end() 4 最大匹配
re.compile("a*?").match('aaaa').end() 0 最小匹配
```
<a name="Ev8NX"></a>
#### 使用原始字符串
字符串表示方法中用`\`表示字符.大量使用影响可读性。<br />解决方法：在字符串前面加一个r表示raw格式。
```python
a = r"a" print a 结果是a
a = r""a" print a 结果是"a
```
<a name="mBdpI"></a>
#### 使用re模块
先用`re.compile`得到一个RegexObject 表示一个`regexp`<br />后用pattern的`match`,`search`的方法,得到`MatchObject`<br />再用match object得到匹配的位置,匹配的字符串等信息<br />`RegxObject`常用函数:
```python
>>> re.compile("a").match("abab") 如果abab的开头和re.compile("a")匹配,得到MatchObject
<_sre.SRE_Match object at 0x81d43c8>
>>> print re.compile("a").match("bbab")
```
None 注：从str的开头开始匹配
```python
>>> re.compile("a").search("abab") 在abab中搜索第一个和re_obj匹配的部分
<_sre.SRE_Match object at 0x81d43c8>
>>> print re.compile("a").search("bbab")
<_sre.SRE_Match object at 0x8184e18> 和match()不同,不必从开头匹配
```
`re_obj.findall(str)` 返回str中搜索所有和re_obj匹配的部分。<br />返回一个tuple,其中元素是匹配的字符串。<br />MatchObject的常用函数<br />`m.start()` 返回起始位置,`m.end()`返回结束位置(不包含该位置的字符)。<br />`m.span()` 返回一个tuple表示(`m.start()`, `m.end()`)<br />`m.pos()`, `m.endpos()`, `m.re()`, `m.string()`<br />`m.re().search(m.string(), m.pos(), m.endpos())` 会得到m本身<br />`m.finditer()`可以返回一个`iterator`,用来遍历所有找到的MatchObject。

```python
for m in re.compile("[ab]").finditer("tatbxaxb"):
    print m.span()
```
高级regexp<br />`|` 表示联合多个regexp, A B两个regexp,A|B表示和A匹配或者跟B匹配。<br />`^` 表示只匹配一行的开始行首,^只有在开头才有此特殊意义。<br />`$` 表示只匹配一行的结尾<br />`A` 表示只匹配第一行字符串的开头 ^匹配每一行的行首<br />`Z` 表示只匹配行一行字符串的结尾 $匹配第一行的行尾<br />只匹配词的边界 例：info 只会匹配"info" 不会匹配information<br />`B` 表示匹配非单词边界<br />示例如下：
```python
>>> print re.compile(r"info").match("info ") #使用raw格式 表示单词边界
<_sre.SRE_Match object at 0x817aa98>
>>> print re.compile("info").match("info ") #没有使用raw 表示退格符号
None
>>> print re.compile("info").match("info ")
<_sre.SRE_Match object at 0x8174948>
```
<br />分组(Group) 示例：`re.compile("(a(b)c)d").match("abcd").groups() ('abc', 'b')`
```python
#!/usr/local/bin/python
import re
x = """
name: Charles
Address: BUPT
name: Ann
Address: BUPT
"""
#p = re.compile(r"^name:(.*) ^Address:(.*) ", re.M)
p = re.compile(r"^name:(?P<name>.*) ^Address:(?P<address>.*) ", re.M)
for m in p.finditer(x):
    print m.span()
    print "here is your friends list"
    print "%s, %s"%m.groups()
```
<a name="zgbH7"></a>
#### Compile Flag
用`re.compile`得到RegxObject时,可以有一些flag用来调整RegxObject的详细特征。<br />`DOTALL`, S 让`.`匹配任意字符,包括换行符<br />`IGNORECASE`, I 忽略大小写<br />`LOCALES`, L 让w W B和当前的locale一致<br />`MULTILINE`, M 多行模式,只影响`^`和`$`(参见上例)<br />`VERBOSE`, X verbose模式
