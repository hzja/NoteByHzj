在Java 21中，除了推出很多新特性之外，一些Bug修复，也需要注意一下。因为这些改变可能在升级的时候，造成影响。
<a name="aN1xl"></a>
## `Double.toString()`和`Float.toString()`的精度问题修复
比如：对于`Double.String(1e23)`：

- 在Java 19后，输出内容为：1.0E23
- 在Java 18中，输出内容为：9.999999999999999E22
<a name="rK6Bt"></a>
## `IdentityHashMap`的`remove(key, value)`和`replace(key, value, newValue)`的错误处理
`IdentityHashMap`是Java中自带的一个集合，是一个比较特殊的Map实现。它使用的是恒等比较（而不是`equals`方法）来判断两个键是否相等。<br />这意味着，对于同一个对象的不同引用，`IdentityHashMap`会将它们视为不同的键，而其他Map实现类则会将它们视为相同的键。<br />在Java 20之前（不含Java 20）`IdentityHashMap`中的`remove(key, value)`和`replace(key, value, newValue)`方法，错误地将值参数与映射中的值进行了equals比较，这使得`remove(key, value)`和`replace(key, value, newValue)`方法执行结果不如预期。<br />如下例子所示，Java 20之前是不对的，但Java 20之后可以正确实现了。
```java
var users = new IdentityHashMap<String, User>();
String key = "abc";
users.put(key, new User("Jane Doe"));
var removed = users.remove(key, new User("Jane Doe"));
assert !removed;
```
