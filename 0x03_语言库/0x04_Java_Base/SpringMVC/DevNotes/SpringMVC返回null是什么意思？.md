JavaSpringMVC<br />目前所有 Controller 中的方法接收到请求之后，都是有返回值的，返回值主要有 2 种类型：<br />1、 输出的是页面：也就是视图（会向客户端输出页面），此时方法的返回值可以是 String（视图名称）、ModelAndView（页面中有数据的情况）<br />2、输出的是 json 格式的数据：需要用到`@ResponseBody` 注解<br />这 2 种情况中，都是 SpringMVC 来处理返回值的，接受到返回值之后，会调用 response 来进行页面跳转或者调用输出流将 json 格式的数据输出。
<a name="KHyjJ"></a>
## 思考一个问题
当方法的返回值为 void 或者方法中返回 null 的时候，SpringMVC 会怎么处理呢？<br />比如下面 2 个方法：
```java
@GetMapping("/test1")
public void test(){
}

@GetMapping("/test1")
public Object test(){
    return null;
}
```
**当出现上面这 2 种情况的时候，SpringMVC 调用这些方法之后，请求就结束了，SpringMVC 会认为在控制器的方法中响应已经被处理过了，不需要 SpringMVC 去处理了。**
<a name="a45Jf"></a>
## SpringMVC 的处理流程
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683905466335-b4d8a391-0e85-4e6f-8bce-11d8649fda6b.png#averageHue=%23f5f5f5&clientId=u10ece8d2-bf0a-4&from=paste&id=u75b0aee4&originHeight=742&originWidth=469&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=udc4336c4-ef70-4555-9786-a7a77380e4b&title=)
<a name="arrSC"></a>
## 使用场景
当响应结果比较复杂的时候，SpringMVC 无法处理这些响应结果的时候，可以在控制器的方法中使用 response 来主动控制输出的结果。<br />比如下载文件、断点下载文件等比较复杂的响应，此时可以在处理器的方法中使用 `HttpServletResponse` 来自己控制输出的内容，可以执行更细粒度的操作。
<a name="TXUvU"></a>
## 总结
到目前主要掌握了 3 种类型的返回值，工作中基本上最常用的就是这 3 种方式，要掌握好：

- 第 1 种：返回视图，即页面，此时返回值可以是 String（视图名称）、或者 `ModelAndView`
- 第 2 种：返回 json 格式数据，需在方法上添加 `@ResponseBody` 注解
- 第 3 种：方法返回值为 `void` 或者 `return null`；此时需要在方法中自己通过 `HttpServletResponse` 对象来主动向客户端输出结果。
