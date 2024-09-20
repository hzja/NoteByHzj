JavaSpring Security<br />`HttpSecurity`包含了一个成员变量`FilterOrderRegistration`，这个类是一个内置过滤器注册表。至于这些过滤器的作用，不是本文介绍的重点，有兴趣可以去看看`FilterOrderRegistration`的源码。
<a name="djRZT"></a>
## 内置过滤器的顺序
`FilterOrderRegistration`维护了一个变量`filterToOrder`，它记录了类之间的顺序和上下之间的间隔步长。复制了一个`FilterOrderRegistration`来直观感受一下过滤器的顺序：
```java
CopyFilterOrderRegistration filterOrderRegistration = new CopyFilterOrderRegistration();
// 获取内置过滤器  此方法并未提供
Map<String, Integer> filterToOrder = filterOrderRegistration.getFilterToOrder();
TreeMap<Integer, String> orderToFilter = new TreeMap<>();
filterToOrder.forEach((name, order) -> orderToFilter.put(order,name));
orderToFilter.forEach((order,name) -> System.out.println(" 顺序：" + order+" 类名：" + name ));
```
打印结果：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650463359863-f2168695-4ea6-4b7a-a73d-2a1ba31e8603.png#clientId=uc7a299fd-0649-4&from=paste&id=ub3d5ed2f&originHeight=840&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u36875202-3c2b-4b95-93e2-6f5f400b642&title=)<br />可以看得出内置过滤器之间的位置是相对固定的，除了第一个跟第二个步长为200外，其它步长为100。<br />内置过滤器并非一定会生效，仅仅是预置了它们的排位，需要通过`HttpSecurity`的`addFilterXXXX`系列方法显式添加才行。
<a name="piHdW"></a>
## 注册过滤器的逻辑
`FilterOrderRegistration`提供了一个`put`方法：
```java
void put(Class<? extends Filter> filter, int position) {
	String className = filter.getName();
	// 如果这个类已经注册就忽略
	if (this.filterToOrder.containsKey(className)) {
		return;
	}
	// 如果没有注册就注册顺序。
	this.filterToOrder.put(className, position);
 }
```
从这个方法可以得到几个结论：

- 内置的34个过滤器是有固定序号的，不可被改变。
- 新加入的过滤器的类全限定名是不能和内置过滤器重复的。
- 新加入的过滤器的顺序是可以和内置过滤器的顺序重复的。
<a name="lDKtf"></a>
## 获取已注册过滤器的顺序值
`FilterOrderRegistration`还提供了一个`getOrder`方法：
```java
Integer getOrder(Class<?> clazz) {
	// 如果类Class 或者 父类Class 名为空就返回null
	while (clazz != null) {
		Integer result = this.filterToOrder.get(clazz.getName());
		// 如果获取到顺序值就返回
		if (result != null) {
			return result;
		}
		// 否则尝试去获取父类的顺序值
		clazz = clazz.getSuperclass();
	}
	return null;
}
```
<a name="sAXjJ"></a>
## `HttpSecurity`维护过滤器的方法
接下来分析一下`HttpSecurity`维护过滤器的几个方法。
<a name="gr2Wk"></a>
### `addFilterAtOffsetOf`
`addFilterAtOffsetOf`是一个`HttpSecurity`的内置私有方法。`Filter`是想要注册到`DefaultSecurityFilterChain`中的过滤器，`offset`是向右的偏移值，`registeredFilter`是已经注册到`FilterOrderRegistration`的过滤器，而且`registeredFilter`没有注册的话会空指针。
```java
private HttpSecurity addFilterAtOffsetOf(Filter filter, int offset, Class<? extends Filter> registeredFilter) {
	// 首先会根据registeredFilter的顺序和偏移值来计算filter的
	int order = this.filterOrders.getOrder(registeredFilter) + offset;
	// filter添加到集合中待排序
	this.filters.add(new OrderedFilter(filter, order));
	// filter注册到 FilterOrderRegistration
	this.filterOrders.put(filter.getClass(), order);
	return this;
 }
```
务必记着`registeredFilter`一定是已注册入`FilterOrderRegistration`的`Filter`。
<a name="kq8x5"></a>
### `addFilter`系列方法
这里以`addFilterAfter`为例。
```java
@Override
public HttpSecurity addFilterAfter(Filter filter, Class<? extends Filter> afterFilter) {
	return addFilterAtOffsetOf(filter, 1, afterFilter);
 }
```
`addFilterAfter`是将`filter`的位置置于`afterFilter`后一位，假如`afterFilter`顺序值为400，则`filter`顺序值为401。`addFilterBefore`和`addFilterAt`逻辑和`addFilterAfter`仅仅是偏移值的区别，这里不再赘述。<br />`addFilter`的方法比较特殊：
```java
@Override
public HttpSecurity addFilter(Filter filter) {
	Integer order = this.filterOrders.getOrder(filter.getClass());
	if (order == null) {
		throw new IllegalArgumentException("The Filter class " + filter.getClass().getName()
										   + " does not have a registered order and cannot be added without a specified order. Consider using addFilterBefore or addFilterAfter instead.");
	}
	this.filters.add(new OrderedFilter(filter, order));
	return this;
 }
```
`filter`必须是已经注册到`FilterOrderRegistration`的`Filter`，这意味着它可能是内置的`Filter`，也可能是先前通过`addFilterBefore`、`addFilterAt`或者`addFilterAfter`注册的非内置`Filter`。
<a name="HD6K6"></a>
## 问题来了
之前看到一个问题，如果`HttpSecurity`注册两个重复序号的`Filter`会是怎么样的顺序？先来看下排序的机制：
```java
// filters
private List<OrderedFilter> filters = new ArrayList<>(); 
//排序
this.filters.sort(OrderComparator.INSTANCE);
```
看了下`OrderComparator`源码，其实还是通过**order**数字的自然排序，数字越小越靠前。如果**order**数字相同，索引越小越靠前。也就是同样的序号，谁先`add`到`filters`谁就越靠前(**先add到List中的索引肯定越小**)。
