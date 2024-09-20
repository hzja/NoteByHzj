Java BigDecimal
<a name="hCOcr"></a>
## 前言
BigDecimal 是 java.math 包中提供的一种可以用来进行精确运算的类型。所以，在支付、电商等业务中，BigDecimal 的使用非常频繁。而且其内部自带了很多方法，如加，减，乘，除等运算方法都是可以直接调用的。除了需要用 BigDecimal 表示数字和进行数字运算以外，代码中还经常需要对于数字进行相等判断。<br />说到等值比较，先看看《阿里巴巴Java开发手册》中的要求👇：<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1645158646218-84a50ba0-52c9-435e-8b63-bab4ebc57e4c.png#clientId=uc6ed81f9-8d16-4&from=paste&height=183&id=u12aa1e3b&originHeight=457&originWidth=1775&originalType=binary&ratio=1&rotation=0&showTitle=false&size=375112&status=done&style=shadow&taskId=u0d723201-2c70-437f-9771-6e65e7e48bd&title=&width=710)<br />那么为什么会有这样的要求呢🤔~ 其中的奥秘是什么呢🤔~
<a name="dDaSG"></a>
## BigDecimal 做等值比较
```java
public static void main(String[] args) {
    BigDecimal bigDecimal1 = new BigDecimal(1);
    BigDecimal bigDecimal2 = new BigDecimal(1);

    if(bigDecimal1 == bigDecimal2){
        //等值比较
    }
}
```
可以看出来上面的代码是有问题的，因为 BigDecimal 是对象，不能使用 == 来做等值判断。<br />如果使用 BigDecimal 的 equals 方法做等值比较是不是可以呢？👇
```java
public static void main(String[] args) {
    BigDecimal bigDecimal1 = new BigDecimal(1);
    BigDecimal bigDecimal2 = new BigDecimal(1);

    if(bigDecimal1.equals(bigDecimal2)){
        //等值比较
    }
}
```
这里先跑代码来看看能不能用 BigDecimal 的 equals 方法做等值比较，
```java
public static void main(String[] args) {
    BigDecimal bigDecimal1 = new BigDecimal(1);
    BigDecimal bigDecimal2 = new BigDecimal(1);
    System.out.println(bigDecimal1.equals(bigDecimal2));

    BigDecimal bigDecimal3 = new BigDecimal(1);
    BigDecimal bigDecimal4 = new BigDecimal(1.0);
    System.out.println(bigDecimal3.equals(bigDecimal4));

    BigDecimal bigDecimal5 = new BigDecimal("1");
    BigDecimal bigDecimal6 = new BigDecimal("1.0");
    System.out.println(bigDecimal5.equals(bigDecimal6));

}
```
![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1645157548499-520e0d80-aa4c-489d-9acc-f2fbe066f37e.png#clientId=uc6ed81f9-8d16-4&from=paste&height=629&id=u99d6de51&originHeight=1572&originWidth=2430&originalType=binary&ratio=1&rotation=0&showTitle=false&size=292417&status=done&style=shadow&taskId=u945cf64b-c5d5-4e6d-88a9-46e39d7d2a3&title=&width=972)<br />可以发现，在使用 BigDecimal 的 equals 方法对 1 和 1.0 进行比较的时候：使用 int、double 定义 BigDecimal 结果是 true；使用 String 定义 BigDecimal 结果是false，为什么会出现这种情况呢？<br />一起来看看 equals 方法的源码 👇
```java
/**
 * Compares this {@code BigDecimal} with the specified
 * {@code Object} for equality.  Unlike {@link
 * #compareTo(BigDecimal) compareTo}, this method considers two
 * {@code BigDecimal} objects equal only if they are equal in
 * value and scale (thus 2.0 is not equal to 2.00 when compared by
 * this method).
 *
 * @param  x {@code Object} to which this {@code BigDecimal} is
 *         to be compared.
 * @return {@code true} if and only if the specified {@code Object} is a
 *         {@code BigDecimal} whose value and scale are equal to this
 *         {@code BigDecimal}'s.
 * @see    #compareTo(java.math.BigDecimal)
 * @see    #hashCode
 */
@Override
public boolean equals(Object x) {
    if (!(x instanceof BigDecimal))
        return false;
    BigDecimal xDec = (BigDecimal) x;
    if (x == this)
        return true;
    if (scale != xDec.scale)
        return false;
    long s = this.intCompact;
    long xs = xDec.intCompact;
    if (s != INFLATED) {
        if (xs == INFLATED)
            xs = compactValFor(xDec.intVal);
        return xs == s;
    } else if (xs != INFLATED)
        return xs == compactValFor(this.intVal);

    return this.inflated().equals(xDec.inflated());
}
```
其实从方法的注释中就能找到答案：equals 方法会比较两部分内容，分别是值（value）和标度（scale），也就是说 bigDecimal5 和 bigDecimal6 的值虽然相同，但是标度是不一样的。<br />打个断点，debug 一下看看~<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1645157641469-a2ea6306-20cd-4cf6-a270-d180a7dfa5f1.png#clientId=uc6ed81f9-8d16-4&from=paste&height=737&id=ue5289ee1&originHeight=1843&originWidth=2470&originalType=binary&ratio=1&rotation=0&showTitle=false&size=408286&status=done&style=shadow&taskId=u1bfbdc53-1ace-44ca-87a4-9588480e17e&title=&width=988)<br />可以看见 bigDecimal5 的标度值是0，而bigDecimal6的标度值是1，所以 bigDecimal5 和 bigDecimal6 的比较结果是false (●ˇ∀ˇ●)<br />那么这时候又产生了一个疑问：为什么标度不同呢？🤔<br />BigDecimal 一共有以下 4 个构造方法：

- `BigDecimal(int)`
- `BigDecimal(double)`
- `BigDecimal(long)`
- `BigDecimal(String)`

其中最容易理解的就是 `BigDecimal(int)`和 `BigDecimal(long)`，因为是整数，所以标度就是 0 （源码如下👇）：
```java
/**
 * Translates an {@code int} into a {@code BigDecimal}.  The
 * scale of the {@code BigDecimal} is zero.
 *
 * @param val {@code int} value to be converted to
 *            {@code BigDecimal}.
 * @since  1.5
 */
public BigDecimal(int val) {
    this.intCompact = val;
    this.scale = 0;
    this.intVal = null;
}

/**
 * Translates a {@code long} into a {@code BigDecimal}.  The
 * scale of the {@code BigDecimal} is zero.
 *
 * @param val {@code long} value to be converted to {@code BigDecimal}.
 * @since  1.5
 */
public BigDecimal(long val) {
    this.intCompact = val;
    this.intVal = (val == INFLATED) ? INFLATED_BIGINT : null;
    this.scale = 0;
}
```
而对于 `BigDecimal (double)`来说，当使用 `new BigDecimal (0.1)`创建一个对象的时候，其实创建出来的对象的值并不是等于0.1，而是等于`0.1000000000000000055511151231257827021181583404541015625`<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1645157782437-d71c58d0-902c-46db-b00a-bdcb99233486.png#clientId=uc6ed81f9-8d16-4&from=paste&height=362&id=uecf3e0da&originHeight=906&originWidth=2460&originalType=binary&ratio=1&rotation=0&showTitle=false&size=154824&status=done&style=shadow&taskId=u3a126096-eb94-4c26-8da5-e8301ee2f13&title=&width=984)<br />再打个断点，debug一下看看标度值是多少<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1645157902529-1c9e6cc9-0134-4bdc-854b-7397b386b612.png#clientId=uc6ed81f9-8d16-4&from=paste&height=737&id=u0fcc888d&originHeight=1842&originWidth=2473&originalType=binary&ratio=1&rotation=0&showTitle=false&size=345737&status=done&style=shadow&taskId=u0dbe3238-f910-4680-9c2e-47194063c5d&title=&width=989.2)<br />可以看到标度值是55，这个值是怎么来的呢？其实很简单，这个标度值就是这个数字的位数，其他的浮点数也同样的道理。对于 `new BigDecimal (1.0)`，和`new BigDecimal (1.00)` 这样的形式来说，因为他本质上也是个整数，所以他创建出来的数字的标度就是0。<br />最后再看看 `BigDecimal(String)` ，当使用 `new BigDecimal ("0.1")` 创建一个 BigDecimal 的时候，其实创建出来的值正好就是等于 0.1 的。那么他的标度也就是 1；如果使用 `new BigDecimal("0.10000")`，那么创建出来的数就是 0.10000，标度也就是 5。<br />到这里相信各位小伙伴也明白了为什么 bigDecimal5 和 bigDecimal6 用equals 方法做等值比较的结果是false了。<br />如果只想判断两个 BigDecimal 的值是否相等，那么该如何判断呢？<br />在 BigDecimal 中也提供了一个方法 —— `compareTo` 方法，这个方法就可以只比较两个数字的值，如果两个数相等，则返回 0。<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1645157975644-30b2ecab-c2b8-4ac6-882c-f19208b40e75.png#clientId=uc6ed81f9-8d16-4&from=paste&height=675&id=u1a6d0f5b&originHeight=1687&originWidth=2508&originalType=binary&ratio=1&rotation=0&showTitle=false&size=331979&status=done&style=shadow&taskId=u63515f88-ecf3-4ffb-aa1a-4fc7b4b6d6f&title=&width=1003.2)<br />把 equals 换成 `compareTo` 后可以发现，bigDecimal5 和 bigDecimal6 等值比较的结果是0，也就是说明这二者的值是相等的。<br />P.S. 所以在做等值比较的时候不要随便用 BigDecimal 的 equals 方法，如果只是要对数值作比较，就果断选择 compareTo 方法。
