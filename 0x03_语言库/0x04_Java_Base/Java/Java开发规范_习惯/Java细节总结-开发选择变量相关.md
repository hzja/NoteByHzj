<a name="v68iG"></a>
# 1、java中的`strictfp`的作用-精确浮点运算
`strictfp` 的意思是FP-strict，也就是说精确浮点的意思。在Java虚拟机进行浮点运算时，如果没有指定`strictfp`关键字时，Java的编译器以及运行环境在对浮点运算的表达式是采取一种近似于我行我素的行为来完成这些操作，以致于得到的结果往往无法令人满意。而一旦使用了`strictfp`来声明一个类、接口或者方法时，那么所声明的范围内Java的编译器以及运行环境会完全依照浮点规范IEEE-754来执行。因此如果想让浮点运算更加精确， 而且不会因为不同的硬件平台所执行的结果不一致的话，那就用关键字`strictfp`。<br />可以将一个类、接口以及方法声明为`strictfp`，但是不允许对接口中的方法以及构造函数声明strictfp关键字
<a name="paxsc"></a>
## 正确和错误的示例
<a name="4T6oJ"></a>
### ①、在接口上使用`strictfp`关键字
```java
/**
 * NumAdd
 * <p>
 * encoding:UTF-8
 *
 * @author Fcant 下午 23:03 2020/6/26/0026
 */
public strictfp interface NumAdd {
	void f();
}
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1593184107303-dc2b3805-63ce-4c63-aa19-3448dceb70be.png#height=266&id=Xi39R&originHeight=797&originWidth=1915&originalType=binary&ratio=1&rotation=0&showTitle=false&size=198653&status=done&style=shadow&title=&width=638.3333333333334)
<a name="QJeSa"></a>
### ②、在类和方法上使用`strictfp`关键字
```java
/**
 * ClassUseStrictfp
 * <p>
 * encoding:UTF-8
 *
 * @author Fcant 下午 23:11 2020/6/26/0026
 */
public strictfp class ClassUseStrictfp {

    public ClassUseStrictfp() {
    }

    public strictfp void sayHello() {
        System.out.println("Hello");
    }
}
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1593184475124-70a7af3a-bb79-46d9-959b-95c2082a02bd.png#height=362&id=FTXzM&originHeight=1086&originWidth=2263&originalType=binary&ratio=1&rotation=0&showTitle=false&size=291546&status=done&style=shadow&title=&width=754.3333333333334)
<a name="BkF2r"></a>
# 2、Java中货币选择的类型
java.math.BigDecimal
