Java SpringBoot
<a name="XW3LX"></a>
### 前言
在平时的 API 开发过程中，总会遇到一些错误异常没有捕捉到的情况。那有的小伙伴可能会想，这还不简单么，在 API 最外层加一个 `try...catch` 不就完事了。<br />这种方法简单粗暴。在每一个 API 入口，都去做 `try...catch` 吗？这样不是代码看起来非常丑陋。所以使用 AOP 来实现是最佳的选择。<br />Spring Boot 通过注解来实现全局异常处理的。
<a name="mUm03"></a>
### `@ControllerAdvice` 和 `@ExceptionHandler`

- `@ControllerAdvice` 相当于 `controller` 的切面，主要用于 `@ExceptionHandler`，`@InitBinder` 和 `@ModelAttribute`，使注解标注的方法对每一个 controller 都起作用。默认对所有 controller 都起作用，当然也可以通过 `@ControllerAdvice` 注解中的一些属性选定符合条件的 controller 。
- `@ExceptionHandler` 用于异常处理的注解，可以通过 value 指定处理哪种类型的异常还可以与 `@ResponseStatus` 搭配使用，处理特定的 http 错误。标记的方法入参与返回值都有很大的灵活性，具体可以看注释也可以在后边的深度探究。
<a name="idvoP"></a>
### 案例分析
通过全局异常处理的方式了解 Spring Boot 的全局异常处理。
<a name="ElW8n"></a>
#### 案例一
一般的异常处理，所有的API都需要有相同的异常结构。<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1627730998715-911ec061-445a-4529-9156-80c95fc50614.png#averageHue=%23fdfdfd&clientId=u535c9a51-6d4b-4&from=paste&id=u4d74cb31&originHeight=564&originWidth=1024&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uafb1bda2-7f00-419d-aa95-62ad4c2e028&title=)<br />在这种情况下，实现是非常简单的，只需要创建 `GeneralExceptionHandler` 类，用 `@ControllerAdvice` 注解来注解它，并创建所需的 `@ExceptionHandler` ，它将处理所有由应用程序抛出的异常，如果它能找到匹配的 `@ExceptionHandler`，它将相应地进行转换。
```java
@ControllerAdvice
public class GeneralExceptionHandler {
    @ExceptionHandler(Exception.class)
    protected ResponseEntity<Error> handleException(Exception ex) {
       MyError myError = MyError.builder()
                         .text(ex.getMessage())
                         .code(ex.getErrorCode()).build();
       return new ResponseEntity(myError,
                               HttpStatus.valueOf(ex.getErrorCode()));
    }
}
```
<a name="NhIE4"></a>
#### 案例二
有一个API，它需要有一个或多个异常以其他格式处理，与其他应用程序的 API 不同。<br />![2021-07-31-19-35-50-840170.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1627731365391-24373ce4-be42-4e10-81cd-5f0d6b797490.png#averageHue=%23fdfdfd&clientId=u535c9a51-6d4b-4&from=ui&id=u9aed1102&originHeight=593&originWidth=1024&originalType=binary&ratio=1&rotation=0&showTitle=false&size=66602&status=done&style=shadow&taskId=u95b951c9-4797-4526-8eaf-a93007d41bb&title=)<br />可以采取两种方式来实现这种情况。可以在 `OtherController` 内部添加 `@ExceptionHandler` 来处理 `OtherException` ，或者为 `OtherController` 创建新的`@ControllerAdvice`，以备在其他 API 中处理 `OtherException`。<br />在 `OtherController` 中添加 `@ExceptionHandler` 来处理 `OtherException` 的代码示例。
```java
@RestController
@RequestMapping("/other")
public class OtherController {
    @ExceptionHandler(OtherException.class)
    protected ResponseEntity<Error> handleException(OtherException ex) {
      MyOtherError myOtherError = MyOtherError.builder()
                         .message(ex.getMessage())
                         .origin("Other API")
                         .code(ex.getErrorCode()).build();
      return new ResponseEntity(myOtherError,
                               HttpStatus.valueOf(ex.getErrorCode()));
    }
}
```
只针对 `OtherController` 控制器的 `@ControllerAdvice` 的代码示例
```java
@ControllerAdvice(assignableTypes = OtherController.class)
public class OtherExceptionHandler {
    @ExceptionHandler(OtherException.class)
    protected ResponseEntity<Error> handleException(OtherException ex) {
      MyOtherError myOtherError = MyOtherError.builder()
                         .message(ex.getMessage())
                         .origin("Other API")
                         .code(ex.getErrorCode()).build();
      return new ResponseEntity(myOtherError,
                               HttpStatus.valueOf(ex.getErrorCode()));
    }
}
```
<a name="h0Qxd"></a>
#### 案例三
与案例二类似，有一个 API 需要以不同于应用程序中其他 API 的方式对异常进行格式化，但这次所有的异常都需要进行不同的转换。<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1627730998708-2c5bd55b-b6fe-4682-8282-3fc0167264c7.png#averageHue=%23fdfdfd&clientId=u535c9a51-6d4b-4&from=paste&id=uec0bf428&originHeight=590&originWidth=1024&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u40a07416-1a9c-43cd-b3f4-1694deb8cb4&title=)<br />为了实现这个案例，将不得不使用两个 `@ControllerAdvice`，并加上 `@Order` 注解的注意事项。因为现在需要告诉 Spring，在处理同一个异常时，哪个 `@ControllerAdvice` 的优先级更高。如果没有指定 `@Order`，在启动时，其中一个处理程序将自动注册为更高的顺序，异常处理将变得不可预测。例如，如果使用 `mvn springboot:run` 任务启动一个应用程序，`OtherExceptionHandler` 是主要的，但是当以jar形式启动时，`GeneralExceptionHandler` 是主要的。
```java
@ControllerAdvice
public class GeneralExceptionHandler {
    @ExceptionHandler(Exception.class)
    protected ResponseEntity<Error> handleException(Exception ex) {
       MyError myError = MyError.builder()
                         .text(ex.getMessage())
                         .code(ex.getErrorCode()).build();
       return new ResponseEntity(myError,
                               HttpStatus.valueOf(ex.getErrorCode()));
    }
}
@ControllerAdvice(assignableTypes = OtherController.class)
@Order(Ordered.HIGHEST_PRECEDENCE)
public class OtherExceptionHandler {
    @ExceptionHandler(Exception.class)
    protected ResponseEntity<Error> handleException(Exception ex) {
       MyError myError = MyError.builder()
                         .message(ex.getMessage())
                         .origin("Other API")
                         .code(ex.getErrorCode()).build();
       return new ResponseEntity(myError,
                               HttpStatus.valueOf(ex.getErrorCode()));
    }
}
```
<a name="dczyt"></a>
### 总结
经过上述的几个案例，应该已经能够轻松应对 Spring Boot 中大部分的全局异常处理的情况。<br />为什么不使用 `@RestControllerAdvice` 呢？如果是用的 `@RestControllerAdvice` 注解，它会将数据自动转换成JSON格式，不再需要 `ResponseEntity` 的处理来。这种与 `Controller` 和 `RestController` 类似，本质是一样的，所以在使用全局异常处理之后可以进行灵活的选择处理。
