# format模块

在%方法中%s只能替代字符串类型，而在format中不需要理会数据类型；

~~~ python
1、按照默认顺序，不指定位置
print("{} {}".format("hello","world"))
>>> hello world

2、设置指定位置，可以多次使用
print("{0} {1} {0}".format("hello","or"))
>>> hello or hello

3、使用列表格式化
person = {"name":"opcai","age":20}
print("My name is {name} . I am {age} years old .".format(**person))
>>> My name is opcai . I am 20 years old .

4、通过列表格式化
stu = ["opcai","linux","MySQL","Python"]
print("My name is {0[0]} , I love {0[1]} !".format(stu))
>>> My name is opcai , I love linux !
~~~

