<a name="xWyw8"></a>
## 一、产品需求
1. excel数据模板下载
2. excel数据导入
3. 导入得时候根据模板得校验规则来进行筛选，导入成功的返回成功列表，数据有问题得返回失败列表，失败列表支持数据编辑修正

看到需求第一眼可能就是第三列有点难度，传统得数据校验是在DTO上面加注解
```java
//第一种
public Result test1(@RequestBody @Validated TestDTO dto) {...}
//第二种
public  Result  test2(@RequestBody @Valid TestDTO dto{...}
//第三种
public Result  test3(@RequestBody @Validated(value = {SaveGroup.class}) TestDTO dto) {...}
```
TestDTO里面呢会有一些类似 `@NotNull`、`@NotBlank`、`@Size`等校验注解，这里就不列了。<br />然后再在全局异常拦截那里进行统一封装，使其放回得数据结构尽量保持统一，所以一般还得有一个`RestExceptionHandler`
```java
@ControllerAdvice
public class RestExceptionHandler {

    /**
     * 处理参数验证失败异常
     * @param e
     * @return
     */
    @ExceptionHandler(MethodArgumentNotValidException.class)
    @ResponseBody
    @ResponseStatus(HttpStatus.OK)
    private Response<?> methodArgumentNotValidException(MethodArgumentNotValidException e) {
        log.warn("MethodArgumentNotValidException", e);
        FieldError fieldError = e.getBindingResult().getFieldError();
        return ResponseUtils.create(CommonCodeEnum.VALIDATE_ERROR.getCode(), CommonCodeEnum.VALIDATE_ERROR.getMessage(), fieldError.getDefaultMessage());
    }
}
```
讲到常见得数据校验，那么画风一转，再回来看需求，可见以上需求是不满足得，首先，入参是一个文件，也就是用户传的那个excel，得先解析文件再进行数据判断，合法得放一个集合，不合法得放一个集合，再者，即使入参是一个数组，这种校验一旦不满足立马进异常处理了，无法返回给前端正确得数据结构，所以引入了解决这类需求得解决方案。
<a name="yRZKs"></a>
## 二、解决方案
<a name="RSRai"></a>
### 1、基础数据
UserExcelVO
```java
@Data
public class UserExcelVO {
    /**
     * 成功列表
     */
    private List<UserExcel> success;
    /**
     * 失败列表
     */
    private List<UserExcel> fail;
}
```
UserExcel
```java
@Data
@AllArgsConstructor
@NoArgsConstructor
public class UserExcel implements Serializable {

    @NotBlank(message = "手机号不能为空")
    @Size(max = 4)
    @ExcelProperty(value = "用户名", index = 0)
    private String name;

    @ExcelProperty(value = "年龄", index = 1)
    private Integer age;

    @Pattern(regexp = "^[1][3,4,5,7,8][0-9]{9}$$", message = "手机号不合法")
    @NotBlank(message = "手机号不能为空")
    @ExcelProperty(value = "手机号", index = 2)
    private String mobile;

    @ExcelProperty(value = "性别", index = 3)
    private String sex;
}
```
easyexcel.xsl
```
用户名 年龄 手机号 性别
张三丰1 11 23847235 男
张三丰2 12 15813847236 男
张三丰3 13 15813847237 男
张三丰4 14 17813847238 男
张三丰5 15 15818847239 男
张三丰6 16 15813847240 男
张三丰7 17 152247241 男
张三丰8 18 15813847242 男
张三丰9 19 15813847243 男
张三丰10 20 15813847244 男
张三丰11 21 15813847245 男
```
<a name="P4qbr"></a>
#### 方案一：大量if else判断校验
```java
@PostMapping("/v1/importExcel")
public UserExcelVO importExcel(@RequestParam("file") MultipartFile file){
    List<UserExcel> list = null;
    List<UserExcel> fail = new ArrayList<>();
    UserExcelVO userExcelVO = new UserExcelVO();
    String mobieReg = "^[1][3,4,5,7,8][0-9]{9}$";
    try {
        list = EasyExcel.read(file.getInputStream(),UserExcel.class,new ModelExcelListener()).sheet().doReadSync();

        list.forEach(data->{
            //处理姓名的校验
            if(StringUtils.isEmpty(data.getName())||data.getName().length()> 4 ){
                fail.add(data);
                return;
            }
            //处理手机号的校验
            if (StringUtils.isEmpty(data.getMobile())|| !data.getMobile().matches(mobieReg)) {
                fail.add(data);
                return;
            }
            //以下根据字段多少可能有n个if

        });
        userExcelVO.setFail(fail);
        list.removeAll(fail);
        userExcelVO.setSuccess(list);
    } catch (IOException e) {
        e.printStackTrace();
    }
    return userExcelVO;
}
```
<a name="D4vxs"></a>
#### 方案二：请求体加入注解进行校验
实际的业务场景，一个excel里面假如是订单数据，最少是几十个字段起步的，难道要写几十个if else吗？方案一明显是不合理的，因此使用注解的方式解决。<br />ValidationUtils
```java
public class ValidationUtils {

    public static Validator getValidator(){
        return validator;
    }

    static Validator validator;
    static{
        ValidatorFactory validatorFactory = Validation.buildDefaultValidatorFactory();
        validator=validatorFactory.getValidator();
    }
}
```
```java
@PostMapping("/v2/importExcel")
public UserExcelVO importExcelV2(@RequestParam("file") MultipartFile file){
    List<UserExcel> list = null;
    List<UserExcel> fail = new ArrayList<>();
    UserExcelVO userExcelVO = new UserExcelVO();
    try {
        list = EasyExcel.read(file.getInputStream(),UserExcel.class,new ModelExcelListener()).sheet().doReadSync();
        list.forEach(data->{
            Set<ConstraintViolation<UserExcel>> violations  =  ValidationUtils.getValidator().validate(data);
            if(violations.size()>0){
                fail.add(data);
            }
        });
        userExcelVO.setFail(fail);
        list.removeAll(fail);
        userExcelVO.setSuccess(list);
    } catch (IOException e) {
        e.printStackTrace();
    }
    return userExcelVO;
}


public static class ModelExcelListener extends AnalysisEventListener<UserExcel> {
    private List<UserExcel> datas = new ArrayList<>();
    /**
     * 通过 AnalysisContext 对象还可以获取当前 sheet，当前行等数据
     */
    @Override
    public void invoke(UserExcel data, AnalysisContext context) {
        //数据存储到list，供批量处理，或后续自己业务逻辑处理。
        log.info("读取到数据{}",data);
        datas.add(data);
        //根据业务自行处理，可以写入数据库等等

    }

    //所有的数据解析完了调用
    @Override
    public void doAfterAllAnalysed(AnalysisContext context) {
        log.info("所有数据解析完成");
    }
}
```
<a name="ItjUh"></a>
## 三、测试结果
访问

- localhost:8086/user/v1/importExcel
- localhost:8086/user/v2/importExcel

发现测试结果一致，后续写代码的时候，除了做功能，应该要考虑代码的扩展性，不然产品说加个功能，又得吭哧吭哧写代码，那这样也是太累了。<br />代码下载地址：[https://gitee.com/jike11231/mybatis-plus-demo-test.git](https://gitee.com/jike11231/mybatis-plus-demo-test.git)
