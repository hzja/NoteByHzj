Maven<br />最简单的方法就行使用直接使用maven插件打包，甚至都不需要知道他是怎么实现的，插件将项目打包为一个jar包，然后使用`java -jar xx.jar`就能运行项目。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1687224430028-3f79d6c7-1ae1-4cc4-8ab0-f286e92f516d.png#averageHue=%233f444c&clientId=ub7264bb6-e25e-4&from=paste&id=u3da450ff&originHeight=414&originWidth=761&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u5b1b8f7d-7f28-480d-8cf7-aba5518a78f&title=)<br />平时使用的在开发中使用的是开发或测试的数据库，和生产上面的一般是隔离的，意味着打包的时候需要激活生产的配置文件，但是不一定有访问生产库的权限，此时直接打包就会出现问题。当直接点击上面的package的时候他会激活单元测试，需要测试通过以后才能打包，但是很显然测是不能通过的，因为激活了生产的配置但是并没有访问上产库的权限，此时就会陷入一直打包却打不完的感觉，这就需要打包时跳过测试。那怎么跳过测试呢？<br />为什么打包时要执行单元测试呢？【这不是重点，就简单讲讲，需要深入了解的自行查阅相关资料。】此时就涉及到 Maven 的生命周期，在 Maven 的 default 生命周期中共有 23 个阶段，每个生命周期中的后面的阶段会依赖于前面的阶段，当执行某个阶段的时候，会先执行其前面的阶段。<br />在 default 生命周期中 package 阶段不是第一个阶段，因此也需要依赖于前面的阶段的执行，正好 test 阶段【测试：使用合适的单元测试框架运行测试（Juint是其中之一）。】就是其前面阶段之一，此时就会必须先通过 test 阶段才会到 package 阶段【 打包：将编译后的代码打包成可分发格式的文件，比如JAR、WAR…】。<br />因为没有访问生产库的权限，此时连 test 阶段都不能通过，那就不会到达 package 这个阶段。那有什么解决方法呢？下面探讨一下此问题的解决方法：
<a name="dUXV6"></a>
## 1、命令行方式跳过测试
可以通过使用命令将项目打包，添加跳过测试的命令就可以了，可以用两种命令来跳过测试：
<a name="dPtw1"></a>
###  `-DskipTests=true`
```bash
mvn package -DskipTests=true
```
`-DskipTests=true`，不执行测试用例，但编译测试用例类生成相应的class文件至 target/test-classes 下。
<a name="vEZ26"></a>
### `-Dmaven.test.skip=true`
```bash
mvn package -Dmaven.test.skip=true
```
`-Dmaven.test.skip=true`，不执行测试用例，也不编译测试用例类。<br />在使用 `mvn package` 进行编译、打包时，Maven会执行 src/test/java 中的 JUnit 测试用例，有时为了跳过测试，会使用参数 `-DskipTests=true` 和 `-Dmaven.test.skip=true`，这两个参数的主要区别是：

- 使用 `-Dmaven.test.skip=true`，不但跳过单元测试的运行，也跳过测试代码的编译；
- 使用 `-DskipTests=true` 跳过单元测试，但是会继续编译。
<a name="ToUQz"></a>
## 2、pom.xml中配置跳过测试
可以在 pom.xml 中添加如下配置来跳过测试：
```xml
<build>
  <plugins>
    <!-- maven 打包时跳过测试 -->
    <plugin>
      <groupId>org.apache.maven.plugins</groupId>
      <artifactId>maven-surefire-plugin</artifactId>
      <configuration>
        <skip>true</skip>
      </configuration>
    </plugin>
  </plugins>
</build>
```
<a name="JdHvn"></a>
## 3、直接配置
Maven命令栏的工具栏有下图中的图标，这个图标就是 Skip Tests。点击选中，再用 LifeStyle 中的打包就会跳过测试。<br />注：因为IDEA是2022的版本，图标可能和以前的版本有些许区别，以前的版本应该是一个蓝色的圆圈里面带一个闪电。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1687224430030-c7093e7d-e908-4bbc-be27-e2c6c6435812.png#averageHue=%233f444c&clientId=ub7264bb6-e25e-4&from=paste&id=uedf31e34&originHeight=440&originWidth=784&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uf5cedb23-ae51-410c-a581-4fa41e4cc7c&title=)
<a name="yKaN0"></a>
## 4、添加Maven配置参数
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1687224430134-2c5700b9-a46e-41b6-a159-a662ef9ec842.png#averageHue=%233f4347&clientId=ub7264bb6-e25e-4&from=paste&id=u53a58f08&originHeight=481&originWidth=1021&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u80620b09-ce6e-496a-8e93-cd1571af3c6&title=)
<a name="ZsX8f"></a>
## 5、通过更改设置
打开配置，找到 Build,Exxcution,Deployment –> Maven Tools –> Maven –> Runner，在 Properties 中勾选 Skip Test 选项。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1687224430080-df563bb2-825a-48c8-8211-b7d240423c80.png#averageHue=%233e4246&clientId=ub7264bb6-e25e-4&from=paste&id=u3b0793ea&originHeight=780&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u09445fb5-2b67-422d-9c5e-9dd476d55f4&title=)<br />以上就是查阅资料之后找到是几种打包跳过测试的方式。
