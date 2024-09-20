Java SpringBoot 参数校验

1. 分组
2. 注解地方
3. 嵌套验证

---

Spring Validation验证框架对参数的验证机制提供了`@Validated`（Spring's JSR-303 规范，是标准 JSR-303 的一个变种），javax提供了`@Valid`（标准JSR-303规范），配合 BindingResult 可以直接提供参数验证结果。<br />在检验 Controller 的入参是否符合规范时，使用 `@Validated` 或者 `@Valid` 在基本验证功能上没有太多区别。但是在分组、注解地方、嵌套验证等功能上两个有所不同：
<a name="Mdh5Y"></a>
## 1、分组
`@Validated`：提供了一个分组功能，可以在入参验证时，根据不同的分组采用不同的验证机制。<br />`@Valid`：作为标准JSR-303规范，还没有分组的功能。
<a name="e18fac86"></a>
## 2、注解地方
`@Validated`：可以用在类型、方法和方法参数上。但是不能用在成员属性（字段）上<br />`@Valid`：可以用在方法、构造函数、方法参数和成员属性（字段）上<br />两者是否能用于成员属性（字段）上直接影响能否提供嵌套验证的功能。
<a name="ef94037b"></a>
## 3、嵌套验证
在比较两者嵌套验证时，比如现在有个实体叫做Item：
```java
public class Item {
    @NotNull(message = "id不能为空")
    @Min(value = 1, message = "id必须为正整数")
    private Long id;
    @NotNull(message = "props不能为空")
    @Size(min = 1, message = "至少要有一个属性")
    private List<Prop> props;
}
```
Item带有很多属性，属性里面有属性id，属性值id，属性名和属性值，如下所示：
```java
public class Prop {
    @NotNull(message = "pid不能为空")
    @Min(value = 1, message = "pid必须为正整数")
    private Long pid;
    @NotNull(message = "vid不能为空")
    @Min(value = 1, message = "vid必须为正整数")
    private Long vid;
    @NotBlank(message = "pidName不能为空")
    private String pidName;
    @NotBlank(message = "vidName不能为空")
    private String vidName;
}
```
属性这个实体也有自己的验证机制，比如属性和属性值id不能为空，属性名和属性值不能为空等。<br />现在有个 ItemController 接受一个Item的入参，对Item进行验证，如下所示：
```java
@RestController
public class ItemController {
    @RequestMapping("/item/add")
    public void addItem(@Validated Item item, BindingResult bindingResult) {
        doSomething();
    }
}
```
在上图中，如果Item实体的props属性不额外加注释，只有`@NotNull`和`@Size`，无论入参采用`@Validated`还是`@Valid`验证，Spring Validation框架只会对Item的id和props做非空和数量验证，不会对`props`字段里的Prop实体进行字段验证，也就是`@Validated`和`@Valid`加在方法参数前，都不会自动对参数进行嵌套验证。也就是说如果传的List中有Prop的pid为空或者是负数，入参验证不会检测出来。<br />为了能够进行嵌套验证，必须手动在Item实体的props字段上明确指出这个字段里面的实体也要进行验证。由于`@Validated`不能用在成员属性（字段）上，但是`@Valid`能加在成员属性（字段）上，而且`@Valid`类注解上也说明了它支持嵌套验证功能，那么能够推断出：`@Valid`加在方法参数时并不能够自动进行嵌套验证，而是用在需要嵌套验证类的相应字段上，来配合方法参数上`@Validated`或`@Valid`来进行嵌套验证。<br />修改Item类如下所示：
```java
public class Item {
    @NotNull(message = "id不能为空")
    @Min(value = 1, message = "id必须为正整数")
    private Long id;
    @Valid // 嵌套验证必须用@Valid
    @NotNull(message = "props不能为空")
    @Size(min = 1, message = "props至少要有一个自定义属性")
    private List<Prop> props;
}
```
然后在ItemController的`addItem`函数上再使用`@Validated`或者`@Valid`，就能对Item的入参进行嵌套验证。此时Item里面的`props`如果含有Prop的相应字段为空的情况，Spring Validation框架就会检测出来，`bindingResult`就会记录相应的错误。
<a name="17a02ba8"></a>
## 总结：`@Validated` 和 `@Valid` 在嵌套验证功能上的区别
`**@Validated**`**：** 用在方法入参上无法单独提供嵌套验证功能。不能用在成员属性（字段）上，也无法提示框架进行嵌套验证。能配合嵌套验证注解`@Valid`进行嵌套验证。<br />`**@Valid**`**：** 用在方法入参上无法单独提供嵌套验证功能。能够用在成员属性（字段）上，提示验证框架进行嵌套验证。能配合嵌套验证注解`@Validated`进行嵌套验证。
