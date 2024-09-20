# C++ 结构体 struct

## 结构体
1.定义结构体

2.定义结构体同时初始化结构体变量

3.定义无名称的结构体同时定义无名称的结构体变量；

### 结构体变量的定义
1.使用struct关键字声明结构体变量；<br />2.也可以不使用关键字直接声明结构体变量
```cpp
#include <iostream>
#include <string>

using namespace std;

// 结构的使用
struct person{
    string name;
    int age;
};

// 定义结构体时创建结构体变量以及赋值
struct student{
    string name;
} ky = {"ky"};

// 定义没有名称的结构体同时定义变量
struct {
    string name;
} brid;

// 定义结构体变量时可以省略结构体关键字struct
struct person p = {"str", 18};
person person1;

int main() {
    cout << p.name << endl;
    cout << ky.name << endl;
    return 0;
}
```

### 结构体中的位字段
C++也允许指定占用特定位数的结构成员，这使得创建与某个硬件设备上的寄存器对应的数据结构非常方便。字段的类型应为整型或枚举，接下来是冒号，冒号后面是一个数字，它指定了使用的位数。可以使用没有名称的字段来提供间距。每个成员都被称为位字段（bit field），位字段通常用在低级编程中。
```cpp
// 结构体中的位字段
//  C++也允许指定占用特定位数的结构成员，这使得创建与某个硬件设备上的寄存器对应的数据结构非常方便。
//  字段的类型应为整型或枚举，接下来是冒号，冒号后面是一个数字，它指定了使用的位数。可以使用没有名称的字段来提供间距
//  每个成员都被称为位字段（bit field）
// 位字段通常用在低级编程中
struct torgle_register{
    unsigned int SN : 4;
    unsigned int : 4;
    bool goodIn: 1;
    bool goodTorgle: 1;
};
```
## 共用体 union 的定义
```cpp
// 共用体union
//      共用体是一种数据格式，它能够存储不同的数据类型，但只能同时存储其中的一种类型。
//          结构体可以同时存储int、long和double
//          共用体只能存储int、long或double,条件是在不同的时间进行
//      用于共用体每次只能存储一个值，因此它必须有足够的空间来存储最大的成员，所以，共用体的长度为其最大成员的长度
//  共用体的用途:当数据项使用两种或更多格式（但不会同时使用）时，可节省空间。
//          例如，假设管理一个小商品目录，其中有一些商品的ID为整数，而另一些ID为字符串，这种情况下可以使用共用体
union one4all{
    int int_val;
    int long long_val;
    double double_val;
};
```
<a name="GSo2R"></a>
## 匿名共用体anonymous union
```cpp
//  匿名共用体anonymous union没有名称
//  其成员位于相同地址处的变量。每次只有一个成员是当前的成员
struct widget{
    char brand[20];
    int type;
    union {
        long id_num;
        char id_char[20];
    };
};
```
<a name="2stm5"></a>
## 枚举类型enum
### 枚举类型的定义及使用
```cpp
#include <iostream>
#include <string>

using namespace std;

// 枚举类型
//  枚举量：默认情况下会将整数值赋值给枚举量，第一个枚举量为0，第二个枚举量为1···，通过指定的整数值覆盖默认值
enum spectrum{
    red, orange, yellow, green, blue, violet, indigo, ultraviolet
};

spectrum band;
int color = 3 + band;

int main() {
    cout << color << endl;
    return 0;
}
```
<a name="EXQZC"></a>
### 设置枚举变量的值
```cpp
// 设置枚举变量的值
// 可以使用赋值操作来显式地设置枚举量的值
enum bits{
    one = 1,
    two = 2,
    four = 4,
    eight = 8
};

// 指定的值必须是整数，也可以只显式地定义其中一些枚举量的值
// 这里first在默认情况下为0，后面没有被初始化的枚举变量的值将比其前面的枚举量大1，因此third的值为101
enum bigstep{
    first, second = 100, third
};

// 创建多个值相同的枚举量
// 其中zero和null都为0，one_c和umero_uno都为1
enum {
    zero, null = 0, one_c, numero_nuo = 1
};
```
