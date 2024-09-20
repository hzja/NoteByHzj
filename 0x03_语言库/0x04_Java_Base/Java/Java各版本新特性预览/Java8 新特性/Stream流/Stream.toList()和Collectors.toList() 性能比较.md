JavaStream<br />Java 16中的Stream增强，可以直接通过`toList()`来转换成List。<br />主要涉及下面这几种转换方式：
```java
list.stream().toList();
list.stream().collect(Collectors.toList());
list.stream().collect(Collectors.toUnmodifiableList());
```
那么`Stream.toList()`和`Collectors.toList()`的区别是什么？哪个性能好？<br />处理结果的区别：

- `Stream.toList()`返回的List是不可变List，不能增删改
- `Collectors.toList()`返回的是个普通的List，可以增删改
- `Collectors.toUnmodifiableList()`返回的List是不可变List，不能增删改

而至于性能的话，来测试一下，看看哪个性能更好。
```java
@BenchmarkMode(Mode.All)
@Fork(1)
@State(Scope.Thread)
@Warmup(iterations = 20, time = 1, batchSize = 10000)
@Measurement(iterations = 20, time = 1, batchSize = 10000)
public class BenchmarkStreamToList {

    @Benchmark
    public List<Integer> streamToList() {
        return IntStream.range(1, 1000).boxed().toList();
    }

    @Benchmark
    public List<Integer> collectorsToList() {
        return IntStream.range(1, 1000).boxed().collect(Collectors.toList());
    }

    @Benchmark
    public List<Integer> streamToList() {
        return IntStream.range(1, 1000).boxed().toList();
    }

}
```
结果报告：
```java
Benchmark                                                                                  Mode  Cnt   Score    Error  Units
BenchmarkStreamToList.collectorsToList                                                    thrpt   20  24.422 ±  0.268  ops/s
BenchmarkStreamToList.collectorsToUnmodifiableList                                        thrpt   20  22.784 ±  0.599  ops/s
BenchmarkStreamToList.streamToList                                                        thrpt   20  31.779 ±  1.732  ops/s
BenchmarkStreamToList.collectorsToList                                                     avgt   20   0.045 ±  0.006   s/op
BenchmarkStreamToList.collectorsToUnmodifiableList                                         avgt   20   0.062 ±  0.035   s/op
BenchmarkStreamToList.streamToList                                                         avgt   20   0.040 ±  0.028   s/op
BenchmarkStreamToList.collectorsToList                                                   sample  445   0.046 ±  0.002   s/op
BenchmarkStreamToList.collectorsToList:collectorsToList·p0.00                            sample        0.039            s/op
BenchmarkStreamToList.collectorsToList:collectorsToList·p0.50                            sample        0.041            s/op
BenchmarkStreamToList.collectorsToList:collectorsToList·p0.90                            sample        0.057            s/op
BenchmarkStreamToList.collectorsToList:collectorsToList·p0.95                            sample        0.073            s/op
BenchmarkStreamToList.collectorsToList:collectorsToList·p0.99                            sample        0.102            s/op
BenchmarkStreamToList.collectorsToList:collectorsToList·p0.999                           sample        0.150            s/op
BenchmarkStreamToList.collectorsToList:collectorsToList·p0.9999                          sample        0.150            s/op
BenchmarkStreamToList.collectorsToList:collectorsToList·p1.00                            sample        0.150            s/op
BenchmarkStreamToList.collectorsToUnmodifiableList                                       sample  460   0.044 ±  0.001   s/op
BenchmarkStreamToList.collectorsToUnmodifiableList:collectorsToUnmodifiableList·p0.00    sample        0.042            s/op
BenchmarkStreamToList.collectorsToUnmodifiableList:collectorsToUnmodifiableList·p0.50    sample        0.044            s/op
BenchmarkStreamToList.collectorsToUnmodifiableList:collectorsToUnmodifiableList·p0.90    sample        0.046            s/op
BenchmarkStreamToList.collectorsToUnmodifiableList:collectorsToUnmodifiableList·p0.95    sample        0.047            s/op
BenchmarkStreamToList.collectorsToUnmodifiableList:collectorsToUnmodifiableList·p0.99    sample        0.051            s/op
BenchmarkStreamToList.collectorsToUnmodifiableList:collectorsToUnmodifiableList·p0.999   sample        0.057            s/op
BenchmarkStreamToList.collectorsToUnmodifiableList:collectorsToUnmodifiableList·p0.9999  sample        0.057            s/op
BenchmarkStreamToList.collectorsToUnmodifiableList:collectorsToUnmodifiableList·p1.00    sample        0.057            s/op
BenchmarkStreamToList.streamToList                                                       sample  655   0.031 ±  0.001   s/op
BenchmarkStreamToList.streamToList:streamToList·p0.00                                    sample        0.030            s/op
BenchmarkStreamToList.streamToList:streamToList·p0.50                                    sample        0.031            s/op
BenchmarkStreamToList.streamToList:streamToList·p0.90                                    sample        0.032            s/op
BenchmarkStreamToList.streamToList:streamToList·p0.95                                    sample        0.033            s/op
BenchmarkStreamToList.streamToList:streamToList·p0.99                                    sample        0.035            s/op
BenchmarkStreamToList.streamToList:streamToList·p0.999                                   sample        0.037            s/op
BenchmarkStreamToList.streamToList:streamToList·p0.9999                                  sample        0.037            s/op
BenchmarkStreamToList.streamToList:streamToList·p1.00                                    sample        0.037            s/op
BenchmarkStreamToList.collectorsToList                                                       ss   20   0.043 ±  0.001   s/op
BenchmarkStreamToList.collectorsToUnmodifiableList                                           ss   20   0.045 ±  0.004   s/op
BenchmarkStreamToList.streamToList                                                           ss   20   0.031 ±  0.001   s/op
```
从报告中可以看到：

- 吞吐量：streamToList > collectorsToList > collectorsToUnmodifiableList
- 平均耗时：streamToList > collectorsToList > collectorsToUnmodifiableList
- p0.9999耗时：streamToList > collectorsToUnmodifiableList > collectorsToList
- 冷启动耗时：streamToList > collectorsToList > collectorsToUnmodifiableList

所以，`Stream.toList()`的性能要各方面都要好于`Collectors.toList()`和`Collectors.toUnmodifiableList()`。<br />再放大一些数据量，试试：
```java
@Benchmark
public List<Integer> streamToList() {
	return IntStream.range(1, 10000).boxed().toList();
}

@Benchmark
public List<Integer> collectorsToList() {
	return IntStream.range(1, 10000).boxed().collect(Collectors.toList());
}

@Benchmark
public List<Integer> streamToList() {
	return IntStream.range(1, 10000).boxed().toList();
}
```
结果报告：
```java
Benchmark                                                                                  Mode  Cnt  Score   Error  Units
BenchmarkStreamToList.collectorsToList                                                    thrpt   20  2.186 ± 0.162  ops/s
BenchmarkStreamToList.collectorsToUnmodifiableList                                        thrpt   20  2.184 ± 0.042  ops/s
BenchmarkStreamToList.streamToList                                                        thrpt   20  3.538 ± 0.058  ops/s
BenchmarkStreamToList.collectorsToList                                                     avgt   20  0.426 ± 0.004   s/op
BenchmarkStreamToList.collectorsToUnmodifiableList                                         avgt   20  0.469 ± 0.016   s/op
BenchmarkStreamToList.streamToList                                                         avgt   20  0.293 ± 0.008   s/op
BenchmarkStreamToList.collectorsToList                                                   sample   58  0.448 ± 0.049   s/op
BenchmarkStreamToList.collectorsToList:collectorsToList·p0.00                            sample       0.414           s/op
BenchmarkStreamToList.collectorsToList:collectorsToList·p0.50                            sample       0.422           s/op
BenchmarkStreamToList.collectorsToList:collectorsToList·p0.90                            sample       0.458           s/op
BenchmarkStreamToList.collectorsToList:collectorsToList·p0.95                            sample       0.560           s/op
BenchmarkStreamToList.collectorsToList:collectorsToList·p0.99                            sample       1.160           s/op
BenchmarkStreamToList.collectorsToList:collectorsToList·p0.999                           sample       1.160           s/op
BenchmarkStreamToList.collectorsToList:collectorsToList·p0.9999                          sample       1.160           s/op
BenchmarkStreamToList.collectorsToList:collectorsToList·p1.00                            sample       1.160           s/op
BenchmarkStreamToList.collectorsToUnmodifiableList                                       sample   60  0.458 ± 0.004   s/op
BenchmarkStreamToList.collectorsToUnmodifiableList:collectorsToUnmodifiableList·p0.00    sample       0.447           s/op
BenchmarkStreamToList.collectorsToUnmodifiableList:collectorsToUnmodifiableList·p0.50    sample       0.455           s/op
BenchmarkStreamToList.collectorsToUnmodifiableList:collectorsToUnmodifiableList·p0.90    sample       0.471           s/op
BenchmarkStreamToList.collectorsToUnmodifiableList:collectorsToUnmodifiableList·p0.95    sample       0.482           s/op
BenchmarkStreamToList.collectorsToUnmodifiableList:collectorsToUnmodifiableList·p0.99    sample       0.492           s/op
BenchmarkStreamToList.collectorsToUnmodifiableList:collectorsToUnmodifiableList·p0.999   sample       0.492           s/op
BenchmarkStreamToList.collectorsToUnmodifiableList:collectorsToUnmodifiableList·p0.9999  sample       0.492           s/op
BenchmarkStreamToList.collectorsToUnmodifiableList:collectorsToUnmodifiableList·p1.00    sample       0.492           s/op
BenchmarkStreamToList.streamToList                                                       sample   78  0.293 ± 0.012   s/op
BenchmarkStreamToList.streamToList:streamToList·p0.00                                    sample       0.277           s/op
BenchmarkStreamToList.streamToList:streamToList·p0.50                                    sample       0.284           s/op
BenchmarkStreamToList.streamToList:streamToList·p0.90                                    sample       0.309           s/op
BenchmarkStreamToList.streamToList:streamToList·p0.95                                    sample       0.377           s/op
BenchmarkStreamToList.streamToList:streamToList·p0.99                                    sample       0.459           s/op
BenchmarkStreamToList.streamToList:streamToList·p0.999                                   sample       0.459           s/op
BenchmarkStreamToList.streamToList:streamToList·p0.9999                                  sample       0.459           s/op
BenchmarkStreamToList.streamToList:streamToList·p1.00                                    sample       0.459           s/op
BenchmarkStreamToList.collectorsToList                                                       ss   20  0.474 ± 0.133   s/op
BenchmarkStreamToList.collectorsToUnmodifiableList                                           ss   20  0.493 ± 0.099   s/op
BenchmarkStreamToList.streamToList                                                           ss   20  0.325 ± 0.056   s/op
```
从报告中可以看到

- 吞吐量：streamToList > collectorsToList > collectorsToUnmodifiableList
- 平均耗时：streamToList > collectorsToList > collectorsToUnmodifiableList
- p0.9999耗时：streamToList > collectorsToUnmodifiableList > collectorsToList
- 冷启动耗时：streamToList > collectorsToList > collectorsToUnmodifiableList

所以，即使集合内的元素增大，`Stream.toList()`的性能在各方面依然都要好于`Collectors`下的方法。
