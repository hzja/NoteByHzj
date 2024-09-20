Java SpringBoot Junit5
<a name="60902f7f"></a>
### `@Test`
在Springboot2.0版本之后，Junit就是5.几的版本，在方法上添加`@Test`即可
```java
@Test
void fun1(){
    int res = 1+1;
    Assertions.assertEquals(2,res);
    System.out.println(123);
}
```
<a name="Es5xE"></a>
### `@BeforeEach` & `@AfterEach`
每执行一个`@Test`就会执行一次
```java
@BeforeEach
void before(){
    System.out.println("before");
}
@AfterEach
void after(){
    System.out.println("after");
}
@Test
void fun1(){
    int res = 1+1;
    Assertions.assertEquals(2,res);
    System.out.println(123);
}
@Test
void fun2(){
    System.out.println(456);
}
```
![](https://cdn.nlark.com/yuque/0/2021/png/396745/1615342525682-2eb88371-d2df-4525-a2e0-e45db6f8faf1.png#align=left&display=inline&height=206&originHeight=206&originWidth=182&size=0&status=done&style=none&width=182)
<a name="AppMq"></a>
### `@BeforeAll` & `@AfterAll`
无论有多少个`@Test`都只执行一次，且`@BeforeAll` & `AfterAll`下的方法必须是静态方法
```java
@BeforeAll
static void init(){
    System.out.println("init");
}
@AfterAll
static void destory(){
    System.out.println("destory");
}
@BeforeEach
void before(){
    System.out.println("before");
}
@AfterEach
void after(){
    System.out.println("after");
}
@Test
void fun1(){
    int res = 1+1;
    Assertions.assertEquals(2,res);
    System.out.println(123);
}
@Test
void fun2(){
    System.out.println(456);
}
```
![](https://cdn.nlark.com/yuque/0/2021/png/396745/1615342525555-eb131e92-9e6d-4614-a4d9-8ad414e896b1.png#align=left&display=inline&height=340&originHeight=340&originWidth=210&size=0&status=done&style=none&width=210)
<a name="CPwut"></a>
### `@SpringBootTest`
在SpringBoot项目中，在测试类中new 一个类时，这个类可能同时会涉及到其他的Bean，而原来的`@Test`并不会涉及到SpringBoot的上下文，所以在SpringBoot项目中编写测试类时，会在类上添加`@SpringBootTest`，在创建项目选择 Spring Initializr创建SpringBoot项目时，目录下会有一个Test文件，就会看到自动创建好的Test类上带有`@SpringBootTest`<br />同时也能配合`@Autowired`的注入
```java
@SpringBootTest  //能够初始化springboot的上下文，防止new Serv01 的对象同时依赖其他的Bean
public class Test2 {
    @Autowired
    Serv01 serv01;
    @Test
    void t1(){
        int add = serv01.add(1, 1);
        Assertions.assertEquals(2,add);
        System.out.println(11);
    }
}
```
当然，运行这个带`@SpringBootTest`的测试用例，也会启动SpringBoot项目
<a name="eUXjL"></a>
### `@MockBean`
通过mock来做到一个模拟，假如测试需要写入数据库，可能会造成一些不可挽回的操作，对这些危险的操作会使用mock来进行一个模拟<br />看下面的代码
```java
@SpringBootTest  //能够初始化springboot的上下文，防止new Serv01 的对象同时依赖其他的Bean
public class Test2 {
    @MockBean
    Serv01 serv01;
    @Test
    void t1(){
        int add = serv01.add(1, 1);
        Assertions.assertEquals(2,add);
        System.out.println(11);
    }
}
```
通过断点得知 变量add 等于0<br />原因就是在SpringBoot上下文的serv01的这个bean已经被mock替换掉了，因为mock并没有任何指定操作，所以int变量add 只会有默认值 0<br />对mock添加一些指定操作
```java
@SpringBootTest
public class Test2 {
    @MockBean
    Serv01 serv01;
    @Test
    void t1(){
        when(serv01.add(1,1)).thenReturn(3);
        int add = serv01.add(1, 1);
        Assertions.assertEquals(2,add);
        System.out.println(11);
    }
}
```

![](https://cdn.nlark.com/yuque/0/2021/png/396745/1615342525737-41bcf582-00f9-4903-b342-15277879ba3b.png#align=left&display=inline&height=183&originHeight=183&originWidth=681&size=0&status=done&style=none&width=681)<br />再次修改代码，在Serv01添加sub的方法，然后运行测试，可以发现在指定规则的add方法，得出的结果是3，而未指定规则的sub方式则默认int变量为0<br />如果想让sub按照原来的方式进行测试，不想被`@MockBean`影响，可以使用`@SpyBean`
<a name="Kfn7I"></a>
### `@SpyBean`
`@SpyBean`是介于`@MockBean`和`@Autowired`之间，在配置了规则的方法就按配置规则的执行，没有配置的就按原来的方法执行测试。如果再创建一个serv02，让serv01调用serv02的方法，运行的结果还是一样吗？
```java
@Service
public class Serv02 {
    int add2(int a, int b){
        return a+b;
    }
}
```
```java
@Service
public class Serv01 {
    @Autowired
    Serv02 serv02;
    public int add(int a,int b){
        return serv02.add2(a,b);
    }
    public int sub(int a,int b){
        return a-b;
    }
}
```
```java
@SpringBootTest  //能够初始化springboot的上下文，防止new Serv01 的对象同时依赖其他的Bean
public class Test2 {
    @SpyBean
    Serv01 serv01;
    @MockBean
    Serv02 serv02;
    @Test
    void t1(){
        when(serv01.add(1,1)).thenReturn(3);
        int add = serv01.add(1, 1);
        int sub = serv01.sub(2,1);
        Assertions.assertEquals(3,add);
        Assertions.assertEquals(1,sub);
        System.out.println(11);
    }
}
```
可以看到运行成功，即使mock的是serv02，通过serv01调用的add方法调用serv02的add2方法，照样运行成功！
