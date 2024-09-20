JavaSpring
<a name="Ikd0k"></a>
## junit用法详解
<a name="EUbnN"></a>
### 使用步骤
1、添加junit Mave配置，这里用4.13，也可以用最新的
```xml
<dependency>
  <groupId>junit</groupId>
  <artifactId>junit</artifactId>
  <version>4.13</version>
</dependency>
```
2、写测试用例，在写好的测试方法上面添加`@Test`注解，比如需要对max方法进行测试，通常会新建一个测试类，类名为被测试的类加上Test后缀，即：MathUtilsTest，然后在这个类需要写max方法的测试方法，如下，需要在max方法上面加上`@Test`注解
```java
package com.javacode2018.junit.demo1;

import org.junit.Assert;
import org.junit.Test;

public class MathUtilsTest {
    @Test
    public void max() throws Exception {
        int result = MathUtils.max(1, 2, 3);
        //判断测试结果和期望的结果是否一致
        Assert.assertEquals(result, 3);
    }
}
```
3、运行测试用例，现在测试代码都写好了，下面写个类来启动测试用例，这里需要使用`JUnitCore.runClasses`方法来运行测试用例，如下：
```java
package com.javacode2018.junit.demo1;

import org.junit.runner.JUnitCore;
import org.junit.runner.Result;
import org.junit.runner.notification.Failure;

public class Demo1TestRunner {
    public static void main(String[] args) {
        //使用JUnitCore.runClasses方法传入测试用例的类，然后获取测试用例的运行结果
        Result result = JUnitCore.runClasses(MathUtilsTest.class);
        //获取失败的用例
        for (Failure failure : result.getFailures()) {
            System.out.println(failure);
        }
        //获取所有测试用例是否执行成功
        System.out.println(result.wasSuccessful());
    }
}
```
<a name="G0l6o"></a>
### 同时运行多个测试用例
可以一个测试类中写多个测试方法，每个方法上加上`@Test`注解就可以了，然后通过`JUnitCore`来执行就可以，下面代码中写2个方法对`MathUtils`中的max和min方法都进行测试，故意将执行结果和期望结果搞成不一致的，运行下面代码，然后看看运行结果。
```java
package com.javacode2018.junit.demo2;

import com.javacode2018.junit.demo1.MathUtils;
import org.junit.Assert;
import org.junit.Test;
import org.junit.runner.JUnitCore;
import org.junit.runner.Result;
import org.junit.runner.manipulation.Ordering;
import org.junit.runner.notification.Failure;

public class MathUtilsTest2 {
    @Test
    public void max() throws Exception {
        int result = MathUtils.max(1, 2, 3);
        //判断测试结果和期望的结果是否一致
        Assert.assertEquals(result, 1);
    }

    @Test
    public void min() throws Exception {
        int result = MathUtils.min(1, 2, 3);
        //判断测试结果和期望的结果是否一致
        Assert.assertEquals(result, 3);
    }

    public static void main(String[] args) {
        Result result = JUnitCore.runClasses(MathUtilsTest2.class);

        System.out.println("失败用例个数:" + result.getFailures().size());
        for (Failure failure : result.getFailures()) {
            System.out.println(failure);
        }
        System.out.println("运行测试用例个数:" + result.getRunCount());
        System.out.println("运行测试用例总耗时(ms):" + result.getRunTime());
        System.out.println("测试用例是否都成功了：" + result.wasSuccessful());
    }
}
```
运行输出如下，运行了2个用例，失败了2个，测试的详细信息都被输出了
```
失败用例个数:2
max(com.javacode2018.junit.demo2.MathUtilsTest2): expected:<3> but was:<1>
min(com.javacode2018.junit.demo2.MathUtilsTest2): expected:<1> but was:<3>
运行测试用例个数:2
运行测试用例总耗时(ms):11
测试用例是否都成功了：false
```
<a name="eH5Yp"></a>
### 使用断言
什么是断言？<br />断言是用来判断程序的运行结果和期望的结果是不是一致的，如果不一致，会抛出异常，断言中有3个信息比较关键<br />1、被测试的数据<br />2、期望的数据<br />3、抛出异常<br />断言提供的方法将被测试的数据和期望的数据进行对比，如果不一样的时候，将抛出异常，程序可以捕获这个异常，这样就可以知道测试失败了。<br />junit中的**org.junit.Assert**类中提供了大量静态方法，用来判断被测试的数据和期望的数据是否一致，不一致，将抛出异常，这里随便列几个大家看一下吧
```java
//判断condition如果不是true，将抛出异常，异常的提示信息是message
public static void assertTrue(String message, boolean condition) 

//判断expected和actual是否相等，如果不相等，将抛出异常
public static void assertEquals(Object expected, Object actual)
```
用法，如：
```java
int result = MathUtils.max(1, 2, 3);
//判断测试结果和期望的结果是否一致
Assert.assertEquals(result, 1);
```
<a name="EfZNN"></a>
### 测试套件：批量运行测试用例
到目前为止，还只能一次运行一个测试类，如下
```java
JUnitCore.runClasses(MathUtilsTest2.class)
```
但是在实际项目中，可能会有很多测试类，需要批量运行。<br />比如有下面2个测试类<br />MathUtilsTest3001.java
```java
package com.javacode2018.junit.demo3;

import com.javacode2018.junit.demo1.MathUtils;
import org.junit.Assert;
import org.junit.Test;

public class MathUtilsTest3001 {
    @Test
    public void max() throws Exception {
        int result = MathUtils.max(1, 2, 3);
        //判断测试结果和期望的结果是否一致
        Assert.assertEquals(result, 3);
    }

    @Test
    public void min() throws Exception {
        int result = MathUtils.min(1, 2, 3);
        //判断测试结果和期望的结果是否一致
        Assert.assertEquals(result, 1);
    }
}
```
MathUtilsTest3002.java
```java
package com.javacode2018.junit.demo3;

import com.javacode2018.junit.demo1.MathUtils;
import org.junit.Assert;
import org.junit.Test;
import org.junit.runner.JUnitCore;
import org.junit.runner.Result;
import org.junit.runner.notification.Failure;

public class MathUtilsTest3002 {
    @Test
    public void max() throws Exception {
        int result = MathUtils.max(100, 99, 200);
        //判断测试结果和期望的结果是否一致
        Assert.assertEquals(result, 200);
    }

    @Test
    public void min() throws Exception {
        int result = MathUtils.min(1, -1, 10);
        //判断测试结果和期望的结果是否一致
        Assert.assertEquals(result, -1);
    }
}
```
现在希望同时运行上面2个测试类，可以这么做，创建一个AllTest.java类，注意这个类上有2个注解比较特殊，都是junit提供的，`@RunWith`表示这是一个测试套件类，需要批量运行测试类，具体要运行哪些测试类呢，通过`@Suite.SuiteClasses`来指定
```java
package com.javacode2018.junit.demo3;

import org.junit.runner.RunWith;
import org.junit.runners.Suite;

@RunWith(Suite.class)
@Suite.SuiteClasses({MathUtilsTest3001.class, MathUtilsTest3002.class})
public class AllTest {
}
```
下面来个启动类，将AllTest传递给`JUnitCore.runClasses`
```java
package com.javacode2018.junit.demo3;

import com.javacode2018.junit.demo1.MathUtilsTest;
import com.javacode2018.junit.demo2.MathUtilsTest2;
import org.junit.runner.JUnitCore;
import org.junit.runner.Result;
import org.junit.runner.notification.Failure;

public class Demo3TestRunner {
    public static void main(String[] args) {
        //@1：传入AllTest
        Result result = JUnitCore.runClasses(AllTest.class);

        System.out.println("失败用例个数:" + result.getFailures().size());
        for (Failure failure : result.getFailures()) {
            System.out.println(failure);
        }
        System.out.println("运行测试用例个数:" + result.getRunCount());
        System.out.println("运行测试用例总耗时(ms):" + result.getRunTime());
        System.out.println("测试用例是否都成功了：" + result.wasSuccessful());
    }
}
```
运行输出
```
失败用例个数:0
运行测试用例个数:4
运行测试用例总耗时(ms):12
测试用例是否都成功了：true
```
测试套件中不仅可以包含基本的测试类，而且可以包含其它的测试套件，这样可以很方便的分层管理不同模块的单元测试代码，比如下面代码，Module2Test和Module2Test都是测试套件
```java
@RunWith(Suite.class)
@Suite.SuiteClasses({Test1.class, Test2.class})
public class Module2Test {
}

@RunWith(Suite.class)
@Suite.SuiteClasses({Test1.class, Test2.class})
public class Module2Test {
}

@RunWith(Suite.class)
@Suite.SuiteClasses({Module2Test.class, Module2Test.class, Test3.java})
public class AllTest {
}

//运行AllTest
JUnitCore.runClasses(AllTest.class);
```
<a name="Aph2c"></a>
### Junit常用注解
<a name="rc4kS"></a>
#### `@Test`
`@Test`：将一个普通方法修饰成一个测试方法<br />`@Test(excepted=xx.class)`：xx.class 表示异常类，表示测试的方法抛出此异常时，认为是正常的测试通过的<br />`@Test(timeout = 毫秒数)`：测试方法执行时间是否符合预期
<a name="En05y"></a>
#### `@BeforeClass`
会在所有的方法执行前被执行，`static` 方法 （全局只会执行一次，而且是第一个运行）
<a name="mQRno"></a>
#### `@AfterClass`
会在所有的方法执行之后进行执行，static 方法 （全局只会执行一次，而且是最后一个运行）
<a name="kVNJ7"></a>
#### `@Before`
会在每一个测试方法被运行前执行一次
<a name="gmTEB"></a>
#### `@After`
会在每一个测试方法运行后被执行一次
<a name="PhhBG"></a>
#### `@Ignore`
所修饰的测试方法会被测试运行器忽略
<a name="dQJhp"></a>
#### `@RunWith`
可以更改测试运行器 org.junit.runner.Runner<br />下面的案例，基本上用到了上面所有的注解，大家结合输出理解一下。
```java
package com.javacode2018.junit.demo4;

import com.javacode2018.junit.demo1.MathUtils;
import org.junit.*;
import org.junit.runner.JUnitCore;
import org.junit.runner.Result;
import org.junit.runner.notification.Failure;

import java.util.concurrent.TimeUnit;

public class MathUtilsTest4 {

    @BeforeClass
    public static void bc() {
        System.out.println("@BeforeClass");
        System.out.println("-----------------");
    }

    @AfterClass
    public static void ac() {
        System.out.println("@AfterClass");
    }

    @Before
    public void bf() {
        System.out.println("@Before:" + this);
    }

    @After
    public void af() {
        System.out.println("@After:" + this);
        System.out.println("##################");
    }

    @Test
    public void max() throws Throwable {
        System.out.println("max():" + this);
        int result = MathUtils.max(1, 2, 3);
        //判断测试结果和期望的结果是否一致
        Assert.assertEquals(result, 3);
    }

    @Test
    public void min() throws Exception {
        System.out.println("min():" + this);
        int result = MathUtils.min(1, 2, 3);
        //判断测试结果和期望的结果是否一致
        Assert.assertEquals(result, 1);
    }

    //方法运行时间超过了timeout，表示测试用例运行失败
    @Test(timeout = 1000)
    public void timeOutTest() throws InterruptedException {
        System.out.println("timeOutTest():" + this);
        TimeUnit.SECONDS.sleep(2000);
    }

    //方法若未抛出expected指定的异常，表示测试用例运行失败
    @Test(expected = NullPointerException.class)
    public void expectedTest() {
        System.out.println("expectedTest():" + this);
        new RuntimeException("异常不匹配");
    }

    @Test
    @Ignore
    public void ignoredMethod() {
        System.out.println("我是被忽略的方法");
    }

    public static void main(String[] args) {
        Result result = JUnitCore.runClasses(MathUtilsTest4.class);
        System.out.println("-----------------");
        System.out.println("运行测试用例个数:" + result.getRunCount());
        System.out.println("失败用例个数:" + result.getFailures().size());
        for (Failure failure : result.getFailures()) {
            System.out.println(failure);
        }
        System.out.println("运行测试用例总耗时(ms):" + result.getRunTime());
        System.out.println("测试用例是否都成功了：" + result.wasSuccessful());

    }
}
```
运行结果如下
```
@BeforeClass
-----------------
@Before:com.javacode2018.junit.demo4.MathUtilsTest4@78e03bb5
timeOutTest():com.javacode2018.junit.demo4.MathUtilsTest4@78e03bb5
@After:com.javacode2018.junit.demo4.MathUtilsTest4@78e03bb5
##################
@Before:com.javacode2018.junit.demo4.MathUtilsTest4@48533e64
max():com.javacode2018.junit.demo4.MathUtilsTest4@48533e64
@After:com.javacode2018.junit.demo4.MathUtilsTest4@48533e64
##################
@Before:com.javacode2018.junit.demo4.MathUtilsTest4@7e0b37bc
min():com.javacode2018.junit.demo4.MathUtilsTest4@7e0b37bc
@After:com.javacode2018.junit.demo4.MathUtilsTest4@7e0b37bc
##################
@Before:com.javacode2018.junit.demo4.MathUtilsTest4@1a93a7ca
expectedTest():com.javacode2018.junit.demo4.MathUtilsTest4@1a93a7ca
@After:com.javacode2018.junit.demo4.MathUtilsTest4@1a93a7ca
##################
@AfterClass
-----------------
运行测试用例个数:4
失败用例个数:3
timeOutTest(com.javacode2018.junit.demo4.MathUtilsTest4): test timed out after 1000 milliseconds
max(com.javacode2018.junit.demo4.MathUtilsTest4): hah 
expectedTest(com.javacode2018.junit.demo4.MathUtilsTest4): Expected exception: java.lang.NullPointerException
运行测试用例总耗时(ms):1018
测试用例是否都成功了：false
```
从输出中可以看出

- `@BeforeClass`和`@AfterClass`标注的方法只会运行一次
- 每个`@Test`标注的方法运行之前会先运行`@Before`标注的方法，然后运行`@Test`标注的这个方法，之后再运行`@After`
- 从this的输出看出，每个`@Test`运行的时候，当前类的实例都会重新创建一个新的
- 不论`@Test`标注的方法是否异常，`@AfterClass`、`@After`标注的方法都会执行，且异常会被淹没，输出中看不到异常信息
<a name="fL7Qf"></a>
### 参数化测试
Junit 4 引入了一个新的功能**参数化测试**。<br />参数化测试允许开发人员使用不同的值反复运行同一个测试，遵循 5 个步骤来创建**参数化测试**。

- 用 `@RunWith(Parameterized.class)` 来注释 test 类。
- 创建一个由 `@Parameters` 注释的公共的静态方法，它返回一个对象的集合(数组)来作为测试数据集合。
- 创建一个公共的构造函数，它接受和一行测试数据相等同的东西。
- 为每一列测试数据创建一个实例变量。
- 用实例变量作为测试数据的来源来创建测试用例。

可能大家看了上面的理解，还是比较迷糊。<br />比如使用com.javacode2018.junit.demo1.MathUtils#max测试下面几组数组
```
1,2,3
100,99,80
30,-1,100
```
可以这么写
```java
package com.javacode2018.junit.demo5;

import com.javacode2018.junit.demo1.MathUtils;
import org.junit.Assert;
import org.junit.Test;
import org.junit.runner.JUnitCore;
import org.junit.runner.Result;
import org.junit.runner.RunWith;
import org.junit.runner.notification.Failure;
import org.junit.runners.Parameterized;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

@RunWith(Parameterized.class)
public class MathUtilsTest5 {

    public static class TestData {
        int[] testData;//测试数据
        int expectedValue;//预期的结果

        public TestData(int[] testData, int expectedValue) {
            this.testData = testData;
            this.expectedValue = expectedValue;
        }

        @Override
        public String toString() {
            return "TestData{" +
            "testData=" + Arrays.toString(testData) +
            ", expectedValue=" + expectedValue +
            '}';
        }
    }

    private TestData testData;

    @Parameterized.Parameters
    public static List<TestData> initTestData() {
        System.out.println("initTestData()");
        //key：期望的结果，value：max方法需要测试的数据
        List<TestData> result = new ArrayList<>();
        result.add(new TestData(new int[]{1, 2, 3}, 3));
        result.add(new TestData(new int[]{100, 99, 80}, 100));
        result.add(new TestData(new int[]{30, -1, 100}, 100));
        return result;
    }

    public MathUtilsTest5(TestData testData) {
        System.out.println("MathUtilsTest5构造器:" + testData);
        this.testData = testData;
    }

    @Test
    public void maxTest() throws Throwable {
        System.out.println(this.hashCode() + ",maxTest():" + this.testData);
        int result = MathUtils.max(this.testData.testData);
        //判断测试结果和期望的结果是否一致
        Assert.assertEquals(this.testData.expectedValue, result);
        System.out.println("###################");
    }

    public static void main(String[] args) {
        Result result = JUnitCore.runClasses(MathUtilsTest5.class);
        System.out.println("-----------------");
        System.out.println("运行测试用例个数:" + result.getRunCount());
        System.out.println("失败用例个数:" + result.getFailures().size());
        for (Failure failure : result.getFailures()) {
            System.out.println(failure);
        }
        System.out.println("运行测试用例总耗时(ms):" + result.getRunTime());
        System.out.println("测试用例是否都成功了：" + result.wasSuccessful());

    }
}
```
为了方便大家理解代码的运行过程，代码中添加了很多日志输出，运行结果如下，结合代码和输出，理解很容易
```
initTestData()
MathUtilsTest5构造器:TestData{testData=[1, 2, 3], expectedValue=3}
721748895,maxTest():TestData{testData=[1, 2, 3], expectedValue=3}
###################
MathUtilsTest5构造器:TestData{testData=[100, 99, 80], expectedValue=100}
463345942,maxTest():TestData{testData=[100, 99, 80], expectedValue=100}
###################
MathUtilsTest5构造器:TestData{testData=[30, -1, 100], expectedValue=100}
195600860,maxTest():TestData{testData=[30, -1, 100], expectedValue=100}
###################
-----------------
运行测试用例个数:3
失败用例个数:0
运行测试用例总耗时(ms):12
测试用例是否都成功了：true
```
<a name="okn2P"></a>
## Spring集成junit
Spring集成junit比较简单，下面来个案例感受一下。
<a name="zOdVP"></a>
### 加入Maven配置
```xml
<dependency>
  <groupId>org.springframework</groupId>
  <artifactId>spring-test</artifactId>
  <version>5.2.6.RELEASE</version>
</dependency>
<dependency>
  <groupId>junit</groupId>
  <artifactId>junit</artifactId>
  <version>4.13</version>
</dependency>
```
<a name="Zpxv2"></a>
### Spring的入口配置类
```java
package com.javacode2018.springjunit;

import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;

@Configuration
public class MainConfig {
    @Bean
    public String name() {
        return "java";
    }

    @Bean
    public int age() {
        return 30;
    }
}
```
<a name="gIIzJ"></a>
### junit测试类集成Spring
下面来个测试类，对上面的MainConfig中注册的2个bean进行测试
```java
package com.javacode2018.springjunit;

import org.junit.Assert;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.test.context.ContextConfiguration;
import org.springframework.test.context.junit4.SpringRunner;

@RunWith(SpringRunner.class) //@1
@ContextConfiguration(classes = {MainConfig.class}) //@2
public class MainConfigTest {

    @Autowired
    private String name;

    @Autowired
    private int age;

    @Test
    public void nameTest() {
        System.out.println(this.name);
        Assert.assertEquals("java", this.name);
    }

    @Test
    public void ageTest() {
        System.out.println(this.age);
        Assert.assertEquals(30, this.age);
    }
}
```
注意上面的@1和@2的代码了，这就是Spring集成junit的关键代码，`@Runwith`的值是`org.springframework.test.context.junit4.SpringRunner`，`@ContextConfiguration`注解通过classes属性指定Spring启动类，如果是xml的方式，可以使用另外一个属性value或者locations来指定入口xml的位置。<br />上面代码中name和age属性上面都有`@Autowired`注解，这2个属性会被自动从Spring容器中注入进来。
<a name="xy12M"></a>
### 测试启动类
```java
package com.javacode2018.springjunit;

import org.junit.runner.JUnitCore;
import org.junit.runner.Result;
import org.junit.runner.notification.Failure;

public class TestRunner {

    public static void main(String[] args) {
        Result result = JUnitCore.runClasses(MainConfigTest.class);
        System.out.println("-----------------");
        System.out.println("运行测试用例个数:" + result.getRunCount());
        System.out.println("失败用例个数:" + result.getFailures().size());
        for (Failure failure : result.getFailures()) {
            System.out.println(failure);
        }
        System.out.println("运行测试用例总耗时(ms):" + result.getRunTime());
        System.out.println("测试用例是否都成功了：" + result.wasSuccessful());

    }

}
```
运行输出
```
30
java
-----------------
运行测试用例个数:2
失败用例个数:0
运行测试用例总耗时(ms):422
测试用例是否都成功了：true
```
<a name="fWScR"></a>
## 总结
1、详细介绍了junit的用法，常用的注解有`@Test`、`@BeforeClass`、`@AfterClass`、`@Before`、`@After`、`@Ignore`、`@RunWith`<br />2、Spring中集成junit，主要的配置就是在测试类上面需要加上下面代码
```java
@RunWith(SpringRunner.class) //@1
@ContextConfiguration(classes = {MainConfig.class}) //@2
```
