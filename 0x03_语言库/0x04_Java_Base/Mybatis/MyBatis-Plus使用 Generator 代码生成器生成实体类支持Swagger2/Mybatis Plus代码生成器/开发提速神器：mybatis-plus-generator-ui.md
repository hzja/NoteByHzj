JavaMybatis
<a name="edGdf"></a>
## **前言**
在基于Mybatis的开发模式中，很多开发者还会选择Mybatis-Plus来辅助功能开发，以此提高开发的效率。虽然Mybatis也有代码生成的工具，但Mybatis-Plus由于在Mybatis基础上做了一些调整，因此，常规的生成工具生成的代码还有一些不太符合预期。而且对于多数据库的支持不是很好。<br />因此，需要一款支持高度定制化，带图形UI页面，能适配多数数据库的基础程序生成框架。本文就介绍这款基于Mybatis-Plus的代码自助生成器，github地址:mybatis-plus-generator-ui。<br />这里通过实例集成的方式来详细讲解mybatis-plus-generator-ui，感兴趣的朋友可以自己clone下来，也可以自己进行扩展自定义。
<a name="Yb1af"></a>
## **一、mybatis-plus-generator-ui是什么？**
它是对mybatis-plus-generator进行封装，通过Web UI快速生成兼容Spring boot，mybatis-plus框架的各类业务代码。提供交互式的Web UI用于生成兼容mybatis-plus框架的相关功能代码，包括Entity、Mapper、Mapper.xml、Service、Controller等，可以自定义模板以及各类输出参数，也可通过SQL查询语句直接生成代码。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1680051653295-a4eb8697-d567-4e62-b194-a0e1dc5cfd86.png#averageHue=%23ded7cc&clientId=uc383ce32-2432-4&from=paste&id=ud91f4aa9&originHeight=388&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uc43620e3-f5ce-445f-b0c5-7add628c49d&title=)

![](https://cdn.nlark.com/yuque/0/2023/png/396745/1680051653315-1f714f74-5f61-43a8-85f6-6045a0519b7b.png#averageHue=%232c2e2d&clientId=uc383ce32-2432-4&from=paste&id=u65bba612&originHeight=596&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u578976e3-005b-47d2-a6d9-6fce4efb178&title=)<br />功能列表：

- **Table查询：**查询配置的关系型数据库表的列表查询。
- **输出配置：**对需要生成的相关代码，比如Entity、Mapper、Servcie、Controller等代码模板信息进行配置，用于在转换时调用。
- **项目导入：**可以导入其它项目配置好的信息给本项目使用。
- **下载模板：**支持本项目配置的模板信息下载后共享。
- **策略配置：**直接定义各种文件的生成策略。
- **模板上传：**支持从别的项目中下载模板，同上传供本项目使用。
- **SQL输入上传：**支持将查询语句直接上传或者复制到输入框中。
- **SQL代码生成：**基于SQL脚本生成相应的代码。
<a name="x8FWw"></a>
## **二、 mybatis-plus-generator-ui怎么用？**
mybatis-plus-generator-ui提供以jar包的形式为外部项目提供服务，通过配置的数据库配置去读取数据库的配置信息，并通过Web UI的方式提供给开发者使用。mybatis-plus-generator-ui支持POSTGRE_SQL、ORACLE、DB2、MySQL、SQLSERVER等常见的关系型数据库。
<a name="mKVGT"></a>
### 1、maven pom引入
```xml
<project xmlns="http://maven.apache.org/POM/4.0.0"
  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
  xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
  <modelVersion>4.0.0</modelVersion>
  <groupId>com.yelang</groupId>
  <artifactId>mybatis-plus-generator-ui-case</artifactId>
  <version>0.0.1-SNAPSHOT</version>

  <dependencies>
    <dependency>
      <groupId>com.github.davidfantasy</groupId>
      <artifactId>mybatis-plus-generator-ui</artifactId>
      <version>1.4.5</version>
    </dependency>

    <dependency>
      <groupId>org.postgresql</groupId>
      <artifactId>postgresql</artifactId>
      <version>42.2.25</version>
    </dependency>
  </dependencies>
</project>
```
<a name="JlisL"></a>
### 2、新建程序入口，以main函数的方式运行
mybatis-plus-generator-ui在1.4.0版本之后，可支持将`GeberatorUIServer`独立部署为一个单独的spring boot项目，通过页面指定目标项目根目录的方式为多个项目提供源码生成服务。这种方式适用于有多个项目库需要独立进行开发的模式。实例关键代码如下：
```java
package com.yelang;

import com.github.davidfantasy.mybatisplus.generatorui.GeneratorConfig;
import com.github.davidfantasy.mybatisplus.generatorui.MybatisPlusToolsApplication;
import com.github.davidfantasy.mybatisplus.generatorui.mbp.NameConverter;

public class GeneratorMain {

    public static void main(String[] args) {
        GeneratorConfig config = GeneratorConfig.builder().jdbcUrl("jdbc:postgresql://127.0.0.1:5432/ghyapp")
        .userName("ghy01").password("ghy01").driverClassName("org.postgresql.Driver")
        // 数据库schema，POSTGRE_SQL,ORACLE,DB2类型的数据库需要指定
        // .schemaName("myBusiness")
        // 如果需要修改各类生成文件的默认命名规则，可自定义一个NameConverter实例，覆盖相应的名称转换方法：
        .nameConverter(new NameConverter() {
            /**
             * 自定义Service类文件的名称规则
             */
            public String serviceNameConvert(String tableName) {
                return this.entityNameConvert(tableName) + "Service";
            }

            /**
             * 自定义Controller类文件的名称规则
             */
            public String controllerNameConvert(String tableName) {
                return this.entityNameConvert(tableName) + "Action";
            }
        }).basePackage("com.github.davidfantasy.mybatisplustools.example").port(8068).build();

        MybatisPlusToolsApplication.run(config);

    }

}
```
在上面的配置中，连接的示例数据库是PostgerSQL，需要在Maven中定义相应的驱动程序，并且在上述代码中正确配置相应的类。最后指定了程序的运行端口为8086，这种运行方式跟SpringBoot非常相似。
<a name="XnJAN"></a>
### 3、实例运行
运行以上的main方法，在控制台可以看到以下输出即为成功部署。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1680051653336-6241a791-d609-4fb5-b1e2-01e9654197ea.png#averageHue=%23f6f5f3&clientId=uc383ce32-2432-4&from=paste&id=u6e32332d&originHeight=414&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u6b8ba616-db62-4b72-9982-60eb1d84b2b&title=)<br />在输出的日志中，可以看到程序的运行端口，以及默认的模板目录地址。在浏览器中输入访问地址http://localhost:8068/，即可进行配置生成。
<a name="VEdYx"></a>
## **三、mybatis-plus-generator-ui代码生成**
<a name="JGowR"></a>
### 1、Table的查询和浏览
可以直接浏览和查询配置的数据源的数据表信息，可选择一个或多个生成模板代码。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1680051653303-8ddf11b3-c5e4-4d97-b6a2-4f34507f9887.png#averageHue=%23e8d9c2&clientId=uc383ce32-2432-4&from=paste&id=uec94b0d4&originHeight=618&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u3416380c-bfce-40e8-8139-d0ada288044&title=)
<a name="UK7v0"></a>
### 2、输出配置
内置Entity、Mapper、Service、Controller等6种类型代码的模板配置，可以上传模板进行替换，并修改各类参数，配置参数已经按照影响的文件类型重新进行了分类，并加入了部分文本说明；也可以自行添加其它类型的自定义输出文件。所有的配置项都会按照项目包名进行保存，只需一次性设置就可以了。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1680051653288-8c8440ce-e54a-49ee-a00a-b877b5504067.png#averageHue=%23edf1f1&clientId=uc383ce32-2432-4&from=paste&id=uc28319a8&originHeight=616&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ue75d7a14-edfb-48f4-870f-fb6ca06001e&title=)
<a name="wDD7D"></a>
### 3、策略配置
将每次生成代码时可能变动的内容加入到代码生成选项中，方便调整每次的生成策略，比如：是否覆盖原文件，生成文件的种类等等：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1680051654016-8993f462-cd73-46f2-b5b8-e5f8d0f577c4.png#averageHue=%236e6a64&clientId=uc383ce32-2432-4&from=paste&id=u8e9e2d9f&originHeight=621&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ubd18efca-c42f-4259-8bd3-c0ec623ccab&title=)
<a name="UmkJe"></a>
### 4、SQL配置生成
通过输入查询SQL，可自动在Mapper（Xml及Java）中生成对应的查询方法，DTO对象和ResultMap（结果集映射配置）<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1680051654137-53759c77-0ec2-447b-ab8c-19b594842bb9.png#averageHue=%23fbf4e1&clientId=uc383ce32-2432-4&from=paste&id=ub4b18999&originHeight=617&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ua9938d10-04bd-472d-8705-cca0e9e3c0c&title=)
<a name="KwcH1"></a>
### 5、代码生成
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1680051654156-43a8cc4c-cc46-46a0-89ee-247721312483.png#averageHue=%23f2eadd&clientId=uc383ce32-2432-4&from=paste&id=u563abba0&originHeight=617&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ua2e2e57e-efd7-4501-bb1b-9996147961c&title=)<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1680051654305-5605d237-54a9-42b0-b7e4-0fdd94d83f9f.png#averageHue=%23f8f7f5&clientId=uc383ce32-2432-4&from=paste&id=u952b575a&originHeight=770&originWidth=755&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u049e303a-7242-417c-902b-35a845c5150&title=)
<a name="U69hs"></a>
## **四、自定义扩展**
<a name="Qsy5P"></a>
### 1、相关模板调整
在相关的页面中，可以进行相应的调整，在对应的btl模板中下载对应文件的具体模板，使用文本工具打开，直接修改源代码，文中取一种方式示例，其它方式一样。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1680051654375-f6b502de-390e-4f58-b5c2-1f6dda4622cb.png#averageHue=%23fefdfc&clientId=uc383ce32-2432-4&from=paste&id=u0cc26c23&originHeight=367&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=udc4bb30a-28c7-4587-b3e8-69a1f576662&title=)<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1680051654507-873c5601-4bfa-4243-86ea-682e5476ce00.png#averageHue=%23fdfcfc&clientId=uc383ce32-2432-4&from=paste&id=ue5924ff1&originHeight=789&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u0597eed9-162c-4ae7-8370-0868e875384&title=)
<a name="dEC0v"></a>
### 2、代码层级的配置
在一些团队中，肯定对Mapper的定义为Dao，Controller层需要定义为Action，通过修改代码模板btl的方式是可以的，还有一种方式是通过调整内部映射的方式来进行修改。主要使用的类是NameConverter。
```java
/**
 * 自定义Service类文件的名称规则
 */
public String serviceNameConvert(String tableName) {
	return this.entityNameConvert(tableName) + "Service";
}

/**
 * 自定义Controller类文件的名称规则
 */
public String controllerNameConvert(String tableName) {
    return this.entityNameConvert(tableName) + "Action";
}
```
除了Service、Controller、Entity、FieldName都可以实现自定义的扩展。下面是NameConverter类的核心代码，这里有详细的定义。
```java
package com.github.davidfantasy.mybatisplus.generatorui.mbp;

import cn.hutool.core.util.StrUtil;
import com.github.davidfantasy.mybatisplus.generatorui.dto.Constant;
import com.google.common.base.Strings;

import static com.github.davidfantasy.mybatisplus.generatorui.dto.Constant.DOT_JAVA;
import static com.github.davidfantasy.mybatisplus.generatorui.dto.Constant.DOT_XML;

/**
 * 自定义各类名称转换的规则
 */
public interface NameConverter {

    /**
     * 自定义Entity.java的类名称
     *
     * @param tableName 表名称
     * @return
     */
    default String entityNameConvert(String tableName) {
        if (Strings.isNullOrEmpty(tableName)) {
            return "";
        }
        tableName = tableName.substring(tableName.indexOf(StrUtil.UNDERLINE) + 1, tableName.length());
        return StrUtil.upperFirst(StrUtil.toCamelCase(tableName.toLowerCase()));
    }

    /**
     * 自定义表字段名到实体类属性名的转换规则
     *
     * @param fieldName 表字段名称
     * @return
     */
    default String propertyNameConvert(String fieldName) {
        if (Strings.isNullOrEmpty(fieldName)) {
            return "";
        }
        if (fieldName.contains("_")) {
            return StrUtil.toCamelCase(fieldName.toLowerCase());
        }
        return fieldName;
    }

    /**
     * 自定义Mapper.java的类名称
     */
    default String mapperNameConvert(String tableName) {
        return entityNameConvert(tableName) + "Mapper";
    }

    /**
     * 自定义Mapper.xml的文件名称
     */
    default String mapperXmlNameConvert(String tableName) {
        return entityNameConvert(tableName) + "Mapper";
    }

    /**
     * 自定义Service.java的类名称
     */
    default String serviceNameConvert(String tableName) {
        return "I" + entityNameConvert(tableName) + "Service";
    }

    /**
     * 自定义ServiceImpl.java的类名称
     */
    default String serviceImplNameConvert(String tableName) {
        return entityNameConvert(tableName) + "ServiceImpl";
    }

    /**
     * 自定义Controller.java的类名称
     */
    default String controllerNameConvert(String tableName) {
        return entityNameConvert(tableName) + "Controller";
    }

    /**
     * 自定义其它生成文件的文件名（不包括entity,mapper.java,mapper.xml,service,serviceImpl,controller这6种）
     *
     * @param fileType  在页面上输入的输出文件标识
     * @param tableName 关联的数据表名称名称
     * @return 生成文件的名称，带后缀
     */
    default String outputFileNameConvert(String fileType, String tableName) {
        if (fileType.equals(Constant.FILE_TYPE_ENTITY)) {
            return this.entityNameConvert(tableName) + DOT_JAVA;
        } else if (fileType.equals(Constant.FILE_TYPE_MAPPER)) {
            return this.mapperNameConvert(tableName) + DOT_JAVA;
        } else if (fileType.equals(Constant.FILE_TYPE_MAPPER_XML)) {
            return this.mapperXmlNameConvert(tableName) + DOT_XML;
        } else if (fileType.equals(Constant.FILE_TYPE_SERVICE)) {
            return this.serviceNameConvert(tableName) + DOT_JAVA;
        } else if (fileType.equals(Constant.FILE_TYPE_SERVICEIMPL)) {
            return this.serviceImplNameConvert(tableName) + DOT_JAVA;
        } else if (fileType.equals(Constant.FILE_TYPE_CONTROLLER)) {
            return this.controllerNameConvert(tableName) + DOT_JAVA;
        }
        return this.entityNameConvert(tableName) + fileType;
    }
}
```
mybatis-plus-generator-ui的功能非常丰富，甚至针对ui都是可以自定义修改的。如果需要定制UI的话，将代码clone下来后，进入到frontend目录下，进行相应的扩展开发。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1680051654559-842cae50-48a3-48a9-9819-7d729f4a3556.png#averageHue=%23fefcfb&clientId=uc383ce32-2432-4&from=paste&id=u465ff336&originHeight=333&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u9c2efdf7-aada-45a2-b139-44c9598fe02&title=)<br />修改完成后，需要另行编译src\frontend中的静态资源（源码中不包含已编译的页面），在src\frontend文件夹中执行：
```bash
yarn install
yarn run build
```
<a name="XwhmA"></a>
## **五、总结**
本文通过实例集成的方式来详细讲解mybatis-plus-generator-ui，从相关概念到实际集成案例，以及具体的扩展开发介绍。如果在工作中有这种需要，不妨采用这种方式。<br />项目地址：[https://github.com/davidfantasy/mybatis-plus-generator-ui](https://github.com/davidfantasy/mybatis-plus-generator-ui)
