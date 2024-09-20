Java Optional
<a name="GxpTc"></a>
## 1、前言
相信不少同学已经被java的NPE(Null Pointer Exception)所谓的空指针异常搞的头昏脑涨, 有大佬说过“防止 NPE，是程序员的基本修养。”但是修养归修养，也是程序员最头疼的问题之一，利用Java8的新特性 `Optional`来尽量简化代码同时高效处理NPE（Null Pointer Exception 空指针异常）
<a name="Sq67k"></a>
## 2、认识`Optional`并使用
简单来说，`Opitonal`类就是Java提供的为了解决用 `null!=obj` 这样的方式存在的判断对象是否为空，从而令人头疼导致NPE（Null Pointer Exception 空指针异常），同时`Optional`的存在可以让代码更加简单，可读性跟高，代码写起来更高效.<br />**常规判断：**
```java
//对象 人
//属性有 name，age
Person person=new Person();
if (null==person){
    return "person为null";
}
return person;
```
**使用**`**Optional**`**：**
```java

//对象 人
//属性有 name，age
Person person=new Person();
return Optional.ofNullable(person).orElse("person为null");
```
Person类代码：
```java
public class Person {
    private String name;
    private Integer age;
    public Person(String name, Integer age) {
        this.name = name;
        this.age = age;
    }
    public Person() {
    }
    public String getName() {
        return name;
    }
    public void setName(String name) {
        this.name = name;
    }
    public Integer getAge() {
        return age;
    }
    public void setAge(Integer age) {
        this.age = age;
    }
}
```
<a name="buzZ0"></a>
### 2.1 `Optional`对象创建
创建`Optional`对象的方法
```java
public final class Optional<T> {
    private static final Optional<?> EMPTY = new Optional<>();
    private final T value;
    //可以看到两个构造方格都是private 私有的
    //说明 没办法在外面new出来Optional对象
    private Optional() {
        this.value = null;
    }
    private Optional(T value) {
        this.value = Objects.requireNonNull(value);
    }
    //这个静态方法大致 是创建出一个包装值为空的一个对象因为没有任何参数赋值
    public static<T> Optional<T> empty() {
        @SuppressWarnings("unchecked")
        Optional<T> t = (Optional<T>) EMPTY;
        return t;
    }
    //这个静态方法大致 是创建出一个包装值非空的一个对象 因为做了赋值
    public static <T> Optional<T> of(T value) {
        return new Optional<>(value);
    }
    //这个静态方法大致是 如果参数value为空，则创建空对象，如果不为空，则创建有参对象
    public static <T> Optional<T> ofNullable(T value) {
        return value == null ? empty() : of(value);
    }
}
```
一个简单的示例，与上面对应
```java
// 1、创建一个包装对象值为空的Optional对象
Optional<String> optEmpty = Optional.empty();
// 2、创建包装对象值非空的Optional对象
Optional<String> optOf = Optional.of("optional");
// 3、创建包装对象值允许为空也可以不为空的Optional对象
Optional<String> optOfNullable1 = Optional.ofNullable(null);
Optional<String> optOfNullable2 = Optional.ofNullable("optional");
```
<a name="h9w7g"></a>
### 2.2 `Optional.get()`方法(返回对象的值)
`get()`方法是返回一个`optional`的实例值 <br />源码：
```java
public T get() {
    if (value == null) {
        throw new NoSuchElementException("No value present");
    }
    return value;
}
```
如果value不为空则做返回，如果为空则抛出异常 "No value present" <br />简单实例展示
```java
Person person=new Person();
person.setAge(2);
Optional.ofNullable(person).get();
```
<a name="a22476e0"></a>
### 2.3 `Optional.isPresent()`方法(判断是否为空)
`isPresent()`方法就是会返回一个`boolean`类型值，如果对象不为空则为真，如果为空则`false `<br />源码:
```java
public boolean isPresent() {
    return value != null;
}
```
简单的实例展示:
```java
Person person=new Person();
person.setAge(2);
if (Optional.ofNullable(person).isPresent()){
    //写不为空的逻辑
    System.out.println("不为空");
}else{
    //写为空的逻辑
    System.out.println("为空");
}
```
<a name="blGjo"></a>
### 2.4 `Optional.ifPresent()`方法(判读是否为空并返回函数)
如果对象非空，则运行函数体 <br />源码:
```java
public void ifPresent(Consumer<? super T> consumer) {
    //如果value不为空，则运行accept方法体
    if (value != null)
        consumer.accept(value);
}
```
看实例：
```java
Person person=new Person();
person.setAge(2);
Optional.ofNullable(person).ifPresent(p -> System.out.println("年龄"+p.getAge()));
```
**如果对象不为空，则会打印这个年龄**，因为内部已经做了NPE（非空判断），所以就不用担心空指针异常了
<a name="7flWt"></a>
### 2.5 `Optional.filter()`方法(过滤对象)
`filter()`方法大致意思是，接受一个对象，然后对他进行条件过滤，如果条件符合则返回`Optional`对象本身，如果不符合则返回空`Optional`<br />源码：
```java
public Optional<T> filter(Predicate<? super T> predicate) {
    Objects.requireNonNull(predicate);
    //如果为空直接返回this
    if (!isPresent())
        return this;
    else
        //判断返回本身还是空Optional
        return predicate.test(value) ? this : empty();
}
```
简单实例：
```java
Person person=new Person();
person.setAge(2);
Optional.ofNullable(person).filter(p -> p.getAge()>50);
```
<a name="OhrHi"></a>
### 2.6 `Optional.map()`方法(对象进行二次包装)
`map()`方法将对应`Funcation`函数式接口中的对象，进行二次运算，封装成新的对象然后返回在`Optional`中 <br />源码：
```java
public<U> Optional<U> map(Function<? super T, ? extends U> mapper) {
    Objects.requireNonNull(mapper);
    //如果为空返回自己
    if (!isPresent())
        return empty();
    else {
        //否则返回用方法修饰过的Optional
        return Optional.ofNullable(mapper.apply(value));
    }
}
```
实例展示：
```java
Person person1=new Person();
person.setAge(2);
String optName = Optional.ofNullable(person).map(p -> person.getName()).orElse("name为空");
```
<a name="FIIry"></a>
### 2.7 `Optional.flatMap()`方法(`Optional`对象进行二次包装)
`map()`方法将对应`Optional< Funcation >`函数式接口中的对象，进行二次运算，封装成新的对象然后返回在`Optional`中 <br />源码:
```java
public<U> Optional<U> flatMap(Function<? super T, Optional<U>> mapper) {
    Objects.requireNonNull(mapper);
    if (!isPresent())
        return empty();
    else {
        return Objects.requireNonNull(mapper.apply(value));
    }
}
```
实例：
```java
Person person=new Person();
person.setAge(2);
Optional<Object> optName = Optional.ofNullable(person).map(p -> Optional.ofNullable(p.getName()).orElse("name为空"));
```
<a name="xui4f"></a>
### 2.8 `Optional.orElse()`方法(为空返回对象)
常用方法之一，这个方法是如果包装对象为空的话，就执行`orElse`方法里的`value`，如果非空，则返回写入对象 <br />源码:
```java
public T orElse(T other) {
    //如果非空，返回value，如果为空，返回other
    return value != null ? value : other;
}
```
实例：
```java
Person person1=new Person();
person.setAge(2);
Optional.ofNullable(person).orElse(new Person("小明", 2));
```
<a name="kscn4"></a>
### 2.9 `Optional.orElseGet()`方法(为空返回`Supplier`对象)
这个与`orElse`很相似，入参不一样，入参为`Supplier`对象，为空返回传入对象的`.get()`方法，如果非空则返回当前对象 <br />源码:
```java
public T orElseGet(Supplier<? extends T> other) {
    return value != null ? value : other.get();
}
```
实例：
```java
Optional<Supplier<Person>> sup=Optional.ofNullable(Person::new);
//调用get()方法，此时才会调用对象的构造方法，即获得到真正对象
Optional.ofNullable(person).orElseGet(sup.get());
```
`Suppiler`是一个接口，是类似Spring的懒加载，声明之后并不会占用内存，只有执行了`get()`方法之后，才会调用构造方法创建出对象 创建对象的语法的话就是`Supplier<Person> supPerson= Person::new;` 需要使用时`supPerson.get()`即可
<a name="9aZME"></a>
### 2.10 `Optional.orElseThrow()`方法(为空返回异常)
`Optional.orElseThrow()`方法作用的话就是如果为空，就抛出定义的异常，如果不为空返回当前对象，在实战中所有异常肯定是要处理好的，为了代码的可读性<br />源码：
```java
public <X extends Throwable> T orElseThrow(Supplier<? extends X> exceptionSupplier) throws X {
    if (value != null) {
        return value;
    } else {
        throw exceptionSupplier.get();
    }
}
```
实例：
```java
//简单的一个查询
Member member = memberService.selectByPhone(request.getPhone());
Optional.ofNullable(member).orElseThrow(() -> new ServiceException("没有查询的相关数据"));
```
<a name="6AXMK"></a>
### 2.11 相似方法进行对比分析
 `orElse()` 和 `orElseGet()` 还有 `orElseThrow()` 很相似，`map()` 和 `flatMap()` 很相似，总结一下不同方法的异同点 
<a name="dgjAk"></a>
####  `orElse()` 和 `orElseGet()` 和 `orElseThrow()` 的异同点
> 方法效果类似，如果对象不为空，则返回对象，如果为空，则返回方法体中的对应参数，所以可以看出这三个方法体中参数是不一样的 `orElse（T 对象）`、 `orElseGet（Supplier < T >对象）`、 `orElseThrow（异常）`

<a name="SIWgL"></a>
#### `map()`和`orElseGet`的异同点
> 方法效果类似，对方法参数进行二次包装，并返回,入参不同 `map（function函数）` 、`flatmap（Optional< function >函数）`

具体要怎么用，要根据业务场景以及代码规范来定义。
<a name="Cs0Ou"></a>
## 3、实战场景再现
场景1：在service层中 查询一个对象，返回之后判断是否为空并做处理
```java
//查询一个对象
Member member = memberService.selectByIdNo(request.getCertificateNo());
//使用ofNullable加orElseThrow做判断和操作
Optional.ofNullable(member).orElseThrow(() -> new ServiceException("没有查询的相关数据"));
```
场景2：可以在dao接口层中定义返回值时就加上`Optional` 例如：使用的是jpa，其他也同理
```java
public interface LocationRepository extends JpaRepository<Location, String> {
    Optional<Location> findLocationById(String id);
}
```
然在是Service中
```java
public TerminalVO findById(String id) {
    //这个方法在dao层也是用了Optional包装了
    Optional<Terminal> terminalOptional = terminalRepository.findById(id);
    //直接使用isPresent()判断是否为空
    if (terminalOptional.isPresent()) {
        //使用get()方法获取对象值
        Terminal terminal = terminalOptional.get();
        //在实战中，我们已经免去了用set去赋值的繁琐，直接用BeanCopy去赋值
        TerminalVO terminalVO = BeanCopyUtils.copyBean(terminal, TerminalVO.class);
        //调用dao层方法返回包装后的对象
        Optional<Location> location = locationRepository.findLocationById(terminal.getLocationId());
        if (location.isPresent()) {
            terminalVO.setFullName(location.get().getFullName());
        }
        return terminalVO;
    }
    //不要忘记抛出异常
    throw new ServiceException("该终端不存在");
}
```
**实战场景还有很多，包括**`**return**`**时可以判断是否返回当前值还是跳转到另一个方法体中。**
<a name="dQVa3"></a>
## 4、`Optional`使用注意事项
`**Optional**`**真么好用，真的可以完全替代if判断吗？** 这肯定是大家使用完之后`Optional`之后可能会产生的想法，**答案是否定的** <br />举一个最简单的栗子：例子1：如果只想判断对象的某一个变量是否为空并且做出判断呢？
```java
Person person=new Person();
person.setName("");
persion.setAge(2);
//普通判断
if(StringUtils.isNotBlank(person.getName())){
  //名称不为空执行代码块
}

//使用Optional做判断
Optional.ofNullable(person).map(p -> p.getName()).orElse("name为空");
```
这个例子就能很好的说明这个问题，**只是一个很简单判断，如果用了**`**Optional**`**还需要考虑包装值，考虑代码书写，考虑方法调用，虽然只有一行，但是可读性并不好，如果别的程序员去读，肯定没有**`**if**`**看的明显**
<a name="PwgBW"></a>
## 5、jdk1.9对`Optional`优化
首先增加了三个方法: `**or()**`**、**`**ifPresentOrElse()**`** 和 **`**stream()**`**。** `**or()**` 与`orElse()` 等方法相似，如果对象不为空返回对象，如果为空则返回`or()`方法中预设的值。`**ifPresentOrElse()**` 方法有两个参数：一个 Consumer 和一个 Runnable。如果对象不为空，会执行 `Consumer` 的动作，否则运行 Runnable。相比 `ifPresent()` 多了`OrElse`判断。`stream()` 将`Optional`转换成`stream`，如果有值就返回包含值的`stream`，如果没值，就返回空的`stream`。
