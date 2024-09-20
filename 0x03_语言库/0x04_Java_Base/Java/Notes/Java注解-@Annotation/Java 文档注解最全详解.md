Java
<a name="ebHhJ"></a>
## 一、简介
在开发项目的时候，时不时需要查阅官方 JDK API 文档，以便于更加清晰的了解某个类方法的用途以及正确的使用姿势。<br />打开 JDK 源码，会看到代码上有大量的文档注释，包括包的描述、类的描述、方法描述以及类属性的描述说明等等，**同时你还会惊奇的发现，源码上的注释与 JDK API 文档中的描述如出一辙**！<br />实际上，JDK API 文档是根据 Javadoc 工具生成的！<br />那什么是 Javadoc 呢？
> 官方回答: Javadoc is a tool for generating API documentation in HTML format from doc comments in source code.

翻译过来的意思是：**Javadoc 是一款能根据源代码中的文档注释来产生 HTML 格式的 API 文档的工具**。<br />简单的说就是，只要在 java 源码中按照标准的格式写注释，就可以利用 javadoc 这款工具自动生成配套的 java API 文档。<br />本篇文章的主要内容，就是总结 java 文档注释应该按照什么样的格式来写，只要格式对了，java API 文档就能按照工具来自动生成。
<a name="ruYQ3"></a>
## 二、文档注释格式总结
Java 文档注释是专门为了用 javadoc 工具自动生成文档而编写的一套注释标准，通过 javadoc 命令可以把文档注释中的内容生成文档，并输出到 HTML 文件中，与一般的注释有所不同，相关的规则如下：

- 所有的 Java 文档注释都以`/**`开头，`*/`结尾，而不是`/*`或`//`
- 文档注释覆盖范围包括：类、接口、方法、构造器、成员字段，如果写在其他位置，比如函数内部，被视为无效的文档注释
- 每个 Java 文档注释都要和其后对应的类/方法/字段/包保持同样的缩进
- Java 文档注释的内容，支持采用HTML语法规则书写，同时也支持一些额外的辅助标签

在类/方法上的文档标注一般分为三段，顺序如下：

- 第一段：概要描述，通常用一句话或者一段话简要描述该类的作用，以英文句号结束，这句话会被提取并放到索引目录中
- 第二段：详细描述，通常用一段话或者多段话详细描述该类的作用，每段话都以英文句号结束，详细描述中可以使用 html 标签，如`<p>`，`<pre>`，`<a>`，`<li>`等标签
- 第三段：文档标记，通常用于标注作者、创建时间、参阅类等信息，**描述部分和文档标记之间必须空一行**

比如java.util.Arrays类和部分方法，源码 java 文档注释如下！
```java
package java.util;

/**
 * This class contains various methods for manipulating arrays (such as
 * sorting and searching). This class also contains a static factory
 * that allows arrays to be viewed as lists.
 *
 * <p>The documentation for the methods contained in this class includes
 * briefs description of the <i>implementations</i>. Such descriptions should
 * be regarded as <i>implementation notes</i>, rather than parts of the
 * <i>specification</i>. Implementors should feel free to substitute other
 * algorithms, so long as the specification itself is adhered to. (For
 * example, the algorithm used by {@code sort(Object[])} does not have to be
 * a MergeSort, but it does have to be <i>stable</i>.)
 *
 * <p>This class is a member of the
 * <a href="{@docRoot}/../technotes/guides/collections/index.html">
 * Java Collections Framework</a>.
 *
 * @author Josh Bloch
 * @author Neal Gafter
 * @author John Rose
 * @since  1.2
 */
public class Arrays {
 
    /**
     * Sorts the specified array into ascending numerical order.
     *
     * <p>Implementation note: The sorting algorithm is a Dual-Pivot Quicksort
     * by Vladimir Yaroslavskiy, Jon Bentley, and Joshua Bloch. This algorithm
     * offers O(n log(n)) performance on many data sets that cause other
     * quicksorts to degrade to quadratic performance, and is typically
     * faster than traditional (one-pivot) Quicksort implementations.
     *
     * @param a the array to be sorted
     */
    public static void sort(int[] a) {
        DualPivotQuicksort.sort(a, 0, a.length - 1, null, 0, 0);
    }
}
```
<a name="zhhnc"></a>
## 三、文档标签总结
Javadoc 工具可以识别文档注释中的一些特殊标签，这些标签一般以`@`开头，后跟一个指定的名字，有的也以`{@`开头，以`}`结束，javadoc 可以识别的标签如下！
<a name="Wjbdz"></a>
### `@author`
说明：用于指明作者或者组织，可以附加邮箱或者超链接，如果有多少个作者就用多少个该标签。适用范围：包、 类、接口 案例如下：
```java
/**
 * @author xxxx
 * @author xxx@123.com
 * @author <a href="mailto:xxx@123.com">xxxxx</a>
 */
```
<a name="dCxVc"></a>
### `@since`
说明：用于指明最早出现在哪个版本，可填版本号或日期，有时也可表明可运行的JDK版本。适用范围：包、类、接口、方法、成员属性、构造器 案例如下：
```java
/**
 * @since 1.4
 * @since 15 April 2001
 * @since JDK1.5
 */
```
<a name="lCgzT"></a>
### `@version`
说明：用于指明当前版本号。适用范围：包、 类、接口 案例如下：
```java
/**
 * @version 1.8.3.4
 */
```
<a name="lxZpQ"></a>
### `@code`
说明：用于将一些关键字或代码解析成代码样式。适用范围：包、类、接口、方法、成员属性、构造器 案例如下：
```java
/**
 * xxxx,{@code int var = 1;}xxxx
 */
```
<a name="gmRJj"></a>
### `@return`
说明：对函数返回值的注释。适用范围：方法 案例如下：
```java
/**
 * @return <code>true</code> 执行成功;<code>false</code> 执行失败.
 */
```
<a name="t9kYK"></a>
### `@param`
说明：用于方法的入参名及描述信息。适用范围：构造器、方法 案例如下：
```java
/**
 * @param img the image to be drawn
 */
```
<a name="o816U"></a>
### `@value`
说明：只能用于对常量进行注释，该标签常用于写在字段上的 Javadoc 注释。适用范围：方法、成员属性 案例如下：
```java
/**
 * Default start value. {@value #START_VALUE}
 */
```
<a name="abYv7"></a>
### `@throws` 和 `@exception`
说明：用户描述构造函数或方法所会抛出的异常。适用范围：方法、构造器 案例如下：
```java
/**
 * @throws IllegalArgumentException
 *         if {@code fromIndex > toIndex}
 */
```
<a name="dN6Cj"></a>
### `@link` 、`@linkplain` 和 `@see`
说明：用于创建一个超链接到相关代码。适用范围：包、类、接口、方法、成员属性、构造器 注意事项：1、`@link` 和 `@linkplain`的区别在于：`@link`直接将将超链接中的地址当作显示的文本，其格式为 `{@link 地址}`；而 `@linkplain`可以自行设定显示的文本，其格式为`{@link 地址 显示文本}`，三个字段用空格隔开。2、`@link` 和 `@see`的区别在于：`@link`可以放在某一行中的任意位置；而 `@see`必须放在某一行中的最开始。<br />案例如下：
```java
/**
 * xxx {@link java.lang.String#charAt(int)}
 * xxx {@link #sort(Object[])}
 * xxx {@linkplain Comparable natural ordering}
 * @see #deepHashCode(Object[])
 */
```
<a name="iUqWn"></a>
### `@inheritDoc`
说明：用于继承父类的 javadoc 注释，父类的文档注释会被拷贝到子类。适用范围：方法 案例如下：
```java
/**
 * {@inheritDoc}
 * <p>
 * The speed of tiger will be returned.
 * 
 * @return the speed of tiger 
 */
```
<a name="F6ciT"></a>
### `@deprecated`
说明：告诉用户该 API 已过时，并且已被哪些新的 API 取代了。用`@see`或 `@link`指向新的API。该旧的 API 之所以不删掉，通常是为了兼容性考虑。。适用范围：包、类、接口、方法、成员属性、构造器 案例如下：
```java
/**
* @deprecated As of JDK 1.1, replaced by 
* {@link #setBounds(int, int, int, int)}
*/
```
<a name="ffhQO"></a>
### `@serial`
说明：说明一个序列化属性。适用范围：方法、成员属性 案例如下：
```java
/**
 * @serial description
 */
```
<a name="Y4oOx"></a>
### `@serialData`
说明：用于说明通过 `writeObject()` 和 `writeExternal()` 方法写的数据。。适用范围：方法、成员属性 案例如下：
```java
/**
 * @serialData description
 */
```
<a name="YYN2i"></a>
### `@serialField`
说明：用于说明一个 ObjectStreamField 组件。适用范围：方法、成员属性 案例如下：
```java
/**
 * @serialField name type description
 */
```
最后，在使用文档注释时，通常会按照如下顺序进行使用。

- `@author`
- `@version`
- `@param`
- `@return`
- `@exception` / `@throws`
- `@see`
- `@since`
- `@serial` / `@serialField` / `@serialData`
- `@deprecated`
<a name="h3POl"></a>
## 四、java api 文档生成方式
Javadoc 是 Sun 公司提供的一个技术，它从程序源代码中抽取类、方法、成员等注释，然后形成一个和源代码配套的 API 帮助文档。<br />文档的创建命令如下：
```bash
javadoc -d 文档存放目录 -author -version 源文件名.java
```
这条命令编译一个名为源文件名.java的 java 源文件，并将生成的文档存放在文档存放目录指定的目录下，生成的文档中index.html就是文档的首页。<br />`-author`和`-version`两个选项可选项，使用者可以通过`javadoc -help`命令查看 javadoc 使用说明。<br />**注意**：Javadoc 默认只提取 public、protected 修饰的部分，如果要提取 private 修饰的部分，需要使用 -private。<br />下面是一个使用说明文档注释的简单实例。
```java
/**
 * 这个类演示了文档注释
 *
 * @author Ayan Amhed
 * @version 1.2
 */
public class SquareNum {
    /**
     * This method returns the square of num.
     * This is a multiline description. You can use
     * as many lines as you like.
     *
     * @param num The value to be squared.
     * @return num squared.
     */
    public double square(double num) {
        return num * num;
    }

    /**
     * This method inputs a number from the user.
     *
     * @return The value input as a double.
     * @throws IOException On input error.
     * @see IOException
     */
    public double getNumber() throws IOException {
        InputStreamReader isr = new InputStreamReader(System.in);
        BufferedReader inData = new BufferedReader(isr);
        String str;
        str = inData.readLine();
        return (new Double(str)).doubleValue();
    }

    /**
     * This method demonstrates square().
     *
     * @param args Unused.
     * @return Nothing.
     * @throws IOException On input error.
     * @see IOException
     */
    public static void main(String args[]) throws IOException {
        SquareNum ob = new SquareNum();
        double val;
        System.out.println("Enter value to be squared: ");
        val = ob.getNumber();
        val = ob.square(val);
        System.out.println("Squared value is " + val);
    }
}
```
使用 javadoc 工具处理 SquareNum.java 文件，生成 javadoc api 文档，在命令行输入如下命令，即可实现文档的生成！
```bash
javadoc SquareNum.java
```
<a name="AMm4n"></a>
## 五、小结
Javadoc 是一款为程序生成 API 文档的工具，只需按照规定的格式编写代码文档注释，即可生成 API 的帮助文档。
