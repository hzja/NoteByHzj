Java StringBuilder
<a name="ANjsA"></a>
## 分析
在分析设个问题之前要知道`StringBuilder`和`StringBuffer`的内部实现跟`String`类一样，都是通过一个`char`数组存储字符串的，不同的是`String`类里面的`char`数组是`final`修饰的，是不可变的，而`StringBuilder`和`StringBuffer`的`char`数组是可变的。<br />首先通过一段代码去看一下多线程操作`StringBuilder`对象会出现什么问题
```java
public class StringBuilderDemo {

    public static void main(String[] args) throws InterruptedException {
        StringBuilder stringBuilder = new StringBuilder();
        for (int i = 0; i < 10; i++){
            new Thread(new Runnable() {
                @Override
                public void run() {
                    for (int j = 0; j < 1000; j++){
                        stringBuilder.append("a");
                    }
                }
            }).start();
        }

        Thread.sleep(100);
        System.out.println(stringBuilder.length());
    }
}
```
能看到这段代码创建了10个线程，每个线程循环1000次往`StringBuilder`对象里面`append`字符。正常情况下代码应该输出10000，但是实际运行会输出什么呢？<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1623834313890-6f23f026-f7c7-4bb5-8dc8-fbd51000a1f4.webp#clientId=u9b7fca1f-bd3e-4&from=paste&id=u18cf5908&originHeight=213&originWidth=1080&originalType=url&ratio=3&status=done&style=none&taskId=ud05aa73e-086d-4c86-82ec-5e89ec965ff)<br />可以看到输出了“9326”，小于预期的10000，并且还抛出了一个`ArrayIndexOutOfBoundsException`异常（异常不是必现）。
<a name="X03xX"></a>
#### 1、为什么输出值跟预期值不一样
先看一下`StringBuilder`的两个成员变量（这两个成员变量实际上是定义在`AbstractStringBuilder`里面的，`StringBuilder`和`StringBuffer`都继承了`AbstractStringBuilder`）
```java
//存储字符串的具体内容
char[] value;
//已经使用的字符数组的数量
int count;
```
再看`StringBuilder`的`append()`方法：
```java
@Override
public StringBuilder append(String str) {
    super.append(str);
    return this;
}
```
`StringBuilder`的`append()`方法调用的父类`AbstractStringBuilder`的`append()`方法
```java
public AbstractStringBuilder append(String str) {
    if (str == null)
        return appendNull();
    int len = str.length();
    ensureCapacityInternal(count + len);
    str.getChars(0, len, value, count);
    count += len;
    return this;
}
```
先不管代码的第五行和第六行干了什么，直接看第七行，`count += len`不是一个原子操作。假设这个时候count值为10，len值为1，两个线程同时执行到了第七行，拿到的count值都是10，执行完加法运算后将结果赋值给count，所以两个线程执行完后count值为11，而不是12。这就是为什么测试代码输出的值要比10000小的原因。
<a name="VO4BU"></a>
#### 2、为什么会抛出`ArrayIndexOutOfBoundsException`异常。
看回`AbstractStringBuilder`的`append()`方法源码的第五行，`ensureCapacityInternal()`方法是检查`StringBuilder`对象的原char数组的容量能不能盛下新的字符串，如果盛不下就调用`expandCapacity()`方法对char数组进行扩容。
```java
private void ensureCapacityInternal(int minimumCapacity) {
        // overflow-conscious code
    if (minimumCapacity - value.length > 0)
        expandCapacity(minimumCapacity);
}
```
扩容的逻辑就是new一个新的char数组，新的char数组的容量是原来char数组的两倍再加2，再通过`System.arryCopy()`函数将原数组的内容复制到新数组，最后将指针指向新的char数组。
```java
void expandCapacity(int minimumCapacity) {
    //计算新的容量
    int newCapacity = value.length * 2 + 2;
    //中间省略了一些检查逻辑
    ...
    value = Arrays.copyOf(value, newCapacity);
}
```
`Arrys.copyOf()`方法
```java
public static char[] copyOf(char[] original, int newLength) {
    char[] copy = new char[newLength];
    //拷贝数组
    System.arraycopy(original, 0, copy, 0,
                         Math.min(original.length, newLength));
    return copy;
}
```
`AbstractStringBuilder`的`append()`方法源码的第六行，是将String对象里面char数组里面的内容拷贝到`StringBuilder`对象的char数组里面，代码如下：
```java
str.getChars(0, len, value, count);
```
`getChars()`方法
```java
public void getChars(int srcBegin, int srcEnd, char dst[], int dstBegin) {
    //中间省略了一些检查
    ...
    System.arraycopy(value, srcBegin, dst, dstBegin, srcEnd - srcBegin);
}
```
拷贝流程见下图<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1623834313726-76faee48-7ede-4ce7-ba92-c26b7ffafc67.webp#clientId=u9b7fca1f-bd3e-4&from=paste&id=u501a6e5a&originHeight=442&originWidth=1080&originalType=url&ratio=3&status=done&style=shadow&taskId=u83718773-d91a-4a17-b820-06d7b6ad2e6)<br />假设现在有两个线程同时执行了`StringBuilder`的`append()`方法，两个线程都执行完了第五行的`ensureCapacityInternal()`方法，此刻count=5。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1623834313661-d150ec62-0c9a-44a4-aeb0-0501baaed4c0.webp#clientId=u9b7fca1f-bd3e-4&from=paste&id=ud0ec3268&originHeight=575&originWidth=1080&originalType=url&ratio=3&status=done&style=shadow&taskId=uffe2c8dd-f286-4707-8c41-5175ba16b8d)<br />这个时候线程1的cpu时间片用完了，线程2继续执行。线程2执行完整个`append()`方法后count变成6了<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1623834313641-88f34c01-b4ba-4d17-b542-60fda960ad2f.webp#clientId=u9b7fca1f-bd3e-4&from=paste&id=u8922b1c1&originHeight=637&originWidth=1080&originalType=url&ratio=3&status=done&style=shadow&taskId=u24660462-cbad-4f20-9a37-534f23c0504)<br />线程1继续执行第六行的`str.getChars()`方法的时候拿到的`count`值就是6了，执行char数组拷贝的时候就会抛出`ArrayIndexOutOfBoundsException`异常。<br />至此，`StringBuilder`为什么不安全已经分析完了。如果将测试代码的`StringBuilder`对象换成`StringBuffer`对象会输出什么呢？<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1623834313782-4beefb25-5849-4d57-bbd3-671ab3147c90.webp#clientId=u9b7fca1f-bd3e-4&from=paste&id=u8311b760&originHeight=92&originWidth=1080&originalType=url&ratio=3&status=done&style=none&taskId=uc45487ec-8a61-45c6-8377-3230f324a4f)<br />当然是输出10000！<br />那么`StringBuffer`用什么手段保证线程安全的？这个问题点进`StringBuffer`的`append()`方法里面就知道了。
