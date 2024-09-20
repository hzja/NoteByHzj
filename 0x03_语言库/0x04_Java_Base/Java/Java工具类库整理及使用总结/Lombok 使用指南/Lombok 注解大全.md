Java Lombok
<a name="YaHBR"></a>
### 一、`@Getter` and `@Setter`
使用`@Getter`和/或`@Setter`注释任何字段，以使`lombok`自动生成默认的`getter / setter`。<br />默认的`getter`只是返回该字段，如果该字段被称为`foo`，则名为`getFoo`（如果该字段的类型为`boolean`，则为`isFoo`）。<br />默认生成的 `getter / setter`方法是公共的，除非你明确指定一个`AccessLevel`。合法访问级别为`PUBLIC，PROTECTED，PACKAGE和PRIVATE`。<br />你还可以在类上添加`@Getter`和/或`@Setter`注释。在这种情况下，就好像你使用该注释来注释该类中的所有非静态字段一样。<br />你始终可以使用特殊的`AccessLevel.NONE`访问级别来手动禁用任何字段的`getter / setter`生成。这使你可以覆盖类上的`@Getter`，`@Setter`或`@Data`注释的行为。<br />With Lombok：
```java
import lombok.AccessLevel;
import lombok.Getter;
import lombok.Setter;
public class GetterSetterExample {

    @Getter 
    @Setter 
    private int age = 10;

    @Setter(AccessLevel.PROTECTED) 
    private String name;

    @Override 
    public String toString() {
        return String.format("%s (age: %d)", name, age);
    }
}
```
Native Java：
```java
public class GetterSetterExample {
    private int age = 10;
    private String name;

    @Override 
    public String toString() {
        return String.format("%s (age: %d)", name, age);
    }

    public int getAge() {
        return age;
    }

    public void setAge(int age) {
        this.age = age;
    }

    protected void setName(String name) {
        this.name = name;
    }
}
```
<a name="NDUFQ"></a>
### 二、`@ToString`
任何类定义都可以使用`@ToString`注释，以使`lombok`生成`toString()`方法的实现。<br />默认情况下，将打印所有非静态字段。如果要跳过某些字段，可以使用`@ ToString.Exclude`注释这些字段。或者，可以通过使用`@ToString（onlyExplicitlyIncluded = true）`，然后使用`@ToString.Include`标记要包含的每个字段，来确切指定希望使用的字段。<br />通过将`callSuper`设置为`true`，可以将`toString`的超类实现的输出包含到输出中。请注意，`java.lang.Object中toString()` 的默认实现几乎毫无意义。<br />With Lombok：
```java
import lombok.ToString;
@ToString
public class ToStringExample {
    private static final int STATIC_VAR = 10;
    private String name;
    private Shape shape = new Square(5, 10);
    private String[] tags;
    @ToString.Exclude 
    private int id;

    public String getName() {
        return this.name;
    }

    @ToString(callSuper=true, includeFieldNames=true)
    public static class Square extends Shape {
        private final int width, height;

        public Square(int width, int height) {
            this.width = width;
            this.height = height;
        }
    }
}
```
Native Java：
```java
import java.util.Arrays;
public class ToStringExample {
    private static final int STATIC_VAR = 10;
    private String name;
    private Shape shape = new Square(5, 10);
    private String[] tags;
    private int id;

    public String getName() {
        return this.name;
    }

    public static class Square extends Shape {
        private final int width, height;

        public Square(int width, int height) {
            this.width = width;
            this.height = height;
        }

        @Override 
        public String toString() {
            return "Square(super=" + super.toString() + ", width=" + this.width + ", height=" + this.height + ")";
        }
    }

    @Override 
    public String toString() {
        return "ToStringExample(" + this.getName() + ", " + this.shape + ", " + Arrays.deepToString(this.tags) + ")";
    }
}
```
<a name="nG5Ow"></a>
### 三、`@EqualsAndHashCode`
任何类定义都可以使用`@EqualsAndHashCode`进行注释，以使`lombok`生成`equals(Object other)`和`hashCode()`方法的实现。默认情况下，它将使用所有非静态，非瞬态字段，但是可以通过使用`@EqualsAndHashCode.Include`标记类型成员来修改使用哪些字段（甚至指定要使用各种方法的输出）。 `@EqualsAndHashCode.Exclude`。或者，可以通过使用`@EqualsAndHashCode.Include`标记并使用`@EqualsAndHashCode(onlyExplicitlyIncluded = true)`来精确指定要使用的字段或方法。<br />如果将`@EqualsAndHashCode`应用于扩展另一个类的类，则此功能会有些棘手。通常，为此类自动生成`equals`和`hashCode`方法是一个坏主意，因为超类还定义了字段，该字段也需要`equals / hashCode`代码，但不会生成此代码。通过将`callSuper`设置为`true`，可以在生成的方法中包括超类的`equals`和`hashCode`方法。<br />With Lombok：
```java
import lombok.EqualsAndHashCode;
@EqualsAndHashCode
public class EqualsAndHashCodeExample {
    private transient int transientVar = 10;
    private String name;
    private double score;
    @EqualsAndHashCode.Exclude 
    private Shape shape = new Square(5, 10);
    private String[] tags;
    @EqualsAndHashCode.Exclude 
    private int id;

    public String getName() {
        return this.name;
    }

    @EqualsAndHashCode(callSuper=true)
    public static class Square extends Shape {
        private final int width, height;

        public Square(int width, int height) {
            this.width = width;
            this.height = height;
        }
    }
}
```
Native Java：
```java
import java.util.Arrays;
public class EqualsAndHashCodeExample {
    private transient int transientVar = 10;
    private String name;
    private double score;
    private Shape shape = new Square(5, 10);
    private String[] tags;
    private int id;

    public String getName() {
        return this.name;
    }

    @Override 
    public boolean equals(Object o) {
        if (o == this) return true;
        if (!(o instanceof EqualsAndHashCodeExample)) return false;
        EqualsAndHashCodeExample other = (EqualsAndHashCodeExample) o;
        if (!other.canEqual((Object)this)) return false;
        if (this.getName() == null ? other.getName() != null : !this.getName().equals(other.getName())) return false;
        if (Double.compare(this.score, other.score) != 0) return false;
        if (!Arrays.deepEquals(this.tags, other.tags)) return false;
        return true;
    }

    @Override 
    public int hashCode() {
        final int PRIME = 59;
        int result = 1;
        final long temp1 = Double.doubleToLongBits(this.score);
        result = (result*PRIME) + (this.name == null ? 43 : this.name.hashCode());
        result = (result*PRIME) + (int)(temp1 ^ (temp1 >>> 32));
        result = (result*PRIME) + Arrays.deepHashCode(this.tags);
        return result;
    }

    protected boolean canEqual(Object other) {
        return other instanceof EqualsAndHashCodeExample;
    }

    public static class Square extends Shape {
        private final int width, height;

        public Square(int width, int height) {
            this.width = width;
            this.height = height;
        }

        @Override 
        public boolean equals(Object o) {
            if (o == this) return true;
            if (!(o instanceof Square)) return false;
            Square other = (Square) o;
            if (!other.canEqual((Object)this)) return false;
            if (!super.equals(o)) return false;
            if (this.width != other.width) return false;
            if (this.height != other.height) return false;
            return true;
        }

        @Override 
        public int hashCode() {
            final int PRIME = 59;
            int result = 1;
            result = (result*PRIME) + super.hashCode();
            result = (result*PRIME) + this.width;
            result = (result*PRIME) + this.height;
            return result;
        }

        protected boolean canEqual(Object other) {
            return other instanceof Square;
        }
    }
}
```
<a name="itjf0"></a>
### 四、`@AllArgsConstructor`、`@RequiredArgsConstructor`，`@NoArgsConstructor`
`@NoArgsConstructor`将生成没有参数的构造函数。如果字段由`final`修饰，则将导致编译器错误，除非使用`@NoArgsConstructor(force = true)`，否则所有`final`字段都将初始化为`0 / false / null`。对于具有约束的字段(例如`@NonNull`字段)，不会生成任何检查。<br />`@RequiredArgsConstructor`为每个需要特殊处理的字段生成一个带有1个参数的构造函数。所有未初始化的final字段都会获取一个参数，以及所有未声明其位置的未标记为`@NonNull`的字段。<br />`@AllArgsConstructor`为类中的每个字段生成一个带有1个参数的构造函数。标有`@NonNull`的字段将对这些参数进行空检查。<br />With Lombok：
```java
import lombok.AccessLevel;
import lombok.RequiredArgsConstructor;
import lombok.AllArgsConstructor;
import lombok.NonNull;
@RequiredArgsConstructor(staticName = "of")
@AllArgsConstructor(access = AccessLevel.PROTECTED)
public class ConstructorExample<T> {
    private int x, y;
    @NonNull 
    private T description;

    @NoArgsConstructor
        public static class NoArgsExample {
            @NonNull 
            private String field;
        }
}
```
Native Java：
```java
public class ConstructorExample<T> {
    private int x, y;
    @NonNull 
    private T description;

    private ConstructorExample(T description) {
        if (description == null) throw new NullPointerException("description");
        this.description = description;
    }

    public static <T> ConstructorExample<T> of(T description) {
        return new ConstructorExample<T>(description);
    }

    @java.beans.ConstructorProperties({"x", "y", "description"})
    protected ConstructorExample(int x, int y, T description) {
        if (description == null) throw new NullPointerException("description");
        this.x = x;
        this.y = y;
        this.description = description;
    }

    public static class NoArgsExample {
        @NonNull 
        private String field;

        public NoArgsExample() {
        }
    }
}
```
<a name="1bb69bcd"></a>
### 五、`@Data`
`@Data`是一个方便的快捷方式批注，它将`@ToString`，`@EqualsAndHashCode`，`@ Getter / @Setter`和`@RequiredArgsConstructor`的功能捆绑在一起：换句话说，`@Data`生成通常与简单`POJO`关联的所有样板（普通的旧`Java`对象）和`bean`：所有字段的`getter`，所有非`final`字段的`setter`，以及涉及类字段的适当的`toString`，`equals`和`hashCode`实现，以及初始化所有`final`字段以及所有非`final`字段的构造函数没有使用`@NonNull`标记的初始化程序，以确保该字段永远不会为`null`。<br />With Lombok：
```java
import lombok.AccessLevel;
import lombok.Setter;
import lombok.Data;
import lombok.ToString;
@Data 
public class DataExample {
    private final String name;
    @Setter(AccessLevel.PACKAGE) 
    private int age;
    private double score;
    private String[] tags;

    @ToString(includeFieldNames=true)
    @Data(staticConstructor="of")
    public static class Exercise<T> {
        private final String name;
        private final T value;
    }
}
```
Native Java：
```java
import java.util.Arrays;
public class DataExample {
    private final String name;
    private int age;
    private double score;
    private String[] tags;

    public DataExample(String name) {
        this.name = name;
    }

    public String getName() {
        return this.name;
    }

    void setAge(int age) {
        this.age = age;
    }

    public int getAge() {
        return this.age;
    }

    public void setScore(double score) {
        this.score = score;
    }

    public double getScore() {
        return this.score;
    }

    public String[] getTags() {
        return this.tags;
    }

    public void setTags(String[] tags) {
        this.tags = tags;
    }

    @Override 
    public String toString() {
        return "DataExample(" + this.getName() + ", " + this.getAge() + ", " + this.getScore() + ", " + Arrays.deepToString(this.getTags()) + ")";
    }

    protected boolean canEqual(Object other) {
        return other instanceof DataExample;
    }

    @Override 
    public boolean equals(Object o) {
        if (o == this) return true;
        if (!(o instanceof DataExample)) return false;
        DataExample other = (DataExample) o;
        if (!other.canEqual((Object)this)) return false;
        if (this.getName() == null ? other.getName() != null : !this.getName().equals(other.getName())) return false;
        if (this.getAge() != other.getAge()) return false;
        if (Double.compare(this.getScore(), other.getScore()) != 0) return false;
        if (!Arrays.deepEquals(this.getTags(), other.getTags())) return false;
        return true;
    }

    @Override 
    public int hashCode() {
        final int PRIME = 59;
        int result = 1;
        final long temp1 = Double.doubleToLongBits(this.getScore());
        result = (result*PRIME) + (this.getName() == null ? 43 : this.getName().hashCode());
        result = (result*PRIME) + this.getAge();
        result = (result*PRIME) + (int)(temp1 ^ (temp1 >>> 32));
        result = (result*PRIME) + Arrays.deepHashCode(this.getTags());
        return result;
    }

    public static class Exercise<T> {
        private final String name;
        private final T value;

        private Exercise(String name, T value) {
            this.name = name;
            this.value = value;
        }

        public static <T> Exercise<T> of(String name, T value) {
            return new Exercise<T>(name, value);
        }

        public String getName() {
            return this.name;
        }

        public T getValue() {
            return this.value;
        }

        @Override 
        public String toString() {
            return "Exercise(name=" + this.getName() + ", value=" + this.getValue() + ")";
        }

        protected boolean canEqual(Object other) {
            return other instanceof Exercise;
        }

        @Override 
        public boolean equals(Object o) {
            if (o == this) return true;
            if (!(o instanceof Exercise)) return false;
            Exercise<?> other = (Exercise<?>) o;
            if (!other.canEqual((Object)this)) return false;
            if (this.getName() == null ? other.getValue() != null : !this.getName().equals(other.getName())) return false;
            if (this.getValue() == null ? other.getValue() != null : !this.getValue().equals(other.getValue())) return false;
            return true;
        }

        @Override 
        public int hashCode() {
            final int PRIME = 59;
            int result = 1;
            result = (result*PRIME) + (this.getName() == null ? 43 : this.getName().hashCode());
            result = (result*PRIME) + (this.getValue() == null ? 43 : this.getValue().hashCode());
            return result;
        }
    }
}
```
<a name="a6szf"></a>
### 六、`@Value`
`@Value`注解和`@Data`类似，区别在于它会把所有成员变量默认定义为`private final`修饰，并且不会生成`set`方法。
<a name="FRRIO"></a>
### 七、`@Builder`
构建者模式<br />只能标注到类上，将生成类的一个当前流程的一种链式构造工厂，如下：
```java
User buildUser = User.builder().username("riemann").password("123").build();
```
可配合`@Singular`注解使用，`@Singular`注解使用在jdk内部集合类型的属性，`Map`类型的属性以及`Guava`的`com.google.common.collect` 的属性上。例如 未标注`@Singular`的属性，一般`setter`时，会直接覆盖原来的引用，标注了`@Singular`的属性，集合属性支持添加操作，会在属性原来的基础上增加。<br />With Lombok：
```java
import lombok.Builder;
import lombok.Singular;
import java.util.Set;
@Builder
public class BuilderExample {
    @Builder.Default 
    private long created = System.currentTimeMillis();
    private String name;
    private int age;
    @Singular 
    private Set<String> occupations;
}
```
Native Java：
```java
import java.util.Set;
public class BuilderExample {
    private long created;
    private String name;
    private int age;
    private Set<String> occupations;

    BuilderExample(String name, int age, Set<String> occupations) {
        this.name = name;
        this.age = age;
        this.occupations = occupations;
    }

    private static long $default$created() {
        return System.currentTimeMillis();
    }

    public static BuilderExampleBuilder builder() {
        return new BuilderExampleBuilder();
    }

    public static class BuilderExampleBuilder {
        private long created;
        private boolean created$set;
        private String name;
        private int age;
        private java.util.ArrayList<String> occupations;

        BuilderExampleBuilder() {
        }

        public BuilderExampleBuilder created(long created) {
            this.created = created;
            this.created$set = true;
            return this;
        }

        public BuilderExampleBuilder name(String name) {
            this.name = name;
            return this;
        }

        public BuilderExampleBuilder age(int age) {
            this.age = age;
            return this;
        }

        public BuilderExampleBuilder occupation(String occupation) {
            if (this.occupations == null) {
                this.occupations = new java.util.ArrayList<String>();
            }

            this.occupations.add(occupation);
            return this;
        }

        public BuilderExampleBuilder occupations(Collection<? extends String> occupations) {
            if (this.occupations == null) {
                this.occupations = new java.util.ArrayList<String>();
            }
            this.occupations.addAll(occupations);
            return this;
        }

        public BuilderExampleBuilder clearOccupations() {
            if (this.occupations != null) {
                this.occupations.clear();
            }

            return this;
        }
        public BuilderExample build() {
            // complicated switch statement to produce a compact properly sized immutable set omitted.
            Set<String> occupations = ...;
            return new BuilderExample(created$set ? created : BuilderExample.$default$created(), name, age, occupations);
        }

        @java.lang.Override
        public String toString() {
            return "BuilderExample.BuilderExampleBuilder(created = " + this.created + ", name = " + this.name + ", age = " + this.age + ", occupations = " + this.occupations + ")";
        }
    }
}
```
<a name="hrJAy"></a>
### 八、`@Accessors`
链式风格<br />`@Accessors`批注用于配置`lombok`如何生成和查找`getter`和`setter`。<br />默认情况下，`lombok`遵循针对`getter`和`setter`的`bean`规范：例如，名为`Pepper`的字段的`getter`是`getPepper`。但是，有些人可能希望打破`bean`规范，以得到更好看的`API`。 `@Accessors`允许您执行此操作。<br />可标注在类或属性上，当然最实用的功能还是标注到类上。<br />标注到类上，`chain`属性设置为`true`时，类的所有属性的`setter`方法返回值将为`this`，用来支持`setter`方法的链式写法。如：
```java
new User().setUsername("riemann").setPassword("123");
```
`fluent`属性设置为`true`时，类的所有`getter`，`setter`方法将省略`get`和`set`前缀，获取属性值直接使用属性名相同的无参方法，设置属性值使用属性名相同的有参方法，并且返回值为this。如：
```java
User user = new User().username("riemann").password("123");
String username = user.username();
String password = user.password();
```
标注到属性上，使用`prefix`设置需要省略的属性生成`getter`，`setter`方法时的前缀，且属性必须为驼峰式命名。
```java
@Accessors(prefix = "r")
@Getter
@Setter
private String rUsername = "riemann";
```
编译之后为
```java
public String getUsername() {
    return rUsername;
}
public void setUsername(String rUsername) {
    this.rUsername = rUsername;
}
```
<a name="x3vVf"></a>
### 九、`@Slf4j` and `@Log4j`
在需要打印日志的类中使用，项目中使用`slf4j`、`log4j`日志框架
<a name="yEFmd"></a>
### 十、`@NonNull`
该注解快速判断是否为空，为空抛出`java.lang.NullPointerException`。
<a name="WSwUW"></a>
### 十一、`@Synchronized`
注解自动添加到同步机制，生成的代码并不是直接锁方法，而是锁代码块， 作用范围是方法上。
<a name="Td2d7"></a>
### 十二、`@Cleanup`
注解用于确保已分配的资源被释放（`IO`的连接关闭）。
