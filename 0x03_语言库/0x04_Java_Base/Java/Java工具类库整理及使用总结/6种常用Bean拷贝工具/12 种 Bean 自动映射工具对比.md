Java
<a name="yhMl2"></a>
## 目录

- 一、前言
- 二、性能测试对比
- 三、12种转换案例
   - 1. get\set
   - 2. json2Json
   - 3. Apache copyProperties
   - 4. Spring copyProperties
   - 5. Bean Mapping
   - 6. Bean Mapping ASM
   - 7. BeanCopier
   - 8. Orika
   - 9. Dozer
   - 10. ModelMapper
   - 11. JMapper
   - 12. MapStruct
- 四、总结
<a name="V4iII"></a>
## 一、前言
用一个属性拷贝工具，把vo转成dto，是 Apache 的还是 Spring 的，还是其他的什么，哪个效率最高？接下来用数据验证下，并提供出各种案例的使用对比
<a name="zVst1"></a>
## 二、性能测试对比
在 Java 系统工程开发过程中，都会有各个层之间的对象转换，比如  VO、DTO、PO、VO 等，而如果都是手动get、set又太浪费时间，还可能操作错误，所以选择一个自动化工具会更加方便。<br />目前整理出，用于对象属性转换有12种，包括：普通的getset、json2Json、Apache属性拷贝、Spring属性拷贝、bean-mapping、bean-mapping-asm、BeanCopier、Orika、Dozer、ModelMapper、JMapper、MapStruct 接下来分别测试这11种属性转换操作分别在一百次、一千次、一万次、十万次、一百万次时候的性能时间对比。<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1641949700821-6a50e143-8e89-4016-93d8-2db45e812404.webp#clientId=uf08a9aba-7e20-4&from=paste&id=ucc4f343f&originHeight=431&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u5e809cbf-f839-4ad7-8d8f-25faae520fd&title=)

- `BeanUtils.copyProperties` 是最常出现的工具类，但只要不把它用错成 Apache 包下的，而是使用 Spring 提供的，就基本还不会对性能造成多大影响。
- 但如果说性能更好，可替代手动get、set的，还是 MapStruct 更好用，因为它本身就是在编译期生成get、set代码，和写get、set一样。
- 其他一些组件包主要基于 AOP、ASM、CGlib，的技术手段实现的，所以也会有相应的性能损耗。
<a name="ahGg4"></a>
## 三、12种转换案例
![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1641949700777-494ebfc4-b93d-495e-abff-14ef46ba325b.webp#clientId=uf08a9aba-7e20-4&from=paste&id=u071b6f79&originHeight=578&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ub50ef478-f1c3-414a-a345-1e3cfb38490&title=)<br />源码：[https://github.com/fuzhengwei/guide-vo2dto](https://github.com/fuzhengwei/guide-vo2dto)<br />描述：在案例工程下创建 interfaces.assembler 包，定义 `IAssembler<SOURCE, TARGET>#sourceToTarget(SOURCE var)` 接口，提供不同方式的对象转换操作类实现，学习的过程中可以直接下载运行调试。
<a name="mTigW"></a>
### 1、get\set
```java
@Component
public class GetSetAssembler implements IAssembler<UserVO, UserDTO> {

    @Override
    public UserDTO sourceToTarget(UserVO var) {
        UserDTO userDTO = new UserDTO();
        userDTO.setUserId(var.getUserId());
        userDTO.setUserNickName(var.getUserNickName());
        userDTO.setCreateTime(var.getCreateTime());
        return userDTO;
    }

}
```

- 推荐：★★★☆☆
- 性能：★★★★★
- 手段：手写
- 点评：其实这种方式也是日常使用的最多的，性能肯定是杠杠的，就是操作起来有点麻烦。尤其是一大堆属性的 VO 对象转换为 DTO 对象时候。但其实也有一些快捷的操作方式，比如可以通过 Shift+Alt 选中所有属性，Shift+Tab 归并到一列，接下来在使用 Alt 选中这一列，批量操作粘贴 userDTO.set 以及快捷键大写属性首字母，最后切换到结尾补充括号和分号，最终格式化一下就搞定了。
<a name="yPoQU"></a>
### 2、json2Json
```java
@Component
public class Json2JsonAssembler implements IAssembler<UserVO, UserDTO> {

    @Override
    public UserDTO sourceToTarget(UserVO var) {
        String strJson = JSON.toJSONString(var);
        return JSON.parseObject(strJson, UserDTO.class);
    }

}
```

- 推荐：☆☆☆☆☆
- 性能：★☆☆☆☆
- 手段：把对象转JSON串，再把JSON转另外一个对象
- 点评：这么写多半有点烧！
<a name="vAb4S"></a>
### 3、Apache copyProperties
```java
@Component
public class ApacheCopyPropertiesAssembler implements IAssembler<UserVO, UserDTO> {

    @Override
    public UserDTO sourceToTarget(UserVO var) {
        UserDTO userDTO = new UserDTO();
        try {
            BeanUtils.copyProperties(userDTO, var);
        } catch (IllegalAccessException | InvocationTargetException e) {
            e.printStackTrace();
        }
        return userDTO;
    }

}
```

- 推荐：☆☆☆☆☆
- 性能：★☆☆☆☆
- 手段：Introspector 机制获取到类的属性来进行赋值操作
- 点评：有坑，兼容性交差，不建议使用
<a name="Gj32C"></a>
### 4、Spring copyProperties
```java
@Component
public class SpringCopyPropertiesAssembler implements IAssembler<UserVO, UserDTO> {

    @Override
    public UserDTO sourceToTarget(UserVO var) {
        UserDTO userDTO = new UserDTO();
        BeanUtils.copyProperties(var, userDTO);
        return userDTO;
    }

}
```

- 推荐：★★★☆☆
- 性能：★★★★☆
- 手段：Introspector机制获取到类的属性来进行赋值操作
- 点评：同样是反射的属性拷贝，Spring 提供的 copyProperties 要比 Apache 好用的多，只要不用错，基本不会有啥问题。
<a name="c7Ruo"></a>
### 5、Bean Mapping
```java
@Component
public class BeanMappingAssembler implements IAssembler<UserVO, UserDTO> {

    @Override
    public UserDTO sourceToTarget(UserVO var) {
        UserDTO userDTO = new UserDTO();
        BeanUtil.copyProperties(var, userDTO);
        return userDTO;
    }

}
```

- 推荐：★★☆☆☆
- 性能：★★★☆☆
- 手段：属性拷贝
- 点评：性能一般
<a name="mtwAQ"></a>
### 6、Bean Mapping ASM
```java
@Component
public class BeanMappingAssembler implements IAssembler<UserVO, UserDTO> {

    @Override
    public UserDTO sourceToTarget(UserVO var) {
        UserDTO userDTO = new UserDTO();
        BeanUtil.copyProperties(var, userDTO);
        return userDTO;
    }

}
```

- 推荐：★★★☆☆
- 性能：★★★★☆
- 手段：基于ASM字节码框架实现
- 点评：与普通的 Bean Mapping 相比，性能有所提升，可以使用。
<a name="mx0nF"></a>
### 7、BeanCopier
```java
@Component
public class BeanCopierAssembler implements IAssembler<UserVO, UserDTO> {

    @Override
    public UserDTO sourceToTarget(UserVO var) {
        UserDTO userDTO = new UserDTO();
        BeanCopier beanCopier = BeanCopier.create(var.getClass(), userDTO.getClass(), false);
        beanCopier.copy(var, userDTO, null);
        return userDTO;
    }

}
```

- 推荐：★★★☆☆
- 性能：★★★★☆
- 手段：基于CGlib字节码操作生成get、set方法
- 点评：整体性能很不错，使用也不复杂，可以使用
<a name="e75wW"></a>
### 8、Orika
```java
@Component
public class OrikaAssembler implements IAssembler<UserVO, UserDTO> {

    /**
     * 构造一个MapperFactory
     */
    private static MapperFactory mapperFactory = new DefaultMapperFactory.Builder().build();

    static {
        mapperFactory.classMap(UserDTO.class, UserVO.class)
                .field("userId", "userId")  // 字段不一致时可以指定
                .byDefault()
                .register();
    }

    @Override
    public UserDTO sourceToTarget(UserVO var) {
        return mapperFactory.getMapperFacade().map(var, UserDTO.class);
    }

}
```

- 官网：[https://orika-mapper.github.io/orika-docs/](https://orika-mapper.github.io/orika-docs/)
- 推荐：★★☆☆☆
- 性能：★★★☆☆
- 手段：基于字节码生成映射对象
- 点评：测试性能不是太突出，如果使用的话需要把 MapperFactory 的构建优化成 Bean 对象
<a name="klm2O"></a>
### 9、Dozer
```java
@Component
public class DozerAssembler implements IAssembler<UserVO, UserDTO> {

    private static DozerBeanMapper mapper = new DozerBeanMapper();

    @Override
    public UserDTO sourceToTarget(UserVO var) {
        return mapper.map(var, UserDTO.class);
    }

}
```

- 官网：[http://dozer.sourceforge.net/documentation/gettingstarted.html](http://dozer.sourceforge.net/documentation/gettingstarted.html)
- 推荐：★☆☆☆☆
- 性能：★★☆☆☆
- 手段：属性映射框架，递归的方式复制对象
- 点评：性能有点差，不建议使用
<a name="Xh77y"></a>
### 10、ModelMapper
```java
@Component
public class ModelMapperAssembler implements IAssembler<UserVO, UserDTO> {

    private static ModelMapper modelMapper = new ModelMapper();

    static {
        modelMapper.addMappings(new PropertyMap<UserVO, UserDTO>() {
            @Override
            protected void configure() {
                // 属性值不一样可以自己操作
                map().setUserId(source.getUserId());
            }
        });
    }

    @Override
    public UserDTO sourceToTarget(UserVO var) {
        return modelMapper.map(var, UserDTO.class);
    }

}
```

- 官网：[http://modelmapper.org](http://modelmapper.org)
- 推荐：★★★☆☆
- 性能：★★★☆☆
- 手段：基于ASM字节码实现
- 点评：转换对象数量较少时性能不错，如果同时大批量转换对象，性能有所下降
<a name="hKlSD"></a>
### 11、JMapper
```java
JMapper<UserDTO, UserVO> jMapper = new JMapper<>(UserDTO.class, UserVO.class, new JMapperAPI()
        .add(JMapperAPI.mappedClass(UserDTO.class)
                .add(JMapperAPI.attribute("userId")
                        .value("userId"))
                .add(JMapperAPI.attribute("userNickName")
                        .value("userNickName"))
                .add(JMapperAPI.attribute("createTime")
                        .value("createTime"))
        ));
```

- 官网：[https://github.com/jmapper-framework/jmapper-core/wiki](https://github.com/jmapper-framework/jmapper-core/wiki)
- 推荐：★★★★☆
- 性能：★★★★★
- 手段：Elegance, high performance and robustness all in one java bean mapper
- 点评：速度真心可以，不过结合 SpringBoot 感觉有的一点点麻烦，可能姿势不对
<a name="kaTAj"></a>
### 12、MapStruct
```java
@Mapper(componentModel = "spring", unmappedTargetPolicy = ReportingPolicy.IGNORE, unmappedSourcePolicy = ReportingPolicy.IGNORE)
public interface UserDTOMapping extends IMapping<UserVO, UserDTO> {

    /** 用于测试的单例 */
    IMapping<UserVO, UserDTO> INSTANCE = Mappers.getMapper(UserDTOMapping.class);

    @Mapping(target = "userId", source = "userId")
    @Mapping(target = "createTime", dateFormat = "yyyy-MM-dd HH:mm:ss")
    @Override
    UserDTO sourceToTarget(UserVO var1);

    @Mapping(target = "userId", source = "userId")
    @Mapping(target = "createTime", dateFormat = "yyyy-MM-dd HH:mm:ss")
    @Override
    UserVO targetToSource(UserDTO var1);

}
```

- 官网：[https://github.com/mapstruct/mapstruct](https://github.com/mapstruct/mapstruct)
- 推荐：★★★★★
- 性能：★★★★★
- 手段：直接在编译期生成对应的get、set，像手写的代码一样
- 点评：速度很快，不需要到运行期处理，结合到框架中使用方便
<a name="wTdhM"></a>
## 四、总结

- 其实对象属性转换的操作无非是基于反射、AOP、CGlib、ASM、Javassist 在编译时和运行期进行处理，再有好的思路就是在编译前生成出对应的get、set，就像手写出来的一样。
- 所以更推荐 MapStruct，这货用起来还是比较舒服的，一种是来自于功能上的拓展性，易用性和兼容性。
- 无论哪种使用，都要做一下完整的测试和验证，不要上来就复制粘贴，否则可能早早的就把挖好坑了。
