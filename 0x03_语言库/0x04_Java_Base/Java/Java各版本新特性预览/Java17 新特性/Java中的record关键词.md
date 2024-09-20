Java<br />以前定义类都是用class关键词，但从Java 16开始，将多一个关键词`record`，它也可以用来定义类。<br />这个`record`关键词的引入，主要是为了提供一种更为简洁、紧凑的`final`类的定义方式。下面就来具体了解`record`类的细节。
<a name="XZipC"></a>
## 声明`record`类
声明record类的基础语法：
```java
record range(int start, int end){}
```
class类可以在单独文件中生命，也可以在其他类中申明。那么record类也一样，它有这几种申明方式：
<a name="nf06J"></a>
### 单独文件申明
```java
public record range(int start, int end){}
```
<a name="ndo1B"></a>
### 在类内部申明
```java
public class DidispaceTest {
    public record range(int start, int end){}
}
```
<a name="Lq8XC"></a>
### 函数内申明
```java
public class DidispaceTest {
	public void test() {
		public record range(int start, int end){}
	}
}
```
<a name="Oo7hV"></a>
## `record`类详解
在知道了如何申明之后，一定会想用`record`申明的就那么点元素，它到底会具备什么能力呢？<br />因为`record`关键词申明类主要是为了简化一些类的申明，所以它本质就是一类特殊的class，或者说是某一个模版的class。<br />`record`申明的类，具备这些特点：

1. 它是一个`final`类
2. 自动实现`equals`、`hashCode`、`toString`函数
3. 成员变量均为`public`属性

所以，对于之前写的range类，它等价于一个这样的类：
```java
public final class range{
    final int start;
    final int end;

    public range(int start, int end) {
        this.start = start;
        this.end = end;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        range range = (range) o;
        return start == range.start && end == range.end;
    }

    @Override
    public int hashCode() {
        return Objects.hash(start, end);
    }

    @Override
    public String toString() {
        return "range{" +
                "start=" + x +
                ", end=" + y +
                '}';
    }

    public int start(){
        return start;
    }

    public int end(){
        return end;
    }
}
```
因为一些函数是隐藏的，在range定义的时候，看不到，所以可以尝试写这样一段测试代码来验证一下：
```java
@Test
public void test() {
	range r = new range(100, 200);
	log.info(r.toString());

	range r2 = new range(100, 200);
	log.info(String.valueOf(r.equals(r2)));
}
```
<a name="O5AI2"></a>
## 定义成员函数
因为`record`申明的本质也是类，那么定义成员函数肯定也是可以的。<br />比如，可以这样在`record`类中定义成员函数：
```java
record range(int start, int end){
	int distance(){
		return end - start;
	}
}
```
然后，就可以这样调用它：
```java
range r = new range(100, 200);
r.distance();
```
