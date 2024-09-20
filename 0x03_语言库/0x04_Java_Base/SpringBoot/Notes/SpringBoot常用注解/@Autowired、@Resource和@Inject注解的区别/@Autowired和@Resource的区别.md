JavaSpring
<a name="w0xKo"></a>
## `@Autowired`
`@Autowired`顾名思义，就是自动装配，而它的作用是为了消除代码Java代码里面的 getter/setter 与 bean 属性中的 property 。<br />当然，getter看个人需求，如果私有属性需要对外提供的话，应当予以保留。<br />也有很多面试官喜欢询问，Spring 的自动装配你了解么？其实问的也是这个注解，而这个注解中有几个属性，需要大家知道。
<a name="ewF8K"></a>
### no
默认值，表示不使用自动装配，Bean 依赖必须通过 ref 元素定义。
<a name="WtBMX"></a>
### byName
根据 Property 的 name 自动装配，如果一个 Bean 的 name 和另一个 Bean 中的 Property 的 name 相同，则自动装配这个 Bean 到 Property 中。（表示按属性名称自动装配，XML 文件中 Bean 的 id 必须与类中的属性名称相同）
<a name="S3MFk"></a>
### byType
根据 Property 的数据类型（Type）自动装配，如果一个 Bean 的数据类型兼容另一个 Bean 中 Property 的数据类型，则自动装配。（XML 文件中 Bean 的 id 与类中的属性名称可以不同，但必须只有一个类型的 Bean。）
<a name="Nsd2G"></a>
### constructor
类似于 byType，根据构造方法参数的数据类型，进行 byType 模式的自动装配。（类中构造函数的参数必须在配置文件中有相同的类型）
<a name="a4iB2"></a>
### autodetect（3.0版本不支持）
如果 Bean 中有默认的构造方法，则用 constructor 模式，否则用 byType 模式。<br />其实这些参数都指明了一件事情，这个 `@Autowired` 的注解，就是根据 type 也就是类型来进行装配，与之而来的就是 `@Resource` 他又是什么呢？
<a name="Mhnr5"></a>
## `@Resource`
官方描述为，一般用在类属性或者setter方法上声明属性需要自动装配。<br />如果在Resource注解中指定了bean的名称，则Spring会只按注解中的bame属性查找合适对象，然后进行装配。如果注解中没有指定名称，先按类属性的变量名查找，如果还是未找到，则按类型进行查找。<br />它的内部也有属性：
<a name="or62u"></a>
### name
定义了name属性的值，就只按照name值匹配，使用`byName`的自动注入策略
<a name="fMV1r"></a>
### type
定义了type属性的值，就只按照type值匹配，使用`byType`的自动注入策略<br />如果不指定的话，他自己会默认使用反射机制，使用`byName`自动注入策略。<br />那么面试官的问题就来了，`@Autowired` 和 `@Resource` 的区别在哪呢？什么时候选择用什么才合适呢？
<a name="vo8To"></a>
## `@Autowired` 和 `@Resource` 的区别
<a name="dIMkt"></a>
### 1.包就不一样
`@Autowired` 是Spring提供的。<br />`@Resource` 是J2EE提供的。也就是Java规范的。
<a name="py71b"></a>
### 2.装配的默认类型不一样
`@Autowired`只按type装配。<br />`@Resource`默认是按name装配。<br />`@Autowired`默认按类型装配，默认情况下必须要求依赖对象存在，如果要允许null值，可以设置它的`required`属性为`false`。<br />如果想使用名称装配可以结合`@Qualifier`注解进行使用。<br />而默认按照名称进行装配，名称可以通过name属性进行指定，如果没有指定name属性，当注解写在字段上时，默认取字段名进行名称查找。<br />如果注解写在setter方法上默认取属性名进行装配。当找不到与名称匹配的bean时才按照类型进行装配。但是需要注意的是，如果name属性一旦指定，就只会按照名称进行装配。<br />那么选择哪一种最好呢？<br />其实大家在开发的时候，有没有注意到一个地方，就是 IDEA 有时候还会在 `@Autowired` 注解上面Field injection is not recommended （字段注入是不被推荐的）这个警告，虽然并不属于报错，但是确实是警告的存在。但是 `@Resource` 就不会呢？<br />其实有很多人也说不上来为啥，`@Autowired`是Spring提供的，它是特定IoC提供的特定注解，这就导致了应用与框架的强绑定，一旦换用了其他的IoC框架，是不能够支持注入的。<br />而 `@Resource`是 Java 自己提供的，它是Java标准，使用的IoC容器应当去兼容它，这样即使更换容器，也可以正常工作。<br />那么为什么还有这么多的人使用的都是 `@Autowired` 注解呢？<br />那就只有两个字了，简单，使用构造器或者setter注入需要写更多业务无关的代码，十分麻烦，而字段注入大幅简化了它们。并且绝大多数情况下业务代码和框架就是强绑定的，完全松耦合只是一件理想上的事，牺牲了敏捷度去过度追求松耦合反而得不偿失。
