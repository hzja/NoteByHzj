JavaSpringAOP
<a name="FSKlB"></a>
## AOP
AOP，Aspect Oriented Programming，面向切面编程。<br />将一些通用的逻辑集中实现，然后通过 AOP 进行逻辑的切入，减少了零散的碎片化代码，提高了系统的可维护性。<br />具体是含义可以理解为：通过代理的方式，在调用想要的对象方法时候，进行拦截处理，执行切入的逻辑，然后再调用真正的方法实现。<br />例如，实现了一个 A 对象，里面有  `addUser` 方法，此时需要记录该方法的调用次数。<br />那么就可以搞个代理对象，这个代理对象也提供了 `addUser` 方法，最终调用的是代理对象的 `addUser` ，在这个代理对象内部填充记录调用次数的逻辑，最终的效果就类似下面代码：
```java
class A代理 {
    A a;// 被代理的 A
   void addUser(User user) {
     count();// 计数
     a.addUser(user);
   }
}
```
最终使用的是：`A代理.addUser(user);`<br />这就叫做**面向切面编程**，当然具体的代理的代码不是像上面这样写死的，**而是动态切入**。<br />实现上代理大体上可以分为：**动态代理**和**静态代理**。

- 动态代理，即**在运行时**将切面的逻辑进去，按照上面的逻辑就是实现 A 类，然后定义要代理的切入点和切面的实现，程序会自动在运行时生成类似上面的代理类。
- 静态代理，**在编译时或者类加载时**进行切面的织入，典型的 AspectJ 就是静态代理。
<a name="QrWUI"></a>
## Spring AOP默认用的是什么动态代理，两者的区别
Spring AOP 的动态代理实现分别是：JDK 动态代理与 CGLIB。<br />默认的实现是 JDK 动态代理。<br />ok，这个问题没毛病（对实际应用来说其实不太准确），然后面试官接着问平时有调试过吗，确定得到的代理对象是 JDK 动态代理实现的？<br />然后信誓旦旦的说，对，都实现接口的，所以是 JDK 动态代理。<br />然而简历上写着项目使用的框架是 SpringBoot，问你 SpringBoot 是什么版本，你说2.x。<br />然后就可以推断，你没看过，大概率仅仅只是网上看了相关的面试题。<br />**要注意上面说的默认实现是 Spring Framework （最新版没去验证），而 SpringBoot 2.x 版本已经默认改成了 CGLIB**。<br />而现在公司大部分使用的都是 SpringBoot 2.x 版本，所以要说默认 JDK 动态代理也没错，但是不符合平日使用的情况，对吧？<br />如果调试过，或者看过调用栈，肯定能发现默认用的是 CGLIB（当然要是没用 SpringBoot 当没说）：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1657786750529-2782a8a1-00fe-4a15-b29e-7b873e5cdafc.png#clientId=ufff45386-f04d-4&from=paste&id=u55fced2c&originHeight=56&originWidth=945&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u85a0d2e8-a608-4839-b498-9350e328552&title=)<br />市面上大部分面试题答案写的就是 JDK 动态代理，是没错，Spring 官网都这样写的。<br />但是现在不都是用 SpringBoot 了，所以这其实不符合当下使用的情况。<br />因此，面试时候不要只说 Spring AOP 默认用的是 JDK 动态代理，**把 SpringBoot 也提一嘴，这不就是让面试官刮目一看**（不过指不定面试官也不知道~）<br />如果要修改 SpringBoot 使用 JDK 动态代理，那么设置 `spring.aop.proxy-target-class=false`<br />如果提了这个，那面试官肯定会追问：<br />那为什么要改成默认用 CGLIB？<br />答案准备好了，来看看：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1657786750503-ad2b2e79-25d2-48d5-9e59-9cc99d20ee08.png#clientId=ufff45386-f04d-4&from=paste&id=uf0667c1a&originHeight=130&originWidth=686&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u445687cd-3cb8-4f9a-86fa-7686ba03722&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1657786750497-97961496-61ac-4bf7-8651-82cb486d45a6.png#clientId=ufff45386-f04d-4&from=paste&id=u93a64f99&originHeight=142&originWidth=689&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u34d9ae01-ab89-415b-8622-debbfb72590&title=)<br />大佬说 JDK 动态代理要求接口，所以没有接口的话会有报错，很令人讨厌，并且让 CGLIB 作为默认也没什么副作用，特别是 CGLIB 已经被重新打包为 Spring 的一部分了，所以就默认 CGLIB 。<br />好吧，其实也没有什么很特殊的含义，就是效果没差多少，还少报错，方便。<br />详细issue 链接：[_https://github.com/spring-projects/spring-boot/issues/5423_](https://github.com/spring-projects/spring-boot/issues/5423)
<a name="vONmk"></a>
## JDK 动态代理
**JDK 动态代理是基于接口的**，也就是被代理的类一定要实现了某个接口，否则无法被代理。<br />主要实现原理就是：

1. 首先通过实现一个 `InvocationHandler` 接口得到一个切面类。
2. 然后利用 Proxy 糅合目标类的类加载器、接口和切面类得到一个代理类。
3. 代理类的逻辑就是执行切入逻辑，把所有接口方法的调用转发到 `InvocationHandler` 的 `invoke()` 方法上，然后根据反射调用目标类的方法。

![](https://cdn.nlark.com/yuque/0/2022/png/396745/1657786750487-f77392da-1e8d-4f3e-9b48-5a696cf766fa.png#clientId=ufff45386-f04d-4&from=paste&id=u2f492aff&originHeight=583&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ufbad30c1-95e1-4adf-af2f-94b98c0c192&title=)再深入一点点了解下原理实现。<br />如果反编译的话，能看到生成的代理类是会先在静态块中通过反射把所有方法都拿到存在静态变量中，（盲写了一下）大致长这样：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1657786750489-7410fd84-6089-4039-a833-cc98266d8ab0.png#clientId=ufff45386-f04d-4&from=paste&id=ua0f41712&originHeight=444&originWidth=1013&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u9fe31dac-d1d9-4584-a0e8-855ed7a3bc9&title=)上面就是把 `getUserInfo` 方法缓存了，然后在调用代理类的 `getUserInfo` 的时候，会调用之前实现的 `InvocationHandler` 里面的 `invoke`。<br />这样就执行到切入的逻辑了，且最终执行了被代理类的  `getUserInfo` 方法。<br />就是中间商拦了一道，道理就是这个道理。
<a name="yJQmR"></a>
## CGLIB
在 Spring 里面，如果被代理的类没有实现接口，那么就用 CGLIB 来完成动态代理。<br />CGLIB 是基于ASM 字节码生成工具，它是**通过继承的方式来实现代理类**，所以要注意 final 方法，这种方法无法被继承。<br />简单理解下，就是生成代理类的子类，如何生成呢？<br />**通过字节码技术动态拼接成一个子类，在其中织入切面的逻辑**。<br />使用例子：
```java
Enhancer en = new Enhancer();
//2.设置父类，也就是代理目标类，上面提到了它是通过生成子类的方式
en.setSuperclass(target.getClass());
//3.设置回调函数，这个this其实就是代理逻辑实现类，也就是切面，可以理解为JDK 动态代理的handler
en.setCallback(this);
//4.创建代理对象，也就是目标类的子类了。
return en.create();
```
JDK 动态代理和 CGLIB 两者经常还可能被面试官问性能对比，所以咱们也列一下（以下内容取自：haiq的博客）：

- jdk6 下，在运行次数较少的情况下，jdk动态代理与 cglib 差距不明显，甚至更快一些；而当调用次数增加之后， cglib 表现稍微更快一些
- jdk7 下，情况发生了逆转！在运行次数较少（1,000,000）的情况下，jdk动态代理比 cglib 快了差不多30%；而当调用次数增加之后(50,000,000)， 动态代理比 cglib 快了接近1倍
- jdk8 表现和 jdk7 基本一致
<a name="OncbM"></a>
## 能说说拦截链的实现吗？
都知道 Spring AOP 提供了多种拦截点，便捷对 AOP 的使用，比如 `@Before`、`@After`、`@AfterReturning`、`@AfterThrowing` 等等。<br />方便在目标方法执行前、后、抛错等地方进行一些逻辑的切入。<br />那 Spring 具体是如何链起这些调用顺序的呢？<br />这就是拦截链干的事，实际上这些注解都对应着不同的 `interceptor` 实现。<br />然后 Spring 会利用一个集合把所有类型的 `interceptor` 组合起来，在代码里用了 `@Before`、`@After`、`@AfterReturning`、`@AfterThrowing`这几个注解。<br />于是集合里就有了这些 interceptor（多了一个 expose...等下解释），这是由 Spring 扫描到注解自动加进来的：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1657786750882-16111cbf-5f6a-4959-af40-80ea1fdc0bfa.png#clientId=ufff45386-f04d-4&from=paste&id=uc47db4fc&originHeight=223&originWidth=943&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ubc7b8502-6d42-4e3b-99f5-4e029d58021&title=)<br />然后通过一个对象 `CglibMethodInvocation` 将这个集合封装起来，紧接着调用这个对象的 proceed 方法，可看到这个集合 chain 被传入了。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1657786750972-9cb098c2-2c89-4a0a-8870-699b6901c3c7.png#clientId=ufff45386-f04d-4&from=paste&id=u71bc05e1&originHeight=130&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u7f97f93c-4c06-4d53-98f7-bf7e7d592a4&title=)<br />来看下 `CglibMethodInvocation#proceed` 方法逻辑。<br />要注意，这里就开始**递归套娃**了，核心调用逻辑就在这里：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1657786751001-5b500d42-7964-407b-befa-938a2589a544.png#clientId=ufff45386-f04d-4&from=paste&id=u1f71f554&originHeight=661&originWidth=1040&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uf3776e06-f6eb-4804-af12-0e095fc0486&title=)<br />可以看到有个 currentInterceptorIndex 变量，**通过递归，每次新增这索引值**，来逐得到下一个 interceptor 。<br />并且每次都传入当前对象并调用  `interceptor#invoke` ，这样就实现了拦截链的调用，所以这是个递归。<br />拿集合里面的 `MethodBeforeAdviceInterceptor` 来举例看下，这个是目标方法执行的前置拦截，看下它的 `invoke` 实现，有更直观的认识：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1657786750991-ed9f4bda-d6cd-422d-ac5c-e08071d0a184.png#clientId=ufff45386-f04d-4&from=paste&id=u65180b5b&originHeight=534&originWidth=1070&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u2db80547-4828-49d1-b38a-461c23e09d0&title=)<br />invoke 的实现是先执行切入的前置逻辑，然后再继续调用 `CglibMethodInvocation#proceed`（也就是mi.proceed），进行下一个 `interceptor` 的调用。<br />**总结下**：<br />Spring 根据 `@Before`、`@After`、`@AfterReturning`、`@AfterThrowing` 这些注解。<br />往集合里面加入对应的 Spring 提供的  `MethodInterceptor` 实现。<br />比如上面的 `MethodBeforeAdviceInterceptor` ，如果没用 `@Before`，集合里就没有 `MethodBeforeAdviceInterceptor` 。<br />然后通过一个对象 `CglibMethodInvocation` 将这个集合封装起来，紧接着调用这个对象的 proceed 方法。<br />具体是利用 currentInterceptorIndex 下标，利用递归顺序地执行集合里面的 `MethodInterceptor` ，这样就完成了拦截链的调用。<br />截个调用链的堆栈截图，可以很直观地看到调用的顺序（从下往上看）：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1657786751021-ad0f2b1a-fcca-4a1a-9e1e-a8f2e918161e.png#clientId=ufff45386-f04d-4&from=paste&id=u85a4a495&originHeight=384&originWidth=751&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u0fd78de9-6b6d-4362-aee3-32533dc6ac7&title=)<br />是按照顺序一个一个往后执行，然后再一个一个返回，就是递归呗。<br />然后再解释下上面的 chain 集合看到第一个索引位置的 `ExposeInvocationInterceptor` 。<br />这个 `Interceptor` 作为第一个被调用，实际上就是将创建的 `CglibMethodInvocation` 这个对象存入 threadlocal 中，方便后面 `Interceptor` 调用的时候能得到这个对象，进行一些调用。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1657786751380-0c06f00d-6f56-49cc-8779-e1e6ecdefa43.png#clientId=ufff45386-f04d-4&from=paste&id=u5e705eba&originHeight=338&originWidth=839&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uc91cbb5f-b993-49ca-9435-f90210db2cc&title=)<br />从名字就能看出 expose：暴露。<br />ok，更多细节还是得自己看源码的，应付面试了解到这个程度差不多的，上面几个关键点一抛，这个题绝对稳了！
<a name="Ozkxh"></a>
## Spring AOP 和 AspectJ 有什么区别
从上面的题目已经知道，两者分别是动态代理和静态代理的区别。<br />Spring AOP 是动态代理，AspectJ 是静态代理。<br />从一个是运行时织入，一个在编译时织入，稍微一想到就能知道，编译时就准备完毕，那么在调用时候没有额外的织入开销，性能更好些。<br />且 AspectJ 提供完整的 AOP 解决方案，像 Spring AOP 只支持方法级别的织入，而 AspectJ 支持字段、方法、构造函数等等，所以它更加强大，当然也更加复杂。
