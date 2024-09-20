<a name="r8dMW"></a>
## 创建一个Java线程的路径
![DM_20231106212136_01.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1699277108209-037e4c39-b911-46b2-a7f3-dc658614bb07.png#averageHue=%23f9f8f4&clientId=u2ba83722-070d-4&from=ui&id=u6bcc8cfb&originHeight=1292&originWidth=2094&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=1254250&status=done&style=none&taskId=u725cbf3a-b9f9-4e8d-8a29-326ee25def0&title=)<br />模糊掉内部的控制逻辑，首先聚焦于线程创建的主流程：
<a name="E8i9J"></a>
## 创建不同层级的线程对象(由Java语言层向下传递)
创建Java语言层面的Thread对象，调用`start()`方法后；会调用JVM的`JVM_StartThread()`方法创建JavaThread对象，并按照当前虚拟机所处的操作系统创建不同的OSThread对象(操作系统核心级线程)；
<a name="UWduv"></a>
## 不同层级的线程依次启动(从OS层开始向上传递)
核心级线程创建成功后，会调用入口函数：`thread_native_entry`方法，该方法会调用`JavaThread::run`方法启动JVM线程，JVM线程启动后会调用Thread对象传入的`thread_entry`方法，该方法会调用`Thread::run`方法-即代码中定义的该线程需要执行的代码块；
<a name="gLRjW"></a>
## 由Java语言层向下传递
<a name="gwMi1"></a>
### Java语言层
在Java语言中，手动创建线程时，会通过类似下面的方式启动：
```java
// 定义Thread对象(提供线程run()方法的实现)
Thread runnableThread = new Thread(() -> {
    System.out.println("[runnableThread] begin start");
    System.out.println("[runnableThread] end start");
});

// 启动定义的Thread (如果我们手动调用Thread.run()，并不会出现上述的日志答打印)
runnableThread.start();
```
进入`start()`方法：
```java
public synchronized void start() {
    /**
     * Thread状态 : 0 -> "NEW"
     */
    if (threadStatus != 0)
        throw new IllegalThreadStateException();

	boolean started = false;
    try {
        // native 方法
        start0();
        started = true;
    }
}

private native void start0();
```
`start0()`方法需要在**「映射表(native方法 -> JVM方法的映射)」** 中找到对应的的JVM层的方法，而Thread.java中的static代码块调用 `registerNatives()`方法初始化了映射表：
```java
// ----------------------------- Java语言层 ---------------------------//
public class Thread implements Runnable {
    // 这个方法对应了JVM中的 Java_java_lang_Thread_registerNatives 方法
    private static native void registerNatives();
    static {
        registerNatives();
    }
}    

// ----------------------------- JVM层 ---------------------------//
Java_java_lang_Thread_registerNatives(JNIEnv *env, jclass cls) {
    // methods即为映射表数组
    (*env)->RegisterNatives(env, cls, methods, ARRAY_LENGTH(methods));
}

// 映射表 eg. 调用start0时 会通过该映射表映射到 JVM_StartThread 这个JVM方法中
static JNINativeMethod methods[] = {
    {"start0",           "()V",        (void *)&JVM_StartThread},
    {"stop0",            "(" OBJ ")V", (void *)&JVM_StopThread},
    {"isAlive",          "()Z",        (void *)&JVM_IsThreadAlive},
    {"suspend0",         "()V",        (void *)&JVM_SuspendThread},
    {"resume0",          "()V",        (void *)&JVM_ResumeThread},
    {"setPriority0",     "(I)V",       (void *)&JVM_SetThreadPriority},
    {"yield",            "()V",        (void *)&JVM_Yield},
    {"sleep",            "(J)V",       (void *)&JVM_Sleep},
    {"currentThread",    "()" THD,     (void *)&JVM_CurrentThread},
    {"countStackFrames", "()I",        (void *)&JVM_CountStackFrames},
    {"interrupt0",       "()V",        (void *)&JVM_Interrupt},
    {"isInterrupted",    "(Z)Z",       (void *)&JVM_IsInterrupted},
    {"holdsLock",        "(" OBJ ")Z", (void *)&JVM_HoldsLock},
    {"getThreads",        "()[" THD,   (void *)&JVM_GetAllThreads},
    {"dumpThreads",      "([" THD ")[[" STE, (void *)&JVM_DumpThreads},
    {"setNativeName",    "(" STR ")V", (void *)&JVM_SetNativeThreadName},
};
```
于是随着`start0()`对应的`JVM_StartThread`这个JVM方法被执行，代码的层次也从Java语言层向下传递到了JVM层；
<a name="HfLoL"></a>
## JVM层
<a name="Q8gJC"></a>
### JVM_StartThread
`JVM_StartThread`方法主要是通过Java语言层的Thread对象中传递的信息，创建了`JavaThread`这个JVM线程对象：
```java
JVM_ENTRY(void, JVM_StartThread(JNIEnv* env, jobject jthread))
  JavaThread *native_thread = NULL;
  // 通过bool量表示是否有异常，因为下面持有 Threads_lock 锁时，抛出异常会导致锁不能被释放
  bool throw_illegal_thread_state = false;
  
  {
    // 获取 Threads_lock 这个JVM内部的 Mutex锁
    MutexLocker mu(Threads_lock);

    // 如果线程已经创建成功，则不可以重复创建
    // PS: 不可以通过Thread状态机判断，因为创建线程成功与修改线程状态这两个操作是非原子的，存在窗口期
    if (java_lang_Thread::thread(JNIHandles::resolve_non_null(jthread)) != NULL) {
      throw_illegal_thread_state = true;
    } else {
      // 获取Thread中定义的栈大小，一般我们创建Thread对象时不会显式指定传入stackSize对象
      // 未指定时，后面在创建栈时会使用 -Xss 这个JVM参数指定的栈大小
      jlong size = java_lang_Thread::stackSize(JNIHandles::resolve_non_null(jthread));
     
      // 创建JVM线程(用JavaThread对象表示)
      // thread_entry 为 Thread::run
      native_thread = new JavaThread(&thread_entry, sz);
     
      ...

JVM_END
```
<a name="pJxpL"></a>
### 构建JavaThread对象
构建JavaThread对象，通过glibc库方法pthread_create创建内核级线程，并将JavaThread(JVM)层线程对象与操作系统内核级线程关联起来；<br />以Linux系统为例，一个内核级线程会对应一个JavaThread(一对一)；
```java
JavaThread::JavaThread(ThreadFunction entry_point, size_t stack_sz) :
                       Thread() {
  // entry_point 线程创建成功后的入口代码 (这里是Java语言层面传入的 Thread.run()方法)
  set_entry_point(entry_point);
  // 创建的Thread类型为javaThread
  os::ThreadType thr_type = os::java_thread;

  // 调用不同操作系统的创建线程方法创建线程
  os::create_thread(this, thr_type, stack_sz);
}

/**
 * JVM为每一种操作系统(linux、bsd...)都写了对应的创建线程的OS方法 
 * 下文以Linux为例
 */
bool os::create_thread(Thread* thread, ThreadType thr_type,
                       size_t req_stack_size) {

  // 操作系统线程对象
  OSThread* osthread = new OSThread(NULL, NULL);
  if (osthread == NULL) {
    return false;
  }
  
  // 将JavaThread(JVM)层线程对象与操作系统内核级线程关联起来，
  // 这样就可以通过使用JavaThread操作内核级线程 (一对一的关系)
  thread->set_osthread(osthread);
  {
    // 值-结果变量
    pthread_t tid;
    // 通过pthread_create方法创建内核级线程
    // thread_native_entry 这个是内核级线程的 start_routine(线程创建成功后会调用)
    int ret = pthread_create(&tid, &attr, (void* (*)(void*)) thread_native_entry, thread);

    // 操作系统 tid (轻量级进程Id)
    osthread->set_pthread_id(tid);
  }
  return true;
}
```
<a name="En9UA"></a>
## OS 层 (通过glibc进行调用)
glibc中的pthread_create方法主要是创建一个OS内核级线程，不深入细节，主要是为该线程分配了栈资源；**「需要注意的是这个栈资源对于JVM而言是堆外内存，因此堆外内存的大小会影响JVM可以创建的线程数。」**<br />对于内核级线程，操作系统会为其创建一套栈：用户栈+内核栈，其中用户栈工作在用户态，内核栈工作在内核态，在发生系统调用时，线程的执行会从用户栈切换到内核栈；<br />**「那么这一套栈与JVM规范中定义的JVM栈+本地方法栈有什么联系呢？」** 在JVM概念中，JVM栈用来执行Java方法，而本地方法栈用来执行native方法；但需要注意的是JVM只是在概念上区分了这两种栈，而并没有规定如何实现，在HotSpot中，则是将JVM栈与本地方法栈二合一，使用核心线程的用户栈来实现(因为JVM栈和本地方法栈都是属于用户态的栈)，即Java方法与native方法都在同一个用户栈中调用，而当发生系统调用时，再切换到核心栈运行。
<a name="r1JTL"></a>
### pthread_create : __pthread_create_2_1
```c
int __pthread_create_2_1 (pthread_t *newthread, const pthread_attr_t *attr,
                          void *(*start_routine) (void *), void *arg)
{
    STACK_VARIABLES;
    struct pthread *pd = NULL;

    // 上游传递的线程属性
    const struct pthread_attr *iattr = (struct pthread_attr *) attr;
    // 为线程分配CPU资源
    cpuset = malloc (cpusetsize);
    // 为线程分配栈资源，分配失败抛出OOM异常-ENOMEM
    int err = ALLOCATE_STACK (iattr, &pd);
    if (__glibc_unlikely (err != 0)) {
        retval = err == ENOMEM ? EAGAIN : err;
        goto out;
    }

    // 后续主要为通过当前核心级线程的TCB clone 出一个新的核心级线程的TCB(包括内核栈与用户栈), 这里不再赘述

    return retval;
}
```
<a name="wBM0x"></a>
### thread_native_entry
OS内核级线程创建完成后，会执行thread_native_entry这个线程入口函数进行OS内核级线程的初始化，并开始运行JavaThread：
```c
static void *thread_native_entry(Thread *thread) {
    OSThread* osthread = thread->osthread();
    // 此时运行的是通过glibc创建的linux核心级线程，所以当前的线程id为linux线程的id
    osthread->set_thread_id(os::current_thread_id());

    {
        // OS核心级线程初始化完毕
        osthread->set_state(INITIALIZED);
    }

    // 初始化完成JVM对应的操作系统线程后，开始运行JavaThread::run
    thread->run();

    return 0;
}
```
<a name="iACk1"></a>
## 从OS层开始向上传递
OS层 -> JVM层 (通过调用 `JavaThread::run` 方法)。
```cpp
// The first routine called by a new Java thread
void JavaThread::run() {
    // 初始化TLAB 线程缓冲区(-XX:+UseTLAB)
    this->initialize_tlab();

    // 设置栈、确定栈的生长方向
    this->record_base_of_stack_pointer();
    this->record_stack_base_and_size();

    thread_main_inner();
}

void JavaThread::thread_main_inner() {
    // JavaThread对象中传入的entry_point为Thread对象的Thread::run方法
    this->entry_point()(this, this);
}
```
<a name="CDTUz"></a>
## JVM层 -> Java语言层 (通过调用`Thread::run`方法)
```cpp
static void thread_entry(JavaThread* thread, TRAPS) {
    HandleMark hm(THREAD);
    Handle obj(THREAD, thread->threadObj());
    JavaValue result(T_VOID);
    // JavaCalls模块是用来调用Java方法的
    // 这里其实就是开始调用Java线程对象的run()方法
    JavaCalls::call_virtual(&result,
        obj, // Thread对象
        SystemDictionary::Thread_klass(), // Java_java_lang_Thread
        vmSymbols::run_method_name(), // run()
        vmSymbols::void_method_signature(), // V
        THREAD);
}
```
<a name="REw66"></a>
## 总结
至此，分析完了一个线程从Java语态到JVM语态再到OS语态的全过程；可以看到，对于HotSpot VM而言，其在Linux操作系统上实现的Java线程即对OS内核级线程进行了一对一的映射，所有的线程调度、内存分配都交由操作系统进行管理。
