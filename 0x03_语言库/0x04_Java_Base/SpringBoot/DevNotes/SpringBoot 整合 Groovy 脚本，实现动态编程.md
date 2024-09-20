JavaSpringBootGroovy
<a name="tHebb"></a>
## Groovy简介
Groovy 是增强 Java 平台的唯一的脚本语言。它提供了类似于 Java 的语法，内置映射（Map）、列表（List）、方法、类、闭包（closure）以及生成器。脚本语言不会替代系统编程语言，两者是相互补充的。<br />大名鼎鼎的 Gradle，背后是 Groovy。Spring 的未来越来越多的使用 Groovy，甚至在用 Jira 跟踪项目时，背后也有 Groovy。实际上，就应用场景而言，Java 开发已经有越来越多的 Groovy 出现在后台了。而对于一般的应用开发，只要能用 Java 就都能用到 Groovy，唯一的难点只在于能不能招到足够的人员。
<a name="RdYEe"></a>
## 应用场景

- 连接已有的组件
- 处理经常变化的多种类型的实体
- 具有图形化用户界面
- 拥有快速变化的功能

Groovy脚本的基础概念请移步

- [http://mvnbook.com/groovy-introduction.html](http://mvnbook.com/groovy-introduction.html)
<a name="WlQis"></a>
## 集成与使用
那么接下来介绍SpringBoot如何集成Groovy脚本，并应用到实际开发中。
<a name="qaiWS"></a>
### 第一步、与SpringBoot集成
pom.xml文件如下：
```xml
<dependency>
  <groupId>org.codehaus.groovy</groupId>
  <artifactId>groovy-all</artifactId>
  <version>2.4.7</version>
</dependency>
<dependency>
  <groupId>org.springframework.boot</groupId>
  <artifactId>spring-boot-starter-web</artifactId>
</dependency>
```
<a name="Uqd5n"></a>
### 第二步、写出Groovy版本的“Hello World”
1、HelloWorld.groovy脚本代码
```groovy
package groovy

def HelloWorld(){
    println "hello world"
}
```
2、创建测试类GroovyTest.java
```java
package com.example.springbootgroovy.service;

import groovy.lang.GroovyShell;
import groovy.lang.Script;

/**
 * 这个是Groovy的第一个小程序，脚本为：
 * 
 package groovy
 
 def helloworld(){
  println "hello world"
 }
 *
 */
public class GroovyTest {

    public static void main(String[] args) throws Exception {
        //创建GroovyShell
        GroovyShell groovyShell = new GroovyShell();
        //装载解析脚本代码
        Script script = groovyShell.parse("package groovy\n" +
                "\n" +
                "def HelloWorld(){\n" +
                "    println \"hello world\"\n" +
                "}");
        //执行
        script.invokeMethod("HelloWorld", null);
    }
}
```
3、运行结果<br />![2022-11-30-10-20-56.548398600.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1669774919305-f306b641-644d-4497-ade7-63f56c52065e.png#averageHue=%23eff5ed&clientId=u48ef9749-937d-4&from=ui&id=GGx19&originHeight=370&originWidth=1336&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1485811&status=done&style=none&taskId=u3350b97c-e531-4803-9a64-5b2121e3587&title=)
<a name="yBiFq"></a>
### 第三步、传入变量与获取返回值
1、变量与返回值Groovy脚本代码
```groovy
package groovy

/**
 * 简易加法
 * @param a 数字a
 * @param b 数字b
 * @return 和
 */
def add(int a, int b) {
    return a + b
}

/**
 * map转化为String
 * @param paramMap 参数map
 * @return 字符串
 */
def mapToString(Map<String, String> paramMap) {
    StringBuilder stringBuilder = new StringBuilder();
    paramMap.forEach({ key, value ->
        stringBuilder.append("key:" + key + ";value:" + value)
    })
    return stringBuilder.toString()
}
```
2、创建测试类GroovyTest2.java
```java
package com.example.springbootgroovy.service;

import groovy.lang.GroovyShell;
import groovy.lang.Script;

import java.util.HashMap;
import java.util.Map;

/**
 * 向Groovy脚本中传入变量，以及获取返回值
 */
public class GroovyTest2 {
    public static void main(String[] args) {
        //创建GroovyShell
        GroovyShell groovyShell = new GroovyShell();
        //装载解析脚本代码
        Script script = groovyShell.parse("package groovy\n" +
                "\n" +
                "/**\n" +
                " * 简易加法\n" +
                " * @param a 数字a\n" +
                " * @param b 数字b\n" +
                " * @return 和\n" +
                " */\n" +
                "def add(int a, int b) {\n" +
                "    return a + b\n" +
                "}\n" +
                "\n" +
                "/**\n" +
                " * map转化为String\n" +
                " * @param paramMap 参数map\n" +
                " * @return 字符串\n" +
                " */\n" +
                "def mapToString(Map<String, String> paramMap) {\n" +
                "    StringBuilder stringBuilder = new StringBuilder();\n" +
                "    paramMap.forEach({ key, value ->\n" +
                "        stringBuilder.append(\"key:\" + key + \";value:\" + value)\n" +
                "    })\n" +
                "    return stringBuilder.toString()\n" +
                "}");
        //执行加法脚本
        Object[] params1 = new Object[]{1, 2};
        int sum = (int) script.invokeMethod("add", params1);
        System.out.println("a加b的和为:" + sum);
        //执行解析脚本
        Map<String, String> paramMap = new HashMap<>();
        paramMap.put("科目1", "语文");
        paramMap.put("科目2", "数学");
        Object[] params2 = new Object[]{paramMap};
        String result = (String) script.invokeMethod("mapToString", params2);
        System.out.println("mapToString:" + result);
    }
}
```
3、运行结果<br />![2022-11-30-10-20-56.762072900.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1669774918998-d59a4d2f-87c9-43a3-96c0-45b8d27b1f66.png#averageHue=%23e7efe4&clientId=u48ef9749-937d-4&from=ui&id=PGJmK&originHeight=200&originWidth=1372&originalType=binary&ratio=1&rotation=0&showTitle=false&size=824814&status=done&style=none&taskId=u7b8ebcb8-b808-4acd-95c1-3ff3f50a009&title=)
<a name="yHv36"></a>
### 第四步、启动SpringBoot，在Groovy脚本中通过`SpringContextUtil`获取SpringBoot容器中的Bean
1、创建SpringContextUtil.java
```java
package com.example.springbootgroovy.util;

import org.springframework.beans.BeansException;
import org.springframework.context.ApplicationContext;
import org.springframework.context.ApplicationContextAware;
import org.springframework.stereotype.Component;

/**
 * Spring上下文获取
 */
@Component
public class SpringContextUtil implements ApplicationContextAware {

    private static ApplicationContext applicationContext;

    @Override
    public void setApplicationContext(ApplicationContext applicationContext) throws BeansException {
        SpringContextUtil.applicationContext = applicationContext;
    }

    public static ApplicationContext getApplicationContext() {
        return applicationContext;
    }

    /**
     * 通过name获取 Bean.
     *
     * @param name
     * @return
     */
    public static Object getBean(String name) {
        return getApplicationContext().getBean(name);
    }

    /**
     * 通过class获取Bean.
     *
     * @param clazz
     * @param <T>
     * @return
     */
    public static <T> T getBean(Class<T> clazz) {
        return getApplicationContext().getBean(clazz);
    }

    /**
     * 通过name,以及Clazz返回指定的Bean
     *
     * @param name
     * @param clazz
     * @param <T>
     * @return
     */
    public static <T> T getBean(String name, Class<T> clazz) {
        return getApplicationContext().getBean(name, clazz);
    }
}
```
2、创建GroovyTestService.java，并加上`@Service`注解加入到SpringBoot容器中
```java
package com.example.springbootgroovy.service;

import org.springframework.stereotype.Service;

@Service
public class GroovyTestService {

    public void test(){
        System.out.println("我是SpringBoot框架的成员类，但该方法由Groovy脚本调用");
    }

}
```
3、Groovy脚本如下
```groovy
package groovy

import com.example.springbootgroovy.service.GroovyTestService
import com.example.springbootgroovy.util.SpringContextUtil

/**
 * 静态变量
 */
class Globals {
    static String PARAM1 = "静态变量"
    static int[] arrayList = [1, 2]
}

def getBean() {
    GroovyTestService groovyTestService = SpringContextUtil.getBean(GroovyTestService.class);
    groovyTestService.test()
}
```
4、启动类代码如下
```java
package com.example.springbootgroovy;

import groovy.lang.GroovyShell;
import groovy.lang.Script;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

@RestController
@RequestMapping("/groovy")
@SpringBootApplication
public class SpringBootGroovyApplication {

    public static void main(String[] args) {
        SpringApplication.run(SpringBootGroovyApplication.class, args);
    }

    @RequestMapping("/test")
    public String test() {
        //创建GroovyShell
        GroovyShell groovyShell = new GroovyShell();
        //装载解析脚本代码
        Script script = groovyShell.parse("package groovy\n" +
                "\n" +
                "import com.example.springbootgroovy.service.GroovyTestService\n" +
                "import com.example.springbootgroovy.util.SpringContextUtil\n" +
                "\n" +
                "/**\n" +
                " * 静态变量\n" +
                " */\n" +
                "class Globals {\n" +
                "    static String PARAM1 = \"静态变量\"\n" +
                "    static int[] arrayList = [1, 2]\n" +
                "}\n" +
                "\n" +
                "def getBean() {\n" +
                "    GroovyTestService groovyTestService = SpringContextUtil.getBean(GroovyTestService.class);\n" +
                "    groovyTestService.test()\n" +
                "}");
        //执行
        script.invokeMethod("getBean", null);
        return "ok";
    }
}
```
5、启动后调用接口：http://localhost:8080/groovy/test，运行结果如下<br />![2022-11-30-10-20-56.883879000.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1669774919306-92c82e6a-70d0-443d-beee-625985871d58.png#averageHue=%23f5f5f4&clientId=u48ef9749-937d-4&from=ui&id=u73da97d2&originHeight=334&originWidth=1234&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1238888&status=done&style=none&taskId=u8b9fff59-267a-4dfc-b677-5a2513b600b&title=)<br />**注意！！！**通过第四步中可以看到，在Groovy中是可以获取到SpringBoot容器对象的。虽然很方便，但是很危险。如果没有做好权限控制，Groovy脚本将会成为攻击系统最有力的武器！！！
