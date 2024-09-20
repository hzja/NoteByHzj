就上线来说，如果组件或者容器没有启动成功，就不应该对外暴露服务，对于下线来说，如果机器已经停机了，就应该保证服务已下线，如此可避免上游流量进入不健康的机器。
<a name="cDDoF"></a>
## 1、优雅下线
<a name="Brj4i"></a>
### 基础下线（Spring/SpringBoot/内置容器）
首先JVM本身是支持通过`shutdownHook`的方式优雅停机的。
```java
Runtime.getRuntime().addShutdownHook(new Thread() {
    @Override
    public void run() {
        close();
    }
});
```
此方式支持在以下几种场景优雅停机:<br />1）程序正常退出 <br />2）使用`System.exit() `<br />3）终端使用Ctrl+C <br />4）使用Kill pid干掉进程<br />那么如果偏偏要kill -9 程序肯定是不知所措的。<br />而在Springboot中，其实已经实现好了一个`shutdownHook`，支持响应Ctrl+c或者kill -15 TERM信号。<br />随便启动一个应用，然后Ctrl+c一下，观察日志就可知，它在`AnnotationConfigEmbeddedWebApplicationContext`这个类中打印出了疑似Closing...的日志，真正的实现逻辑在其父类<br />`AbstractApplicationContext`中(这个其实是spring中的类，意味着什么呢，在spring中就支持了对优雅停机的扩展)。
```java
public void registerShutdownHook() {
    if (this.shutdownHook == null) {
        this.shutdownHook = new Thread() {
            public void run() {
                synchronized(AbstractApplicationContext.this.startupShutdownMonitor) {
                    AbstractApplicationContext.this.doClose();
                }
            }
        };
        Runtime.getRuntime().addShutdownHook(this.shutdownHook);
    }

}

public void destroy() {
    this.close();
}

public void close() {
    Object var1 = this.startupShutdownMonitor;
    synchronized(this.startupShutdownMonitor) {
        this.doClose();
        if (this.shutdownHook != null) {
            try {
                Runtime.getRuntime().removeShutdownHook(this.shutdownHook);
            } catch (IllegalStateException var4) {
                ;
            }
        }

    }
}

protected void doClose() {
    if (this.active.get() && this.closed.compareAndSet(false, true)) {
        if (this.logger.isInfoEnabled()) {
            this.logger.info("Closing " + this);
        }

        LiveBeansView.unregisterApplicationContext(this);

        try {
            this.publishEvent((ApplicationEvent)(new ContextClosedEvent(this)));
        } catch (Throwable var3) {
            this.logger.warn("Exception thrown from ApplicationListener handling ContextClosedEvent", var3);
        }

        if (this.lifecycleProcessor != null) {
            try {
                this.lifecycleProcessor.onClose();
            } catch (Throwable var2) {
                this.logger.warn("Exception thrown from LifecycleProcessor on context close", var2);
            }
        }

        this.destroyBeans();
        this.closeBeanFactory();
        this.onClose();
        this.active.set(false);
    }

}
```
能对它做些什么呢，其实很明显，在`doClose`方法中它发布了一个`ContextClosedEvent`的方法，不就是给扩展用的么。于是可以写个监听器监听`ContextClosedEvent`，在发生事件的时候做下线逻辑，对微服务来说即是从注册中心中注销掉服务。
```java
@Component
public class GracefulShutdownListener implements ApplicationListener<ContextClosedEvent> {

    @Override
    public void onApplicationEvent(ContextClosedEvent contextClosedEvent){
        //注销逻辑
        zookeeperRegistry.unregister(mCurrentServiceURL);
        ...
    }
}
```
可能会有疑问的是，微服务中一般来说，注销服务往往是优雅下线的第一步，接着才会执行停机操作，那么这个时候流量进来怎么办呢？<br />个人会建议是，在注销服务之后就可开启请求挡板拒绝流量了，通过微服务框架本身的故障转移功能去处理被拒绝的流量即可。
<a name="ZHbvu"></a>
### Docker中的下线
用docker部署服务，支不支持优雅下线。那来看看docker的一些停止命令都会干些啥：<br />一般来说，正常人可能会用`docker stop`或者`docker kill` 命令去关闭容器（当然如果上一步注册了USR2自定义信息，可能会通过`docker exec kill -12`去关闭）。<br />对于`docker stop`来说，它会发一个SIGTERM(kill -15 term信息)给容器的PID1进程，并且默认会等待10s，再发送一个SIGKILL(kill -9 信息)给PID1。<br />那么很明显，docker stop允许程序有个默认10s的反应时间去做一下优雅停机的操作，程序只要能对kill -15 信号做些反应就好了，如上一步描述。那么这是比较良好的方式。<br />当然如果`shutdownHook`方法执行了个50s，那肯定不优雅了。可以通过docker stop -t 加上等待时间。
<a name="e5DRk"></a>
### 外置容器的shutdown脚本（Jetty）
如果非要用外置容器方式部署（个人认为浪费资源并提升复杂度）。那么能不能优雅停机呢。<br />可以当然也是可以的，这里有两种方式：<br />首先RPC框架本身提供优雅上下线接口，以供调用来结束整个应用的生命周期，并且提供扩展点供开发者自定义服务下线自身的停机逻辑。同时调用该接口的操作会封装成一个preStop操作固化在jetty或者其他容器的shutdown脚本中，保证在容器停止之前先调用下线接口结束掉整个应用的生命周期。shutdown脚本中执行类发起下线服务 -> 关闭端口 -> 检查下线服务直至完成 -> 关闭容器的流程。<br />而更简单的另一种方法是直接在脚本中加入`kill -15`命令。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1697177810028-d2b172b6-d60e-4896-acd5-9f07018b2932.png#averageHue=%23aeafae&clientId=u5ef51985-b59d-4&from=paste&id=u70f0b2cd&originHeight=560&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ueaca41df-f653-4c3c-b0bd-8e43be760a6&title=)
<a name="Lllu0"></a>
## 2、优雅上线
优雅上线相对来说可能会更加困难一些，因为没有什么默认的实现方式，但是总之呢，一个原则就是确保端口存在之后才上线服务。
<a name="ck0BY"></a>
### SpringBoot内置容器优雅上线
这个就很简单了，并且业界在应用层面的优雅上线均是在内置容器的前提下实现的，并且还可以配合一些列健康检查做文章。<br />参看sofa-boot的健康检查的源码，它会在程序启动的时候先对SpringBoot的组件做一些健康检查，然后再对它自己搞得sofa的一些中间件做健康检查，整个健康检查的流程完毕之后（sofaboot 目前是没法对自身应用层面做健康检查的，它有写相关接口，但是写死了port is ready...）才会暴露服务或者说优雅上线，那么它健康检查的时机是什么时候呢：
```java
@Override
public void onApplicationEvent(ContextRefreshedEvent event) {
    healthCheckerProcessor.init();
    healthIndicatorProcessor.init();
    afterHealthCheckCallbackProcessor.init();
    publishBeforeHealthCheckEvent();
    readinessHealthCheck();
}
```
可以看到它是监听了`ContextRefreshedEvent`这个事件。在内置容器模式中，内置容器模式的start方法是在`refreshContext`方法中，方法执行完成之后发布一个`ContextRefreshedEvent`事件，也就是说在监听到这个事件的时候，内置容器必然是启动成功了的。<br />但`ContextRefreshedEvent`这个事件，在一些特定场景中由于种种原因，`ContextRefreshedEvent`会被监听到多次，没有办法保证当前是最后一次event，从而正确执行优雅上线逻辑。<br />在SpringBoot中还有一个更加靠后的事件，叫做`ApplicationReadyEvent`，它的发布藏在了`afterRefresh`还要后面的那一句`listeners.finished(context, null)`中，完完全全可以保证内置容器 端口已经存在了，所以可以监听这个事件去做优雅上线的逻辑，甚至可以把中间件相关的健康检查集成在这里。
```java
@Component
public class GracefulStartupListener implements ApplicationListener<ApplicationReadyEvent> {    
    @Override
    public void onApplicationEvent(ApplicationReadyEvent applicationReadyEvent){
        //注册逻辑 优雅上线
        apiRegister.register(urls);
        ...
    }
}
```
<a name="QEmxV"></a>
### 外置容器(Jetty)优雅上线
目前大多数应用的部署模式不管是jetty部署模式还是docker部署模式(同样使用jetty镜像)，本质上用的都是外置容器。那么这个情况就比较困难了，至少在应用层面无法观察到外部容器的运行状态，并且容器本身没有提供什么hook实现。<br />那么和优雅上线一样，需要RPC框架提供优雅上线接口来初始化整个应用的生命周期，并且提供扩展点给开发者供执行自定义的上线逻辑(上报版本探测信息等)。同样将调用这个接口封装成一个postStart操作，固化在jetty等外置容器的startup脚本中，保证应用在容器启动之后在上线。容器执行类似启动容器 -> 健康检查 -> 上线服务逻辑 -> 健康上线服务直至完成 的流程。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1697177810066-973d1c2e-f1c1-4411-bae7-e7b8f1363c2f.png#averageHue=%23f6f7fa&clientId=u5ef51985-b59d-4&from=paste&id=u4572def7&originHeight=716&originWidth=937&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u56e6c4f1-009a-4422-b261-a8091e37a71&title=)
