Java
<a name="qOqsj"></a>
## 简介
Reflections通过扫描classpath，索引元数据，并且允许在运行时查询这些元数据。<br />使用Reflections可以很轻松的获取以下元数据信息：

- 获取某个类型的全部子类
- 只要类型、构造器、方法，字段上带有特定注解，便能获取带有这个注解的全部信息（类型、构造器、方法，字段）
- 获取所有能匹配某个正则表达式的资源
- 获取所有带有特定签名的方法，包括参数，参数注解，返回类型
- 获取所有方法的名字
- 获取代码里所有字段、方法名、构造器的使用
<a name="zRBvt"></a>
### Maven依赖
在pom.xml中添加reflections的依赖：
```xml
<dependency>
    <groupId>org.reflections</groupId>
    <artifactId>reflections</artifactId>
    <version>0.9.11</version>
</dependency>
```
```java
// 实例化Reflections，并指定要扫描的包名
Reflections reflections = new Reflections("my.project");
// 获取某个类的所有子类
Set<Class<? extends SomeType>> subTypes = reflections.getSubTypesOf(SomeType.class);
// 获取包含某个注解的所有类
Set<Class<?>> annotated = reflections.getTypesAnnotatedWith(SomeAnnotation.class);
```
<a name="lQf46"></a>
## 使用
```java
//scan urls that contain 'my.package', include inputs starting with 'my.package', use the default scanners
Reflections reflections = new Reflections("my.package");

//or using ConfigurationBuilder
new Reflections(new ConfigurationBuilder()
                .setUrls(ClasspathHelper.forPackage("my.project.prefix"))
                .setScanners(new SubTypesScanner(), 
                             new TypeAnnotationsScanner().filterResultsBy(optionalFilter), ...),
                .filterInputsBy(new FilterBuilder().includePackage("my.project.prefix"))
                ...);
```
<a name="pbgSm"></a>
### 扫描子类
```java
Set<Class<? extends Module>> modules = reflections.getSubTypesOf(com.google.inject.Module.class);
```
<a name="grQqE"></a>
### 扫描注解
```java
//TypeAnnotationsScanner 
Set<Class<?>> singletons = reflections.getTypesAnnotatedWith(javax.inject.Singleton.class);
```
<a name="ONenK"></a>
### 扫描资源
```java
//ResourcesScanner
Set<String> properties = reflections.getResources(Pattern.compile(".*\\.properties"));
```
<a name="DIaMo"></a>
### 扫描方法注解
```java
//MethodAnnotationsScanner
Set<Method> resources = reflections.getMethodsAnnotatedWith(javax.ws.rs.Path.class);
Set<Constructor> injectables = reflections.getConstructorsAnnotatedWith(javax.inject.Inject.class);
```
<a name="bG1Gc"></a>
### 扫描字段注解
```java
//FieldAnnotationsScanner
Set<Field> ids = reflections.getFieldsAnnotatedWith(javax.persistence.Id.class);
```
<a name="cdELy"></a>
### 扫描方法参数
```java
//MethodParameterScanner
Set<Method> someMethods = reflections.getMethodsMatchParams(long.class, int.class);
Set<Method> voidMethods = reflections.getMethodsReturn(void.class);
Set<Method> pathParamMethods = reflections.getMethodsWithAnyParamAnnotated(PathParam.class);
```
<a name="t6LeN"></a>
### 扫描方法参数名
```java
//MethodParameterNamesScanner
List<String> parameterNames = reflections.getMethodParamNames(Method.class)
```
<a name="CqGqJ"></a>
### 扫描方法调用情况
```java
//MemberUsageScanner
Set<Member> usages = reflections.getMethodUsages(Method.class)
```
<a name="nPWp9"></a>
## 参考资料
[https://github.com/ronmamo/reflections](https://github.com/ronmamo/reflections)
