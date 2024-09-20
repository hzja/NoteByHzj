Java MapStruct<br />Mapstruct可以代替BeanUtil来进行**DTO**、**VO**、**PO**之间的转换。它使用的是Java编译期的  annotation processor 机制，说白了它就是一个代码生成器，代替手工进行类型转换期间的取值赋值操作。
```java
@Mapper(componentModel = "spring")
public interface AreaMapping {

    List<AreaInfoListVO> toVos(List<Area> areas);
}
```
就这么几行就把一个**PO**的集合转换成了对应**VO**的集合。
```java
// spring bean 
@Autowired
AreaMapping areaMapping
    
// 转换源 areas    
List<Area> areas = ……;
// 转换目标 vos 
List<AreaInfoListVO> vos = areaMapping.toVos(areas)
```
换成手写试试，起码得五分之一炷香的功夫。<br />但是这样写还是不太爽，每次都要挂对应的Mapper类。
<a name="i1nNd"></a>
## `Converter`
**Spring framework**提供了一个`Converter<S,T>`接口：
```java
@FunctionalInterface
public interface Converter<S, T> {
    @Nullable
    T convert(S source);

    default <U> Converter<S, U> andThen(Converter<? super T, ? extends U> after) {
        Assert.notNull(after, "After Converter must not be null");
        return (s) -> {
            T initialResult = this.convert(s);
            return initialResult != null ? after.convert(initialResult) : null;
        };
    }
}
```
它的作用是将S转换为T，这和**Mapstruct**的作用不谋而合。<br />`Converter`会通过`ConverterRegistry`这个注册接口注册到`ConversionService`，然后就可以通过`ConversionService`的`convert`方法来进行转换：
```java
<T> T convert(@Nullable Object source, Class<T> targetType);
```
<a name="WmKpQ"></a>
## MapStruct Spring Extensions
根据上面的机制官方推出了**MapStruct Spring Extensions**插件， 它实现了一种机制，所有的**Mapstruct**映射接口(**Mapper**)只要实现了`Converter`，都会自动注册到`ConversionService`，只需要通过`ConversionService`就能完成任何转换操作。
```java
/**
 * @since 1.0.0
 */
@Mapper(componentModel = "spring")
public interface CarMapper extends Converter<Car, CarDto> {

    @Mapping(target = "seats", source = "seatConfiguration")
    CarDto convert(Car car);
}
```
调用时：
```java
@Autowired
private ConversionService conversionService;

Car car = ……;
CarDto carDto = conversionService.convert(car,CarDto.class);
```
**MapStruct Spring Extensions** 会自动生成一个适配类处理**Mapper**注册：
```java
package org.mapstruct.extensions.spring.converter;

import cn.fcant.mapstruct.entity.Car;
import cn.fcant.mapstruct.entity.CarDto;
import org.springframework.context.annotation.Lazy;
import org.springframework.core.convert.ConversionService;
import org.springframework.stereotype.Component;
/**
 * @since 1.0.0
 */
@Component
public class ConversionServiceAdapter {
    private final ConversionService conversionService;

    public ConversionServiceAdapter(@Lazy final ConversionService conversionService) {
        this.conversionService = conversionService;
    }

    public CarDto mapCarToCarDto(final Car source) {
        return (CarDto)this.conversionService.convert(source, CarDto.class);
    }
}
```
<a name="QPRzD"></a>
### 自定义
<a name="WpCCW"></a>
#### 自定义适配类的包路径和名称
默认情况下，生成的适配类将位于包org.mapstruct.extensions.spring.converter中，名称固定为`ConversionServiceAdapter`。如果希望修改包路径或者名称，可以这样：
```java
package cn.fcant.mapstruct.config;

import org.mapstruct.MapperConfig;
import org.mapstruct.extensions.spring.SpringMapperConfig;

/**
 * @since 1.0.0
 */
@MapperConfig(componentModel = "spring")
@SpringMapperConfig(conversionServiceAdapterPackage = "cn.fcant.mapstruct.config",
        conversionServiceAdapterClassName = "MapStructConversionServiceAdapter")
public class MapperSpringConfig {
}
```
不指定`conversionServiceAdapterPackage`元素，生成的 Adapter 类将与注解的 Config 驻留在同一个包中，所以上面的路径是可以省略的。
<a name="F0HhF"></a>
#### 指定`ConversionService`
如果**Spring IoC**容器中有多个`ConversionService`，可以通过`@SpringMapperConfig`注解的`conversionServiceBeanName` 参数指定。
```java
package cn.fcant.mapstruct.config;

import org.mapstruct.MapperConfig;
import org.mapstruct.extensions.spring.SpringMapperConfig;

/**
 * @since 1.0.0
 */
@MapperConfig(componentModel = "spring")
@SpringMapperConfig(conversionServiceAdapterPackage = "cn.fcant.mapstruct.config",
        conversionServiceAdapterClassName = "MapStructConversionServiceAdapter",
                   conversionServiceBeanName = "myConversionService")
public class MapperSpringConfig {
}
```
<a name="ES7Gx"></a>
#### 集成Spring的内置转换
**Spring**内部提供了很多好用的`Converter<S,T>`实现，有的并不直接开放，如果想用**Mapstruct**的机制使用它们，可以通过`@SpringMapperConfig`注解的 `externalConversions`注册它们。
```java
@MapperConfig(componentModel = "spring")
@SpringMapperConfig(
   externalConversions = @ExternalConversion(sourceType = String.class, targetType = Locale.class))
public interface MapstructConfig {}
```
会在适配器中自动生成相应的转换：
```java
@Component
public class ConversionServiceAdapter {
	private final ConversionService conversionService;
	
	public ConversionServiceAdapter(@Lazy final ConversionService conversionService) {
		this.conversionService = conversionService;
	}
	
	public Locale mapStringToLocale(final String source) {
		return conversionService.convert(source, Locale.class);
	}
}
```
<a name="ByAUd"></a>
## 总结
**mapstruct-spring-annotations** 使开发人员能够通过`ConversionService`使用定义的 **Mapstruct** 映射器，而不必单独导入每个 **Mapper**，从而允许 **Mapper** 之间的松散耦合。，它本身不会影响**Mapstruct**的机制。
