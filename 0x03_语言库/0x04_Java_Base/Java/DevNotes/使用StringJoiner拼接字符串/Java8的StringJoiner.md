Java StringJoiner
<a name="myhn8"></a>
## 为什么会新增这样一个String辅助类？
原有的StringBuilder太死板，不支持分割，如果想让最终的字符串以逗号隔开，需要这样写
```java
StringBuilder sb = new StringBuilder();
IntStream.range(1,10).forEach(i->{
    sb.append(i+"");
    if( i < 10){
        sb.append(",")
    } 
});
```
是不是太死板了，不好用，StringJoiner怎样写呢？
```java
StringJoiner sj = new StringJoiner(",");
IntStream.range(1,10).forEach(i->sj.add(i+""));
```
有哪些平时用的还比较少的功能：

- `setEmptyValue`，默认情况下的`emptyValue`是前缀加后缀， 用户可自定义`emptyValue`
- `merge(StringJoiner other)`，合并另外一个joiner
- `length`，当前长度
<a name="gEcQu"></a>
## 自己怎么实现StringJoiner
维护一个List，最后`toString`的时候`join`一下就好了优势：实现非常方便缺点：list太浪费空间(扩容时都是按照系数扩容的)在`StringBuilder`基础上改造（jdk实现方式就是以组合的形式增强的`StringBuilder`）
<a name="w9BEI"></a>
## jdk实现的源码分析
<a name="vpTyy"></a>
### 成员变量
```java
private final String prefix;
private final String delimiter;
private final String suffix;

/*
     * StringBuilder value -- at any time, the characters constructed from the
     * prefix, the added element separated by the delimiter, but without the
     * suffix, so that we can more easily add elements without having to jigger
     * the suffix each time.
     */
private StringBuilder value;

/*
     * By default, the string consisting of prefix+suffix, returned by
     * toString(), or properties of value, when no elements have yet been added,
     * i.e. when it is empty.  This may be overridden by the user to be some
     * other value including the empty String.
     */
private String emptyValue;
```
其实从成员变量的注释里就能看出他们的作用和需要注意的点了。
<a name="tr4jW"></a>
### 构造函数
```java
public StringJoiner(CharSequence delimiter,
                    CharSequence prefix,
                    CharSequence suffix) {
    Objects.requireNonNull(prefix, "The prefix must not be null");
    Objects.requireNonNull(delimiter, "The delimiter must not be null");
    Objects.requireNonNull(suffix, "The suffix must not be null");
    // make defensive copies of arguments
    this.prefix = prefix.toString();
    this.delimiter = delimiter.toString();
    this.suffix = suffix.toString();
    // ！！！构造时就直接将emptyValue拼接好了。
    this.emptyValue = this.prefix + this.suffix;
}
```
为什么要一开始就构造好呢？如果想直接自定义`emptyValue`直接用构造函数初始化不是更方便。
<a name="VoNi1"></a>
### 添加元素
```java
public StringJoiner add(CharSequence newElement) {
    prepareBuilder().append(newElement);
    return this;
}

private StringBuilder prepareBuilder() {
    // 从构造函数和类变量的声明可以看出，没有添加元素前stringbuilder是没有初始化的
    if (value != null) {
        // 已经有元素存在的情况下，添加元素前先将分隔符添加进去
        value.append(delimiter);
    } else {
        // 没有元素存在的情况下先把前缀加进去
        value = new StringBuilder().append(prefix);
    }
    return value;
}
```
可以看出再添加元素的过程中就已经把前缀和分割字符什么的都处理好了，全部都在stringbuilde中了，唯一没有处理的就是后缀。为什么？这样做toString什么的时候真的超级方便。
<a name="vqwfY"></a>
### 关键的toString
```java
public String toString() {
    if (value == null) {
        // 这里如果没有自定义空值就是前缀+后缀咯。。
        return emptyValue;
    } else {
        // 为什么不直接value.toString()+suffix?????
        if (suffix.equals("")) {
            return value.toString();
        } else {

            int initialLength = value.length();
            String result = value.append(suffix).toString();
            // reset value to pre-append initialLength
            value.setLength(initialLength);
            return result;
        }
    }
}
```
为什么不直接`value.toString()+suffix`？答案在`merge`方法
<a name="ERc5u"></a>
### merge
```java
public StringJoiner merge(StringJoiner other) {
    Objects.requireNonNull(other);
    if (other.value != null) {
        final int length = other.value.length();
        // 下面这段注释是说避免merge(this)时受影响，为什么？
        // lock the length so that we can seize the data to be appended
        // before initiate copying to avoid interference, especially when
        // merge 'this'
        StringBuilder builder = prepareBuilder();
        builder.append(other.value, other.prefix.length(), length);
    }
    return this;
}

private StringBuilder prepareBuilder() {
    if (value != null) {
        value.append(delimiter);
    } else {
        value = new StringBuilder().append(prefix);
    }
    return value;
}
```
`merge`的思路是用当前的`striingBuilder`去append other的value（必须去掉前缀），源码注释中的`merge`'this'问题是什么呢？`prepareBuilder()`的时候可能会先`append(delimiter)`，如果other就是this，那么length其实就多了一个`delimiter`，此时`append`还是得以添加前的length为准。<br />`merge`的实现方式决定了`toString`时不能直接`value.append(suffix).toString()`，因为`builder.append(other.value, other.prefix.length(), length);`这行代码，默认加上`suffix`后这里的`merge`的length得减去suffix的length，而且`merge`时得把另外一个sj的内容append到当前这个sj的`suffix`之前。
<a name="HCsgk"></a>
### length
```java
public int length() {
    // Remember that we never actually append the suffix unless we return
    // the full (present) value or some sub-string or length of it, so that
    // we can add on more if we need to.
    return (value != null ? value.length() + suffix.length() :
            emptyValue.length());
}
```
length不只是`add`的元素的length，还有前后缀。
<a name="VELuk"></a>
## 总结
基于`StringBuilder`实现，add时就把`prefix`和分隔符给加上了，`suffix`永远都不加，知道`toString`和`length`调用时才加入计算。这样带来的`merge`操作实现的极大便利性！<br />`emptyValue`这个一定要构造时就生成吗？用户想有自己的默认值还需要先构造实例再注入吗。这个觉得还是有点奇怪。<br />Objects这个工具方法是返回的校验的值本身。
```java
public StringJoiner setEmptyValue(CharSequence emptyValue) {
    // 注意这个Objects.requireNonNull方法是return的第一个参数。。。
    this.emptyValue = Objects.requireNonNull(emptyValue,
                                             "The empty value must not be null").toString();
    return this;
}
```
