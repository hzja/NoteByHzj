Java 性能测试
<a name="yoeG8"></a>
## 什么是JMH
JMH，即Java Microbenchmark Harness，Java平台下的一套微基准测试工具。如果需要测试API性能的话，就可以用上这个工具，所以它并不是取代单元测试的。它可以在开发阶段提供性能参考标准，不过这并不代表线上的性能表现，不同的硬件和操作系统也会带来性能差异，所以最终还是需要上到测试或沙箱环境，让测试人员进行压测。
<a name="xFz7t"></a>
## 为什么需要JMH
在了解JMH之前，如果需要性能测试，通常会使用for循环，或者JMeter。而JMH正是比for循环严谨，比JMeter使用简单的测试工具。<br />再者，不知道你注意过没有，在使用for循环测试时，第一次或者头几次运行总是最慢的，越到后面越快。从《计算机组成与设计 硬件软件接口》一书中可以了解到，从更底层讲，Java是解释型的语言。虽然Java也需要编译，但是编译后只是字节码，还需要JVM解释成对应宿主机的机器码。解释的优势是可移植性，但是性能较差。在20世纪80和90年代，虽然解释型语言的性能也飞速提升，但是与C语言相比，仍有10倍的性能差距。<br />**为了保持可移植性，同时又提高性能，Java便开发了即时编译器（Just In Time complier），其通过记录运行的程序来找到所谓的“热点”方法，然后将它们直接编译成宿主机的指令序列**，即不通过JVM解释那一层。这样以后该方法的运行就会更快。<br />看到这里也就明白了，为什么程序越到后面就会越快。JMH在真正的测试之前会预热程序，而且还可以通过配置进程数、线程数等参数来使程序更接近实际的运行状况。
<a name="tIKwB"></a>
## 如何使用
首先引入Maven依赖：
```xml
<dependency>
  <groupId>org.openjdk.jmh</groupId>
  <artifactId>jmh-core</artifactId>
  <version>1.21</version>
</dependency>
<dependency>
  <groupId>org.openjdk.jmh</groupId>
  <artifactId>jmh-generator-annprocess</artifactId>
  <version>1.21</version>
  <scope>test</scope>
</dependency>
```
本案例中，写了一个简单的小程序，它会从指定目录读取文件夹内容（每行一个数字），然后会对取出来的数字进行排序。排序算法选择了插入排序和归并排序，通过基准测试来看看两者的性能差距。<br />读取文件内容
```java
public class ReadFile {
   public static int[] readInteger(String path){
       try(BufferedReader in = new BufferedReader(new FileReader(path));) {
           List<Integer> temp = new ArrayList<>();
           String str;
           while ((str = in.readLine()) != null) {
               temp.add(Integer.parseInt(str));
           }
           int[] result = new int[temp.size()];
           for(int i=0;i<temp.size();i++){
               result[i]=temp.get(i);
           }
           return result;
       } catch (Exception e) {
           e.printStackTrace();
           return new int[0];
       }
   }
}
```
两个排序算法就不贴了，网上可以搜到很多。实际的开发可能会用上SpringBoot，所以还得与Junit整合，并使用自动注入功能。先直接贴上测试代码：
```java
@BenchmarkMode(Mode.All)
@Warmup(iterations = 3)//预热轮数
@Measurement(iterations = 1, time = 1, timeUnit = TimeUnit.SECONDS)
@Threads(8)//线程数
@Fork(0) //fork的次数，如果想用Autowired自动注入，这个填0
@OutputTimeUnit(TimeUnit.MILLISECONDS)
@State(Scope.Benchmark)
@RunWith(SpringRunner.class) //整合SpringBoot的测试运行环境
@SpringBootTest
public class JHMTest {
   //想用自动注入功能，对象必须是静态的，fork填0
    private static SortService service;

    @Autowired
    void setService(SortService service){
        JHMTest.service =service;
    }

    @Test
    public void executeBenchmark() throws RunnerException {
        //JMH的选项配置，除了上面的注解方式的配置，也可以直接在这个Options里面配置。
       //其中/Users/xxxx/Desktop/Benchmark.json是结果的输出文件
        Options options = new OptionsBuilder().include(this.getClass().getSimpleName())
                .output("/Users/xxxx/Desktop/Benchmark.json").build();
        new Runner(options).run();
    }

    @Benchmark
    public void insertSortTest(){
        int[] arr = ReadFile.readInteger("/Users/xxxxx/Desktop/test.txt");
        service.insertSort(arr);
    }

    @Benchmark
    public void mergeSortTest(){
        int[] arr = ReadFile.readInteger("/Users/xxxxx/Desktop/test 2.txt");
        service.mergeSort(arr);
    }
}
```
上面注释简单写了几个关键点，**执行**`**executeBenchmark**`**方法，JMH就会执行该类下带有**`**Benchmark**`**注解的方法。最终结果会输出到指定文件中**。<br />其他注解的解释可见图<br />![2021-10-06-17-23-52-877586.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1633512294459-e4a4f555-d182-42bb-acba-8faf613e88e1.png#clientId=u25ca41e7-450a-4&from=ui&id=u5d105d59&originHeight=2763&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=876995&status=done&style=shadow&taskId=ud0224863-4d90-4923-98fb-e9d26a0dd9c&title=)
<a name="M7nTM"></a>
## 结果查看
打开结果文件，前面一大坨是系统信息，可以简单看看，直接拉到最后，结果如下：
```
Benchmark                                        Mode     Cnt    Score   Error   Units
JHMTest.insertSortTest                          thrpt          129.302          ops/ms
JHMTest.mergeSortTest                           thrpt          122.224          ops/ms
JHMTest.insertSortTest                           avgt            0.065           ms/op
JHMTest.mergeSortTest                            avgt            0.066           ms/op
JHMTest.insertSortTest                         sample  122410    0.066 ± 0.002   ms/op
JHMTest.insertSortTest:insertSortTest·p0.00    sample            0.014           ms/op
JHMTest.insertSortTest:insertSortTest·p0.50    sample            0.050           ms/op
JHMTest.insertSortTest:insertSortTest·p0.90    sample            0.106           ms/op
JHMTest.insertSortTest:insertSortTest·p0.95    sample            0.120           ms/op
JHMTest.insertSortTest:insertSortTest·p0.99    sample            0.192           ms/op
JHMTest.insertSortTest:insertSortTest·p0.999   sample            0.492           ms/op
JHMTest.insertSortTest:insertSortTest·p0.9999  sample           11.891           ms/op
JHMTest.insertSortTest:insertSortTest·p1.00    sample           17.334           ms/op
JHMTest.mergeSortTest                          sample  122055    0.066 ± 0.002   ms/op
JHMTest.mergeSortTest:mergeSortTest·p0.00      sample            0.014           ms/op
JHMTest.mergeSortTest:mergeSortTest·p0.50      sample            0.050           ms/op
JHMTest.mergeSortTest:mergeSortTest·p0.90      sample            0.107           ms/op
JHMTest.mergeSortTest:mergeSortTest·p0.95      sample            0.121           ms/op
JHMTest.mergeSortTest:mergeSortTest·p0.99      sample            0.187           ms/op
JHMTest.mergeSortTest:mergeSortTest·p0.999     sample            0.457           ms/op
JHMTest.mergeSortTest:mergeSortTest·p0.9999    sample           11.957           ms/op
JHMTest.mergeSortTest:mergeSortTest·p1.00      sample           12.419           ms/op
JHMTest.insertSortTest                             ss            0.020           ms/op
JHMTest.mergeSortTest                              ss            0.020           ms/op
```
结果如上，**Mode中thrpt代表吞吐量，单位时间内的执行次数。avgt是平均时间，一次执行需要的单位时间。sample是基于采样的执行时间，采样频率由JMH自动控制。ss是单次执行的时间**。<br />从结果上看，两种排序算法的性能相差无几，当然与逻辑太简单也有关系。
