JavaSpringBoot<br />对于 `@Component` 注解在日常的工作中相信很多小伙伴都会使用到，作为一种 Spring 容器托管的通用模式组件，任何被 `@Component` 注解标注的组件都会被 Spring 容器扫描。<br />那么有的小伙伴就要问了，很多时候并没有直接写 `@Component` 注解呀，写的是类似于 `@Service`，`@RestController`，`@Configuration` 等注解，不也是一样可以被扫描到吗？那这个 `@Component` 有什么特别的吗？
<a name="j8uGk"></a>
## 元注解
在回答上面的问题之前，先来了解一下什么叫元注解，所谓元注解就是指一个能声明在其他注解上的注解，换句话说就是如果一个注解被标注在其他注解上，那么它就是元注解。<br />要说明的是这个元注解并不是 Spring 领域的东西， 而是 Java 领域的，像 Java 中的很多注解比如 `@Document`，`@Repeatable` ，`@Target` 等都属于元注解。<br />根据上面的解释可以发现在 Spring 容器里 `@Component`  就是以元注解的形式存在，因为可以在很多其他注解里面找到它的身影，如下所示<br />![Configuration](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1671585304564-93339978-6ef4-4030-a33c-7846cc784849.jpeg#averageHue=%233f3e3c&clientId=u55d131ca-efbd-4&from=paste&id=u74b31edd&originHeight=755&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=uc0cd15ad-23fc-4197-9a9e-ee6a7b388d8&title=Configuration "Configuration")<br />![controller](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1671585304477-630928fe-34cf-491c-bcb4-c5702b80448e.jpeg#averageHue=%233e3722&clientId=u55d131ca-efbd-4&from=paste&id=u8faa4be7&originHeight=760&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=u681959b6-24da-4744-bdcc-a087a9ede1a&title=controller "controller")
<a name="S9qE0"></a>
## `@Component` 的派生性
通过上面的内容是不是可以猜测一下那就是 `@Component` 注解的特性被"继承"下来了？这就可以解释为什么可以直接写`@Service`，`@RestController` 注解也是可以被扫描到的。但是由于 Java 的注解是不支持继承的，比如想通过下面的方式来实现注解的继承是不合法的。<br />![@interface](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1671585304466-dec0eb61-1fa4-4195-a417-0bf0a39c5c8b.jpeg#averageHue=%233f3927&clientId=u55d131ca-efbd-4&from=paste&id=ub40dbde6&originHeight=496&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=u3b13ed1f-ef86-4a25-ac1e-2f3ff74b959&title=%40interface "@interface")<br />为了验证猜想，可以通过跟踪源代码来验证一下，目的是研究为什么不直接使用 `@Component` 注解也能被 Spring 扫描到，换句话说就是使用 `@Service` 和 `@RestController` 的注解也能成为 Spring Bean。<br />那很自然的就可以想到，在扫描的时候一定是根据注解来进行了判断是否要初始化成 Spring Bean 的。只要找到了判断条件就可以解决疑惑了。<br />由于 SpringBoot 项目是通过 main 方法进行启动的，调试起来还是很方便的，这边准备了一个简单的 SpringBoot 工程，里面除了启动类之外只有一个DemoController.java 代码如下
```java
package com.example.demojar.controller;

import org.springframework.web.bind.annotation.RestController;

@RestController
public class DemoController {
}
```
启动类如下
```java
package com.example.demojar;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.WebApplicationType;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.boot.builder.SpringApplicationBuilder;
import org.springframework.context.ConfigurableApplicationContext;

@SpringBootApplication(scanBasePackages = {"com.example.demojar"})
public class DemoJarApplication {

    public static void main(String[] args) {
        SpringApplication.run(DemoJarApplication.class, args);
    }
}
```
Debug run 方法，可以定位到 `org.springframework.boot.SpringApplication#run(java.lang.String...)` 方法，该方法里面会初始化 SpringBoot 上下文 context。
```java
context = createApplicationContext();
```
默认情况下会进到下面的方法，并创建 `AnnotationConfigServletWebServerApplicationContext` 并且其构造函数中构造了 `ClassPathBeanDefinitionScanner` 类路径 Bean 扫描器。此处已经越来越接近扫描相关的内容了。<br />`org.springframework.boot.web.servlet.context.AnnotationConfigServletWebServerApplicationContext.Factory#create`<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1671585304469-b32b1784-15f5-4ffe-b3d4-342d754c5b91.jpeg#averageHue=%23454943&clientId=u55d131ca-efbd-4&from=paste&id=ud318c8d8&originHeight=139&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uf1b609c8-e208-4b84-b624-da82da4ce41&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1671585304454-2829a3cd-09e1-41c3-8ade-7cf6211b221a.jpeg#averageHue=%23353124&clientId=u55d131ca-efbd-4&from=paste&id=uf33d7d72&originHeight=137&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uc303a919-f0a7-4965-9d93-a8cbbbd5ba4&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1671585304786-d838b822-a128-49d9-bd7d-bec9759d5482.jpeg#averageHue=%23443726&clientId=u55d131ca-efbd-4&from=paste&id=u18e56dd4&originHeight=222&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u2b9bdaad-a26b-4fff-b574-d272d9360b6&title=)<br />context 上下文创建完成过后，接下来会接入到 `org.springframework.context.support.AbstractApplicationContext#refresh`，再到 `org.springframework.context.support.AbstractApplicationContext#invokeBeanFactoryPostProcessors`<br />`org.springframework.context.annotation.ConfigurationClassPostProcessor#postProcessBeanDefinitionRegistry`<br />`org.springframework.context.annotation.ConfigurationClassPostProcessor#processConfigBeanDefinitions`<br />经过上面的步骤，最终可以可以定位到扫描的代码在下面的方法 `org.springframework.context.annotation.ComponentScanAnnotationParser#parse` 里面，调用前面上下文初始化的扫描器的 `org.springframework.context.annotation.ClassPathBeanDefinitionScanner#doScan` 方法，<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1671585304837-3b1ef5b9-16a2-4eb5-8de4-e7803875f87a.jpeg#averageHue=%2336342e&clientId=u55d131ca-efbd-4&from=paste&id=u25ac51f2&originHeight=336&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u9e5579ad-0c6d-4513-9eb8-ce22a405a06&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1671585304857-adec7be6-5dfd-4716-9773-e171707b4071.jpeg#averageHue=%232b2f27&clientId=u55d131ca-efbd-4&from=paste&id=ue8f2136d&originHeight=570&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u82a0747f-e26e-47ad-8ac1-5a245c5b162&title=)<br />到这里已经定位到了扫描具体包路径的方法，这个方法里面主要看 `findCandidateComponents(basePackage);` 方法的内容，这个方法就是返回合法的候选组件。说明这个方法会最终返回需要被注册成 Spring Bean 的候选组件，那重点就要看这个方法的实现。<br />跟踪这个方法 `org.springframework.context.annotation.ClassPathScanningCandidateComponentProvider#findCandidateComponents` 进去可以看到通过加进类路径里面的资源文件，然后再根据资源文件生成 `MetadataReader` 对象，最后判断这个 `MetadataReader` 对象是否满足候选组件的条件，如果满足就添加到 Set 集合中进行返回。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1671585304908-43d4016a-a6b6-4929-b8a5-fb7c9fb76979.jpeg#averageHue=%232a2821&clientId=u55d131ca-efbd-4&from=paste&id=u96aa113d&originHeight=611&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u58902374-4356-49c9-8383-49f859d7e1a&title=)<br />继续追踪源码可以找到具体的判断方法在 `org.springframework.core.type.filter.AnnotationTypeFilter#matchSelf` 方法中，如下所示，可以看到这里对 `MetadataReader` 对象进行了判断是否有元注解 `@Component`。在调试的时候会发现 DemoController 在此处会返回 true，并且该 `MetadataReader` 对象里面还有多个 mappings ，其实这些 mappings 对应的就是 Spring 的注解。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1671585304960-be9a7d4d-f9eb-4ce3-9305-265b79cc4a05.jpeg#averageHue=%23333927&clientId=u55d131ca-efbd-4&from=paste&id=u485715f2&originHeight=179&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u24cfd642-1053-4162-ba6f-01a206de230&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1671585305173-52767559-3840-48d6-b66b-f995677171fe.jpeg#averageHue=%233d403e&clientId=u55d131ca-efbd-4&from=paste&id=u40afc2c3&originHeight=498&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uc5eeee06-4748-49ae-ae40-e8300a3a118&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1671585305233-1156d449-88df-4bd8-b247-0d92bde6ac3e.jpeg#averageHue=%233e403f&clientId=u55d131ca-efbd-4&from=paste&id=u4240ccdd&originHeight=476&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ueda28563-0b16-4be4-8bdc-5f179a4fb1a&title=)<br />这个 mappings 里面的注解确实包含了 `@Component` 注解，因此会返回 true。那么接下来问题就转换成，`DemoController` 对应的 `MetadataReader` 对象是如何创建的。<br />回到 `org.springframework.context.annotation.ClassPathScanningCandidateComponentProvider#scanCandidateComponents` 方法，看看具体 `MetadataReader` 对象是如何创建的，`MetadataReader metadataReader = getMetadataReaderFactory().getMetadataReader(resource);`<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1671585305240-a2af3555-e190-4e34-9008-2d13a68aff8d.jpeg#averageHue=%2344473d&clientId=u55d131ca-efbd-4&from=paste&id=u2b111d7d&originHeight=223&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u6a6b07ee-1a85-49b5-9543-98df0875f78&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1671585305306-4fe0c2a5-154b-4367-9355-b031bd2c9dd8.jpeg#averageHue=%23353124&clientId=u55d131ca-efbd-4&from=paste&id=u1344dfc5&originHeight=184&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u6f7f82be-adce-4953-aec9-bafad78ab1a&title=)<br />通过构造方法 `org.springframework.core.type.classreading.SimpleMetadataReader#SimpleMetadataReader` 进行 `MetadataReader`  对象的创建，`org.springframework.core.type.classreading.SimpleAnnotationMetadataReadingVisitor#visitEnd`，最终定位到 `org.springframework.core.annotation.MergedAnnotationsCollection#MergedAnnotationsCollection` 这里进行 mappings 赋值。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1671585305340-f915e6f6-4d22-42c6-af0a-561e8a3fc315.jpeg#averageHue=%23373a37&clientId=u55d131ca-efbd-4&from=paste&id=uf10bd21f&originHeight=429&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u9ed28447-ecc0-4bb0-8f84-6fe18334ee4&title=)<br />继续定位到 `org.springframework.core.annotation.AnnotationTypeMappings.Cache#createMappings`，`org.springframework.core.annotation.AnnotationTypeMappings#addAllMappings`，`addAllmappings` 方法，内部使用了一个 while 循环和 Deque 来循环查询元注解进行赋值，代码如下所示，重点是这一行 `Annotation[] metaAnnotations = AnnotationsScanner.getDeclaredAnnotations(source.getAnnotationType(), false);`
```java
private void addAllMappings(Class<? extends Annotation> annotationType,
                            Set<Class<? extends Annotation>> visitedAnnotationTypes) {

    Deque<AnnotationTypeMapping> queue = new ArrayDeque<>();
    addIfPossible(queue, null, annotationType, null, visitedAnnotationTypes);
    while (!queue.isEmpty()) {
        AnnotationTypeMapping mapping = queue.removeFirst();
        this.mappings.add(mapping);
        addMetaAnnotationsToQueue(queue, mapping);
    }
}

private void addMetaAnnotationsToQueue(Deque<AnnotationTypeMapping> queue, AnnotationTypeMapping source) {
    Annotation[] metaAnnotations = AnnotationsScanner.getDeclaredAnnotations(source.getAnnotationType(), false);
    for (Annotation metaAnnotation : metaAnnotations) {
        if (!isMappable(source, metaAnnotation)) {
            continue;
        }
        Annotation[] repeatedAnnotations = this.repeatableContainers.findRepeatedAnnotations(metaAnnotation);
        if (repeatedAnnotations != null) {
            for (Annotation repeatedAnnotation : repeatedAnnotations) {
                if (!isMappable(source, repeatedAnnotation)) {
                    continue;
                }
                addIfPossible(queue, source, repeatedAnnotation);
            }
        }
        else {
            addIfPossible(queue, source, metaAnnotation);
        }
    }
}
```
综上所述可以发现尽管没有直接写 `@Component` 注解，只要加了类似于 `@Service`，`@RestController` 等注解也是可以成功被 Spring 扫描到注册成 Spring Bean 的，本质的原因是因为这些注解底层都使用了 `@Component` 作为元注解，经过源码分析可以发现只要有 `@Component` 元注解标注的注解类也是同样会被进行扫描的。
<a name="NcBEN"></a>
## 总结
上面的源码追踪过程可能会比较枯燥和繁琐，最后来简单总结一下上面的内容：

1. 方法 `org.springframework.boot.SpringApplication#run(java.lang.String...)` 中进行 Spring 上下文的创建；
2. 在初始化上下文的时候会创建扫描器 `ClassPathBeanDefinitionScanner`；
3. 在 `org.springframework.context.support.AbstractApplicationContext#refresh` 进行 beanFactory 准备；
4. `org.springframework.context.annotation.ClassPathBeanDefinitionScanner#doScan` 进行资源扫描
5. 在 `org.springframework.core.annotation.MergedAnnotationsCollection#MergedAnnotationsCollection` 进行注解 mappings 的赋值；
6. `org.springframework.context.annotation.ClassPathScanningCandidateComponentProvider#scanCandidateComponents` 方法中进行候选组件的判断；

上面追踪的过程可能会比较复杂，但是只要理解了原理还是可以慢慢跟上的，因为只要把握好了方向，知道首先肯定会进行资源扫描，扫描完了肯定是根据注解之间的关系进行判断，最终得到需要的候选组件集合。至于如何创建 MetadataReader 和如何获取元注解，只要一步步看下去就是可以找到的。<br />最后说明一下，Spring Framework 每个版本的具体实现会有差异，这里使用的版本是 5.3.24 ，所以如果小伙伴看到自己的代码追踪的效果跟这里的不一样也不会奇怪，可能是因为版本不一样而已，不过本质上都是一样的。
