JavaSpring<br />用过 Spring 的朋友，或多或少见过这 2 个注解`@Qualifier`、`@Primary`，特别重要的 2 个注解，试问下自己

- 这俩注解是干什么的？
- 他们有什么区别呢？
<a name="KQ1dd"></a>
## 1、来看一段代码

- IService 接口有 2 个实现类
- 2 个类上标注了`@Component` 注解，会被注册到 Spring 容器
```java
public interface IService {
}

@Component
public class ServiceA implements IService{
}

@Component
public class ServiceB implements IService{
}
```
现在另外一个类需要使用 IService，通过下面的方式注入
```java
@Autowired
private IService service;
```
最终运行的时候，报错了，提示需要自动注入IService对象，但是容器中却找到了 2 个匹配的对象[serviceA, serviceB]，此时 Spring 不知道选择哪个，然后就报错啦。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1672188860828-be4a3e22-8035-436d-a142-97f135ee1589.png#averageHue=%23fbf7f5&clientId=u1f503322-c69a-4&from=paste&id=u280e52df&originHeight=317&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ua55cf9c5-5dbb-4029-abd3-dbc9486baae&title=)
<a name="Zm6kn"></a>
## 2、常见的 2 种解决方式
<a name="fi59I"></a>
### 2.1、方式 1：使用`@Qualifier` 注解解决
可以使用`@Qualifier("需要注入的bean的名称")`用来指定具体需要注入那个 bean，当存在多个 bean 的时候，可以通过这个注解来指定具体需要注入的 bean，这种方式控制方在使用方。
```java
@Autowired
@Qualifier("serviceB")
private IService service;
```
<a name="Sj0lT"></a>
### 2.2、方式 2：使用`@Primary` 解决
当自动注入的时候，存在多个候选 bean 的时候，可以选择一个候选 bean，在其上添加`@Primary`注解，标注这个 bean 为主要的候选 bean，当自动注入的时候，匹配到多个的时候，会优先选择`@Primary`的 bean 来作为目标 bean 进行注入，比如想让ServiceA作为首选的 bean，那么调整一下代码就行了，那么上面通过`@Autowired private IService service;`注入的就是 ServiceA。
```java
@Component
@Primary
public class ServiceA implements IService {
}
```
这里说一下`@Primary`的使用场景，比如原本只有 ServiceA，那么文本开头的注入是没有问题的，后续可能准备将ServiceA接口废掉，此时添加了一个ServiceB，那么只需在定义ServiceB的时候，加上`@Primary`注解就可以了。
