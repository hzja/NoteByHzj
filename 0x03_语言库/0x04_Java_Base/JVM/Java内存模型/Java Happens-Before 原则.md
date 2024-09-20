Java<br />Happens-Before原则核心就是表达在一些条件下，前面的操作对于后面的操作是可见的。它有六个条件，或者说是六条原则。
<a name="rucOg"></a>
### 一、线程中的顺序性原则
这个最容易理解，这个原则是指在同一个线程中，按照程序的顺序，前面的操作Happens-Before后面的操作。也就说在同一个线程中，前面先修改的数据，对于后面的操作是可见的。下面举例说明：
```java
int a = 1;
if(a == 1){

}
```
上面代码中第一行对变量a进行的修改Happens-Before第二行查询a的操作，所以第二行`a == 1`为true。
<a name="jqCXQ"></a>
### 二、volatile变量原则
这个原则是指在对于用`volatile`修饰的变量，它的写操作Happens-Before后续对它的读操作。这个原则就是说在一个线程中先对`volatile`变量进行了修改后，在另一个线程中读取该`volatile`变量能够读到新值（因为CPU缓存的存在，可能在一个线程中修改了普通变量，另一个线程无法感知）。当然如果在同一个线程中，就可以直接使用原则一了。
```java
volatile int a = 1;

// 线程A
a = 2;

// 线程B
if(a == 2){

}
```
上面代码中，假设有两个线程并发执行，线程A先对a进行赋值操作（第四行），然后线程B立刻读取a（第七行），此时能够保证读取的a一定是2。
<a name="mJIBY"></a>
### 三、传递性
这个原则是指Happens-Before原则是可以传递的，指如果A Happens-Before B，且B Happens-Before C，那么A Happens-Before C。
```java
volatile boolean a = false;
int b = 1;

// 线程A
b = 2;
a = true;

// 线程B
if(a){
    System.out.println(b);
}
```
上面代码中，线程A先对b进行赋值，然后对a进行赋值，然后线程B读取a后，然后执行打印b的值（根据原则二a一定为true），b的值一定为2。因为根据原则一，对b赋值Happens-Before对a赋值，再根据原则二，对a的赋值Happens-Before对a的读取，最后再根据原则三，对b的赋值Happens-Before对b的读取的，所以b一定为2。
<a name="ESJRj"></a>
### 四、管程中锁的原则
这个原则是指对同一个锁的解锁Happens-Before后续对这个锁的加锁。
```java
int a = 1;

// 线程执行
synchronized (this){
    if(a == 1){
        a = 2;
    }
}
```
上面代码中，线程A先对a进行赋值，然后解锁，线程B再加锁后，此时读到的a一定为2。
<a name="SW6P1"></a>
### 五、线程`start`原则
这条原则是关于线程启动的。它是指主线程A启动子线程B后，子线程B能够看到主线程在启动子线程B前的操作。也就是说如果线程A调用线程B的 `start()` 方法（即在线程A中启动线程B），那么该`start()`操作 Happens-Before 于线程B中的任意操作。
```java
int a = 1;
Thread B = new Thread(() -> {
    // 读取a
});
a = 2; // 假设能修改
B.start();
```
上面代码中，线程A先对a进行赋值，然后再启动线程B，所以线程B如果能够读取a的话（当然int类型，java中是不允许的），那么a一定为2。
<a name="WER5p"></a>
### 六、线程join原则
这条是关于线程等待的。它是指主线程A等待子线程B完成（主线程A通过调用子线程B的`join()`方法实现），当子线程B完成后（主线程A中`join()`方法返回），主线程能够看到子线程的操作。
```java
int a = 1;
Thread B = new Thread(() -> {
    // 修改a
    a = 2;  // 假设能修改
});
B.start();
B.join();
// 读取a
```
上面代码中，读到的a一定为2。也就是说线程B中的操作对于`B.join()`后面的操作都是可见的。
