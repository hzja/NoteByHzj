Java<br />在日常开发中还有一些类经常被人叫做**POJO**，还有的人叫它们**Java Bean**。这些概念都是在特定场景下引入，用来表明它们的特性的。那这些称呼都是啥意思，有啥标准和特征呢？今天就来分享一下这些没用的知识。
<a name="UY5Lx"></a>
## POJO
**POJO**是 **Plain Old Java Object** 的简写，大概意思就是“淳朴的Java对象”。这个词是国外一家外包公司的员工创造的。哪些类是**POJO**类还是有说法的，需要同时满足以下几个条件：

1. **不实现任何接口的类。**
2. **不继承任何其它类的类。**
3. **不使用任何外部注解的类。**

这种类其实就是切断了和外界联系的Java类，下面这个类肯定不是：
```java
@Data
public class Dog {
	private String name;
	private Integer age;
}
```
这个才是**POJO**：
```java
public class Dog {
	private String name;
	private Integer age;
}
```
<a name="rR6XD"></a>
## Java Bean
**Java Bean**也经常出现在各种技术文献中，也不是随便什么类都能叫做**Java Bean**的，它需要有以下定义：

- **有无参数构造。**
- **所有的属性必须是私有属性（**`**private**`**）。**
- **所有的属性必须有公共的（**`**public**`**）的**`**Getter**`**和**`**Setter**`**。**
- **它必须是可以被序列化的，也就是实现 **`**java.io.Serializable**`**接口。**

按照这个定义，**POJO**类如果想成为**Java Bean**，需要改造成下面的形式:
```java
import java.io.Serializable;

public class Dog implements Serializable {
    private static final long serialVersionUID = 6723564465081191620L;
    private String name;
    private Integer age;

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
<a name="UG8Sr"></a>
## Spring Bean
既然说到这里了，不妨再说下**Spring Bean**。**Spring Bean**的要求就低多了，只要这个类（接口）被注入了**Spring IoC**，那么这个类（接口）都可以被称作一个**Spring Bean**。至于一个类如何注入**Spring IoC**，这里就不说了，大家天天都在做。
<a name="FYObm"></a>
## 最后
最后，可以得出结论，一个**POJO**总是孤孤单单的，它不可能成为一个**Java Bean**或者**Spring Bean**；但是**Java Bean**可以同时是一个**Spring Bean**；**Spring Bean**也可以是一个**Java Bean**。
