# 列表 list		

序列是 Python 中最基本的数据结构其每个值对应位置值称索引，第一个索引是 0，第二个索引是 1，依此类推；

Python 有 6 个序列的内置类型，但最常见的是列表和元组；

列表都可以进行的操作包括索引、切片、加、乘、检查成员，且Python 已经内置确定序列的长度以及确定最大和最小的元素的方法；

列表是最常用的 Python 数据类型，可作为一方括号内逗号分隔值出现，且列表的数据项不需要具有相同的类型；



## 创建列表

创建一个列表，只要把逗号分隔的不同的数据项使用方括号括起来即可，如下所示：

~~~ python
list1 = ['Google', 'Runoob', 1997, 2000]
list2 = [1, 2, 3, 4, 5 ]
list3 = ["a", "b", "c", "d"]
list4 = ['red', 'green', 'blue', 'yellow', 'white', 'black']
~~~



## 更新列表

~~~ python
# 列表最后增加值
my_list.append(777)

# 插入值
my_list.insert(1, '888') 

# 列表合并
my_list.extend(my_list2)
~~~



## 删除元素

~~~ python
# 删除指定索引的数据
del my_list[1]

# 只会删除第一次出现的值
my_list.remove(40)

# 删除末尾数据
my_list.pop()

# 清空列表
my_list.clear()
~~~



## 修改和查询

~~~ python
# 根据值查询对应的索引
my_list.index(3)

# 统计数据出现次数
my_list.count(2)

# 最大值，最小值
max(my_list)
min(my_list)
~~~



## 查询列表

### 查询值

#### 正向索引

与字符串中索引一样，列表索引从<kbd>0</kbd>开始，第二个索引是<kbd>1</kbd>，通过索引列表可截取、组合等操作

![positive-indexes-1](.\图片\positive-indexes-1.png)

~~~ python
list = ['red', 'green', 'blue', 'yellow', 'white', 'black']
print( list[0] )
print( list[1] )
print( list[2] )

输出结果：
red
green
blue
~~~



#### 反向索引

索引也可从尾部开始，最后一个元素索引为<kbd>-1</kbd>，往前一位为<kbd>-2</kbd>等

![negative-indexes](.\图片\negative-indexes.png)

~~~ python
list = ['red', 'green', 'blue', 'yellow', 'white', 'black']
print( list[-1] )
print( list[-2] )
print( list[-3] )

输出结果：
black
white
yellow
~~~



### 截取片段

使用下标索引访问列表中的值，同样也可用方括号<kbd>[]</kbd>形式截取字符：

![first-slice](.\图片\first-slice.png)

#### 正向截取

~~~ python
nums = [10, 20, 30, 40, 50, 60, 70, 80, 90]
print(nums[0:4])

输出结果：
[10, 20, 30, 40]
~~~



#### 反向截取

~~~ python
list = ['Google', 'Runoob', "Zhihu", "Taobao", "Wiki"]
# 读取第二位
print ("list[1]: ", list[1])
# 从第二位开始（包含）截取到倒数第二位（不包含）
print ("list[1:-2]: ", list[1:-2])

输出结果：
list[1]:  Runoob
list[1:-2]:  ['Runoob', 'Zhihu']
~~~



## 列表排序

~~~ python
# 排序， 默认从小到大
my_list.sort()

# 降序
my_list.sort(reverse=True)

# 排序,处理的一般是可迭代对象，不会直接修改原列表，通常用它替代sort()
my_list.sorted()

# 反转列表
my_list.reverse()
~~~



### sort 与sorted的区别

sort是在原位重新排列列表，而sorted()是产生一个新的列表。

sort 是应用在 list 上的方法，sorted 可以对所有可迭代的对象进行排序操作。



## 列表遍历

~~~ python
# 遍历列表返回下标和元素  
seq = ['one', 'two', 'three']
for i, element in enumerate(seq):
	print i, element
 
0 one
1 two
2 three
~~~



## 列表脚本操作符

列表对<kbd>+</kbd>和<kbd>*</kbd>的操作符与字符串类似，<kbd>+</kbd>用于组合列表而<kbd>  * </kbd>用于重复列表

| python表达式                          | 结果                         | 描述                 |
| ------------------------------------- | ---------------------------- | -------------------- |
| len([1, 2, 3])                        | 3                            | 长度                 |
| [1, 2, 3] + [4, 5, 6]                 | [1, 2, 3, 4, 5, 6]           | 组合                 |
| ['Hi!'] * 4                           | ['Hi!', 'Hi!', 'Hi!', 'Hi!'] | 重复                 |
| 3 in [1, 2, 3]                        | True                         | 元素是否存在于列表中 |
| for x in [1, 2, 3]: print(x, end=" ") | 1 2 3                        | 迭代                 |



## 列表截取与拼接

| python表达式 | 结果                 | 描述                                               |
| ------------ | -------------------- | -------------------------------------------------- |
| L[2]         | 'Taobao'             | 读取第三个元素                                     |
| L[-2]        | 'Runoob'             | 从右侧开始读取倒数第二个元素: count from the right |
| L[1:]        | ['Runoob', 'Taobao'] | 输出从第二个元素开始后的所有元素                   |

~~~ python 
截取列表：
L=['Google', 'Runoob', 'Taobao']
>>>L=['Google', 'Runoob', 'Taobao']
>>> L[2]
'Taobao'
>>> L[-2]
'Runoob'
>>> L[1:]
['Runoob', 'Taobao']

拼接列表：
>>>squares = [1, 4, 9, 16, 25]
>>> squares += [36, 49, 64, 81, 100]
>>> squares
[1, 4, 9, 16, 25, 36, 49, 64, 81, 100]
~~~



## 列表嵌套

使用嵌套列表在列表中创建其他列表

~~~ python
>>>a = ['a', 'b', 'c']
>>> n = [1, 2, 3]
>>> x = [a, n]
>>> x
[['a', 'b', 'c'], [1, 2, 3]]
>>> x[0]
['a', 'b', 'c']
>>> x[0][1]
'b'
~~~



## 列表比较

列表比较需引入<kbd>operator</kbd>模块的<kbd>eq</kbd>方法

~~~ python
import operator
a = [1, 2]
b = [2, 3]
c = [2, 3]
print("operator.eq(a,b): ", operator.eq(a,b))
print("operator.eq(c,b): ", operator.eq(c,b))

输出结果：
operator.eq(a,b):  False
operator.eq(c,b):  True
~~~



## 列表函数和方法

### 列表函数

| 序号 | 函数                                                         |
| ---- | ------------------------------------------------------------ |
| 1    | [len(list)](https://www.runoob.com/python3/python3-att-list-len.html) 列表元素个数 |
| 2    | [max(list)](https://www.runoob.com/python3/python3-att-list-max.html) 返回列表元素最大值 |
| 3    | [min(list)](https://www.runoob.com/python3/python3-att-list-min.html) 返回列表元素最小值 |
| 4    | [list(seq)](https://www.runoob.com/python3/python3-att-list-list.html) 将元组转换为列表 |

### 列表方法

| 序号 | 方法                                                         |
| ---- | ------------------------------------------------------------ |
| 1    | [list.append(obj)](https://www.runoob.com/python3/python3-att-list-append.html) 在列表末尾添加新的对象 |
| 2    | [list.count(obj)](https://www.runoob.com/python3/python3-att-list-count.html) 统计某个元素在列表中出现的次数 |
| 3    | [list.extend(seq)](https://www.runoob.com/python3/python3-att-list-extend.html) 在列表末尾一次性追加另一个序列中的多个值（用新列表扩展原来的列表） |
| 4    | [list.index(obj)](https://www.runoob.com/python3/python3-att-list-index.html) 从列表中找出某个值第一个匹配项的索引位置 |
| 5    | [list.insert(index, obj)](https://www.runoob.com/python3/python3-att-list-insert.html) 将对象插入列表 |
| 6    | [list.pop([index=-1\])](https://www.runoob.com/python3/python3-att-list-pop.html)移除列表中的一个元素（默认最后一个元素），并且返回该元素的值 |
| 7    | [list.remove(obj)](https://www.runoob.com/python3/python3-att-list-remove.html) 移除列表中某个值的第一个匹配项 |
| 8    | [list.reverse()](https://www.runoob.com/python3/python3-att-list-reverse.html) 反向列表中元素 |
| 9    | [list.sort( key=None, reverse=False)](https://www.runoob.com/python3/python3-att-list-sort.html) 对原列表进行排序 |
| 10   | [list.clear()](https://www.runoob.com/python3/python3-att-list-clear.html) 清空列表 |
| 11   | [list.copy()](https://www.runoob.com/python3/python3-att-list-copy.html) 复制列表 |

