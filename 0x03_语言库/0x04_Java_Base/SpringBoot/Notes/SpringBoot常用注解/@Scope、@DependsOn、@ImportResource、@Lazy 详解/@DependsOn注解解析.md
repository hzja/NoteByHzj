JavaSpringBoot
<a name="uPk3a"></a>
## 作用
`@DependsOn`注解可以定义在类和方法上，意思是这个组件要依赖于另一个组件，也就是说被依赖的组件会比该组件先注册到IOC容器中。
<a name="WRlxp"></a>
## 在哪里被解析
解析的地方在 `ComponentScanAnnotationParser类#doScan`方法 【可查看Spring基于注解的扫描底层流程】 代码为  `AnnotationConfigUtils.processCommonDefinitionAnnotations((AnnotatedBeanDefinition) candidate);` 将元数据的信息塞入到 `BeanDefinition`里面
<a name="VTuKS"></a>
## 在哪里别调用
调用的地方在  `AbstractBeanFactory`类`#doGetBean`方法里面  ，代码展示
```java
//根据获取BeanDefinition信息
final RootBeanDefinition mbd = getMergedLocalBeanDefinition(beanName);
//校验一下正确性
checkMergedBeanDefinition(mbd, beanName, args);

// 拿出DependsOn注解的信息
String[] dependsOn = mbd.getDependsOn();
if (dependsOn != null) {
    for (String dep : dependsOn) {
        // 检测beanName是不是被dep依赖了，如果是则出现循环依赖
        if (isDependent(beanName, dep)) {
            throw new BeanCreationException(mbd.getResourceDescription(), beanName,
                    "Circular depends-on relationship between '" + beanName + "' and '" + dep + "'");
        }
        // dep被beanName依赖了，存入dependentBeanMap中，dep为key，beanName为value
        registerDependentBean(dep, beanName);
        try {
            getBean(dep);
        }
        catch (NoSuchBeanDefinitionException ex) {
            throw new BeanCreationException(mbd.getResourceDescription(), beanName,
                    "'" + beanName + "' depends on missing bean '" + dep + "'", ex);
        }
    }
}
```
<a name="Pl5y4"></a>
## 汇总说明
这种本质上也是循环依赖问题产生的一种，而且Spring没有办法解决，唯有靠写代码的时候进行避免。也就是说 `dependentBeanMap` 里面存储了一样的依赖的时候就会抛出错误。
