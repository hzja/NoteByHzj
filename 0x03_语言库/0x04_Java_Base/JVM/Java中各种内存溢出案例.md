Java<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1624335784209-310b9955-58ab-4d1a-b0f3-241437f0a7c6.webp#clientId=u9a2317b2-1adb-4&from=paste&id=ue51d18ca&originHeight=469&originWidth=725&originalType=url&ratio=3&status=done&style=none&taskId=u2a1a2c16-b775-4ffd-8dd0-fea8e964737)
<a name="SjrgV"></a>
## 定义主类结构
首先，创建一个名称为 `BlowUpJVM` 的类，之后所有的案例实验都是基于这个类进行，如下所示。
```java
public class BlowUpJVM {  
} 
```
<a name="WnUXG"></a>
## 栈深度溢出
```java
public static void  testStackOverFlow(){ 
    BlowUpJVM.testStackOverFlow(); 
} 
```
栈不断递归，而且没有处理，所以虚拟机栈就不断深入不断深入，栈深度就这样溢出了。
<a name="omjSv"></a>
## 永久代内存溢出
```java
public static void testPergemOutOfMemory1(){ 
    //方法一失败 
    List<String> list = new ArrayList<String>(); 
    while(true){ 
        list.add(UUID.randomUUID().toString().intern()); 
    } 
} 
```
打算把 String 常量池堆满，没想到失败了，JDK1.7 后常量池放到了堆里，也能进行垃圾回收了。<br />然后换种方式，使用 cglib，用 Class 把老年代堆满。
```java
public static void testPergemOutOfMemory2(){ 
    try { 
        while (true) { 
            Enhancer enhancer = new Enhancer(); 
            enhancer.setSuperclass(OOM.class); 
            enhancer.setUseCache(false); 
            enhancer.setCallback(new MethodInterceptor() { 
                @Override 
                public Object intercept(Object obj, Method method, Object[] args, MethodProxy proxy) throws Throwable { 
                    return proxy.invokeSuper(obj, args); 
                } 
            }); 
            enhancer.create(); 
        } 
    } 
    catch (Exception e){ 
        e.printStackTrace(); 
    } 
} 
```
虚拟机成功内存溢出了，那 JDK 动态代理产生的类能不能溢出呢？
```java
public static void testPergemOutOfMemory3(){ 
    while(true){ 
        final OOM oom = new OOM(); 
        Proxy.newProxyInstance(oom.getClass().getClassLoader(), oom.getClass().getInterfaces(), new InvocationHandler() { 
            public Object invoke(Object proxy, Method method, Object[] args) throws Throwable { 
                Object result = method.invoke(oom, args); 
                return result; 
            } 
        }); 
    } 
} 
```
事实表明，JDK 动态代理产生的类不会造成内存溢出，原因是：JDK 动态代理产生的类信息，不会放到永久代中，而是放在堆中。
<a name="rtXSz"></a>
## 本地方法栈溢出
```java
public static void testNativeMethodOutOfMemory(){ 
    int j = 0; 
    while(true){ 
        Printer.println(j++); 
        ExecutorService executors = Executors.newFixedThreadPool(50); 
        int i=0; 
        while(i++<10){ 
            executors.submit(new Runnable() { 
                public void run() { 
                } 
            }); 
        } 
    } 
} 
```
这个的原理就是不断创建线程池，而每个线程池都创建 10 个线程，这些线程池都是在本地方法区的，久而久之，本地方法区就溢出了。
<a name="sYzLz"></a>
## JVM栈内存溢出
```java
public static void testStackOutOfMemory(){ 
    while (true) {   
        Thread thread = new Thread(new Runnable() {   
            public void run() { 
                while(true){ 
                } 
            }   
        });   
        thread.start();   
    }   
} 
```
线程的创建会直接在 JVM 栈中创建，但是本例子中，没看到内存溢出，主机先挂了，不是 JVM 挂了，真的是主机挂了，无论在 mac 还是在 windows，都挂了。
:::danger
温馨提示，这个真的会死机的。
:::
<a name="LvZ4n"></a>
## 堆溢出
```java
public static void testOutOfHeapMemory(){ 
    List<StringBuffer> list = new ArrayList<StringBuffer>(); 
    while(true){ 
        StringBuffer B = new StringBuffer(); 
        for(int i = 0 ; i < 10000 ; i++){ 
            B.append(i); 
        } 
        list.add(B); 
    } 
} 
```
不断往堆中塞新增的 `StringBuffer` 对象，堆满了就直接溢出了。
<a name="JJ1V7"></a>
## 测试案例完整代码
```java
public class BlowUpJVM {
    //栈深度溢出
    public static void  testStackOverFlow(){ 
        BlowUpJVM.testStackOverFlow(); 
    } 

    //不能引起永久代溢出
    public static void testPergemOutOfMemory1(){ 
        //方法一失败 
        List<String> list = new ArrayList<String>(); 
        while(true){ 
            list.add(UUID.randomUUID().toString().intern()); 
        } 
    } 

    //永久代溢出
    public static void testPergemOutOfMemory2(){ 
        try { 
            while (true) { 
                Enhancer enhancer = new Enhancer(); 
                enhancer.setSuperclass(OOM.class); 
                enhancer.setUseCache(false); 
                enhancer.setCallback(new MethodInterceptor() { 
                    @Override 
                    public Object intercept(Object obj, Method method, Object[] args, MethodProxy proxy) throws Throwable { 
                        return proxy.invokeSuper(obj, args); 
                    } 
                }); 
                enhancer.create(); 
            } 
        } 
        catch (Exception e){ 
            e.printStackTrace(); 
        } 
    } 

    //不会引起永久代溢出
    public static void testPergemOutOfMemory3(){ 
        while(true){ 
            final OOM oom = new OOM(); 
            Proxy.newProxyInstance(oom.getClass().getClassLoader(), oom.getClass().getInterfaces(), new InvocationHandler() { 
                public Object invoke(Object proxy, Method method, Object[] args) throws Throwable { 
                    Object result = method.invoke(oom, args); 
                    return result; 
                } 
            }); 
        } 
    } 

    //本地方法栈溢出
    public static void testNativeMethodOutOfMemory(){ 
        int j = 0; 
        while(true){ 
            Printer.println(j++); 
            ExecutorService executors = Executors.newFixedThreadPool(50); 
            int i=0; 
            while(i++<10){ 
                executors.submit(new Runnable() { 
                    public void run() { 
                    } 
                }); 
            } 
        } 
    } 

    //JVM内存溢出
    public static void testStackOutOfMemory(){ 
        while (true) {   
            Thread thread = new Thread(new Runnable() {   
                public void run() { 
                    while(true){ 
                    } 
                }   
            });   
            thread.start();   
        }   
    } 

    //堆溢出
    public static void testOutOfHeapMemory(){ 
        List<StringBuffer> list = new ArrayList<StringBuffer>(); 
        while(true){ 
            StringBuffer B = new StringBuffer(); 
            for(int i = 0 ; i < 10000 ; i++){ 
                B.append(i); 
            } 
            list.add(B); 
        } 
    } 
} 
```
