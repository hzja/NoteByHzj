Java<br />在Java开发中，字符串处理是一个常见的操作，而org.apache.commons.lang3.StringUtils工具类提供了一系列强大而实用的方法，其中的`isEmpty`和`isBlank`系列方法是常用的工具。应当合理的利用起来，让代码看起来更优雅。<br />除了`isEmpty`/`isNotEmpty`/`isNotBlank`/`isBlank`外，还有`isAnyEmpty`/`isNoneEmpty`/`isAnyBlank`/`isNoneBlank`的存在，来探索`org.apache.commons.lang3.StringUtils;`这个工具类。
<a name="JEdmP"></a>
## isEmpty系列
<a name="wjI06"></a>
### `StringUtils.isEmpty()`
这个方法用于判断`CharSequence`是否为空，即为null或长度为0。方法的实现十分简洁，通过对`CharSequence`的null检查和长度检查来确定是否为空。
```java
public static boolean isEmpty(final CharSequence cs) {
    return cs == null || cs.length() == 0;
}
```
是否为空。可以看到 `" "` 空格是会绕过这种空判断，因为是一个空格，并不是严格的空值，会导致 `isEmpty(" ")=false`
```java
StringUtils.isEmpty(null) = true
StringUtils.isEmpty("") = true
StringUtils.isEmpty(" ") = false
StringUtils.isEmpty("bob") = false
StringUtils.isEmpty(" bob ") = false

/**
 *
 * <p>NOTE: This method changed in Lang version 2.0.
 * It no longer trims the CharSequence.
 * That functionality is available in isBlank().</p>
 *
 * @param cs  the CharSequence to check, may be null
 * @return {@code true} if the CharSequence is empty or null
 * @since 3.0 Changed signature from isEmpty(String) to isEmpty(CharSequence)
 */
public static boolean isEmpty(final CharSequence cs) {
    return cs == null || cs.length() == 0;
}
```
<a name="HeoKH"></a>
### `StringUtils.isNotEmpty()`
相当于不为空，`= !isEmpty()`，与`isEmpty()`相反，该方法用于判断`CharSequence`是否不为空。这是一个实用的方法，通过调用`isEmpty()`的结果的取反来实现。
```java
public static boolean isNotEmpty(final CharSequence cs) {
    return !isEmpty(cs);
}
```
<a name="P7KFz"></a>
### `StringUtils.isAnyEmpty()`
该方法用于判断一组`CharSequence`中是否至少有一个为空。通过遍历传入的`CharSequence`数组，检查每个元素是否为空，如果存在任何一个为空，则返回true。
```java
public static boolean isAnyEmpty(final CharSequence... css) {
    if (ArrayUtils.isEmpty(css)) {
        return true;
    }
    for (final CharSequence cs : css){
        if (isEmpty(cs)) {
            return true;
        }
    }
    return false;
}
```
是否有一个为空，只有一个为空，就为`true`。
```java
StringUtils.isAnyEmpty(null) = true
StringUtils.isAnyEmpty(null, "foo") = true
StringUtils.isAnyEmpty("", "bar") = true
StringUtils.isAnyEmpty("bob", "") = true
StringUtils.isAnyEmpty(" bob ", null) = true
StringUtils.isAnyEmpty(" ", "bar") = false
StringUtils.isAnyEmpty("foo", "bar") = false

/**
 * @param css  the CharSequences to check, may be null or empty
 * @return {@code true} if any of the CharSequences are empty or null
 * @since 3.2
 */
public static boolean isAnyEmpty(final CharSequence... css) {
  if (ArrayUtils.isEmpty(css)) {
    return true;
  }
  for (final CharSequence cs : css){
    if (isEmpty(cs)) {
      return true;
    }
  }
  return false;
}
```
<a name="dwbtZ"></a>
### `StringUtils.isNoneEmpty()`
该方法与`isAnyEmpty()`相反，用于判断一组`CharSequence`是否全部不为空。通过调用`isAnyEmpty()`的结果的取反来实现。
```java
public static boolean isNoneEmpty(final CharSequence... css) {
    return !isAnyEmpty(css);
}
```
相当于`!isAnyEmpty(css)`，必须所有的值都不为空才返回true
```java
/**
 * <p>Checks if none of the CharSequences are empty ("") or null.</p>
 *
 * <pre>
 * StringUtils.isNoneEmpty(null)             = false
 * StringUtils.isNoneEmpty(null, "foo")      = false
 * StringUtils.isNoneEmpty("", "bar")        = false
 * StringUtils.isNoneEmpty("bob", "")        = false
 * StringUtils.isNoneEmpty("  bob  ", null)  = false
 * StringUtils.isNoneEmpty(" ", "bar")       = true
 * StringUtils.isNoneEmpty("foo", "bar")     = true
 * </pre>
 *
 * @param css  the CharSequences to check, may be null or empty
 * @return {@code true} if none of the CharSequences are empty or null
 * @since 3.2
 */
public static boolean isNoneEmpty(final CharSequence... css) {}
```
<a name="zc35r"></a>
## `isBank`系列
<a name="kVCoY"></a>
### `StringUtils.isBlank()`
`isBlank()`方法用于判断`CharSequence`是否为空白，包括null、空字符串或仅包含空格的字符串。通过遍历`CharSequence`中的每个字符，检查是否存在非空格字符，从而判断是否为空白。
```java
public static boolean isBlank(final CharSequence cs) {
    final int strLen = length(cs);
    if (strLen == 0) {
        return true;
    }
    for (int i = 0; i < strLen; i++) {
        if (!Character.isWhitespace(cs.charAt(i))) {
            return false;
        }
    }
    return true;
}
```
是否为真空值(空格或者空值)
```java
StringUtils.isBlank(null) = true
StringUtils.isBlank("") = true
StringUtils.isBlank(" ") = true
StringUtils.isBlank("bob") = false
StringUtils.isBlank(" bob ") = false

/**
 * <p>Checks if a CharSequence is whitespace, empty ("") or null.</p>
 * @param cs  the CharSequence to check, may be null
 * @return {@code true} if the CharSequence is null, empty or whitespace
 * @since 2.0
 * @since 3.0 Changed signature from isBlank(String) to isBlank(CharSequence)
 */
public static boolean isBlank(final CharSequence cs) {
    int strLen;
    if (cs == null || (strLen = cs.length()) == 0) {
        return true;
    }
    for (int i = 0; i < strLen; i++) {
        if (Character.isWhitespace(cs.charAt(i)) == false) {
            return false;
        }
    }
    return true;
}
```
<a name="rKnqA"></a>
### `StringUtils.isNotBlank()`
是否真的不为空，不是空格或者空值，与`isBlank()`相反，`isNotBlank()`方法用于判断`CharSequence`是否不为空白。通过调用`isBlank()`的结果的取反来实现，相当于`!isBlank();`
```java
public static boolean isNotBlank(final CharSequence cs) {
    return !isBlank(cs);
}
```
<a name="oQJTV"></a>
### `StringUtils.isAnyBlank()`
该方法用于判断一组`CharSequence`中是否至少有一个为空白。通过遍历传入的`CharSequence`数组，检查每个元素是否为空白，如果存在任何一个为空白，则返回true。
```java
public static boolean isAnyBlank(final CharSequence... css) {
    if (ArrayUtils.isEmpty(css)) {
        return true;
    }
    for (final CharSequence cs : css){
        if (isBlank(cs)) {
            return true;
        }
    }
    return false;
}
```
是否包含任何真空值(包含空格或空值)
```java
StringUtils.isAnyBlank(null) = true
StringUtils.isAnyBlank(null, "foo") = true
StringUtils.isAnyBlank(null, null) = true
StringUtils.isAnyBlank("", "bar") = true
StringUtils.isAnyBlank("bob", "") = true
StringUtils.isAnyBlank(" bob ", null) = true
StringUtils.isAnyBlank(" ", "bar") = true
StringUtils.isAnyBlank("foo", "bar") = false

 /**
 * <p>Checks if any one of the CharSequences are blank ("") or null and not whitespace only..</p>
 * @param css  the CharSequences to check, may be null or empty
 * @return {@code true} if any of the CharSequences are blank or null or whitespace only
 * @since 3.2
 */
public static boolean isAnyBlank(final CharSequence... css) {
  if (ArrayUtils.isEmpty(css)) {
    return true;
  }
  for (final CharSequence cs : css){
    if (isBlank(cs)) {
      return true;
    }
  }
  return false;
}
```
<a name="yXC5k"></a>
### `StringUtils.isNoneBlank()`
是否全部都不包含空值或空格，该方法与`isAnyBlank()`相反，用于判断一组`CharSequence`是否全部不为空白。通过调用`isAnyBlank()`的结果的取反来实现。
```java
StringUtils.isNoneBlank(null) = false
StringUtils.isNoneBlank(null, "foo") = false
StringUtils.isNoneBlank(null, null) = false
StringUtils.isNoneBlank("", "bar") = false
StringUtils.isNoneBlank("bob", "") = false
StringUtils.isNoneBlank(" bob ", null) = false
StringUtils.isNoneBlank(" ", "bar") = false
StringUtils.isNoneBlank("foo", "bar") = true

/**
 * <p>Checks if none of the CharSequences are blank ("") or null and whitespace only..</p>
 * @param css  the CharSequences to check, may be null or empty
 * @return {@code true} if none of the CharSequences are blank or null or whitespace only
 * @since 3.2
 */
public static boolean isNoneBlank(final CharSequence... css) {
  return !isAnyBlank(css);
}
```
<a name="X2raF"></a>
## StringUtils的其他方法
可以参考官方的文档，里面有详细的描述，有些方法还是很好用的。<br />[https://commons.apache.org/proper/commons-lang/apidocs/org/apache/commons/lang3/StringUtils.html](https://commons.apache.org/proper/commons-lang/apidocs/org/apache/commons/lang3/StringUtils.html)<br />![2021-08-20-23-49-18-700000.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1629474752940-b1b8ce6b-e5c9-489e-932d-ec71e7f82dda.png#averageHue=%23f7f6f6&clientId=u5aaabe6e-feef-4&from=ui&id=u21bc46a2&originHeight=662&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=210817&status=done&style=none&taskId=ua1642036-1975-432f-9df2-058d1d7da59&title=)![2021-08-20-23-49-18-875003.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1629474752890-3453596c-57b4-4795-a67b-8aa3797e59d5.png#averageHue=%23f6f5f4&clientId=u5aaabe6e-feef-4&from=ui&id=u5ef06fba&originHeight=717&originWidth=965&originalType=binary&ratio=1&rotation=0&showTitle=false&size=34943&status=done&style=none&taskId=u2e8673f6-3bfe-4601-b2dd-9beaacf006c&title=)
