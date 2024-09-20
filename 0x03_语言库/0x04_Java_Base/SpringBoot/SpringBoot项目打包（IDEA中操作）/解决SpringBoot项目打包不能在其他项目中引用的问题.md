<a name="SN3j2"></a>
## 问题描述
采用feign进行消费者与提供者进行开发的时候，往往会把公共的代码部分打成jar包进行引用，可以避免冗余的开发。例如一些config的配置文件、domon文件、以及filter跟工具类，把这些统一叫做common项目。但是当把common打包成jar包，引入其他项目的时候，可以正常引用，但是里面的类却无法引用。原因如下：<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1634616685665-a495f8c7-c153-4798-b48a-cc6448319030.png#clientId=ub27bacc4-464d-4&from=paste&height=630&id=u4de1937e&originHeight=1890&originWidth=3040&originalType=binary&ratio=1&rotation=0&showTitle=false&size=456338&status=done&style=shadow&taskId=u7e101d83-8fb7-4eca-901d-d0c3013746c&title=&width=1013.3333333333334)<br />所有的common里面的代码都是存在于BOOF-INF下，这样其他的项目是无法依赖的，并且在METE-INF下定义了启动入口<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1634616747292-716caf69-fc36-4a5b-9a4f-3c4aaefc8ac1.png#clientId=ub27bacc4-464d-4&from=paste&height=355&id=ubca12490&originHeight=1066&originWidth=3209&originalType=binary&ratio=1&rotation=0&showTitle=false&size=351665&status=done&style=shadow&taskId=u56bf3a7c-dae7-4e26-a40f-15d343abbd6&title=&width=1069.6666666666667)
<a name="v0EoU"></a>
## 解决方式
<a name="bmmkp"></a>
### 方式一：打包时生成可依赖jar与可执行jar
正常情况下公共部分打包都是采用SpringBoot提供的Maven插件进行打包操作的
```xml
<groupId>org.springframework.boot</groupId>
<artifactId>spring-boot-maven-plugin</artifactId>
```
只需要用`classifier`改一下可执行jar包的名字，这种方式，默认打包成两个，一个是可依赖的jar包，一个是可执行的后缀名为exec的jar包。
```xml
<plugin>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-maven-plugin</artifactId>
    <configuration>
        <classifier>exec</classifier>
    </configuration>
</plugin>
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1634616966071-fc4bfb91-08bc-4564-95a8-d74a50557aea.png#clientId=ub27bacc4-464d-4&from=paste&height=661&id=ucbcbdcb7&originHeight=1983&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=751055&status=done&style=shadow&taskId=u4201c22d-0519-43f7-b32c-adcea8ba290&title=&width=1280)<br />可以在target目录下看到成功编译可引用的`jar`包以及可执行的`-.exec.jar`<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1634617133811-4c12e67f-dcd7-4ea9-b658-6ab5a8833c15.png#clientId=ub27bacc4-464d-4&from=paste&height=458&id=u047da4d6&originHeight=1375&originWidth=2725&originalType=binary&ratio=1&rotation=0&showTitle=false&size=413160&status=done&style=shadow&taskId=ua3b04ba3-8624-4e52-9863-fff3d3e5378&title=&width=908.3333333333334)<br />此时再查看新发布的包内容，并且成功引入了该依赖。<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1634617063727-7134a607-634f-40ba-ad33-10cf5aebc3fa.png#clientId=ub27bacc4-464d-4&from=paste&height=338&id=u1f489e70&originHeight=1014&originWidth=2680&originalType=binary&ratio=1&rotation=0&showTitle=false&size=265224&status=done&style=shadow&taskId=ub77f52fd-1238-4bf6-9ad4-faa38423259&title=&width=893.3333333333334)
<a name="J3vLx"></a>
### 方式二：直接不使用spring-boot-maven-plugin插件
![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1662689846228-70d87cb5-13b1-4ae0-b427-d872597b9c34.png#clientId=u6988c150-95b4-4&from=paste&height=186&id=u91ccc4ef&originHeight=464&originWidth=2158&originalType=binary&ratio=1&rotation=0&showTitle=false&size=81801&status=done&style=shadow&taskId=u7b2e2423-b5c2-4d2a-9f59-7edd1fae68d&title=&width=863.2)
