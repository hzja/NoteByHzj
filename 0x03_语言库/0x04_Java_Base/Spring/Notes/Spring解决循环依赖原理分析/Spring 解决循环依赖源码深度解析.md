JavaSpring循环依赖<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660881539776-71c7030a-4859-413b-9049-8f9836c1123d.png#clientId=u625fc785-c4da-4&from=paste&id=u9e7c04e9&originHeight=650&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u588d2181-8b70-4aef-848f-6900213a8a3&title=)
<a name="IyH50"></a>
## 1、基础知识
<a name="vz9ju"></a>
### 1.1 什么是循环依赖 ？
一个或多个对象之间存在直接或间接的依赖关系，这种依赖关系构成一个环形调用，有下面 3 种方式。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660881539780-d322c7c1-1b11-4904-a9a3-7ad7df3cbb00.png#clientId=u625fc785-c4da-4&from=paste&id=u704f3e22&originHeight=292&originWidth=742&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u2d573bc3-e9d9-4aa5-a926-f56ed11246e&title=)<br />看一个简单的 Demo，对标“情况 2”。
```java
@Service
public class Louzai1 {

    @Autowired
    private Louzai2 louzai2;

    public void test1() {
    }
}

@Service
public class Louzai2 {
    @Autowired
    private Louzai1 louzai1;

    public void test2() {
    }
}
```
这是一个经典的循环依赖，它能正常运行，后面会通过源码的角度，解读整体的执行流程。
<a name="pPxoL"></a>
### 1.2 三级缓存
解读源码流程之前，spring 内部的三级缓存逻辑必须了解，要不然后面看代码会蒙圈。

- **第一级缓存：**`**singletonObjects**`**，用于保存实例化、注入、初始化完成的 bean 实例；**
- **第二级缓存：**`**earlySingletonObjects**`**，用于保存实例化完成的 bean 实例；**
- **第三级缓存：**`**singletonFactories**`**，用于保存 bean 创建工厂，以便后面有机会创建代理对象。**

这是最核心，直接上源码：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660881539807-30cb66d4-7593-4e11-b431-cb5033fcd4e3.png#clientId=u625fc785-c4da-4&from=paste&id=u0ab82792&originHeight=554&originWidth=801&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u7be8ff5f-7581-43ca-8b74-5746d665272&title=)<br />执行逻辑：

- **先从“第一级缓存”找对象，有就返回，没有就找“二级缓存”；**
- **找“二级缓存”，有就返回，没有就找“三级缓存”；**
- **找“三级缓存”，找到了，就获取对象，放到“二级缓存”，从“三级缓存”移除。**
<a name="gNIym"></a>
### 1.3 原理执行流程
把“情况 2”执行的流程分解为下面 3 步，是不是和“套娃”很像 ？<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660881539763-73c1a0a2-ad88-4b8b-983e-d2e714d18e03.png#clientId=u625fc785-c4da-4&from=paste&id=u63e5a70f&originHeight=561&originWidth=1038&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uca82e2f0-4c1c-4398-83a7-b71fd13ce65&title=)<br />整个执行逻辑如下：

1. **在第一层中，先去获取 A 的 Bean，发现没有就准备去创建一个，然后将 A 的代理工厂放入“三级缓存”（这个 A 其实是一个半成品，还没有对里面的属性进行注入），但是 A 依赖 B 的创建，就必须先去创建 B；**
2. **在第二层中，准备创建 B，发现 B 又依赖 A，需要先去创建 A；**
3. **在第三层中，去创建 A，因为第一层已经创建了 A 的代理工厂，直接从“三级缓存”中拿到 A 的代理工厂，获取 A 的代理对象，放入“二级缓存”，并清除“三级缓存”；**
4. **回到第二层，现在有了 A 的代理对象，对 A 的依赖完美解决（这里的 A 仍然是个半成品），B 初始化成功；**
5. **回到第一层，现在 B 初始化成功，完成 A 对象的属性注入，然后再填充 A 的其它属性，以及 A 的其它步骤（包括 AOP），完成对 A 完整的初始化功能（这里的 A 才是完整的 Bean）。**
6. **将 A 放入“一级缓存”。**

为什么要用 3 级缓存 ？先看源码执行流程，后面会给出答案。
<a name="w79qw"></a>
## 2、源码解读
注意：Spring 的版本是 **5.2.15.RELEASE**，否则和这里的代码不一样！！！<br />上面的知识，网上其实都有，下面才是重头戏，走一遍代码流程。
<a name="leLVa"></a>
### 2.1 代码入口
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660881539831-d5e6785b-41db-472a-8f55-9398ceabb09a.png#clientId=u625fc785-c4da-4&from=paste&id=u5fcbc4ab&originHeight=874&originWidth=838&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u45587c0a-e6d1-4f7b-b904-708b54f42ed&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660881540682-26ba8992-4786-4896-beb4-fe619400083a.png#clientId=u625fc785-c4da-4&from=paste&id=u735f251e&originHeight=666&originWidth=939&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ue2109fde-166e-404b-9212-41dd918bd90&title=)<br />这里需要多跑几次，把前面的 beanName 跳过去，只看 louzai1。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660881540931-30235c66-a953-4806-ab5b-6f452ce04d3e.png#clientId=u625fc785-c4da-4&from=paste&id=ucf04a455&originHeight=938&originWidth=1019&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u77ad4962-051b-4131-ac32-dee8620c507&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660881541209-ecde386f-3268-4c65-860e-ee3c61f9ace7.png#clientId=u625fc785-c4da-4&from=paste&id=u11051b64&originHeight=102&originWidth=870&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u4b0c1eac-cc15-4ca9-83aa-99156bfd6a9&title=)
<a name="F8ZfL"></a>
### 2.2 第一层
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660881543258-60aaa605-36a9-4190-bfc9-71573582425c.png#clientId=u625fc785-c4da-4&from=paste&id=ue4542303&originHeight=252&originWidth=1012&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ub21dbf42-d771-45da-bcb1-08e4626abb8&title=)<br />进入 doGetBean()，从 getSingleton() 没有找到对象，进入创建 Bean 的逻辑。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660881545688-0a7e4c0c-f7b5-4611-9a58-440eeb9e1034.png#clientId=u625fc785-c4da-4&from=paste&id=u184340bb&originHeight=369&originWidth=959&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u0acdf980-b0e4-4369-8058-08cdd45b226&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660881546012-b11445d0-6d83-4177-b6c5-83824db05039.png#clientId=u625fc785-c4da-4&from=paste&id=u4bd65c69&originHeight=899&originWidth=829&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u10448fbf-5c49-40bf-a0c5-ba750cdabd1&title=)<br />进入 `doCreateBean()` 后，调用 `addSingletonFactory()`。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660881546509-637ff007-6c55-420a-958d-1cbde9dabb57.png#clientId=u625fc785-c4da-4&from=paste&id=u0baf27a7&originHeight=932&originWidth=861&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ub8d5024e-bad8-47f6-998e-881defa4635&title=)<br />往三级缓存 `singletonFactories` 塞入 louzai1 的工厂对象。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660881546521-c8880026-35f7-4e7e-872b-6200a6534e71.png#clientId=u625fc785-c4da-4&from=paste&id=u698ece1d&originHeight=514&originWidth=1074&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u20e69ac0-da62-438b-bf9c-eb3a3bb1e40&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660881546646-2309d844-bb1b-4142-8152-04405e4d2fe7.png#clientId=u625fc785-c4da-4&from=paste&id=u7d7fa786&originHeight=484&originWidth=877&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ue902076b-ef95-40ec-97f8-a968f0f6907&title=)<br />进入到 `populateBean()`，执行 `postProcessProperties()`，这里是一个策略模式，找到下图的策略对象。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660881546667-ec9f588d-bfba-4f69-9b9f-c4ee42aeb014.png#clientId=u625fc785-c4da-4&from=paste&id=u613f6d66&originHeight=442&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u8ff30095-d5a6-4c7c-9dc7-1627f10c952&title=)<br />正式进入该策略对应的方法。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660881547117-19ae8c57-10a6-4a32-915c-0c7e1e5a4c2f.png#clientId=u625fc785-c4da-4&from=paste&id=ud6d4a540&originHeight=495&originWidth=892&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ufc1230a8-54a4-406b-9ccf-e06a2bd6c1e&title=)<br />下面都是为了获取 louzai1 的成员对象，然后进行注入。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660881547582-a0131b05-c784-44d8-afcf-bc89f9dc1804.png#clientId=u625fc785-c4da-4&from=paste&id=u770e5bac&originHeight=236&originWidth=837&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uc89a933d-d66d-4904-b30a-b571120984b&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660881548319-bca23f85-2efa-4761-b21a-f2219dbdc8b6.png#clientId=u625fc785-c4da-4&from=paste&id=u3d385ace&originHeight=473&originWidth=783&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u52f604c5-e9f3-443a-bc2e-d8db0a93f15&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660881548690-7238c3e7-1421-4492-8e4a-8a3867545361.png#clientId=u625fc785-c4da-4&from=paste&id=ud734a458&originHeight=333&originWidth=963&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ud8436a93-06ac-41ce-a3d4-b5bda41d92a&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660881549372-ce4ecab1-ce90-4377-9ae3-8ef2d98027e8.png#clientId=u625fc785-c4da-4&from=paste&id=u7123532b&originHeight=568&originWidth=994&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u3fb7c66e-ab1a-47ff-b5a4-5881c406634&title=)<br />进入 `doResolveDependency()`，找到 louzai1 依赖的对象名 louzai2<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660881549484-9017cd47-b4b2-450f-9213-bc55e5915d56.png#clientId=u625fc785-c4da-4&from=paste&id=uce6e52c1&originHeight=258&originWidth=1051&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u543210dd-13d5-4a0c-90cb-5ce499a1485&title=)<br />需要获取 louzai2 的 bean，是 `AbstractBeanFactory` 的方法。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660881549572-1e7bca38-9fdf-40f5-8ca2-c0caf7f2ddee.png#clientId=u625fc785-c4da-4&from=paste&id=ud4d8b7f7&originHeight=296&originWidth=870&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u6ca0bcfa-26b5-47a7-b3a1-f73d1646d15&title=)<br />正式获取 louzai2 的 bean。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660881549883-095ffe64-31ed-42dc-af06-bfbf379c15ed.png#clientId=u625fc785-c4da-4&from=paste&id=u3498bc41&originHeight=115&originWidth=796&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u1cbc66ca-01d9-4cdd-b936-f1988a7c0c1&title=)<br />**到这里，第一层套娃基本结束，因为 louzai1 依赖 louzai2，下面进入第二层套娃。**
<a name="XVOE7"></a>
### 2.3 第二层
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660881550237-e98f3fd2-0bab-4359-9093-3247ffc9b173.png#clientId=u625fc785-c4da-4&from=paste&id=u76bdaf74&originHeight=99&originWidth=998&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ub6fc3d06-2490-4aef-a6a0-42849d745fc&title=)<br />获取 louzai2 的 bean，从 `doGetBean()`，到 `doResolveDependency()`，和第一层的逻辑完全一样，找到 louzai2 依赖的对象名 louzai1。<br />前面的流程全部省略，直接到 `doResolveDependency()`。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660881550414-ab220261-81ce-4cd8-b535-ff972d525d64.png#clientId=u625fc785-c4da-4&from=paste&id=ud23ea1ee&originHeight=584&originWidth=1005&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u8f018cd8-12a5-4d28-a736-b629db7d63a&title=)<br />正式获取 louzai1 的 bean。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660881550486-aa88c69c-8fba-4127-935f-a4c9881fe593.png#clientId=u625fc785-c4da-4&from=paste&id=u1c5990dc&originHeight=385&originWidth=974&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u9aaf6349-1792-464c-98ce-31f231ff9bb&title=)<br />**到这里，第二层套娃结束，因为 louzai2 依赖 louzai1，所以进入第三层套娃。**
<a name="LoGfA"></a>
### 2.4 第三层
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660881550600-8581b06d-0c1a-43c2-89c1-90234d2469ae.png#clientId=u625fc785-c4da-4&from=paste&id=u57556748&originHeight=132&originWidth=815&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ubc574057-6ca1-48a9-b44b-c894eaef804&title=)<br />获取 louzai1 的 bean，在第一层和第二层中，每次都会从 `getSingleton()` 获取对象，但是由于之前没有初始化 louzai1 和 louzai2 的三级缓存，所以获取对象为空。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660881550634-fd5317c1-9d0a-4e21-998f-001c65d53e63.png#clientId=u625fc785-c4da-4&from=paste&id=u487c4a8c&originHeight=325&originWidth=905&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ub5f2d22c-915d-46ee-8dcb-0e9936512b1&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660881550707-fceabfff-155d-48ab-a645-18c2fe29fc3d.png#clientId=u625fc785-c4da-4&from=paste&id=u3b03eb5f&originHeight=138&originWidth=915&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uce274bba-c437-4573-9695-20534b0fc2c&title=)<br />**敲重点！敲重点！！敲重点！！！**<br />到了第三层，由于第三级缓存有 louzai1 数据，这里使用三级缓存中的工厂，为 louzai1 创建一个代理对象，塞入二级缓存。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660881551103-c2549dcd-aec1-4cb2-ab8b-d02eb7c511aa.png#clientId=u625fc785-c4da-4&from=paste&id=u331fa17f&originHeight=779&originWidth=995&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u763845ac-edd9-449f-a228-f131f78a9cd&title=)<br />这里就拿到了 louzai1 的代理对象，解决了 louzai2 的依赖关系，返回到第二层。
<a name="nGEN3"></a>
### 2.5 返回第二层
返回第二层后，louzai2 初始化结束，这里就结束了么？二级缓存的数据，啥时候会给到一级呢？<br />甭着急，看这里，还记得在 `doGetBean()` 中，会通过 `createBean()` 创建一个 louzai2 的 bean，当 louzai2 的 bean 创建成功后，会执行 `getSingleton()`，它会对 louzai2 的结果进行处理。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660881551535-ab7c475d-55fb-4b2d-8cb7-3b8ce52cf44f.png#clientId=u625fc785-c4da-4&from=paste&id=ue46599f9&originHeight=300&originWidth=897&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u26093460-c171-4b0e-827e-19c281e181d&title=)<br />进入 `getSingleton()`，会看到下面这个方法。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660881551670-eb61f817-d150-431f-a379-253ff3dd7da9.png#clientId=u625fc785-c4da-4&from=paste&id=ud8310dbf&originHeight=377&originWidth=996&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ud2b31107-381c-4321-98c7-827ab6de2ea&title=)<br />这里就是处理 louzai2 的 一、二级缓存的逻辑，将二级缓存清除，放入一级缓存。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660881551890-57c319da-e22d-4e6c-9463-30ac532e4333.png#clientId=u625fc785-c4da-4&from=paste&id=ud3fc672f&originHeight=397&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ud7cfff4e-8f8b-49c6-8b9c-c07f141b404&title=)
<a name="r43SL"></a>
### 2.6 返回第一层
同 2.5，louzai1 初始化完毕后，会把 louzai1 的二级缓存清除，将对象放入一级缓存。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660881551898-90c789f8-75f6-4d6a-867c-1aa16c278178.png#clientId=u625fc785-c4da-4&from=paste&id=u2040835c&originHeight=394&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ud00b6fae-a27f-4150-87f2-284fd63bf0d&title=)<br />到这里，所有的流程结束，返回 louzai1 对象。
<a name="tdhMf"></a>
## 3、原理深度解读
<a name="qOBEZ"></a>
### 3.1 什么要有 3 级缓存 ？
这是一道非常经典的面试题，前面已经告诉大家详细的执行流程，包括源码解读，但是没有告诉大家为什么要用 3 级缓存？<br />**这里是重点！敲黑板！！！**<br />先说“一级缓存”的作用，变量命名为 `singletonObjects`，结构是 `Map<String, Object>`，它就是一个单例池，将初始化好的对象放到里面，给其它线程使用，**如果没有第一级缓存，程序不能保证 Spring 的单例属性。**<br />“二级缓存”先放放，直接看“三级缓存”的作用，变量命名为 `singletonFactories`，结构是 `Map<String, ObjectFactory<?>>`，Map 的 Value 是一个对象的代理工厂，所以“三级缓存”的作用，其实就是用来存放对象的代理工厂。<br />那这个对象的代理工厂有什么作用呢，先给出答案，**它的主要作用是存放半成品的单例 Bean，目的是为了“打破循环”**，可能大家还是不太懂，这里再稍微解释一下。<br />回到开头的例子，创建 A 对象时，会把实例化的 A 对象存入“三级缓存”，这个 A 其实是个半成品，因为没有完成 A 的依赖属性 B 的注入，所以后面当初始化 B 时，B 又要去找 A，这时就需要从“三级缓存”中拿到这个半成品的 A（这里描述，其实也不完全准确，因为不是直接拿，为了让大家好理解，就先这样描述），打破循环。<br />那再问一个问题，**为什么“三级缓存”不直接存半成品的 A，而是要存一个代理工厂呢 ？答案是因为 AOP。**<br />在解释这个问题前，看一下这个代理工厂的源码，让大家有一个更清晰的认识。<br />直接找到创建 A 对象时，把实例化的 A 对象存入“三级缓存”的代码，直接用前面的两幅截图。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660881552888-019e368b-a561-491a-8cff-5ab205eeb39f.png#clientId=u625fc785-c4da-4&from=paste&id=u167d35f0&originHeight=932&originWidth=861&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uadafabb4-c8bf-4d78-9239-365227af39f&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660881552913-8517f048-c6d0-4136-8d94-643a8f3b19df.png#clientId=u625fc785-c4da-4&from=paste&id=u2a2ed332&originHeight=514&originWidth=1074&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ucecc6684-3bca-4429-b9a1-d38c41fc3e8&title=)<br />下面主要看这个对象工厂是如何得到的，进入 `getEarlyBeanReference()` 方法。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660881553156-04fb8619-5db5-4e50-83d1-ea50da568d87.png#clientId=u625fc785-c4da-4&from=paste&id=u07a149a3&originHeight=285&originWidth=948&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u71e36cc4-927b-4869-a189-b607cbae92a&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660881554337-8cf6ac49-29f4-4bc4-a470-f3abce696665.png#clientId=u625fc785-c4da-4&from=paste&id=uf2d96b6b&originHeight=293&originWidth=1028&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u25f7972f-ce33-45f0-9cbd-0d5dd14caba&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660881555739-3ccec9d4-65e9-4929-a7fd-6f363a6f5f48.png#clientId=u625fc785-c4da-4&from=paste&id=ua369f052&originHeight=137&originWidth=941&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ua68b9168-8700-41e1-be9f-2d256d12591&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660881556287-feda9ec7-6851-4c7f-bce4-8fb6910b33f6.png#clientId=u625fc785-c4da-4&from=paste&id=ub9b5c464&originHeight=602&originWidth=926&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ue6c3cb5d-e894-4a98-a3b5-f595faaccbb&title=)<br />最后一幅图太重要了，知道这个对象工厂的作用：

- **如果 A 有 AOP，就创建一个代理对象；**
- **如果 A 没有 AOP，就返回原对象。**

**那“二级缓存”的作用就清楚了，就是用来存放对象工厂生成的对象，这个对象可能是原对象，也可能是个代理对象。**<br />再问一个问题，为什么要这样设计呢？把二级缓存干掉不行么 ？继续往下看。
<a name="EgfA0"></a>
### 3.2 能干掉第 2 级缓存么 ？
```java
@Service
public class A {

    @Autowired
    private B b;

    @Autowired
    private C c;

    public void test1() {
    }
}

@Service
public class B {
    @Autowired
    private A a;

    public void test2() {
    }
}

@Service
public class C {

    @Autowired
    private A a;

    public void test3() {
    }
}
```
根据上面的套娃逻辑，A 需要找 B 和 C，但是 B 需要找 A，C 也需要找 A。<br />**假如 A 需要进行 AOP**，因为代理对象每次都是生成不同的对象，如果干掉第二级缓存，只有第一、三级缓存：

- **B 找到 A 时，直接通过三级缓存的工厂的代理对象，生成对象 A1。**
- **C 找到 A 时，直接通过三级缓存的工厂的代理对象，生成对象 A2。**

看到问题没？**通过 A 的工厂的代理对象，生成了两个不同的对象 A1 和 A2**，所以为了避免这种问题的出现，搞个二级缓存，把 A1 存下来，下次再获取时，直接从二级缓存获取，无需再生成新的代理对象。<br />**所以“二级缓存”的目的是为了避免因为 AOP 创建多个对象，其中存储的是半成品的 AOP 的单例 bean。**<br />如果没有 AOP 的话，其实只要 1、3 级缓存，就可以满足要求。
<a name="B4wAK"></a>
## 4、写在最后
再回顾一下 3 级缓存的作用：

- **一级缓存：为“Spring 的单例属性”而生，就是个单例池，用来存放已经初始化完成的单例 Bean；**
- **二级缓存：为“解决 AOP”而生，存放的是半成品的 AOP 的单例 Bean；**
- **三级缓存：为“打破循环”而生，存放的是生成半成品单例 Bean 的工厂方法。**

这里也简单说一下看源码的心得：

1. **需要掌握基本的设计模式；**
2. **看源码前，最好能找一些理论知识先看看；**
3. **学会读英文注释，不会的话就百度翻译；**
4. **debug 时，要克制自己，不要陷入无用的细节，这个最重要。**

其中最难的是第 4 步，因为很多同学看 Spring 源码，每看一个方法，就想多研究研究，这样很容易被绕进去了，这个**要学会克制，有大局观，并能分辨哪里是核心逻辑**，至于如何分辨，可以在网上先找些资料，如果没有的话，就只能多看代码了。
