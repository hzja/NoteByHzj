Java动态代理
<a name="qgsn8"></a>
## **前言**
动态代理应用非常的广泛，在各种开源的框架中都能看到他们的身影，比如Spring中的aop使用动态代理增强，mybatis中使用动态代理生成mapper，动态代理主要有JDK和CGLIB两种方式，来学习下这两种方式的实现，以及它们的优缺点<br />动态代理：是使用反射和字节码的技术，在运行期创建指定接口或类的子类，以及其实例对象的技术，通过这个技术可以无侵入的为代码进行增强
<a name="drUiN"></a>
## **一、JDK实现的动态代理**
<a name="fEPoK"></a>
### 1、解析
jdk实现的动态代理由两个重要的成员组成，分别是`Proxy`、`InvocationHandler`

- `**Proxy**`**：** 是所有动态代理的父类，它提供了一个静态方法来创建动态代理的class对象和实例
- `**InvocationHandler**`**：** 每个动态代理实例都有一个关联的`InvocationHandler`，在代理实例上调用方法是，方法调用将被转发到`InvocationHandler`的`invoke`方法
<a name="DcBSl"></a>
### 2、简单看下jdk的动态代理的原理图
![2022-10-02-23-44-16.733824700.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1664725510588-8fb1a4a4-c967-4034-a70b-a81ad8fb83e8.png#clientId=u45a42e99-f81e-4&from=ui&id=vD3o6&originHeight=420&originWidth=1043&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1316804&status=done&style=shadow&taskId=u1ef0b8b2-ee3e-46e5-b7a4-2247193cd4c&title=)
<a name="aov0M"></a>
### 3、代码实现
现在模拟一个用户注册的功能，动态代理对用户的注册功能进行增强，会判断用户名和密码的长度，如果用户名<=1和密码<6则会抛出异常<br />User.java
```java
package com.taolong;

public class User {

    private String name;

    private Integer age;

    private String password;

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }


    public Integer getAge() {
        return age;
    }

    public void setAge(Integer age) {
        this.age = age;
    }

    public String getPassword() {
        return password;
    }

    public void setPassword(String password) {
        this.password = password;
    }

    @Override
    public String toString() {
        return "User [name=" + name + ", age=" + age + ", password=" + password + "]";
    }

}
```
UserService.java
```java
package com.taolong.jdk;

import com.taolong.User;

public interface UserService {

    void addUser(User user);
}
```
UserServiceImpl.java
```java
package com.taolong.jdk;

import com.taolong.User;

public class UserServiceImpl implements UserService {

    @Override
    public void addUser(User user) {
        System.out.println("jdk...正在注册用户，用户信息为："+user);
    }

}
```
UserServiceInterceptor.java
```java
package com.taolong.jdk;

import java.lang.reflect.InvocationHandler;
import java.lang.reflect.Method;

import com.taolong.User;

public class UserServiceInterceptor implements InvocationHandler {

    private Object realObj;

    public UserServiceInterceptor(Object realObject) {
        super();
        this.realObj = realObject;
    }

    @Override
    public Object invoke(Object proxy, Method method, Object[] args) throws Throwable {
        if (args!=null && args.length > 0 && args[0] instanceof User) {
            User user = (User)args[0];
            //进行增强判断
            if (user.getName().length() <= 1) {
                throw new RuntimeException("用户名长度必须大于1");
            }
            if (user.getPassword().length() <= 6) {
                throw new RuntimeException("密码长度必须大于6");
            }
        }
        Object result = method.invoke(realObj, args);
        System.out.println("用户注册成功...");
        return result;
    }

    public Object getRealObj() {
        return realObj;
    }

    public void setRealObj(Object realObj) {
        this.realObj = realObj;
    }

}
```
ClientTest.java
```java
package com.taolong.jdk;

import java.lang.reflect.InvocationHandler;
import java.lang.reflect.Proxy;

import com.taolong.User;

public class ClientTest {

    public static void main(String[] args) {
        User user = new User();
        user.setName("hongtaolong");
        user.setPassword("hong");
        user.setAge(23);
        //被代理类
        UserService delegate = new UserServiceImpl();
        InvocationHandler userServiceInterceptor = new UserServiceInterceptor(delegate);
        //动态代理类
        UserService userServiceProxy = (UserService)Proxy.newProxyInstance(delegate.getClass().getClassLoader(),
                                                                           delegate.getClass().getInterfaces(), userServiceInterceptor);
        System.out.println("动态代理类："+userServiceProxy.getClass());
        userServiceProxy.addUser(user);
    }
}
```
运行结果：当密码的长度小于6时<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1664725429123-695db536-b883-46a4-9ef2-6fc6ce4baa58.png#clientId=u45a42e99-f81e-4&from=paste&id=uf10d7708&originHeight=109&originWidth=822&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uc2b71e0f-a9e4-4a51-bfab-e426da7fe57&title=)<br />这里就起到了动态增强的作用，mybatis的使用中知道不需要创建dao中的mapper接口的子类，也能调用到相应的方法，其实就是生成的实现了mapper接口的动态的代理类，可以去看看它的这个方法<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1664725429110-875174b8-5a1f-4b63-99f2-c4e391735d83.png#clientId=u45a42e99-f81e-4&from=paste&id=ue7e50668&originHeight=191&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u9478bdaf-7f8c-4459-b657-abcaa6b4c15&title=)<br />接下来看下cglib的使用。
<a name="Fa6Y0"></a>
## **二、CGLIB动态代理**
<a name="pdEEI"></a>
### 1、解析
CGLIB（Code Generation Library）是一个基于ASM的字节码生成库，它允许在运行时对字节码进行修改和动态生成。CGLIB通过继承的方式实现代理（最后这部分深思一下，它可能有哪些局限，`final`方法是不能够被重写，所以它不能增强被`final`修饰的方法，这个等下来验证）<br />CGLIB的实现也有两个重要的成员组成，`Enhancer`、`MethodInterceptor`，其实这两个的使用和jdk实现的动态代理的`Proxy`、`InvocationHandler`非常相似

- `**Enhancer**`**：** 来指定要代理的目标对象，实际处理代理逻辑的对象，最终通过调用`create()`方法得到代理对象、对这个对象所有的非final方法的调用都会转发给`MethodInterceptor`
- `**MethodInterceptor**`**：** 动态代理对象的方法调用都会转发到`intercept`方法进行增强
<a name="CVnXM"></a>
### 2、图解
![2022-10-02-23-44-16.768781700.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1664725510506-967bbad9-09a5-4619-ab5c-aa8b074d8211.png#clientId=u45a42e99-f81e-4&from=ui&id=u57c9b7fc&originHeight=388&originWidth=911&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1062589&status=done&style=shadow&taskId=u24aac5ec-3b9f-4d62-9ea8-5eef8dda20e&title=)
<a name="q8N9B"></a>
### 3、代码的实现
还是上面的场景，注册用户进行拦截增强，部分代码如下<br />UserServiceCglibInterceptor.java
```java
package com.taolong.cglib;

import java.lang.reflect.Method;

import com.taolong.User;

import net.sf.cglib.proxy.MethodInterceptor;
import net.sf.cglib.proxy.MethodProxy;

public class UserServiceCglibInterceptor implements MethodInterceptor {

    private Object realObject;

    public UserServiceCglibInterceptor(Object realObject) {
        super();
        this.realObject = realObject;
    }

    @Override
    public Object intercept(Object object, Method method, Object[] args, MethodProxy proxy) throws Throwable {
        if (args!=null && args.length > 0 && args[0] instanceof User) {
            User user = (User)args[0];
            //进行增强判断
            if (user.getName().length() <= 1) {
                throw new RuntimeException("用户名长度必须大于1");
            }
            if (user.getPassword().length() <= 6) {
                throw new RuntimeException("密码长度必须大于6");
            }
        }
        Object result = method.invoke(realObject, args);
        System.out.println("用户注册成功...");
        return result;
    }

}
```
ClientTest.java
```java
package com.taolong.cglib;

import com.taolong.User;

import net.sf.cglib.proxy.Enhancer;

public class ClientTest {

    public static void main(String[] args) {
        User user = new User();
        user.setName("hongtaolong");
        user.setPassword("hong");
        user.setAge(23);
        //被代理的对象
        UserServiceImplCglib delegate = new UserServiceImplCglib();
        UserServiceCglibInterceptor serviceInterceptor = new UserServiceCglibInterceptor(delegate);
        Enhancer enhancer = new Enhancer();
        enhancer.setSuperclass(delegate.getClass());
        enhancer.setCallback(serviceInterceptor);
        //动态代理类
        UserServiceImplCglib cglibProxy = (UserServiceImplCglib)enhancer.create();
        System.out.println("动态代理类父类："+cglibProxy.getClass().getSuperclass());
        cglibProxy.addUser(user);
    }
}
```
运行结果：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1664725429193-1a7c5d14-ffd9-471e-a530-dc755e9a5611.png#clientId=u45a42e99-f81e-4&from=paste&id=u018e7487&originHeight=118&originWidth=957&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u2df52b3d-d75b-40cf-991b-a618d3f5874&title=)<br />这里顺便打印了动态代理类的父类，接下来将它的父类UserServiceImplCglib的addUser方法用`final`修饰，看看是否会被增强<br />UserServiceImplCglib.java
```java
package com.taolong.cglib;

import com.taolong.User;

public class UserServiceImplCglib {

    final void addUser(User  user) {
        System.out.println("cglib...正在注册用户，用户信息为："+user);
    }
}
```
运行结果：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1664725429389-520ed1db-c9c3-4c3f-ac84-86a2cf0bdef9.png#clientId=u45a42e99-f81e-4&from=paste&id=ud373ed03&originHeight=91&originWidth=670&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u669fd072-6f27-4ce1-acbe-2e0fb107bb9&title=)
