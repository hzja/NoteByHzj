Java JNI
<a name="HnI2W"></a>
## 1、JNI概述
JNI 是 Java Native Interface 的简称

- Java是跨平台的编程语言，但是在有些时候仍然是有需要调用本地代码（这些代码通常是由 C 与 C++ 编写的）。
- JNI 是 Java 平台的一个功能强大的接口。这个 JNI接口提供了Java与操作系统本地代码相互调用的功能。
<a name="AGQKi"></a>
## 2、C/C++代码的步骤
<a name="XexB5"></a>
### 在 Java 类中声明一个`native`方法
```java
public native void sayHello();
```
<a name="JbuDL"></a>
### 使用 `javah` 命令生成包含 `native` 方法声明的C/C++头文件
```bash
javah com.omg.NativeLib
```
<a name="zbQEd"></a>
### 将生成的C/C++头文件来编写C/C++源文件
```c
#include<iostream>
#include "com_omg_NativeLib.h"
using namespace std;


JNIEXPORT void JNICALL Java_com_olive_NativeLib_sayHello (JNIEnv *, jobject){
    cout << "hello world" << endl;
}
```
<a name="AsWvt"></a>
### 将C/C++源文件编译成动态链接库（windows对应DLL，Linux对应so）
<a name="LVzrJ"></a>
### 把dll文件的路径添加到环境变量PATH中
<a name="zOxE1"></a>
### 在Java类中加载dll文件，然后调用声明的native方法
```java
System.load("C:\\lib\\NativeCodeJni.dll");
NativeLib nl = new NativeLib();
nl.sayHello();
```
或者
```java
System.loadLibrary("NativeCodeJni");
NativeLib nl = new NativeLib();
nl.sayHello();
```
<a name="TdVKT"></a>
## 3、本地代码调用Java代码
<a name="QEHKU"></a>
### 1. 本地代码访问Java代码

- 在被调用的C/C++函数中也可以反过来访问Java程序中的方法
- javah 工具生成的C/C++函数声明中，可以看到有两个参数:
```c
JNIEXPORT void JNICALL Java_com_omg_NativeLib_sayHello (JNIEnv *env, jobject jobj){
     cout << "hello world" << endl;
}
```
<a name="O1Acw"></a>
### 2. JNIEnv 类型

- JNIENV类型实际上代表了Java环境，通过这个`JNIEnv*`指针，就可以对Java端的代码进行操作。例如，创建Java类的对象、调用Java对象的方法、获取Java对象的属性等。JNIEnv的指针会被JNI传入到本地方法的实现函数中来对Java端的代码进行操作。
- `JNIEnv`类中有很多函数可以使用
```c
NewObject / NewString / New<TYPE>Array
Get<TYPE>Field / Set<TYPE>Field
GetStatic<TYPE>Field / SetStatic<TYPE>Field
Call<TYPE>Method / CallStatic<TYPE>Method
```
等许许多多的函数

- jobject 什么的一个类型呢？

不是静态方式时，传入的是当前关联的类对象<br />静态方法时，传入当前类的Class对象
<a name="H8zi2"></a>
### 3. Java的类型在C/C++中的映射关系
| Java类型 | 本地类型 | JNI定义的别名 |
| --- | --- | --- |
| int | long | jint / jsize |
| long | __int64 | jlong |
| byte | signed char | jbyte |
| boolean | unsigned char | jboolean |
| char | unsigned short | jchar |
| short | short | jshort |
| float | float | jfloat |
| double | double | jdouble |
| Object | _jobject* | jobject |

<a name="IiLVK"></a>
### 4. jclass如何获取

- 为了能够在C/C++中使用Java类。JNI.h头文件中，专门定义了jclass类型来表示Java中的Class类
- JNIEnv类中有以下几个函数可以获取jclass：
```c
jclass FindClass(const char* clsName);
jclass GetObjectClass(jobject ojb);
jclass GetSuperClass(jclass obj);
```
在Java JDK中也有这几个方法

- FindClass 将会在classpath环境变量下寻找类。传入完整类名，注意包与包之间是使用  “`/` ” ，而不是使用 “`.`”  来分隔
```c
jclass cls_str = env->FindClass("java/lang/String");
```
<a name="JVWA2"></a>
### 5. 访问Java类中的属性与方法

- 在C/C++本地代码中访问Java端的代码，一个常见的场景就是获取类的属性和调用类的方法。为了在C/C++中表示属性和方法，JNI在jni.h头文件中定义了jfieldID和jmethdID类型来分别代表Java端的属性和方法
- 在访问，或者设置Java属性的时候，首先就要先在本地代码中获得代表Java属性的jfieldID；然后才能在本地代码中进行Java的属性操作。同样的，在需要调用Java的方法时，也是需要先获取到代表该方法的jmethodID才能进行Java方法调用

使用JNIEnv的
```c
GetFieldID / GetMethodID
GetStaticFieldID / GetStaticMethodID
```
来取得相应的jfieldID和jmethodID<br />来看下方法定义
```c
jfieldID GetFieldID(jclass clazz, const char *name, const char *sig);

jmethodID GetMethodID(jclass clazz, const char *name, const char *sig);

jfieldID GetStaticFieldID(jclass clazz, const char *name, const char *sig);

jmethodID GetStaticMethodID(jclass clazz, const char *name, const char *sig);
```

- GetMethodID也能够获得构造函数的jmethodID。创建一个Java对象时可以调用指定的构造方法，这个将在后面向大家介绍

如：`env -> GetMethodID(data_Clazz, "<init>", "()V");`

- 类似Java的反射（`Reflect`），需要指定类跟属性/方法名来获取相应的jfieldID或者jmethodID。而sign又是什么呢？
<a name="D68QO"></a>
### 6. Sign是什么？
例如：TestNative类中有两个重载的方法
```java
package com.omg;

public class TestNative {
    
    public void function( int i ){
        System.out.println("Integer: " + i);
    }
    
    public void function( double d ){
        System.out.println("double: " + d);
    }
}
```
然后再C/C++代码中调用其中的一个function方法的话；
```c
//首先取得要调用的方法所在的类
jclass clazz_TestNative = env->FindClass("com/omg/TestNative");
//获取jmethodID之后才能进行调用
jmethodID id_func = env->GetMethodID(clazz_TestNative, "function", ??);
```
但是到底取得的是 `void function(int i)` 方法还是 `void function(double d)` 方法的jmethodID呢？<br />这就是`sign`的作用了，他用于指定要取得的属性/方法的类型。<br />这里的`sign`如果指定为"(I)V"，则获得的是 `void function(int i)` 方法的jmethodID；如果指定为"(D)V"，则获得的是 `void function(double d)` 方法的jmethodID。
<a name="hOhhE"></a>
### 7. sign签名
用来表示要取得的属性/方法的类型

| 类型 | 签名 |
| --- | --- |
| boolean | Z |
| byte | B |
| char | C |
| short | S |
| int | I |
| long | L |
| float | F |
| double | D |
| void | V |
| object | L + 用/分隔包的完整类名 + ; |
| Array | [ + 签名   例如，[I、[Ljava/lang/String; |
| Method | (参数1类型签名 参数2类型签名...)返回值类型签名 |

<a name="DYG6x"></a>
### 8. 使用签名获得属性/方法ID的例子（复杂）
```java
import java.util.Date;

public class HelloNative(){
    
    public int property;
    
    public int function(int foo, Date date, int[] arr){
        System.out.println("function");
    }
    
    public native void test();
}
```
test本地方法的实现
```c
JNIEXPORT void Java_Hello_test(JNIEnv* env, jboject ojb) {
    //因为test不是静态函数，所以传进来的就是调用这个函数的对象
    jclass hello_clazz = env->GetObjectClass(obj);
    jfieldID filedID_prop = env->GetFieldID(hello_clazz, "property", "I");
    jmethodID methodID_func = env->GetMethodID("hello_clazz", "function", "(ILjava/util/Date;[I)I");
    env->CallIntMethod(obj, methodID_func, 0L, NULL, NULL); //invoke
}
```
取得的property是int类型的，所以在签名中传入"`I`"<br />取得function的ID时<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1645955486933-f267060b-48a9-4a6b-b1e0-552a4091e1bf.png#clientId=ub19018c0-7dfc-4&from=paste&id=u5f5f6f28&originHeight=320&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uedb2f51f-1df2-4ced-a7a0-ad00e8eb312&title=)<br />所以在最后得到(ILjava/util/Date;[I)I
<a name="lSZCS"></a>
### 9. 使用javap命令来产生签名

- JDK提供了一个工具来查看一个类的申请，其中就可以设置输出每一个方法/属性的签名
- `java -s -p [full Calss Name]`

`-s` 表示输出签名信息<br />`-p` 同`-private`，输出包括`private`访问权限的成员信息
<a name="RyxTu"></a>
## 4、取得Java属性和设置Java属性值
<a name="YGgyK"></a>
### 1. 取得Java属性/设定Java属性值

- 获得了相应属性的jfieldID之后，就可以使用
```c
Set<TYPE>Field
Get<TYPE>Field
SetStatic<TYPE>Field
GetStatic<TYPT>Field
```
等函数来对Java属性进行操作了。

- 类实例方法
```c
Set/GetBooleanField
Set/GetByteField
Set/GetDoubleField
Set/GetFloatField
Set/GetIntField
Set/GetLongField
Set/GetShortField
Set/GetObjectField
```

- 类静态方法
```c
Set/GetStaticBooleanField
Set/GetStaticByteField
Set/GetStaticDoubleField
Set/GetStaticFloatField
Set/GetStaticIntField
Set/GetStaticLongField
Set/GetStaticShortField
Set/GetStaticObjectField
```

- 怎样获取数组的属性呢？

可以使用GetObjectField来获取数组类型的属性。不过JNI提供了一系列方法获取数组的属性，后续介绍。
<a name="xY1C3"></a>
#### 案例
在C/C++中修改Java中的属性
```java
package com.omg;
public class TestNative {
    public int number;
    public native void sayHello();
}
```
C++代码
```cpp
jclass clazz_TestNative = env->GetObjectClass( obj);
jfieldID id_number = env->GetFieldID(clazz_TestNative, "number", "I");
jint number = env->GetIntFiled(obj, id_number);
count << number << endl;
env->SetIntField(obj, id_number, 100L);
```
<a name="kGWlp"></a>
### 2. Java方法调用

- `JNIEnv`提供了众多的 `Call<TYPE>Method` 与 `CallStatic<TYPE>Method`，还有`CallNonvirtual<TYPE>Method`函数。需要通过GetMethodID取得相应方法的jmethodID来传入上述函数的参数中。
- 调用实例方法的三种形式
```cpp
Call<TYPE>Method( jobject obj, jmethodID id, ...);
Call<TYPE>MethodV( jobject obj, jmethodID id, va_list lst);
Call<TYPE>MethodA( jobject obj, jmethodID id, jvalue* v);
```

1. 第一种是最常用的方式
2. 第二种是当调用这个函数的时候，有一个指向参数表的va_list变量时使用（不常用）
3. 第三种是当调用这个函数的时候，有一个指向jvalue或者jvalue数组的指针时使用

调用静态方法的三种形式
```cpp
CallStatic<TYPE>Method( jclass clazz, jmethodID id, ...);
CallStatic<TYPE>MethodV( jclass clazz, jmethodID id, va_list lst);
CallStatic<TYPE>MethodA( jclass clazz, jmethodID id, jvalue* v);
```
伪代码<br />java：
```java
public boolean function(int i, double d, char c){
     //......
}
```
C
```cpp
env->CallBooleanMethod(obj, id_function, 100L, 1.11, L'3')

jvalue * args = new jvalue[3];
args[0].i = 100L;
args[1].d = 3.44;
args[2].c = L'3';
env->CallBooleanMethod( obj, id_function, args);
delete [] args;
```
<a name="xuipG"></a>
#### 案例
在C/C++中代用Java中的方法<br />Java代码
```java
package com.omg;

public class TestNative {
    public max(double num1, double num2){
        return num1 > num2 ? num1 : num2;
    }
}
```
C++代码
```cpp
jclass clazz_TestNative = env->GetObjectClass( obj );
jmethodID id_max = env->GetMetodID( clazz_TestNative, obj, "(DD)D");
jdouble maxVal = env->GetDoubleMethod(obj, id_max, 2.1, 3.14);
cout << maxVal << endl;
```
<a name="N9oAN"></a>
### 3. `CallNonvirtual<TYPE>Method`
有如下Java代码
```java

public class Father {

    public void function(){
            System.out.println("Father function");
        }
    }
  
   public class Child extends Father {
      public void function(){
        System.out.println("Child function");
    }

}
```
想想这段Java代码调用的是哪个类的方法？
```java
Father p = new Child();
p.function();
```
如果想调用父类的function是没有办法的<br />有如下C++代码
```cpp
class Father {

    public:
    void function(){
        cout << "Father function" <<endl;
    }

}

  class Child : public Father {
      public:
       void function(){
        cout << "Child function" <<endl;
    }

}
```
想想这段代码调用的是哪个类的成员函数？(调用的是父类的函数)
```cpp
Father* p = new Child();
p->function();
```
C++有虚函数的概念，修改一下父类加一个 virtual
```cpp
class Father {

    public:
    virtual void function(){
        cout << "Father function" <<endl;
   }

}

class Child : public Father {

    public:
    void function(){
          cout << "Child function" <<endl;
     }

}
```
再想想这段代码调用的是哪个类的成员函数？(调用的是子类的函数)
```cpp
Father* p = new Child();
p->function();
```
在Java语言中，所有的函数都是虚函数

- 在JNI中定义了`CallNonvirtual<TYPE>Method`就能实现子类对象调用父类方法的功能，如果想要调用一个对象的父类的方法，而不是子类的重写（over write）的方法的话，就可以使用`CallNonVirtual<TYPE>Method`
- 要使用`CallNonVirtual<TYPE>Method`，首先要取得父类及要调用的父类的方法的jmethodID。然后传入到这个函数就能通过子类对象调用被重写（over write）的父类的方法
<a name="E87gC"></a>
#### 案例
```java

package com.omg;

public class Father {
    public void function() {
       System.out.println("Father function");
    } 
}

package com.omg;

public class Child extends Father {

    public void function() {
        System.out.println("Child function");
    }
}
```
测试<br />Java定义成员变量
```java
public Father p = new Child();
```
C++代码
```cpp
jclass clazz_TestNative = env->GetObjectClass( obj );
jfieldID id_p = env->GetFieldID(clazz_TestNative, "p", "Lcom/omg/Father;");
jobject p = env->GetObjectField(obj, id_p);

jclass clazz_Father = env->FindClass( "Lcom/omg/Father;" );
jmethodID id_Father_function = env->GetMethodID(clazz_Father, "function", "()V");

env->CallVoidMethod(p, id_Father_function);

env->CallNonvirtualVoidMethod(p, clazz_Father, id_Father_function);
```
<a name="zXrag"></a>
## 5、使用JNI弊端

- 使用了 JNI，那么这个Java Application 将不能跨平台了。如果需要移植到别的平台上，那么需要重新编写`native`代码
- Java 是强类型的语言，而 C/C++并不是。因此，在编写JNI时要更小心
- 总之，必须在构建Java程序的时候，尽量少用本地代码
