JavaThreadLocal<br />想要子线程获取父线程中`ThreadLocal`中的值，需要其子类`InheritableThreadLocal`实现。<br />**测试代码**
```java
Thread parentParent = new Thread(() ->
                                 ThreadLocal<Integer> threadLocal = new ThreadLocal<>();
    threadLocal.set(1);
    InheritableThreadLocal<Integer> inheritableThreadLocal = new InheritableThreadLocal<>();
    inheritableThreadLocal.set(2);
    new Thread(() -> {
        System.out.println("threadLocal=" + threadLocal.get());
        System.out.println("inheritableThreadLocal=" + inheritableThreadLocal.get());
    }).start();
    }, "父线程");
    parentParent.start();
}
```
**运行结果**<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1663738937762-a2895e2d-c87e-40d2-b777-b65179b16b71.jpeg#clientId=ued0c8af8-3a21-4&errorMessage=unknown%20error&from=paste&id=u27d2bf8f&originHeight=142&originWidth=748&originalType=url&ratio=1&rotation=0&showTitle=false&status=error&style=none&taskId=u19d3e34c-1859-48d0-850c-fbea417b494&title=)<br />**原理如下**<br />**首先要知道 Thread类维护了两个**`**ThreadLocalMap**`<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1663738937780-737a7873-ff6d-49e5-8124-a6803b2afca9.jpeg#clientId=ued0c8af8-3a21-4&errorMessage=unknown%20error&from=paste&id=u33b9f194&originHeight=326&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=error&style=none&taskId=u1da08819-e654-47d6-85f0-50ada8370e1&title=)<br />**跟进**`**new Thread()**`** 方法**<br />其构造方法调用了`init`方法，`init`方法把`inheritThreadLocals`值设置为了`true`<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1663738937699-b96dcaf6-779b-468a-81d5-e2ed1f463e21.jpeg#clientId=ued0c8af8-3a21-4&errorMessage=unknown%20error&from=paste&id=uebe2ffef&originHeight=275&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=error&style=none&taskId=ueceb04f8-a30b-4c94-9031-4936e756a9f&title=)<br />**继续跟进**<br />当`inheritThreadLocals`的值为`true`并且其父线程的`inheritableThreadLocals`不为`null`时, 把其父线程`inheritableThreadLocals `赋值给当前线程的`inheritableThreadLocals`。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1663738937769-ed1ab6a8-d9fa-4924-a570-88300eeced6f.jpeg#clientId=ued0c8af8-3a21-4&errorMessage=unknown%20error&from=paste&id=ua55e7cbe&originHeight=116&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=error&style=none&taskId=u929b3e42-061e-4bc4-badf-2c325067989&title=)<br />这就是子线程可以获取到父线程ThreadLocal值的关键。<br />**继续跟进 看看 **`**InheritableThreadLocal**`** 的**`**get()**`** 方法**<br />`get()`方法没什么好看的，就是`ThreadLocal`的`get()`方法。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1663738937698-d265052a-0d10-437d-90fb-4e1537c52dc3.jpeg#clientId=ued0c8af8-3a21-4&errorMessage=unknown%20error&from=paste&id=u14ed2763&originHeight=534&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=error&style=none&taskId=u0cec8ddb-eaa4-4bdf-833b-c1f444099fe&title=)<br />**注意：**`InheritableThreadLocal `对`ThreadLocal` 的`getMap()`方法进行重写。
