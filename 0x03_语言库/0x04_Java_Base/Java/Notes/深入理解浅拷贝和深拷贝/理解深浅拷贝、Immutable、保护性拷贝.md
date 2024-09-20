Java
<a name="eHCPS"></a>
## 一、对象拷贝
使用 `=` 的时候，其实是引用的拷贝。 多个引用指向的其实是同一个对象。<br />上面的例子中 `ArrayList<Integer> list = new ArrayList<>()`在堆内存中创建了ArrayList对象并且把list引用指向该对象的地址。<br />`ArrayList<Integer> list2 = list` 把list的引用赋值给list2，两个引用指向的都是上一步创建的对象。<br />对象拷贝分为深浅拷贝。
<a name="cyyHV"></a>
### 深浅拷贝的概念
对于基本类型来说拷贝只是值传递，拷贝后的对象和原对象的基本类型变量是相互独立的。 以下只讨论引用类型的情况。
<a name="jqAWP"></a>
#### 浅拷贝
如果是引用类型，拷贝的是引用类型的地址值，也就是和原对象的引用指向相同的一块内存区域。 这时候如果对象发生更改。 拷贝对象和原对象都会受到影响。
<a name="WmI54"></a>
#### 深拷贝
深拷贝将具有原始对象的所有字段的精确复制，就像浅复制一样。但是，另外，如果原始对象有任何对其他对象的引用作为字段，那么也可以通过调用这些对象上的`clone()`方法来创建这些对象的副本。这意味着克隆对象和原始对象将是100%不相交的。它们是100%相互独立的。对克隆对象所做的任何更改都不会反映在原始对象中，反之亦然。
<a name="m0EEG"></a>
### 实现拷贝的方法：

1. 继承Cloneable接口并重写`clone()`方法<br />如果是浅拷贝，只需要让外层的对象重写`clone()`方法。<br />如果要实现深拷贝，则需要逐层实现Cloneable接口实现`clone()`方法。
2. 拷贝构造器<br />最常用的方法，通过构造方法或者静态工厂方法来创建原对象的拷贝。
3. 通过序列化的方式<br />如果对象的嵌套层次很深，或者后续修改增加了一些字段，这时候维护`clone()`方法或者手动构造对象都很麻烦。<br />这时候可以考虑使用`Serializable`反序列化来构建一个新的对象。 反序列化出的对象和原对象内存地址是完全独立的，属于深拷贝。
<a name="zC4Y6"></a>
## 二、不可变类Immutable
上面提到了通过深拷贝可以创建和原对象互不影响的拷贝，但是维护起来非常麻烦。<br />Java提供了另外一种方式来保证这种独立性， 他在被创建后其内部状态就不能被修改， 也称作Immutable对象。 JDK中的Immutable对象包括String、基本类型的包装类(Integer，Double，Float…)、BigDecimal，BigInteger。<br />一个Immutable类想要维持不可变性， 需要遵循以下规则：
<a name="ErTGx"></a>
### 类用`final`修饰 或者 私有构造器
不管是`final`修饰类还是私有构造器，都是为了防止被继承。<br />如果不可变类能被继承，由于父类引用指向子类的实例时，很明显没法约束每个子类的不可变性，那么父类的不可变性就会遭到破坏。
<a name="fhEF7"></a>
### 类中的属性声明为`private`，并且不对外界提供`setter`方法
从访问级别上控制不可变性。
<a name="nKK2a"></a>
### 类中的属性声明为`final`
如果属性是基本类型，那么声明为`final`后就不能改变。<br />如果属性是引用类型， `final`只是声明这个对象的引用不能改变， 注意对象的属性还是可以改变的。所以有第四点来补充
<a name="hHm68"></a>
### 如果类中存在可变类的属性，当访问他的时候需要进行保护性拷贝。
如果类中存在可变类的变量，虽然已经对他加上了`final`修饰符，但这仅仅表示这个变量的引用不能指向别的地址。 但是还是可以通过可变属性的引用来修改他可变类内部的属性，从而破坏可变类对象调用者的不可变性。<br />在构造器， 访问方法， 和序列化的`readObject`方法中，如果用到了这个可变对象的变量， 需要对他进行保护性拷贝， 避免通过可变的引用影响到他的调用者。
<a name="uMFGg"></a>
## 三、保护性拷贝
在构造器，`getter`方法，序列化的`readObject`方法(隐式构造器)中，进行保护性拷贝(defensive copies)来返回对象的拷贝 而不是 对象本身。<br />看EffectiveJava中的例子，Period是一个描述日期的类，他的构造方法进行了参数合法性检查start < end
```java
// Broken "immutable" time period class
public final class Period {
    private final Date start;
    private final Date end;

    /**
     * @param  start the beginning of the period
     * @param  end the end of the period; must not precede start
     * @throws IllegalArgumentException if start is after end
     * @throws NullPointerException if start or end is null
     */
    public Period(Date start, Date end) {
        if (start.compareTo(end) > 0)
            throw new IllegalArgumentException(
                start + " after " + end);

        this.start = start;
        this.end   = end;
    }

    public Date start() {
        return start;
    }

    public Date end() {
        return end;
    }
    ...    // Remainder omitted
}
```
<a name="R2H02"></a>
### 构造器
在构造方法执行后，由于Date是不可变对象，可以引用start,end所指向的变量进行外界的修改。
```java
// Attack the internals of a Period instance
Date start = new Date();
Date end = new Date();
Period p = new Period(start, end);
end.setYear(78);  // Modifies internals of p!
```
显然这里不希望在对象初始化后受到外界的影响来破坏start < end的约束<br />进行保护性拷贝后，直接使用参数构建一个新的对象，这样外部的修改根本不会影响到新的对象。
```java
// Repaired constructor - makes defensive copies of parameters
public Period(Date start, Date end) {
    this.start = new Date(start.getTime());
    this.end   = new Date(end.getTime());

    if (this.start.compareTo(this.end) > 0)
      throw new IllegalArgumentException(
          this.start + " after " + this.end);
}
```
<a name="APZkL"></a>
### `getter`方法
同理，对于`getter`方法暴露宿主类内部可变对象的引用时，也要进行保护性拷贝防止外部通过引用来修改，影响到宿主类。
```java
// Repaired accessors - make defensive copies of internal fields
public Date start() {
    return new Date(start.getTime());
}

public Date end() {
    return new Date(end.getTime());
}
```
<a name="t0Odx"></a>
### `readObject()`
如果对象实现了`Serializable`，在反序列化`readObject`方法中，序列化反序列化是通过流的方式进行的，攻击者可以伪造一个流来修改对象内可变参数，对于这些可变参数也要进行保护性拷贝。
```java
// readObject method with defensive copying and validity checking
private void readObject(ObjectInputStream s)
            throws IOException, ClassNotFoundException {
    s.defaultReadObject();
    // Defensively copy our mutable components
    start = new Date(start.getTime());
    end = new Date(end.getTime());
    // Check that our invariants are satisfied
    if (start.compareTo(end) > 0)
    throw new InvalidObjectException(start +" after "+ end);
}
```
<a name="W9aQo"></a>
### 注意点：

1. 保护拷贝不止针对不可变类，对于可变类，只要他内部的可变对象暴露的引用可能会影响到他的内部稳定性，就需要对他的这个可变对象进行保护性拷贝。
2. 不管是可变类还是不可变类，只要有可能，应该尽量使用不可变类型和基本数据类型作为类的属性，基本类型是不存在拷贝的概念完全独立的，而Immutable对象在初始化后就不会发生改变，不需要也不应该对他做拷贝处理。
3. 拷贝一个对象时，最好使用构造器或者静态工厂来进行拷贝，而不是调用它的`clone()`方法(因为对于不可变类可能是没有final修饰的，他的子类可能会重写`clone()`方法)
4. 保护性拷贝是视情况而定的，如果不需要保持对象内部的稳定性，那么不需要对暴露的可变对象属性做处理。
<a name="fhT0x"></a>
## 四、不可变类的优缺点
<a name="dzu8Y"></a>
### 优点
<a name="FMjFQ"></a>
#### 1. 安全性高，不可变类的对象被声明后就不能改变
不可变对象作为属性被别的对象使用后，对于调用者来说无需担心赋值后，不可变类的后续修改会影响到调用者。<br />要注意这里是：不可变类对象内容不能修改，但并不代表其引用不能改变。举个例子
```java
static class StringW{
    private String value;
    public StringW(String value) {
        this.value = value;
    }
}

private static void test2() {
    //修改不可变类String的值
    String strKey = "key";
    String strValue = "value";
    HashMap<String, String> map = new HashMap<>();
    map.put(strKey, strValue);
    HashMap<String, StringW>  maps = new HashMap<>();
    StringW strwValue = new StringW("value");
    maps.put(strKey, strwValue);

    //修改不可变类的值
    strValue = "value1111";
    System.out.println(map.get("key"));
    //修改可变类的值
    strwValue.value = "value11111";
    System.out.println(maps.get(strKey).value);
}
```
打印结果
```java
value

value11111
```
可以看到String的修改并没有影响到他的调用者，而自定义的可变类StringW的修改影响到了他的调用者。<br />这是因为Java中说的对象分为 对象开辟的内存 和 指向该内存地址的引用两部分。

- 不可变对象在声明赋值后，后续的修改并不是在原对象上进行的，而是直接断开指向原对象内存的引用，重新在堆区新建一个对象并指向新的对象内存地址。而原内存的内容不会受到影响。
- 而可变对象的修改则是在原对象上进行的，只要指向可变对象的引用都会受到影响。
<a name="KU4lL"></a>
#### 2. 线程安全
不可变类型的对象在创建后就不会被修改，所以不需要考虑多线程下对象的读写造成的同步问题，他是线程安全的。
<a name="EIECC"></a>
### 缺点
由于不可变类创建后就不能改变的特性，在频繁改变值的场景下，不可变类的引用需要不断的断开与原来对象内存的链接，并指向新的对象内存区。最明显的就是String类，每修改一次String，就会在内存创建一个新的String对象，原有的就会被丢弃。<br />例如
```java
String string = "a";
string = string + "b";
string = string + "c";
string = "3"+string;
```
中间的过程内存里会创建"a","b","ab","c","abc"等大量的对象，很明显只关心最后的结果，无需开辟这么多的内存空间，尤其是在移动端上。可以使用StringBuilder来直接对一块内存进行修改。<br />Java中也针对这种情况做了优化，String类有位于方法区的常量池保存这些创建过的字符串变量，这个方法区被所有的线程共享。<br />而Byte, Short, Integer, Long, Character, Boolean, Float, Double, 除Float和Double以外，其它六种都实现了常量池，但是它们只在大于等于-128并且小于等于127时才使用常量池。以Character为例，调用`valueOf(char c)`创建对象的时候会优先取静态内部类CharacterCache缓存的值。
```java
public static Character valueOf(char c) {
	if (c <= 127) { // must cache
		return CharacterCache.cache[(int)c];
	}
	return new Character(c);
}

private static class CharacterCache {
	private CharacterCache(){}

	static final Character cache[] = new Character[127 + 1];

	static {
		for (int i = 0; i < cache.length; i++)
			cache[i] = new Character((char)i);
	}
}
```
<a name="Z8hdA"></a>
## 五、破坏不可变性的方法
通过反射可以绕过不可变类的限制，从而修改他内部的属性来破坏不可变性。
```java
String str = "12345";
//获取String类中的value字段
Field valueFieldOfString = String.class.getDeclaredField("value");
//改变value属性的访问权限
valueFieldOfString.setAccessible(true);
//获取s对象上的value属性的值
char[] value = (char[]) valueFieldOfString.get(str);
//修改数组末位
value[4] = '0';  
System.out.println("str = " + str);
```
输出结果
```java
str = 12340
```
可见通过反射，可以破坏不可变类的不可变性。
<a name="YBMhR"></a>
## 总结

- 深拷贝浅拷贝取决于 原对象和拷贝对象是否完全独立，都可以通过覆盖`clone()`方法或者手动构造对象来实现。
- 对于基本数据类型，不存在拷贝的概念，他们会重新开辟一块内存。
- Immutable类不需要进行拷贝操作，他们本身就是不可变的。<br />如果Immutable对象的引用指向了一个新对象，那么他会断开和原对象的引用链再指向新的对象，这时指向原对象的其他引用是不会受到影响的。
- 对于对象内的可变类型参数，如果对外暴露了可变类型参数的引用，需要视情况进行保护性拷贝来返回可变类型对象的拷贝而不是对象本身。
