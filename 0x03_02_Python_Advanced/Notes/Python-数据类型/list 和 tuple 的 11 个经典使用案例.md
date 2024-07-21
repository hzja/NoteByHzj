Python
<a name="zHXyc"></a>
### 1、判断 list 内有无重复元素
可以实现一个判断是否重复的`is_duplicated`方法，使用 list 封装的 `count` 方法，依次判断每个元素 x 在 list 内的出现次数。<br />如果大于 1，则立即返回 True，表示有重复。<br />如果完成遍历后，函数没返回，表明 list 内没有重复元素，返回 False。
```python
def is_duplicated(list):
    for item in list:
        # 判断item在list中出现次数
        if list.count(item) > 1:
            return True
    return False

if __name__ == '__main__':
    lis = [45, 89, 23, 45, 36, 78, 15, 59, 35, 21, 75, 89, 36]
    print(is_duplicated(lis))

'''
True
'''
```
<a name="VOmkF"></a>
#### 方法2：判断 list 内有无重复元素
`set()` 函数创建一个无序不重复元素集，可进行关系测试，删除重复数据，还可以计算交集、差集、并集等。<br />以上方法实现不简洁，借助 set 判断更方便
```python
def is_duplicated(lst):
    return len(lst) != len(set(lst))

if __name__ == '__main__':
    lis = [45, 89, 23, 36, 78, 15, 59, 35, 21, 75]
    print(is_duplicated(lis))

'''
False
'''
```
<a name="e72qd"></a>
### 2、列表反转
一行代码实现列表反转，非常简洁。<br />可以借助python切片`[::-1]` 生成逆向索引（负号表示逆向），步长为 1 的切片。
```python
def reverse(lst):
    # 根据切片一步实现反转列表
    return lst[::-1]

if __name__ == '__main__':
    lis = [45, 89, 23, 36, 78, 15, 59, 35, 21, 75]
    print('正向列表', lis)
    print('反向列表', reverse(lis))


'''
正向列表 [45, 89, 23, 36, 78, 15, 59, 35, 21, 75]
反向列表 [75, 21, 35, 59, 15, 78, 36, 23, 89, 45]
'''
```
<a name="LvRK4"></a>
### 3、找出列表中的所有重复元素
遍历列表，如果出现次数大于 1，且不在返回列表 ret 中，则添加到 ret 中。
```python
def find_duplicated(lis):
    new_list = []
    for item in lis:
        if lis.count(item) > 1 and item not in new_list:
            new_list.append(item)
    return new_list

if __name__ == '__main__':
    lis = [45, 89, 23, 36, 78, 15, 59, 35, 21, 75, 89, 15, 21]
    print('重复元素：', find_duplicated(lis))


'''
重复元素： [89, 15, 21]
'''
```
<a name="lj1uP"></a>
### 4、生成器生成斐波那契数列
斐波那契数列（Fibonacci sequence），又称黄金分割数列<br />使用 Python 的生成器生成斐波那契数列，保证代码简洁的同时，<br />还能节省内存：
```python
def fibonacci(n):
    a, b = 1, 1
    for x in range(n):
        yield a
        a, b = b, a + b

if __name__ == '__main__':
    print(list(fibonacci(10)))

'''
[1, 1, 2, 3, 5, 8, 13, 21, 34, 55]
'''
```
<a name="eD9Nz"></a>
### 5、更多参数
带有一个 `*` 的参数为可变的位置参数，意味着能传入任意多个位置参数。<br />求众多列表中元素最多的一个：
```python
def max_len(*lists):
    # key 函数定义怎么比较大小：lambda 的参数 v 是 lists 中的一个元素。
    return max(*lists, key=lambda v: len(v))

if __name__ == '__main__':
    print('最长列表是：', max_len([1, 2, 3], [4, 5, 6, 7], [8, 9, 10]))


'''
最长列表是： [4, 5, 6, 7]
'''
```
<a name="nvw2Q"></a>
### 6、求表头
返回列表的第一个元素，注意列表为空时，返回 None。
```python
def head(lst):
    return lst[0] if len(lst) > 0 else None

if __name__ == '__main__':
    print('列表头元素：', head([-9, 8, 9, 10]))


'''
列表头元素： -9
'''
```
<a name="O6ntM"></a>
### 7、求表尾
有头就有尾，在Python中，可以使用索引-1来获取列表的最后一个元素。
```python
def tail(lst):
    return lst[-1] if len(lst) > 0 else None

if __name__ == '__main__':
    print('列表尾元素：', tail([-9, 8, 9, 10, 100]))


'''
列表尾元素：100
'''
```
<a name="Grsbx"></a>
### 8、打印乘法表
for-for外层循环一次，`print()`，换行；内层循环一次，打印一个等式。
```python
def mul_table():
    for i in range(1, 10):
        for j in range(1, i + 1):
            print(f'{j}x{i}={i * j}\t', end='')
        print()    # 打印一个换行

if __name__ == '__main__':
    mul_table()



'''
1x1=1 
1x2=2 2x2=4 
1x3=3 2x3=6 3x3=9 
1x4=4 2x4=8 3x4=12 4x4=16 
1x5=5 2x5=10 3x5=15 4x5=20 5x5=25 
1x6=6 2x6=12 3x6=18 4x6=24 5x6=30 6x6=36 
1x7=7 2x7=14 3x7=21 4x7=28 5x7=35 6x7=42 7x7=49 
1x8=8 2x8=16 3x8=24 4x8=32 5x8=40 6x8=48 7x8=56 8x8=64 
1x9=9 2x9=18 3x9=27 4x9=36 5x9=45 6x9=54 7x9=63 8x9=72 9x9=81 
'''
```
<a name="y59m8"></a>
### 9、拼接元素
`zip()` 函数用于将可迭代的对象作为参数，将对象中对应的元素打包成一个个元组，然后返回由这些元组组成的列表。<br />类似于下：
```python
a = [1, 11, 111]
b = [2, 22, 222]
print(list(zip(a, b)))

'''
[(1, 2), (11, 22), (111, 222)]
'''
```
`t[:-1]`：原列表切掉最后一个元素；<br />`t[1:]`：原列表切掉第一个元素；<br />`zip(iter1, iter2)`：实现 iter1 和 iter2 的对应索引处的元素拼接。
```python
def pair(t):
    # 生成相邻元素对
    return list(zip(t[:-1], t[1:]))


if __name__ == '__main__':
    print(pair([4, 5, 6, 7]))
    print(pair(range(10)))

'''
[(4, 5), (5, 6), (6, 7)]
[(0, 1), (1, 2), (2, 3), (3, 4), (4, 5), (5, 6), (6, 7), (7, 8), (8, 9)]
'''
```
<a name="b6OiS"></a>
### 10、样本抽样
内置 random 模块中，有一个 sample 函数，实现“抽样”功能。<br />下面例子从 100 个样本中，随机抽样 10 个。<br />首先，使用列表生成式，创建长度为 100 的列表 lst；<br />然后，sample 抽样 10 个样本。
```python
from random import randint, sample

# randint 生成随机整数；
lst = [randint(0, 50) for x in range(100)]
print(lst)
print('列表前五个元素：', lst[:5])
print('列表最后五个元素：', lst[-5:])

# sample 从 lst 中抽样 10 个元素
lst_sample = sample(lst, 10)
print('列表随机十个元素：', lst_sample)

'''
[46, 13, 8, 14, 2, 35, 16, 18, 29, 28, 0, 14, 4, 31, 26, 26, 17, 31, 26, 3, 17, 29, 14, 9, 42, 11, 27, 0, 1, 39, 34, 35, 34, 6, 50, 40, 30, 40, 44, 2, 0, 15, 12, 3, 34, 7, 40, 29, 14, 15, 27, 39, 6, 32, 1, 38, 35, 26, 50, 27, 11, 2, 28, 33, 49, 47, 43, 44, 47, 23, 33, 15, 19, 36, 31, 49, 42, 6, 13, 21, 1, 29, 12, 9, 46, 43, 37, 18, 22, 47, 36, 8, 12, 4, 39, 17, 48, 4, 33, 21]
列表前五个元素： [46, 13, 8, 14, 2]
列表最后五个元素： [17, 48, 4, 33, 21]
列表随机十个元素： [30, 12, 40, 31, 47, 42, 6, 14, 14, 49]
'''
```
<a name="KLhhw"></a>
### 11、重洗数据集
内置 `random` 中的 `shuffle` 函数，能冲洗数据。<br />值得注意，shuffle 是对输入列表就地（in place）洗牌，节省存储空间。
```python
from random import shuffle, randint
lst = [randint(0, 30) for _ in range(100)]
print(lst[:5])

# 重洗数据
shuffle(lst)
print(lst[:5])

'''
[30, 27, 8, 25, 14]
[2, 29, 29, 5, 14]
'''
```
