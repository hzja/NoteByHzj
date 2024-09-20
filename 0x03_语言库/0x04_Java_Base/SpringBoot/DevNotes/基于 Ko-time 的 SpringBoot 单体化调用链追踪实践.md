JavaSpringBoot

- 前言
- 关于Ko-Time
   1. 是什么？
   2. ko-time更新时间线
- Ko-time怎么用？
   1. 依赖引入
   2. 配置集成
   3. 权限放行
- 链路追踪
   1. 系统运行
   2. 链路追踪
   3. 长时间调用模拟
- 总结

---

<a name="mPPCB"></a>
## 1、前言
熟悉微服务的老司机一定了解，在微服务模式下，在一次调用链路中，可能设计到多个微服务，如果在线上，某个微服务出现故障，如何快速定位故障所在的微服务呢？**解决思路是可以使用链路追踪技。**<br />通常在链路追踪领域有以下的一些备选技术，比如可以用由大众点评开源，基于Java开发的实时应用监控平台Cat，包括实时应用监控，业务监控 。<br />集成方案是通过代码埋点的方式来实现监控，比如：拦截器，过滤器等。对代码的侵入性很大，集成成本较高、风险较大。<br />也可以采用韩国人开源的基于字节码注入的调用链分析，以及应用监控分析工具Pinpoint。它的特点是支持多种插件，UI功能强大，接入端无代码侵入。<br />还可以采用本土开源的基于字节码注入的调用链分析以及应用监控分析工具SkyWalking。特点是支持多种插件，UI功能较强，接入端无代码侵入。目前已加入Apache孵化器--开源。<br />上述场景是解决微服务的链路追踪的，那么一般采用单体化架构的开发模式时，在小场景下，您是如何定位代码的性能，在出现问题时，是通过log+调用起始时间进行分段汇总吗？这样费时又费力，主要还不直观。<br />那么怎么在Springboot架构下进行单体开发时，快速集成单体化模式下的链路追踪呢？<br />本文介绍一款开源的基于Java开发的监控组件，同时详细讲解如何在项目中进行深度集成，最后给出详细的实现案例。不仅方便进行调用链路追踪，还自带可视化UI，甚至有报警配置，喜欢的朋友可以前来看看。
<a name="cU03m"></a>
## 2、关于Ko-Time
<a name="FkY8W"></a>
### 1、是什么？
koTime是一个轻量级的SpringBoot项目性能分析工具，通过追踪方法调用链路以及对应的运行时长快速定位性能瓶颈，ko-time gitee地址：[https://gitee.com/huoyo/ko-time](https://gitee.com/huoyo/ko-time)

- ✅ 实时监听方法，统计运行时长
- ✅ web展示方法调用链路，瓶颈可视化追踪
- ✅ 追踪系统异常，精确定位到方法
- ✅ 接口超时邮件通知，无需实时查看
- ✅ 线上热更新：无需重启更新代码
- ✅ 线程管理：线程实时统计与状态查看
- ✅ 使用简单，无技术学习成本
- ✅ pom依赖即可，无代码侵入，无多余部署成本

![](https://cdn.nlark.com/yuque/0/2023/png/396745/1697689787921-b05f3530-b603-4518-ab0f-2ddda43b38d6.png#averageHue=%23fdfcfb&clientId=udba93213-7f21-4&from=paste&id=ue27e5de3&originHeight=554&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ue5f29398-6387-4cc8-a01e-cc987ea4d11&title=)
<a name="zZC9f"></a>
### 2、ko-time更新时间线
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1697689787875-0d89e86d-aa8b-4145-890c-0a078c08f15f.png#averageHue=%23fdfdfd&clientId=udba93213-7f21-4&from=paste&id=u97d0294b&originHeight=1094&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u064e12f4-cba6-411f-847b-80e562ab6d3&title=)
<a name="YHjSO"></a>
## 3、Ko-time怎么用？
<a name="kc3dv"></a>
### 1、依赖引入
```xml
<dependency>
  <groupId>cn.langpy</groupId>
  <artifactId>ko-time</artifactId>
  <version>2.4.1</version>
</dependency>
<!--阿里云maven仓库更新会慢一两天左右，拉取失败的切换到maven中央仓库-->
```
<a name="vdwDZ"></a>
### 2、配置集成
本次代码集成以ruoyi单体化架构集成为例，ruoyi的配置文件主要配置在yml中，需要进行一定的配置才能运行。官网提供的是基于properties的配置模式，下面提供yml的配置转换，具体代码如下：
```yaml
ko-time: 
  # 是否开启koTime，默认开启，当为false时，关闭koTime 
  enable: true
  # 是否开启控制输出，默认false
  log-enable: false    
  # 语言（english/chinese）默认chinese
  language: chinese  
  # 时间阈值，用于前端展示，大于阈值显示红色，小于阈值显示绿色，默认800
  threshold: 800.0   
  # context-path: http://localhost:80 # 前端页面调用接口的上下文环境，无法自动获取时可手动配置，一般情况切记不要配置   v2.0.1开始支持  
  exception-enable: true # 是否开启异常检测，默认为false,开启后会对方法内部抛出的异常进行统计 v2.0.0开始支持  
  # 是否开启认证，默认为false,开启后需要登录才能访问调用链路 v2.0.2开始支持
  auth-enable: false   
  # 登录用户 v2.0.2开始支持
  #user-name: admin   
  # 登录密码 v2.0.2开始支持 
  #password: 123456  
  # 使用静态的token值进行认证访问（/koTime?kotoken=xxx） v2.3.7开始支持，该模式和user-name认证模式二选一 
  #static-token: xxxx 
  # 是否开启入参组合分析 默认开启 v2.0.8开始支持 双击方法节点即可看到效果
  param-analyse: true  
  # 启动时是否删除过往数据 默认false v2.2.3开始支持
  data-reset: false 
  # 调用信息存储线程数（为了不影响项目本身性能，链路存储异步进行），默认2，该值并非越大越好，瓶颈取决于数据库性能和服务器配置，尽可能少占用项目资源为上  v2.2.0-BETA开始支持
  thread-num: 2 
  # 丢弃率（0-1） 同一个方法在多次连续调用时，耗时差距并不大，为了存储的IO性能考虑，可以随机丢弃一部分耗时数据。默认30% v2.2.5开始支持
  discard-rate: 0.3 
  # 登录超时时间考虑 默认43200s(12个小时) v2.3.0开始支持
  auth-expire: 43200 
  # 获取最新版本通知开关 在页面上可以看到 默认true v2.3.3开始支持
  version-notice: true 
  #需要监测的范围，是链路包含的范围，不仅仅是接口层，参考aop的@pointcut
  pointcut: execution(public * com.ruoyi.project..*.*(..))
```
请注意，以上部分参数的值请根据具体的代码进行修改，请不要直接复制，以免造成必须要的误解。<br />比如，在这里ko-time的web-ui访问页面的访问权限已经设置关闭，实际项目中一定要打开，并且设置一个用户名和密码。在这里可以配置代码的时间阈值，这里采用默认值，单位是毫秒。<br />最后需要注意的是，由于这种单体化切入的模式都是采用AOP模式，因此必须要设置合理的切入点。见配置文件中的pointcut参数配置，这里选择监控的是com.ruoyi.project下的任意方法。<br />关于切入配置参考如下：<br />假设项目的包路径为：
```
com.huoyo.demo
        |-controller
        |-service
        |-mapper
        |-others
            |-other1
            |-other2
            |-Test.java
```
想要切cn.langpy.demo下面的所有方法（包括子包中的），可以写：
```java
execution(public * com.huoyo.demo..*.*(..)) #切记，是两个点.
```
只想要切cn.langpy.demo.controller下面的类的所有方法（不包括子包的），可以写：
```java
execution(public * com.huoyo.demo.controller.*.*(..)) #切记，是一个点.
```
只想要切cn.langpy.demo.others下面的类的所有方法（不包括other1和other2下面的），可以写：
```java
execution(public * com.huoyo.demo.others.*.*(..))
```
只想要切cn.langpy.demo.others下面的类的所有方法（包括other1和other2），可以写：
```java
execution(public * com.huoyo.demo.others..*.*(..))
```
如果想排除某些类如Test.class，可以
```java
execution(public * com.huoyo.demo.others..*.*(..)) && !execution(public * com.huoyo.demo.others.Test(..))
```
<a name="FigmZ"></a>
### 3、权限放行
由于系统配置的权限，因此要在后台系统中将ko-time的访问权限放开。在ShiroConfig中进行配置。
```java
//设置/koTime允许匿名访问
filterChainDefinitionMap.put("/koTime/**","anon");
filterChainDefinitionMap.put("/koTime", "anon");
```
<a name="HyOFh"></a>
## 4、链路追踪
<a name="N1cRk"></a>
### 1、系统运行
不管是微服务的链路追踪或者单体化链路追踪，其原理都是需要进行服务和接口的调用，所以模拟进行服务访问。点击登录和进行一些方法的操作。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1697689787913-9d5f89d3-adbc-44b7-9e54-77c7a04abca3.png#averageHue=%23dcddd8&clientId=udba93213-7f21-4&from=paste&id=ue50a9519&originHeight=613&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uf03323e1-1bea-4117-82f0-7fe95a90a46&title=)
<a name="GpauT"></a>
### 2、链路追踪
在进行了页面访问之后，在浏览器中输入http://ip:port/koTime就可以访问到链路追踪的web界面。这里就详细的列出了调用信息，比如接口总数，列表，异常列表，线程列表。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1697689787918-c1d3697d-87b8-4cf1-963d-18cf47e3317a.png#averageHue=%23f3f7f5&clientId=udba93213-7f21-4&from=paste&id=u7e7970d2&originHeight=1101&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u3a2bfd2b-1b96-47fb-991d-e42f9ff5a52&title=)<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1697689787924-abed8ab5-67bb-4535-b09c-fbdb8f4df0a5.png#averageHue=%23eeefec&clientId=udba93213-7f21-4&from=paste&id=u7f188d08&originHeight=700&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ua056fb74-3a77-4093-b448-2ccec9f34ea&title=)<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1697689792575-a3297423-1049-4128-995e-68fd326dc1d9.png#averageHue=%23f3f2f1&clientId=udba93213-7f21-4&from=paste&id=u23dcb8a9&originHeight=1319&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uc89be355-4f1f-4aea-bc8b-5f1a845791a&title=)
<a name="nELFs"></a>
### 3、长时间调用模拟
为了模拟实际的访问慢的接口，在代码中进行了休眠设置（线上代码不建议如此）。
```java
/**
 * 修改子元素关系
 * 
 * @param deptId 被修改的部门ID
 * @param newAncestors 新的父ID集合
 * @param oldAncestors 旧的父ID集合
 */
public void updateDeptChildren(Long deptId, String newAncestors, String oldAncestors){
    try {
        for(int i =0;i<5;i++) {
            Thread.sleep(200);
        }
    } catch (InterruptedException e) {
    }
    List<Dept> children = deptMapper.selectChildrenDeptById(deptId);
    for (Dept child : children){
        child.setAncestors(child.getAncestors().replaceFirst(oldAncestors, newAncestors));
    }
    if (children.size() > 0){
        deptMapper.updateDeptChildren(children);
    }
}
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1697689793424-4551fb7c-f4c4-4ecc-8938-66fa021802a6.png#averageHue=%23fcfcfc&clientId=udba93213-7f21-4&from=paste&id=uf781d357&originHeight=1006&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u4eaf2cdf-6127-44cb-bd5b-3beb7c73044&title=)<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1697689794130-ca112669-89b3-49af-8923-3f85c4006c48.png#averageHue=%23fcfbfb&clientId=udba93213-7f21-4&from=paste&id=u7d11635f&originHeight=466&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u40dbe061-c1fa-4d7f-970f-9f733719d7d&title=)
<a name="O0GrN"></a>
## 5、总结
介绍了一款开源的基于Java开发的监控组件，同时详细讲解如何在项目中进行深度集成，最后给出详细的实现案例。不仅方便进行调用链路追踪，还自带可视化UI，甚至有报警配置。
