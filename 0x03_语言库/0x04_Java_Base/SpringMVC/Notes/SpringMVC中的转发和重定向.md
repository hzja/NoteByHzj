JavaSpringMVC
<a name="Bms7g"></a>
## 1、转发
<a name="dMold"></a>
### 1.1、Servlet 原生实现转发
```java
request.getRequestDispatcher(path).forward(request,response);
```
<a name="F0Pwd"></a>
### 1.2、SpringMVC 实现转发
接口需满足下面这 3 条的会被 SpringMVC 当做转发进行处理

- 接口返回值为 String 类型
- 返回值格式：`forward:转发的路径`
- 方法或者类上不要标注`@ResponseBody` 注解

案例代码如下，当访问/forward/test1的时候，返回值以`forward:`开头，SpringMVC 会将请求转发到/forward/test2
```java
@RequestMapping("/forward/test1")
public String test1() {
    return "forward:/forward/test2";
}

@RequestMapping(value = "/forward/test2", produces = "text/html;charset=UTF-8")
@ResponseBody
public String test2() {
    return "我是test2";
}
```
测试效果：浏览器中访问/forward/test1输出<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1684054134422-32e4e68c-13ba-4c46-b31e-1db325735432.png#averageHue=%23e3c087&clientId=ubc60f644-65fc-4&from=paste&id=uc058bd7f&originHeight=67&originWidth=429&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=shadow&taskId=u84d1b818-528f-4bdd-bafa-b429704c051&title=)
<a name="EyA2B"></a>
## 2、重定向
<a name="F4EIx"></a>
### 2.1、Servlet 原生实现重定向
```java
response.sendRedirect(url);
```
<a name="NYAgD"></a>
### 2.2、SpringMVC 实现重定向
接口需满足下面这 3 条的会被 SpringMVC 当做转发进行处理

- 接口返回值为 String 类型
- 返回值格式：`redirect:重定向的路径`
- 方法或者类上不要标注`@ResponseBody` 注解

案例代码如下，当访问/redirect/test1的时候，返回值以`redirect:`开头，SpringMVC 会将请求转发到/redirect/test2
```java
@RequestMapping("/redirect/test1")
public String test1() {
    return "redirect:/redirect/test2";
}

@RequestMapping(value = "/redirect/test2", produces = "text/html;charset=UTF-8")
@ResponseBody
public String test2() {
    return "我是test2";
}
```
测试效果：浏览器中访问/redirect/test1，效果如下，浏览器地址栏变成/redirect/test2了<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1684054134537-5a1ee781-c98c-4ad8-8505-eb6b3037af82.png#averageHue=%23f0e8c0&clientId=ubc60f644-65fc-4&from=paste&id=u21dffdc5&originHeight=78&originWidth=413&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=shadow&taskId=u8b0aec2f-2dc1-4ed6-add1-e2da3e4262a&title=)
<a name="cC2ZV"></a>
### 2.3、重定向传参方式 1：手动在地址后拼接参数
直接在重定向的 url 中直接拼接参数，如`redirect:地址?name=路人&age=30`，如
```java
@RequestMapping("/redirect/test1")
public String test1() {
    return "redirect:/redirect/test2?name=路人&age=30";
}
```
<a name="gc7Gf"></a>
### 2.4、重定向传参方式 2：`RedirectAttributes.addAttribute("参数","值")`
<a name="JxQRr"></a>
#### 用法

- 接口中需要有一个类型为`RedirectAttributes`的参数
- 调用`redirectAttributes.addAttribute("参数","值")`，放入重定向需要传递的参数
- 原理：通过`redirectAttributes.addAttribute`丢进去的参数，SpringMVC 重定向的时候，会自动将这些参数以?参数1=值1&参数2=值2拼接到重定向的地址上，类似于上面的方式 1。
<a name="TGBXp"></a>
#### 案例代码
访问接口 test3，会被重定向到 test4，顺便传递了 2 个参数
```java
@RequestMapping("/redirect/test3")
public String test3(RedirectAttributes redirectAttributes) {
    redirectAttributes.addAttribute("name", "路人");
    redirectAttributes.addAttribute("age", 30);
    return "redirect:/redirect/test4";
}

@RequestMapping(value = "/redirect/test4", produces = MediaType.APPLICATION_JSON_VALUE)
@ResponseBody
public Map<String, Object> test4(@RequestParam("name") String name, @RequestParam("age") int age) {
    Map<String, Object> result = new LinkedHashMap<>();
    result.put("name", name);
    result.put("age", age);
    return result;
}
```
<a name="egN5d"></a>
#### 测试效果
浏览器中访问/redirect/test3接口，会被重定向到/redirect/test4，效果如下，test3 方法中丢到`addAttribute`中的 2 个参数name和age，被自动拼接到地址后面了。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1684054134538-9b4c6268-1ff0-4cb6-b757-7834cd42c0ec.png#averageHue=%23fdfdfc&clientId=ubc60f644-65fc-4&from=paste&id=u8667cf0f&originHeight=125&originWidth=588&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=shadow&taskId=uf9f20821-b56e-4e81-b45c-06f8de3dc08&title=)
<a name="u8hA3"></a>
### 2.5、重定向传参方式 2：`RedirectAttributes.addFlashAttribute("参数","值")`
上面使用的是`RedirectAttributes`的`addAttribute`放入参数，这次要使用另外一个方法`addFlashAttribute`放入重定向需要传递的参数，具体有什么区别呢，请向下看。
<a name="nBfSP"></a>
#### 用法

- 接口中需要有一个类型为`RedirectAttributes`的参数
- 调用`redirectAttributes.addFlashAttribute("参数","值")`，这种方式传递的参数是被隐藏的，不会被拼接在地址后，内部是通过 session 共享数据来实现的。
- 被重定向到的接口，需要使用一个`org.springframework.ui.Model`或者`org.springframework.ui.ModelMap`类型的参数来接收传递过来的参数，调用`model.getAttribute("参数名")`可以获取传递过来的参数
<a name="cUq7L"></a>
#### 案例代码
访问接口 test5，会被重定向到 test6，顺便传递了 2 个参数
```java
@RequestMapping("/redirect/test5")
public String test5(RedirectAttributes redirectAttributes) {
    redirectAttributes.addFlashAttribute("name", "路人");
    redirectAttributes.addFlashAttribute("age", 30);
    return "redirect:/redirect/test6";
}

/**
 * 需要使用一个org.springframework.ui.Model或者org.springframework.ui.ModelMap类型的参数来接收传递过来的参数，
 * 方法内部调用model.getAttribute("参数名")可以获取传递过来的参数
 *
 * @param model
 * @return
 */
@RequestMapping(value = "/redirect/test6", produces = MediaType.APPLICATION_JSON_VALUE)
@ResponseBody
public Map<String, Object> test6(Model model) {
    String name = (String) model.getAttribute("name");
    Integer age = (Integer) model.getAttribute("age");
    Map<String, Object> result = new LinkedHashMap<>();
    result.put("name", name);
    result.put("age", age);
    return result;
}
```
<a name="J0Y6o"></a>
#### 测试效果
浏览器中访问/redirect/test5接口，会被重定向到/redirect/test6，效果如下，参数传递成功了，传递是隐藏式的。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1684054134519-7a4f8b6b-fb60-4f6f-8f33-47def0c279d5.png#averageHue=%23fdfdfc&clientId=ubc60f644-65fc-4&from=paste&id=u27e870de&originHeight=124&originWidth=443&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=shadow&taskId=u0fb459d6-a45b-4bba-a1ea-240f3050411&title=)
<a name="IWsNk"></a>
#### 原理
`redirectAttributes.addFlashAttribute` 放入重定向需要传递的参数，SpringMVC 在重定向到新地址之前，会将这部分数据丢到 session 中，当重定向的请求过来后，SpringMVC 又会从 session 中拿到这部分数据，然后丢到 Model 或者 `ModelMap` 中，然后冲 session 中清理掉这部分数据。
<a name="fr5eq"></a>
### 2.6、`RedirectAttributes.addAttribute` 和 `RedirectAttributes.addFlashAttribute` 区别

- 都可以实现重定向传递参数
- `addAttribute` 传递的参数，最后会追加在新的地址上，而 `addFlashAttribute` 传递的参数是隐藏式的
- `addFlashAttribute` 可以传递大量的信息，不过 `addFlashAttribute` 有个弊端，重定向到新地址之后，如下图，如果此时用户刷新页面，传递的参数取不到了，就丢失了，建议使用方式 1 和方式 2；方式 3 可以作为了解。

![](https://cdn.nlark.com/yuque/0/2023/png/396745/1684054134369-043fbef6-7301-477a-8bf0-e456cd2a6de2.png#averageHue=%23fdfcfb&clientId=ubc60f644-65fc-4&from=paste&id=uadfe3ac2&originHeight=128&originWidth=418&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=shadow&taskId=u98acaadd-92c2-4bb6-8d57-fb4f95ed9b6&title=)
