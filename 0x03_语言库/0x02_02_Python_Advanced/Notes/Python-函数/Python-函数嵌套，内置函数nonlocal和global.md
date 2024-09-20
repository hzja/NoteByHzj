Python 函数嵌套 内置函数 nonlocal global
<a name="IEzRt"></a>
### 一、函数的嵌套
<a name="OqHbp"></a>
#### 案例1
代码的执行顺序是按从上到下执行，以下给大家列出代码的执行顺序，代码先加载函数名.函数真正调用的时候加载函数里的代码
```python
def test1(): #1
    print("test1方法----") #6
def test2(): #2
    print("test2方法") #4
test2()#3
test1()#5
print("test1方法")#7

输出：
test2方法
test1方法----
test1方法
```
<a name="EkH0r"></a>
#### 案例2
```python
def test1(): #1
    print("test1方法") #4
    def test2():#5
        print("test2方法") #8
    print("****1") #6
    test2() #7
    print("****2") #9
print("***3") #2
test1()#3
print("***4") #10

#输出：
***3
test1方法
****1
test2方法
****2
***4
```
<a name="N1Sl7"></a>
### 二、 内置函数gloabal、nonlocal
1、注意：关键字nonlocal:是python3.X中出现的,所以在python2.x中无法直接使用<br />2、区别：<br />（1）global关键字用来在函数或其它局部作用域中使用全局变量。但是如果不使用全局变量也可以不适用global关键字声明。<br />（2）nonlocal关键字用来在函数或其它作用域中使用外层（非全局）变量<br />3.具体例子：
<a name="St0ZC"></a>
#### 函数gloabal的使用
```python
a = 10
def test1():
    global  a
    print(a)
    a = 100
print(a)
test1()
print(a)

输出：
10
10
100
```
```python
对于可变数据类型可以直接进⾏访问
lst = ["python", "java", "C++"]
def func():
    lst.append("C")
    print(lst)
func()
print(lst)
```
<a name="oa0nr"></a>
#### 函数nonlocal的使用
```python
带有nonlocal a
a = 100
def test1():
    a = 200
    def test2():
        nonlocal a
        a = 300
        print(a)
    test2()
    print(a)
test1()

#输出：
300
300
```
```python
没有nonlocal a
a = 100
def test1():
    a = 200
    def test2():
        a = 300
        print(a)
    test2()
    print(a)
test1()

#输出：
300
200
```
