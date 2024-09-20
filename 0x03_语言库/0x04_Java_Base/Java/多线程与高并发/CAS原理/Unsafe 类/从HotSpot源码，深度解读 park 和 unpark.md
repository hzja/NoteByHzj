Java<br />Java 并发包下的类基本都是基于AQS（AbstractQueuedSynchronizer）框架实现的，而 AQS 线程安全的实现又是基于两个很关键的类 `Unsafe` 和 `LockSupport`。<br />其中 `Unsafe` 主要直接提供 CAS 操作，`LockSupport` 主要提供 park/unpark 操作，而 park/unpark 最终调用还是 Unsafe 类，所以 Unsafe 类才是关键。<br />如果在面试中能从 JVM 源码的角度解读其原理，定会给面试官留下不一样的印象。下面开始看源码。<br />LockSupport 类源码：
```java
public static void park() {
    UNSAFE.park(false, 0L);
}
public static void unpark(Thread thread) {
    if (thread != null)
        UNSAFE.unpark(thread);
}
```
Unsafe 类源码：
```java
//park
public native void park(boolean isAbsolute, long time);
 //unpack
public native void unpark(Object var1);
```
由代码可见，Unsafe 类的 park/unpark 是 `native` 级别的实现。使用 `native` 关键字说明这个方法是原生函数，也就是这个方法是用 C/C++ 语言实现的，并且被编译成了 DLL，由 Java 去调用。<br />park 函数作用是将当前调用线程阻塞，unpark 函数则是将指定线程线程唤醒。
<a name="YL4nj"></a>
## park 和 unpark 作用
park 是等待一个许可，unpark 是为某线程提供一个许可。如果某线程A调用 park，那么除非另外一个线程调用 unpark(A) 给A一个许可，否则线程 A 将阻塞在 park 操作上。每次调用一次 park，需要有一个 unpark来解锁。<br />并且 unpark 可以先于 park 调用，但是不管 unpark 先调用几次，都只提供一个许可，不可叠加，只需要一次 park 来消费掉 unpark 带来的许可，再次调用会阻塞。
<a name="xQ8XQ"></a>
## `Unsafe.park` 源码
在 Linux 系统下，park 和 unpark 是用的 Posix 线程库 pthread 中的 mutex（互斥量），condition（条件变量）来实现的。<br />简单来说，mutex 和 condition 保护了一个叫 `_counter` 的信号量。当 park 时，这个变量被设置为 0，当 unpark 时，这个变量被设置为1。当 `_counter=0` 时线程阻塞，当 `_counter > 0` 直接设为 0 并返回。<br />每个 Java 线程都有一个 Parker 实例，Parker 类部分源码如下：
```cpp
class Parker : public os::PlatformParker {  
private:  
  volatile int _counter ;  
  ...  
public:  
  void park(bool isAbsolute, jlong time);  
  void unpark();  
  ...  
}  
class PlatformParker : public CHeapObj<mtInternal> {  
  protected:  
    pthread_mutex_t _mutex [1] ;  
    pthread_cond_t  _cond  [1] ;  
    ...  
}
```
由源码可知 Parker 类继承于 PlatformParker，实际上是用 Posix 的 mutex，condition 来实现的。Parker 类里的 _counter 字段，就是用来记录 park 和 unpark 是否需要阻塞的标识。
<a name="pBOlg"></a>
## 执行过程
具体的执行逻辑已经用注释标记在代码中，简要来说，就是检查 `_counter` 是不是大于 0，如果是，则把 `_counter` 设置为 0，返回。如果等于零，继续执行，阻塞等待。<br />具体请看源码中注释。
```cpp
void Parker::park(bool isAbsolute, jlong time) {
    //判断信号量counter是否大于0，如果大于设为0返回
    if (Atomic::xchg(0, &_counter) > 0) return;

    //获取当前线程
    Thread* thread = Thread::current();
    assert(thread->is_Java_thread(), "Must be JavaThread");
    JavaThread *jt = (JavaThread *)thread;

    //如果中途已经是interrupt了，那么立刻返回，不阻塞
    // Check interrupt before trying to wait
    if (Thread::is_interrupted(thread, false)) {
        return;
    }

    //记录当前绝对时间戳
    // Next, demultiplex/decode time arguments
    timespec absTime;
    //如果park的超时时间已到，则返回
    if (time < 0 || (isAbsolute && time == 0) ) { // don't wait at all
        return;
    }
    //更换时间戳
    if (time > 0) {
        unpackTime(&absTime, isAbsolute, time);
    }

    // Enter safepoint region
    // Beware of deadlocks such as 6317397.
    // The per-thread Parker:: mutex is a classic leaf-lock.
    // In particular a thread must never block on the Threads_lock while
    // holding the Parker:: mutex.  If safepoints are pending both the
    // the ThreadBlockInVM() CTOR and DTOR may grab Threads_lock.
    //进入安全点，利用该thread构造一个ThreadBlockInVM
    ThreadBlockInVM tbivm(jt);

    // Don't wait if cannot get lock since interference arises from
    // unblocking.  Also. check interrupt before trying wait
    if (Thread::is_interrupted(thread, false) || pthread_mutex_trylock(_mutex) != 0) {
        return;
    }

    //记录等待状态
    int status ;
    //中途再次检查许可，有则直接返回不等带。
    if (_counter > 0)  { // no wait needed
        _counter = 0;
        status = pthread_mutex_unlock(_mutex);
        assert (status == 0, "invariant") ;
        // Paranoia to ensure our locked and lock-free paths interact
        // correctly with each other and Java-level accesses.
        OrderAccess::fence();
        return;
    }

    OSThreadWaitState osts(thread->osthread(), false /* not Object.wait() */);
    jt->set_suspend_equivalent();
    // cleared by handle_special_suspend_equivalent_condition() or java_suspend_self()

    assert(_cur_index == -1, "invariant");
    if (time == 0) {
        _cur_index = REL_INDEX; // arbitrary choice when not timed
        //线程条件等待 线程等待信号触发，如果没有信号触发，无限期等待下去。
        status = pthread_cond_wait (&_cond[_cur_index], _mutex) ;
    } else {
        _cur_index = isAbsolute ? ABS_INDEX : REL_INDEX;
        //线程等待一定的时间，如果超时或有信号触发，线程唤醒
        status = os::Linux::safe_cond_timedwait (&_cond[_cur_index], _mutex, &absTime) ;
        if (status != 0 && WorkAroundNPTLTimedWaitHang) {
            pthread_cond_destroy (&_cond[_cur_index]) ;
            pthread_cond_init    (&_cond[_cur_index], isAbsolute ? NULL : os::Linux::condAttr());
        }
    }
    _cur_index = -1;
    assert_status(status == 0 || status == EINTR ||
        status == ETIME || status == ETIMEDOUT,
        status, "cond_timedwait");

    _counter = 0 ;
    status = pthread_mutex_unlock(_mutex) ;
    assert_status(status == 0, status, "invariant") ;
    // Paranoia to ensure our locked and lock-free paths interact
    // correctly with each other and Java-level accesses.
    OrderAccess::fence();

    // If externally suspended while waiting, re-suspend
    if (jt->handle_special_suspend_equivalent_condition()) {
        jt->java_suspend_self();
    }
}
```
<a name="EzJGb"></a>
## `Unsafe.unpark`源码
unpark 直接设置 _counter 为 1，再 unlock mutex 返回。如果 _counter 之前的值是 0，则还要调用 `pthread_cond_signal` 唤醒在 park 中等待的线程。源码如下，已做详细注释：
```cpp
void Parker::unpark() {
    //定义两个变量，staus用于判断是否获取锁
    int s, status ;
    //获取锁
    status = pthread_mutex_lock(_mutex);
    //判断是否成功
    assert (status == 0, "invariant") ;
    //存储原先变量_counter
    s = _counter;
    //把_counter设为1
    _counter = 1;
    if (s < 1) {
        // thread might be parked
        if (_cur_index != -1) {
            // thread is definitely parked
            if (WorkAroundNPTLTimedWaitHang) {
                status = pthread_cond_signal (&_cond[_cur_index]);
                assert (status == 0, "invariant");
                status = pthread_mutex_unlock(_mutex);
                assert (status == 0, "invariant");
            } else {
                status = pthread_mutex_unlock(_mutex);
                assert (status == 0, "invariant");
                status = pthread_cond_signal (&_cond[_cur_index]);
                assert (status == 0, "invariant");
            }
        } else {
            //释放锁
            pthread_mutex_unlock(_mutex);
            assert (status == 0, "invariant") ;
        }
    } else {
        //释放锁
        pthread_mutex_unlock(_mutex);
        assert (status == 0, "invariant") ;
    }
}
```
