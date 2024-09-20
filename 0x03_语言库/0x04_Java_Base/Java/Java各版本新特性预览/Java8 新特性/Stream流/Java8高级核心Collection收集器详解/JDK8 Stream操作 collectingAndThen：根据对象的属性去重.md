Java Stream<br />ExportTemperatureDto实体对象：
```java
@Getter
@Setter
@ToString
public class ExportTemperatureDto {
    private String name;
    private Double morningTemperature;
    private Double afternoonTemperature;
    private String classId;
    private String gradeId;
    private Integer personId;
}
```
在一个ExportTemperatureDto的集合中，根据personId属性去重，生成一个新的集合。
```java
import static java.util.Comparator.comparing;
import static java.util.stream.Collectors.collectingAndThen;
import static java.util.stream.Collectors.toCollection;
 
public class StreamTest {
 
    public static void main(String[] args) {
        List<ExportTemperatureDto> temperatureList = Lists.newArrayList();
        temperatureList.add(new ExportTemperatureDto(1, "haha"));
        temperatureList.add(new ExportTemperatureDto(2, "haha"));
        temperatureList.add(new ExportTemperatureDto(3, "haha"));
        temperatureList.add(new ExportTemperatureDto(4, "haha"));
 
        temperatureList.add(new ExportTemperatureDto(1, "hahaasdas"));
        temperatureList.add(new ExportTemperatureDto(2, "hahaasdas"));
 
        List<ExportTemperatureDto> result = temperatureList.stream()
                .collect(
                        collectingAndThen(
                                toCollection(
                                        () -> new TreeSet<>(comparing(ExportTemperatureDto::getPersonId))
                                ),
                                ArrayList::new
                        )
                );
 
        result.forEach(System.out::println);
 
        /*
            输出结果为：
                personId为1的，其名称为haha
                personId为2的，其名称为haha
            因为TreeSet底层是使用TreeMap进行实现的，传入了根据getPersonId进行比较的比较器
            在判断personId相同时，其比较结果为0，然后就会替换其value值，而key值是不会变化的，
            又因为TreeSet是将传入的元素作为key的，所以使用TreeSet时，当比较器比较的结果相同时，以不会将原来的值替换成比较后的值
         */
 
    }
}
```
知其然知其所以然，这个stream流的操作看起来还是有点难度的。<br />使用到了`collectingAndThen`完成根据属性进行去重的操作，对于该去重操作的关键使用到了`collectingAndThen`、`toCollection`、`TreeSet`，有点难以理解。<br />理解根据对象的属性进行去重的核心是，将集合放到TreeSet中，然后再将TreeSet转为List， 其中TreeSet要传入一个根据哪个属性进行比较的比较器，然后使用`public ArrayList(Collection<? extends E> c)`将TreeSet放入构造器中生成List。<br />上面的Stream操作可以使用普通的集合：
```java
TreeSet<ExportTemperatureDto> treeSet = new TreeSet<>(Comparator.comparing(ExportTemperatureDto::getPersonId));
for (ExportTemperatureDto temperatureDto : temperatureList){
    treeSet.add(temperatureDto);
}
List<ExportTemperatureDto> result2 =  new ArrayList<>(treeSet);
```
只要能够理解普通集合怎么操作的，那么使用Stream流操作时，就是要看对于API的使用是否熟悉，其实这个才是关键，只有理解了`collectingAndThen`、`toCollection`、JDK8的匿名函数这样内容，才能看懂这个式子。<br />下面就简单介绍一下：<br />首先说一下`collectingAndThen`方法的使用-------**先进行结果集的收集，然后将收集到的结果集进行下一步的处理**，红字的两句话是理解`collectingAndThen`的关键，首先看一下`collectingAndThen`需要传递的参数：
```java
public static<T,A,R,RR> Collector<T,A,RR> 
         collectingAndThen(Collector<T,A,R> downstream,
         Function<R,RR> finisher) 
```
可以看到第一个参数是Collector接口的子类，所以还是对于对于Collector的处理，Collectors工具类里面的`toList()`、`toSet()`、`joining()`、`mapping()`、`collectingAndThen()`等几乎所有的方法都可以使用，这样感觉这个`collectingAndThen`就很强大了，可以嵌套的去使用。<br />第二个参数是一个Function函数，熟悉的同学都知道，Function函数是这样的：`R apply(T t)`，这个也是理解上面去重式子的关键，原来想的是`ArrayList::new`调用的无参的构造方法，其实他调用的ArrayList的有参构造方法，
```java
public ArrayList(Collection<? extends E> c)
```
调用的是上面那个构造方法，这样就很清晰，就是把第一个参数`downstream`的结果，交给第二个参数`Function`函数的参数里面，`R apply(T t)`，也就是将结果设置成t。<br />对于`toCollection`是一个通用的转为集合的操作，当然在Collectors类里面也有`toList()`、`toSet()`方法，但是都不满足于使用`TreeSet`来收集集合的方法，所以使用`toCollection`是一个通用的方法，使用`TreeSet`进行收集，然后传入根据哪个属性进行比较的比较器，这样就可以了。
