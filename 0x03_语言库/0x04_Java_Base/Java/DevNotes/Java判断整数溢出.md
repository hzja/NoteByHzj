Java
<a name="0837U"></a>
### 问题
解决int相加后怎么判断是否溢出，如果溢出就返回`Integer.MAX_VALUE`
<a name="urJ7p"></a>
### 解决方案
JDK8已经实现了在Math包下
<a name="s0DFa"></a>
#### 加法
```java
public static int addExact(int x, int y) {
    int r = x + y;
    // HD 2-12 Overflow iff both arguments have the opposite sign of the result
    if (((x ^ r) & (y ^ r)) < 0) {
        throw new ArithmeticException("integer overflow");
    }
    return r;
}
```
<a name="6ada91c8"></a>
#### 减法
```java
public static int subtractExact(int x, int y) {
    int r = x - y;
    // HD 2-12 Overflow iff the arguments have different signs and
    // the sign of the result is different than the sign of x
    if (((x ^ y) & (x ^ r)) < 0) {
        throw new ArithmeticException("integer overflow");
    }
    return r;
}
```
<a name="MZ4gQ"></a>
#### 乘法
```java
public static int multiplyExact(int x, int y) {
    long r = (long)x * (long)y;
    if ((int)r != r) {
        throw new ArithmeticException("integer overflow");
    }
    return (int)r;
}
```
注意 long和int是不一样的
```java
public static long multiplyExact(long x, long y) {
    long r = x * y;
    long ax = Math.abs(x);
    long ay = Math.abs(y);
    if (((ax | ay) >>> 31 != 0)) {
        // Some bits greater than 2^31 that might cause overflow
        // Check the result using the divide operator
        // and check for the special case of Long.MIN_VALUE * -1
        if (((y != 0) && (r / y != x)) ||
            (x == Long.MIN_VALUE && y == -1)) {
            throw new ArithmeticException("long overflow");
        }
    }
    return r;
}
```
