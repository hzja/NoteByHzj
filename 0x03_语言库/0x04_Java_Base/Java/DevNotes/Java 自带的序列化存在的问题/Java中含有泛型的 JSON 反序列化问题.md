Java 反序列化 JSON
<a name="BThxU"></a>
## 一、背景
```java
public static void main(String[] args) {

    String jsonString = "[\"a\",\"b\"]";
    List<String> list = JSONObject.parseObject(jsonString, List.class);
    System.out.println(list);
}
```
例子中使用fastjson 的类库。<br />为什么 IDEA 会给出下面的警告，该如何解决？<br />![2021-05-13-13-38-31-117773.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620884320183-91623ee4-1aee-4fea-be48-e6611f425b3a.png#clientId=u02538e38-6a43-4&from=ui&id=ufcef304b&originHeight=312&originWidth=986&originalType=binary&size=66365&status=done&style=none&taskId=u49817010-a4e3-4632-b99a-3f8b2ef9586)
<a name="jogEW"></a>
## 二、分析
<a name="mjvZX"></a>
### 2.1 事出诡异必有妖
IDEA 不会无缘无故给出警告提示，警告的原因上图已经给出。<br />把不带泛型的 List 赋值给带泛型的 List， Java 编译器并不知道右侧返回不带泛型的实际 List 是否符合带泛型的 List 约束。<br />和下面的例子非常类似：
```java
public static void main(String[] args) {
    List first = new ArrayList();
    first.add(1);
    first.add("2");
    first.add('3');

    // 提示上述警告
    List<String> third = first;
    System.out.println(third);
}
```
将 first 赋值给 third 时，不能保证 first 元素符合 List的约束，即列表中全是 String。<br />如果执行上述代码，会发现没有报错，哈哈。<br />但是如果使用 foreach 循环或者迭代器取 String 循环时会发生类型转换异常。
```java
public static void main(String[] args) {
    List first = new ArrayList();
    first.add(1);
    first.add("2");
    first.add('3');

    List<String> third = first;
    for (String each : third) { // 类型转换异常
        System.out.println(each);
    }
}
```
类型转换异常？<br />使用 IDEA 的 jclasslib 反编译插件，得到 main 函数的 Code 如下：
```java
 0 new #2 <java/util/ArrayList>
 3 dup
 4 invokespecial #3 <java/util/ArrayList.<init>>
 7 astore_1
 8 aload_1
 9 iconst_1
10 invokestatic #4 <java/lang/Integer.valueOf>
13 invokeinterface #5 <java/util/List.add> count 2
18 pop
19 aload_1
20 ldc #6 <2>
22 invokeinterface #5 <java/util/List.add> count 2
27 pop
28 aload_1
29 bipush 51
31 invokestatic #7 <java/lang/Character.valueOf>
34 invokeinterface #5 <java/util/List.add> count 2
39 pop
40 aload_1
41 astore_2
42 aload_2
43 invokeinterface #8 <java/util/List.iterator> count 1
48 astore_3
49 aload_3
50 invokeinterface #9 <java/util/Iterator.hasNext> count 1
55 ifeq 79 (+24)
58 aload_3
59 invokeinterface #10 <java/util/Iterator.next> count 1
64 checkcast #11 <java/lang/String>
67 astore_4
69 getstatic #12 <java/lang/System.out>
72 aload_4
73 invokevirtual #13 <java/io/PrintStream.println>
76 goto 49 (-27)
79 return
```
从 42 到76 行 对应 foreach 循环的逻辑，可以看出底层使用 List 的迭代器进行遍历，取出每个元素后强转为 String 类型，存储到局部变量表索引为 4 的位置，然后进行打印。<br />如果对反编译不熟悉可以去 target 目录，双击编译后的class 文件，使用 IDEA 自带的插件进行反编译：
```java
//
// Source code recreated from a .class file by IntelliJ IDEA
// (powered by Fernflower decompiler)
//

package com.chujianyun.common.json;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

public class JsonGenericDemo {
    public JsonGenericDemo() {
    }

    public static void main(String[] args) {
        List first = new ArrayList();
        first.add(1);
        first.add("2");
        first.add('3');
        List<String> third = first;
        Iterator var3 = first.iterator();

        while(var3.hasNext()) {
            String each = (String)var3.next();
            System.out.println(each);
        }
    }
}
```
印证了上述说法，显然在 `String each = (String)var3.next();` 这里出现了类型转换异常。
<a name="G9sZW"></a>
## 三、解决之道
<a name="kFlgM"></a>
### 3.1 猜想验证
猜测是不是可以通过某种途径将泛型作为参数传给 fastjson, 让 fastjson 某个返回值是带泛型的，从而解决这个告警呢？<br />显然要去源码中寻找， 在 JSONObject 类中找到了下面的方法：
```java
/**
 * <pre>
 * String jsonStr = "[{\"id\":1001,\"name\":\"Jobs\"}]";
 * List<Model> models = JSON.parseObject(jsonStr, new TypeReference<List<Model>>() {});
 * </pre>
 * @param text json string
 * @param type type refernce
 * @param features
 * @return
 */
@SuppressWarnings("unchecked")
public static <T> T parseObject(String text, TypeReference<T> type, Feature... features) {
    return (T) parseObject(text, type.type, ParserConfig.global, DEFAULT_PARSER_FEATURE, features);
}
```
该函数的注释上还贴心地给出了相关用法，因此改造下：
```java
public static void main(String[] args) {
    String jsonString = "[\"a\",\"b\"]";
    List<String> list = JSONObject.parseObject(jsonString, new TypeReference<List<String>>() {
    });
    System.out.println(list);
}
```
警告解除了。所以大功告成？
```java
import lombok.Data;

@Data
public class User {
    private Long id;

    private String name;
}

mport com.alibaba.fastjson.JSON;
import com.alibaba.fastjson.JSONObject;

import java.util.ArrayList;
import java.util.List;

public class JsonGenericDemo {

    public static void main(String[] args) {
        // 构造数据
        User user = new User();
        user.setId(0L);
        user.setName("tom");

        List<User> users = new ArrayList<>();
        users.add(user);
        // 转为JSON字符串
        String jsonString = JSON.toJSONString(users);

        // 反序列化
        List<User> usersGet = JSONObject.parseObject(jsonString, List.class);

        for (User each : usersGet) {
            System.out.println(each);
        }
    }

}
```
执行上述例子会出现类型转换异常！
```java
Exception in thread “main” java.lang.ClassCastException: com.alibaba.fastjson.JSONObject cannot be cast to com.chujianyun.common.json.User at com.chujianyun.common.json.JsonGenericDemo.main(JsonGenericDemo.java:26)
```
有了第二部分的分析，大家可能就可以比较容易地想到<br />`JSONObject.parseObject(jsonString, List.class)`构造出来的 List 存放的是 `JSONObject` 元素， foreach 循环底层使用迭代器遍历每个元素并强转为 User 类型是报类型转换异常。<br />那么为啥 fastjson 不能帮转换为`List<User>`类型呢？<br />有人说“由于泛型擦除，没有泛型信息，所以无法逆向构造回原有类型”。<br />其实看下`JSONObject.parseObject(jsonString, List.class);`第一个参数是字符串，第二个参数是 List.class。压根就没有提供泛型信息给 fastjson。<br />作为这个工具函数本身，怎么猜得到要 List 里面究竟该存放啥类型呢？<br />因此如果能够通过某种途径，告诉它泛型的类型，就可以反序列化成真正的类型。<br />使用`JSONObject.parseObject(jsonString, new TypeReference<List<User>>() { });`即可。<br />因此使用 TypeReference 并不仅仅是为了消除警告，而是为了告知 fastjson 泛型的具体类型，正确反序列化泛型的类型。<br />那么底层原理是啥呢？看下`com.alibaba.fastjson.TypeReference#TypeReference()`
```java
/**
 * Constructs a new type literal. Derives represented class from type
 * parameter.
 *
 * <p>Clients create an empty anonymous subclass. Doing so embeds the type
 * parameter in the anonymous class's type hierarchy so we can reconstitute it
 * at runtime despite erasure.
 */
protected TypeReference(){
    // 获取父类的 Type
    Type superClass = getClass().getGenericSuperclass();

    // 如果父类是参数化类型，会返回 java.lang.reflect.ParameterizedType
    // 调用 getActualTypeArguments 获取实际类型的数组 并拿到第一个
    Type type = ((ParameterizedType) superClass).getActualTypeArguments()[0];

    // 缓存中有优先取缓存，没有则存入并设置
    Type cachedType = classTypeCache.get(type);
    if (cachedType == null) {
        classTypeCache.putIfAbsent(type, type);
        cachedType = classTypeCache.get(type);
    }

    this.type = cachedType;
}
```
通过代码和注释了解到：<br />创建一个空的匿名子类。将类型参数嵌入到匿名继承结构中，即使运行时类型擦除也可以重建。<br />再回到 `parseObject` 函数，可以看到底层用的就是这个 type。
```java
/**
 * <pre>
 * String jsonStr = "[{\"id\":1001,\"name\":\"Jobs\"}]";
 * List<Model> models = JSON.parseObject(jsonStr, new TypeReference<List<Model>>() {});
 * </pre>
 * @param text json string
 * @param type type refernce
 * @param features
 * @return
 */
@SuppressWarnings("unchecked")
public static <T> T parseObject(String text, TypeReference<T> type, Feature... features) {
    return (T) parseObject(text, type.type, ParserConfig.global, DEFAULT_PARSER_FEATURE, features);
}
```
<a name="JUlSd"></a>
### 3.2 举一反三
很多其他框架也会采用类似的方法来获取泛型类型。<br />大家可以看看其他 gson 类库
```xml
<dependency>
  <groupId>com.google.code.gson</groupId>
  <artifactId>gson</artifactId>
  <version>2.8.6</version>
</dependency>
```
看看其中的com.google.gson.reflect.TypeToken类，是不是似曾相识呢？<br />此外，如果自己除了JSON反序列化场景之外也有类似获取泛型参数的需求，是不是也可以采用类似的方法呢？
