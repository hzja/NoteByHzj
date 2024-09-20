JavaSpring<br />总结：在类上使用 Lombok的`@RequiredArgsConstructor` 注解来替代类中的多处`@Autowired`和`@Resource`，原本的多个注解，现在简化为一个。
<a name="PeaU2"></a>
## 依赖注入
先回顾一下Spring的3种依赖注入。
<a name="g49Vg"></a>
### 属性注入
```java
public class SysUserController extends BaseController {
    @Autowired
    private ISysUserService userService;

    @Resource
    private ISysRoleService roleService;
}
```
`@Autowired`默认按类型装配，`@Resource`默认按名称装配，当找不到与名称匹配的bean时，才会按类型装配。<br />而`@Qualifier`和`@Autowired`配合使用，指定bean的名称，也可以做到按名称装配。<br />IDEA中直接在变量上使用 `@Autowired`会发现警告提示：Field injection is not recommended。<br />原因是官方建议使用构造器注入方式，这种方式存在明显的弊端，比如：注入对象不能用`final`修饰、无法发现NullPointException的存在。
<a name="tmxsR"></a>
### 构造器注入
```java
public class SysUserController extends BaseController {

    private final ISysUserService userService;

    private final ISysRoleService roleService;

    public SysUserController(ISysUserService userService, ISysRoleService roleService) {
        this.userService = userService;
        this.roleService = roleService;
    }
}
```
构造器依赖注入通过容器触发一个类的构造器来实现的，通过强制指明依赖注入来保证这个类的运行，防止NullPointerException；<br />Spring官方推荐使用构造器注入不仅是因为这种情况下成员属性可以使用`final`关键字修饰，更关键的一点是能够避免循环依赖，如果存在循环依赖，Spring项目启动的时候就会报错。<br />为什么说是避免而不是解决呢？<br />因为构造器注入是通过构造方法来生成对象，其必须要先获取属性，才能生成调用构造方法进行实例化，这种情况的循环依赖是无法解决的。<br />下面通过一张图来看下A、B俩相互依赖的对象注入方式不同时Spring能否解决循环依赖的情况：

| 依赖情况 | 依赖注入方式 | 循环依赖是否被解决 |
| --- | --- | --- |
| AB相互依赖（循环依赖） | 均采用setter方法注入 | 是 |
| AB相互依赖（循环依赖） | 均采用构造器注入 | 否 |
| AB相互依赖（循环依赖） | A中注入B的方式为setter方法，B中注入A的方式为构造器 | 是 |
| AB相互依赖（循环依赖） | B中注入A的方式为setter方法，A中注入B的方式为构造器 | 否 |

构造器注入方式解决循环依赖:

1. 代码重构
2. `@Lazy`注解
3. 改用属性注入
<a name="Mu33s"></a>
### Setter注入
```java
public class SysUserController extends BaseController {

    private ISysUserService userService;

    @Autowired
    public void setUserService(ISysUserService userService) {
        this.userService = userService;
    }
}
```
需要注意的是，在使用Setter注入时需要加`@Autowired`或`@Resource`注解，否则是无法注入成功的。<br />另外要注意一点，属性注入和Setter注入的变量都无法使用`final`关键字修饰。
<a name="FWdfN"></a>
## `@RequiredArgsConstructor`
这里可能会有人说不推荐使用Lombok，只要知其然且知其所以然，那他就是一个帮助我们快速开发的好工具。<br />在说完Spring的三种依赖注入后，来认识一下Lombok的`@RequiredArgsConstructor` 注解。<br />在Lombok中，生成构造方法的注解一共有三个，分别是`@NoArgsConstructor`, `@RequiredArgsConstructor`, `@AllArgsContructor`，这里只介绍`@RequiredArgsConstructor`。
```java
@Controller
@RequiredArgsConstructor
public class SysUserController extends BaseController {

    private final ISysUserService userService;

    private ISysRoleService roleService;

    //----------------------------
}
```
使用`@RequiredArgsConstructor`会生成一个包含常量、使用`final`关键字修饰的变量的私有构造方法。<br />那就可以不使用属性注入(`@Autowired`和`@Resource`)的方式，直接通过构造器的方式来完成注入，不仅能够省略简化许多代码，也解决了属性注入可能存在的空指针问题。<br />当然，如果没有 Lombok 的，也可以自己封装一个类似实现。甚至是全局实现，连 `@RequiredArgsConstructor` 注解也可以省了！！！
