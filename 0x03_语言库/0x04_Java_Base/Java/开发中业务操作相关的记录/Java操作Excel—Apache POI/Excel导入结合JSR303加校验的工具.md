Java Excel 
<a name="AHH3e"></a>
## easyexcel 库
POI是Java操作Excel的基础库。为了通用性并没有做定制，而且还有一些局限性。这里采用二次封装库easyexcel来进行业务开发。
```xml
<dependency>
    <groupId>com.alibaba</groupId>
    <artifactId>easyexcel</artifactId>
    <version>${easyexcel.version}</version>
</dependency>
```
easyexcel将读取Excel的生命周期抽象为了几个阶段，方便在各个阶段注入想要实现的逻辑。这几个阶段包含在ReadListener接口中
```java
public interface ReadListener<T> extends Listener {
    /**
     * 当任何一个侦听器执行错误报告时，所有侦听器都将接收此方法。 如果在此处引发异常，则整个读取将终止。
     * 这里是处理读取excel异常的
     *
     * @param exception
     * @param context
     * @throws Exception
     */
    void onException(Exception exception, AnalysisContext context) throws Exception;

    /**
     * 读取每行excel表头时会执行此方法
     *
     * @param headMap
     * @param context
     */
    void invokeHead(Map<Integer, CellData> headMap, AnalysisContext context);

    /**
     * 读取每行数据的时候回执行此方法 
     *
     * @param data
     *            one row value. Is is same as {@link AnalysisContext#readRowHolder()}
     * @param context
     *            analysis context
     */
    void invoke(T data, AnalysisContext context);

    /**
     * 如果有额外的单元格信息返回就用此方法处理
     *
     * @param extra
     *            extra information
     * @param context
     *            analysis context
     */
    void extra(CellExtra extra, AnalysisContext context);

    /**
     * 在整个excel sheet解析完毕后执行的逻辑。
     *
     * @param context
     */
    void doAfterAllAnalysed(AnalysisContext context);

    /**
     * 用来控制是否读取下一行的策略
     *
     * @param context
     * @return
     */
    boolean hasNext(AnalysisContext context);
}
```
其抽象实现`AnalysisEventListener<T>`提供更加符合需要的抽象，进一步实现这个抽象来实现Excel的导入和校验。<br />在了解一个框架的抽象接口后，尽量要去看一下它有没有能满足需要的实现。<br />另外这里要多说一点，接口中的`AnalysisContext`包含了很多有用的上下文元信息，比如当前行、当前的配置策略、excel整体结构等信息，可以在需要的时候调用这些信息。
<a name="TdKRs"></a>
## JSR303校验
业界已经有的规范-JSR303校验规范，它将数据模型（Model）和校验（Validation）各自抽象，非常灵活，而且工作量明显降低。只需要找到和esayexcel生命周期结合的地方就行了。只需要引入以下依赖就能在Spring Boot项目中集成JSR303校验：
```xml
<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-validation</artifactId>
</dependency>
```
<a name="EddfC"></a>
## 实现过程
可以在解析每个字段的时候校验，这对应`ReadListener`的`invoke(T data, AnalysisContext context)`方法，这种方式可以实现当字段校验触发约束时就停止excel解析的策略；另一种可以在Excel解析完毕后执行校验，对应`doAfterAllAnalysed(AnalysisContext context)`。这里以第二种为例来实现一下。<br />在编写代码时，尽量职责单一，一个类或者一个方法尽量只干一个事，这样让自己的代码足够清晰。
<a name="PQdgR"></a>
### 编写校验处理类
这里把解析和校验分开实现，先编写JSR303校验工具。这里假设已经有了校验器javax.validation.Validator的实现。
```java
import cn.fcant.validate.Excel;
import lombok.AllArgsConstructor;
import org.springframework.util.StringUtils;

import javax.validation.ConstraintViolation;
import javax.validation.Validator;
import java.util.*;
import java.util.stream.Collectors;

/**
 *  excel 校验工具
 *
 * @param <T> the type parameter
 */
@AllArgsConstructor
public class ExcelValidator<T> {

    private final Validator validator;
    private final Integer beginIndex;


    /**
     *  集合校验
     *
     * @param data 待校验的集合
     * @return list
     */
    public List<String> validate(Collection<T> data) {
        int index = beginIndex + 1;
        List<String> messages = new ArrayList<>();
        for (T datum : data) {
            String validated = this.doValidate(index, datum);
            if (StringUtils.hasText(validated)) {
                messages.add(validated);
            }
            index++;
        }
        return messages;
    }
    
    /**
     * 这里是校验的根本方法
     *
     * @param index 本条数据所在的行号
     * @param data 待校验的某条数据
     * @return 对数据的校验异常进行提示，如果有触发校验规则的会封装提示信息。
     */
    private String doValidate(int index, T data) {
        // 这里使用了JSR303的的校验器，同时使用了分组校验，Excel为分组标识
        Set<ConstraintViolation<T>> validate = validator.validate(data, Excel.class);
        return validate.size()>0 ? "第" + index +
                "行，触发约束：" + validate.stream()
                .map(ConstraintViolation::getMessage)
                .collect(Collectors.joining(",")): "";
    }
}
```
上面就是整个校验的逻辑，如果校验通过不提示任何信息，如果校验不通过把校验的约束信息封装返回。这里的Validator是从哪里来的呢？当Spring Boot集成了JSR303会有一个Validator实现被自动注入Spring IoC，可以利用它来完成校验操作。
<a name="xjdF5"></a>
### 实现AnalysisEventListener
这个完全是easyexcel的功能了，只需要实现最开始提到的Excel抽象解析监听器接口`AnalysisEventListener`，并将解析字段加入集合，等完全解析完毕后再进行校验。这里如果校验不通过就会抛出携带校验信息的异常，异常经过处理返回前端提示。<br />切记：`AnalysisEventListener`的实现不能注入Spring IoC。
```java
import cn.hutool.json.JSONUtil;
import com.alibaba.excel.context.AnalysisContext;
import com.alibaba.excel.event.AnalysisEventListener;
import cn.fcant.exception.ServiceException;
import org.springframework.util.CollectionUtils;

import java.util.ArrayList;
import java.util.Collection;
import java.util.List;
import java.util.function.Consumer;

/**
 * 该类不可被Spring托管
 *
 * @param <T> the type parameter
 */
public class JdbcEventListener<T> extends AnalysisEventListener<T> {
    /**
     * Excel总条数阈值
     */
    private static final Integer MAX_SIZE = 10000;
    /**
     * 校验工具
     */
    private final ExcelValidator<T> excelValidator;
    /**
     * 如果校验通过消费解析得到的excel数据
     */
    private final Consumer<Collection<T>> batchConsumer;
    /**
     * 解析数据的临时存储容器
     */
    private final List<T> list = new ArrayList<>();

    /**
     * Instantiates a new Jdbc event listener.
     *
     * @param excelValidator Excel校验工具
     * @param batchConsumer  Excel解析结果批量消费工具，可实现为写入数据库等消费操作
     */
    public JdbcEventListener(ExcelValidator<T> excelValidator, Consumer<Collection<T>> batchConsumer) {
        this.excelValidator = excelValidator;
        this.batchConsumer = batchConsumer;
    }

    @Override
    public void onException(Exception exception, AnalysisContext context) throws Exception {
        list.clear();
        throw exception;
    }

    @Override
    public void invoke(T data, AnalysisContext context) {
        // 如果没有超过阈值就把解析的excel字段加入集合
        if (list.size() >= MAX_SIZE) {
            throw new ServiceException("单次上传条数不得超过：" + MAX_SIZE);
        }
        list.add(data);
    }

    @Override
    public void doAfterAllAnalysed(AnalysisContext context) {
        //全部解析完毕后 对集合进行校验并消费
        if (!CollectionUtils.isEmpty(this.list)) {
            List<String> validated = this.excelValidator.validate(this.list);
            if (CollectionUtils.isEmpty(validated)) {
                this.batchConsumer.accept(this.list);
            } else {
                throw new ServiceException(JSONUtil.toJsonStr(validated));
            }
        }
    }
}
```
<a name="vVMhw"></a>
### 封装最终的工具
这里参考esayexcel的文档封装成一个通用的Excel读取工具
```java
import com.alibaba.excel.EasyExcel;
import lombok.AllArgsConstructor;
import lombok.Data;

import javax.validation.Validator;
import java.io.InputStream;
import java.util.Collection;
import java.util.function.Consumer;

/**
 * excel读取工具
 *
 */
@AllArgsConstructor
public class ExcelReader {
    private final Validator validator;

    /**
     * Read Excel.
     *
     * @param <T>  the type parameter
     * @param meta the meta
     */
    public <T> void read(Meta<T> meta) {
        ExcelValidator<T> excelValidator = new ExcelValidator<>(validator, meta.headRowNumber);
        JdbcEventListener<T> readListener = new JdbcEventListener<>(excelValidator, meta.consumer);
        EasyExcel.read(meta.excelStream, meta.domain, readListener)
                .headRowNumber(meta.headRowNumber)
                .sheet()
                .doRead();
    }


    /**
     * 解析需要的元数据
     *
     * @param <T> the type parameter
     */
    @Data
    public static class Meta<T> {
        /**
         * excel 文件流
         */
        private InputStream excelStream;
        /**
         * excel头的行号，参考easyexcel的api和你的实际情况
         */
        private Integer headRowNumber;
        /**
         * 对应excel封装的数据类，需要参考easyexcel教程
         */
        private Class<T> domain;
        /**
         * 解析结果的消费函数
         */
        private Consumer<Collection<T>> consumer;
    }

}
```
把这个工具注入Spring IoC，方便使用。
```java
/**
 * Excel 读取工具
 *
 * @param validator the validator
 * @return the excel reader
 */
@Bean
public ExcelReader excelReader(Validator validator) {
    return new ExcelReader(validator);
}
```
<a name="lE7UZ"></a>
### 编写接口
这里Excel的数据类ExcelData就不赘述了，过于简单！去看esayexcel的文档即可。编写一个Spring MVC接口示例。
```java
@Autowired
private  ExcelReader excelReader;
@Autowired
private  DataService dataService;

@PostMapping("/excel/import")
public Rest<?> importManufacturerInfo(@RequestPart MultipartFile file) throws IOException {
    InputStream inputStream = file.getInputStream();
    ExcelReader.Meta<ExcelData> excelDataMeta = new ExcelReader.Meta<>();
    excelDataMeta.setExcelStream(inputStream);
    excelDataMeta.setDomain(ExcelData.class);
    excelDataMeta.setHeadRowNumber(2);
    // 批量写入数据库的逻辑
    excelDataMeta.setConsumer(dataService::saveBatch);
    this.excelReader.read(excelDataMeta);
    return RestBody.ok();
}
```
