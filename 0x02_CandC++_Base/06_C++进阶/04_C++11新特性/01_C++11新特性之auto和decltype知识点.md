# C++11 auto decltype

C++11引入了auto和decltype关键字，使用它们可以在编译期就推导出变量或者表达式的类型，方便开发者编码的同时也简化了代码。

## auto
auto可以让编译器在编译器就推导出变量的类型，看代码：
```cpp
auto a = 10; // 10是int型，可以自动推导出a是int
int i = 10;auto b = i; // b是int型
auto d = 2.0; // d是double型
```
这就是auto的基本用法，可以通过=右边的类型推导出变量的类型。

### auto推导规则
直接看代码

代码1：

```cpp
int i = 10;
auto a = i, &b = i, *c = &i; // a是int，b是i的引用，c是i的指针，auto就相当于int
auto d = 0, f = 1.0; // error，0和1.0类型不同，对于编译器有二义性，没法推导
auto e; // error，使用auto必须马上初始化，否则无法推导类型
```
代码2：
```cpp
void func(auto value) {} // error，auto不能用作函数参数

class A {
    auto a = 1; // error，在类中auto不能用作非静态成员变量
    static auto b = 1; // error，这里与auto无关，正常static int b = 1也不可以
    static const auto int c = 1; // ok
};

void func2() {
    int a[10] = {0};
    auto b = a; // ok
    auto c[10] = a; // error，auto不能定义数组，可以定义指针
    vector<int> d;
    vector<auto> f = d; // error，auto无法推导出模板参数
}
```
auto的限制：

- auto的使用必须马上初始化，否则无法推导出类型<br />
- auto在一行定义多个变量时，各个变量的推导不能产生二义性，否则编译失败<br />
- auto不能用作函数参数<br />
- 在类中auto不能用作非静态成员变量<br />
- auto不能定义数组，可以定义指针<br />
- auto无法推导出模板参数<br />

再看这段代码：
```cpp
int i = 0;
auto *a = &i; // a是int*
auto &b = i; // b是int&
auto c = b; // c是int，忽略了引用

const auto d = i; // d是const int
auto e = d; // e是int

const auto& f = e; // f是const int&
auto &g = f; // g是const int&
```
首先，介绍下，这里的cv是指const 和volatile<br />推导规则

- 在不声明为引用或指针时，auto会忽略等号右边的引用类型和cv限定<br />
- 在声明为引用或者指针时，auto会保留等号右边的引用和cv属性<br />
<a name="nWKqH"></a>
### 什么时候使用auto？
这里没有绝对答案，在不影响代码代码可读性的前提下尽可能使用auto是蛮好的，复杂类型就使用auto，int、double这种就没有必要使用auto了，看下面这段代码：
```cpp
auto func = [&] {
    cout << "xxx";
}; // 对于func难道不使用auto吗，反正是不关心lambda表达式究竟是什么类型。

auto asyncfunc = std::async(std::launch::async, func);
// 对于asyncfunc难道不使用auto吗，懒得写std::futurexxx等代码，而且也记不住它返回的究竟是什么...
```

## decltype
上面介绍auto用于推导变量类型，而decltype则用于推导表达式类型，这里只用于编译器分析表达式的类型，表达式实际不会进行运算，上代码：
```cpp
int func() { return 0; }
decltype(func()) i; // i为int类型

int x = 0;
decltype(x) y; // y是int类型
decltype(x + y) z; // z是int类型
```
:::warning
注意：decltype不会像auto一样忽略引用和cv属性，decltype会保留表达式的引用和cv属性
:::
```cpp
cont int &i = 1;
int a = 2;
decltype(i) b = 2; // b是const int&
```
### decltype推导规则
对于decltype(exp)有

- exp是表达式，decltype(exp)和exp类型相同
- exp是函数调用，decltype(exp)和函数返回值类型相同

- 其它情况，若exp是左值，decltype(exp)是exp类型的左值引用

```cpp
int a = 0, b = 0;
decltype(a + b) c = 0; // c是int，因为(a+b)返回一个右值
decltype(a += b) d = c;// d是int&，因为(a+=b)返回一个左值

d = 20;
cout << "c " << c << endl; // 输出c 20
```


## auto和decltype的配合使用

auto和decltype一般配合使用在推导函数返回值的类型问题上。

下面这段代码

```cpp
template<typename T, typename U>
return_value add(T t, U u) { // t和v类型不确定，无法推导出return_value类型
    return t + u;
}
```
上面代码由于t和u类型不确定，那如何推导出返回值类型呢，可能会想到这种
```cpp
template<typename T, typename U>
decltype(t + u) add(T t, U u) { // t和u尚未定义
    return t + u;
}
```
这段代码在C++11上是编译不过的，因为在decltype(t +u)推导时，t和u尚未定义，就会编译出错，所以有了下面的叫做返回类型后置的配合使用方法：
```cpp
template<typename T, typename U>
auto add(T t, U u) -> decltype(t + u) {
    return t + u;
}
```
返回值后置类型语法就是为了解决函数返回值类型依赖于参数但却难以确定返回值类型的问题。
