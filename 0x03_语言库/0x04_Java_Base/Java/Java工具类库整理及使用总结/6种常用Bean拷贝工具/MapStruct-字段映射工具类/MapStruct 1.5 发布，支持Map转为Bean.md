Java<br />MapStruct 1.5.0 Final发布，本次正式版距离上次正式版发布已经过去了快7年（上个正式版发布于2015年11月），此次发布除了修复了110多个bug外，还有以下新特性值得关注：

- 支持了`Map<String,?>`到bean的转换
- 支持更加完备的条件转换（Conditional mapping）
- 支持子类之间的转换（Support for subclass mapping）
<a name="meMjy"></a>
## 1、新增Map到Java bean的转换（Mapping from Map to Bean）
如果有以下Java Bean
```java
public class Customer {
    private Long id;
    private String name;
    //getters and setter omitted for brevity
}
```
相应的MapStruct代码如下：
```java
@Mapper
public interface CustomerMapper {
    @Mapping(target = "name", source = "customerName")
    Customer toCustomer(Map<String, String> map);
}
```
那最终会生成类似如下的转换代码：
```java
// GENERATED CODE
public class CustomerMapperImpl implements CustomerMapper {
 
    @Override
    public Customer toCustomer(Map<String, String> map) {
        // ...
        if ( map.containsKey( "id" ) ) {
            customer.setId( Integer.parseInt( map.get( "id" ) ) );
        }
        if ( map.containsKey( "customerName" ) ) {
            customer.setName( source.get( "customerName" ) );
        }
        // ...
    }
}
```
不过需要注意，待转换的Map的key必须是String类型的，否则，转换代码会跳过这个key
<a name="cXm3o"></a>
## 2、更加完备的条件转换（ Conditional Mapping）
从1.5.0 Final版本之前，如果Java bean中含有`hasXXX`或者`isXXX`的这类方法（XXX是bean中的属性名），则MapStruct生成的代码中则会调用这类方法来判断是否在转换后的bean中是否包含原来的属性，但是遗憾的是，大多数情况下，并不能直接修改原bean的代码。基于此，1.5.0 Final版本引入了org.mapstruct.Condition注解来实现条件转换。例如有如下转换代码：
```java
@Mapper
public interface CarMapper {
 
    CarDto carToCarDto(Car car);
 
    @Condition
    default boolean isNotEmpty(String value) {
        return value != null && !value.isEmpty();
    }
}
```
则Map Struct 1.5.0 Final生成的代码是：
```java
// GENERATED CODE
public class CarMapperImpl implements CarMapper {
 
    @Override
    public CarDto carToCarDto(Car car) {
        if ( car == null ) {
            return null;
        }
        CarDto carDto = new CarDto();
        if ( isNotEmpty( car.getOwner() ) ) {
            carDto.setOwner( car.getOwner() );
        }
        // Mapping of other properties
        return carDto;
    }
}
```
`org.mapstruct.Condition`除了作用到整个bean外还可以修饰具体的属性值，实现bean属性维度的条件转换。
<a name="ERjF0"></a>
## 3、增加对子类转换的支持（Subclass Mapping）
假如有父类Fruit和两个子类Apple和Banana，在新特性的支持下转换代码可以写的更加简洁：
```java
@Mapper
public interface FruitMapper {
	
	@SubclassMapping( source = AppleDto.class, target = Apple.class )
	@SubclassMapping( source = BananaDto.class, target = Banana.class )
	Fruit map( FruitDto source );
}
```
如果Fruit是抽象类或者是接口，则会报编译错误。
