JavaSpringBoot<br />Spring Boot 允许从外部加载配置，这样的话，就可以在不同的环境中使用相同的代码。支持的外部配置源包括：Java属性文件、YAML文件、环境变量、命令行参数。<br />用`@Value`注解可以将属性值直接注入到beans中。命令行参数以 `--` 开头<br />配置文件加载的顺序如下：

1. Application properties packaged inside your jar (application.properties and YAML variants)
2. Profile-specific application properties packaged inside your jar (application-{profile}.properties and YAML variants)
3. Application properties outside of your packaged jar (application.properties and YAML variants)
4. Profile-specific application properties outside of your packaged jar (application-{profile}.properties and YAML variants)

应用程序启动的时候，Spring Boot 会自动从以下位置查找并加载 application.properties 和 application.yaml 文件：

1. The classpath root
2. The classpath /config package
3. The current directory
4. The /config subdirectory in the current directory
5. Immediate child directories of the /config subdirectory

查找顺序按优先级来，后面的可以覆盖前面的。文件中的每一项会作为PropertySources被添加到Spring Environment<br />如果不想用"application"作为配置文件名称，可以在环境属性中通过`spring.config.name`来指定。还可以使用`spring.config.location`来显式指定配置文件的位置（文件路径或目录用逗号分隔）<br />注意，`spring.config.name` 和 `spring.config.location` 必须定义为一个环境属性(通常是一个OS环境变量、一个系统属性或一个命令行参数)
> 如果 `spring.config.location` 指定的是一个目录，那么必须以 `/` 结尾

无论直接指定文件还是包含在目录中，配置文件必须在其名称中包含文件扩展名。通常，扩展名支持 .properties, .yaml, .yml<br />举个栗子<br />指定单个文件
```bash
$ java -jar myproject.jar --spring.config.name=myproject
```
指定多个位置
```bash
$ java -jar myproject.jar --spring.config.location=optional:classpath:/default.properties,optional:classpath:/override.properties
```
默认情况下，如果指定的位置不存在，则会报错。如果不确定它是否存在，使用使用 `optional:` 前缀<br />例如：
```bash
optional:classpath:/custom-config/,optional:file:./custom-config/
```
<a name="yIvbJ"></a>
### Profile Specific Files
Spring Boot 会尝试加载按照命名规约 application-{profile} 的文件，把这样的文件称为Profile-specific文件。<br />例如，如果激活了一个名称为prod的profile，并且使用YAML文件，那么 application.yml 和 application-prod.yml 都会被尝试加载。<br />Profile-specific文件必须要和application.properties在相同的位置，而且profile-specific文件中的配置项会覆盖非profile-specific文件中相同的配置项。如果有多个profile-specific文件被激活，则后面会覆盖前面的。<br />举个例子，假设`spring.profiles.active=prod,live`，那么application-prod.properties中的配置项会被application-live.properties中的同名配置项覆盖。如果没有明确指定激活哪个profile，则application-default会被考虑激活。
<a name="RVbVA"></a>
### YAML 文件
考虑下面的配置文件
```yaml
hello:
  world: "hehe"

my:
  roles:
    - "admin"
    - "guest"
```
转换成properties文件应该是：
```
hello.world=hehe
my.roles[0]=admin
my.roles[1]=guest
```
Spring Framework提供了两个方便的类，可以用来加载YAML文档。YamlPropertiesFactoryBean 将 YAML 作为 Properties 加载，而 YamlMapFactoryBean 将 YAML 作为一个 Map 加载。<br />可以用`@Value`注解将属性值注入到bean中
```java
@Value("${hello.world}")
private String wahaha;

@Value("${my.roles[1]}")
private String roles1;
```
<a name="fQjv3"></a>
### 划重点
1、支持四种外部配置源：Java属性文件、YAML文件、环境变量、命令行参数；<br />2、配置文件查找位置，依次为：<br /> (1) classpath 根路径<br /> (2) classpath 根路径下config目录<br /> (3) jar包当前目录<br /> (4) jar包当前目录的config目录<br /> (5) /config子目录的直接子目录<br />3、配置文件加载顺序：<br /> (1) 当前jar包内部的application.properties和application.yml<br /> (2) 当前jar包内部的application-{profile}.properties 和 application-{profile}.yml<br /> (3) 引用的外部jar包的application.properties和application.yml<br /> (2) 引用的外部jar包的application-{profile}.properties 和 application-{profile}.yml<br />4、后面的可以覆盖前面的同名配置项<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1659623190281-281e157b-d3e8-44a0-bf89-860267e87501.png#clientId=u083cbf9f-aa01-4&from=paste&id=ub5d77f78&originHeight=159&originWidth=273&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u9b7f502a-282f-4638-8e8e-29f5a544828&title=)<br />[https://docs.spring.io/spring-boot/docs/current/reference/html/spring-boot-features.html#boot-features-external-config-files](https://docs.spring.io/spring-boot/docs/current/reference/html/spring-boot-features.html#boot-features-external-config-files)
