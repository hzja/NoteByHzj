Java JNI
<a name="nCtfA"></a>
## 1、在C/C++本地代码中创建Java的对象
<a name="rqY8z"></a>
### 1.1 Java对象的创建使用 NewObject 方法

- 使用函数 NewObject 可以用来创建Java对象
- GetMethod 能够取得构造方法的 jmethodID，如果传入的要取的方法名称设定为 "`<init>`" 就能够取得构造方法
- 因为构造方法没有返回值，所以构造方法的方法返回值类型的签名始终为void

案例
```java
jclass class_date = env->FindClass("java/util/Date");
jmethodID mid_date = env->GetMethodId(class_date, "<init>", "()V");
jobject now = env->NewObject(class_date, mid_date);

jmethodID mid_date_getTime = env->GetMethod(class_date , "getTime", "()j");
jlong time = CallLongMethod(now, mid_date_getTime);

cout << time << endl;
```
<a name="Jipd9"></a>
### 1.2 Java对象的创建 AllocObject

- 使用函数 AllocObject 可以根据传入的 jclass 创建一个 Java 对象，但是他的状态是非初始化的，在使用这个对象之前一定要调用 CallNonvirtualVoidMethod 来调用该 jclass 的构造函数，这样就可以延迟构造函数的调用。这一个部分用的很少，在这里只做简单的说明。
```java
jclass clazz_str = env->FindClass("java/lang.String");
jmethodID methodID_str = env->GetMethodID(clazz_str, "<init>", "([C)V");

//预先创建一个没有初始化的字符串
jobject str = env->AllocObject(clazz_str);
//创建一个4个元素的字符串数组，然后以 'B' 'U' 'G' '弄' '潮' '儿' 赋值
jcharArray arg = env->NewCharArray(6);
env->SetCharArrayRegion(arg, 0, 6, L"BUG弄潮儿");
//调用构造函数
env->CallNonvirtualVoidMethod(str, clazz_str, methodID_str, arg);


jclass clazz_this = env->GetObjectClass( obj );
//这里假设这个对象的类中有定义 static String STATIC_STR;
jfieldID jfield_str = env->GetStaticField( clazz_this, "STATIC_STR", "Ljava/lang/String;");
env->SetStaticObjectField(clazz_str, jfield_str, str);
```
<a name="m3onJ"></a>
## 2、Java字符串 & C/C++的字符串
在C/C++本地代码中创建Java的String字符串对象

- 在Java中，使用的字符串String对象是 Unicode ( UTF-16 ) 编码，即每个字符不论是中文、英文还是符号，一个字符总是占两个字节
- Java 通过 JNI 接口可以将 Java的字符串转换到 C/C++ 中的宽字符串（wchar_t*），或者是传回一个UTF-8的字符串（char*）到 C/C++。反过来，C/C++ 可以通过一个宽字符串，或者是一个 UTF-8 编码的字符串来创建一个Java端的 String 对象
<a name="SZCeC"></a>
### 2.1  GetStringChars & GetStringUTFChars
```java
GetStringChars
GetStringUTFChars
```

- 这两个函数用来取得某个jstring对象相关的Java字符串。分别可以取得UTF-16编码的宽字符串（jchar*）与UTF8编码的字符串(char*)
```java
const jchar* GetStringChars(jstring str, jboolean* copied);
const char* GetStringUTFChars(jstring str, jboolean* copied);
```
第一个参数传入一个指向 Java 中的 String 字符串对象的 jstring 变量；<br />第二参数传入的是一个jboolean的指针；

- 这两个函数分别都会有两个不同的动作：
1. 开辟新内存，然后把Java中的String拷贝到这个内存中，然后返回指向这个内存地址的指针
2. 直接返回指向 Java 中 String 内存的指针。这个时候千万不要改变这个内存的内容；如果改变，那么这样将破坏String在Java中始终是常量这个原则
- 第二个参数是用来标识是否对Java的String对象进行了拷贝的

如果传入的这个jboolean指针不是NULL，则它会给该指针指向的内存传入JNI_TRUE或者JNI_FALSE表示是否进行了拷贝<br />传入NULL表示不关心是否拷贝字符串，它就不会给jboolean*指向的内存赋值

- 使用这两个函数取得的字符串，在不使用的时候，必须使用ReleaseStringChars / ReleaseStringUTFChars 来释放拷贝的内存，或者是否对Java的String对象的应用。
```java
ReleaseStringChars(jstring jstr, const jchar* str);
ReleaseStringUTFChars(jstring jstr, const char* str);
```
第一个参数指定一个 jstring 变量，即是要释放的本地字符串的来源；<br />第二个参数就是要释放的本地字符串；
<a name="xNPSQ"></a>
### 2.2 GetStringCritical

- 为了增加直接传入指向Java字符串的指针的可能性（而不是拷贝），JDK 1.2 增加了函数 GetStringCritical / ReleaseStringCritical
```java
const jchar* GetStringCritical( jstring str, jboolean* copied);
void ReleaseStringCritical( jstring jstr, const jchar* str);
```

- 在GetStringCritical /ReleaseStringCritical 之间是一个关键区，在这个关键区之间绝对不能呼叫 JNI 的其他函数和会造成当前线程中断或是会让当前线程等待的任何本地代码。否则将造成关键区代码执行期间垃圾回收器停止运作，任何触发垃圾回收器的线程也会暂停。其它的触发垃圾回收器的线程不能前进直到当前线程结束而激活垃圾回收器
- 在关键区中千万不要出现中断操作，或是在JVM中分配任何新对象；否则会造成JVM死锁
- 虽说这个函数会增加直接传回指向Java字符串的指针的可能性，不过还是会根据情况传回拷贝过的字符串
- 不支持GetStringUTFCritical，没有这样的函数。由于Java字符串用的是UTF16编码，要转换成UTF8编码的字符串始终需要进行一次拷贝，所以没有这样的函数。
<a name="NDOcd"></a>
### 2.3 `GetStringRegin` & `GetStringUTFRegion`

- Java 1.2 增加了 GetStringRegin / GetStringUTFRegion 函数。这个函数的动作，是把Java字符串的内容直接拷贝到 C/C++ 的字符数组中。在调用这个函数之前必须有一个 C/C++ 分配出来的字符串，然后传入到这个函数中进行字符串的拷贝
- 由于 C/C++ 中分配内存开销相对小，而且Java中的String内容拷贝的开销可以忽略，更好的一点是此函数不分配内存，不会抛出OutOfMemoryError 异常
```java
//拷贝Java字符串并以UTF-8编码传入buffer
GetStringUTFRegion( jstring srt, jsize start, jsize len, char* buffer);

//拷贝Java字符串并以UTF-16编码传入buffer
GetStringRegion( jstring srt, jsize start, jsize len, char* buffer);
```
![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1646135747802-74a52be3-71d8-46f8-8de8-ff1e3873ce2a.jpeg)
<a name="FDCBy"></a>
### 2.4 其他字符串函数
```java
jstring NewString( const jchar* str, jsize len);
jstring NewStringUTF(const char* str);
jsize GetStringLength(jstring str);
jsize GetStringUTFLength(jstring str);
```
