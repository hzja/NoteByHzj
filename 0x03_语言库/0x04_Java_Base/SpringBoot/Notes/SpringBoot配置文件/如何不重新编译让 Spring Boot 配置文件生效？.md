JavaSpringBoot
<a name="hQzM0"></a>
## 1、 配置文件位置
首先要明白，Spring Boot 默认加载的配置文件是 application.properties 或者 application.yaml，默认的加载位置一共有五个，五个位置可以分为两类：<br />**从 classpath 下加载**，这个又细分为两种：

1. 直接读取 classpath 下的配置文件，对应到 Spring Boot 项目中，就是 resources 目录下的配置。
2. 读取 classpath:/config/ 目录下的文件，对应到 Spring Boot 项目中就是 resources/config 目录下的配置。

这两种情况如下图：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1669294387524-97837089-d6db-4eb7-b520-9e9096fcf752.png#averageHue=%23fbfbfb&clientId=uf2e97b41-643c-4&from=paste&id=ue2de61ac&originHeight=412&originWidth=662&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u34a074dd-6deb-45df-b98b-b248e0ca863&title=)<br />**从项目所在的当前目录下加载**，这个又细分为三种情况：

1. 从项目当前目录下加载配置文件。
2. 从项目当前目录下的 config 文件夹中加载配置文件。
3. 从项目当前目录下的 config 文件夹的子文件夹中加载（孙子文件夹不可以）。

这三种情况如下图：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1669294387556-be27a69d-3b04-4f62-ac1d-3825125c611e.png#averageHue=%23fdfbfa&clientId=uf2e97b41-643c-4&from=paste&id=u2b46b9da&originHeight=522&originWidth=786&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ud0972888-9d8c-44d0-af19-f1475db1e38&title=)
:::danger
config 目录下的配置文件可以被加载，config/a 目录下的配置文件也可以被加载，但是 config/a/b 目录下的配置文件不会被加载，因为不是直接子文件夹。
:::
配置文件可以放在这么多不同的位置，如果同一个属性在多个配置文件中都写了，那么后面加载的配置会覆盖掉前面的。例如在 classpath:application.yaml 中设置项目端口号是 8080，在 项目当前目录/config/a/application.yaml 中设置项目端口是 8081，那么最终的项目端口号就是 8081。<br />这是默认的文件位置。<br />如果不想让自己的配置文件叫 application.properties 或者 application.yaml，那么也可以自定义配置文件名称，只需要在项目启动的时候指定配置文件名即可，例如设置配置文件名为 app.yaml，那么可以在启动 jar 包的时候按照如下方式配置，此时系统会自动去上面提到的五个位置查找对应的配置文件：
```bash
java -jar boot_config_file-0.0.1-SNAPSHOT.jar --spring.config.name=app
```
如果项目已经打成 jar 包启动了，那么前面所说的目录中，后三个中的项目当前目录就是指 jar 包所在的目录。<br />如果不想去这五个位置查找，那么也可以在启动 jar 包的时候明确指定配置文件的位置和名称，如下：
```bash
java -jar boot_config_file-0.0.1-SNAPSHOT.jar --spring.config.location=optional:classpath:/app.yaml
```
注意，在 classpath 前面加上了 `optional:` 表示如果这个配置文件不存在，则按照默认的方式启动，而不会报错说找不到这个配置文件。如果不加这个前缀，那么当系统找不到指定的配置文件时，就会抛出 `ConfigDataLocationNotFoundException` 异常，进而导致应用启动失败。<br />如果配置文件和 jar 包在相同的目录结构下，如下图：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1669294387532-06d429ce-b373-42cd-a158-6213dca6fa8a.png#averageHue=%23f8f7dd&clientId=uf2e97b41-643c-4&from=paste&id=uec0834b4&originHeight=126&originWidth=602&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u9335a59b-5728-4a14-ba3f-4b3d2f0271c&title=)<br />那么启动脚本如下：
```bash
java -jar boot_config_file-0.0.1-SNAPSHOT.jar --spring.config.location=optional:javaboy/app.yaml
```
如果 `spring.config.location` 的配置，只是指定了目录，那么必须以 `/` 结尾，例如上面这个启动脚本，也可以按照如下方式启动：
```bash
java -jar boot_config_file-0.0.1-SNAPSHOT.jar --spring.config.location=optional:javaboy/ --spring.config.name=app
```
通过 `spring.config.location` 属性锁定配置文件的位置，通过 spring.config.name 属性锁定配置文件的文件名。
<a name="CKme6"></a>
## 2、额外位置
前面关于配置文件位置的设置，都是覆盖掉已有的配置，如果不想覆盖掉 Spring Boot 默认的配置文件查找策略，又想加入自己的，那么可以按照如下方式指定配置文件位置：
```bash
java -jar boot_config_file-0.0.1-SNAPSHOT.jar --spring.config.additional-location=optional:javaboy/app.yaml
```
如果这个额外指定的配置文件和已有的配置文件有冲突，那么还是以后来者为准。
<a name="WW0HQ"></a>
## 3、位置通配符
有一种情况，假设有 redis 和 mysql 的配置，想将之放在两个不同的文件夹中以便于管理，像下面这样：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1669294387526-ab95a212-a730-4460-be6a-b1644d7bf817.png#averageHue=%23f9f8df&clientId=uf2e97b41-643c-4&from=paste&id=u742d7137&originHeight=238&originWidth=656&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ufa8d5bec-3885-4e07-823c-788b24c0fcd&title=)<br />那么在项目启动时，可以通过通配符 `*` 批量扫描相应的文件夹：
```bash
java -jar boot_config_file-0.0.1-SNAPSHOT.jar --spring.config.location=optional:config/*/
```
使用通配符批量扫描 mysql 和 redis 目录时，默认的加载顺序是按照文件夹的字母排序，即先加载 mysql 目录后加载 redis 目录。
:::danger
需要注意的是，通配符只能用在外部目录中，不可以用在 classpath 中的目录上。另外，包含了通配符的目录，只能有一个通配符 `*`，不可以有多个，并且还必须是以 `*/` 结尾，即一个目录的最后部分可以不确定。
:::
<a name="Nzzmh"></a>
## 4、导入外部配置
从 Spring Boot2.4 开始，也可以使用 `spring.config.import` 方法来导入配置文件，相比于 `additional-location` 配置，这个 `import` 导入更加灵活，可以导入任意名称的配置文件。
```
spring.config.import=optional:file:./dev.properties
```
甚至，这个 `spring.config.import` 还可以导入无扩展名的配置文件，例如有一个配置文件，是 properties 格式的，但是这个这个配置文件没有扩展名，现在想将之作为 properties 格式的配置文件导入，方式如下：
```
spring.config.import=optional:file:/Users/sang/dev[.properties]
```
