Java a.equals(b)
<a name="XOBOz"></a>
### 一、值是null的情况

1. `a.equals(b)`，a 是null，抛出NullPointException异常。
2. `a.equals(b)`，a不是null，b是null，返回false
3. `Objects.equals(a, b)`比较时，若a 和 b 都是null，则返回 true，如果a 和 b 其中一个是null，另一个不是null，则返回false。注意：不会抛出空指针异常。
```java
null.equals("abc")    →   抛出 NullPointerException 异常
"abc".equals(null)    →   返回 false
null.equals(null)     →   抛出 NullPointerException 异常
Objects.equals(null, "abc")    →   返回 false
Objects.equals("abc",null)     →   返回 false
Objects.equals(null, null)     →   返回 true
```
<a name="zJvmU"></a>
### 二、值是空字符串的情况

1. a 和 b 如果都是空值字符串：""，则 `a.equals(b)`，返回的值是true，如果a和b其中有一个不是空值字符串，则返回false;
2. 这种情况下 `Objects.equals` 与情况1 行为一致。
```java
"abc".equals("")    →   返回 false
"".equals("abc")    →   返回 false
"".equals("")       →   返回 true
Objects.equals("abc", "")    →   返回 false
Objects.equals("","abc")     →   返回 false
Objects.equals("","")        →   返回 true
```
<a name="LSAX3"></a>
### 三、源码分析
<a name="MrTvc"></a>
#### 1、源码
```java
* @since 1.7
 */
public final class Objects {
    private Objects() {
        throw new AssertionError("No java.util.Objects instances for you!");
    }
 
    /**
     * Returns {@code true} if the arguments are equal to each other
     * and {@code false} otherwise.
     * Consequently, if both arguments are {@code null}, {@code true}
     * is returned and if exactly one argument is {@code null}, {@code
     * false} is returned.  Otherwise, equality is determined by using
     * the {@link Object#equals equals} method of the first
     * argument.
     *
     * @param a an object
     * @param b an object to be compared with {@code a} for equality
     * @return {@code true} if the arguments are equal to each other
     * and {@code false} otherwise
     * @see Object#equals(Object)
     */
    public static boolean equals(Object a, Object b) {
        return (a == b) || (a != null && a.equals(b));
    }
}
```
<a name="NGzuj"></a>
#### 2、说明
首先，进行了对象地址的判断，如果是真，则不再继续判断。<br />如果不相等，后面的表达式的意思是，先判断a不为空，然后根据上面的知识点，就不会再出现空指针。<br />所以，如果都是null，在第一个判断上就为true了。如果不为空，地址不同，就重要的是判断a.equals(b)。
<a name="sQ71L"></a>
### 四、“`a==b`”和“`a.equals(b)`”有什么区别？
如果 a 和 b 都是对象，则 `a==b` 是比较两个对象的引用，只有当 a 和 b 指向的是堆中的同一个对象才会返回 true。<br />而 `a.equals(b)` 是进行逻辑比较，当内容相同时，返回true，所以通常需要重写该方法来提供逻辑一致性的比较。
