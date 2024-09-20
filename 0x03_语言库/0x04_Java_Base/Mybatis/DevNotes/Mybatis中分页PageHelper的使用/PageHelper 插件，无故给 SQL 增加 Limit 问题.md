Java Mybatis PageHelper<br />在项目中，使用了 MyBatis 的 PageHelper 插件，发现了一个奇怪的问题，经常会给SQL无缘无故的增加Limit语句，经过调查，发现原因是没有安全的使用PageHelper插件，先来看一个例子：
```java
PageHelper.startPage(pageable.getPageNumber(), pageable.getPageSize());
PageInfolist = null;
if (condition.getCustomerIdListForUser() != null && condition.getCustomerIdListForUser().size() > 0) {
    list = new PageInfo<>(orderMapper.findOrderList(condition)); //分页查询语句
} else {
    list = new PageInfo<>(new ArrayList());
    list.setPageNum(0);
    list.setPageSize(1);
    list.setTotal(0);
}
```
在例子中，`PageHelper.startPage`就属于明显的不安全调用，因为`PageHelper`的原理是，在`PageHelper.startPage`调用时，会给全局对象`LOCAL_PAGE`设值，然后通过拦截器拦截每个SQL语句，如果`LOCAL_PAGE`有值，则给该SQL增加Limit，并调用`clearPage`方法清除`LOCAL_PAGE`的值；<br />但是上面的代码，其分页查询语句有可能因为if的条件的不满足没有执行，所以在程序执行结束时，`PageHelper.startPage`已经执行，`LOCAL_PAGE`的值已经设置<br />当线程池再次分配该线程执行其他程序时，可能会在该程序的第一个SQL上增加了Limit语句。<br />解决该问题的方法是，要绝对保证`PageHelper.startPage`和分页查询语句之间不要有任何其他语句，或者在程序结束时增加`PageHelper.clearPage();`的调用，例：
```java
PageInfolist = null;
if (condition.getCustomerIdListForUser() != null && condition.getCustomerIdListForUser().size() > 0) {
      PageHelper.startPage(pageable.getPageNumber(), pageable.getPageSize());
     list = new PageInfo<>(orderMapper.findOrderList(condition));//分页查询语句
} else {
    list = new PageInfo<>(new ArrayList());
    list.setPageNum(0);
    list.setPageSize(1);
    list.setTotal(0);
}
```
或
```java
PageHelper.startPage(pageable.getPageNumber(), pageable.getPageSize());
PageInfolist = null;
if (condition.getCustomerIdListForUser() != null && condition.getCustomerIdListForUser().size() > 0) {
     list = new PageInfo<>(orderMapper.findOrderList(condition));
} else {
     list = new PageInfo<>(new ArrayList());
     list.setPageNum(0);
     list.setPageSize(1);
     list.setTotal(0);
}
PageHelper.clearPage();
```
另外也可以这样
```java
PageHelper.startPage(pageable.getPageNumber(), pageable.getPageSize());
PageInfolist = null;
if (condition.getCustomerIdListForUser() != null && condition.getCustomerIdListForUser().size() > 0) {
    try{
        list = new PageInfo<>(orderMapper.findOrderList(condition));
    }finally{
        PageHelper.clearPage();
    }
} else {
     list = new PageInfo<>(new ArrayList());
     list.setPageNum(0);
     list.setPageSize(1);
     list.setTotal(0);
}
```
但是这样写有点冗余，而且没有必要。<br />**官网的解释 ：**<br />PageHelper 方法使用了静态的 ThreadLocal 参数，分页参数和线程是绑定的。只要可以保证在 PageHelper 方法调用后紧跟 MyBatis 查询方法，这就是安全的。因为 PageHelper 在 `finally` 代码段中自动清除了 ThreadLocal 存储的对象。如果代码在进入 Executor 前发生异常，就会导致线程不可用，这属于人为的 Bug（例如，接口方法和 XML 中的不匹配，导致找不到 `MappedStatement` 时）， 这种情况由于线程不可用，也不会导致 ThreadLocal 参数被错误的使用。
