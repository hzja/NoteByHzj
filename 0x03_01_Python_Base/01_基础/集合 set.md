# 集合 set

## 什么是集合？

> 1. 字典是键值对，键值不能重复
> 2. 集合值, 值不能重复
> 3. 集合也是无序的， 也就是相当于没有下标 没有索引
> 4. 集合我们一般用来去重

集合(set)是一无序不重复元素序列
可用大括号 **{ }** 或 **set()** 函数创建集合,注意:创建一空集合须用 **set()** 而非 **{ }**因为 **{ }** 用来创建一空字典



### 创建格式

```
parame = {value01,value02,...}
或者
set(value)
```

~~~ pyhton 
>>> basket = {'apple', 'orange', 'apple', 'pear', 'orange', 'banana'}
>>> print(basket)                      # 这里演示的是去重功能
{'orange', 'banana', 'pear', 'apple'}
>>> 'orange' in basket                 # 快速判断元素是否在集合内
True
>>> 'crabgrass' in basket
False

>>> # 下面展示两个集合间的运算.
...
>>> a = set('abracadabra')
>>> b = set('alacazam')
>>> a                                  
{'a', 'r', 'b', 'c', 'd'}
>>> a - b                              # 集合a中包含而集合b中不包含的元素
{'r', 'd', 'b'}
>>> a | b                              # 集合a或b中包含的所有元素
{'a', 'c', 'r', 'd', 'b', 'm', 'z', 'l'}
>>> a & b                              # 集合a和b中都包含了的元素
{'a', 'c'}
>>> a ^ b                              # 不同时包含于a和b的元素
{'r', 'd', 'b', 'm', 'z', 'l'}
~~~

类似列表推导式，同样集合支持集合推导式(Set comprehension):

~~~ python 
>>> a = {x for x in 'abracadabra' if x not in 'abc'}
>>> a
{'r', 'd'}
~~~



## 集合的基本操作

### 1.添加元素

**语法格式：**

```python
s.add( x )
```



将元素x添加到集合 s 中，如元素已存在则不进行任何操作

~~~ python 
>>> thisset = set(("Google", "Runoob", "Taobao"))`
>>> thisset.add("Facebook")
>>> **print**(thisset)`
{'Taobao', 'Facebook', 'Google', 'Runoob
~~~



还有一方法也可添加元素且参数可是列表,元组,字典等，语法格式如下：

```
s.update( x )
```



x 可有多个，用逗号分开

~~~ python 
>>> thisset = set(("Google", "Runoob", "Taobao"))
>>> thisset.update({1,3})
>>> **print**(thisset)
{1, 3, 'Google', 'Taobao', 'Runoob'}
>>> thisset.update([1,4],[5,6]) 
>>> **print**(thisset)
{1, 3, 4, 5, 6, 'Google', 'Taobao', 'Runoob'}
~~~



### 2、移除元素

**语法格式：**

```
s.remove( x )
```



将元素 x 从集合 s 中移除，如元素不存在则发生错误

~~~ python 
>>> thisset = set(("Google", "Runoob", "Taobao"))
>>> thisset.remove("Taobao")
>>> **print**(thisset)
{'Google', 'Runoob'}
>>> thisset.remove("Facebook")  # 不存在会发生错误
Traceback (most recent call last):
 File "<stdin>", line 1, **in** <module>
KeyError: 'Facebook'
~~~



此外还有一方法也是移除集合中元素,且如元素不存在不会发生错误. 格式如下：

```
s.discard( x )
```

~~~ python 
>>> thisset = set(("Google", "Runoob", "Taobao"))
>>> thisset.discard("Facebook") # 不存在不会发生错误
>>> **print**(thisset)
{'Taobao', 'Google', 'Runoob'}
~~~



也可设随机删除集合中一元素，语法格式：

```
s.pop() 
```

~~~ python
thisset = set(("Google", "Runoob", "Taobao", "Facebook"))
x = thisset.pop()

print(x)
~~~

输出结果：

```
$ python3 test.py 
Runoob
```



多次执行测试结果都不一样,set 集合 pop 方法会对集合进行无序排列后将这无序排列集合的左面第一个元素删除



### 3、计算集合元素个数

**语法格式：**

```
len(s)
```



计算集合 s 元素个数

~~~ python
>>> thisset = set(("Google", "Runoob", "Taobao"))
>>> len(thisset)
3
~~~



### 4、清空集合

**语法格式：**

```
s.clear()
```



清空集合 s

~~~ python
>>> thisset = set(("Google", "Runoob", "Taobao"))
>>> thisset.clear()
>>> print(thisset)
set()
~~~



### 5、判断元素是否在集合中存在

**语法格式：**

```
x in s
```



判断元素 x 是否在集合s中，存在返 True不存在返False

~~~ python 
>>> thisset = set(("Google", "Runoob", "Taobao"))
>>> "Runoob" in thisset
True
>>> "Facebook" in thisset
False
~~~



## 集合内置方法完整列表

| 方法                                                         | 描述                                                         |
| :----------------------------------------------------------- | :----------------------------------------------------------- |
| [add()](https://www.runoob.com/python3/ref-set-add.html)     | 为集合添加元素                                               |
| [clear()](https://www.runoob.com/python3/ref-set-clear.html) | 移除集合中的所有元素                                         |
| [copy()](https://www.runoob.com/python3/ref-set-copy.html)   | 拷贝一个集合                                                 |
| [difference()](https://www.runoob.com/python3/ref-set-difference.html) | 返回多个集合的差集                                           |
| [difference_update()](https://www.runoob.com/python3/ref-set-difference_update.html) | 移除集合中的元素，该元素在指定的集合也存在。                 |
| [discard()](https://www.runoob.com/python3/ref-set-discard.html) | 删除集合中指定的元素                                         |
| [intersection()](https://www.runoob.com/python3/ref-set-intersection.html) | 返回集合的交集                                               |
| [intersection_update()](https://www.runoob.com/python3/ref-set-intersection_update.html) | 返回集合的交集。                                             |
| [isdisjoint()](https://www.runoob.com/python3/ref-set-isdisjoint.html) | 判断两个集合是否包含相同的元素，如果没有返回 True，否则返回 False。 |
| [issubset()](https://www.runoob.com/python3/ref-set-issubset.html) | 判断指定集合是否为该方法参数集合的子集。                     |
| [issuperset()](https://www.runoob.com/python3/ref-set-issuperset.html) | 判断该方法的参数集合是否为指定集合的子集                     |
| [pop()](https://www.runoob.com/python3/ref-set-pop.html)     | 随机移除元素                                                 |
| [remove()](https://www.runoob.com/python3/ref-set-remove.html) | 移除指定元素                                                 |
| [symmetric_difference()](https://www.runoob.com/python3/ref-set-symmetric_difference.html) | 返回两个集合中不重复的元素集合。                             |
| [symmetric_difference_update()](https://www.runoob.com/python3/ref-set-symmetric_difference_update.html) | 移除当前集合中在另外一个指定集合相同的元素，并将另外一个指定集合中不同的元素插入到当前集合中。 |
| [union()](https://www.runoob.com/python3/ref-set-union.html) | 返回两个集合的并集                                           |
| [update()](https://www.runoob.com/python3/ref-set-update.html) | 给集合添加元素                                               |