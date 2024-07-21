# sys模块

[sys -系统相关的形参和函数-文档](https://docs.python.org/zh-cn/3/library/sys.html#module-sys)

## sys.argv

向程序内传参

~~~ python
import sys
 
for i in sys.argv:
   print(i)

[root@elk ~]# python3 test.py  1 2 3
>>> test.py
>>> 1
>>> 2
>>> 3
print(len(sys.argv))
>>> 4
print(type(sys.argv[3]))
>>> <class 'str'>
print(type(int(sys.argv[3])))
>>> <class 'int'>
~~~



## sys.exit(n)

若需要中途退出程序可调用sys.exit()函数(arg=0为正常退出,其他为异常)

~~~ python
import sys

def exitfunc(value):
    print value
    sys.exit(0)

print "hello"

try:
    sys.exit(1)
except SystemExit,value:
    exitfunc(value)

print "come?"
~~~



## sys.path

获取指定模块搜索路径字符串集合，可将写好模块放在得到的某路径下就可程序中import时正确找到

~~~ python
import sys

print(sys.path)
['', '/usr/lib/python2.7', '/usr/lib/python2.7/plat-x86_64-linux-gnu', '/usr/lib/python2.7/lib-tk', '/usr/lib/python2.7/lib-old', '/usr/lib/python2.7/lib-dynload', '/usr/local/lib/python2.7/dist-packages', '/usr/lib/python2.7/dist-packages', '/usr/lib/python2.7/dist-packages/PILcompat', '/usr/lib/python2.7/dist-packages/gtk-2.0', '/usr/lib/python2.7/dist-packages/ubuntu-sso-client']
~~~

