Java SpringBoot
<a name="OxzfC"></a>
## 何为优雅关机
就是为确保应用关闭时，通知应用进程释放所占用的资源

- 线程池，shutdown（不接受新任务等待处理完）还是`shutdownNow`（调用 `Thread.interrupt`进行中断）
- socket 链接，比如：netty、mq
- 告知注册中心快速下线（靠心跳机制客服早都跳起来了），比如：eureka
- 清理临时文件，比如：poi
- 各种堆内堆外内存释放

总之，进程强行终止会带来数据丢失或者终端无法恢复到正常状态，在分布式环境下还可能导致数据不一致的情况。
<a name="qTtBQ"></a>
## kill指令
`kill -9 pid` 可以模拟了一次系统宕机，系统断电等极端情况，而`kill -15 pid` 则是等待应用关闭，执行阻塞操作，有时候也会出现无法关闭应用的情况（线上理想情况下，是bug就该寻根溯源）
```bash
#查看jvm进程pid
jps
#列出所有信号名称
kill -l

# Windows下信号常量值
# 简称  全称  数值  
# INT   SIGINT     2       Ctrl+C中断
# ILL   SIGILL     4       非法指令
# FPE   SIGFPE     8       floating point exception(浮点异常)
# SEGV  SIGSEGV    11      segment violation(段错误)
# TERM  SIGTERM    5       Software termination signal from kill(Kill发出的软件终止)
# BREAK SIGBREAK   21      Ctrl-Break sequence(Ctrl+Break中断)
# ABRT  SIGABRT    22      abnormal termination triggered by abort call(Abort)

#linux信号常量值
# 简称  全称数值  
# HUP   SIGHUP      1    终端断线  
# INT   SIGINT      2    中断（同 Ctrl + C）        
# QUIT  SIGQUIT     3    退出（同 Ctrl + \）         
# KILL  SIGKILL     9    强制终止         
# TERM  SIGTERM     15    终止         
# CONT  SIGCONT     18    继续（与STOP相反， fg/bg命令）         
# STOP  SIGSTOP     19    暂停（同 Ctrl + Z）        
#....

#可以理解为操作系统从内核级别强行杀死某个进程
kill -9 pid 
#理解为发送一个通知，等待应用主动关闭
kill -15 pid
#也支持信号常量值全称或简写（就是去掉SIG后）
kill -l KILL
```
思考：jvm是如何接受处理linux信号量的？<br />当然是在jvm启动时就加载了自定义`SignalHandler`，关闭jvm时触发对应的handle。
```java
public interface SignalHandler {
    SignalHandler SIG_DFL = new NativeSignalHandler(0L);
    SignalHandler SIG_IGN = new NativeSignalHandler(1L);

    void handle(Signal var1);
}
class Terminator {
    private static SignalHandler handler = null;

    Terminator() {
    }
    //jvm设置SignalHandler，在System.initializeSystemClass中触发
    static void setup() {
        if (handler == null) {
            SignalHandler var0 = new SignalHandler() {
                public void handle(Signal var1) {
                    Shutdown.exit(var1.getNumber() + 128);//调用Shutdown.exit
                }
            };
            handler = var0;

            try {
                Signal.handle(new Signal("INT"), var0);//中断时
            } catch (IllegalArgumentException var3) {
                ;
            }

            try {
                Signal.handle(new Signal("TERM"), var0);//终止时
            } catch (IllegalArgumentException var2) {
                ;
            }

        }
    }
}
```
<a name="iTZqf"></a>
## `Runtime.addShutdownHook`
在了解`Shutdown.exit`之前，先看<br />`Runtime.getRuntime().addShutdownHook(shutdownHook);`则是为jvm中增加一个关闭的钩子，当jvm关闭的时候调用。
```java
public class Runtime {
    public void addShutdownHook(Thread hook) {
        SecurityManager sm = System.getSecurityManager();
        if (sm != null) {
            sm.checkPermission(new RuntimePermission("shutdownHooks"));
        }
        ApplicationShutdownHooks.add(hook);
    }
}


class ApplicationShutdownHooks {
    /* The set of registered hooks */
    private static IdentityHashMap<Thread, Thread> hooks;
    static synchronized void add(Thread hook) {
        if(hooks == null)
            throw new IllegalStateException("Shutdown in progress");

        if (hook.isAlive())
            throw new IllegalArgumentException("Hook already running");

        if (hooks.containsKey(hook))
            throw new IllegalArgumentException("Hook previously registered");

        hooks.put(hook, hook);
    }
}


//它含数据结构和逻辑管理虚拟机关闭序列
class Shutdown {
    /* Shutdown 系列状态*/
    private static final int RUNNING = 0;
    private static final int HOOKS = 1;
    private static final int FINALIZERS = 2;
    private static int state = RUNNING;
    /* 是否应该运行所以finalizers来exit? */
    private static boolean runFinalizersOnExit = false;
    // 系统关闭钩子注册一个预定义的插槽.
    // 关闭钩子的列表如下:
    // (0) Console restore hook
    // (1) Application hooks
    // (2) DeleteOnExit hook
    private static final int MAX_SYSTEM_HOOKS = 10;
    private static final Runnable[] hooks = new Runnable[MAX_SYSTEM_HOOKS];
    // 当前运行关闭钩子的钩子的索引
    private static int currentRunningHook = 0;
    /* 前面的静态字段由这个锁保护 */
    private static class Lock { };
    private static Object lock = new Lock();

    /* 为native halt方法提供锁对象 */
    private static Object haltLock = new Lock();

    static void add(int slot, boolean registerShutdownInProgress, Runnable hook) {
        synchronized (lock) {
            if (hooks[slot] != null)
                throw new InternalError("Shutdown hook at slot " + slot + " already registered");

            if (!registerShutdownInProgress) {//执行shutdown过程中不添加hook
                if (state > RUNNING)//如果已经在执行shutdown操作不能添加hook
                    throw new IllegalStateException("Shutdown in progress");
            } else {//如果hooks已经执行完毕不能再添加hook。如果正在执行hooks时，添加的槽点小于当前执行的槽点位置也不能添加
                if (state > HOOKS || (state == HOOKS && slot <= currentRunningHook))
                    throw new IllegalStateException("Shutdown in progress");
            }

            hooks[slot] = hook;
        }
    }
    /* 执行所有注册的hooks
     */
    private static void runHooks() {
        for (int i=0; i < MAX_SYSTEM_HOOKS; i++) {
            try {
                Runnable hook;
                synchronized (lock) {
                    // acquire the lock to make sure the hook registered during
                    // shutdown is visible here.
                    currentRunningHook = i;
                    hook = hooks[i];
                }
                if (hook != null) hook.run();
            } catch(Throwable t) {
                if (t instanceof ThreadDeath) {
                    ThreadDeath td = (ThreadDeath)t;
                    throw td;
                }
            }
        }
    }
    /* 关闭JVM的操作
     */
    static void halt(int status) {
        synchronized (haltLock) {
            halt0(status);
        }
    }
    //JNI方法
    static native void halt0(int status);
    // shutdown的执行顺序：runHooks > runFinalizersOnExit
    private static void sequence() {
        synchronized (lock) {
            /* Guard against the possibility of a daemon thread invoking exit
             * after DestroyJavaVM initiates the shutdown sequence
             */
            if (state != HOOKS) return;
        }
        runHooks();
        boolean rfoe;
        synchronized (lock) {
            state = FINALIZERS;
            rfoe = runFinalizersOnExit;
        }
        if (rfoe) runAllFinalizers();
    }
    //Runtime.exit时执行，runHooks > runFinalizersOnExit > halt
    static void exit(int status) {
        boolean runMoreFinalizers = false;
        synchronized (lock) {
            if (status != 0) runFinalizersOnExit = false;
            switch (state) {
            case RUNNING:       /* Initiate shutdown */
                state = HOOKS;
                break;
            case HOOKS:         /* Stall and halt */
                break;
            case FINALIZERS:
                if (status != 0) {
                    /* Halt immediately on nonzero status */
                    halt(status);
                } else {
                    /* Compatibility with old behavior:
                     * Run more finalizers and then halt
                     */
                    runMoreFinalizers = runFinalizersOnExit;
                }
                break;
            }
        }
        if (runMoreFinalizers) {
            runAllFinalizers();
            halt(status);
        }
        synchronized (Shutdown.class) {
            /* Synchronize on the class object, causing any other thread
             * that attempts to initiate shutdown to stall indefinitely
             */
            sequence();
            halt(status);
        }
    }
    //shutdown操作，与exit不同的是不做halt操作(关闭JVM)
    static void shutdown() {
        synchronized (lock) {
            switch (state) {
            case RUNNING:       /* Initiate shutdown */
                state = HOOKS;
                break;
            case HOOKS:         /* Stall and then return */
            case FINALIZERS:
                break;
            }
        }
        synchronized (Shutdown.class) {
            sequence();
        }
    }
}
```
<a name="D9fQS"></a>
## Spring 3.2.12
在Spring中通过`ContextClosedEvent`事件来触发一些动作（可以拓展），主要通过`LifecycleProcessor.onClose`来做`stopBeans`。由此可见Spring也基于jvm做了拓展。
```java
public abstract class AbstractApplicationContext extends DefaultResourceLoader {
    public void registerShutdownHook() {
        if (this.shutdownHook == null) {
            // No shutdown hook registered yet.
            this.shutdownHook = new Thread() {
                @Override
                public void run() {
                    doClose();
                }
            };
            Runtime.getRuntime().addShutdownHook(this.shutdownHook);
        }
    }
    protected void doClose() {
        boolean actuallyClose;
        synchronized (this.activeMonitor) {
            actuallyClose = this.active && !this.closed;
            this.closed = true;
        }
        
        if (actuallyClose) {
            if (logger.isInfoEnabled()) {
                logger.info("Closing " + this);
            }
            
            LiveBeansView.unregisterApplicationContext(this);
            
            try {
                //发布应用内的关闭事件
                publishEvent(new ContextClosedEvent(this));
            }
            catch (Throwable ex) {
                logger.warn("Exception thrown from ApplicationListener handling ContextClosedEvent", ex);
            }
            
            // 停止所有的Lifecycle beans.
            try {
                getLifecycleProcessor().onClose();
            }
            catch (Throwable ex) {
                logger.warn("Exception thrown from LifecycleProcessor on context close", ex);
            }
            
            // 销毁spring 的 BeanFactory可能会缓存单例的 Bean.
            destroyBeans();
            
            // 关闭当前应用上下文（BeanFactory）
            closeBeanFactory();
            
            // 执行子类的关闭逻辑
            onClose();
            
            synchronized (this.activeMonitor) {
                this.active = false;
            }
        }
    }  
}
public interface LifecycleProcessor extends Lifecycle {
    /**
    * Notification of context refresh, e.g. for auto-starting components.
    */
    void onRefresh();
    
    /**
    * Notification of context close phase, e.g. for auto-stopping components.
    */
    void onClose();
}
```
<a name="S3CEb"></a>
## SpringBoot
到这里就进入重点了，SpringBoot中有spring-boot-starter-actuator 模块提供了一个 restful 接口，用于优雅停机。执行请求 `curl -X POST http://127.0.0.1:8088/shutdown` ，待关闭成功则返回提示。<br />注：线上环境该url需要设置权限，可配合 spring-security使用或在nginx中限制内网访问
```
#启用shutdown
endpoints.shutdown.enabled=true
#禁用密码验证
endpoints.shutdown.sensitive=false
#可统一指定所有endpoints的路径
management.context-path=/manage
#指定管理端口和IP
management.port=8088
management.address=127.0.0.1


#开启shutdown的安全验证（spring-security）
endpoints.shutdown.sensitive=true
#验证用户名
security.user.name=admin
#验证密码
security.user.password=secret
#角色
management.security.role=SUPERUSER
```
SpringBoot的shutdown原理也不复杂，其实还是通过调用`AbstractApplicationContext.close`实现的。 
```java

@ConfigurationProperties(
    prefix = "endpoints.shutdown"
)
public class ShutdownMvcEndpoint extends EndpointMvcAdapter {
    public ShutdownMvcEndpoint(ShutdownEndpoint delegate) {
        super(delegate);
    }
    //post请求
    @PostMapping(
        produces = {"application/vnd.spring-boot.actuator.v1+json", "application/json"}
    )
    @ResponseBody
    public Object invoke() {
        return !this.getDelegate().isEnabled() ? new ResponseEntity(Collections.singletonMap("message", "This endpoint is disabled"), HttpStatus.NOT_FOUND) : super.invoke();
    }
}

@ConfigurationProperties(
    prefix = "endpoints.shutdown"
)
public class ShutdownEndpoint extends AbstractEndpoint<Map<String, Object>> implements ApplicationContextAware {
    private static final Map<String, Object> NO_CONTEXT_MESSAGE = Collections.unmodifiableMap(Collections.singletonMap("message", "No context to shutdown."));
    private static final Map<String, Object> SHUTDOWN_MESSAGE = Collections.unmodifiableMap(Collections.singletonMap("message", "Shutting down, bye..."));
    private ConfigurableApplicationContext context;

    public ShutdownEndpoint() {
        super("shutdown", true, false);
    }
    //执行关闭
    public Map<String, Object> invoke() {
        if (this.context == null) {
            return NO_CONTEXT_MESSAGE;
        } else {
            boolean var6 = false;

            Map var1;

            class NamelessClass_1 implements Runnable {
                NamelessClass_1() {
                }

                public void run() {
                    try {
                        Thread.sleep(500L);
                    } catch (InterruptedException var2) {
                        Thread.currentThread().interrupt();
                    }
                    //这个调用的就是AbstractApplicationContext.close
                    ShutdownEndpoint.this.context.close();
                }
            }

            try {
                var6 = true;
                var1 = SHUTDOWN_MESSAGE;
                var6 = false;
            } finally {
                if (var6) {
                    Thread thread = new Thread(new NamelessClass_1());
                    thread.setContextClassLoader(this.getClass().getClassLoader());
                    thread.start();
                }
            }

            Thread thread = new Thread(new NamelessClass_1());
            thread.setContextClassLoader(this.getClass().getClassLoader());
            thread.start();
            return var1;
        }
    }
}
```
