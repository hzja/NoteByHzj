Java Spring
<a name="z8vtC"></a>
### `@Qualifier`的作用
这是官方的介绍
> This annotation may be used on a field or parameter as a qualifier for candidate beans when autowiring. It may also be used to annotate other custom annotations that can then in turn be used as qualifiers.

简单的理解就是：<br />（1）在使用`@Autowire`自动注入的时候，加上`@Qualifier("test")`可以指定注入哪个对象；<br />（2）可以作为筛选的限定符，在做自定义注解时可以在其定义上增加`@Qualifier`，用来筛选需要的对象。这个理解看下面的代码吧，不好解释。
<a name="WHFOJ"></a>
### 功能介绍
首先是对（1）的理解。
```java
//定义了两个TestClass对象，分别是testClass1和testClass2 
//如果在另外一个对象中直接使用@Autowire去注入的话，spring肯定不知道使用哪个对象 
//会排除异常 required a single bean, but 2 were found
@Configuration
public class TestConfiguration {
   @Bean("testClass1")
   TestClass testClass1(){
       return new TestClass("TestClass1");
   }
   @Bean("testClass2")
   TestClass testClass2(){
       return new TestClass("TestClass2");
   }
}
```
下面是正常的引用
```java
@RestController
public class TestController {

    //此时这两个注解的连用就类似 @Resource(name="testClass1")
    @Autowired
    @Qualifier("testClass1")
    private TestClass testClass;
    
    @GetMapping("/test")
    public Object test(){
        return testClassList;
    }

}
```
`@Autowired`和`@Qualifier`这两个注解的连用在这个位置就类似 `@Resource(name="testClass1")`<br />对(2)的理解
```java
@Configuration
public class TestConfiguration {
    //调整下在testClass1上增加@Qualifier注解
    @Qualifier
    @Bean("testClass1")
    TestClass testClass1(){
        return new TestClass("TestClass1");
    }

    @Bean("testClass2")
    TestClass testClass2(){
        return new TestClass("TestClass2");
    }
}

@RestController
public class TestController {
    //这里使用一个list去接收testClass的对象
    @Autowired
    List<TestClass> testClassList= Collections.emptyList();
    
    @GetMapping("/test")
    public Object test(){
        return testClassList;
    }
}
```
调用得到的结果是
```json
[
     {
        "name": "TestClass1"
     },
    {
       "name": "TestClass2"
    }
]
```
可以看到所有的testclass都获取到了。接下来修改下代码
```java
@RestController
public class TestController {

    @Qualifier //在这增加注解
    @Autowired
    List<TestClass> testClassList= Collections.emptyList();
    
    @GetMapping("/test")
    public Object test(){
        return testClassList;
    }
}
```
和上面代码对比就是在接收参数上增加了`@Qualifier`注解，这样看是有什么区别，调用下，结果如下：
```json
[
  {
    "name": "TestClass1"
  }
]
```
返回结果只剩下增加了`@Qualifier`注解的TestClass对象，这样就可以理解官方说的标记筛选是什么意思了。另外，`@Qualifier`注解是可以指定value的，这样可以通过values来分类筛选想要的对象了。
