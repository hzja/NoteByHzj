Java Spring Controller<br />测试代码示例：
```java
@Controller
public class ScopeTestController {

    private int num = 0;

    @RequestMapping("/testScope")
    public void testScope() {
        System.out.println(++num);
    }

    @RequestMapping("/testScope2")
    public void testScope2() {
        System.out.println(++num);
    }
}
```
首先访问 http://localhost:8080/testScope，得到的答案是 1；<br />然后再访问 http://localhost:8080/testScope2，得到的答案是 2。
<a name="B6FJN"></a>
### 得到的不同的值，这是线程不安全的。
在 ScopeTestController 类上加了一个注解 `@Scope("prototype")`。
```java
import org.springframework.context.annotation.Scope;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;

/**
 * @date 2019/07/29 22:56
 */
@Controller
@Scope("prototype")
public class ScopeTestController {

    private int num = 0;

    @RequestMapping("/testScope")
    public void testScope() {
        System.out.println(++num);
    }

    @RequestMapping("/testScope2")
    public void testScope2() {
        System.out.println(++num);
    }

}
```
再次进行重复的验证 http://localhost:8080/testScope，得到的答案是 1。<br />再访问 http://localhost:8080/testScope2，得到的答案还是 1。
<a name="LoZcy"></a>
### 解决方案

- 不要在controller中定义成员变量。
- 万一必须要定义一个非静态成员变量时候，则通过注解`@Scope("prototype")`，将其设置为多例模式。
- 在Controller中使用ThreadLocal变量
<a name="jUEy4"></a>
### 补充说明
Spring Bean作用域有以下5个：<br />`**singleton**`**:** 单例模式，当Spring创建applicationContext容器的时候，Spring会初始化所有的该作用域实例，加上lazy-init就可以避免预处理；<br />`**prototype**`**：** 原型模式，每次通过getBean获取该bean就会新产生一个实例，创建后Spring将不再对其管理；<br />（下面是在web项目下才用到的）<br />`**request**`**：** 搞web的大家都应该明白request的域了吧，就是每次请求都新产生一个实例，和prototype不同就是创建后，接下来的管理，Spring依然在监听；<br />`**session**`**:** 每次会话，同上；<br />**global session:** 全局的web域，类似于servlet中的application。<br />为什么加上 `@Scope("prototype")` 注解之后同样的验证方式就得出了不一样的结论呢？<br />其实真正的答案就藏在 `org.springframework.beans.factory.support.AbstractBeanFactory#doGetBean` 方法里面。<br />这就是 Spring 的源码，而且是一段比较重要的源码。<br />举个简单的例子，比如去调试这部分源码的时候，可以看到这行代码：<br />![2021-05-22-16-47-07-445604.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621673359619-3a4ad144-b6a3-4082-b661-20d9ff8a4333.png#clientId=u8ff146cc-614a-4&from=ui&id=ue05b576c&originHeight=125&originWidth=623&originalType=binary&ratio=1&rotation=0&showTitle=false&size=12042&status=done&style=none&taskId=ud726b28a-1dfb-40a0-9f1b-6c9dc9e4b8c&title=)
> Set default singleton scope, if not configured before.

再举个例子：<br />![2021-05-22-16-47-07-578090.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621673311912-43951a1c-61c2-422e-ab4d-722f4c731d39.png#clientId=u8ff146cc-614a-4&from=ui&id=u93a91717&originHeight=673&originWidth=938&originalType=binary&ratio=1&rotation=0&showTitle=false&size=55286&status=done&style=shadow&taskId=uf67e0467-c49d-42ea-8748-640ee1a1f3c&title=)<br />这里有三个分支。<br />前两个，一个是作用域为单例的情况、一个是作用域为多例（原型）的情况。<br />一个 bean 的作用域既不是单例、也不是多例，那会是什么？<br />很明显就是自定义作用域了。<br />单例和多例是 Spring 去管理实例的方式而已。<br />Spring 当然也留下了口子，可以按照自己的想法去管理自己的实例。<br />那就是自定义作用域：`org.springframework.beans.factory.config.Scope`<br />![2021-05-22-16-47-07-733191.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621673267101-c205f60a-8778-4f4c-b672-203ce5572902.png#clientId=u8ff146cc-614a-4&from=ui&id=uc7879a28&originHeight=370&originWidth=727&originalType=binary&ratio=1&rotation=0&showTitle=false&size=14105&status=done&style=shadow&taskId=ud0cd56e4-357d-4147-8018-74ca1196f8e&title=)<br />这是一个接口，这个接口就是口子。<br />而这个自定义作用域对应的 Spring 源码的入口就是 `doGetBean` 方法。
