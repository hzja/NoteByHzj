Java SpringBoot
<a name="Wlrek"></a>
## 一、Spring Web MVC 与 Spring Bean 注解
<a name="FLMih"></a>
### Spring Web MVC 注解
<a name="gF1TI"></a>
#### `@RequestMapping`
`@RequestMapping`注解的主要用途是将Web请求与请求处理类中的方法进行映射。Spring MVC和Spring WebFlux都通过`RquestMappingHandlerMapping`和`RequestMappingHndlerAdapter`两个类来提供对`@RequestMapping`注解的支持。<br />`@RequestMapping`注解对请求处理类中的请求处理方法进行标注；`@RequestMapping`注解拥有以下的六个配置属性：

- `value`:映射的请求URL或者其别名
- `method`:兼容HTTP的方法名
- `params`:根据HTTP参数的存在、缺省或值对请求进行过滤
- `header`:根据HTTP Header的存在、缺省或值对请求进行过滤
- `consume`:设定在HTTP请求正文中允许使用的媒体类型
- `product`:在HTTP响应体中允许使用的媒体类型

提示：在使用`@RequestMapping`之前，请求处理类还需要使用`@Controller`或`@RestController`进行标记<br />下面是使用`@RequestMapping`的两个示例：<br />![2021-10-07-14-20-55-307531.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1633587662583-ab46c2de-518c-4929-82a1-dadc50f1a4b0.png#clientId=ud4b4041a-51f6-4&from=ui&id=u5cf138dd&originHeight=426&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=63822&status=done&style=none&taskId=u8cfcb008-7a4f-4aed-b635-98aa35013a5&title=)<br />`@RequestMapping`还可以对类进行标记，这样类中的处理方法在映射请求路径时，会自动将类上`@RequestMapping`设置的value拼接到方法中映射路径之前，如下：<br />![2021-10-07-14-20-55-500521.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1633587662602-d92a4fbe-6bd5-4308-9c5c-29cb2a60de18.png#clientId=ud4b4041a-51f6-4&from=ui&id=qBZKo&originHeight=503&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=83600&status=done&style=none&taskId=u5d1b6aa5-4ab8-4a7e-99d6-52d19b666c3&title=)
```java
@RestController
public class ControllerTest {

    @RequestMapping(value = "/demo/home", method = RequestMethod.GET)
    public String home() {
        return "home";
    }

    // controller–限制接收post 请求以及consumes="application/json"。
    @RequestMapping(value = "/testMethod", method = RequestMethod.POST,consumes="application/json")
    public String testMethod() {
        System.out.println("testMethod");
        return "SUCCESS";
    }

    // 返回值类型是json格式
    // text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8
    // 其中最后一项 ：*/*;q=0.8。
    // 该项表明可以接收任何类型的，权重系数0.8表明如果前面几种类型不能正常接收。则使用该项进行自动分析。
    // application/json 几种主流浏览器都可以自动解析。
    @RequestMapping(value = "/testMethod", method = RequestMethod.POST,produces="application/json")
    @ResponseBody
    public RestMessage testMethod2(Model model) throws IOException {


        return new RestMessage("SUCCESS");
    }

    // 设定必须包含username 和age两个参数，且age参数不为10 (可以有多个参数)。
    @RequestMapping(value = "testParams", params = { "username","age!=10" })
    public String testParams() {
        System.out.println("testParamsAndHeaders");
        return "SUCCESS";
    }

    // 设定请求头中第一语言必须为US。
    // 必须包含username 和age两个参数，且age参数不为10 (可以有多个参数)。
    @RequestMapping(value = "testParamsAndHeaders", params = { "username","age!=10" }, headers = { "Accept-Language=US,zh;q=0.8" })
    public String testParamsAndHeaders() {
        System.out.println("testParamsAndHeaders");
        return "SUCCESS";
    }
}
```

---

<a name="Cuorr"></a>
#### `@RequestBody`
`@RequestBody`在处理请求方法的参数列表中使用，它可以将请求主体中的参数绑定到一个对象中，请求主体参数是通过`HttpMessageConverter`传递的，根据请求主体中的参数名与对象的属性名进行匹配并绑定值。此外，还可以通过`@Valid`注解对请求主体中的参数进行校验。<br />下面是一个使用`@RequestBody`的示例：<br />![2021-10-07-14-20-55-622516.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1633587662570-6847a8e5-b3ee-48ba-b929-c47a2b38991d.jpeg#clientId=ud4b4041a-51f6-4&from=ui&id=AtvgR&originHeight=578&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=40887&status=done&style=none&taskId=u740821b3-a883-4a1d-bd1c-4efe01f9310&title=)

---

<a name="Foeh5"></a>
#### `@GetMapping`
`@GetMapping`注解用于处理HTTP GET请求，并将请求映射到具体的处理方法中。具体来说，`@GetMapping`是一个组合注解，它相当于是`@RequestMapping(method=RequestMethod.GET)`的快捷方式。<br />下面是`@GetMapping`的一个使用示例：<br />![2021-10-07-14-20-55-754513.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1633587689428-e37207df-8ddb-43ca-878f-9b064e9f2dc7.jpeg#clientId=ud4b4041a-51f6-4&from=ui&id=ufa7bafe8&originHeight=592&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=42535&status=done&style=none&taskId=uf3c94368-1309-404d-927f-162bb68b98d&title=)

---

<a name="feH4O"></a>
#### `@PostMapping`
`@PostMapping`注解用于处理HTTP POST请求，并将请求映射到具体的处理方法中。`@PostMapping`与`@GetMapping`一样，也是一个组合注解，它相当于是`@RequestMapping(method=HttpMethod.POST)`的快捷方式。<br />下面是使用`@PostMapping`的一个示例：<br />![2021-10-07-14-20-55-819515.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1633587689434-523570dd-c7d2-430d-938f-f2a9df589126.jpeg#clientId=ud4b4041a-51f6-4&from=ui&id=yTlnI&originHeight=578&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=40887&status=done&style=none&taskId=u40f58015-7535-438e-a967-26dca2ce8c9&title=)

---

<a name="xv6hd"></a>
#### `@PutMapping`
`@PutMapping`注解用于处理HTTP PUT请求，并将请求映射到具体的处理方法中，`@PutMapping`是一个组合注解，相当于是`@RequestMapping(method=HttpMethod.PUT)`的快捷方式。<br />下面是使用`@PutMapping`的一个示例：<br />![2021-10-07-14-20-55-908563.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1633587689449-a5c3e78c-0a13-4c16-891b-660bc62dabad.jpeg#clientId=ud4b4041a-51f6-4&from=ui&id=W30go&originHeight=649&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=61181&status=done&style=none&taskId=ub6ca89a9-9c7d-4f35-aebe-1fdae75693d&title=)

---

<a name="eiPoD"></a>
#### `@DeleteMapping`
`@DeleteMapping`注解用于处理HTTP DELETE请求，并将请求映射到删除方法中。`@DeleteMapping`是一个组合注解，它相当于是`@RequestMapping(method=HttpMethod.DELETE)`的快捷方式。<br />下面是使用`@DeleteMapping`的一个示例：<br />![2021-10-07-14-20-55-998543.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1633587713345-a0d0e3ae-620b-4ccc-a56e-879ce6af6490.jpeg#clientId=ud4b4041a-51f6-4&from=ui&id=udcf61990&originHeight=563&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=52079&status=done&style=none&taskId=u12db207b-04ba-4d54-b7f6-acaa0353176&title=)

---

<a name="SMvZZ"></a>
#### `@PatchMapping`
`@PatchMapping`注解用于处理HTTP PATCH请求，并将请求映射到对应的处理方法中。`@PatchMapping`相当于是`@RequestMapping(method=HttpMethod.PATCH)`的快捷方式。<br />下面是一个简单的示例：<br />![2021-10-07-14-20-56-064511.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1633587713331-7e9614c6-e7b6-480c-9364-52108bf93ced.jpeg#clientId=ud4b4041a-51f6-4&from=ui&id=YsGav&originHeight=326&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=27814&status=done&style=none&taskId=ubce2df99-863c-4388-b7c0-46725c28ffe&title=)

---

<a name="xQptL"></a>
#### `@ControllerAdvice`
`@ControllerAdvice`是`@Component`注解的一个延伸注解，Spring会自动扫描并检测被`@ControllerAdvice`所标注的类。`@ControllerAdvice`需要和`@ExceptionHandler`、`@InitBinder`以及`@ModelAttribute`注解搭配使用，主要是用来处理控制器所抛出的异常信息。<br />首先，需要定义一个被`@ControllerAdvice`所标注的类，在该类中，定义一个用于处理具体异常的方法，并使用`@ExceptionHandler`注解进行标记。<br />此外，在有必要的时候，可以使用`@InitBinder`在类中进行全局的配置，还可以使用`@ModelAttribute`配置与视图相关的参数。使用`@ControllerAdvice`注解，就可以快速的创建统一的，自定义的异常处理类。<br />下面是一个使用`@ControllerAdvice`的示例代码：<br />![2021-10-07-14-20-56-152508.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1633587713469-e9a279c6-e7aa-47c2-aae9-f83ea14928bb.jpeg#clientId=ud4b4041a-51f6-4&from=ui&id=ARjgO&originHeight=821&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=75474&status=done&style=none&taskId=u6e15cbeb-8dbc-4760-bf6c-5e9019cbf26&title=)

---

<a name="CrVyr"></a>
#### `@ResponseBody`
`@ResponseBody`会自动将控制器中方法的返回值写入到HTTP响应中。特别的，`@ResponseBody`注解只能用在被`@Controller`注解标记的类中。如果在被`@RestController`标记的类中，则方法不需要使用`@ResponseBody`注解进行标注。`@RestController`相当于是`@Controller`和`@ResponseBody`的组合注解。<br />下面是使用该注解的一个示例<br />![2021-10-07-14-20-56-229519.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1633587736175-d53095ef-5dc9-4fd2-a1e1-06e561ef6b07.png#clientId=ud4b4041a-51f6-4&from=ui&id=uf245701e&originHeight=288&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=57319&status=done&style=none&taskId=uc03f36e1-dfe8-4951-81c7-1859b4239fb&title=)

---

<a name="eDRdE"></a>
#### `@ExceptionHandler`
`@ExceptionHander`注解用于标注处理特定类型异常类所抛出异常的方法。当控制器中的方法抛出异常时，Spring会自动捕获异常，并将捕获的异常信息传递给被`@ExceptionHandler`标注的方法。<br />下面是使用该注解的一个示例：<br />![2021-10-07-14-20-56-381516.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1633587736168-347e4e46-4fe8-43b0-a640-10bc8d276159.jpeg#clientId=ud4b4041a-51f6-4&from=ui&id=AgpUx&originHeight=335&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=32862&status=done&style=none&taskId=u8b5af742-166f-44b3-b623-c54fc6bded2&title=)

---

<a name="foWA9"></a>
#### `@ResponseStatus`
`@ResponseStatus`注解可以标注请求处理方法。使用此注解，可以指定响应所需要的HTTP STATUS。特别地，可以使用`HttpStauts`类对该注解的value属性进行赋值。<br />下面是使用`@ResponseStatus`注解的一个示例：<br />![2021-10-07-14-20-56-472522.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1633587736170-fab441b0-0858-45a6-90df-534ed9c8835c.jpeg#clientId=ud4b4041a-51f6-4&from=ui&id=PoS5r&originHeight=364&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=38912&status=done&style=none&taskId=u719c54d5-ebaf-4859-8b3f-a1056b8f25e&title=)

---

<a name="j745z"></a>
#### `@PathVariable`
`@PathVariable`注解是将方法中的参数绑定到请求URI中的模板变量上。可以通过`@RequestMapping`注解来指定URI的模板变量，然后使用`@PathVariable`注解将方法中的参数绑定到模板变量上。<br />特别地，`@PathVariable`注解允许开发者使用value或name属性来给参数取一个别名。下面是使用此注解的一个示例：<br />![2021-10-07-14-20-56-577517.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1633587770969-ad00baeb-ead4-4868-8dc8-ba66b93ea21c.jpeg#clientId=ud4b4041a-51f6-4&from=ui&id=u435172e8&originHeight=278&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=25675&status=done&style=none&taskId=ub8f0cae1-ea58-47d5-9ffb-d0da5cf234f&title=)<br />模板变量名需要使用`{ }`进行包裹，如果方法的参数名与URI模板变量名一致，则在`@PathVariable`中就可以省略别名的定义。<br />下面是一个简写的示例：<br />![2021-10-07-14-20-56-742508.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1633587770982-f2e4b148-f6c9-4a4d-a985-65136379510b.jpeg#clientId=ud4b4041a-51f6-4&from=ui&id=X4eEj&originHeight=278&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=23157&status=done&style=none&taskId=uf62893ac-a9e9-4556-93be-859bf9f4c95&title=)<br />提示：如果参数是一个非必须的，可选的项，则可以在`@PathVariable`中设置`require = false`

---

<a name="xJrip"></a>
#### `@RequestParam`
`@RequestParam`注解用于将方法的参数与Web请求的传递的参数进行绑定。使用`@RequestParam`可以轻松的访问HTTP请求参数的值。<br />下面是使用该注解的代码示例：<br />![2021-10-07-14-20-56-863519.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1633587770983-081a38db-56e5-4d97-9004-928843c9732c.png#clientId=ud4b4041a-51f6-4&from=ui&id=LULlP&originHeight=278&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=56109&status=done&style=none&taskId=u794b2a28-d036-4ec0-a275-8f98ee06bd8&title=)<br />该注解的其他属性配置与`@PathVariable`的配置相同，特别的，如果传递的参数为空，还可以通过`defaultValue`设置一个默认值。示例代码如下：<br />![2021-10-07-14-20-56-993515.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1633587808497-1f4dec58-3425-4741-a00b-fd674a95aab4.jpeg#clientId=ud4b4041a-51f6-4&from=ui&id=u7304583b&originHeight=364&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=31603&status=done&style=none&taskId=u1057f83a-28c7-4edd-85cb-62fae03ff08&title=)

---

<a name="Xa7Lz"></a>
#### `@Controller`
`@Controller`是`@Component`注解的一个延伸，Spring会自动扫描并配置被该注解标注的类。此注解用于标注Spring MVC的控制器。下面是使用此注解的示例代码：<br />![2021-10-07-14-20-57-127513.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1633587808522-55cd3a66-d802-4d2c-9325-45837f9338cf.jpeg#clientId=ud4b4041a-51f6-4&from=ui&id=IxUJa&originHeight=543&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=39582&status=done&style=none&taskId=u3139c79c-3abb-4eba-a23c-f6dfbd2fafa&title=)

---

<a name="h2yIh"></a>
#### `@RestController`
`@RestController`是在Spring 4.0开始引入的，这是一个特定的控制器注解。此注解相当于`@Controller`和`@ResponseBody`的快捷方式。当使用此注解时，不需要再在方法上使用`@ResponseBody`注解。<br />下面是使用此注解的示例代码：<br />![2021-10-07-14-20-57-190513.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1633587808523-bd6f9dd0-7c5f-4572-912e-b6f73e1c6721.jpeg#clientId=ud4b4041a-51f6-4&from=ui&id=o7Lm6&originHeight=511&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=35943&status=done&style=none&taskId=ued00cf9e-f030-43ed-95b2-4f8d701f535&title=)

---

<a name="C6emw"></a>
#### `@ModelAttribute`
通过此注解，可以通过模型索引名称来访问已经存在于控制器中的model。下面是使用此注解的一个简单示例：<br />![2021-10-07-14-20-57-246516.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1633587831450-9f418285-1d40-41cf-a9af-48b521aaaaa7.png#clientId=ud4b4041a-51f6-4&from=ui&id=u92d1bcac&originHeight=285&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=53711&status=done&style=none&taskId=u85d7585f-dfea-41cd-97ee-ed29acc504f&title=)<br />与`@PathVariable`和`@RequestParam`注解一样，如果参数名与模型具有相同的名字，则不必指定索引名称，简写示例如下：<br />![2021-10-07-14-20-57-404514.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1633587831455-9e25a941-6cb4-4132-ad48-d2d5f1465e9c.png#clientId=ud4b4041a-51f6-4&from=ui&id=vg1vE&originHeight=326&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=60362&status=done&style=none&taskId=u66aa65aa-d049-47cc-9ad5-29d088ddf21&title=)<br />特别地，如果使用`@ModelAttribute`对方法进行标注，Spring会将方法的返回值绑定到具体的Model上。示例如下：<br />![2021-10-07-14-20-57-502546.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1633587831477-50d76934-78c2-4d53-ba9c-664eb8eb4565.png#clientId=ud4b4041a-51f6-4&from=ui&id=j1nxT&originHeight=412&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=63108&status=done&style=none&taskId=u35727afe-ef70-4969-a68a-81f7408c001&title=)<br />在Spring调用具体的处理方法之前，被`@ModelAttribute`注解标注的所有方法都将被执行。

---

<a name="I1Ddh"></a>
#### `@CrossOrigin`
`@CrossOrigin`注解将为请求处理类或请求处理方法提供跨域调用支持。如果将此注解标注类，那么类中的所有方法都将获得支持跨域的能力。使用此注解的好处是可以微调跨域行为。使用此注解的示例如下：<br />![2021-10-07-14-20-57-574548.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1633587855581-63d407a2-dc4b-4260-8085-973cfe23b8f3.jpeg#clientId=ud4b4041a-51f6-4&from=ui&id=ud560a6e1&originHeight=335&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=25462&status=done&style=none&taskId=u33405526-416c-46a2-958a-0746d809eb2&title=)

---

<a name="X0FCF"></a>
#### `@InitBinder`
`@InitBinder`注解用于标注初始化`WebDataBinider`的方法，该方法用于对Http请求传递的表单数据进行处理，如时间格式化、字符串处理等。下面是使用此注解的示例：<br />![2021-10-07-14-20-57-658522.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1633587855604-0239b23e-6033-4884-b676-6a812e0ac76e.png#clientId=ud4b4041a-51f6-4&from=ui&id=F9oYZ&originHeight=263&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=52107&status=done&style=none&taskId=u92731b9f-a674-49da-bda4-d519737ba9e&title=)
<a name="gEY6v"></a>
## 二、Spring Bean 注解
在本小节中，主要列举与Spring Bean相关的4个注解以及它们的使用方式。
<a name="Jxpv5"></a>
#### `@ComponentScan`
`@ComponentScan`注解用于配置Spring需要扫描的被组件注解注释的类所在的包。可以通过配置其`basePackages`属性或者value属性来配置需要扫描的包路径。value属性是`basePackages`的别名。此注解的用法如下：

---

<a name="xZVVT"></a>
#### `@Component`
`@Component`注解用于标注一个普通的组件类，它没有明确的业务范围，只是通知Spring被此注解的类需要被纳入到Spring Bean容器中并进行管理。此注解的使用示例如下：<br />![2021-10-07-14-20-57-739547.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1633587855611-6adde38c-1596-483d-afc7-cf4f2b498428.png#clientId=ud4b4041a-51f6-4&from=ui&id=cnk0h&originHeight=377&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=45627&status=done&style=none&taskId=u997a9b65-7eb0-44c6-bf9e-1de64c27183&title=)

---

<a name="PttM9"></a>
#### `@Service`
`@Service`注解是`@Component`的一个延伸（特例），它用于标注业务逻辑类。与`@Component`注解一样，被此注解标注的类，会自动被Spring所管理。下面是使用`@Service`注解的示例：<br />![2021-10-07-14-20-57-842524.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1633588013384-b8808e1c-6956-45ff-b8eb-d384ae5dc5fa.jpeg#clientId=ud4b4041a-51f6-4&from=ui&id=u90141943&originHeight=933&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=55212&status=done&style=none&taskId=u4db0b9bc-9c2d-45ae-9f0a-865ac1de503&title=)

---

<a name="Twq16"></a>
#### `@Repository`
`@Repository`注解也是`@Component`注解的延伸，与`@Component`注解一样，被此注解标注的类会被Spring自动管理起来，`@Repository`注解用于标注DAO层的数据持久化类。此注解的用法如下：<br />![2021-10-07-14-20-58-260000.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1633588013377-dc9b0ccc-0cdf-4454-8866-5b35b3de80d1.jpeg#clientId=ud4b4041a-51f6-4&from=ui&id=pHzPQ&originHeight=471&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=24637&status=done&style=none&taskId=uc9607057-da7c-4d74-b97f-15906132047&title=)
<a name="x1WXX"></a>
## 三、Spring Dependency Inject 与 Bean Scops注解
<a name="P3Txy"></a>
### Spring DI注解
<a name="ZJuJm"></a>
#### `@DependsOn`
`@DependsOn`注解可以配置Spring IoC容器在初始化一个Bean之前，先初始化其他的Bean对象。下面是此注解使用示例代码：<br />![2021-10-07-14-20-58-427039.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1633588013399-876248bb-fc4c-41e0-9531-1ea9289731ad.jpeg#clientId=ud4b4041a-51f6-4&from=ui&id=tpd5r&originHeight=1817&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=82418&status=done&style=none&taskId=u25f9f85e-0a64-459c-96dd-ce554c074be&title=)

---

<a name="vtSeU"></a>
#### `@Bean`
`@Bean`注解主要的作用是告知Spring，被此注解所标注的类将需要纳入到Bean管理工厂中。`@Bean`注解的用法很简单，在这里，着重介绍`@Bean`注解中`initMethod`和`destroyMethod`的用法。示例如下：<br />![2021-10-07-14-20-58-495996.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1633588061745-18ad2050-8dba-43ac-8cf6-9bdb2df44797.jpeg#clientId=ud4b4041a-51f6-4&from=ui&id=u38291222&originHeight=874&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=47253&status=done&style=none&taskId=u87d72d08-8775-4e8b-b642-ccd8269060e&title=)
<a name="wQUxd"></a>
### Scops注解
<a name="WQrQK"></a>
#### `@Scope`
`@Scope`注解可以用来定义`@Component`标注的类的作用范围以及`@Bean`所标记的类的作用范围。`@Scope`所限定的作用范围有：`singleton`、`prototype`、`request`、`session`、`globalSession`或者其他的自定义范围。这里以prototype为例子进行讲解。<br />当一个Spring Bean被声明为`prototype`（原型模式）时，在每次需要使用到该类的时候，Spring IoC容器都会初始化一个新的改类的实例。在定义一个Bean时，可以设置Bean的scope属性为`prototype：scope="prototype"`,也可以使用`@Scope`注解设置，如下：
```java
@Scope(value=ConfigurableBeanFactory.SCOPE_PROPTOTYPE)
```
下面将给出两种不同的方式来使用`@Scope`注解，示例代码如下：<br />![2021-10-07-14-20-58-561041.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1633588061782-90c21293-e652-4818-a7b6-53b8db7db777.jpeg#clientId=ud4b4041a-51f6-4&from=ui&id=sBEPI&originHeight=1207&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=62597&status=done&style=none&taskId=uc6cac521-ba84-4af0-ba26-be799338418&title=)

---

<a name="n6lvG"></a>
#### `@Scope` 单例模式
当`@Scope`的作用范围设置成Singleton时，被此注解所标注的类只会被Spring IoC容器初始化一次。在默认情况下，Spring IoC容器所初始化的类实例都为`singleton`。同样的原理，此情形也有两种配置方式，示例代码如下：<br />![2021-10-07-14-20-58-623029.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1633588061739-05e134fc-c5e9-4585-9ea3-6725e2edcb4a.png#clientId=ud4b4041a-51f6-4&from=ui&id=MyZhz&originHeight=915&originWidth=819&originalType=binary&ratio=1&rotation=0&showTitle=false&size=18570&status=done&style=none&taskId=ua0173707-8800-4470-93aa-540b08af593&title=)
<a name="wvVuC"></a>
## 四、容器配置注解
<a name="FDhG1"></a>
### `@Autowired`
`@Autowired`注解用于标记Spring将要解析和注入的依赖项。此注解可以作用在构造函数、字段和setter方法上。
<a name="Jp309"></a>
#### 作用于构造函数
下面是`@Autowired`注解标注构造函数的使用示例：<br />![2021-10-07-14-20-58-749994.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1633588100987-54cb3481-cf4b-48ca-8305-f8105de52ae3.png#clientId=ud4b4041a-51f6-4&from=ui&id=ubc26f9c9&originHeight=348&originWidth=763&originalType=binary&ratio=1&rotation=0&showTitle=false&size=5840&status=done&style=none&taskId=ua0941595-b99f-4668-b6f0-fcb1ce883ac&title=)

---

<a name="iGfZF"></a>
#### 作用于setter方法
下面是`@Autowired`注解标注setter方法的示例代码：<br />![2021-10-07-14-20-58-821017.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1633588100984-75d45909-1de8-42c5-8d99-c23722b1303e.png#clientId=ud4b4041a-51f6-4&from=ui&id=fg7lV&originHeight=348&originWidth=763&originalType=binary&ratio=1&rotation=0&showTitle=false&size=6077&status=done&style=none&taskId=u7f78414a-b53a-4e2d-8585-87610f5bbc0&title=)

---

<a name="bTWrF"></a>
#### 作用于字段
`@Autowired`注解标注字段是最简单的，只需要在对应的字段上加入此注解即可，示例代码如下：<br />![2021-10-07-14-20-58-914994.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1633588100990-f5a902f3-0fb8-4307-83c2-4a1fd90a5918.png#clientId=ud4b4041a-51f6-4&from=ui&id=VnUB5&originHeight=264&originWidth=763&originalType=binary&ratio=1&rotation=0&showTitle=false&size=4355&status=done&style=none&taskId=u50d44d90-ee50-46b3-b038-07e82f9ae62&title=)
<a name="fgZhB"></a>
### `@Primary`
当系统中需要配置多个具有相同类型的bean时，`@Primary`可以定义这些Bean的优先级。下面将给出一个实例代码来说明这一特性：<br />![2021-10-07-14-20-58-984990.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1633588164627-1a78fe15-1c8d-4d0a-8d8b-46e092879091.png#clientId=ud4b4041a-51f6-4&from=ui&id=CuS9C&originHeight=1104&originWidth=795&originalType=binary&ratio=1&rotation=0&showTitle=false&size=26503&status=done&style=none&taskId=u67709de4-8a6c-4bba-bc23-8637a2fdb12&title=)<br />输出结果：
```
this is send DingDing method message.
```
<a name="j5ih0"></a>
### `@PostConstruct`与`@PreDestroy`
值得注意的是，这两个注解不属于Spring，它们是源于JSR-250中的两个注解，位于common-annotations.jar中。`@PostConstruct`注解用于标注在Bean被Spring初始化之前需要执行的方法。`@PreDestroy`注解用于标注Bean被销毁前需要执行的方法。下面是具体的示例代码：<br />![2021-10-07-14-20-59-027995.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1633588163517-9ec11fdd-0276-4456-af9b-bfd182f0523a.png#clientId=ud4b4041a-51f6-4&from=ui&id=u1b1a321f&originHeight=495&originWidth=795&originalType=binary&ratio=1&rotation=0&showTitle=false&size=9230&status=done&style=none&taskId=u15743d1e-e968-4f94-b61d-e5c5abdd54a&title=)
<a name="r5VH8"></a>
### `@Qualifier`
当系统中存在同一类型的多个Bean时，`@Autowired`在进行依赖注入的时候就不知道该选择哪一个实现类进行注入。此时，可以使用`@Qualifier`注解来微调，帮助`@Autowired`选择正确的依赖项。下面是一个关于此注解的代码示例：<br />![2021-10-07-14-20-59-088991.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1633588164665-9c9fc28e-13f6-4d47-ba62-e2f48025c971.png#clientId=ud4b4041a-51f6-4&from=ui&id=Sf0Oj&originHeight=1062&originWidth=795&originalType=binary&ratio=1&rotation=0&showTitle=false&size=26506&status=done&style=none&taskId=u0916b30a-5110-4f64-a0db-91ef6b7162e&title=)
<a name="m2vxH"></a>
## 五、Spring Boot注解
<a name="IeeHS"></a>
### `@SpringBootApplication`
`@SpringBootApplication`注解是一个快捷的配置注解，在被它标注的类中，可以定义一个或多个Bean，并自动触发自动配置Bean和自动扫描组件。此注解相当于`@Configuration`、`@EnableAutoConfiguration`和`@ComponentScan`的组合。<br />在Spring Boot应用程序的主类中，就使用了此注解。示例代码如下：
```java
@SpringBootApplication
public class Application{
    public static void main(String [] args){
        SpringApplication.run(Application.class,args);
    }
}
```
<a name="RI9pG"></a>
### `@EnableAutoConfiguration`
`@EnableAutoConfiguration`注解用于通知Spring，根据当前类路径下引入的依赖包，自动配置与这些依赖包相关的配置项。

---

<a name="TfRJQ"></a>
### `@ConditionalOnClass`与`@ConditionalOnMissingClass`
这两个注解属于类条件注解，它们根据是否存在某个类作为判断依据来决定是否要执行某些配置。下面是一个简单的示例代码：
```java
@Configuration
@ConditionalOnClass(DataSource.class)
class MySQLAutoConfiguration {
    //...
}
```

---

<a name="ZB8L3"></a>
### `@ConditionalOnBean`与`@ConditionalOnMissingBean`
这两个注解属于对象条件注解，根据是否存在某个对象作为依据来决定是否要执行某些配置方法。示例代码如下：
```java
@Bean
@ConditionalOnBean(name="dataSource")
LocalContainerEntityManagerFactoryBean entityManagerFactory(){
    //...
}
@Bean
@ConditionalOnMissingBean
public MyBean myBean(){
    //...
}
```

---

<a name="qNNiZ"></a>
### `@ConditionalOnProperty`
`@ConditionalOnProperty`注解会根据Spring配置文件中的配置项是否满足配置要求，从而决定是否要执行被其标注的方法。示例代码如下：
```java
@Bean
@ConditionalOnProperty(name="alipay",havingValue="on")
Alipay alipay(){
    return new Alipay();
}
```

---

<a name="N7qvt"></a>
### `@ConditionalOnResource`
此注解用于检测当某个配置文件存在使，则触发被其标注的方法，下面是使用此注解的代码示例：
```java
@ConditionalOnResource(resources = "classpath:website.properties")
Properties addWebsiteProperties(){
    //...
}

```

---

<a name="ILIeV"></a>
### `@ConditionalOnWebApplication`与`@ConditionalOnNotWebApplication`
这两个注解用于判断当前的应用程序是否是Web应用程序。如果当前应用是Web应用程序，则使用Spring WebApplicationContext，并定义其会话的生命周期。下面是一个简单的示例：
```java
@ConditionalOnWebApplication
HealthCheckController healthCheckController(){
    //...
}
```

---

<a name="zQEEq"></a>
### `@ConditionalExpression`
此注解可以控制更细粒度的基于表达式的配置条件限制。当表达式满足某个条件或者表达式为真的时候，将会执行被此注解标注的方法。
```java
@Bean
@ConditionalException("${localstore} && ${local == 'true'}")
LocalFileStore store(){
    //...
}
```

---

<a name="U0r3s"></a>
### `@Conditional`
`@Conditional`注解可以控制更为复杂的配置条件。在Spring内置的条件控制注解不满足应用需求的时候，可以使用此注解定义自定义的控制条件，以达到自定义的要求。下面是使用该注解的简单示例：
```java
@Conditioanl(CustomConditioanl.class)
CustomProperties addCustomProperties(){
    //...
}
```
