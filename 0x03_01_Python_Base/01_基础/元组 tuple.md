# 元组 tuple

> 元组与列表类似，不同在于元组元素不能修改；
>
> 元组用小括号 **( )**，列表用方括号 **[ ]**；
>
> 元组创建很简单只需括号中添加元素并用逗号隔开即可；

~~~ bash
# 元组和列表最大的区别？ 元组中的元素不能修改，列表中的元素可以任意修改.
~~~

![tup-2020-10-27-10-26-2](D:\Note\python\基础\图片\tup-2020-10-27-10-26-2.png)



~~~ python
>>> tup1 = ('Google', 'Runoob', 1997, 2000)
>>> tup2 = (1, 2, 3, 4, 5 )
>>> tup3 = "a", "b", "c", "d"   #  不需要括号也可以
>>> type(tup3)
<class 'tuple'>
~~~



创建空元组

```python
tup1 = ()
```



元组中只含一元素时需在元素后加逗号 **,**否则括号被当作运算符用：

~~~ python
>>> tup1 = (50)
>>> type(tup1)     # 不加逗号，类型为整型
<class 'int'>

>>> tup1 = (50,)
>>> type(tup1)     # 加上逗号，类型为元组
<class 'tuple'>
~~~



元组与字符串类似，下标索引从 0 开始可截取、组合等

![py-tup-10-26](D:\Note\python\基础\图片\py-tup-10-26.png)



## 创建一元组

~~~ python
my_tuple = (10, 20, 30, 40)
print(my_tuple[0], my_tuple[1], my_tuple[2])


# 如果元组中只有一个元素的时候，添加一个逗号
my_tuple = (10,)
print(type(my_tuple))
~~~



## 访问元组

元组可用下标索引访问元组中值，如下实例:

~~~ python 
#!/usr/bin/python3
 
tup1 = ('Google', 'Runoob', 1997, 2000)
tup2 = (1, 2, 3, 4, 5, 6, 7 )
 
print ("tup1[0]: ", tup1[0])
print ("tup2[1:5]: ", tup2[1:5])
~~~

以上实例输出结果：

```python 
tup1[0]:  Google
tup2[1:5]:  (2, 3, 4, 5)
```



## 修改元组

元组中元素值不允许修改，但可对元组连接组合，如下实例:

~~~ python
#!/usr/bin/python3
 
tup1 = (12, 34.56)
tup2 = ('abc', 'xyz')
 
# 以下修改元组元素操作是非法的。
# tup1[0] = 100
 
# 创建一个新的元组
tup3 = tup1 + tup2
print (tup3)
~~~

以上实例输出结果：

```
(12, 34.56, 'abc', 'xyz')
```



## 删除元组

元组元素值不允许删除，但可用del语句删除整个元组，如下实例:

~~~ python
#!/usr/bin/python3
 
tup = ('Google', 'Runoob', 1997, 2000)
 
print (tup)
del tup
print ("删除后的元组 tup : ")
print (tup)
~~~

以上实例元组被删除后输出变量会有异常信息，输出如下：

```
删除后的元组 tup : 
Traceback (most recent call last):
  File "test.py", line 8, in <module>
    print (tup)
NameError: name 'tup' is not defined
```



## 元组运算符

与字符串一样元组间可使用 **+**、**+=**和 ***** 运算，意味着可组合和复制，运算后生成一新元组

| Python 表达式                                                | 结果                         | 描述                                                         |
| :----------------------------------------------------------- | :--------------------------- | :----------------------------------------------------------- |
| `len((1, 2, 3))`                                             | 3                            | 计算元素个数                                                 |
| `>>> a = (1, 2, 3) >>> b = (4, 5, 6) >>> c = a+b >>> c (1, 2, 3, 4, 5, 6)` | (1, 2, 3, 4, 5, 6)           | 连接，c 就是一个新的元组，它包含了 a 和 b 中的所有元素。     |
| `>>> a = (1, 2, 3) >>> b = (4, 5, 6) >>> a += b >>> a (1, 2, 3, 4, 5, 6)` | (1, 2, 3, 4, 5, 6)           | 连接，a 就变成了一个新的元组，它包含了 a 和 b 中的所有元素。 |
| `('Hi!',) * 4`                                               | ('Hi!', 'Hi!', 'Hi!', 'Hi!') | 复制                                                         |
| `3 in (1, 2, 3)`                                             | True                         | 元素是否存在                                                 |
| `for x in (1, 2, 3):     print (x, end=" ")`                 | 1 2 3                        | 迭代                                                         |



## 元组索引，截取

因为元组也是一序列，所以可访问元组中指定位置的元素，也可截取索引中一段元素，如下所示：

```
tup = ('Google', 'Runoob', 'Taobao', 'Wiki', 'Weibo','Weixin')
```

![py-tup-7](D:\Note\python\基础\图片\py-tup-7.png)

| Python 表达式 | 结果                                            | 描述                                             |
| :------------ | :---------------------------------------------- | :----------------------------------------------- |
| tup[1]        | 'Runoob'                                        | 读取第二个元素                                   |
| tup[-2]       | 'Weibo'                                         | 反向读取，读取倒数第二个元素                     |
| tup[1:]       | ('Runoob', 'Taobao', 'Wiki', 'Weibo', 'Weixin') | 截取元素，从第二个开始后的所有元素。             |
| tup[1:4]      | ('Runoob', 'Taobao', 'Wiki')                    | 截取元素，从第二个开始到第四个元素（索引为 3）。 |

运行如下：

~~~ python
>>> tup = ('Google', 'Runoob', 'Taobao', 'Wiki', 'Weibo','Weixin')
>>> tup[1]
'Runoob'
>>> tup[-2]
'Weibo'
>>> tup[1:]
('Runoob', 'Taobao', 'Wiki', 'Weibo', 'Weixin')
>>> tup[1:4]
('Runoob', 'Taobao', 'Wiki')
~~~



## 元组内置函数

元组包含以下内置函数

| 序号 | 方法及描述                               | 实例                                                         |
| :--- | :--------------------------------------- | :----------------------------------------------------------- |
| 1    | len(tuple) 计算元组元素个数。            | `>>> tuple1 = ('Google', 'Runoob', 'Taobao') >>> len(tuple1) 3 >>> ` |
| 2    | max(tuple) 返回元组中元素最大值。        | `>>> tuple2 = ('5', '4', '8') >>> max(tuple2) '8' >>> `      |
| 3    | min(tuple) 返回元组中元素最小值。        | `>>> tuple2 = ('5', '4', '8') >>> min(tuple2) '4' >>> `      |
| 4    | tuple(iterable) 将可迭代系列转换为元组。 | `>>> list1= ['Google', 'Taobao', 'Runoob', 'Baidu'] >>> tuple1=tuple(list1) >>> tuple1 ('Google', 'Taobao', 'Runoob', 'Baidu')` |



### 关于元组不可变

元组不可变指元组所指向的内存中内容不可变

~~~ python
>>> tup = ('r', 'u', 'n', 'o', 'o', 'b')
>>> tup[0] = 'g'     # 不支持修改元素
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
TypeError: 'tuple' object does not support item assignment
>>> id(tup)     # 查看内存地址
4440687904
>>> tup = (1,2,3)
>>> id(tup)
4441088800    # 内存地址不一样了
~~~

从以上实例，重新赋值的元组 tup绑定到新对象，不是修改了原来的对象



## 解包

~~~python
my_tuple = (10, 20)
a, b = my_tuple
print(a, b)
~~~

