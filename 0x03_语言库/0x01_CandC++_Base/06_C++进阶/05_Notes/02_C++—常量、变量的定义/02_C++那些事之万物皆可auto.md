## C++那些事之万物皆可auto
C++17引入了许多旨在提高代码表达性并减少模板代码冗余的功能。其中一个显著的增强是使用auto关键字声明非类型模板参数的能力。在某些场景，这个特性特别方便，因为它消除了显式类型声明的需要。以下将会引入在工作中，也就是数据库当中如何使用这一特性！
## 在C++17中使用auto的非类型模板参数
### 在数据库场景中进行排序
考虑一个与数据库相关的类Foo，希望根据枚举表示的特定顺序进行排序：
```cpp
enum class SortOrder { Ascending, Descending };
```
有不同排序顺序的Foo实例：
```cpp
Foo<SortOrder::Ascending> f1;
Foo<SortOrder::Descending> f2;
```
<a name="I726n"></a>
### 以前的实现方式
在C++17之前，可能会这样实现：
```cpp
template <typename U, template <U> class T, U K>
auto GetSortOrder(const T<K>&) {
  return K;
}
```
然而，这可能会导致错误，因为编译器无法推断模板参数'U'。
<a name="Hz7Mk"></a>
### 现在的简化方式
在C++17之后，对于非类型模板参数，可以使用`auto`关键字来取代：
```cpp
template <template <auto> class T, auto K>
auto GetSortOrder(const T<K>&) {
  return K;
}
```
这种方式更为简便，无需显式指定类型即可使用非类型模板参数。
<a name="FW5C4"></a>
### `integral_constant`的简化
以前，可能使用以下方式实现`integral_constant`：
```cpp
template <typename _Tp, _Tp __v>
struct integral_constant {}
```
而现在只需：
```cpp
template <auto __v>
struct integral_constant {}
```
这种简化使得代码更为清晰和易读。
<a name="MgOL2"></a>
## 结论
C++17引入的`auto`关键字用于非类型模板参数的声明极大地简化了模板代码。通过消除繁琐的类型声明，能够更加直观地表达代码意图，提高代码的可读性和可维护性。在处理排序等场景时，这一特性尤为有用，更专注于问题的本质，而不是模板参数的细节。
