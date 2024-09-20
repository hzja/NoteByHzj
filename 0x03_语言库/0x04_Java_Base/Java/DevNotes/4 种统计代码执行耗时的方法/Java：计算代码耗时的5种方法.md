Java
<a name="h4ugc"></a>
### 1、使用`System.currentTimeMillis()`函数
代码如下：
```java
long start = System.currentTimeMillis();
// some code
long finish = System.currentTimeMillis();
long timeElapsed = finish - start;
```
<a name="ZAQxH"></a>
### 2、使用`System.nanoTime()`函数
代码如下：
```java
long start = System.nanoTime();
// some code
long finish = System.nanoTime();
long timeElapsed = finish - start;
```
<a name="vgPUC"></a>
### 3、在java8中使用`Instant.now()`函数
代码如下：
```java
Instant start = Instant.now();
// some code       
Instant finish = Instant.now();
long timeElapsed = Duration.between(start, finish).toMillis();
```
<a name="yncsv"></a>
### 4、使用apache.commons提供的`StopWatch`
首先，在pom.xml中添加如下依赖：
```xml
<dependency>
	<groupId>org.apache.commons</groupId>
	<artifactId>commons-lang3</artifactId>
	<version>3.7</version>
</dependency>
```
<a name="KXY2H"></a>
### 5、使用Spring 框架提供的`StopWatch`
代码如下：
```java
import org.springframework.util.StopWatch;

StopWatch watch = new StopWatch();
watch.start("watcher");

//some code

watch.stop();
System.out.println(watch.prettyPrint());
```
