Java
<a name="MfloR"></a>
## 一、前言
`@Builder` 注解的其中一个大坑会导致默认值失效！<br />`@Builder` 会让人误以为是遵循构建器模式，实则不然，后面会介绍。<br />总的来说，不推荐再使用 `@Builder` 注解，接下来讲重点介绍其原因和替代方案。
<a name="cDdDb"></a>
## 二、场景复现
<a name="BYeyj"></a>
### 2.1 如果不使用 `@Builder`
类定义：
```java
package io.gitrebase.demo;

import lombok.AllArgsConstructor;
import lombok.Builder;
import lombok.Data;
import lombok.NoArgsConstructor;


@Data
@NoArgsConstructor
@AllArgsConstructor
public class APIResponse<T> {

    private T payload;

    private Status status;

}
```
使用示例：
```java
package io.gitrebase.demo;

import lombok.extern.slf4j.Slf4j;
import org.springframework.http.HttpStatus;
import org.springframework.web.bind.annotation.ResponseStatus;
import org.springframework.web.bind.annotation.RestControllerAdvice;

@Slf4j
@RestControllerAdvice(assignableTypes = io.gitrebase.demo.RestApplication.class)
public class ApplicationExceptionHandler {

    @ResponseStatus(code = HttpStatus.INTERNAL_SERVER_ERROR)
    public APIResponse handleException(Exception exception) {
        log.error("Unhandled Exception", exception);
        Status status = new Status();
        status.setResponseCode("RESPONSE_CODE_IDENTIFIER");
        status.setDescription("Bla Bla Bla");
        APIResponse response = new APIResponse();
        response.setStatus(status);
        return response;
    }

}
```
<a name="aciNk"></a>
### 2.2 使用 `@Builder`
类定义：
```java
package io.gitrebase.demo;

import lombok.AllArgsConstructor;
import lombok.Builder;
import lombok.Data;
import lombok.NoArgsConstructor;

@Builder
@Data
@NoArgsConstructor
@AllArgsConstructor
public class APIResponse<T> {

    private T payload;

    private Status status;

}
```
使用示例：
```java
package io.gitrebase.demo;

import lombok.extern.slf4j.Slf4j;
import org.springframework.http.HttpStatus;
import org.springframework.web.bind.annotation.ResponseStatus;
import org.springframework.web.bind.annotation.RestControllerAdvice;

@Slf4j
@RestControllerAdvice(basePackageClasses = io.gitrebase.demo.RestApplication.class)
public class ApplicationExceptionHandler {

    @ResponseStatus(code = HttpStatus.INTERNAL_SERVER_ERROR)
    public APIResponse handleException(Exception exception) {
        log.error("Unhandled Exception", exception);
        return APIResponse.builder().status(Status.builder()
                .responseCode("RESPONSE_CODE_IDENTIFIER")
                .description("Bla Bla Bla")
                .build())
                .build();
    }

}
```
<a name="erESA"></a>
## 三、为什么不推荐使用 `@Builder`?
`@Builder` 会生成一个不完美的构建器，它不能区分哪些参数是必须的，哪些是可选的。这可能会导致构建对象时出现错误或不一致的情况。<br />很多人习惯于将 `@Builder` 和 `@Data` 一起使用使用会生成一个可变的构建器，它有 setter 方法可以修改构建器的状态。这违反了构建器模式的原则，即构建器应该是不可变的，一旦创建就不能修改。<br />`@Builder` 会生成一个具体类型的构建器，它不能适应不同类型的参数。这限制了构建器模式的优势，即可以根据不同的抽象类型创建不同风格的对象。<br />`@Builder` 的使用场景很有限，它只适合那些有很多参数且大部分是可选的对象。对于那些只想实现一个流式风格的对象创建，`@Builder` 并不是一个好的选择。
<a name="uoAfv"></a>
## 四、替代方案
<a name="d4vhj"></a>
### 4.1 首推：`@Accessor`
类的定义：
```java
package io.gitrebase.demo;

import lombok.Data;
import lombok.experimental.Accessors;


@Data
@Accessors(chain = true)
public class APIResponse<T> {

    private T payload;

    private Status status;

}
```
编译后的类：
```java
package io.gitrebase.demo;

import lombok.experimental.Accessors;

@Accessors(chain = true)
public class APIResponse<T> {

    private T payload;

    private Status status;

    public T getPayload() {
        return this.payload;
    }

    public APIResponse<T> setPayload(T payload) {
        this.payload = payload;
        return this;
    }

    public Status getStatus() {
        return this.status;
    }

    public APIResponse<T> setStatus(Status status) {
        this.status = status;
        return this;
    }
}
```
使用示例：
```java
package io.gitrebase.demo;

import lombok.extern.slf4j.Slf4j;
import lombok.var;
import org.springframework.http.HttpStatus;
import org.springframework.web.bind.annotation.ResponseStatus;
import org.springframework.web.bind.annotation.RestControllerAdvice;

@Slf4j
@RestControllerAdvice(basePackageClasses = io.gitrebase.demo.RestApplication.class)
public class ApplicationExceptionHandler {

    @ResponseStatus(code = HttpStatus.INTERNAL_SERVER_ERROR)
    public APIResponse handleException(Exception exception) {
        log.error("Unhandled Exception", exception);
        var status = new Status().setResponseCode("RESPONSE_CODE_IDENTIFIER").setDescription("Bla Bla Bla");
        return new APIResponse().setStatus(status);
    }

}
```
此外，该注解还支持一些高级方法：
```java
/**
 * A container for settings for the generation of getters and setters.
 * <p>
 * Complete documentation is found at <a href="https://projectlombok.org/features/experimental/Accessors">the project lombok features page for &#64;Accessors</a>.
 * <p>
 * Using this annotation does nothing by itself; an annotation that makes lombok generate getters and setters,
 * such as {@link lombok.Setter} or {@link lombok.Data} is also required.
 */
@Target({ElementType.TYPE, ElementType.FIELD})
@Retention(RetentionPolicy.SOURCE)
public @interface Accessors {
     /**
      * If true, accessors will be named after the field and not include a {@code get} or {@code set}
      * prefix. If true and {@code chain} is omitted, {@code chain} defaults to {@code true}.
      * <strong>default: false</strong>
      * 
      * @return Whether or not to make fluent methods (named {@code fieldName()}, not for example {@code setFieldName}).
      */
     boolean fluent() default false;
     
     /**
      * If true, setters return {@code this} instead of {@code void}.
      * <strong>default: false</strong>, unless {@code fluent=true}, then <strong>default: true</strong>
      * 
      * @return Whether or not setters should return themselves (chaining) or {@code void} (no chaining).
      */
     boolean chain() default false;
     
     /**
      * If present, only fields with any of the stated prefixes are given the getter/setter treatment.
      * Note that a prefix only counts if the next character is NOT a lowercase character or the last
      * letter of the prefix is not a letter (for instance an underscore). If multiple fields
      * all turn into the same name when the prefix is stripped, an error will be generated.
      * 
      * @return If you are in the habit of prefixing your fields (for example, you name them {@code fFieldName}, specify such prefixes here).
      */
     String[] prefix() default {};
}
```
另外如果一个类有些参数必传，有些参数选传，可以将必传参数定义到构造方法上，非必传参数采用 `@Accessor` 方式链式设置。
```java
// 导入 lombok 注解
import lombok.Data;
import lombok.experimental.Accessors;

// 定义 Person 类
@Getter // 自动生成 getter 方法
@Accessors(chain = true) // 开启链式调用
public class Person {
    // 定义必传的属性
    private String name; // 姓名
    private int id; // 编号

    // 定义选填的属性
    private int age; // 年龄
    private String address; // 地址

    // 定义构造函数，接收必传的参数
    public Person(String name, int id) {
        this.name = name;
        this.id = id;
    }
}

// 使用示例
public class Main {
    public static void main(String[] args) {
        // 创建一个 Person 对象，传入必要的参数,通过链式调用，设置选填的属性
        Person person = new Person("张三", 1001).setAge(25).setAddress("北京市");

        // 打印 Person 对象的信息
        System.out.println(person);
    }
}
```
<a name="ImJA3"></a>
### 4.2 手动模拟 `@Accessor`
由于 `@Accessor` 在 lombok.experimental包下，有极个非常谨慎的人会担心未来不稳定，未来可能被移除。<br />其实，这个担心有些多余，目前这个注解比 `@Builder` 更适合使用，而且一个成熟的工具类库不会轻易移除一个功能，而且及时移除了这个功能编译期就可以感知到，替换起来也很容易。<br />如果真的担心不稳定或者不想依赖 lombok，那么自己在默认生成的 Setter 方法上改造一下即可。
<a name="kavFu"></a>
## 五、启发
大多数同学使用 lombok 注解都不会主动看源码，了解有哪些高级配置。建议工作之余稍微花点时间去看一下源码。<br />大家在使用 lombok 注解时，一定要在脑海中能够准确“编译” 出背后的代码。如果没有这个能力，早晚会遇到坑。如果没有这个能力，那么多去看编译后的类，熟能生巧。<br />并不是大家都在用的都是对的，使用某些功能时需要主动思考是否正确，哪怕是正确的是否是最佳的。`@Builder` 注解的确和构建器设计模式有些背离，很多时候需要的是`@Accessor` 的行为。
<a name="te2fy"></a>
### <br /><br />
