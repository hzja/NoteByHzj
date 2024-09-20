JavaSpringBoot<br />`@Autowired` 和 `@Resource` 都是 Spring/Spring Boot 项目中，用来进行依赖注入的注解。它们都提供了将依赖对象注入到当前对象的功能，但二者却有众多不同，并且这也是常见的面试题之一。`@Autowired` 和 `@Resource` 的区别主要体现在以下 5 点：

1. **来源不同；**
2. **依赖查找的顺序不同；**
3. **支持的参数不同；**
4. **依赖注入的支持不同；**
5. **编译器 IDEA 的提示不同。**
<a name="iyje7"></a>
## 1、来源不同
`**@Autowired**`** 和 **`**@Resource**`** 来自不同的“父类”，其中 **`**@Autowired**`** 是 Spring 定义的注解，而 **`**@Resource**`** 是 Java 定义的注解，它来自于 JSR-250（Java 250 规范提案）。**<br />小知识：JSR 是 Java Specification Requests 的缩写，意思是“Java 规范提案”。任何人都可以提交 JSR 给 Java 官方，但只有最终确定的 JSR，才会以 JSR-XXX 的格式发布，如 JSR-250，而被发布的 JSR 就可以看作是 Java 语言的规范或标准。
<a name="bdfyb"></a>
## 2、依赖查找顺序不同
依赖注入的功能，是通过先在 Spring IoC 容器中查找对象，再将对象注入引入到当前类中。而查找有分为两种实现：按名称（byName）查找或按类型（byType）查找，其中 `@Autowired` 和 `@Resource` 都是既使用了名称查找又使用了类型查找，但二者进行查找的顺序却截然相反。
<a name="C1L61"></a>
### **2.1 **`**@Autowired**`** 查找顺序**
`**@Autowired**`** 是先根据类型（**`**byType**`**）查找，如果存在多个 Bean 再根据名称（**`**byName**`**）进行查找**，它的具体查找流程如下：![](https://cdn.nlark.com/yuque/0/2022/png/396745/1662208083650-d282ec7d-87b3-4043-91b2-9fe9a5c4a504.png#clientId=u16ecc995-7112-4&from=paste&id=u6b1b5c99&originHeight=552&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u25efeeee-0643-4326-a3c3-7ddca909d4e&title=)关于以上流程，可以通过查看 Spring 源码中的 `org.springframework.beans.factory.annotation.AutowiredAnnotationBeanPostProcessor#postProcessPropertyValues` 实现分析得出，源码执行流程如下图所示：![](https://cdn.nlark.com/yuque/0/2022/png/396745/1662208083672-1b02e17c-5b5b-401d-bf31-c7a5f7a23ebd.png#clientId=u16ecc995-7112-4&from=paste&id=u0e216a23&originHeight=620&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ue5c4b900-9c50-4018-bc19-16d887a7778&title=)
<a name="Pa1ml"></a>
### **2.2 **`**@Resource**`** 查找顺序**
`**@Resource**`** 是先根据名称查找，如果（根据名称）查找不到，再根据类型进行查找**，它的具体流程如下图所示：![](https://cdn.nlark.com/yuque/0/2022/png/396745/1662208083749-65845780-1b37-4794-9dde-9c6c5f7445f1.png#clientId=u16ecc995-7112-4&from=paste&id=u81c0a09f&originHeight=411&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uf122cfd1-1968-4d9a-95b9-947376a1eb3&title=)关于以上流程可以在 Spring 源码的 `org.springframework.context.annotation.CommonAnnotationBeanPostProcessor#postProcessPropertyValues` 中分析得出。虽然 `@Resource` 是 JSR-250 定义的，但是由 Spring 提供了具体实现，它的源码实现如下：![](https://cdn.nlark.com/yuque/0/2022/png/396745/1662208083648-b4a42aee-6827-4769-9d14-0817617cd336.png#clientId=u16ecc995-7112-4&from=paste&id=u3c71eae9&originHeight=607&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u429bd0c3-5b22-4f91-b112-e13559d9e9e&title=)
<a name="ZKJGW"></a>
### **2.3 查找顺序小结**
由上面的分析可以得出：

- `**@Autowired**`** 先根据类型（**`**byType**`**）查找，如果存在多个（Bean）再根据名称（**`**byName**`**）进行查找；**
- `**@Resource**`** 先根据名称（**`**byName**`**）查找，如果（根据名称）查找不到，再根据类型（**`**byType**`**）进行查找。**
<a name="OJszV"></a>
## 3、支持的参数不同
`@Autowired` 和 `@Resource` 在使用时都可以设置参数，比如给 `@Resource` 注解设置 `name` 和 `type` 参数，实现代码如下：
```java
@Resource(name = "userinfo", type = UserInfo.class)
private UserInfo user;
```
但**二者支持的参数以及参数的个数完全不同，其中 **`**@Autowired**`** 只支持设置一个 **`**required**`** 的参数，而 **`**@Resource**`** 支持 7 个参数**，支持的参数如下图所示：![](https://cdn.nlark.com/yuque/0/2022/png/396745/1662208083671-16fa44a7-579c-4de8-af0c-33f2654db0dd.png#clientId=u16ecc995-7112-4&from=paste&id=u7ff2026a&originHeight=250&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uf56e3202-e4ec-4f3a-ad0a-433f4c502bf&title=)![](https://cdn.nlark.com/yuque/0/2022/png/396745/1662208083976-1c4d72a4-f5e2-494e-b738-31416a6673c6.png#clientId=u16ecc995-7112-4&from=paste&id=u2aaee162&originHeight=331&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u0e7ea44e-3ddd-44df-9908-1fc211416ad&title=)
<a name="qLM2Z"></a>
## 4、依赖注入的支持不同
`@Autowired` 和 `@Resource` 支持依赖注入的用法不同，常见依赖注入有以下 3 种实现：

1. **属性注入**
2. **构造方法注入**
3. **Setter 注入**

这 3 种实现注入的实现代码如下。
<a name="Wamy2"></a>
### a) 属性注入
```java
@RestController
public class UserController {
    // 属性注入
    @Autowired
    private UserService userService;

    @RequestMapping("/add")
    public UserInfo add(String username, String password) {
        return userService.add(username, password);
    }
}
```
<a name="gGaMe"></a>
### b) 构造方法注入
```java
@RestController
public class UserController {
    // 构造方法注入
    private UserService userService;

    @Autowired
    public UserController(UserService userService) {
        this.userService = userService;
    }

    @RequestMapping("/add")
    public UserInfo add(String username, String password) {
        return userService.add(username, password);
    }
}
```
<a name="alfBX"></a>
### c) Setter 注入
```java
@RestController
public class UserController {
    // Setter 注入
    private UserService userService;

    @Autowired
    public void setUserService(UserService userService) {
        this.userService = userService;
    }

    @RequestMapping("/add")
    public UserInfo add(String username, String password) {
        return userService.add(username, password);
    }
}
```
其中，`**@Autowired**`** 支持属性注入、构造方法注入和 Setter 注入，而 **`**@Resource**`** 只支持属性注入和 Setter 注入**，当使用 `@Resource` 实现构造方法注入时就会提示以下错误：![](https://cdn.nlark.com/yuque/0/2022/png/396745/1662208084103-59b3d45a-b404-4719-b1e9-178d3ca8a6fd.png#clientId=u16ecc995-7112-4&from=paste&id=ubf381d2a&originHeight=464&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u6a894cef-fe9f-41a3-9673-387173572a0&title=)
<a name="tSdvu"></a>
## 5、编译器提示不同
**当使用 IDEA 专业版在编写依赖注入的代码时，如果注入的是 Mapper 对象，那么使用 **`**@Autowired**`** 编译器会提示报错信息**，报错内容如下图所示：![](https://cdn.nlark.com/yuque/0/2022/png/396745/1662208084108-2ff07017-7d05-493f-af96-ef5f013ae211.png#clientId=u16ecc995-7112-4&from=paste&id=ucb475d1d&originHeight=450&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u2c216d44-42e8-4a02-b107-a9a0fc876fa&title=)虽然 IDEA 会出现报错信息，但程序是可以正常执行的。然后再**将依赖注入的注解更改为 **`**@Resource**`** 就不会出现报错信息了**，具体实现如下：![](https://cdn.nlark.com/yuque/0/2022/png/396745/1662208084195-e22429a3-9dc2-4924-ac2b-0f8e67658957.png#clientId=u16ecc995-7112-4&from=paste&id=u4c21f57d&originHeight=467&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u326e02ca-312a-49b7-89a5-f2e5395e570&title=)
<a name="YsASO"></a>
## 总结
`@Autowired` 和 `@Resource` 都是用来实现依赖注入的注解（在 Spring/Spring Boot 项目中），但二者却有着 5 点不同：

1. **来源不同：**`**@Autowired**`** 来自 Spring 框架，而 **`**@Resource**`** 来自于（Java）JSR-250；**
2. **依赖查找的顺序不同：**`**@Autowired**`** 先根据类型再根据名称查询，而 **`**@Resource**`** 先根据名称再根据类型查询；**
3. **支持的参数不同：**`**@Autowired**`** 只支持设置 1 个参数，而 **`**@Resource**`** 支持设置 7 个参数；**
4. **依赖注入的用法支持不同：**`**@Autowired**`** 既支持构造方法注入，又支持属性注入和 Setter 注入，而 **`**@Resource**`** 只支持属性注入和 Setter 注入；**
5. **编译器 IDEA 的提示不同：当注入 Mapper 对象时，使用 **`**@Autowired**`** 注解编译器会提示错误，而使用 **`**@Resource**`** 注解则不会提示错误。**
