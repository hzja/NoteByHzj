# C++ const auto

<a name="FSJmg"></a>

## const限定符
<a name="KpfR4"></a>
### 1、限定常量
有时希望定义这样的变量：它的值不能被改变。为了满足这一要求，使用const对变量的类型加以限定：
```cpp
const int bufSize = 512;
```
这样就把`bufSize`定义成了一个常量，它的值不能再发生变化。所以这也就意味着const对象必须要初始化。那么初始化操作对于const对象来说意味着什么呢？如果利用一个对象去初始化另外一个对象，则它们是不是const都无关紧要：
```cpp
int i = 42;
const int ci = i;  //正确
int j = ci;  //正确
```
这里要注意的是，虽然使用常量ci去初始化j，但是拷贝一个对象的值并不会改变它，一旦完成拷贝，新的对象就和原来的对象没有什么关系了。
<a name="c67ff87f"></a>
### 2、const对象仅在文件内有效
默认情况下，const对象被限定仅在文件内有效。当多个文件中出现了同名的const变量时，其实等同于在不同文件中分别定义了独立的变量。**但是，如果需要将const变量在多个文件中实现共享该怎么办呢？**要做到在一个文件中定义const，而在其它多个文件中声明并且使用它，解决的办法就是：对于const变量不管是声明还是定义都添加extern关键字，这样只需要定义一次就够了。
```cpp
//第一个文件file1
extern const int bufSize = fcn();
//第二个文件file2
extern const int bufSize;  //与file1里面的是同一个
```
<a name="rqte7"></a>
## const的引用
对常量的引用不能被用作修改它所绑定的对象：
```cpp
const int ci = 1024;
const int &r1 = ci;  //正确，引用及其对应的对象都是常量
r1 = 42;    //错误，r1是对常量的引用
int &r2 = ci;    //错误，r2是非常量引用，ci是一个常量
```
引用的类型必须与其所引用的对象的类型一致，但是有两个特殊情况：<br />（1）初始化常量引用时允许用任意表达式作为初始值，只要该表达式的结果能转换成引用的类型即可。
```cpp
double dval = 3.14;
const int &ri = dval;
```
ri引用了一个int型的数，但是dval确实一个双精度浮点数，编译器会将其变成如下形式：
```cpp
const int temp = dval;
const int &ri = temp;
```
（2）对const的引用可能引用一个并非const的对象
```cpp
int i = 42;
const int &ri = i;
```
这种方式是允许的。
<a name="LbNIq"></a>
## const与指针
顶层const<br />顶层const表示指针本身是一个常量，不能改变它的指向。
```cpp
int i = 43;
int * const p1 = &i;
```
底层const表示指针所指的对象是一个常量。
```cpp
int i = 43;
const int * p1 = &i;
int const * p2 = &i;
```
<a name="pwB18"></a>
## 常量表达式与constexpr
常量表达式指的是**值不会改变且在编译过程中就能得到计算结果的表达式**：
```cpp
const int max_file = 20;    //是常量表达式
const int limit = max_file + 1    //limit是常量表达式
int stuff_size = 10;      //stuff_size不是常量表达式
const int sz = getSize();     //sz不是常量表达式
```
C++11标准规定，允许将变量声明为constexpr类型以便由编译器来验证变量的值是否是一个常量表达式。声明为constexpr的变量一定是一个常量，而且必须用常量表达式初始化：
```cpp
constexpr int mf = 20;  //  正确
constexpr int limit = mf + 1;    //mf+1是常量表达式
constexpr int sz = size();      //只有当size()是一个constexpr函数时才是一条正确的语句
```
需要注意的是，在constexpr声明中如果定义了一个指针，限定符constexpr仅针对指针有效，与所指的对象无关。
<a name="KNKBl"></a>
## auto类型说明符
C++11引入auto类型说明符，它可以让编译器通过初始值来推断变量的类型。显然，auto定义的变量必须有初始值。但是，auto会忽略掉顶层const：
```cpp
const int ci = i, &cr = ci;
auto b = ci;     //b是一个整数
auto c = cr;    //c是一个整数
auto d = &i； //d是一个整型指针
auto e = &ci；  //e是一个指向整型常量的指针（这是底层const）
```
但是，有时会遇到一种情况：如果想知道这个表达式结果的类型，但是又不想真的用这个表达式的结果来初始化变量。这个时候就要用到另外一个**类型指示符decltype**
```cpp
decltype(f()) sum = x;    //sum的类型就是f()函数的返回类型
```
编译器并不会实际调用函数`f()`，而是使用当调用发生时f的返回值类型作为sum的类型。decltype的一个坑：对于decltype所用的表达式来说，如果decltype给变量加上括号，则返回的是引用类型：
```cpp
int i = 42;
decltype((i)) d;    //错误，d是int&，必须初始化
```
