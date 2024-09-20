JavaSpringMVC
<a name="ZMEPu"></a>
## 1、`@RequestParam` 注解的作用
标注在接口的方法参数上，被标注的参数的值来源于`request.getParameter`或`request.getParameterValues`。
<a name="fjO6h"></a>
## 2、`@RequestParam` 源码
```java
@Target(ElementType.PARAMETER)
@Retention(RetentionPolicy.RUNTIME)
@Documented
public @interface RequestParam {

    /**
     * 对应request中参数名称
     */
    @AliasFor("name")
    String value() default "";

    /**
     * 同value
     */
    @AliasFor("value")
    String name() default "";

    /**
     * 请求中是否必须有这个参数，默认为true
     */
    boolean required() default true;

    /**
     * 默认值
     */
    String defaultValue() default ValueConstants.DEFAULT_NONE;

}
```
<a name="FtRtY"></a>
## 3、案例 1：`@RequestParam` 指定 name，获取对应参数的值
<a name="BZrFr"></a>
### 3.1、案例接口代码
```java
/**
 * {@link RequestParam}中指定name，用来取name的值对应的请求参数中的值
 *
 * @param name：可以不传递name参数，不传递的时候默认值为ready
 * @param age
 * @param pets
 * @return
 */
@RequestMapping("/requestparam/test1")
public Map<String, Object> test1(@RequestParam(value = "name", required = false, defaultValue = "ready") String name, //相当于request.getParameter("name")
                                 @RequestParam("age") int age, //Integer.parseInt(request.getParameter("age"))
                                 @RequestParam("interests") String[] interests, //request.getParameterValues("pets")
                                 @RequestParam("pets") List<String> pets //Arrays.asList(request.getParameterValues("pets"))
                                ) {
    Map<String, Object> result = new LinkedHashMap<>();
    result.put("name", name);
    result.put("age", age);
    result.put("interests", interests);
    result.put("pets", pets);
    return result;
}
```
<a name="B1vUl"></a>
### 3.2、用例 1：所有参数都传值
```http
POST http://localhost:8080/chat18/requestparam/test1
Content-Type: application/x-www-form-urlencoded

name=路人&age=35&interests=篮球&interests=旅游&pets=小狗&pets=小猫
```
运行上面用例代码输出
```json
{
  "name": "路人",
  "age": 35,
  "interests": [
    "篮球",
    "旅游"
  ],
  "pets": [
    "小狗",
    "小猫"
  ]
}
```
<a name="KIDdU"></a>
### 3.3、用例 2：name 不传递，会取默认值 ready
```http
POST http://localhost:8080/chat18/requestparam/test1
Content-Type: application/x-www-form-urlencoded

age=35&interests=篮球&interests=旅游&pets=小狗&pets=小猫
```
运行上面用例代码输出
```json
{
  "name": "ready",
  "age": 35,
  "interests": [
    "篮球",
    "旅游"
  ],
  "pets": [
    "小狗",
    "小猫"
  ]
}
```
<a name="okorL"></a>
### 3.4、用例 3：required 属性为 `true`，不传，则报错
```http
POST http://localhost:8080/chat18/requestparam/test1
Content-Type: application/x-www-form-urlencoded
```
参数都没有传递，而接口要求除 name 之外的，其他几个参数都必须传递，所以这个案例结果会报 400 错，提示 age 参数不存在，这个错误比较常见，大家熟悉下，以后看到了就知道什么问题了。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1684049843310-35fdbe9e-6f2d-4180-9fb2-9c3cdbba6a58.png#averageHue=%23e8c68a&clientId=u6029061a-bdce-4&from=paste&id=u8afe8551&originHeight=250&originWidth=905&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u294b145a-351a-4de8-8a40-1155f749429&title=)
<a name="OVKRE"></a>
## 4、案例 2：`@RequestParam` 不指定 name，获取所有参数值
当想用一个 Map 来接收所有参数的之后，代码如下，`@RequestParam` 不用指定 name 的值，参数类型为 `Map<String,String>`，所有请求参数会以参数名称:值的方式丢在 Map 中。
```java
/**
 * {@link RequestParam}不指定name，用于接收所有参数的值，
 * 参数类型为Map<String,String>,key为请求中的参数名称，value为值
 *
 * @param paramMap
 * @return
 */
@RequestMapping("/requestparam/test2")
public Map<String, String> test2(@RequestParam Map<String, String> paramMap) {
    return paramMap;
}
```
运行下面的用例调用上面接口
```http
POST http://localhost:8080/chat18/requestparam/test2
Content-Type: application/x-www-form-urlencoded

name=路人&age=35&interests=篮球&interests=旅游&pets=小狗&pets=小猫
```
运行输出
```json
{
  "name": "路人",
  "age": "35",
  "interests": "篮球",
  "pets": "小狗"
}
```
interests 和 pet 都是有多个值，上面的结果中都只获取了第一个 值，如果想获取所有的值呢，下面看案例 3。
<a name="zXKG0"></a>
## 5、案例 3：`@RequestParam` 不指定 name，获取所有参数值
```java
/**
 * {@link RequestParam}不指定name，用于接收所有参数的值，
 * 参数类型为MultiValueMap<String, String>：key为请求中的参数名称，value为值的集合List<String>
 *
 * @param paramMap
 * @return
 */
@RequestMapping(value = "/requestparam/test3", produces = MediaType.APPLICATION_JSON_VALUE)
public MultiValueMap<String, String> test3(@RequestParam MultiValueMap<String, String> paramMap) {
    return paramMap;
}
```
这个接口的参数是`MultiValueMap`类型，这玩意是干啥的？好像很陌生。<br />再陌生的东西，把其源码放出来，瞬间明了了，如下，可以看出来`MultiValueMap`相当于`Map<String,List<String>>`
```java
public interface MultiValueMap<K, V> extends Map<K, List<V>> {
}
```
运行下面的用例调用上面接口，注意下面第 3 行，表示期望服务器端返回 json 格式数据
```http
POST http://localhost:8080/chat18/requestparam/test3
Content-Type: application/x-www-form-urlencoded
Accept: application/json

name=路人&age=35&interests=篮球&interests=旅游&pets=小狗&pets=小猫
```
运行输出，结果的值比较特别，是一个 String 类型的数组，这次获取到所有参数的值了。
```json
{
  "name": [
    "路人"
  ],
  "age": [
    "35"
  ],
  "interests": [
    "篮球",
    "旅游"
  ],
  "pets": [
    "小狗",
    "小猫"
  ]
}
```
<a name="B30bZ"></a>
## 6、`@RequestParam` 原理
`@RequestParam` 标注的参数的值是有下面 2 个类处理的：
```
org.springframework.web.method.annotation.RequestParamMethodArgumentResolver
org.springframework.web.method.annotation.RequestParamMapMethodArgumentResolver
```
<a name="fToFs"></a>
## 7、总结

- `@RequestParam` 注解用来标注在控制器方法的参数上，SpringMVC 从 request 中获取请求的值赋值给方法的参数
- `@RequestParam` 指定 name 时，可以获取 request 中指定参数的值，相当于 `request.getParameter(name)`或 `request.getParameters(name)`
- `@RequestParam` 未指定 name，参数类型为：`Map<String,String>`时，用来接收 request 中所有参数的值，Map 中 key 为参数名称，value 为参数的值
- `@RequestParam` 未指定 name，参数类型为 `MultiValueMap<String, String>`时，用来接收 request 中所有参数的值，key 为请求中的参数名称，value 为值的集合 `List<String>`
