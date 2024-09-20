JavaSpringBoot事务<br />在 Spring Boot 中操作事务有两种方式：编程式事务或声明式事务，接下来一起来看二者的具体实现。
<a name="wALCm"></a>
## 1、编程式事务
在 Spring Boot 中实现编程式事务又有两种实现方法：

1. 使用 `TransactionTemplate` 对象实现编程式事务；
2. 使用更加底层的 `TransactionManager` 对象实现编程式事务。

它们的具体实现代码如下。
<a name="owBIW"></a>
### 1.1 `TransactionTemplate` 使用
要使用 `TransactionTemplate`  对象需要先将 `TransactionTemplate` 注入到当前类中 ，然后再使用它提供的 `execute` 方法执行事务并返回相应的执行结果，如果程序在执行途中出现了异常，那么就可以使用代码手动回滚事务，具体实现代码如下：![](https://cdn.nlark.com/yuque/0/2022/png/396745/1663047437070-86308022-7a53-48f6-ad05-048e333fc6c3.png#clientId=u8a41356e-e420-4&from=paste&id=uc8b8b1b7&originHeight=811&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u70f9c7e4-7366-4142-99e5-95604c56bce&title=)
<a name="Wdj1m"></a>
### 1.2 `TransactionManager` 使用
`TransactionManager` 实现编程式事务相对麻烦一点，它需要使用两个对象：`TransactionManager` 的子类，加上 `TransactionDefinition` 事务定义对象，再通过调用 `TransactionManager` 的 `getTransaction` 获取并开启事务，然后调用 `TransactionManager` 提供的 `commit` 方法提交事务，或使用它的另一个方法 `rollback` 回滚事务，它的具体实现代码如下：![](https://cdn.nlark.com/yuque/0/2022/png/396745/1663047437105-e047407e-d48e-4a69-b9c7-93f96efbbaa1.png#clientId=u8a41356e-e420-4&from=paste&id=u1b61d7be&originHeight=810&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ua14e090c-d6dc-48c9-9503-2863ad66e92&title=)从上述代码可以看出，**使用编程式事务更加灵活，但写法比较麻烦**。
<a name="VT3Ym"></a>
## 2、声明式事务
声明式事务的实现比较简单，**只需要在方法上或类上添加 **`**@Transactional**`** 注解即可，当加入了 **`**@Transactional**`** 注解就可以实现在方法执行前，自动开启事务；在方法成功执行完，自动提交事务；如果方法在执行期间，出现了异常，那么它会自动回滚事务。**它的具体使用如下：![](https://cdn.nlark.com/yuque/0/2022/png/396745/1663047437100-602beea9-6978-4bd0-a375-e51f78009b1e.png#clientId=u8a41356e-e420-4&from=paste&id=u36c9babe&originHeight=383&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u2838509b-9af7-4ca0-8193-a10bf9d8e0b&title=)当然，`@Transactional` 支持很多参数的设置，它的参数设置列表如下：![](https://cdn.nlark.com/yuque/0/2022/png/396745/1663047437008-fdcc9441-c8ba-44a8-b35a-10789f53d63a.png#clientId=u8a41356e-e420-4&from=paste&id=u452ce717&originHeight=694&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uafc33146-a10b-480f-88f9-7939a8f4480&title=)参数的设置方法如下：![](https://cdn.nlark.com/yuque/0/2022/png/396745/1663047437024-40bb0d3c-c3ca-4f48-844a-3f36ebd1dea6.png#clientId=u8a41356e-e420-4&from=paste&id=u7de29938&originHeight=436&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u9dcb7eb1-3252-4a39-ac2c-05ea3947e66&title=)
<a name="eWbT9"></a>
## 总结
这里介绍了两种事务的实现方式：编程式事务或声明式事务。其中编程式事务又分为两种实现：使用 `TransactionTemplate` 对象或更加底层的 `TransactionManager` 对象分别实现编程式事务，它们的优点是灵活性更高，可以在任何代码片段上添加事务；而声明式事务的实现更加简单，只需要在类或方法上添加 `@Transactional` 注解即可实现事务的自动开启和提交（以及回滚）。
