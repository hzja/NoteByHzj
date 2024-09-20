JavaSpring<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1663809092890-c85ca6e0-da98-4990-a0ab-bc805090c7e2.png#clientId=ua7c4dc0e-caed-4&from=paste&id=uabcf2654&originHeight=818&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u68331eea-ce2f-4789-a39a-70b1ae42d0e&title=)
<a name="TAlac"></a>
## 1、基础知识
<a name="TBTYy"></a>
### 1.1 什么是 IoC ？
IoC，控制反转，想必大家都知道，所谓的控制反转，就是把 new 对象的权利交给容器，所有的对象都被容器控制，这就叫所谓的控制反转。<br />IoC 很好地体现了面向对象设计法则之一 —— 好莱坞法则：“别找我们，我们找你”，即由 IoC 容器帮对象找相应的依赖对象并注入，而不是由对象主动去找。<br />理解好 IoC 的关键是要明确 “谁控制谁，控制什么，为何是反转（有反转就应该有正转了），哪些方面反转了”。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1663809092872-e399e9bc-d376-4eb4-8994-b5f4c407263b.png#clientId=ua7c4dc0e-caed-4&from=paste&id=uce3a2ff8&originHeight=498&originWidth=777&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u20a9151b-0d54-4138-8009-4050a490446&title=)<br />谁控制谁，控制什么？<br />传统 Java SE 程序设计，直接在对象内部通过 new 进行创建对象，是程序主动去创建依赖对象。而 IoC 是由专门一个容器来创建这些对象，即由 IoC 容器来控制对象的创建。

- 谁控制谁？当然是 IoC 容器控制了对象；
- 控制什么？主要控制了外部资源获取（不只是对象，比如包括文件等）。

为何是反转，哪些方面反转了？<br />有反转就有正转，传统应用程序是由自己在对象中主动控制去直接获取依赖对象，也就是正转，而反转则是由容器来帮忙创建及注入依赖对象。

- 为何是反转？因为由容器查找及注入依赖对象，对象只是被动的接受依赖对象，所以是反转；
- 哪些方面反转了？依赖对象的获取被反转了。
<a name="wUtcN"></a>
### 1.2 Bean 生命周期
对 Prototype Bean 来说，当用户 getBean 获得 Prototype Bean 的实例后，IOC 容器就不再对当前实例进行管理，而是把管理权交由用户，此后再 getBean 生成的是新的实例。<br />所以描述 Bean 的生命周期，都是指的 Singleton Bean。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1663809092874-63e96c2d-38ba-497c-8d37-8668b5829326.png#clientId=ua7c4dc0e-caed-4&from=paste&id=u3d1f3d45&originHeight=602&originWidth=896&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uf0d4cec0-b55d-4200-a56c-b4ad140ef43&title=)<br />Bean 生命周期过程：

- 实例化：第 1 步，实例化一个 Bean 对象；
- 属性赋值：第 2 步，为 Bean 设置相关属性和依赖；
- 初始化：初始化的阶段的步骤比较多，5、6 步是真正的初始化，第 3、4 步为在初始化前执行，第 7 步在初始化后执行，初始化完成之后，Bean 就可以被使用了；
- 销毁：第 8~10 步，第 8 步其实也可以算到销毁阶段，但不是真正意义上的销毁，而是先在使用前注册了销毁的相关调用接口，为了后面第 9、10 步真正销毁 Bean 时再执行相应的方法。

整个执行流程稍微有些抽象，下面通过代码，来演示执行流程。
<a name="iXtf8"></a>
### 1.3 执行流程
创建一个 LouzaiBean。
```java
public class LouzaiBean implements InitializingBean, BeanFactoryAware, BeanNameAware, DisposableBean {

    /**
     * 姓名
     */
    private String name;

    public LouzaiBean() {
        System.out.println("1.调用构造方法：我出生了！");
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
        System.out.println("2.设置属性：我的名字叫"+name);
    }

    @Override
    public void setBeanName(String s) {
        System.out.println("3.调用BeanNameAware#setBeanName方法:我要上学了，起了个学名");
    }

    @Override
    public void setBeanFactory(BeanFactory beanFactory) throws BeansException {
        System.out.println("4.调用BeanFactoryAware#setBeanFactory方法：选好学校了");
    }

    @Override
    public void afterPropertiesSet() throws Exception {
        System.out.println("6.InitializingBean#afterPropertiesSet方法：入学登记");
    }

    public void init() {
        System.out.println("7.自定义init方法：努力上学ing");
    }

    @Override
    public void destroy() throws Exception {
        System.out.println("9.DisposableBean#destroy方法：平淡的一生落幕了");
    }

    public void destroyMethod() {
        System.out.println("10.自定义destroy方法:睡了，别想叫醒我");
    }

    public void work(){
        System.out.println("Bean使用中：工作，只有对社会没有用的人才放假。。");
    }
}
```
自定义一个后处理器 MyBeanPostProcessor。
```java
public class MyBeanPostProcessor implements BeanPostProcessor {

    @Override
    public Object postProcessBeforeInitialization(Object bean, String beanName) throws BeansException {
        System.out.println("5.BeanPostProcessor.postProcessBeforeInitialization方法：到学校报名啦");
        return bean;
    }

    @Override
    public Object postProcessAfterInitialization(Object bean, String beanName) throws BeansException {
        System.out.println("8.BeanPostProcessor#postProcessAfterInitialization方法：终于毕业，拿到毕业证啦！");
        return bean;
    }
}
```
applicationContext.xml 配置文件（部分）。
```xml
<bean name="myBeanPostProcessor" class="demo.MyBeanPostProcessor" />
<bean name="louzaiBean" class="demo.LouzaiBean"
  init-method="init" destroy-method="destroyMethod">
  <property name="name" value="楼仔" />
</bean>
```
测试入口：
```java
public class MyTest {
    public static void main(String[] args) {
        ApplicationContext context =new ClassPathXmlApplicationContext("classpath:applicationContext.xml");
        LouzaiBean louzaiBean = (LouzaiBean) context.getBean("louzaiBean");
        louzaiBean.work();
        ((ClassPathXmlApplicationContext) context).destroy();
    }
}
```
执行结果：
```
1.调用构造方法：我出生了！
2.设置属性：我的名字叫楼仔
3.调用BeanNameAware#setBeanName方法:我要上学了，起了个学名
4.调用BeanFactoryAware#setBeanFactory方法：选好学校了
5.BeanPostProcessor.postProcessBeforeInitialization方法：到学校报名啦
6.InitializingBean#afterPropertiesSet方法：入学登记
7.自定义init方法：努力上学ing
8.BeanPostProcessor#postProcessAfterInitialization方法：终于毕业，拿到毕业证啦！
Bean使用中：工作，只有对社会没有用的人才放假。。
9.DisposableBean#destroy方法：平淡的一生落幕了
10.自定义destroy方法:睡了，别想叫醒我
```
这个流程非常清晰，Bean 生命周期流程图能完全对应起来。
<a name="VIS9B"></a>
### 1.4 扩展方法
**可以**发现，整个生命周期有很多扩展过程，大致可以分为 4 类：

- Aware 接口：让 Bean 能拿到容器的一些资源，例如 `BeanNameAware` 的 `setBeanName()`，`BeanFactoryAware` 的 `setBeanFactory()`；
- 后处理器：进行一些前置和后置的处理，例如 `BeanPostProcessor` 的 `postProcessBeforeInitialization()` 和 `postProcessAfterInitialization()`；
- 生命周期接口：定义初始化方法和销毁方法的，例如 `InitializingBean` 的 `afterPropertiesSet()`，以及 `DisposableBean` 的 `destroy()`；
- 配置生命周期方法：可以通过配置文件，自定义初始化和销毁方法，例如配置文件配置的 `init()` 和 `destroyMethod()`。
<a name="IIpxC"></a>
## 2、源码解读
注意：Spring 的版本是 5.2.15.RELEASE，否则和这里的代码不一样！！！<br />上面的知识，网上其实都有，下面才是重头戏，跟着走一遍代码流程。
<a name="l4WkE"></a>
### 2.1 代码入口
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1663809092848-fba2f97e-c881-4c90-8ad8-9a0e49b9c4a2.png#clientId=ua7c4dc0e-caed-4&from=paste&id=u8ceae486&originHeight=874&originWidth=838&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u1115a3f7-d304-4ade-83a8-b796df517ea&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1663809092891-48763b67-ecd9-4f7a-bbeb-1c3390072052.png#clientId=ua7c4dc0e-caed-4&from=paste&id=u1eb3df53&originHeight=666&originWidth=939&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uf5a59a36-a262-40c7-8ad9-fc7e78b1d82&title=)<br />这里需要多跑几次，把前面的 beanName 跳过去，只看 louzaiBean。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1663809093685-f3f57d40-a1dc-4b0b-bd0b-f799f98f2371.png#clientId=ua7c4dc0e-caed-4&from=paste&id=uac0adead&originHeight=932&originWidth=909&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u23893b67-3b64-46d0-9027-05be8b4c283&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1663809093782-9ce52dff-4c45-40c2-8a25-965edad85936.png#clientId=ua7c4dc0e-caed-4&from=paste&id=u8dad6466&originHeight=102&originWidth=870&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uc2ed7b27-786a-42b0-a344-46055fb3007&title=)<br />进入 `doGetBean()`，从 `getSingleton()` 没有找到对象，进入创建 Bean 的逻辑。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1663809093859-6846c176-615c-49b4-837d-3c8ab9cb2105.png#clientId=ua7c4dc0e-caed-4&from=paste&id=ud3c78e78&originHeight=362&originWidth=931&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u627db7d8-ad42-46d7-8269-5a5ca8bfaba&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1663809093897-37d25699-c82c-4508-8443-b58fd6e53588.png#clientId=ua7c4dc0e-caed-4&from=paste&id=u5cda0047&originHeight=899&originWidth=829&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u584b1bc1-ee23-4e0f-9c45-3bfd07efb53&title=)
<a name="gB3gG"></a>
### 2.2 实例化
进入 `doCreateBean()` 后，调用 `createBeanInstance()`。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1663809094206-f15d7e38-c5a3-443a-903b-8d92e2be539e.png#clientId=ua7c4dc0e-caed-4&from=paste&id=u8abfa069&originHeight=290&originWidth=848&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ude7f819e-cfa1-4c17-bee0-0ea764d2c85&title=)<br />进入 `createBeanInstance()` 后，调用 `instantiateBean()`。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1663809094266-288f6b19-e22e-4904-80ff-3337bf281eec.png#clientId=ua7c4dc0e-caed-4&from=paste&id=ucbf42af7&originHeight=390&originWidth=933&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ue23ae27a-240f-4198-ba26-c20b1c54f23&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1663809094342-4d04cc6f-fd8d-4fdc-a5ec-6409c4d6ea28.png#clientId=ua7c4dc0e-caed-4&from=paste&id=u76f55276&originHeight=326&originWidth=927&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ud242d0f8-e3f6-4695-bb08-54ffe464a3d&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1663809094412-ff12a1fc-a486-49dc-beb0-26d81b856dde.png#clientId=ua7c4dc0e-caed-4&from=paste&id=ucf41c68d&originHeight=633&originWidth=877&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u0bbb9228-7936-4cd0-a667-5ea64e7e46e&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1663809094609-74613df9-be50-4169-819d-8193da4d5cb7.png#clientId=ua7c4dc0e-caed-4&from=paste&id=uf2440872&originHeight=509&originWidth=1031&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uff724069-2393-4345-a7c7-ef417b578f7&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1663809094723-6b53d6c8-a2aa-4a18-8bed-b6469e9986b8.png#clientId=ua7c4dc0e-caed-4&from=paste&id=uddd3db90&originHeight=436&originWidth=810&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u9fec0441-5063-4353-88cf-a74daa18b4d&title=)<br />走进示例 LouzaiBean 的方法，实例化 LouzaiBean。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1663809094801-751408cd-fe6e-449d-9f15-1c5aeed90274.png#clientId=ua7c4dc0e-caed-4&from=paste&id=u939ce7d9&originHeight=244&originWidth=729&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u8dda75e6-d212-47b0-9569-4d2ff427919&title=)
<a name="hqPhD"></a>
### 2.3 属性赋值
再回到 `doCreateBean()`，继续往后走，进入 `populateBean()`。<br />这个方法非常重要，里面其实就是依赖注入的逻辑，不过这个不是这里的重点。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1663809094819-28a6e4ef-ea70-48e1-8431-91c25bb45db1.png#clientId=ua7c4dc0e-caed-4&from=paste&id=u59178795&originHeight=238&originWidth=906&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u02a865a8-5ac7-4a51-b620-3278db8aea9&title=)<br />进入 `populateBean()` 后，执行 `applyPropertyValues()`<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1663809094917-f619aa05-ad34-488d-b069-8f3c4c0dc666.png#clientId=ua7c4dc0e-caed-4&from=paste&id=ucba731d2&originHeight=246&originWidth=931&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u219fa672-3927-4276-a249-142fa245f99&title=)<br />进入 `applyPropertyValues()`，执行 `bw.setPropertyValues()`<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1663809095015-fc8bef59-9c68-4503-934f-347917069a13.png#clientId=ua7c4dc0e-caed-4&from=paste&id=ue79d9870&originHeight=217&originWidth=871&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=udcd49de8-1723-4342-88f5-ff4b6efafe8&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1663809095200-a9df5fbf-2784-4dfa-8037-dc93c4a6b1ec.png#clientId=ua7c4dc0e-caed-4&from=paste&id=u4e71d9eb&originHeight=94&originWidth=943&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u386b4253-41f8-4507-a706-d1e005dc934&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1663809095278-558a3ad7-ab9a-47be-8951-5e9e97809275.png#clientId=ua7c4dc0e-caed-4&from=paste&id=ue54393c1&originHeight=448&originWidth=930&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uc7723518-1b7c-45dc-80d9-0d6f21e0837&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1663809095294-7289675b-3c94-4c41-904d-6d89eb1e09d7.png#clientId=ua7c4dc0e-caed-4&from=paste&id=ueee50f59&originHeight=193&originWidth=897&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ub0a5e3d3-c65e-4727-bfcd-7a96c6833e7&title=)<br />进入 `processLocalProperty()`，执行 `ph.setValue()`。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1663809095316-012cecdd-3c2e-40d7-a836-8aa8ad91e352.png#clientId=ua7c4dc0e-caed-4&from=paste&id=u3077ef1a&originHeight=213&originWidth=878&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u39005b0a-4c05-49e6-9aa6-b7b51793fb2&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1663809095515-84659df5-22e3-479b-ac1b-6509b44f4f85.png#clientId=ua7c4dc0e-caed-4&from=paste&id=udccfd8a2&originHeight=490&originWidth=868&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u7849ea5f-3482-4e38-afe2-e782b9c0c3e&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1663809095743-2a900350-70e2-41e1-a32b-0043d9d07efc.png#clientId=ua7c4dc0e-caed-4&from=paste&id=u4d889da4&originHeight=603&originWidth=901&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ubd8e48f6-f0e1-4208-a788-9ec9e1e0e5e&title=)<br />走进示例 LouzaiBean 的方法，给 LouzaiBean 赋值 name。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1663809095679-f068a866-9bdc-47e4-a225-cf0c39f1b6fc.png#clientId=ua7c4dc0e-caed-4&from=paste&id=u275f0bdd&originHeight=106&originWidth=754&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ue1105952-6ccb-4fd7-acca-f459b86c52f&title=)<br />到这里，`populateBean()` 就执行完毕，下面开始初始化 Bean。
<a name="lxi2h"></a>
### 2.4 初始化
继续回到 `doCreateBean()`，往后执行 `initializeBean()`。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1663809095761-9e022621-4bf4-4e92-b112-4c52b974faef.png#clientId=ua7c4dc0e-caed-4&from=paste&id=u046763cb&originHeight=223&originWidth=1000&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u593df376-67b9-4cf2-8d5f-ad5593e6167&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1663809095806-3baa7fb2-5d06-41e9-ad1c-f0fd6d0c40dd.png#clientId=ua7c4dc0e-caed-4&from=paste&id=ue85c221b&originHeight=245&originWidth=835&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u9e2eaf77-498e-4d68-b033-ade12e17c69&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1663809096017-e0a2e713-5b09-4b0f-b1fc-390460a651c0.png#clientId=ua7c4dc0e-caed-4&from=paste&id=u9d97b55f&originHeight=369&originWidth=1004&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ua311d6fe-b6cf-42d7-a10d-310b3a631aa&title=)<br />走进示例 LouzaiBean 的方法，给 LouzaiBean 设置 BeanName。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1663809096130-baf54f25-f1ec-4759-b3fa-f763714fc0b3.png#clientId=ua7c4dc0e-caed-4&from=paste&id=u07d875a7&originHeight=106&originWidth=789&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u54d1a86b-b983-41f9-8aa7-4880ee3298f&title=)<br />回到 `invokeAwareMethods()`。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1663809096274-c39be924-6cd0-4fe2-bf1b-a36746c7030e.png#clientId=ua7c4dc0e-caed-4&from=paste&id=ud10692a4&originHeight=361&originWidth=1059&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ud0c486af-694a-4be5-8214-6502f1e58b8&title=)<br />走进示例 LouzaiBean 的方法，给 LouzaiBean 设置 BeanFactory。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1663809096388-ca0681f0-d72c-4100-9b32-158cdb2750f4.png#clientId=ua7c4dc0e-caed-4&from=paste&id=ua0cf1724&originHeight=103&originWidth=764&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u377bc0ee-98a9-48b8-8e9a-4a49394cbe5&title=)<br />第一次回到 `initializeBean()`，执行下面逻辑。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1663809096332-e973497b-096b-4c44-9b16-7881a0c09c58.png#clientId=ua7c4dc0e-caed-4&from=paste&id=u43a58afc&originHeight=378&originWidth=950&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u1c57a5ed-523a-49ef-ab5a-bb99a09323d&title=)<br />这里需要多循环几次，找到 `MyBeanPostProcessor` 的策略方法。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1663809096497-79443450-a9d6-4b13-854b-d2ab940a85d4.png#clientId=ua7c4dc0e-caed-4&from=paste&id=u1e480b97&originHeight=515&originWidth=1070&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ub24f135e-1c6f-4fd0-a3ca-9c2026e8a5f&title=)<br />自己定义的后置处理方法。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1663809096560-033b0219-4c73-4503-9c89-62651e0f0a61.png#clientId=ua7c4dc0e-caed-4&from=paste&id=ua0d26738&originHeight=322&originWidth=939&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u56a24e02-9f12-4e0a-8af8-c1e4123cfcd&title=)<br />第二次回到 `initializeBean()`，执行下面逻辑。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1663809096661-b31f42e3-24f1-4fd6-a36c-539f4a426351.png#clientId=ua7c4dc0e-caed-4&from=paste&id=uc04ca9cf&originHeight=604&originWidth=939&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ua42e8714-8b0a-499c-993c-7f509ed49c8&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1663809096704-b1915f5f-7c5f-4c8d-bf56-67589f8a940c.png#clientId=ua7c4dc0e-caed-4&from=paste&id=uacc50771&originHeight=511&originWidth=922&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u0f0c0b29-6725-489c-b6c5-139dec28b84&title=)<br />走进示例 LouzaiBean 的方法，执行 `afterPropertiesSet()`。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1663809096755-d3ad4496-e9f9-470d-b91e-dd090678e9f3.png#clientId=ua7c4dc0e-caed-4&from=paste&id=u9d8ab72d&originHeight=103&originWidth=733&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u9229d455-2382-4682-bb82-f1a90e3c2a3&title=)<br />返回 `invokeInitMethods()`，执行下面逻辑。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1663809096917-eb08635f-1337-4726-b600-c3acbb66a3b3.png#clientId=ua7c4dc0e-caed-4&from=paste&id=u5008e1ae&originHeight=743&originWidth=942&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u968c446e-6d84-48cb-a4e4-ea0d53130ea&title=)<br />进入 `invokeCustomInitMethod()`，执行下面逻辑。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1663809097138-1204f007-4a24-4b23-8223-c6ddbabd2e59.png#clientId=ua7c4dc0e-caed-4&from=paste&id=ucb2f2338&originHeight=465&originWidth=977&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ud03d8822-5c54-4522-a040-25a69b8fc60&title=)<br />走进示例 LouzaiBean 的方法，执行 `init()`。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1663809097122-7b8fd5a0-5958-43ad-b248-4f46c8005ea2.png#clientId=ua7c4dc0e-caed-4&from=paste&id=u05b921f5&originHeight=84&originWidth=753&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u24d9696a-f2b0-4ef9-9a2a-afa2557e268&title=)<br />第三次回到 `initializeBean()`，执行下面逻辑。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1663809097168-a2b43dc7-edcd-40f6-b6c0-dc0c494c4ca2.png#clientId=ua7c4dc0e-caed-4&from=paste&id=ub0038867&originHeight=672&originWidth=1030&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u29ff6d9d-fb92-47c7-9f3c-558476f8da5&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1663809097259-a91ce9c3-f074-44eb-8c74-144c01fe7201.png#clientId=ua7c4dc0e-caed-4&from=paste&id=u211c8f73&originHeight=564&originWidth=979&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u7ea03371-adb7-4e56-a2c9-88045489c9d&title=)<br />自己定义的后置处理方法。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1663809097488-98b8bad1-213a-4dd5-8bf1-c170e5368c5f.png#clientId=ua7c4dc0e-caed-4&from=paste&id=uc0fbd7f9&originHeight=312&originWidth=1024&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u11d96396-c8d7-4a4f-8e52-fb3d44ec1b1&title=)<br />到这里，初始化的流程全部结束，都是围绕 `initializeBean()` 展开。
<a name="sqIC5"></a>
### 2.5 销毁
当 louzaiBean 生成后，后面开始执行销毁操作，整个流程就比较简单。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1663809097506-2a74ac22-5ce2-47d4-9305-f633fff7e984.png#clientId=ua7c4dc0e-caed-4&from=paste&id=u99baf656&originHeight=100&originWidth=724&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u8f14508d-ea0b-4ff0-9bb2-5cb11ecf15e&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1663809097699-ac7eb401-cc45-411e-a734-afe268f8a92f.png#clientId=ua7c4dc0e-caed-4&from=paste&id=ue3c03394&originHeight=369&originWidth=835&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u21def2d2-da95-4d72-9e9f-3cdcd70f87a&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1663809097839-6f6fa59a-5009-4bd5-ab8b-b85722a2278c.png#clientId=ua7c4dc0e-caed-4&from=paste&id=u13e9083e&originHeight=674&originWidth=1019&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u7d7f4697-b167-4482-b5f4-55be0ac0be5&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1663809098052-ebaf167d-e99e-4664-b1f2-df4346f849c5.png#clientId=ua7c4dc0e-caed-4&from=paste&id=u31a4a4f4&originHeight=86&originWidth=843&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u613bc721-d477-42df-91ab-0868b5a1c7a&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1663809098016-42866620-0db0-43e7-9043-671bf592e4e7.png#clientId=ua7c4dc0e-caed-4&from=paste&id=uece8a9f3&originHeight=143&originWidth=915&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u2578d716-e7c5-444c-a893-d3b625dc4b8&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1663809098117-093087ec-8f6c-47bc-b23d-4a75d28cc22a.png#clientId=ua7c4dc0e-caed-4&from=paste&id=u2a7eccc6&originHeight=502&originWidth=1003&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uca428c7b-99c7-4f41-9a4d-ab4e2294fdf&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1663809098212-349bb7fc-fcfb-4c57-9e56-522981bd6e1f.png#clientId=ua7c4dc0e-caed-4&from=paste&id=u9421a11d&originHeight=137&originWidth=837&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u071bcec3-b611-40cb-a4ee-d32142a1d07&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1663809098545-2cb3f62a-7824-4a1d-bc36-53cb21bf5ad9.png#clientId=ua7c4dc0e-caed-4&from=paste&id=uc7b3ea23&originHeight=257&originWidth=991&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u1c18e7af-85d0-43a1-a746-38c8255fbe1&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1663809098599-6bcfc037-a79a-4720-8ee0-0efdfbcf5bc1.png#clientId=ua7c4dc0e-caed-4&from=paste&id=ub9778254&originHeight=599&originWidth=958&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u351ed447-eed9-4789-817b-ddcc9f2eb1f&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1663809098520-bd451a1e-bdf4-456e-a10a-c9e21087c887.png#clientId=ua7c4dc0e-caed-4&from=paste&id=udad9caef&originHeight=573&originWidth=966&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u72376ec1-9b11-4465-a6c2-19c26adf7a3&title=)<br />走进示例 LouzaiBean 的方法，执行 `destroy()`。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1663809098589-48c22021-058d-4162-b5c2-3911a962c5c1.png#clientId=ua7c4dc0e-caed-4&from=paste&id=u4561eaaf&originHeight=109&originWidth=913&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ufb272672-5fe7-485e-a0df-94a751a408d&title=)<br />回到 `destroy()`，执行下面逻辑。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1663809098867-b7859ad6-c335-4984-89e4-88442b6c414f.png#clientId=ua7c4dc0e-caed-4&from=paste&id=ub7af5c3d&originHeight=601&originWidth=969&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u33976434-1c4b-4e27-aca1-5571ff05fc8&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1663809098881-105d3beb-5e1e-40a1-8a66-67c0688bfb3a.png#clientId=ua7c4dc0e-caed-4&from=paste&id=u1ab807d3&originHeight=620&originWidth=910&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uaa5149f7-d327-4c2e-a281-72af5704878&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1663809098992-b81bd4b7-d2e2-4f71-a561-58461d44d881.png#clientId=ua7c4dc0e-caed-4&from=paste&id=ucca81126&originHeight=682&originWidth=930&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u8bb064fa-24ab-4686-b7f6-5af347d3a90&title=)<br />走进示例 LouzaiBean 的方法，执行 `destroyMethod()`。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1663809099081-0b41bab2-00e1-434e-99e2-a99ea0e47093.png#clientId=ua7c4dc0e-caed-4&from=paste&id=u342359af&originHeight=95&originWidth=973&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ua7ed45f6-0676-4a80-95ce-3ede79dbbcb&title=)<br />到这里，所有的流程全部结束，文章详细描述所有的代码逻辑流转，可以完全根据上面的逻辑，自己 debug 一遍。
<a name="Aw6rJ"></a>
## 3、写在最后
再回顾一下几个重要的方法：

- `doCreateBean()`：这个是入口；
- `createBeanInstance()`：用来初始化 Bean，里面会调用对象的构造方法；
- `populateBean()`：属性对象的依赖注入，以及成员变量初始化；
- `initializeBean()`：里面有 4 个方法，
   - 先执行 aware 的 `BeanNameAware`、`BeanFactoryAware` 接口；
   - 再执行 `BeanPostProcessor` 前置接口；
   - 然后执行 `InitializingBean` 接口，以及配置的 `init()`；
   - 最后执行 `BeanPostProcessor` 的后置接口。
- `destory()`：先执行 `DisposableBean` 接口，再执行配置的 `destroyMethod()`。

对于 `populateBean()`，里面的核心其实是对象的依赖注入，这里也是常考的知识点，比如循环依赖。
