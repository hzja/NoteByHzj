Python
<a name="VIfoX"></a>
## 1、交换两个变量
```python
# a = 1; b = 2
a, b = b, a
# print(a,b) >> 2 1
```
从经典开始：通过简单地交换赋值位置来交换变量的值——这是最直观的方式。无需使用临时变量。它甚至适用于两个以上的变量。
<a name="J7JFw"></a>
## 2、多变量赋值
```python
a, b, *c = [1,2,3,4,5]
# print(a,b,c) >> 1 2 [3, 4, 5]
```
交换变量实际上是 Python 一次分配多个变量的能力的一个特例。在这里可以使用它来将列表元素分配给给定的变量，这也称为_拆包_。将*再次打包剩余的值，这会产生一个子列表c。它甚至适用于每个其他位置*（例如列表的开头或中间部分）。
<a name="gXCEm"></a>
## 3、对列表的每个第二个元素求和
```python
# a = [1,2,3,4,5,6]
s = sum(a[1::2])
# print(s) >> 12
```
这里不需要特殊的 `reduce` 函数，sum只需添加每个给定 iterable 的项目。此处使用扩展切片语法 [::]来返回每隔一个元素。可以将其读作 `[start : stop : step]`，因此`[1::2]`转换为从索引 1 的元素（第二个元素）开始，直到列表结束才停止（没有为第二个参数提供参数）并且始终采取 2 steps。
<a name="Y3itC"></a>
## 4、删除多个元素
```python
# a = [1,2,3,4,5]
del a[::2]
# print(a) >> [2, 4]
```
扩展的切片语法也可用于一次删除多个列表元素。
<a name="JxnfO"></a>
## 5、将文件读入行数组
```python
c = [line.strip() for line in open('file.txt')]
# print(c) >> ['test1', 'test2', 'test3', 'test4']
```
使用 Python 的内联 for 循环，可以轻松地将文件读入行数组。`strip()`需要删除尾随换行符。如果想保留它们或者它们无关紧要，可以使用更短的单线：
```python
c = list(open('file.txt'))
# print(c) >> ['test1\n', 'test2\n', 'test3\n', 'test4\n']
```
用 Python 读取文件真的就是这么简单。旁注：也可以根据需要使用该`readlines()`方法。
<a name="Y3NeJ"></a>
## 6、将字符串写入文件
```
with open('file.txt', 'a') as f: f.write('hello world')
# print(list(open('file.txt'))) >> ['test1\n', 'test2\n', 'test3\n', 'test4\n', 'hello world']
```
借助`with`语句，可以直接将内容写入文件。确保使用正确的模式打开文件（此处'a'为附加**内容**）。
<a name="jPuvH"></a>
## 7、列表创建
```python
l = [('Hi ' + x) for x in ['Alice', 'Bob', 'Pete']]
# print(l) >> ['Hi Alice', 'Hi Bob', 'Hi Pete']
```
可以使用内联 for 循环从其他列表动态创建列表。可以直接修改值，如本例中的字符串连接。
<a name="pZy79"></a>
## 8、列表映射
```python
l = list(map(int, ['1', '2', '3']))
# print(l) >> [1, 2, 3]
```
可以使用 Python `map()`函数将每个列表元素转换为另一种类型。
<a name="pyJme"></a>
## 9、创建集合
```python
squares = { x**2 for x in range(6) if x < 4 }
# print(squares) >> {0, 1, 4, 9}
```
它与sets相似，除了内联 for 循环，甚至可以直接附加条件！
<a name="CymQL"></a>
## 10、回文检查
```python
# phrase = 'deleveled'
isPalindrome = phrase == phrase[::-1]
# print(isPalindrome) >> true
```
回文是一系列正向和反向读取相同的字符。通常需要一些循环和条件来检查给定的字符串是否是回文。在 Python 中，只需将字符串与其反向字符串进行比较。除了使用切片运算符`[::-1]`，还可以使用`reverse()`函数来反转字符串。
