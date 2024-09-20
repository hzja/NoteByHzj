IDEAPlugins
<a name="q9gAy"></a>
## 1、什么是DTO、VO、BO、PO、DO、POJO
POJO的定义是无规则简单的对象，在日常的代码分层中pojo会被分为VO、BO、 PO、 DTO
<a name="IeaYy"></a>
### VO （view object/value object）表示层对象
1、前端展示的数据，在接口数据返回给前端的时候需要转成VO<br />2、使用场景，接口层服务中，将DTO转成VO，返回给前台
<a name="y1LGM"></a>
### BO（bussines object）业务层对象
1、主要在服务内部使用的业务对象<br />2、可以包含多个对象，可以用于对象的聚合操作<br />3、使用场景在服务层服务中，由DTO转成BO然后进行业务处理后，转成DTO返回到接口层
<a name="uTOxT"></a>
### PO（persistent object）持久对象
1、出现位置为数据库数据，用来存储数据库提取的数据<br />2、只存储数据，不包含数据操作<br />3、使用场景在数据库层中，获取的数据库数据存储到PO中，然后转为DTO返回到服务层中
<a name="LAbN9"></a>
### DTO（Data Transfer Object）数据传输对象
1、在服务间的调用中，传输的数据对象<br />2、DTO是可以存在于各层服务中（接口、服务、数据库等等）服务间的交互使用DTO来解耦<br />DO（domain object）领域实体对象
<a name="y5xBZ"></a>
### DO 现在主要有两个版本：
①阿里巴巴的开发手册中的定义，DO（ Data Object）这个等同于上面的PO<br />②DDD（Domain-Driven Design）领域驱动设计中，DO（Domain Object）这个等同于上面的BO
<a name="AUYHV"></a>
## 2、插件如何完成转化
插件名称：Simple Object Copy<br />1、定义方法出入参<br />2、光标定位方法内，使用快捷键ALT+INSERT(WIN) 、 command + N(mac) ，或者右键鼠标选择Generate，弹出生成选项框后，选择genCopyMethod，代码就生成好了<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1642748763166-5d798b73-3d27-4ace-8319-1c3044eac701.webp#averageHue=%233a3c40&clientId=u4e6adf06-b8f2-4&errorMessage=unknown%20error&from=paste&id=u363c3587&originHeight=509&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=error&style=none&taskId=u452ec72b-82cf-41b5-af87-6d5e742d819&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1642748763217-c9464b01-f6c0-4577-a341-73e3754d43d1.webp#averageHue=%232f3332&clientId=u4e6adf06-b8f2-4&errorMessage=unknown%20error&from=paste&id=u1cca3ab1&originHeight=445&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=error&style=none&taskId=u3a20584e-b468-4051-935f-7d82da71d39&title=)<br />复杂对象转化展示<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1642748763110-a32ef4a6-0191-48a8-b5cf-56982a3b6a41.webp#averageHue=%23333639&clientId=u4e6adf06-b8f2-4&errorMessage=unknown%20error&from=paste&id=u69d9391e&originHeight=937&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=error&style=none&taskId=u9daecff7-4588-4a31-b87b-15ca5c5b5d7&title=)<br />代码展示：
```java
@Data
public class UserVO {
    private String name;
    private Date entryDate;
    private String userId;
    private List roleList;
    private RoomVO room;
    public static UserVO convertToUserVO(UserDTO item) {
        if (item == null) {
            return null;
        }
        UserVO result = new UserVO();
        result.setName(item.getName());
        result.setEntryDate(item.getEntryDate());
        result.setUserId(item.getUserId());
        List<RoleDTO> roleList = item.getRoleList();
        if (roleList == null) {
            result.setRoleList(null);
        } else {
            result.setRoleList(roleList.stream().map(UserVO::convertToRoleVO).collect(Collectors.toList());
        }
        result.setRoom(convertToRoomVO(item.getRoom()));
        return result;
    }

    public static RoomVO convertToRoomVO(RoomDTO item) {
        if (item == null) {
            return null;
        }
        RoomVO result = new RoomVO();
        result.setRoomId(item.getRoomId());
        result.setBuildingId(item.getBuildingId());
        result.setRoomName();
        result.setBuildingName();
        return result;
    }

    public static RoleVO convertToRoleVO(RoleDTO item) {
        if (item == null) {
            return null;
        }
        RoleVO result = new RoleVO();
        result.setRoleId(item.getRoleId());
        result.setRoleName(item.getRoleName());
        result.setCreateTime(item.getCreateTime());
        return result;
    }

}
@Data
public class UserDTO {
    private String name;
    private Date entryDate;
    private String userId;
    private List roleList;
    private RoomDTO room;
}
@Data
public class RoleVO {
    private String roleId;
    private String roleName;
    private LocalDateTime createTime;
}
@Data
public class RoleDTO {
    private String roleId;
    private String roleName;
    private LocalDateTime createTime;
}
@Data
public class RoomVO {
    private String roomId;
    private String buildingId;
    private String roomName;
    private String buildingName;
}
@Data
public class RoomDTO {
    private String roomId;
    private String buildingId;
}
```
<a name="Zu8UT"></a>
## 3、其他转化方式
<a name="KFGyI"></a>
### 无入侵
市面上有很多类似的工具类，比较常用的有

- Spring BeanUtils （copyProperties）
- Cglib  BeanCopier （copyProperties）
- Apache BeanUtils （copyProperties）
- Apache PropertyUtils （copyProperties）
- Dozer
- mapstruct
- JSON 序列化 再反序列化

这些工具，不仅要引入相应的依赖jar包，而且对代码有入侵，要调用对应得api方法才能进行转化，一旦遇到类型不一致，字段名稍有变动，就需要另写java代码补全字段，整体代码非常丑陋。
<a name="MngRo"></a>
### 性能优势
相比上面的工具类，不是使用反射、就是是用代理、序列化操作。相比于纯正的set方法去转化，差距不是一个量级。此次不赘述。
<a name="GdfwI"></a>
### 灵活性、兼容性
跟上述工具类相比插件有很大优势，不再赘述，下面比较一下常用的idea插件generateO2O

| 插件 | 特点 |
| --- | --- |
| Simple Object Copy | 依据返回值为主，根据字段名去匹配，不会导致返回值漏属性 |
|  | 支持对象包含对象、对象包含list、set集合的转化 |
| generateO2O | 以入参为主匹配字段，存在漏属性的情况 |
|  | 不支持子对象的转化、不支持list泛型不同的转化 |

在此推荐一个其他常用插件：generateAllSetter，搭配使用更佳
<a name="ayhFn"></a>
## 4、如何下载
打开idea plugins，切market place 搜索：Simple Object Copy<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1642748894038-5968ecfd-a5c5-4901-980f-beb13eda9abe.png#averageHue=%23d3ac6e&clientId=u4e6adf06-b8f2-4&errorMessage=unknown%20error&from=paste&height=706&id=u48722dcb&originHeight=1766&originWidth=2368&originalType=binary&ratio=1&rotation=0&showTitle=false&size=238967&status=error&style=shadow&taskId=u293e0f26-09d1-4796-bb81-2bce5f6f585&title=&width=947.2)
<a name="csU3J"></a>
## 5、插件优点
1、可以节省一个个字段的设置的开发时间<br />2、避免了漏字段设置，ps：前端同学总是来问为啥字段总是null。<br />3、而且通过出入参的设计思想去开发，规范了代码，在有特殊请求转化的时候也比较方便。
:::danger
注意：该插件需要付费，6元（人民币）每年，当然学生、教育机构、公益免费。
:::
