Java fastjson
<a name="rkh5j"></a>
## 前言
fastjson是一个非常好用的java库，用于操作对象json序列化等等。
<a name="TVxXq"></a>
## 问题
最近在写代码的时候遇到问题，通过`JSON.toJSONString`方法将一个实体对象转为json字符串，转出来发现原本对象中value为null的字段被过滤掉了。<br />实体对象
```java
import lombok.Data;

@Data
public class Person {
    String name;
    String gender;
    String nickname;
    String phone;
}
```
测试代码
```java
public static void main(String[] args) {
    Person person = new Person();
    person.setName("Fcant");
    String json = JSON.toJSONString(person);
    System.out.println(json);
}
```
输出结果<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1629543724999-61f0c6ad-3f9c-4f4d-a9ea-fda0a3fbc68c.png#clientId=u12dc450f-3543-4&from=paste&height=173&id=ua4b4be28&originHeight=519&originWidth=1993&originalType=binary&ratio=1&size=58270&status=done&style=shadow&taskId=u2ca1d2a3-8703-44bd-9e45-a397bcce1c8&width=664.3333333333334)<br />发现只有设置了值的字段name才有输出，而其余为空的字段则都被过滤掉了
<a name="N5I9e"></a>
## 解决
<a name="Moie8"></a>
### 怎样让值为null的key不被过滤掉

- 设置`SerializerFeature`

设置`SerializerFeature`里面有很多特性可以使用<br />要让空值也能够输出的特性值是`SerializerFeature.WriteMapNullValue`<br />改造代码：
```java
public static void main(String[] args) {
    Person person = new Person();
    person.setName("Fcant");
    //        String json = JSON.toJSONString(person);
    //        System.out.println(json);

    // 设置SerializerFeature
    System.out.println(JSON.toJSONString(person, SerializerFeature.WriteMapNullValue));
}
```
输出结果：<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1629543782384-0b08f491-0e51-44ee-bac5-31eaa5bf7f18.png#clientId=u12dc450f-3543-4&from=paste&height=164&id=u4f0e9bfa&originHeight=491&originWidth=2143&originalType=binary&ratio=1&size=68604&status=done&style=shadow&taskId=u424c2878-08c0-41fb-bfb5-f6a414b02cb&width=714.3333333333334)
<a name="qUOhf"></a>
### 值为null属性直接放到json字符串里面不太好看，能不能把null值转成其他的值，比如空字符串
这个时候就需要用到过滤器了，仔细看看`JSON.toJSONString`的其他几个方法<br />`toJSONString`方法中可以放一个或多个过滤器，常用的过滤器有以下几种：<br />要实现把null值转为空字符串的效果可以使用`ValueFilter`<br />代码实现：
```java
public static void main(String[] args) {
    Person person = new Person();
    person.setName("Fcant");
    //        String json = JSON.toJSONString(person);
    //        System.out.println(json);

    // 设置SerializerFeature
    //        System.out.println(JSON.toJSONString(person, SerializerFeature.WriteMapNullValue));

    // 设置NameFilter
    System.out.println(JSON.toJSONString(person, (ValueFilter) (object, name, value) -> {
        if(value == null){
            return "";
        }

        return value;
    }));
}
```
输出结果：<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1629543888467-5665d083-f00e-42ec-a026-2857808d3884.png#clientId=u12dc450f-3543-4&from=paste&height=188&id=u25ca818b&originHeight=564&originWidth=2084&originalType=binary&ratio=1&size=69876&status=done&style=shadow&taskId=u18b8dc80-d166-4bc9-94d7-29235197e24&width=694.6666666666666)
<a name="onnTi"></a>
### 一个特殊的需求，有的值为null需要过滤，有的又不需要过滤
使用`PropertyFilter`<br />代码实现：
```java
public static void main(String[] args) {
    Person person = new Person();
    person.setName("Fcant");
    //        String json = JSON.toJSONString(person);
    //        System.out.println(json);

    // 设置SerializerFeature
    //        System.out.println(JSON.toJSONString(person, SerializerFeature.WriteMapNullValue));

    // 设置NameFilter
    //        System.out.println(JSON.toJSONString(person, (ValueFilter) (object, name, value) -> {
    //            if(value == null){
    //                return "";
    //            }
    //
    //            return value;
    //        }));

    // 设置PropertyFilter
    System.out.println(JSON.toJSONString(person, new PropertyFilter() {
        @Override
        public boolean apply(Object object, String name, Object value) {
            return name.equals("gender") || value != null;
        }
    }, SerializerFeature.WriteMapNullValue));
}
```
输出结果：<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1629543962624-2dbc06d9-cb2e-4a1b-9b55-6b3c55e62c82.png#clientId=u12dc450f-3543-4&from=paste&height=164&id=ud465e849&originHeight=492&originWidth=1870&originalType=binary&ratio=1&size=60047&status=done&style=shadow&taskId=u8d81fd83-92a5-49dd-bff2-d4f6a9e8d1a&width=623.3333333333334)
