Java函数式接口<br />在开发过程中经常会使用if...else...进行判断抛出异常、分支处理等操作。这些if...else...充斥在代码中严重影响了代码代码的美观，这时可以利用Java 8的Function接口来消灭if...else...。
```java
if (...){
	throw new RuntimeException("出现异常了")；
} 

if (...){
	doSomething();
} else {
	doOther();
}
```
<a name="k30mP"></a>
## Function函数式接口
使用注解`@FunctionalInterface`标识，并且只包含一个抽象方法的接口是函数式接口。函数式接口主要分为Supplier供给型函数、Consumer消费型函数、Runnable无参无返回型函数和Function有参有返回型函数。<br />Function可以看作转换型函数。
<a name="FliQB"></a>
### Supplier供给型函数
Supplier的表现形式为不接受参数、只返回数据。<br />![Supplier函数](https://cdn.nlark.com/yuque/0/2022/png/396745/1653630551485-2d6e64ef-05f4-4285-9b46-c920216ed648.png#clientId=u0411e914-86e0-4&from=paste&id=u130c345c&originHeight=700&originWidth=830&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=u05a91a42-25b9-4459-a820-403e73882fa&title=Supplier%E5%87%BD%E6%95%B0 "Supplier函数")
<a name="GzLMe"></a>
### Consumer消费型函数
Consumer消费型函数和Supplier刚好相反。Consumer接收一个参数，没有返回值。<br />![Consumer消费型函数](https://cdn.nlark.com/yuque/0/2022/png/396745/1653630551541-297f6d37-2c8a-4f48-94bf-b6f065ac5b67.png#clientId=u0411e914-86e0-4&from=paste&id=u2a45e9c7&originHeight=992&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=ub2d1086e-054b-4c14-b2b2-723196e6fe6&title=Consumer%E6%B6%88%E8%B4%B9%E5%9E%8B%E5%87%BD%E6%95%B0 "Consumer消费型函数")
<a name="ahx3D"></a>
### Runnable无参无返回型函数
Runnable的表现形式为即没有参数也没有返回值。<br />![Runnable无参无返回型函数](https://cdn.nlark.com/yuque/0/2022/png/396745/1653630551481-6999fcdd-ab42-410f-a88b-e380f05935d2.png#clientId=u0411e914-86e0-4&from=paste&id=u2fa1770c&originHeight=868&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=u002d3e2a-fe36-4940-94fe-8c923026fd8&title=Runnable%E6%97%A0%E5%8F%82%E6%97%A0%E8%BF%94%E5%9B%9E%E5%9E%8B%E5%87%BD%E6%95%B0 "Runnable无参无返回型函数")
<a name="jlANw"></a>
### Function函数的表现形式为接收一个参数，并返回一个值。Supplier、Consumer和Runnable可以看作Function的一种特殊表现形式。
![@FunctionalInterface](https://cdn.nlark.com/yuque/0/2022/png/396745/1653630551544-57086e90-861b-464f-a206-93b35564eafd.png#clientId=u0411e914-86e0-4&from=paste&id=u967f20fe&originHeight=760&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=u22ecce0c-5430-44c1-b421-1e47235085a&title=%40FunctionalInterface "@FunctionalInterface")
<a name="zr4S6"></a>
## 使用小技巧
<a name="N9Ngl"></a>
### 处理抛出异常的if
<a name="DSwTF"></a>
#### 1、定义函数
定义一个抛出异常的形式的函数式接口，这个接口只有参数没有返回值是个消费型接口。
```java
/**
 * 抛异常接口
 **/
@FunctionalInterface
public interface ThrowExceptionFunction {

    /**
     * 抛出异常信息
     *
     * @param message 异常信息
     * @return void
     **/
    void throwMessage(String message);
}
```
<a name="ttx0B"></a>
#### 2、编写判断方法
创建工具类`VUtils`并创建一个`isTure`方法，方法的返回值为刚才定义的函数式接口-`ThrowExceptionFunction`。`ThrowExceptionFunction`的接口实现逻辑为当参数b为`true`时抛出异常
```java
/**
 *  如果参数为true抛出异常
 * 
 * @param b 
 * @return com.example.demo.func.ThrowExceptionFunction
 **/
public static ThrowExceptionFunction isTure(boolean b){

    return (errorMessage) -> {
        if (b){
            throw new RuntimeException(errorMessage);
        }
    };
}
```
<a name="oXlPT"></a>
#### 3、使用方式
调用工具类参数参数后，调用函数式接口的throwMessage方法传入异常信息。当出入的参数为false时正常执行。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1653630551882-d7ccd36f-939b-4dc7-8238-8f4787c35531.png#clientId=u0411e914-86e0-4&from=paste&id=ud5b918bf&originHeight=600&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u5a025c93-1caf-40db-bb3b-1f80a4e6362&title=)<br />当出入的参数为true时抛出异常。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1653630551930-c01c2b77-e4f7-4836-9a4a-af308f398a19.png#clientId=u0411e914-86e0-4&from=paste&id=u27473031&originHeight=448&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ue51c1740-9c54-4955-96fa-83740f402e1&title=)
<a name="QBKOH"></a>
### 处理if分支操作
<a name="akYct"></a>
#### 1、定义函数式接口
创建一个名为`BranchHandle`的函数式接口，接口的参数为两个`Runnable`接口。这两个两个`Runnable`接口分别代表了为true或false时要进行的操作
```java
/**
 * 分支处理接口
 **/
@FunctionalInterface
public interface BranchHandle {

    /**
     * 分支操作
     *
     * @param trueHandle 为true时要进行的操作
     * @param falseHandle 为false时要进行的操作
     * @return void
     **/
    void trueOrFalseHandle(Runnable trueHandle, Runnable falseHandle);

}
```
<a name="a9gdM"></a>
#### 2、编写判断方法
创建一个名为`isTureOrFalse`的方法，方法的返回值为刚才定义的函数式接口-`BranchHandle`。
```java
/**
 * 参数为true或false时，分别进行不同的操作 
 * 
 * @param b 
 * @return com.example.demo.func.BranchHandle     
 **/
public static BranchHandle isTureOrFalse(boolean b){
    
    return (trueHandle, falseHandle) -> {
        if (b){
            trueHandle.run();
        } else {
            falseHandle.run();
        }
    };
}
```
<a name="wqLUS"></a>
#### 3、使用方式
参数为true时，执行trueHandle。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1653630551999-b0cc10bb-db39-4ed5-aeff-079e08548288.png#clientId=u0411e914-86e0-4&from=paste&id=u60b90232&originHeight=851&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ue315f2fb-4c57-45cd-bbdb-a14059fbf8f&title=)<br />参数为false时，执行falseHandle。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1653630552127-ac220818-f17e-4a80-aba2-fb41b4e102f8.png#clientId=u0411e914-86e0-4&from=paste&id=u169f7420&originHeight=697&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u630b3f99-1eec-466e-9747-bd6640fbb97&title=)
<a name="dwfGU"></a>
### 如果存在值执行消费操作，否则执行基于空的操作
<a name="pEath"></a>
#### 1、定义函数
创建一个名为`PresentOrElseHandler`的函数式接口，接口的参数一个为`Consumer`接口。一个为`Runnable`，分别代表值不为空时执行消费操作和值为空时执行的其他操作
```java
/**
 * 空值与非空值分支处理
 */
public interface PresentOrElseHandler<T extends Object> {

    /**
     * 值不为空时执行消费操作
     * 值为空时执行其他的操作
     * 
     * @param action 值不为空时，执行的消费操作
     * @param emptyAction 值为空时，执行的操作
     * @return void    
     **/
   void presentOrElseHandle(Consumer<? super T> action, Runnable emptyAction);
   
}
```
<a name="izGZC"></a>
#### 2、编写判断方法
创建一个名为`isBlankOrNoBlank`的方法，方法的返回值为刚才定义的函数式接口-`PresentOrElseHandler`。
```java
/**
 * 参数为true或false时，分别进行不同的操作
 *
 * @param b
 * @return com.example.demo.func.BranchHandle
 **/
public static PresentOrElseHandler<?> isBlankOrNoBlank(String str){

    return (consumer, runnable) -> {
        if (str == null || str.length() == 0){
            runnable.run();
        } else {
            consumer.accept(str);
        }
    };
}
```
<a name="Kx6c5"></a>
#### 3、使用方式
调用工具类参数参数后，调用函数式接口的`presentOrElseHandle`方法传入一个Consumer和Runnable。<br />参数不为空时，打印参数。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1653630552213-e3cea1f9-fd23-405c-93fa-4fce7a4f08af.png#clientId=u0411e914-86e0-4&from=paste&id=u3875628b&originHeight=541&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u6bde2b7f-1b8d-4914-9395-2b910140108&title=)<br />参数为空时。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1653630552257-322a6445-b3bc-44f7-9614-98540543af9c.png#clientId=u0411e914-86e0-4&from=paste&id=ua0b43743&originHeight=585&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u3b48d73f-40f9-4a1e-a281-faad6cc1b17&title=)
<a name="P4aUv"></a>
## 结尾
Function函数式接口是java 8非常重要的特性，利用好Function函数可以极大的简化代码。
