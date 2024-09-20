Python sys模块
<a name="hSAJf"></a>
### `sys.stdin` 接收用户的输入，默认是控制台
`sys.stdin` 接收用户的输入，就是读取键盘里输入的数据，默认是控制台。`input`方法就是读取 `sys.stdin` 里的数据。
```python
import sys
s_in = sys.stdin
while True:
    content = s_in.readline().rstrip('\n')
    if content == '':
        break
    print(content)
```
<a name="J6rwQ"></a>
### `sys.stdout` 标准输出，默认是控制台
`sys.stdout` 标准输出，默认是控制台
```python
import sys
m = open('stdout.txt', 'w', encoding='utf8')
sys.stdout = m
print('hello')
print('yes')
print('good')
m.close()
```
运行结果：生成一个stdout.txt文件，文件内容如下：
```
helloyesgood
```
<a name="YpLcv"></a>
### `sys.stderr` 错误输出，默认是控制台
`sys.stderr` 错误输出，默认是控制台
```python
import sys
x = open('stderr.txt', 'w', encoding='utf8')
sys.stderr = x
print(1 / 0)
x.close()
```
运行结果：生成一个stderr.txt文件，文件内容如下：
```python
Traceback (most recent call last):
  File "E:/python基础/demo.py", line 4, in <module>
    print(1 / 0)
ZeroDivisionError: division by zero
```
<a name="MrOLZ"></a>
### sys模块常用方法
sys模块包括了一组非常实用的服务，内含很多函数方法和变量，用来处理Python运行时配置以及资源，从而可以与前当程序之外的系统环境交互。
```python
#命令行参数List，第一个元素是程序本身路径
sys.argv  

#返回系统导入的模块字段，key是模块名，value是模块
sys.modules 

#退出程序，正常退出时exit(0)
sys.exit(n)    

#返回模块的搜索路径，初始化时使用PYTHONPATH环境变量的值
sys.path    

#标准输出
sys.stdout.write('output') 

 #标准输入
sys.stdin.readline()[:-1]

#返回所有已经导入的模块名
sys.modules.keys() 

#返回所有已经导入的模块
sys.modules.values() 

 #获取当前正在处理的异常类,exc_type、exc_value、exc_traceback当前处理的异常详细信息
sys.exc_info() 


#获取Python解释程序的版本值，16进制格式如：0x020403F0
sys.hexversion    

#获取Python解释程序的版本
sys.version     

 #解释器的C的API版本
sys.api_version   

#解释器版本信息 返回major=3, minor=6, micro=6, releaselevel='final', serial=0
#‘final’表示最终,也有’candidate’表示候选，serial表示版本级别，是否有后继的发行
sys.version_info

#如果value非空，这个函数会把他输出到sys.stdout，并且将他保存进__builtin__._.指在python的交互式解释器里，’_’ 代表上次你输入得到的结果，hook是钩子的意思，将上次的结果钩过来
sys.displayhook(value)  


#打印出给定的回溯和异常sys.stderr。
sys.excepthook(type，value,callback)

#返回当前你所用的默认的字符编码格式
sys.getdefaultencoding()

#返回将Unicode文件名转换成系统文件名的编码的名字
sys.getfilesystemencoding()

 #用来设置当前默认的字符编码，如果name和任何一个可用的编码都不匹配，抛出 LookupError，这个函数只会被site模块的sitecustomize使用，一旦别site模块使用了，他会从sys模块移除
sys.setdefaultencoding(name)

#Python解释器导入的模块列表
sys.builtin_module_names   

#Python解释程序路径
sys.executable       

#获取Windows的版本
sys.getwindowsversion()    

#记录python版权相关的东西
sys.copyright    

#本地字节规则的指示器，big-endian平台的值是’big’,little-endian平台的值是’little’
sys.byteorder     

#用来清除当前线程所出现的当前的或最近的错误信息
sys.exc_clear()   

#返回平台独立的python文件安装的位置
sys.exec_prefix  

#错误输出
sys.stderr       

#标准输入
sys.stdin    

#标准输出
sys.stdout  

#返回操作系统平台名称
sys.platform    

#最大的Unicode值
sys.maxunicode   

#最大的Int值
sys.maxint     

#呼叫func(*args)，同时启用跟踪。跟踪状态被保存，然后恢复。
#这是从调试器从检查点调用，以递归调试其他一些代码。
sys.call_tracing(func，args) 

#清除内部类型缓存。类型缓存用于加速属性和方法查找
sys._clear_type_cache()

#返回一个字典，将每个线程的标识符映射到调用该函数时该线程中当前活动的最顶层堆栈帧。
sys._current_frames()

#指定Python DLL句柄的整数。
sys.dllhandle
 
#如果这是真的，Python将不会尝试在源模块的导入上编写.pyc或.pyo文件。
#此值最初设置为True或 False取决于-B命令行选项
#和 PYTHONDONTWRITEBYTECODE 环境变量，但您可以自己设置它来控制字节码文件的生成。
sys.dont_write_bytecode 

#获取检查间隔
sys.getcheckinterval()

#返回用于dlopen()调用的标志的当前值。标志常量在dl和DLFCN模块中定义。可用性：Unix。
sys.getdlopenflags()

#返回对象的引用计数。返回的计数通常比您预期的高一个，因为它包含（临时）引用作为参数getrefcount()。
sys.getrefcount(对象)

#以字节为单位返回对象的大小。对象可以是任何类型的对象。所有内置对象都将返回正确的结果，但这不一定适用于第三方扩展，因为它是特定于实现的。
#如果给定，则如果对象未提供检索大小的方法，则将返回default。否则TypeError将被提出。
#如果对象由垃圾收集器管理，则调用该对象的方法并添加额外的垃圾收集器开销。
sys.getsizeof(对象[, 默认]) 

#从调用堆栈返回一个框架对象。如果给出了可选的整数深度，
#则返回堆栈顶部下方多次调用的帧对象。如果它比调用堆栈更深，
#ValueError则引发。深度的默认值为零，返回调用堆栈顶部的帧。
sys._getframe([ 深度] )

#获取设置的探查器功能setprofile()。
sys.getprofile() 

#获取设置的跟踪功能settrace()。
sys.gettrace() 

#返回一个描述当前正在运行的Windows版本的命名元组
sys.getwindowsversion()
```

