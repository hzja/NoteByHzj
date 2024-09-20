Java
<a name="Wp3Lr"></a>
## 官方介绍
首先打开 MapStruct 的官网地址，映入眼帘的就是下边的三步曲：<br />**What is it?**<br />MapStruct 是一个**代码生成器**，它基于约定优先于配置的方法大大简化了 JavaBean 类型之间映射的实现。生成的映射代码使用**普通方法**调用，因此速度快、类型**安全**且易于理解。<br />**Why?**<br />多层应用程序通常需要在不同的对象模型（例如实体和 DTO）之间进行**映射**。编写这样的映射代码是一项乏味且容易出错的任务。MapStruct 旨在通过尽可能自动化来简化这项工作。<br />与其他映射框架不同，MapStruct 在**编译时**生成 bean 映射，这确保了高性能，允许快速的开发人员反馈和彻底的错误检查。<br />**How?**<br />MapStruct 是插入 Java 编译器的**注释**处理器，可以在命令行构建（Maven、Gradle等）中使用，也可以在首选 IDE 中使用。它使用合理的默认值，但在配置或实现特殊行为时，用户可以自定义实现。<br />官网的解释总是咬文嚼字，晦涩难懂的，看到这只需要记住 MapStruct 是用来做实体类映射——实体类拷贝 的就可以了。<br />源码地址：[https://github.com/mapstruct/mapstruct ](https://github.com/mapstruct/mapstruct )<br />官网推荐的 Demo： [https://github.com/mapstruct/mapstruct-examples](https://github.com/mapstruct/mapstruct-examples)
<a name="hS8qj"></a>
## 简单实现
官网中有涉及到简单样例的实现，用2分钟来分析一波：
<a name="sJkva"></a>
### 1、引入依赖
```xml
<dependency>
  <groupId>org.mapstruct</groupId>
  <artifactId>mapstruct-jdk8</artifactId>
  <version>1.3.0.Final</version>
</dependency>
//注解处理器，根据注解自动生成mapper的实现
<dependency>
  <groupId>org.mapstruct</groupId>
  <artifactId>mapstruct-processor</artifactId>
  <version>1.2.0.Final</version>
</dependency>
```
在编译时会报 java: No property named "numberOfSeats" exists in source parameter(s). Did you mean "null"? 错误，经过查阅资料发现 mapstruct-processor 和 Lombok 的版本需要统一一下：mapstruct-processor：1.2.0.Final ， Lombok：1.16.14。
<a name="YxMCp"></a>
### 2、准备实体类 Car.java 和 数据传输类 CarDto.java
```java
@NoArgsConstructor
@AllArgsConstructor
@Data
public class Car {
    private String make;
    private int numberOfSeats;
    private CarType type;
}

@Data
@NoArgsConstructor
@AllArgsConstructor
public class CarDto {
    private String make;
    private int seatCount;
    private String type;

}
```
<a name="uhBiQ"></a>
### 3、创建映射器接口，里边定义映射方法
```java
@Mapper
public interface CarMapper {
 
    CarMapper INSTANCE = Mappers.getMapper( CarMapper.class );

    @Mapping(source = "numberOfSeats", target = "seatCount")
    CarDto carToCarDto(Car car); 
   
}
```
**解析分析：**

- `@Mapper` 将接口标记为映射接口，并允许 MapStruct 处理器在编译期间启动。这里的 `@Mapper` 注解不是 mybatis 的注解，而是 org.mapstruct.Mapper 的；
- 实际映射方法 `carToCarDto()` 期望源对象 Car 作为参数，并返回目标对象 CarDto ，方法名可以自由选择；
- 对于源对象和目标对象中具有**不同名称**的属性，可以使用 `@Mapping` 注释来配置名称；
- 对于源对象和目标对象中具有**不同类型**的属性，也可以使用 `@Mapping` 注释来进行转换，比如：类型属性将从枚举类型转换为字符串；
- 一个接口中可以有多个映射方法，对于所有的这些方法，MapStruct 将生成一个实现；
- 该接口的实现实例可以从 Mappers 中获得，接口声明一个 `INSTANCE`，为客户端提供对映射器实现的访问。
<a name="UT0M4"></a>
### 4、实现类
可以将代码进行编译，然后会发现在 target 文件中生成了 CarMapperImpl.class 文件：![](https://cdn.nlark.com/yuque/0/2023/png/396745/1679550086894-da0d4471-9fc5-4f6c-b3d0-8068441afd2f.png#averageHue=%23575043&clientId=u2eed58f2-c92f-4&from=paste&id=u5f1552b0&originHeight=580&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u93a76ac3-4a47-4066-8aac-529b6dd835f&title=)<br />从代码中可以看出 MapStruct 自动生成了 set/get 代码，并且对**枚举类**进行了特殊处理。
<a name="WGlnG"></a>
### 5、客户端使用
```java
@Test
public void shouldMapCarToDto() {

    Car car = new Car( "Morris", 5, CarType.SEDAN );
    CarDto carDto = CarMapper.INSTANCE.carToCarDto( car );
    System.out.println(carDto);
    
}
```
执行结果：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1679550086890-9e3fe940-0316-44cb-819e-d26e3ab93915.png#averageHue=%2331302f&clientId=u2eed58f2-c92f-4&from=paste&id=ue3881e5f&originHeight=121&originWidth=565&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u907c8879-77c3-4864-869e-39bc7123512&title=)<br />小结: MapStruct 基于 mapper 接口，在**编译期**动态生成 set/get 代码的 class 文件 ，在运行时直接调用该 class 文件。
<a name="Z4nDX"></a>
## MapStruct 配置
<a name="re5J8"></a>
### `@Mapper`
翻开上边提到的 Mapper 注释的源码，该注释的解释是：将接口或抽象类标记为**映射器**，并通过 MapStruct 激活**该类型实现**的生成。找到其中的 **componentModel** 属性，默认值为 `default`，它有四种值供选择：

- `default`：映射器不使用组件模型，实例通常通过 `Mappers.getMapper(java.lang.Class)`获取;
- `cdi`：生成的映射器是 application-scoped 的CDI bean，可以通过 `@Inject` 获取；
- `spring`：生成的映射器是 Spring bean，可以通过 `@Autowired` 获取；
- `jsr330`：生成的映射器被 `@javax.inject.Named` 和 `@Singleton` 注释，可以通过 `@inject` 获取；

上边用的就是默认的方法，当然也可以用 `@Autowired` 来引入接口依赖，此处不再举例，有兴趣的小伙伴可以自己试试！<br />另外可以看下 uses 属性：可以通过定义其他类来完成字段转换，接下来来个小例子演示一下：
<a name="vy8mc"></a>
#### 1、定义一个 CarVo.java 类
```java
@Data
@NoArgsConstructor
@AllArgsConstructor
public class CarVo {

    private String make;
    private int seatCount;
    private boolean type;
}
```
<a name="L2X2g"></a>
#### 2、在 mapper 中定义一个 vo 转为 dto 的方法 `CarDto carVoToCarDto(CarVo carVo);`
当不加 uses 属性时，查看编译后生成的实现类
```java
public CarDto carVoToCarDto(CarVo carVo) {
    if (carVo == null) {
    return null;
} else {
    CarDto carDto = new CarDto();
    carDto.setMake(carVo.getMake());
    carDto.setSeatCount(carVo.getSeatCount());
    carDto.setType(String.valueOf(carVo.isType()));
    return carDto;
}
```
<a name="LpFre"></a>
#### 3、在 mapper 上增加 uses 属性，并指定自定义的处理类，代码如下：
```java
@Mapper(uses = {BooleanStrFormat.class})
public interface CarMapper {
    ......
}

/**
* 自定义的转换类
*/
@Component
public class BooleanStrFormat {
    public String toStr(boolean type) {
        if(type){
            return "Y";
        }else{
            return "N";
        }
    }

    public boolean toBoolean(String type) {
        if (type.equals("Y")) {
            return true;
        } else {
            return false;
        }
    }
}

/**
* 查看编译后生成的实现类
*/
public CarDto carVoToCarDto(CarVo carVo) {
    if (carVo == null) {
    	return null;
    } else {
        CarDto carDto = new CarDto();
        carDto.setMake(carVo.getMake());
        carDto.setSeatCount(carVo.getSeatCount());
        //调用自定义的类中的方法
        carDto.setType(this.booleanStrFormat.toStr(carVo.isType()));
        return carDto;
    }
}
```
<a name="vhb6y"></a>
#### 4、客户端代码
```java
@Test
public void shouldMapCarVoToDto() {

    CarVo carVo = new CarVo( "Morris", 5, false );
    CarDto carDto = CarMapper.INSTANCE.carVoToCarDto( carVo );

    System.out.println(carDto);
}
```
执行结果：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1679550086946-875c5797-6432-4ddd-998a-2132327587bb.png#averageHue=%2330302f&clientId=u2eed58f2-c92f-4&from=paste&id=uc5914dc8&originHeight=119&originWidth=593&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u98f360af-9fb7-473b-a4a1-193eed1a19d&title=)
<a name="G5vdP"></a>
### `@Mapping`
`@Mapping` 可以用来配置一个 bean 属性或枚举常量的映射，默认是将具有相同名称的属性进行映射，当然也可以用 source、expression 或者 constant 属性手动指定，接下来分析下常用的属性值。

1. target：属性的目标名称，同一目标属性不能映射多次。如果用于映射枚举常量，则将给出常量成员的名称，在这种情况下，源枚举中的多个值可以映射到目标枚举的相同值。
2. source：属性的源名称，
   - 如果带注释的方法有多个源参数，则属性名称必须使用参数名称限定，例如“addressParam.city"；
   - 当找不到匹配的属性时，MapStruct 将查找匹配的参数名称；
   - 当用于映射枚举常量时，将给出常量成员的名称；
   - 该属性不能与 constant 或 expression 一起使用；
3. dateFormat：通过 SimpleDateFormat 实现 String 到 Date 日期之间相互转换。
4. numberFormat：通过 DecimalFormat 实现 Number 与 String 的数值格式化。
5. constant：设置指定目标属性的常量字符串，当指定的目标属性的类型为：primitive 或 boxed（例如 Long）时，MapStruct 检查是否可以将该 primitive 作为有效的文本分配给 primitive 或 boxed 类型。如果可能，MapStruct 将分配为文字；如果不可能，MapStruct 将尝试应用用户定义的映射方法。 另外，MapStruct 将常量作为字符串处理，将通过应用匹配方法、类型转换方法或内置转换来转换该值。此属性不能与 `source`、`defaultValue`、`defaultExpression` 或 `expression` 一起使用。
6. expression：是一个表达式，根据该表达式设置指定的目标属性。他的属性不能与 `source`、 `defaultValue`、`defaultExpression`、`constant` 一起使用。
7. ignore: 忽略这个字段。

用 expression 这个属性来实现一下上边用 uses 实现的案例：
<a name="Ovb2x"></a>
#### 1、在 mapper 中定义方法
```java
@Mapping(target = "type", expression = "java(new com.ittest.controller.BooleanStrFormat().toStr(carVo.isType()))")
CarDto carVoToDtoWithExpression(CarVo carVo);
```
<a name="f0euP"></a>
#### 2、生成的实现类
```java
@Override
public CarDto carVoToDtoWithExpression(CarVo carVo) {
    if ( carVo == null ) {
        return null;
    }
    
    CarDto carDto = new CarDto();
    
    carDto.setMake( carVo.getMake() );
    carDto.setSeatCount( carVo.getSeatCount() );
    
    carDto.setType( new com.ittest.controller.BooleanStrFormat().toStr(carVo.isType()) );
    
    return carDto;
}
```
<a name="Po3ZX"></a>
#### 3、客户端
```java
@Test
public void mapCarVoToDtoWithExpression() {

    CarVo carVo = new CarVo( "Morris", 5, false );
    CarDto carDto = CarMapper.INSTANCE.carVoToDtoWithExpression( carVo );

    System.out.println(carDto);
}
```
运行结果：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1679550086891-796e0760-61af-4a58-b689-8487fe70ad22.png#averageHue=%232f2e2e&clientId=u2eed58f2-c92f-4&from=paste&id=uad539d59&originHeight=273&originWidth=624&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u909e9017-5b45-42d6-976c-6cdbad67a8b&title=)<br />至于其他的用法大家可以多多探索。<br />重要提示：枚举映射功能已被弃用，并被 `ValueMapping` 取代。它将在后续版本中删除。
<a name="pj5ps"></a>
### `@Mappings`
可以配置多个 `@Mapping`，例如
```java
@Mappings({
    @Mapping(source = "id", target = "carId"),
    @Mapping(source = "name", target = "carName"),
    @Mapping(source = "color", target = "carColor")
})
```
<a name="p0Tg3"></a>
### `@MappingTarget`
用于更新已有对象，还是用例子来说明吧：
<a name="fIHIK"></a>
#### 1、创建 BMWCar.java 类
```java
@NoArgsConstructor
@AllArgsConstructor
@Data
public class BMWCar {
    private String make;
    private int numberOfSeats;
    private CarType type;

    private String color;
    private String price;

}
```
<a name="wPXce"></a>
#### 2、mapper 中创建更新方法，并查看实现类
```java
// 更新方法
void updateBwmCar(Car car, @MappingTarget BMWCar bwmCar);

// 实现类
public void updateBwmCar(Car car, BMWCar bwmCar) {
    if (car != null) {
        bwmCar.setMake(car.getMake());
        bwmCar.setNumberOfSeats(car.getNumberOfSeats());
        bwmCar.setType(car.getType());
    }
}
```
<a name="i01JS"></a>
#### 3、客户端代码
```java
@Test
public void updateBwmCar() {
    Car car = new Car( "Morris", 5, CarType.SEDAN );
    BMWCar bwmCar = new BMWCar("BWM", 5, CarType.SPORTS, "RED", "50w");
    System.out.println("更新前 car:"+car.toString());
    System.out.println("更新前 BWMCar:"+bwmCar.toString());

    CarMapper.INSTANCE.updateBwmCar(car, bwmCar);

    System.out.println("更新后 car:"+car.toString());
    System.out.println("更新后 BWMCar:"+bwmCar.toString());
}
```
执行结果：![](https://cdn.nlark.com/yuque/0/2023/png/396745/1679550086877-566df663-6a93-4355-a81d-ced8bd62b874.png#averageHue=%232f2f2e&clientId=u2eed58f2-c92f-4&from=paste&id=ud8406764&originHeight=240&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=udd92df0d-69ba-4752-ad52-3545d2f1870&title=)
<a name="mLvj1"></a>
### 扩展：多个对象映射一个对象
<a name="oiL0p"></a>
#### 1、准备实体类 Benz4SMall.java 和 Mall4S.java
```java
@NoArgsConstructor
@AllArgsConstructor
@Data
public class Mall4S {

    private String address;

    private String mobile;

}

@Data
@NoArgsConstructor
@AllArgsConstructor
public class Benz4SMall {

    private String address;
    private String mobile;
    private String make;
    private int numberOfSeats;
}
```
<a name="w4xz7"></a>
#### 2、mapper 创建转换方法并查看生成的实现类
```java
Benz4SMall mallCarToBenzMall(Car car, Mall4S mall4S);

/**
 * 实现类
 */
public Benz4SMall mallCarToBenzMall(Car car, Mall4S mall4S) {
    if (car == null && mall4S == null) {
        return null;
    } else {
        Benz4SMall benz4SMall = new Benz4SMall();
        if (car != null) {
            benz4SMall.setMake(car.getMake());
            benz4SMall.setNumberOfSeats(car.getNumberOfSeats());
        }

        if (mall4S != null) {
            benz4SMall.setAddress(mall4S.getAddress());
            benz4SMall.setMobile(mall4S.getMobile());
        }

        return benz4SMall;
    }
}
```
<a name="QqvYO"></a>
#### 3、客户端
```java
@Test
public void mallCarToBenzMall() {
    Car car = new Car( "Morris", 5, CarType.SEDAN );
    Mall4S mall4S = new Mall4S("北京市", "135XXXX4503");
    Benz4SMall benz4SMall = CarMapper.INSTANCE.mallCarToBenzMall(car, mall4S);
    System.out.println(benz4SMall.toString());
}
```
执行结果![](https://cdn.nlark.com/yuque/0/2023/png/396745/1679550087417-10e10feb-5261-4bcd-93b2-9ec3be26f532.png#averageHue=%232e2d2d&clientId=u2eed58f2-c92f-4&from=paste&id=u04decaa5&originHeight=268&originWidth=1072&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u25349cca-efe0-4c23-90ff-808ea0a5015&title=)
<a name="iiynK"></a>
## 深拷贝与浅拷贝
深拷贝和浅拷贝最根本的区别在于是否真正获取一个对象的复制**实体**，而不是引用。<br />假设 B 复制了 A ，修改 A 的时候，看 B 是否发生变化：如果 B 跟着也变了，说明是浅拷贝，**拿人手短**！（修改堆内存中的同一个值）；如果 B 没有改变，说明是深拷贝，**自食其力**！（修改堆内存中的不同的值）<br />MapStruct 中是**创建新的对象**，也就是**深拷贝**。
<a name="cQIm5"></a>
## MapStruct 与其他 Copy 的对比
在平时的项目中经常会使用到拷贝的功能，将他们做一下对比结果：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1679550087445-a35522a6-1e9c-48f0-8067-f63e98ab120f.png#averageHue=%232d2e2f&clientId=u2eed58f2-c92f-4&from=paste&id=u6f18da73&originHeight=399&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u02ef80c0-263c-4ee6-a486-6ab36f2e7ba&title=)<br />输出结果：手动Copy >Mapstuct>= cglibCopy > springBeanUtils > apachePropertyUtils > apacheBeanUtils 可以理解为: 手工复制 > cglib > 反射 > Dozer。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1679550087364-b8877351-c477-4cba-9cf5-7cf3d3fd19a1.png#averageHue=%232d2e30&clientId=u2eed58f2-c92f-4&from=paste&id=u3476b20f&originHeight=313&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uf226b83e-687e-47c9-9b79-f4243094a8c&title=)<br />根据测试结果，可以得出在速度方面，MapStruct 是最好的，执行速度是 Apache BeanUtils 的10倍、Spring BeanUtils 的 4-5倍、和 BeanCopier 的速度差不多。<br />总结：在大数据量级的情况下，MapStruct 和 BeanCopier 都有着较高的性能优势，其中 MapStruct 尤为优秀。如果仅是在日常处理少量的对象时，选取哪个其实变得并不重要，但数据量大时建议还是使用 MapStruct 或 BeanCopier 的方式，提高接口性能。
