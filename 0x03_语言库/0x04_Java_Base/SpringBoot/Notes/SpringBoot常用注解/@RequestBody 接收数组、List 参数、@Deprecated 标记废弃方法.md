Java SpringBoot
<a name="KqYqv"></a>
### `@RequestBody` 概述
1、`@RequestBody` 主要用来接收前端传递给后端的 json 格式的数据的(请求体中的数据的)，有一个属性 required，表示参数是否必须要传，默认为 true.<br />2、GET 方式无请求体，所以 `@RequestBody` 接收数据时，前端必须是 POST 方式进行提交，然后给页面的数据默认也是 json<br />3、同一个方法中， `@RequestBody`  与 `@RequestParam()`  可以同时使用，前者最多只能有一个，后者可以有多个，<br />4、`@RequestBody` 接收的是请求体里面的数据， `@RequestParam`  接收的是 key-value  里面的参数。
<a name="ykKY8"></a>
### 接收单个 String 参数
后台代码：
```java
/**
 * 单个参数时使用 String 类型传输，比如 int、long、flout 等等，都使用 String
 *
 * @param uid
 * @return
 */
@PostMapping("save4")
public String save4(@RequestBody String uid) {
    logger.info("保存单个参数：" + uid);
    return "保存完毕：" + uid;
}
```
前端 ajax 代码：
```javascript
$.ajax({
    //默认传输的内容类型为 key-value 型,后台使用数组、List 接收参数时,
    // headers 中必须指定内容类型为 json 类型，否则会报415错误
    headers: {
        'Content-Type': 'application/json;charset=utf8'
    },
    type: "post",
    url: "http://localhost:8080/user/save4",
    //指定 json 类型后，传输的数据理所当然应该是 json 格式。
    data: JSON.stringify({"uid": "9527"}),
    success: function (data) {
        console.log(data);
    },
    error: function (data) {
        console.log("ERROR:" + JSON.stringify(data));
    }
});
```
![](https://cdn.nlark.com/yuque/0/2022/gif/396745/1651209343287-2cf90f97-de40-4f2a-8d65-694ab8bf7bde.gif#clientId=ud9510e61-0639-4&from=paste&id=u10a654ac&originHeight=304&originWidth=884&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uf60aca6a-5b59-432e-a0d6-5c09c5d50a7&title=)
<a name="ypRME"></a>
### 接收 字符串数组 参数
后台 MVC 代码：
```java
 /**
 * @param ids ：接收页面单个：参数字符串数组。必须加 org.springframework.web.bind.annotation.RequestBody 注解。
 * 如果是整形数组，则换成  Integer[] ids 即可。
 * @return
 */
@PostMapping("save1")
public String save1(@RequestBody String[] ids) {
    logger.info("save1--->" + Arrays.asList(ids));
    return new Date() + "传输完毕：" + Arrays.asList(ids);
}
```
前端 ajax 代码：
```javascript
var data = ["18ui", "89uj", "987u", "98uj"];//如果后台接收的是整形数组，则 data 中使用整形即可，如：data = [80, 90, 100, 110,120];
$.ajax({
    //默认传输的内容类型为 key-value 型,后台使用数组、List 接收参数时,
    // headers 中必须指定内容类型为 json 类型，否则会报415错误
    headers: {
        'Content-Type': 'application/json'
    },
    type: "post",
    url: "http://localhost:8080/user/save1",
    //指定 json 类型后，传输的数据理所当然应该是 json 格式。
    data: JSON.stringify(data),
    success: function (data) {
        console.log(data);
    },
    error: function (data) {
        console.log("ERROR:" + JSON.stringify(data));
    }
});
```
<a name="q9ILn"></a>
### 接收 `List<Map<String,Object>` 参数
完全与数组同理，后台 MVC 代码：
```java
/**
 * @param ids ：接收页面单个：字符串 List。必须加 @RequestBody 注解
 * @return
 */
@PostMapping("save2")
public String save2(@RequestBody List<String> ids) {
    logger.info("save2--->" + ids);
    return new Date() + "传输完毕：" + ids;
}
```
前端 ajax 代码(与数组前端完全一致)：
```javascript
//因为 ArrayList 底层也是数组，不难明白前端都传数组
var data = [{"name": "华安", "id": 9527, "salary": 8890.98}, {"name": "张三", "id": 9528, "salary": 8790.98}];
 
$.ajax({
    //默认传输的内容类型为 key-value 型,后台使用数组、List 接收参数时,
    // headers 中必须指定内容类型为 json 类型，否则会报415错误
    headers: {
        'Content-Type': 'application/json'
    },
    type: "post",
    url: "http://localhost:8080/user/save2",
    //指定 json 类型后，传输的数据理所当然应该是 json 格式。
    data: JSON.stringify(data),
    success: function (data) {
        console.log(data);
    },
    error: function (data) {
        console.log("ERROR:" + JSON.stringify(data));
    }
});
```
```java
/**
 * http://localhost:8080/sys/backupSalary
 * 页面传入 json 数据：[{"name":"华安","id":9527,"salary":8890.98}]
 * 直接转换为 List<Map>
 *
 * @param mapList
 * @return
 * @throws JsonProcessingException
 */
@PostMapping("backupSalary")
public String backupSalary(@RequestBody List<Map<String,Object>> mapList) throws JsonProcessingException {
	System.out.println(mapList);
	ObjectMapper objectMapper = new ObjectMapper();
	String result = objectMapper.writeValueAsString(mapList);
	return result;
}
```
![](https://cdn.nlark.com/yuque/0/2022/gif/396745/1651209343340-99e91ce4-46d1-4d0c-bbc5-75b62ee9c625.gif#clientId=ud9510e61-0639-4&from=paste&id=u5ba9319a&originHeight=498&originWidth=850&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ud4c9779c-f2ff-4100-9f17-48705a291d6&title=)
<a name="DDekq"></a>
### 接收整形数组
整型 List 、Flout等其它类型也是同理，会自动转型。后台代码：
```java
/**
 * Long[] ids 参数时，前端也使用整型
 *
 * @param ids
 * @return
 */
@PostMapping("save5")
public String save5(@RequestBody Long[] ids) {
    logger.info("save5--->" + Arrays.asList(ids));
    return new Date() + "传输完毕：" + Arrays.asList(ids);
}
```
前端 ajax 代码：
```javascript
var data = [1, 2, 3, 4, 5, 10, 22];
$.ajax({
    //默认传输的内容类型为 key-value 型,后台使用数组、List 接收参数时,
    // headers 中必须指定内容类型为 json 类型，否则会报415错误
    headers: {
        'Content-Type': 'application/json;charset=utf8'
    },
    type: "post",
    url: "http://localhost:8080/user/save5",
    //指定 json 类型后，传输的数据理所当然应该是 json 格式。
    data: JSON.stringify(data),
    success: function (data) {
        console.log(data);
    },
    error: function (data) {
        console.log("ERROR:" + JSON.stringify(data));
    }
});
```
<a name="W3VIo"></a>
### 接收单个 POJO 对象
后台提供一个 POJO 对象：
```java
class User {
    private Integer uId;
    private String uName;
 
    public Integer getuId() {
        return uId;
    }
 
    public void setuId(Integer uId) {
        this.uId = uId;
    }
 
    public String getuName() {
        return uName;
    }
 
    public void setuName(String uName) {
        this.uName = uName;
    }
 
    @Override
    public String toString() {
        return "User{" +
                "uId=" + uId +
                ", uName='" + uName + '\'' +
                '}';
    }
}
```
后台 mvc 代码：
```java
/**
 * 保存单个实体
 *
 * @param user
 * @return
 */
@PostMapping("save6")
public String save6(@RequestBody User user) {
    logger.info("save6=> " + user.toString());
    return "保存完成：" + user;
}
```
前端 ajax 代码：
```javascript
var data = {"uId": 1000, "uName": "华安"};
$.ajax({
    //默认传输的内容类型为 key-value 型,后台使用数组、List 接收参数时,
    // headers 中必须指定内容类型为 json 类型，否则会报415错误
    headers: {
        'Content-Type': 'application/json;charset=utf8'
    },
    type: "post",
    url: "http://localhost:8080/user/save6",
    //指定 json 类型后，传输的数据理所当然应该是 json 格式。
    data: JSON.stringify(data),
    success: function (data) {
        console.log(data);
    },
    error: function (data) {
        console.log("ERROR:" + JSON.stringify(data));
    }
});
```
<a name="zTd5f"></a>
### 接收 POJO List 参数
POJO 数组也是同理。后台 MVC 代码：
```java
 /**
 * @param userList ：接收页面单个：POJO List。必须加 @RequestBody 注解
 * @return
 */
@PostMapping("save3")
public String save3(@RequestBody List<User> userList) {
    for (User user : userList) {
        logger.info("save3--->" + user);
    }
    return new Date() + "传输完毕：" + userList;
}
```
前端 ajax 代码：
```javascript
//后台是 List<User>，则前台使用数组([])表示 List，使用 json 对象表示 User 对象
var data = [];
for (var i = 0; i < 3; i++) {
    var user = {};//每一个 json 对象相当于后台的 User 对象，因为都是 key-value 格式。
    user.uId = (i + 1);//key 值必须与后台 POJO 属性一致。value 的类型必须与后台数据类型一致。
    user.uName = "huaAn_" + (i + 1);
    data.push(user);//将 json 对象添加到数组中
}
$.ajax({
    //默认传输的内容类型为 key-value 型,后台使用数组、List 接收参数时,
    // headers 中必须指定内容类型为 json 类型，否则会报415错误
    headers: {
        'Content-Type': 'application/json'
    },
    type: "post",
    url: "http://localhost:8080/user/save3",
    //指定 json 类型后，传输的数据理所当然应该是 json 格式。
    data: JSON.stringify(data),
    success: function (data) {
        console.log(data);
    },
    error: function (data) {
        console.log("ERROR:" + JSON.stringify(data));
    }
});
```
![](https://cdn.nlark.com/yuque/0/2022/gif/396745/1651209343357-beaeb01a-320f-47d3-838a-3f108f4a51a8.gif#clientId=ud9510e61-0639-4&from=paste&id=ua46c6137&originHeight=515&originWidth=1079&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u2a2510b1-2725-4553-861a-0707c9510e7&title=)
<a name="rCZDC"></a>
### `@Deprecated` 标记废弃方法
1、应用中对于外部正在调用或者二方库依赖的接口，不允许修改方法签名，避免对接口调用方产生影响，过时接口可以加上`@Deprecated`注解，并清晰地说明采用的新接口或新服务是什么。<br />2、`@Deprecated` 注解表示 "废弃;不赞成;不推荐使用"，可以标注的位置有：`@Target(value={CONSTRUCTOR, FIELD, LOCAL_VARIABLE, METHOD, PACKAGE, PARAMETER, TYPE})`，<br />3、比如某个类或某方法加上该注解之后，表示此方法或类不再建议使用，调用时编辑器也会自动提示删除线，但并不代表不能用，只是不推荐使用，因为还有更好的方法可以调用。
```java
public class HelloWorld {
    //id 不再推荐使用，请使用 uId
    @Deprecated
    private String id;
    private String uId;
 
    //showInfo() 方法不再推荐使用，1.5 版本开始请使用 getInfo
    @Deprecated
    public void showInfo() {
        String info = "天下归心";
        System.out.println(info);
    }   
    /**
     * @since 1.5
     */
    public String getInfo() {
        String info = "天下归心";
        return info;
    }   
}
```
![](https://cdn.nlark.com/yuque/0/2022/gif/396745/1651209343349-717dad52-109d-4141-8be9-963f37140aac.gif#clientId=ud9510e61-0639-4&from=paste&id=ube98f140&originHeight=479&originWidth=798&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u945ed9de-263a-4f8b-a23d-89c6aaec843&title=)
