<a name="sHG0p"></a>
## `StringUtils.isEmpty`
<a name="fn4MF"></a>
### 源码
这个方法判断的是字符串是否为null或者其长度是否为零。
```java
public static boolean isEmpty(CharSequence cs) {
    return cs == null || cs.length() == 0;
}
public static boolean isNotEmpty(String str) {
    return !isEmpty(str);
}
```
<a name="qL3uH"></a>
### 测试效果
```java
package com.fcant.fclink.utils;

import org.apache.commons.lang3.StringUtils;

/**
 * TestCheckNull
 * <p>
 * encoding:UTF-8
 *
 * @author Fcant 上午 10:12 2020/7/26/0026
 */
public class TestCheckNull {

    public static void main(String[] args) {
        BlankAndEmpty();
    }

    public static void BlankAndEmpty(){
        System.out.println(StringUtils.isEmpty(null));
        System.out.println(StringUtils.isEmpty(""));
        System.out.println(StringUtils.isEmpty(" "));
        System.out.println(StringUtils.isEmpty("\t"));
        System.out.println(StringUtils.isEmpty("Fcant"));
    }
}

```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1595729928938-261a2018-75e3-49af-a3da-2f8759a2bbaa.png#averageHue=%23f8f8f7&height=687&id=c70jp&originHeight=2060&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=754221&status=done&style=shadow&title=&width=1280)
<a name="yC36A"></a>
## `StringUtils.isBlank`
<a name="NVDLq"></a>
### 源码
这个方法除了判断字符串是否为null和长度是否为零，还判断了是否为空格，如果是空格也返回true。
```java
public static boolean isBlank(CharSequence cs) {
    int strLen = length(cs);
    if (strLen == 0) {
        return true;
    } else {
        for(int i = 0; i < strLen; ++i) {
            if (!Character.isWhitespace(cs.charAt(i))) {
                return false;
            }
        }

        return true;
    }
}
public static boolean isNotBlank(String str) {
    return !isBlank(str);
}
```
<a name="JaYBr"></a>
### 测试效果
```java
package com.fcant.fclink.utils;

import org.apache.commons.lang3.StringUtils;

/**
 * TestCheckNull
 * <p>
 * encoding:UTF-8
 *
 * @author Fcant 上午 10:12 2020/7/26/0026
 */
public class TestCheckNull {

    public static void main(String[] args) {
        BlankAndEmpty();
    }

    public static void BlankAndEmpty(){
        System.out.println(StringUtils.isBlank(null));
        System.out.println(StringUtils.isBlank(""));
        System.out.println(StringUtils.isBlank(" "));
        System.out.println(StringUtils.isBlank("\t"));
        System.out.println(StringUtils.isBlank("Fcant"));
    }
}
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1595730072793-e939553c-d800-487b-bba3-3692d74a3adb.png#averageHue=%23f9f8f8&height=687&id=aVNBU&originHeight=2060&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=708647&status=done&style=shadow&title=&width=1280)
<a name="aQwiP"></a>
## 总结

- `isEmpty`：如果是null或者""则返回true。
- `isBlank`：如果是null或者""或者空格或者制表符则返回true。**「**`**isBlank**`**判空更加准确」**。
<a name="j1O0T"></a>
## 扩展
判断几个字段同时不能为空，如果还用isBlank就显得有点累赘了。可以使用String的可变参数提供如下工具类。
```java
public class StringTool {
    public static boolean isNullStr(String.. . args) {
        boolean falg = false;
        for (String arg : args) {
            if (Stringutils.isBlank(arg) || arg.equals("null")) {
                falg = true;
                return falg;
            }
        }
        return falg;
    }
}
```
这个工具类的优点很明显，一方面判断了字符串“null”，另一方面对参数个数无限制，只要有一个参数是空则返回true。
