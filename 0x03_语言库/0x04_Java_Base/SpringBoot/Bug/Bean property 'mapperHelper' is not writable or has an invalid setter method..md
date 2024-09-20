<a name="z6SdM"></a>
# 1、报错内容
```bash
2020-08-10 10:11:15.818 ERROR 15704 --- [  restartedMain] o.s.boot.SpringApplication               : Application startup failed

org.springframework.beans.factory.BeanCreationException: Error creating bean with name 'doCustomerFieldService': Injection of resource dependencies failed; nested exception is org.springframework.beans.factory.UnsatisfiedDependencyException: Error creating bean with name 'customerFieldSetServiceImpl': Unsatisfied dependency expressed through field 'customerFieldSetMapper'; nested exception is org.springframework.beans.factory.BeanCreationException: Error creating bean with name 'customerFieldSetMapper' defined in file [D:\LinkSpace\Work\WorkSpace\Code\IDEA\wm-web\target\classes\com\wm\dao\CustomerFieldSetMapper.class]: Error setting property values; nested exception is org.springframework.beans.NotWritablePropertyException: Invalid property 'mapperHelper' of bean class [org.mybatis.spring.mapper.MapperFactoryBean]: Bean property 'mapperHelper' is not writable or has an invalid setter method. Does the parameter type of the setter match the return type of the getter?
	at org.springframework.context.annotation.CommonAnnotationBeanPostProcessor.postProcessPropertyValues(CommonAnnotationBeanPostProcessor.java:321)
	at org.springframework.beans.factory.support.AbstractAutowireCapableBeanFactory.populateBean(AbstractAutowireCapableBeanFactory.java:1264)
	at org.springframework.beans.factory.support.AbstractAutowireCapableBeanFactory.doCreateBean(AbstractAutowireCapableBeanFactory.java:553)
	at org.springframework.beans.factory.support.AbstractAutowireCapableBeanFactory.createBean(AbstractAutowireCapableBeanFactory.java:483)
	at org.springframework.beans.factory.support.AbstractBeanFactory$1.getObject(AbstractBeanFactory.java:306)
	at org.springframework.beans.factory.support.DefaultSingletonBeanRegistry.getSingleton(DefaultSingletonBeanRegistry.java:230)
	at org.springframework.beans.factory.support.AbstractBeanFactory.doGetBean(AbstractBeanFactory.java:302)
	at org.springframework.beans.factory.support.AbstractBeanFactory.getBean(AbstractBeanFactory.java:197)
	at org.springframework.beans.factory.support.DefaultListableBeanFactory.preInstantiateSingletons(DefaultListableBeanFactory.java:761)
	at org.springframework.context.support.AbstractApplicationContext.finishBeanFactoryInitialization(AbstractApplicationContext.java:867)
	at org.springframework.context.support.AbstractApplicationContext.refresh(AbstractApplicationContext.java:543)
	at org.springframework.boot.context.embedded.EmbeddedWebApplicationContext.refresh(EmbeddedWebApplicationContext.java:122)
	at org.springframework.boot.SpringApplication.refresh(SpringApplication.java:693)
	at org.springframework.boot.SpringApplication.refreshContext(SpringApplication.java:360)
	at org.springframework.boot.SpringApplication.run(SpringApplication.java:303)
	at org.springframework.boot.SpringApplication.run(SpringApplication.java:1118)
	at org.springframework.boot.SpringApplication.run(SpringApplication.java:1107)
	at com.wm.Application.main(Application.java:19)
	at sun.reflect.NativeMethodAccessorImpl.invoke0(Native Method)
	at sun.reflect.NativeMethodAccessorImpl.invoke(NativeMethodAccessorImpl.java:62)
	at sun.reflect.DelegatingMethodAccessorImpl.invoke(DelegatingMethodAccessorImpl.java:43)
	at java.lang.reflect.Method.invoke(Method.java:498)
	at org.springframework.boot.devtools.restart.RestartLauncher.run(RestartLauncher.java:49)
Caused by: org.springframework.beans.factory.UnsatisfiedDependencyException: Error creating bean with name 'customerFieldSetServiceImpl': Unsatisfied dependency expressed through field 'customerFieldSetMapper'; nested exception is org.springframework.beans.factory.BeanCreationException: Error creating bean with name 'customerFieldSetMapper' defined in file [D:\LinkSpace\Work\WorkSpace\Code\IDEA\wm-web\target\classes\com\wm\dao\CustomerFieldSetMapper.class]: Error setting property values; nested exception is org.springframework.beans.NotWritablePropertyException: Invalid property 'mapperHelper' of bean class [org.mybatis.spring.mapper.MapperFactoryBean]: Bean property 'mapperHelper' is not writable or has an invalid setter method. Does the parameter type of the setter match the return type of the getter?
	at org.springframework.beans.factory.annotation.AutowiredAnnotationBeanPostProcessor$AutowiredFieldElement.inject(AutowiredAnnotationBeanPostProcessor.java:588)
	at org.springframework.beans.factory.annotation.InjectionMetadata.inject(InjectionMetadata.java:88)
	at org.springframework.beans.factory.annotation.AutowiredAnnotationBeanPostProcessor.postProcessPropertyValues(AutowiredAnnotationBeanPostProcessor.java:366)
	at org.springframework.beans.factory.support.AbstractAutowireCapableBeanFactory.populateBean(AbstractAutowireCapableBeanFactory.java:1264)
	at org.springframework.beans.factory.support.AbstractAutowireCapableBeanFactory.doCreateBean(AbstractAutowireCapableBeanFactory.java:553)
	at org.springframework.beans.factory.support.AbstractAutowireCapableBeanFactory.createBean(AbstractAutowireCapableBeanFactory.java:483)
	at org.springframework.beans.factory.support.AbstractBeanFactory$1.getObject(AbstractBeanFactory.java:306)
	at org.springframework.beans.factory.support.DefaultSingletonBeanRegistry.getSingleton(DefaultSingletonBeanRegistry.java:230)
	at org.springframework.beans.factory.support.AbstractBeanFactory.doGetBean(AbstractBeanFactory.java:302)
	at org.springframework.beans.factory.support.AbstractBeanFactory.getBean(AbstractBeanFactory.java:202)
	at org.springframework.beans.factory.config.DependencyDescriptor.resolveCandidate(DependencyDescriptor.java:208)
	at org.springframework.beans.factory.support.DefaultListableBeanFactory.doResolveDependency(DefaultListableBeanFactory.java:1138)
	at org.springframework.beans.factory.support.DefaultListableBeanFactory.resolveDependency(DefaultListableBeanFactory.java:1066)
	at org.springframework.context.annotation.CommonAnnotationBeanPostProcessor.autowireResource(CommonAnnotationBeanPostProcessor.java:518)
	at org.springframework.context.annotation.CommonAnnotationBeanPostProcessor.getResource(CommonAnnotationBeanPostProcessor.java:496)
	at org.springframework.context.annotation.CommonAnnotationBeanPostProcessor$ResourceElement.getResourceToInject(CommonAnnotationBeanPostProcessor.java:627)
	at org.springframework.beans.factory.annotation.InjectionMetadata$InjectedElement.inject(InjectionMetadata.java:169)
	at org.springframework.beans.factory.annotation.InjectionMetadata.inject(InjectionMetadata.java:88)
	at org.springframework.context.annotation.CommonAnnotationBeanPostProcessor.postProcessPropertyValues(CommonAnnotationBeanPostProcessor.java:318)
	... 22 common frames omitted
Caused by: org.springframework.beans.factory.BeanCreationException: Error creating bean with name 'customerFieldSetMapper' defined in file [D:\LinkSpace\Work\WorkSpace\Code\IDEA\wm-web\target\classes\com\wm\dao\CustomerFieldSetMapper.class]: Error setting property values; nested exception is org.springframework.beans.NotWritablePropertyException: Invalid property 'mapperHelper' of bean class [org.mybatis.spring.mapper.MapperFactoryBean]: Bean property 'mapperHelper' is not writable or has an invalid setter method. Does the parameter type of the setter match the return type of the getter?
	at org.springframework.beans.factory.support.AbstractAutowireCapableBeanFactory.applyPropertyValues(AbstractAutowireCapableBeanFactory.java:1568)
	at org.springframework.beans.factory.support.AbstractAutowireCapableBeanFactory.populateBean(AbstractAutowireCapableBeanFactory.java:1276)
	at org.springframework.beans.factory.support.AbstractAutowireCapableBeanFactory.doCreateBean(AbstractAutowireCapableBeanFactory.java:553)
	at org.springframework.beans.factory.support.AbstractAutowireCapableBeanFactory.createBean(AbstractAutowireCapableBeanFactory.java:483)
	at org.springframework.beans.factory.support.AbstractBeanFactory$1.getObject(AbstractBeanFactory.java:306)
	at org.springframework.beans.factory.support.DefaultSingletonBeanRegistry.getSingleton(DefaultSingletonBeanRegistry.java:230)
	at org.springframework.beans.factory.support.AbstractBeanFactory.doGetBean(AbstractBeanFactory.java:302)
	at org.springframework.beans.factory.support.AbstractBeanFactory.getBean(AbstractBeanFactory.java:202)
	at org.springframework.beans.factory.config.DependencyDescriptor.resolveCandidate(DependencyDescriptor.java:208)
	at org.springframework.beans.factory.support.DefaultListableBeanFactory.doResolveDependency(DefaultListableBeanFactory.java:1138)
	at org.springframework.beans.factory.support.DefaultListableBeanFactory.resolveDependency(DefaultListableBeanFactory.java:1066)
	at org.springframework.beans.factory.annotation.AutowiredAnnotationBeanPostProcessor$AutowiredFieldElement.inject(AutowiredAnnotationBeanPostProcessor.java:585)
	... 40 common frames omitted
Caused by: org.springframework.beans.NotWritablePropertyException: Invalid property 'mapperHelper' of bean class [org.mybatis.spring.mapper.MapperFactoryBean]: Bean property 'mapperHelper' is not writable or has an invalid setter method. Does the parameter type of the setter match the return type of the getter?
	at org.springframework.beans.BeanWrapperImpl.createNotWritablePropertyException(BeanWrapperImpl.java:243)
	at org.springframework.beans.AbstractNestablePropertyAccessor.processLocalProperty(AbstractNestablePropertyAccessor.java:437)
	at org.springframework.beans.AbstractNestablePropertyAccessor.setPropertyValue(AbstractNestablePropertyAccessor.java:292)
	at org.springframework.beans.AbstractNestablePropertyAccessor.setPropertyValue(AbstractNestablePropertyAccessor.java:280)
	at org.springframework.beans.AbstractPropertyAccessor.setPropertyValues(AbstractPropertyAccessor.java:95)
	at org.springframework.beans.AbstractPropertyAccessor.setPropertyValues(AbstractPropertyAccessor.java:75)
	at org.springframework.beans.factory.support.AbstractAutowireCapableBeanFactory.applyPropertyValues(AbstractAutowireCapableBeanFactory.java:1564)
	... 51 common frames omitted
```
<a name="571WS"></a>
# 2、问题分析及解决办法
SpringBoot的热部署插件devtools不支持通用型Mapper，注释掉（或者不引用）相关依赖即可<br />reimport pom（重新导入依赖），再次运行即可
