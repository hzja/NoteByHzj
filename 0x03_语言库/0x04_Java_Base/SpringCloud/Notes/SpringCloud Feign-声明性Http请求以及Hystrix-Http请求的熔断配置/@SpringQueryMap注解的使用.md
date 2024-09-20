Java SpringCloud<br />SpringCloud项目使用feign的时候都会发现一个问题，如果是GET方法，需要指定一连串参数，这时候使用对象去封装这些参数，但是get方式无法解析对象参数，如果方法的参数是一个对象，则会被强行转变成Post请求。其实feign是支持对象传递的，但是得是Map形式，而且不能为空，与Spring在机制上不兼容，因此无法使用。<br />在SpringCloud 2.1.x 以上的版本，提供了一个新的注解`@SpringQueryMap`，这个注解可以实现将JavaBean转为Get请求的参数。
<a name="TTkzu"></a>
## 引入依赖
```xml
<dependency>
  <groupId>org.springframework.cloud</groupId>
  <artifactId>spring-cloud-starter-openfeign</artifactId>
  <version>3.0.4</version>
</dependency>
```
<a name="I6J1E"></a>
## 使用方法
```java
@RequestMapping(value = "/safrv_2meta_id_name/", method = RequestMethod.GET, produces = MediaType.APPLICATION_JSON_UTF8_VALUE)
Response verifyIdCardAndNameDTO(@SpringQueryMap AliyunVerifyIdCardAndNameReq app, @RequestHeader("Authorization") String authorization);
```
<a name="oMRhh"></a>
## 源码
可以翻翻`feign`的源码，相对来说应该是比较简单的。直接拿这个注解全局搜一下，看看有哪些地方使用到了，在每个地方都打上一个断点试试![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1634039062171-a68f82b7-333a-404c-a59f-0c86809c6f6f.webp#clientId=u4e17ccc9-1709-4&from=paste&id=u868df1e6&originHeight=217&originWidth=1019&originalType=url&ratio=1&status=done&style=none&taskId=ue1f6896e-a3ba-4c97-867d-ca50499a860)全局搜下发现使用的地方主要在`QueryMapParameterProcessor`这个类里面。所以可以在这个类里面打上一个断点试试。
```java
/**
 * {@link SpringQueryMap} parameter processor.
 *
 * @author Aram Peres
 * @see AnnotatedParameterProcessor
 */
public class QueryMapParameterProcessor implements AnnotatedParameterProcessor {

    private static final Class<SpringQueryMap> ANNOTATION = SpringQueryMap.class;

    @Override
    public Class<? extends Annotation> getAnnotationType() {
        return ANNOTATION;
    }

    @Override
    public boolean processArgument(AnnotatedParameterContext context, Annotation annotation, Method method) {
        int paramIndex = context.getParameterIndex();
        MethodMetadata metadata = context.getMethodMetadata();
        if (metadata.queryMapIndex() == null) {
            metadata.queryMapIndex(paramIndex);
            metadata.queryMapEncoded(SpringQueryMap.class.cast(annotation).encoded());
        }
        return true;
    }
}
```
可以发现这个类的话在容器启动的时候会进行加载，并且会执行`processArgument`方法，接下来来看看 Feign真正发起调用的地方找到`SynchronousMethodHandler#invoke`方法
```java
public RequestTemplate create(Object[] argv) {
 ... 省略部分代码
 // metadata.queryMapIndex() 就是QueryMapParameterProcessor #processArgument方法赋值的
      if (metadata.queryMapIndex() != null) {
        // add query map parameters after initial resolve so that they take
        // precedence over any predefined values
        // 通过下标获取到需要特殊处理的对象，这里有个问题只会处理方法参数的第一个@SpringQueryMap注解，
        // 原因就是QueryMapParameterProcessor #processArgument这个方法只会把第一个下标赋值进去，然后这里也只会取第一个下标，所以只会处理第一个@SpringQueryMap注解
        Object value = argv[metadata.queryMapIndex()];
        //将对象转换为map  这里需要注意下默认使用解析参数的是FieldQueryMapEncoder类所以它并不会去解析父类的参数，如果需要解析父类的参数我们需要在feign的Config里面指定QueryMapEncoder为FieldQueryMapEncoder
        Map<String, Object> queryMap = toQueryMap(value);
        //拼接解析完成的对象为URL参数
        template = addQueryMapQueryParameters(queryMap, template);
      }
... 省略部分代码
}
```
上述代码逻辑还是挺好理解的

- 首先去判断是否需要处理下`querymap`
- 通过下标获取到需要特殊处理的对象
- 将对象转换为`map`(这里有个坑默认不会去解析父类的字段)
- 拼接追加`map`到`url`中
<a name="x7dKc"></a>
## 总结

- 上面通过`@SpringQueryMap`注解实现了`get`传参，但是如果需要传递多个`@SpringQueryMap`注解怎么来实现呢？
- 或者可以自己动手来实现一个`SpringQueryMap`，该如何实现？
- `@SpringQueryMap`注解默认是不会去解析父类的参数，如果需要解析父类的参数需要修改`Feign`的`config#QueryMapEncoder`为`FieldQueryMapEncoder`。
- 如果自己去实现了一个`AnnotatedParameterProcessor`所有默认的`PathVariableParameterProcessor`、`RequestParamParameterProcessor`、`RequestHeaderParameterProcessor`、`QueryMapParameterProcessor`都会失效，为啥会失效需要去看`SpringMvcContract`这个类。所以自定义`AnnotatedParameterProcessor`需要慎重。
