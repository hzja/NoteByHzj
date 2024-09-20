# C++ Lambda

Lambda函数是 Modern C++在C++11中的一个体现 。
<a name="q1kSY"></a>

## 什么是lambda函数？
本文标题有点误导。因为 lambda并不总是函数指针，它一个表达式。但是为了简单起见，一直都称它为函数。<br />Lambda函数是一段简短的代码片段，它具有以下特点：<br />（1）不值得命名（无名，匿名，可处理等，无论您如何称呼）<br />（2）不会重复使用<br />换句话说，它只是语法糖。lambda函数的语法定义为：
```cpp
[ capture list ] (parameters) -> return-type 
{  
    method definition
}
```
通常， 编译器会评估`lambda`函数本身的返回类型。因此，不需要显式指定返回类型，即 `-> return-type`，但是在某些复杂的情况下，编译器无法推断出返回类型，此时需要指定返回类型。
<a name="qfQiK"></a>
## 为什么要使用lambda函数？
 C++包括许多有用的通用函数，例如 std::for_each，通常情况下方便了使用。但是，当需要考虑特殊需求的时候，就比较头疼了。如下代码所示：
```cpp
struct print
{
    void operator()(int element)
{
        cout <<element << endl;
    }
};
int main(void)
{
    std::vector<int> v = {1, 2, 3, 4, 5};
    std::for_each(v.begin(),v.end(), print());
    return 0;
}
```
如果只在某个特定位置使用一次print，却写了一个类。这样的操作代价似乎太大了。<br />但是，对于这种情况，内联代码将更合适，并且可以通过如下的lambda函数来实现：
```cpp
std::for_each(v.begin(), v.end(), [](int element) { cout <<element << endl; });
```
<a name="ISSnp"></a>
## lambda函数工作原理
```cpp
[&i] ( ) { std::cout << i; }

// is equivalent to

struct anonymous
{
    int &m_i;
    anonymous(int &i) :m_i(i) {}
    inline autooperator()()const
{
        std::cout << i;
    }
};
```
如代码所示，编译器为每一个lambda函数生成独特的闭合。捕获列表将成为闭包中的构造函数参数，如果按值捕获，则会在闭包中创建相应类型的数据成员。此外，可以在lambda函数参数中声明变量/对象，它将成为调用运算符的参数，即operator()。
<a name="B9aVl"></a>
## 使用Lambda函数的好处
（1）零成本抽象。lambda不会牺牲性能，运行速度和普通函数一样快。<br />（2）此外，代码会变得更加紧凑，结构层次更加明显和代码可读性更佳。
<a name="bC2Mk"></a>
## 学习lambda表达式
<a name="J2IFv"></a>
### 1.通过引用/值捕获
```cpp
int main()
{
    int x = 100, y = 200;
    auto print = [&]{ // Capturing object by reference
        std::cout <<__PRETTY_FUNCTION__ << " : " << x<< " , " << y << std::endl;
    };
    print();
    return 0;
}
```
输出：
```
main()::<lambda()> : 100 , 200
```
在上面的示例中，在捕获列表中用到了 `&`。通过引用的方式捕获变量 `x`和 `y`。同样，`=` 表示按值捕获，这将在闭包内创建相同类型的数据成员，同时赋上相同的值。需要注意的是，参数列表是可选的， 如果不将参数传递给lambda表达式，则可以省略空括号。
<a name="aec91b1c"></a>
### 2.Lambda捕获列表
<a name="9f0d91f3"></a>
#### 2.1 将lambda作为参数传递
```cpp
template <typename Functor>
void f(Functor functor)
{
    std::cout <<__PRETTY_FUNCTION__ << std::endl;
}
/* Or alternatively you can use this
void f(std::function<int(int)> functor)
{
    std::cout <<__PRETTY_FUNCTION__ << std::endl;
}
*/
int g() { static int i = 0; return i++; }
int main()
{
    auto lambda_func =[i = 0]() mutable { return i++; };
    f(lambda_func); // Pass lambda
    f(g);           // Pass function
}
```
输出：
```
Function Type : void f(Functor) [with Functor = main()::<lambda(int)>]
Function Type : void f(Functor) [with Functor = int (*)(int)]
```
也可以将`lambda`函数作为参数传递给其他函数，就像上面编写的普通函数一样。在捕获列表中声明了变量`i`，它将成为数据成员。所以，每次调用`lambda_func`时，它将被返回并递增。
<a name="OIehn"></a>
#### 2.2lambda捕获this指针或成员变量
```cpp
class Example
{
public:
    Example() : m_var(10) {}
    void func()
{
        [=]() { std::cout << m_var<< std::endl; }(); // IIFE
    }
private:
    int m_var;
};
int main()
{
    Example e;
    e.func();
}
```
捕获`this`指针也可以使用 `[this]`， `[=]`或者 `[&]`。在上述任何情况下，类内数据成员（包括 private）的访问方式与常规方法一样。<br />可以看到`lambda`表达式的末尾，多写了一个 ( )，该函数通常在声明之后立刻对其进行调用。它称为IIFE（立即调用函数表达式）。
<a name="PNyAQ"></a>
## C++ lambda函数类型
<a name="NYiwT"></a>
### 1.通用lambda
```cpp
const auto l = [](auto a, auto b, auto c) {};
// is equivalent to
struct anonymous
{
    template <class T0, class T1, class T2>
    auto operator()(T0 a, T1 b, T2 c) const
    {
    }
};
```
C++ 14中引入的通用`lambda`可以使用`auto`说明符。
<a name="HLC2o"></a>
### 2.可变参数通用λ
```cpp
void print() {}
template <typename First, typename... Rest>
void print(const First &first, Rest &&... args)
{
    std::cout << first<< std::endl;
    print(args...);
}
int main()
{
    auto variadic_generic_lambda = [](auto... param) {
        print(param...);
    };
    variadic_generic_lambda(1, "lol", 1.1);
}
```
具有可变参数的`Lambda`在许多情况下非常有用，例如调试，使用不同的数据输入重复操作等。
<a name="MkAaE"></a>
### 3.mutable lambda函数
通常，lambda的函数调用运算符是const-by-value，这意味着lambda需要捕获可变值的 关键字时，需要使用`mutable`关键字。
```cpp
[]() mutable {}

// is equivalentto

struct anonymous
{
    auto operator()()  // call operator
{
    }
};
```
<a name="x8YZ9"></a>
### 4.Lambda作为函数指针
```cpp
#include<iostream>
#include<type_traits>

int main()
{
      auto funcPtr = +[] {};
      static_assert(std::is_same<decltype(funcPtr), void (*)()>::value);
}
```
可以通过添加命令“+”来强制编译器将`lambda`生成为函数指针而不是闭包。
<a name="1UpWA"></a>
### 5.lambda函数作为返回值
```cpp
const auto less_than = [](auto x) {
    return [x](auto y) {
        return y < x;
    };
};

int main(void)
{
    auto less_than_five = less_than(5);
    std::cout << less_than_five(3) << std::endl;
    std::cout << less_than_five(10) << std::endl;
    return 0;
}
```
再进一步，`lambda`函数还可以返回另一个`lambda`函数。这也为代码的自定义性，代码可读性和紧凑性带来无限可能。
<a name="d2023bc7"></a>
### 6.constexpr lambda表达式
从C ++ 17开始，可以将`lambda`表达式声明为 `constexpr`。
```cpp
constexpr auto sum = [](const auto &a, const auto &b) { return a + b; };
/*
    is equivalent to
    constexpr struct anonymous
    {
        template <class T1, class T2>
        constexpr auto operator()(T1 a, T2 b)const
        {
            return a + b;
        }
    };
*/
constexpr int answer = sum(10, 10);
```
即使没有指定 `constexpr`，如果它恰好满足所有 `constexpr`函数的要求，那么它也会被声明为`constexpr`。
