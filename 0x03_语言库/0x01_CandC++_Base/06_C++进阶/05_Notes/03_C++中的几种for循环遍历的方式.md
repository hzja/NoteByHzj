# C++ for

<a name="GvcMx"></a>

#### 通过 `:` 遍历
C++新标准提供的范围for语句。这种语句遍历给定**序列中**个元素并对序列中每一个值执行某种操作，其语法形式是：
```cpp
for(declaration : expression)
   statement
```
其中，`expression`部分是一个对象，用于表示一个序列。`declaration`部分负责定义一个变量，该变量将用于访问序列中的基础元素。每次迭代，`declaration`部分的变量会被初始化为`expression`部分的`下一个元素值`。<br />例子：
```cpp
#include <iostream>
using namespace std;
int main()
{
    string str("this is a c++");
    //每行输出str中的一个字符
    for(auto c : str)
        cout<<c<<endl;
    system("pause");
    return 0;
}
```
代码输出：
```
t
h
i
s
i
s
a
c
+
+
请按任意键继续. . .
```
<a name="Gqkfb"></a>
#### 通过遍历的对象的长度进行循环
代码中的 `auto` 关键字让编译器来决定 `c`的类型，每次迭代后，`str`的下一个字符赋值给 `c`。<br />看看比较正常的 for 语句
```cpp
#include <iostream>
using namespace std;
int main()
{
    string str("this is a c++");
    for(int i = 0;i<str.length();i++)
        cout<<str.at(i)<<endl;
    system("pause");
    return 0;
}
```
输出：
```
t
h
i
s
i
s
a
c
+
+
请按任意键继续. . .
```
<a name="qamgd"></a>
#### 通过字符串的 `begin()` 和 `end()` 方法
```cpp
#include <iostream>
using namespace std;
int main()
{
    string str("this is a c++");
    for(auto i = str.begin(); i!= str.end();++i)
        cout<<(*i)<<endl;
    system("pause");
    return 0;
}
```
输出
```
t
h
i
s
i
s
a
c
+
+
请按任意键继续. . .
```


#### 通过STL函数

使用STL函数，需要包含头文件。
```cpp
#include <iostream>
#include <algorithm>
#include <vector> 
using namespace std;
int main()
{
    string str("this is a c++");
    for_each(str.begin(),str.end(),[](char item)
             {
                 cout<<item << " ";
             });
    system("pause");
    return 0;
}
```
输出
```
t h i s   i s   a   c + + 请按任意键继续. . .
```
