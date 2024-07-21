# C++11 Lambda表达式 std::function

c++11新增了`std::function`、`std::bind`、`lambda`表达式等封装使函数调用更加方便。

## `std::function`
讲`std::function`前首先需要了解下什么是可调用对象

满足以下条件之一就可称为可调用对象：

- 是一个函数指针
- 是一个具有`operator()`成员函数的类对象(传说中的仿函数)，lambda表达式
- 是一个可被转换为函数指针的类对象
- 是一个类成员(函数)指针
- bind表达式或其它函数对象

而`std::function`就是上面这种可调用对象的封装器，可以把`std::function`看做一个函数对象，用于表示函数这个抽象概念。`std::function`的实例可以存储、复制和调用任何可调用对象，存储的可调用对象称为`std::function`的目标，若`std::function`不含目标，则称它为空，调用空的`std::function`的目标会抛出`std::bad_function_call`异常。<br />使用参考如下实例代码：
```cpp
std::function<void(int)> f; // 这里表示function的对象f的参数是int，返回值是void
#include <functional>
#include <iostream>

struct Foo {
    Foo(int num) : num_(num) {}
    void print_add(int i) const { std::cout << num_ + i << '\n'; }
    int num_;
};

void print_num(int i) { std::cout << i << '\n'; }

struct PrintNum {
    void operator()(int i) const { std::cout << i << '\n'; }
};

int main() {
    // 存储自由函数
    std::function<void(int)> f_display = print_num;
    f_display(-9);

    // 存储 lambda
    std::function<void()> f_display_42 = []() { print_num(42); };
    f_display_42();

    // 存储到 std::bind 调用的结果
    std::function<void()> f_display_31337 = std::bind(print_num, 31337);
    f_display_31337();

    // 存储到成员函数的调用
    std::function<void(const Foo&, int)> f_add_display = &Foo::print_add;
    const Foo foo(314159);
    f_add_display(foo, 1);
    f_add_display(314159, 1);

    // 存储到数据成员访问器的调用
    std::function<int(Foo const&)> f_num = &Foo::num_;
    std::cout << "num_: " << f_num(foo) << '\n';

    // 存储到成员函数及对象的调用
    using std::placeholders::_1;
    std::function<void(int)> f_add_display2 = std::bind(&Foo::print_add, foo, _1);
    f_add_display2(2);

    // 存储到成员函数和对象指针的调用
    std::function<void(int)> f_add_display3 = std::bind(&Foo::print_add, &foo, _1);
    f_add_display3(3);

    // 存储到函数对象的调用
    std::function<void(int)> f_display_obj = PrintNum();
    f_display_obj(18);
}
```
从上面可以看到`std::function`的使用方法，当给`std::function`填入合适的参数表和返回值后，它就变成了可以容纳所有这一类调用方式的函数封装器。`std::function`还可以用作回调函数，或者在C++里如果需要使用回调那就一定要使用`std::function`，特别方便。

## `std::bind`
使用`std::bind`可以将可调用对象和参数一起绑定，绑定后的结果使用`std::function`进行保存，并延迟调用到任何需要的时候。

`std::bind`通常有两大作用：

- 将可调用对象与参数一起绑定为另一个`std::function`供调用
- 将n元可调用对象转成m(m < n)元可调用对象，绑定一部分参数，这里需要使用`std::placeholders`

具体示例：
```cpp
#include <functional>
#include <iostream>
#include <memory>

void f(int n1, int n2, int n3, const int& n4, int n5) {
    std::cout << n1 << ' ' << n2 << ' ' << n3 << ' ' << n4 << ' ' << n5 << std::endl;
}

int g(int n1) { return n1; }

struct Foo {
    void print_sum(int n1, int n2) { std::cout << n1 + n2 << std::endl; }
    int data = 10;
};

int main() {
    using namespace std::placeholders;  // 针对 _1, _2, _3...

    // 演示参数重排序和按引用传递
    int n = 7;
    // （ _1 与 _2 来自 std::placeholders ，并表示将来会传递给 f1 的参数）
    auto f1 = std::bind(f, _2, 42, _1, std::cref(n), n);
    n = 10;
    f1(1, 2, 1001);  // 1 为 _1 所绑定， 2 为 _2 所绑定，不使用 1001
    // 进行到 f(2, 42, 1, n, 7) 的调用

    // 嵌套 bind 子表达式共享占位符
    auto f2 = std::bind(f, _3, std::bind(g, _3), _3, 4, 5);
    f2(10, 11, 12);  // 进行到 f(12, g(12), 12, 4, 5); 的调用

    // 绑定指向成员函数指针
    Foo foo;
    auto f3 = std::bind(&Foo::print_sum, &foo, 95, _1);
    f3(5);

    // 绑定指向数据成员指针
    auto f4 = std::bind(&Foo::data, _1);
    std::cout << f4(foo) << std::endl;

    // 智能指针亦能用于调用被引用对象的成员
    std::cout << f4(std::make_shared<Foo>(foo)) << std::endl;
}
```


## `lambda`表达式

lambda表达式可以说是c++11引用的最重要的特性之一，它定义了一个匿名函数，可以捕获一定范围的变量在函数内部使用，一般有如下语法形式：
```cpp
auto func = [capture] (params) opt -> ret { func_body; };
```
其中`func`是可以当作`lambda`表达式的名字，作为一个函数使用，`capture`是捕获列表，`params`是参数表，`opt`是函数选项(mutable之类)， ret是返回值类型，func_body是函数体。<br />一个完整的lambda表达式：
```cpp
auto func1 = [](int a) -> int { return a + 1; };
auto func2 = [](int a) { return a + 2; };
cout << func1(1) << " " << func2(2) << endl;
```
如上代码，很多时候lambda表达式返回值是很明显的，c++11允许省略表达式的返回值定义。<br />`lambda`表达式允许捕获一定范围内的变量：

- `[]`不捕获任何变量
- `[&]`引用捕获，捕获外部作用域所有变量，在函数体内当作引用使用
- `[=]`值捕获，捕获外部作用域所有变量，在函数内内有个副本使用
- `[=, &a]`值捕获外部作用域所有变量，按引用捕获a变量
- `[a]`只值捕获a变量，不捕获其它变量
- `[this]`捕获当前类中的this指针

lambda表达式示例代码：
```cpp
int a = 0;
auto f1 = [=](){ return a; }; // 值捕获a
cout << f1() << endl;

auto f2 = [=]() { return a++; }; // 修改按值捕获的外部变量，error
auto f3 = [=]() mutable { return a++; };
```
代码中的f2是编译不过的，因为修改了按值捕获的外部变量，其实lambda表达式就相当于是一个仿函数，仿函数是一个有`operator()`成员函数的类对象，这个`operator()`默认是`const`的，所以不能修改成员变量，而加了`mutable`，就是去掉`const`属性。

还可以使用lambda表达式自定义stl的规则，例如自定义sort排序规则：

```cpp
struct A {
    int a;
    int b;
};

int main() {
    vector<A> vec;
    std::sort(vec.begin(), vec.end(), [](const A &left, const A &right) { return left.a < right.a; });
}
```

## 总结
`std::function`和`std::bind`在平时编程过程中封装函数更加的方便，而lambda表达式将这种方便发挥到了极致，可以在需要的时间就地定义匿名函数，不再需要定义类或者函数等，在自定义STL规则时候也非常方便，让代码更简洁，更灵活，提高开发效率。
