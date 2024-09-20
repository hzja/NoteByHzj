Java ClassLoader 类加载器<br />`ClassLoader`的中文名翻译过来叫做类加载器，主要的工作就是负责把Class加载到JVM中，看起来挺高大撒上，其实就是个看大门的，只能放VIP进去，不同级别的VIP会安排到不同级别的地方，别让什么闲杂人等进JVM里面的捣乱；<br />虽然ClassLoader的职业比较“低下”，但是并没有阻挡人家发光发热的心，不仅可以辨认谁该进谁不该，还可以审查每个类应该由谁来加载，而且是一种父优先的等级加载机制；<br />还没完，人家觉悟高着呢，谁说看大门的就不能出成绩？还有第三个功能：把Class字节码重新解析成JVM统一要求的对象格式；
<a name="YMlA2"></a>
## `ClassLoader`都有哪些函数？
 ![2021-06-25-21-36-24-218182.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1624628317821-ec7e1fb7-faae-4f7e-8343-029b02dda553.png#clientId=u7ee0ecb2-6a28-4&from=ui&id=u1b5efcfd&originHeight=300&originWidth=722&originalType=binary&ratio=3&size=68528&status=done&style=shadow&taskId=udff41488-e257-45fa-bc67-9743842fc34)
<a name="mzmt0"></a>
## `ClassLoader`的等级加载机制
前面说过，不同等级的VIP是必须要进入到不同的VIP接待室的，并且一个VIP去一个VIP接待室而且也只能去一次，也就是说VIP和VIP接待室是一对多的关系，还得为不知去哪里的VIP指路，同时还要防止有的VIP浑水摸鱼去更高级别的VIP接待室，如此重要的任务就落在了`ClassLoader`的头上，谁让咱颜值高呢，啊，不是，能力大呢！<br />`ClassLoade`是如何解决的呢？第一步：如果一个VIP到达了一个VIP接待室，那么VIP接待室会首先检查这个VIP是否已经被自己接待过，如果是，当然是拒绝，如果不是的话也不能急着放行，人家VIP并不知道自己该去哪一个VIP接待室，去级别低了掉价，去级别高了VIP接待室掉价；<br />第二步：向上级询问这个VIP是不是应该去级别更高的VIP接待室，那么上级也会先检查是否接待过，是的话直接拒绝并把结果反馈下去，如果不是就继续向上级的上级询问；<br />接着上级的上级继续重复这些检查，直到一个级别的VIP接待室说可以接待，并且也不能比他（他指的是说可以接待的）低一级的接待以及比他上一级的接待室说这个VIP级别不够，那么，这个VIP就会进入到刚才确定的VIP接待室，同时也会标记上相应的等级；<br />麻烦是麻烦了点，不过可以保证安全，也就是既不会让VIP掉价，也不会让VIP接待室掉价；
<a name="zAvKk"></a>
## 三种类加载器
JVM平台提供了三层`ClassLoader`，这三层`ClassLoader`又分为两种类型：<br />第一种：Booststrap `ClassLoader`(为接待室服务的接待室)，这个类别人是访问不到的，必须是JVM来控制，因为它主要加载JVM自身工作需要的类，这个类是不用遵循刚才说的繁琐的规则的，仅仅就是类加载工具而已，没有父加载器也没有子加载器<br />第二种：`AppClassLoader`(为VIP服务的接待室),他的父类是`ExtClassLoader`，可以加载`System.getProperty("java.class.path")`目录下的所有的类，如果要实现自己的类加载器的话，父类必须是`AppClassLoader`；<br />再说一下`ExtClassLoader`，他有点特殊，有点类似于与VIP接待室合作的VIP,主要加载`System.getPorperty("java.ext.dirs")`目录下的类；
<a name="GOPhN"></a>
## JVM都是如何加载Class文件的？
JVM其实就是个中转站，最终代码还是交给硬件来处理，JVM把Class文件记载到内存中主要有两种方式：<br />第一种，隐式加载：不通过调用`ClassLoader`来加载需要的类，而是通过JVM来自动加载；<br />第二种，显式加载：也就是通过调用`ClassLoader`来加载；<br />这两种方式可以混合着使用；<br />那么，有一个问题，Class是怎么被加载到JVM中的？一图胜千言！<br />![2021-06-25-21-36-24-392718.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1624628330538-d8717982-2739-4025-bfd5-8bc82ea44883.png#clientId=u7ee0ecb2-6a28-4&from=ui&id=ufd06259f&originHeight=481&originWidth=757&originalType=binary&ratio=3&size=64858&status=done&style=shadow&taskId=u874b154b-4fb2-4071-8b66-ebdaa93d923)<br />加载完了并不意味着工作结束了，还有验证和解析：<br />1. 字节码验证：类装入器对类的字节码的验证是为了保证格式正确；<br />2. 类准备：主要是准备好类所需要的字段、函数以及实现接口所必须的数据结构；<br />3. 解析：类装入器需要装入类所引用得其他类；<br />这些工作完成之后就可以初始化Class对象，也就是在类中包含的所有的静态初始化器都会被执行；
<a name="mL8kJ"></a>
## 如何自己实现一个`ClassLoader`呢？
已经知道`ClassLoader`可以做什么了；

1.  在自定义路径下查找自定义的Class类文件；
2.  对于要加载的类走特殊处理，比如：加密、解密；

就这三种情况自己实现一个`ClassLoader`；
<a name="yPd2j"></a>
### 1. 在自定义路径下查找自定义的Class类文件；
![2021-06-25-21-36-24-579268.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1624628342108-c7f067df-fef1-48b3-abc9-768bc434b7b7.png#clientId=u7ee0ecb2-6a28-4&from=ui&id=uf2b429bd&originHeight=593&originWidth=503&originalType=binary&ratio=3&size=37467&status=done&style=none&taskId=u2e19b504-b42b-4542-997f-66d10b314f5)![2021-06-25-21-36-24-779681.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1624628342132-4537b01e-11d4-405c-8bdb-7ccb6b0e8788.png#clientId=u7ee0ecb2-6a28-4&from=ui&id=u8053c3cc&originHeight=514&originWidth=498&originalType=binary&ratio=3&size=28178&status=done&style=none&taskId=u7d7b3614-690f-453f-9340-1831c875773)
<a name="J6IQE"></a>
### 2.对于要加载的类走特殊处理，比如：加密、解密；
 ![2021-06-25-21-36-25-251419.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1624628359769-644bad96-4e2b-4977-a333-45344be45b8f.png#clientId=u7ee0ecb2-6a28-4&from=ui&id=ue88bc295&originHeight=580&originWidth=493&originalType=binary&ratio=3&size=34151&status=done&style=none&taskId=u69fe753a-83e1-450b-9e11-0fc860e3bc7)![2021-06-25-21-36-25-450884.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1624628359785-bc0e056a-123b-4e43-beb5-b79f296eb377.png#clientId=u7ee0ecb2-6a28-4&from=ui&id=u94401a2e&originHeight=554&originWidth=495&originalType=binary&ratio=3&size=28597&status=done&style=none&taskId=ucf0b1a2e-22a1-47f8-9f97-2eebf3eb490)
<a name="rn0KG"></a>
## 类的热部署的咋回事？
JVM在加载类之前都会调用`findLoadedClass()`函数来检查是否能返回类实例，同时还会判断一下一个类是否是同一个类：一个是看这个类的类名和包名是否一样，再一个就是看类加载器是否是同一个(这里主要是看实例)；<br />![2021-06-25-21-36-25-594501.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1624628373268-d6eab551-624b-47be-990e-6e1cc9893705.png#clientId=u7ee0ecb2-6a28-4&from=ui&id=u04ff7936&originHeight=588&originWidth=498&originalType=binary&ratio=3&size=38924&status=done&style=none&taskId=ubb8da0f2-54f4-4f90-84f1-7ef5ada9a54)![2021-06-25-21-36-25-759068.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1624628373259-2d8dad11-eeda-48ac-be38-77236db68146.png#clientId=u7ee0ecb2-6a28-4&from=ui&id=u98ea1039&originHeight=483&originWidth=497&originalType=binary&ratio=3&size=24321&status=done&style=none&taskId=ufce2d436-7b4f-42d9-a086-bfff5a5a7c4)
<a name="fioVV"></a>
## Java 动态加载类是最优解吗？
做Java开发的都知道java有个痛点：每修改一个类就必须重启一次；java的优势是基于共享对象这个机制的，也就是通过保存并持有对象的状态而省去类信息的重复创建和回收，如果能动态加载类了，还能实现对象的平滑过度吗？ <br /> JVM中对象只有一份，理论上可以直接替换这个对象，做起来并不难吧，JVM只要更新一下java栈中所有对原对象的引用关系就行，好了，打住！别忘了对象的引用关系只能是对象的创建者持有和使用，JVM不能干涉的，JVM只知道对象的编译时类型，对于对象的运行时类型一无所知！<br />那是不是就没办法了？办法还是有的，办法总比问题多嘛，可以先不保存对象的状态，对象被创建使用后就被释放掉，等到下次修改的时候，对象就是新的了，听起来是不是很耳熟？对了，这就是JSP！其实不仅仅是JSP这么做，所有的解释性语言都这样做！
