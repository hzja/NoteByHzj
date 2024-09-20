Java Lombok <br />Builder 使用创建者模式又叫建造者模式。简单来说，就是一步步创建一个对象，它对用户屏蔽了里面构建的细节，但却可以精细地控制对象的构造过程。
<a name="BAijo"></a>
### 基础使用
`@Builder`注释为类生成相对略微复杂的构建器API。`@Builder`可以以下面显示的那样调用代码，来初始化实例对象：
```java
Student.builder()
    .sno( "001" )
    .sname( "admin" )
    .sage( 18 )
    .sphone( "110" )
    .build();
```
`@Builder`可以放在类，构造函数或方法上。虽然放在类上和放在构造函数上这两种模式是最常见的用例，但`@Builder`最容易用放在方法的用例来解释。
<a name="rWSyF"></a>
### `@Builder`内部做了什么

1. 创建一个名为`ThisClassBuilder`的内部静态类，并具有和实体类形同的属性（称为构建器）。
2. 在构建器中：对于目标类中的所有的属性和未初始化的`final`字段，都会在构建器中创建对应属性。
3. 在构建器中：创建一个无参的`default`构造函数。
4. 在构建器中：对于实体类中的每个参数，都会对应创建类似于`setter`的方法，只不过方法名与该参数名相同。并且返回值是构建器本身（便于链式调用），如上例所示。
5. 在构建器中：一个`build()`方法，调用此方法，就会根据设置的值进行创建实体对象。
6. 在构建器中：同时也会生成一个`toString()`方法。
7. 在实体类中：会创建一个`builder()`方法，它的目的是用来创建构建器。

通过下面这个例子来理解
```java
@Builder
public class User {
    private final Integer code = 200;
    private String username;
    private String password;
}
 
// 编译后：
public class User {
    private String username;
    private String password;
    User(String username, String password) {
        this.username = username; this.password = password;
    }
    public static User.UserBuilder builder() {
        return new User.UserBuilder();
    }
 
    public static class UserBuilder {
        private String username;
        private String password;
        UserBuilder() {}
 
        public User.UserBuilder username(String username) {
            this.username = username;
            return this;
        }
        public User.UserBuilder password(String password) {
            this.password = password;
            return this;
        }
        public User build() {
            return new User(this.username, this.password);
        }
        public String toString() {
            return "User.UserBuilder(username=" + this.username + ", password=" + this.password + ")";
        }
    }
}
```
<a name="TmP1u"></a>
### 组合用法
<a name="VPNWD"></a>
#### `@Builder`中使用 `@Singular` 注释集合
`@Builder`也可以为集合类型的参数或字段生成一种特殊的方法。它采用修改列表中一个元素而不是整个列表的方式，可以是增加一个元素，也可以是删除一个元素。
```java
Student.builder()
    .sno("001")
    .sname("admin")
    .sage(18)
    .sphone("110").sphone("112")
    .build();
```
这样就可以轻松地将`List<String>`字段中包含 2 个字符串。但是想要这样来操作集合，需要使用`@Singular`来注释字段或参数。<br />在使用`@Singular`注释注释一个集合字段（使用`@Builder`注释类），lombok会将该构建器节点视为一个集合，并生成两个`adder`方法而不是`setter`方法。

- 一个向集合添加单个元素
- 一个将另一个集合的所有元素添加到集合中

将不生成仅设置集合（替换已添加的任何内容）的 setter。还生成了 clear 方法。这些 singular 构建器相对而言是有些复杂的，主要是来保证以下特性：

1. 在调用`build()`时，生成的集合将是不可变的。
2. 在调用`build()`之后调用其中一个`adder`方法或`clear`方法不会修改任何已经生成的对象。如果对集合修改之后，再调用`build()`，则会创建一个基于上一个对象创建的对象实体。
3. 生成的集合将被压缩到最小的可行格式，同时保持高效。

`@Singular`只能应用于lombok已知的集合类型。目前，支持的类型有：<br />java.util:

- `Iterable`, `Collection`, 和`List`(一般情况下，由压缩的不可修改的`ArrayList`支持).
- `Set`, `SortedSet`, and `NavigableSet `(一般情况下，生成可变大小不可修改的`HashSet`或者`TreeSet`).
- `Map`, `SortedMap`, and `NavigableMap `(一般情况下，生成可变大小不可修改的`HashMap`或者`TreeMap`).

Guava’s com.google.common.collect:

- `ImmutableCollection` and `ImmutableList`
- `ImmutableSet` and `ImmutableSortedSet`
- `ImmutableMap`, `ImmutableBiMap`, and `ImmutableSortedMap`
- `ImmutableTable`

来看看使用了`@Singular`注解之后的编译情况：
```java
@Builder
public class User {
    private final Integer id;
    private final String zipCode = "123456";
    private String username;
    private String password;
    @Singular
    private List<String> hobbies;
}
 
// 编译后：
public class User {
    private final Integer id;
    private final String zipCode = "123456";
    private String username;
    private String password;
    private List<String> hobbies;
    User(Integer id, String username, String password, List<String> hobbies) {
        this.id = id; this.username = username;
        this.password = password; this.hobbies = hobbies;
    }
 
    public static User.UserBuilder builder() {return new User.UserBuilder();}
 
    public static class UserBuilder {
        private Integer id;
        private String username;
        private String password;
        private ArrayList<String> hobbies;
        UserBuilder() {}
        public User.UserBuilder id(Integer id) { this.id = id; return this; }
        public User.UserBuilder username(String username) { this.username = username; return this; }
        public User.UserBuilder password(String password) { this.password = password; return this; }
 
        public User.UserBuilder hobby(String hobby) {
            if (this.hobbies == null) {
                this.hobbies = new ArrayList();
            }
            this.hobbies.add(hobby);
            return this;
        }
 
        public User.UserBuilder hobbies(Collection<? extends String> hobbies) {
            if (this.hobbies == null) {
                this.hobbies = new ArrayList();
            }
            this.hobbies.addAll(hobbies);
            return this;
        }
 
        public User.UserBuilder clearHobbies() {
            if (this.hobbies != null) {
                this.hobbies.clear();
            }
            return this;
        }
 
        public User build() {
            List hobbies;
            switch(this.hobbies == null ? 0 : this.hobbies.size()) {
            case 0:
                hobbies \= Collections.emptyList();
                break;
            case 1:
                hobbies \= Collections.singletonList(this.hobbies.get(0));
                break;
            default:
                hobbies \= Collections.unmodifiableList(new ArrayList(this.hobbies));
            }
            return new User(this.id, this.username, this.password, hobbies);
        }
        public String toString() {
            return "User.UserBuilder(id=" + this.id + ", username=" + this.username + ", password=" + this.password + ", hobbies=" + this.hobbies + ")";
        }
    }
}
```
其实，lombok 的创作者还是很用心的，在进行`build()`来创建实例对象时，<br />并没有直接使用`Collections.unmodifiableList(Collection)`此方法来创建实例，而是分为三种情况。

- 第一种，当集合中没有元素时，创建一个空 list
- 第二种情况，当集合中存在一个元素时，创建一个不可变的单元素 list
- 第三种情况，根据当前集合的元素数量创建对应合适大小的 list

当然看编译生成的代码，创建了三个关于集合操作的方法：

- `hobby(String hobby)`：向集合中添加一个元素
- `hobbies(Collection<? extends String> hobbies)`：添加一个集合所有的元素
- `clearHobbies()`：清空当前集合数据
<a name="sPupi"></a>
#### `@Singular` 注解配置 value 属性
先来看看 `@Singular` 注解的详情：
```java
@Target({FIELD, PARAMETER})
@Retention(SOURCE)
public @interface Singular {
    // 修改添加集合元素的方法名
    String value() default "";
}
```

- 测试如何使用注解属性value
```java
@Builder
public class User {
    private final Integer id;
    private final String zipCode = "123456";
    private String username;
    private String password;
    @Singular(value \= "testHobbies")
    private List<String> hobbies;
}
 
// 测试类
public class BuilderTest {
    public static void main(String\[\] args) {
        User user \= User.builder()
                .testHobbies("reading")
                .testHobbies("eat")
                .id(1)
                .password("admin")
                .username("admin")
                .build();
        System.out.println(user);
    }
}
```
说明，当使用了注解属性`value`之后，在使用添加集合元素时的方法名发生相应的改变。但是，同时生成的添加整个集合的方法名发生改变了吗？再来看看编译后的代码：
```java
/ 编译后：
public class User {
    // 省略部分代码，只看关键部分
    public static class UserBuilder {
        public User.UserBuilder testHobbies(String testHobbies) {
            if (this.hobbies == null) {
                this.hobbies = new ArrayList();
            }
            this.hobbies.add(testHobbies);
            return this;
        }
 
        public User.UserBuilder hobbies(Collection<? extends String> hobbies) {
            if (this.hobbies == null) {
                this.hobbies = new ArrayList();
            }
            this.hobbies.addAll(hobbies);
            return this;
        }
        
        public User.UserBuilder clearHobbies() {
            if (this.hobbies != null) {
                this.hobbies.clear();
            }
            return this;
        }
    }
}
```
可以看到，只有添加一个元素的方法名发生了改变。
<a name="SB1kc"></a>
#### `@Builder.Default` 的使用
比如有这样一个实体类：
```java
@Builder
@ToString
public class User {
    @Builder.Default
    private final String id = UUID.randomUUID().toString();
    private String username;
    private String password;
    @Builder.Default
    private long insertTime = System.currentTimeMillis();
}
```
在类中在`id`和`insertTime`上都添加注解`@Builder.Default`，当使用这个实体对象时，就不需要在为这两个字段进行初始化值，如下面这样：
```java
public class BuilderTest {
    public static void main(String\[\] args) {
        User user \= User.builder()
                .password("admin")
                .username("admin")
                .build();
        System.out.println(user);
    }
}

// 输出内容：
User(id=416219e1-bc64-43fd-b2c3-9f8dc109c2e8, username=admin, password=admin, insertTime=1546869309868)
```
lombok在实例化对象时就初始化了这两个字段值。<br />当然，如果再对这两个字段进行设值的话，那么默认定义的值将会被覆盖掉，如下面这样：
```java
public class BuilderTest {
    public static void main(String[] args) {
        User user = User.builder()
                .id("admin")
                .password("admin")
                .username("admin")
                .build();
        System.out.println(user);
    }
}
// 输出内容
User(id=admin, username=admin, password=admin, insertTime=1546869642151)
```
<a name="rwdBQ"></a>
#### `@Builder` 详细配置
下面再来详细看看`@Builder`这个注解类地详细实现：
```java
@Target({TYPE, METHOD, CONSTRUCTOR})
@Retention(SOURCE)
public @interface Builder {
    // 如果@Builder注解在类上，可以使用 @Builder.Default指定初始化表达式
    @Target(FIELD)
    @Retention(SOURCE)
    public @interface Default {}
    // 指定实体类中创建 Builder 的方法的名称，默认为: builder (个人觉得没必要修改)
    String builderMethodName() default "builder";
    // 指定 Builder 中用来构件实体类的方法的名称，默认为：build (个人觉得没必要修改)
    String buildMethodName() default "build";
    // 指定创建的建造者类的名称，默认为：实体类名+Builder
    String builderClassName() default "";
    // 使用toBuilder可以实现以一个实例为基础继续创建一个对象。（也就是重用原来对象的值）
    boolean toBuilder() default false;

    @Target({FIELD, PARAMETER})
    @Retention(SOURCE)
    public @interface ObtainVia {
        // 告诉lombok使用表达式获取值
        String field() default "";
        // 告诉lombok使用表达式获取值
        String method() default "";

        boolean isStatic() default false;
    }
}
```
以上注解属性，只测试一个比较常用的`toBuilder`，因为在对实体对象进行操作时，往往会存在对某些实体对象的某个字段进行二次赋值，这个时候就会用到这一属性。但是，这会创建一个新的对象，而不是原来的对象，原来的对象属性是不可变的，除非自己想要给这个实体类再添加上`@Data`或者`@setter`方法。下面就来测试一下：
```java
@Builder(toBuilder = true)
@ToString
public class User {
    private String username;
    private String password;
}
// 测试类
public class BuilderTest {
    public static void main(String\[\] args) {
        User user1 \= User.builder()
                .password("admin")
                .username("admin")
                .build();
        System.out.println(user1);
 
        User user2 \= user1.toBuilder().username("admin2").build();
        // 验证user2是否是基于user1的现有属性创建的
        System.out.println(user2);
        // 验证对象是否是同一对象
        System.out.println(user1 == user2);
    }
}
// 输出内容
User(username=admin, password=admin)
User(username\=admin2, password=admin)
false
```
<a name="J2JCF"></a>
#### `@Builder` 全局配置
```
# 是否禁止使用@Builder
lombok.builder.flagUsage = [warning | error] (default: not set)
# 是否使用Guaua
lombok.singular.useGuava = [true | false] (default: false)
# 是否自动使用singular，默认是使用
lombok.singular.auto = [true | false] (default: true)
```
总的来说`@Builder`还是很好用的。
