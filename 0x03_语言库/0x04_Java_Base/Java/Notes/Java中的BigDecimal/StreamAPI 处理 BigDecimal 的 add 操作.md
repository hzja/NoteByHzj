Java Stream BigDecimal
<a name="G5ucq"></a>
## 1、引言
通常使用Java  Stream API  来处理数据集合。<br />一个不错的特性是支持对数字流的操作，比如_sum_操作。但是，**不能以这种方式处理所有数值类型**，那么如何对诸如_BigDecimal_之类的数字流执行_sum_操作。
<a name="5wDxz"></a>
## 2、如何用流求和
Stream API 提供数字流 numbers Stream，包括 _IntStream、DoubleStream、和LongStream_通过创建一个数字流来来了解它们是如何工作的。然后，用 _[IntStream#sum]_ 计算它的总数：
```java
IntStream intNumbers = IntStream.range(0, 3);
assertEquals(3, intNumbers.sum());
```
可以从_Double_s的列表开始执行类似的操作。通过使用streams，可以使用 _mapToDouble_ 将对象流转换为_Double stream_：
```java
List<Double> doubleNumbers = Arrays.asList(23.48, 52.26, 13.5);
double result = doubleNumbers.stream()
    .mapToDouble(Double::doubleValue)
    .sum();
assertEquals(89.24, result, .1);
```
所以，如果能用同样的方法总结出一组 _BigDecimal_ 数字，那就很有用了。<br />不幸的是，没有BigDecimalStream。所以需要另一个解决方案。
<a name="flye7"></a>
## 3、使用Reduce添加_BigDecimal_数字
使用 Stream.reduce 来计算sum：
```java
Stream<Integer> intNumbers = Stream.of(5, 1, 100);
int result = intNumbers.reduce(0, Integer::sum);
assertEquals(106, result);
```
**这适用于任何可以逻辑相加的数据**，包括_BigDecimal_：
```java
Stream<BigDecimal> bigDecimalNumber = 
  Stream.of(BigDecimal.ZERO, BigDecimal.ONE, BigDecimal.TEN);
BigDecimal result = bigDecimalNumber.reduce(BigDecimal.ZERO, BigDecimal::add);
assertEquals(11, result);
```
_reduce_方法有两个参数：

- _Identity_ – 等于_0_–它是还原的起始值
- _Accumulator function_ – 接受两个参数，目前为止的结果，以及流的下一个元素
