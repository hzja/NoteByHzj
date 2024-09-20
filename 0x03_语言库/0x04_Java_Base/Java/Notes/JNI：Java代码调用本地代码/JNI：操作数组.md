Java JNI
<a name="EIXRv"></a>
## 1、数组的分类

- 数组分为两种
   1. 基本数据类型的数组
   2. 对象类型（`Object[]`）的数组
- 一个能通用于两种不同数据类型数组的函数
```cpp
GetArrayLength( jarray array);//获取长度
```
<a name="z8AYd"></a>
## 2、基本数据类型数组

- 处理基本数据类型数组的时候，也是跟处理字符串类型，有很相似的函数
```cpp
Get<TYPE>ArrayElements( <TYPE>Array arr, jboolean* isCopied);
```
这类函数可以把Java基本类型的数组转换到C/C++的数组，有两种处理方式，一是拷贝一份传回本地代码，另一个是把指向Java数组的指针直接传回到本地代码。处理完本地的数组后，通过`Release<TYPE>ArrayElements`来释放数组。
```cpp
Release<TYPE>ArrayElements( <TYPE>Array arr, <TYPE>* array, jint mode );
```
用这个函数可以选择将如何处理Java跟C++的数组。是提交、还是撤销等；内存释放还是不释放等。<br />mode 可以取下面的值：

1. 0 对Java的数组进行更新，并释放 C/C++ 的数组
2. JNI_COMMIT 对Java的数组进行更新，但是不释放C/C++的数组
3. JNI_ABORT  对Java的数组不进行更新，释放C/C++的数组
- JDK 1.2 为了增加直接传回指向 Java 数组的指针，而增加了以下函数。但是同样的，也会有同`GetStringCritical`的死锁问题
```cpp
GetPrimitiveArrayCritical( jarray arr, jboolan* isCopied);ReleasePrimitiveArrayCritical(jarray arr, void array, jint mode);
```

- 在C/C++预先开辟一块内存，然后把Java基本数组类型的数组拷贝到这块内存当中。跟`GetStringRegion`原理相似
```cpp
Get<TYPE>ArrayRegion( <TYPE>Array arr, jsize, start, jsize len, <TYPE>* buffer);
```

- 把Java基本数据类型的数组中的指定范围的元素用 C/C++的数组中的元素来赋值
```cpp
Set<TYPE>ArrayRegion( <TYPE>Array arr, jsize start, jsize len, const <TYPE>* buffer );
```

- 指定一个长度，然后返回相应Java基本类型的数组
```cpp
<TYPE>Array New<TYPE>Array( jsize len );
```
<a name="Idh3Y"></a>
## 3、对象类型数组——`Object[]`
JNI 没有提供直接把Java的对象类型数组（ `Object[]` ）直接转到 C/C++ 的 `jobject[]` 数组的函数。而是直接通过`GetObjectArrayElement`/`SetObjectArrayElement`这样的函数来对Java的`Object[]`数组进行操作。
