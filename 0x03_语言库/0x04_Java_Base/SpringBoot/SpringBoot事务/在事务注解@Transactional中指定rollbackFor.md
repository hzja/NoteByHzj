Java Spring 事务
<a name="KZSBb"></a>
### 异常的分类
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1595830960506-3f7414a2-99a6-490e-817c-fd42d1fe9175.png#align=left&display=inline&height=512&originHeight=1535&originWidth=1568&size=138262&status=done&style=shadow&width=522.6666666666666)

| **运行时异常** | **非运行时异常** |
| :---: | :---: |
| NullPointerException - 空指针引用异常<br />ClassCastException - 类型强制转换异常<br />IllegalArgumentException - 传递非法参数异常<br />ArrayStoreException - 向数组中存放与声明类型不兼容对象异常<br />IndexOutofBoundsException -下标越界异常<br />NegativeArraysizeException - 创建一个大小为负数的数组错误异常<br />NumberF ormatException - 数字格式异常<br />SecurityException - 安全异常<br />UnsupportedOperationException - 不支持的操作异常 | IOException |
| 运行中逻辑错误引起的 | 编译时无法通 |
| 不可查的异常(unchecked exceptions) |  |

error是一定会回滚的<br />这里Exception是异常，他又分为运行时异常RuntimeException和非运行时异常

- 可查的异常（checked exceptions）:Exception下除了RuntimeException外的异常<br />
- 不可查的异常（unchecked exceptions）:RuntimeException及其子类和错误（Error）<br />

如果不对运行时异常进行处理，那么出现运行时异常之后，要么是线程中止，要么是主程序终止。  如果不想终止，则必须捕获所有的运行时异常，决不让这个处理线程退出。<br />队列里面出现异常数据了，正常的处理应该是把异常数据舍弃，然后记录日志。不应该由于异常数据而影响下面对正常数据的处理。<br />非运行时异常是RuntimeException以外的异常，类型上都属于Exception类及其子类。如IOException、SQLException等以及用户自定义的Exception异常。<br />对于这种异常，JAVA编译器强制要求必需对出现的这些异常进行catch并处理，否则程序就不能编译通过。所以，面对这种异常不管是否愿意，只能自己去写一大堆catch块去处理可能的异常。
<a name="ffe068f8"></a>
### `@Transactional` 的用法
**Spring框架的事务基础架构代码将默认地只在抛出运行时和unchecked exceptions时才标识事务回滚。**
> 也就是说，当抛出个RuntimeException 或其子类例的实例时。（Errors 也一样 - 默认地 - 标识事务回滚。）从事务方法中抛出的Checked exceptions将不被标识进行事务回滚。

- 让checked例外也回滚：在整个方法前加上 `@Transactional(rollbackFor=Exception.class)`<br />
- 让unchecked例外不回滚：`@Transactional(notRollbackFor=RunTimeException.class)`<br />
- 不需要事务管理的(只查询的)方法：`@Transactional(propagation=Propagation.NOT_SUPPORTED)`<br />
> 注意： 如果异常被try｛｝catch｛｝了，事务就不回滚了，如果想让事务回滚必须再往外抛try｛｝catch｛throw Exception｝。

<a name="F5jgk"></a>
### 注意
Spring团队的建议是在具体的类（或类的方法）上使用 `@Transactional` 注解，而不要使用在类所要实现的任何接口上。<br />当然可以在接口上使用 `@Transactional` 注解，但是这将只能当设置了基于接口的代理时它才生效。<br />因为**注解是不能继承的**，这就意味着如果正在使用基于类的代理时，那么事务的设置将不能被基于类的代理所识别，而且对象也将不会被事务代理所包装（将被确认为严重的）。因此，接受Spring团队的建议并且在具体的类上使用 `@Transactional` 注解。<br />`@Transactional` 注解标识的方法，处理过程尽量的简单。尤其是带锁的事务方法，能不放在事务里面的最好不要放在事务里面。可以将常规的数据库查询操作放在事务前面进行，而事务内进行增、删、改、加锁查询等操作。
> 注：rollbackFor 可以指定能够触发事务回滚的异常类型。Spring默认抛出了未检查unchecked异常（继承自 RuntimeException 的异常）或者 Error才回滚事务；其他异常不会触发回滚事务。
> 如果在事务中抛出其他类型的异常，但却期望 Spring 能够回滚事务，就需要指定 rollbackFor属性。


