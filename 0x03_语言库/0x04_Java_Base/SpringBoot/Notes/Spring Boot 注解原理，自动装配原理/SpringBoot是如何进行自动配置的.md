JavaSpringBoot
<a name="XG6HZ"></a>
### SpringBoot
都知道 SpringBoot 是要比原始的 SpringMVC 这些好用的，毕竟如果经历过最早的 SSM 模式的开发的话，一定对那些大批量的繁琐的配置文件印象颇深，因为之前使用 SSM 框架来进行开发的时候，那配置文件多的让人心态都容易崩溃，所以就有了这个 SpringBoot 来简化这些配置项，于是面试官就开始了对 SpringBoot 的各种面试题的问法，而比较经典的，就是 SpringBoot 是如何实现自动配置的。<br />来说一下这个问题。
<a name="sWvfk"></a>
### SpringBootApplication
SpringBoot 一直是约定优于配置。具体的体现就是在下面的几点

- maven 项目的配置文件存放在 resources 资源目录下。
- maven 项目默认编译后的文件放于 target 目录。
- maven 项目默认打包成 jar 格式。
- 配置文件默认为 application.yml 或者 application.yaml 或者 application.properties。
- 默认通过配置文件 spring.profiles.active 来激活配置。

SpringBootApplication 实际上是一个复合注解，其实复合注解就是把一些其他的注解组合起来组装成一个壳子，而外面套的这个壳子，就是复合注解。<br />其中，`@SpringBootConfiguration`，`@EnableAutoConfiguration`，`@ComponentScan`三个注解尤为重要。来看看这三个注解是干嘛用的。
<a name="syHvL"></a>
### `SpringBootConfiguration`
```java
@Target(ElementType.TYPE)
@Retention(RetentionPolicy.RUNTIME)
@Documented
@Configuration
public @interface SpringBootConfiguration {

    @AliasFor(annotation = Configuration.class)
    boolean proxyBeanMethods() default true;

}
```
它实际上就是一个 `@Configuration` 注解，这个注解大家应该很熟悉了，加上这个注解就是为了让当前类作为一个配置类交由 Spring 的 IOC 容器进行管理
<a name="zqXNZ"></a>
### `ComponentScan`
```java
@Retention(RetentionPolicy.RUNTIME)
@Target(ElementType.TYPE)
@Documented
@Repeatable(ComponentScans.class)
public @interface ComponentScan {


    @AliasFor("basePackages")
    String[] value() default {};


    @AliasFor("value")
    String[] basePackages() default {};


    Class<?>[] basePackageClasses() default {};


    Class<? extends BeanNameGenerator> nameGenerator() default BeanNameGenerator.class;


    Class<? extends ScopeMetadataResolver> scopeResolver() default AnnotationScopeMetadataResolver.class;


    ScopedProxyMode scopedProxy() default ScopedProxyMode.DEFAULT;


    String resourcePattern() default ClassPathScanningCandidateComponentProvider.DEFAULT_RESOURCE_PATTERN;


    boolean useDefaultFilters() default true;


    Filter[] includeFilters() default {};


    Filter[] excludeFilters() default {};


    boolean lazyInit() default false;



    @Retention(RetentionPolicy.RUNTIME)
    @Target({})
    @interface Filter {


        FilterType type() default FilterType.ANNOTATION;


        @AliasFor("classes")
        Class<?>[] value() default {};

        @AliasFor("value")
        Class<?>[] classes() default {};


        String[] pattern() default {};

    }

}
```
用于定义 Spring 的扫描路径，等价于在 xml 文件中配置 `context:component-scan`，假如不配置扫描路径，那么 Spring 就会默认扫描当前类所在的包及其子包中的所有标注了 `@Component`，`@Service`，`@Controller` 等注解的类
<a name="AyQ11"></a>
### `EnableAutoConfiguration`
这个注解才是实现自动装配的关键，而这个注解，也是一个复合注解，<br />点进去之后发现，它是一个由 `@AutoConfigurationPackage` 和 `@Import` 注解组成的复合注解。<br />主要的就是这个 `import`，来看看他的源码<br />`@Import` 注解其实就是为了去导入一个类`AutoConfigurationImportSelector`，
```java
@Override
public String[] selectImports(AnnotationMetadata annotationMetadata) {
    //检查自动配置功能是否开启，默认开启
    if (!isEnabled(annotationMetadata)) {
        return NO_IMPORTS;
    }
    //加载自动配置的元信息
    AutoConfigurationMetadata autoConfigurationMetadata = AutoConfigurationMetadataLoader
    .loadMetadata(this.beanClassLoader);
    AnnotationAttributes attributes = getAttributes(annotationMetadata);
    //获取候选配置类
    List<String> configurations = getCandidateConfigurations(annotationMetadata,
                                                             attributes);
    //去掉重复的配置类
    configurations = removeDuplicates(configurations);
    //获得注解中被exclude和excludeName排除的类的集合
    Set<String> exclusions = getExclusions(annotationMetadata, attributes);
    //检查被排除类是否可实例化、是否被自动注册配置所使用，不符合条件则抛出异常
    checkExcludedClasses(configurations, exclusions);
    //从候选配置类中去除掉被排除的类
    configurations.removeAll(exclusions);
    //过滤
    configurations = filter(configurations, autoConfigurationMetadata);
    //将配置类和排除类通过事件传入到监听器中
    fireAutoConfigurationImportEvents(configurations, exclusions);
    //最终返回符合条件的自动配置类的全限定名数组
    return StringUtils.toStringArray(configurations);
}
```
已经在注释中给大家解释了他们每一步的意思<br />既然已经看完了这个 `import`了，接下来就得看看这个 `AutoConfigurationPackage`
<a name="YwipI"></a>
### `AutoConfigurationPackage`
`AutoConfigurationPackage`注解的作用是将添加该注解的类所在的package作为自动配置package进行管理。<br />可以通过 `AutoConfigurationPackages` 工具类获取自动配置package列表。<br />当通过注解`@SpringBootApplication`标注启动类时，已经为启动类添加了`@AutoConfigurationPackage`注解。<br />路径为 `@SpringBootApplication` -> `@EnableAutoConfiguration` -> `@AutoConfigurationPackage`。<br />也就是说当SpringBoot应用启动时默认会将启动类所在的package作为自动配置的package。
```java
@Target(ElementType.TYPE)
@Retention(RetentionPolicy.RUNTIME)
@Documented
@Inherited
@Import(AutoConfigurationPackages.Registrar.class)
public @interface AutoConfigurationPackage {

    String[] basePackages() default {};

    Class<?>[] basePackageClasses() default {};

}
```
看到源码中，他 `import` 了一个 `Registrar` ，
```java
static class Registrar implements ImportBeanDefinitionRegistrar, DeterminableImports {

    @Override
    public void registerBeanDefinitions(AnnotationMetadata metadata, BeanDefinitionRegistry registry) {
        register(registry, new PackageImports(metadata).getPackageNames().toArray(new String[0]));
    }

    @Override
    public Set<Object> determineImports(AnnotationMetadata metadata) {
        return Collections.singleton(new PackageImports(metadata));
    }

}
```
而这个类其实作用就是读取到在最外层的 `@SpringBootApplication` 注解中配置的扫描路径（没有配置则默认当前包下），然后把扫描路径下面的类都加到数组中返回。
