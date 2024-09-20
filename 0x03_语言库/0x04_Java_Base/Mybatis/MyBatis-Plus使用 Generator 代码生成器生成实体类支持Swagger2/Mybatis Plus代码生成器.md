JavaMybatis Plus<br />一键生成Mybatis Plus的相关Entity、Mapper、Mapper XML、Service、Controller 等各个模块的代码。
<a name="aEGbp"></a>
### 1、官方教程
参考文档如下：[https://mybatis.plus/guide/generator.html](https://mybatis.plus/guide/generator.html)<br />其实官方教程写的很详细了，这里我在详细的讲解一下官方的文档。
<a name="VUm4o"></a>
### 2、添加依赖
MyBatis-Plus 从 3.0.3 之后移除了代码生成器与模板引擎的默认依赖，需要手动添加相关依赖。
<a name="xLH5w"></a>
#### ①、添加代码生成器依赖
```xml
<dependency>
    <groupId>com.baomidou</groupId>
    <artifactId>mybatis-plus-generator</artifactId>
    <version>3.3.2</version>
</dependency>
```
<a name="kEAJr"></a>
#### ②、添加 模板引擎 依赖
Mybatis Plus 提供了三种模板引擎，另外也可以通过继承类 com.baomidou.mybatisplus.generator.engine.AbstractTemplateEngine，来自定义模板引擎。<br />通常使用导入默认的依赖就好。
```xml
<dependency>
    <groupId>org.apache.velocity</groupId>
    <artifactId>velocity-engine-core</artifactId>
    <version>2.2</version>
</dependency>
```
<a name="SaPnb"></a>
### 3、编写配置
主要包括全局配置`GlobalConfig`（生成文件的目录，作者名等），数据源配置 `DataSourceConfig`，模板配置 `TemplateConfig`。<br />下面贴上完整代码，具体可以查看代码解释。<br />GitHub地址如下：[https://github.com/YSOcean/mybatisplusstudy.git](https://github.com/YSOcean/mybatisplusstudy.git)<br />另外，代码中的相关配置可以参考官网：[https://mybatis.plus/config/generator-config.html](https://mybatis.plus/config/generator-config.html)
```java
package com.ys.mybatisplusstudy.generator;
 
import com.baomidou.mybatisplus.core.exceptions.MybatisPlusException;
import com.baomidou.mybatisplus.core.toolkit.StringPool;
import com.baomidou.mybatisplus.core.toolkit.StringUtils;
import com.baomidou.mybatisplus.generator.AutoGenerator;
import com.baomidou.mybatisplus.generator.InjectionConfig;
import com.baomidou.mybatisplus.generator.config.*;
import com.baomidou.mybatisplus.generator.config.po.TableInfo;
import com.baomidou.mybatisplus.generator.config.rules.NamingStrategy;
import com.baomidou.mybatisplus.generator.engine.FreemarkerTemplateEngine;
 
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;
 
/**
 * 执行 main 方法控制台输入模块表名回车自动生成对应项目目录中
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
        // 代码生成器
        AutoGenerator mpg = new AutoGenerator();
 
        // 全局配置
        GlobalConfig gc = new GlobalConfig();
        String projectPath = System.getProperty("user.dir");
        //生成文件的目录
        gc.setOutputDir(projectPath + "/src/main/java");
        //生成文件的作者声明
        gc.setAuthor("IT可乐");
        //是否打开输出目录
        gc.setOpen(false);
        //是否开启 BaseResultMap
        gc.setBaseResultMap(true);
        //是否开启baseColumnList
        gc.setBaseColumnList(true);
        //实体属性 Swagger2 注解
        gc.setSwagger2(true);
        mpg.setGlobalConfig(gc);
 
        // 数据源配置
        DataSourceConfig dsc = new DataSourceConfig();
        dsc.setUrl("jdbc:mysql://localhost:3306/mybatisplus?useUnicode=true&characterEncoding=UTF-8");
        // dsc.setSchemaName("public");
        dsc.setDriverName("com.mysql.jdbc.Driver");
        dsc.setUsername("root");
        dsc.setPassword("root");
        mpg.setDataSource(dsc);
 
        // 包配置
        PackageConfig pc = new PackageConfig();
        pc.setModuleName(scanner("模块名"));
        pc.setParent("com.ys.mybatisplusstudy.generator");
        mpg.setPackageInfo(pc);
 
        // 自定义配置
        InjectionConfig cfg = new InjectionConfig() {
            @Override
            public void initMap() {
                // to do nothing
            }
        };
 
        // 如果模板引擎是 freemarker
        //String templatePath = "/templates/mapper.xml.ftl";
        // 如果模板引擎是 velocity
        String templatePath = "/templates/mapper.xml.vm";
 
        // 自定义输出配置
        List<FileOutConfig> focList = new ArrayList<>();
        // 自定义配置会被优先输出
        focList.add(new FileOutConfig(templatePath) {
            @Override
            public String outputFile(TableInfo tableInfo) {
                // 自定义输出文件名 ， 如果你 Entity 设置了前后缀、此处注意 xml 的名称会跟着发生变化！！
                return projectPath + "/src/main/resources/mapper/" + pc.getModuleName()
                        + "/" + tableInfo.getEntityName() + "Mapper" + StringPool.DOT_XML;
            }
        });
        /*
        cfg.setFileCreate(new IFileCreate() {
            @Override
            public boolean isCreate(ConfigBuilder configBuilder, FileType fileType, String filePath) {
                // 判断自定义文件夹是否需要创建
                checkDir("调用默认方法创建的目录，自定义目录用");
                if (fileType == FileType.MAPPER) {
                    // 已经生成 mapper 文件判断存在，不想重新生成返回 false
                    return !new File(filePath).exists();
                }
                // 允许生成模板文件
                return true;
            }
        });
        */
        cfg.setFileOutConfigList(focList);
        mpg.setCfg(cfg);
 
        // 配置模板
        TemplateConfig templateConfig = new TemplateConfig();
 
        // 配置自定义输出模板
        //指定自定义模板路径，注意不要带上.ftl/.vm, 会根据使用的模板引擎自动识别
        // templateConfig.setEntity("templates/entity2.java");
        // templateConfig.setService();
        // templateConfig.setController();
 
        templateConfig.setXml(null);
        mpg.setTemplate(templateConfig);
 
        // 策略配置
        StrategyConfig strategy = new StrategyConfig();
        strategy.setNaming(NamingStrategy.underline_to_camel);
        strategy.setColumnNaming(NamingStrategy.underline_to_camel);
        //strategy.setSuperEntityClass("你自己的父类实体,没有就不用设置!");
        strategy.setEntityLombokModel(true);
        strategy.setRestControllerStyle(true);
        // 公共父类
        //strategy.setSuperControllerClass("你自己的父类控制器,没有就不用设置!");
        // 写于父类中的公共字段
        strategy.setSuperEntityColumns("id");
        strategy.setInclude(scanner("表名，多个英文逗号分割").split(","));
        strategy.setControllerMappingHyphenStyle(true);
        strategy.setTablePrefix(pc.getModuleName() + "_");
        mpg.setStrategy(strategy);
        //mpg.setTemplateEngine(new FreemarkerTemplateEngine());
        mpg.execute();
    }
}
```
演示如下：<br />![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1675609135340-761b75ef-7d56-40ab-a3f2-df330b75fcfc.gif#averageHue=%23fbfbfa&clientId=u3e59d55c-4e50-4&from=paste&id=ud7575236&originHeight=499&originWidth=1079&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uda6eb667-179e-458e-b7bf-8ebc18f36f1&title=)
