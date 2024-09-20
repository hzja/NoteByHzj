Java BeanUtils
<a name="heyld"></a>
## 1、背景
“不推荐使用属性拷贝工具”推荐直接定义转换类和方法使用 IDEA 插件自动填充 get / set 函数。<br />不推荐的主要理由是：

- 有些属性拷贝工具性能有点差
- 有些属性拷贝工具有“BUG”
- 使用属性拷贝工具容易存在一些隐患
<a name="len1O"></a>
## 2、示例
<a name="oR3lm"></a>
### Spring 的 BeanUtils 的属性拷贝会存在的问题
```java

import lombok.Data;

import java.util.List;

@Data
public class A {
    private String name;

    private List<Integer> ids;
}
```
```java
@Data
public class B {
    private String name;

    private List<String> ids;
}
```
```java
import org.springframework.beans.BeanUtils;

import java.util.Arrays;

public class BeanUtilDemo {
    public static void main(String[] args) {
        A first = new A();
        first.setName("demo");
        first.setIds(Arrays.asList(1, 2, 3));

        B second = new B();
        BeanUtils.copyProperties(first, second);
        for (String each : second.getIds()) {// 类型转换异常
            System.out.println(each);
        }
    }
}
```
运行上述示例时，会发生类型转换异常。<br />打断点可以看到，属性拷贝之后 B 类型的 second 对象中 ids 仍然为 Integer 类型：<br />![2021-05-23-13-51-06-742611.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621749206507-388f6bc4-927d-4fac-91e0-7f19183582ff.png#clientId=u1235a37b-3dc0-4&from=ui&id=u0b193204&originHeight=663&originWidth=982&originalType=binary&size=112764&status=done&style=none&taskId=u6225f98d-b5a9-4127-a778-917f36acc4a)<br />如果不转换为字符串，直接进行打印，并不会报错。

---

<a name="kKVdi"></a>
### 使用CGlib 在不定义Converter 的情况下也会遇到类似问题
```java
import org.easymock.cglib.beans.BeanCopier;

import java.util.Arrays;

public class BeanUtilDemo {
    public static void main(String[] args) {
        A first = new A();
        first.setName("demo");
        first.setIds(Arrays.asList(1, 2, 3));

        B second = new B();
        final BeanCopier beanCopier = BeanCopier.create(A.class, B.class, false);
        beanCopier.copy(first,second,null);

        for (String each : second.getIds()) {// 类型转换异常
            System.out.println(each);
        }
    }
}
```
同样，问题在运行时才暴露出来。

---

<a name="gm8JH"></a>
### 使用MapStruct存在的问题
接下来看下 MapStruct：
```java
import org.mapstruct.Mapper;
import org.mapstruct.factory.Mappers;

@Mapper
public interface Converter {
    Converter INSTANCE = Mappers.getMapper(Converter.class);

    B aToB(A car);
}
```
```java
import java.util.Arrays;

public class BeanUtilDemo {
    public static void main(String[] args) {
        A first = new A();
        first.setName("demo");
        first.setIds(Arrays.asList(1, 2, 3));

        B second = Converter.INSTANCE.aToB(first);
        for (String each : second.getIds()) {// 正常
            System.out.println(each);
        }
    }
}
```
可以成功的将 A 中 `List<Integer>` 转为 B 中的 `List<String>` 类型。<br />看编译生成的 Converter 实现类：
```java
import java.util.ArrayList;
import java.util.List;
import javax.annotation.Generated;
import org.springframework.stereotype.Component;

@Generated(
    value = "org.mapstruct.ap.MappingProcessor",
    comments = "version: 1.3.1.Final, compiler: javac, environment: Java 1.8.0_202 (Oracle Corporation)"
)
@Component
public class ConverterImpl implements Converter {

    @Override
    public B aToB(A car) {
        if ( car == null ) {
            return null;
        }

        B b = new B();

        b.setName( car.getName() );
        b.setIds( integerListToStringList( car.getIds() ) );

        return b;
    }

    protected List<String> integerListToStringList(List<Integer> list) {
        if ( list == null ) {
            return null;
        }

        List<String> list1 = new ArrayList<String>( list.size() );
        for ( Integer integer : list ) {
            list1.add( String.valueOf( integer ) );
        }

        return list1;
    }
}
```
自动进行了转换，可能不会意识到类型并不一致。<br />如果在 A 类中添加一个 String number 属性，在 B 类中添加一个 Long number 属性，使用 mapstruect 当 number 设置为非数字类型时就会报 `.NumberFormatException` 。
```java
@Override
public B aToB(A car) {
    if ( car == null ) {
        return null;
    }

    B b = new B();

    b.setName( car.getName() );
    if ( car.getNumber() != null ) { // 问题出在这里
        b.setNumber( Long.parseLong( car.getNumber() ) );
    }
    b.setIds( integerListToStringList( car.getIds() ) );

    return b;
}
```
使用 cglib 默认则不会映射 number 属性，B 中的 number 为 null。

---

如果手动定义转换器，使用 IDEA 插件(如 generateO2O)自动转换：
```java
public final class A2BConverter {

    public static B from(A first) {
        B b = new B();
        b.setName(first.getName());
        b.setIds(first.getIds());
        return b;
    }
}
```
在编码阶段就可以非常明确地发现这个问题：<br />![2021-05-23-13-51-06-889248.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621749486561-41a25e3d-4930-40ed-aa46-fe0c66ba3919.png#clientId=u1235a37b-3dc0-4&from=ui&id=ub489f63f&originHeight=405&originWidth=897&originalType=binary&size=51721&status=done&style=none&taskId=uef4c4254-187d-4a26-a750-4179ac98d3f)
<a name="iZKoY"></a>
## 3、结论
由于 Java 的泛型其实是编译期检查，编译后泛型擦除，导致运行时 `List<Integer>` 和 `List<String>` 都是 List 类型，可以正常赋值。这就导致在使用很多属性映射工具时，编译时不容易明显的错误。<br />MapStruct 自定义了注解处理器，在编译阶段可以读取映射双方的泛型类型，进而进行映射。但是这种映射也很可怕，有时候由于粗心等原因定义错了类型，自动进行了转换，会带了很多副作用。<br />之前对各种属性映射工具的性能进行了简单的对比，结果如下：<br />![2021-05-23-13-51-07-099513.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621749535367-0373bef7-762a-4900-9304-7c0464773be3.png#clientId=u1235a37b-3dc0-4&from=ui&id=ue93242c1&originHeight=930&originWidth=1080&originalType=binary&size=78511&status=done&style=shadow&taskId=u61685630-4147-4fd4-86bd-6163110509b)<br />因此慎用属性转换工具，如果可能建议自定义转换类，使用 IDEA插件自动填充，效率也挺高， A 或 B 中任何属性类型不匹配，甚至删除一个属性，编译阶段即可报错，而且直接调用 get set 的效率也是非常高的。
