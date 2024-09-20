Java JNI<br />从Java虚拟机创建的对象传到本地 C/C++ 代码时就会产生引用。根据Java的垃圾回收机制，只要有引用存在就不会触发该引用指向的Java对象的垃圾回收。这些引用在 JNI 中分为三种

1. 全局引用 （Global Reference）
2. 局部引用 （Local Reference）
3. 弱全局引用 （Weak Global Reference）， JDK 1.2 引入
<a name="VC3jU"></a>
## 1、局部引用

- 最常见的引用类型，基本上通过JNI返回来的引用都是局部引用

例如，使用NewObject就会返回创建出来的实例的局部引用。局部引用只在该native函数中有效，所有在该函数中产生的局部引用，都会在函数返回的时候自动释放（freed）。也可以使用DeleteLocalRef函数进行手动释放该引用。

- 想一想既然局部引用能够在函数返回时自动释放，为什么还需要DeleteLocalRef函数呢？

实际上局部引用存在，就会防止其指向的对象被垃圾回收。尤其是当一个局部引用指向一个很庞大的对象，或是在一个循环中生成了局部应用；最好的做法就是在使用完该对象后，或在该循环尾部把这个引用释放掉，以确保在垃圾回收器被触发的时候被回收。

- 在局部引用的有效期中，可以传递到别的本地函数中，要强调的是它的有效期仍然只在一次的Java本地函数调用中，所以千万不能用C++全局变量保存它或是把它定义为C++静态局部变量。
<a name="Vf0OU"></a>
## 2、全局引用

- 全局引用可以跨越当前线程，在多个native函数中有效，不过需要编程人员手动来释放该引用。全局引用存在期间会防止在Java的垃圾回收的回收。
- 与局部引用不同，全局引用的创建不是由 JNI 自动创建的，全局引用需要调用 NewGlobalRef 函数，而释放它需要使用 ReleaseGlobalRef 函数。
<a name="sBOV2"></a>
## 3、弱全局引用
弱全局应用是 JDK 1.2 新出来的功能，与全局引用相似，创建跟释放都需要由编程人员来进行操作。这种引用与全局引用一样可以在多个本地代码有效，也可以跨越多线程有效；不一样的是，这种引用将不会阻止垃圾回收器回收这个引用所指向的对象。<br />使用 NewWeakGlobalRef 跟 ReleaseWeakGlobalRef 来产生和释放应用。
<a name="X8Hkh"></a>
## 4、关于引用的一些函数
```java
jobject NewGlabalRef(jobject obj);
jobject NewLocalRef(jobject obj);
jobject NewWeakGlobalRef(jobject obj);

void DeleteGlobalRef(jobject obj);
void DeleteLocalRef(jobject obj);
jboolean IsSameObject(jobject obj1, jobject obj2);
```
IsSameObject 函数对于弱引用全局应用还有一个特别的功能，把NULL传入要比较的对象中，就能够判断弱全局引用所指向的Java对象是否被回收。
<a name="aexX4"></a>
## 5、缓存jfieldID / jmethodID
获取 jfieldID与jmethodID 的时候会通过该属性/方法名称加上签名来查询相应的 jfieldID/jmethodID。这种查询相对来说开销较大。在开发中可以将这些 FieldID/MethodID 缓存起来，这样就只需要查询一次，以后就使用缓存起来的 FieldID/MethodID。

- 下面介绍两种缓存方式
1. 在使用时缓存 （Caching at the Point of Use）
2. 在Java类初始化时缓存 （Caching at the Defining Class's Inititalizer）
<a name="vc9DA"></a>
### 5.1 在使用时缓存
在native 代码中使用static局部变量来保存已经查询过的jfieldID/jmethodID ，这样就不会在每次的函数调用时查询，而只要一次查询成功后就保存起来了。
```java
JNIEXPORT void JNICALL Java_Test_native( JNIEnv* env, jobject ojb) {
static jfieldID fieldID_str = NULL;
   jclass clazz = env->GetObjectClass( obj );
   if(fieldID_str == NULL){
       fieldID_str = env->GetFieldID(clazz, "strField", "Ljava/lang/String");
   }
     //TODO Other codes
}
```
不过这种情况下，就不得不考虑多线程同时调用此函数时可能导致同时查询的并发问题，不过这种情况是无害的，因为查询同一个属性或者方法的ID，通常返回的值是一样的。
<a name="EM4vf"></a>
### 5.2 在Java类初始化时缓存

- 更好的一个方式就是在任何native函数调用之前把id全部缓存起来。
- 可以让Java在第一次加载这个类的时候，首先调用本地代码初始化所有的 jfieldID/jmethodID，这样的话就可以省去多次判断id是否存在的冗余代码。当然，这些 jfieldID/jmethodID 是定义在C/C++ 的全局。
- 使用这种方式还有好处，当Java类卸载或者重新加载的时候，也会重新调用该本地代码来重新计算IDs。

java代码
```java
public class TestNative {
	
	static {
		initNativeIDs();
	}
	
	static native void initNativeIDs();
	
	int propInt =0;
	
	String propStr = "";
	
	public native void otherNative();
	
	//TODO Other codes
}
```
C/C++ 代码
```cpp
//global variables
jfieldID g_propInt_id = 0;
jfieldID g_propStr_id = 0;


JNIEXPORT void JNICALL Java_TestNative_initNativeIDs( JNIEnv* env, jobject clazz){
	g_propInt_id = env->GetFieldID(clazz, "propInt", "I");
	g_propStr_id = env->GetFieldID(clazz, "propStr", "Ljava/lang/String;");    
}


JNIEXPORT void JNICALL Java_TestNative_otherNative( JNIEnv* env, jobject obj){
	// TODO get field with g_propInt_id/g_propStr_id
}
```
<a name="JF89m"></a>
## 6、总结

- 最简单的Java调用C/C++函数的方法
- 获取方法/属性的ID；学会了获取/设置属性；还有Java函数的调用
- Java/C++之间的字符串的转换问题
- 在C/C++下如何操作Java的数组
- 三种引用方式
- 如何缓存属性/方法的ID
<a name="eoPU9"></a>
## 7、回顾

- 使用了JNI，那么这个Java应用将不能跨平台了。如果要移植到别的平台上，那么native代码就需要重新进行编写
- Java是强类型的语言，而C/C++不是。因此，必须在写JNI时倍加小心
- 总之，必须在构建Java程序的时候，尽量不用或者少用本地代码
