Java Mybatis-plus Generator Swagger2
<a name="EsN2k"></a>
### 1、先搭建项目，引入依赖
```xml
<parent>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-parent</artifactId>
    <version>2.1.9.RELEASE</version>
</parent>

<dependencies>
    <dependency>
        <groupId>org.springframework.boot</groupId>
        <artifactId>spring-boot-starter-web</artifactId>
    </dependency>
    <!-- jdbc -->
    <dependency>
        <groupId>org.springframework.boot</groupId>
        <artifactId>spring-boot-starter-jdbc</artifactId>
    </dependency>
    <!-- MySQL -->
    <dependency>
        <groupId>mysql</groupId>
        <artifactId>mysql-connector-java</artifactId>
    </dependency>
    <!-- lombok -->
    <dependency>
        <groupId>org.projectlombok</groupId>
        <artifactId>lombok</artifactId>
        <optional>true</optional>
    </dependency>
    <!-- mybatis-plus -->
    <dependency>
        <groupId>com.baomidou</groupId>
        <artifactId>mybatis-plus-boot-starter</artifactId>
        <version>3.1.1</version>
    </dependency>
    <dependency>
        <groupId>com.baomidou</groupId>
        <artifactId>mybatis-plus-generator</artifactId>
        <version>3.1.1</version>
    </dependency>
    <!-- swagger2 -->
    <dependency>
        <groupId>io.springfox</groupId>
        <artifactId>springfox-swagger2</artifactId>
        <version>2.9.2</version>
    </dependency>
    <!-- Velocity模板引擎 -->
    <dependency>
        <groupId>org.apache.velocity</groupId>
        <artifactId>velocity-engine-core</artifactId>
        <version>2.1</version>
    </dependency>
</dependencies>
```
<a name="EIJXT"></a>
### 2、编写代码生成器代码（有说明）
```java
/**
 * @Description: 代码生成器
 */
public class CodeGenerator {

    /**
     * <p>
     * 读取控制台内容
     * </p>
     */
    public static String scanner(String tip) {
        Scanner scanner = new Scanner(System.in);
        StringBuilder help = new StringBuilder();
        help.append("请输入" + tip + "：");
        System.out.println(help.toString());
        if (scanner.hasNext()) {
            String ipt = scanner.next();
            if (StringUtils.isNotEmpty(ipt)) {
                return ipt;
            }
        }
        throw new MybatisPlusException("请输入正确的" + tip + "！");
    }

    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        
        AutoGenerator generator = new AutoGenerator();
        // 全局变量配置
        GlobalConfig gc = new GlobalConfig();
        String projectPath = System.getProperty("user.dir"); //当前项目
        gc.setOutputDir(projectPath+"/src/main/java"); // 输出路径
        gc.setFileOverride(true); // 默认 false ,是否覆盖已生成文件
        gc.setOpen(false); //默认true ,是否打开输出目录
        gc.setEnableCache(false); // 默认false,是否开启二级缓存
        gc.setAuthor("liangshaolian"); // 作者
        gc.setSwagger2(true); //默认false
        gc.setBaseResultMap(true); // 默认false
        gc.setDateType(DateType.TIME_PACK); // 时间策略 默认TIME_PACK
        gc.setBaseColumnList(true); //默认false  和basemodel相似
        gc.setEntityName("%s");
        gc.setControllerName("%sController");
        gc.setServiceName("I%sService");
        gc.setServiceImplName("%sServiceImpl");
        gc.setMapperName("I%sMapper");
        gc.setXmlName("%sMapper");
        gc.setIdType(IdType.AUTO); // 指定生成的主键类型
        generator.setGlobalConfig(gc);

        // 数据源配置
        DataSourceConfig dc = new DataSourceConfig();
        dc.setDbQuery(new MySqlQuery()); // 数据库信息查询 //默认mysql
        dc.setDbType(DbType.MYSQL);// 数据库类型
        dc.setTypeConvert(new MySqlTypeConvert()); //类型转换 默认mysql
        dc.setUrl("jdbc:mysql://127.0.0.1:3306/数据库名?serverTimezone=CTT&useUnicode=true&characterEncoding=utf-8&useSSL=false");
        dc.setDriverName("com.mysql.cj.jdbc.Driver");
        dc.setUsername("root");
        dc.setPassword("密码");
        generator.setDataSource(dc);

        // 包配置
        PackageConfig pc = new PackageConfig();
        pc.setParent("com.liang.module");//代码生成到哪个包下面
//        pc.setModuleName(""); //此处是所属模块名称
//        pc.setEntity("entity"); //默认entity,controller,service,service.impl,mapper,mapper.xml
        generator.setPackageInfo(pc);
        // 自定义配置
        InjectionConfig cfg = new InjectionConfig() {
            @Override
            public void initMap() {
                // to do nothing
            }
        };
        /**
         * 将xml生成到resource下面
         */
        String templatePath = "/templates/mapper.xml.vm"; // Velocity模板
        // 自定义输出配置
        List<FileOutConfig> focList = new ArrayList<>();
        // 自定义配置会被优先输出
        focList.add(new FileOutConfig(templatePath) {
            @Override
            public String outputFile(TableInfo tableInfo) {
                // 自定义输出文件名 ， 如果你 Entity 设置了前后缀、此处注意 xml 的名称会跟着发生变化！！
                return projectPath + "/src/main/resources/mapper/"
//                        + pc.getModuleName()
                        + "/" + tableInfo.getEntityName() + "Mapper" + StringPool.DOT_XML;
            }
        });
        cfg.setFileOutConfigList(focList);
        generator.setCfg(cfg);

        // 配置模板
        TemplateConfig tc = new TemplateConfig();
        // templates/entity.java 模板路径配置，默认在templates目录下，.vm 后缀不用加
        tc.setEntity("templates/entity.java");//使用自定义模板生成实体类
        tc.setXml("");
        generator.setTemplate(tc);

        // 数据库表配置
        StrategyConfig sc = new StrategyConfig();
        sc.setCapitalMode(false); //是否大写命名 默认false
        sc.setSkipView(true); //是否跳过试图 默认false
        sc.setNaming(NamingStrategy.underline_to_camel);// 表映射 驼峰命名
        sc.setColumnNaming(NamingStrategy.underline_to_camel); // 字段映射 驼峰
        sc.setEntityLombokModel(true); //是否使用lombak 默认为false
        sc.setRestControllerStyle(true); // 默认false
        sc.setEntitySerialVersionUID(true); //默认true
        sc.setEntityColumnConstant(true); //默认false 将mysql字段名生成静态变量
        sc.setInclude(scanner("表名，多个英文逗号分割").split(",")); //表名，用，隔开  需要生产
   //     sc.setExclude(""); //                 不需要生成  二选一
        sc.setEntityTableFieldAnnotationEnable(true); // 默认false 注释
        sc.setControllerMappingHyphenStyle(false); //默认false
        sc.setLogicDeleteFieldName("status"); // 逻辑删除字段名称
        generator.setStrategy(sc);

        // 模板引擎
        generator.setTemplateEngine(new VelocityTemplateEngine());
        generator.execute();
    }
}

```
<a name="VGbA9"></a>
### 3、在resources目录下创建templates目录
<a name="otzhM"></a>
### 4、在templates目录下创建entity.java.vm模板(代码如下)
```java
package ${package.Entity};

#foreach($pkg in ${table.importPackages})
import ${pkg};
#end
import io.swagger.annotations.ApiModel;
import io.swagger.annotations.ApiModelProperty;
#if(${entityLombokModel})
import lombok.Data;
import lombok.EqualsAndHashCode;
##import lombok.experimental.Accessors;
#end

/**
 * @Description:$!{table.comment}
 * @author ${author}
 * @since ${date}
 */
@ApiModel(value ="$!{table.comment}")
#if(${entityLombokModel})
@Data
#if(${superEntityClass})
@EqualsAndHashCode(callSuper = true)
#else
@EqualsAndHashCode(callSuper = false)
#end
##@Accessors(chain = true)
#end
#if(${table.convert})
@TableName("${table.name}")
#end
#if(${superEntityClass})
public class ${entity} extends ${superEntityClass}#if(${activeRecord})<${entity}>#end {
#elseif(${activeRecord})
public class ${entity} extends Model<${entity}> {
#else
public class ${entity} implements Serializable {
#end

    private static final long serialVersionUID = 1L;

## ----------  BEGIN 字段循环遍历  ----------
#foreach($field in ${table.fields})
    #if(${field.keyFlag})
        #set($keyPropertyName=${field.propertyName})
    #end
    #if("$!field.comment" != "")
    @ApiModelProperty(value = "${field.comment}")
    #end
    #if(${field.keyFlag})
    ## 主键s
#if(${field.keyIdentityFlag})
@TableId(value = "${field.name}", type = IdType.AUTO)
#elseif(!$null.isNull(${idType}) && "$!idType" != "")
@TableId(value = "${field.name}", type = IdType.${idType})
#elseif(${field.convert})
@TableId("${field.name}")
#end
    ## 普通字段
    #elseif(${field.fill})
    ## -----   存在字段填充设置   -----
    #if(${field.convert})
    @TableField(value = "${field.name}", fill = FieldFill.${field.fill})
    #else
    @TableField(fill = FieldFill.${field.fill})
    #end
    #elseif(${field.convert})
    @TableField("${field.name}")
    #end
## 乐观锁注解
    #if(${versionFieldName}==${field.name})
    @Version
    #end
## 逻辑删除注解
    #if(${logicDeleteFieldName}==${field.name})
    @TableLogic
    #end
    private ${field.propertyType} ${field.propertyName};
#end
## ----------  END 字段循环遍历  ----------

#if(!${entityLombokModel})
    #foreach($field in ${table.fields})
        #if(${field.propertyType.equals("boolean")})
            #set($getprefix="is")
        #else
            #set($getprefix="get")
        #end

    public ${field.propertyType} ${getprefix}${field.capitalName}() {
        return ${field.propertyName};
    }

    #if(${entityBuilderModel})
    public ${entity} set${field.capitalName}(${field.propertyType} ${field.propertyName}) {
    #else
    public void set${field.capitalName}(${field.propertyType} ${field.propertyName}) {
        #end
        this.${field.propertyName} = ${field.propertyName};
        #if(${entityBuilderModel})
        return this;
        #end
    }
    #end
#end

#if(${entityColumnConstant})
    #foreach($field in ${table.fields})
    public static final String ${field.name.toUpperCase()} = "${field.name}";

    #end
#end
#if(${activeRecord})
@Override
protected Serializable pkVal() {
    #if(${keyPropertyName})
            return this.${keyPropertyName};
    #else
            return null;
    #end
        }
#end
#if(!${entityLombokModel})
@Override
public String toString() {
        return "${entity}{" +
    #foreach($field in ${table.fields})
        #if($!{velocityCount}==1)
                "${field.propertyName}=" + ${field.propertyName} +
        #else
                ", ${field.propertyName}=" + ${field.propertyName} +
        #end
    #end
        "}";
    }
#end
}
```
<a name="eQg47"></a>
### 5、基本完成只要运行代码生成器代码，输入表名即可
生成的实体类效果如下:
```java
/**
 * @Description:汽车品牌表
 */
@ApiModel(value ="汽车品牌表")
@Data
@EqualsAndHashCode(callSuper = false)
@TableName("tn_pur_brand")
public class TnPurBrand implements Serializable {

    private static final long serialVersionUID = 1L;

    @ApiModelProperty(value = "主键")
    @TableId(value = "id", type = IdType.AUTO)
    private Long id;
    @ApiModelProperty(value = "首字母")
    @TableField("initials")
    private String initials;
    @ApiModelProperty(value = "汽车品牌")
    @TableField("brand_name")
    private String brandName;
    @ApiModelProperty(value = "品牌LOGO")
    @TableField("logo")
    private String logo;

    public static final String ID = "id";

    public static final String INITIALS = "initials";

    public static final String BRAND_NAME = "brand_name";

    public static final String LOGO = "logo";

}
```
