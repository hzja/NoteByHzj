JavaMaven<br />Maven 依赖可以分为如下几部分：

1. 直接依赖，就是本项目 dependencies 部分的依赖
2. 间接依赖，就是本项目 dependencies 部分的依赖所包含的依赖
3. 依赖管理，就是本项目 dependency management 里面的依赖
4. parent 的直接依赖
5. parent 的间接依赖
6. parent 的依赖管理
7. bom 的直接依赖（一般没有）
8. bom 的间接依赖（一般没有）
9. bom 的依赖管理

PS：bom 就是工程项目中最外层主 POM ，也就是 dependencyManagement 那个 POM
<a name="ILbcQ"></a>
## 一、 Maven 依赖三大原则
<a name="NxQ7n"></a>
### 1.1 最短路径优先原则
Maven 依赖遵循最短路径优先原则，当项目直接依赖一个 C-api-1.0 和 A-api-2.1 包，并且 C-api-1.0 有如下间接依赖关系：C-api-1.0 —> B-api-1.0 —> A-api-1.1 这时候项目里包含了 A-api 的 1.1 和 2.1 两个版本，由于存在最短路径原则明显 Project —> A-api-2.1 短于 Project —> C-api-1.0 —> B-api-1.0 —> A-api-1.1 故 Project 项目里会使用 A-api-2.1<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1693284207530-eb4edc6c-3b4a-4019-b19e-a2c8d7cdb4e2.png#averageHue=%23f2f28a&clientId=u6e513954-c3a4-4&from=paste&id=u2f6c210d&originHeight=1218&originWidth=1004&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u54758cd0-0cfb-457b-baa7-db4164616df&title=)<br />示例项目中包含如下依赖
```xml
<dependencies>
  <dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter</artifactId>
  </dependency>

  <dependency>
    <groupId>org.apache.poi</groupId>
    <artifactId>poi-ooxml</artifactId>
    <version>3.10-FINAL</version>
  </dependency>

  <dependency>
    <artifactId>QLExpress</artifactId>
    <groupId>com.alibaba</groupId>
    <version>3.2.2</version>
  </dependency>

</dependencies>
```
其中 poi-ooxml 存在如下依赖关系：poi-ooxml-->poi-->commons-logging (版本 1.1 ) QLExpress 存在如下依赖关系：QLExpress —> commons-logging (版本 1.1.1 ) 由于存在最短路径原则，明显 QLExpress —> commons-logging 路径更短，项目会使用 commons-logging 的 1.1.1 版本<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1693284207554-6c0a477d-1c38-4ace-9f2e-7a3398e54221.png#averageHue=%2345484a&clientId=u6e513954-c3a4-4&from=paste&id=ua06bbb42&originHeight=322&originWidth=427&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u89b5320e-ac80-44c6-adb1-c1b42fb9a3a&title=)
<a name="VlBny"></a>
### 1.2 POM 文件中申明顺序优先原则
Maven 依赖遵循 POM 文件中申明顺序优先原则，当项目里存在直接依赖 C-api-1.0 和 B-api-1.0 其中存在如下间接依赖关系：C-api-1.0 —> A-api-2.1 B-api-1.0 —> A-api-1.1 这时项目间接依赖了 A-api 的 2.1 和 1.1 两个版本，由于存在 POM 文件中申明顺序优先原则，故项目中会使用 A-api-2.1![](https://cdn.nlark.com/yuque/0/2023/png/396745/1693284207518-7354b81b-6152-4db0-a959-0c10e9b6b55d.png#averageHue=%23f3f38a&clientId=u6e513954-c3a4-4&from=paste&id=ufa9915e7&originHeight=1104&originWidth=952&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u2fd868c4-7093-402c-8017-024e7013cae&title=)示例项目中存在如下依赖
```xml
<dependencies>
  <dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter</artifactId>
  </dependency>


  <dependency>
    <groupId>org.apache.poi</groupId>
    <artifactId>poi-ooxml</artifactId>
    <version>3.10-FINAL</version>
  </dependency>

  <dependency>
    <groupId>org.apache.poi</groupId>
    <artifactId>poi-scratchpad</artifactId>
    <version>3.17-beta1</version>
  </dependency>
</dependencies>
```
其中 poi-ooxml 依赖了包 poi ，poi-scratchpad 也依赖了 poi 包，但是 poi-ooxml 依赖了 poi 包的 3.10-FINAL 版本， poi-scratchpad 依赖了 poi 包的 3.17-beta1 版本，由于存在申明顺序优先原则，项目会使用 poi 包的 3.10-FINAL 版本![](https://cdn.nlark.com/yuque/0/2023/png/396745/1693284207529-0650f4fc-bc7f-4984-9ae9-d24d95f430e0.png#averageHue=%23434648&clientId=u6e513954-c3a4-4&from=paste&id=ua3a2df91&originHeight=287&originWidth=529&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u26300818-3d45-4b75-af26-76803f4e1bc&title=)
<a name="o6LHK"></a>
### 1.3 覆盖优先原则
Maven 依赖遵循覆盖优先原则，项目父 POM 中直接依赖包 A-api-1.1 ，子模块 Module A 的 parent 直接依赖了项目的 POM ，但是同时也直接依赖了 A-api-1.2 。由于存在覆盖优先原则子模块 Module A 中会优先使用 A-api-1.2 而不是父POM的 A-api-1.1![](https://cdn.nlark.com/yuque/0/2023/png/396745/1693284207490-ac06d2a9-c841-4234-a2cd-58b47a56d779.png#averageHue=%23f8f88d&clientId=u6e513954-c3a4-4&from=paste&id=u6b2fd815&originHeight=916&originWidth=996&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u2215312b-f2bc-4b4d-9353-383b3719f9b&title=)在以上项目工程下，新建一个子工程，在子工程POM添加如下依赖
```
<dependencies>
    <dependency>
      <groupId>org.apache.poi</groupId>
      <artifactId>poi</artifactId>
      <version>5.2.3</version>
    </dependency>
</dependencies>
```
由于外部工程依赖了 poi 包的 3.10-FINAL ，但是子工程依赖了 poi 的 5.2.3 版本，故整体包依赖会包含poi的两个版本，但是在子工程中使用的是 5.2.3<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1693284208297-38d8d35d-a677-411d-95b2-c8effd8bd038.png#averageHue=%23444749&clientId=u6e513954-c3a4-4&from=paste&id=uca5eb4c5&originHeight=381&originWidth=464&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u4a5f42d9-eaed-4cb3-842d-bb7c7e6bec3&title=)<br />怎么证明在子工程使用的是 poi 的 5.2.3 版本呢？在子工程可以写一个 main 方法调用方法 org.apache.poi.util.Units 类的 `columnWidthToEMU` 方法，在父工程同样调用这个方法，发现父工程这个方法报错不存在。
```java
import org.apache.poi.util.Units;
public class Test {

    public static void main(String[] args) {
        Units.columnWidthToEMU(1);
    }

}
```
<a name="iF9WF"></a>
## 二、 Maven 依赖冲突常见报错
<a name="LChV9"></a>
### 2.1 `ClassNotFoundException`
当项目启动时出现 `ClassNotFoundException` 这样的错误，表示由于项目使用的包版本下找不到当前需要的类 <br />1、调用 class 的 forName 方法时，找不到指定的类。<br />2、 `ClassLoader` 中的 findSystemClass() 方法时，找不到指定的类。<br />3、 `ClassLoader` 中的 `loadClass()` 方法时，找不到指定的类。
<a name="PJy6I"></a>
### 2.2 `NoSuchMethodError`
`NoSuchMethodError` 就是程序在运行中找不到运行的方法导致的 <br />1、有可能发生的就是 jar 冲突，可能是两个高低版本的 jar 包导致。<br />2、有可能是有两个 jar 包有相同的类与方法，导致程序调用过程中找不到正确的方法。
<a name="W7kPI"></a>
## 三、Maven 依赖加载流程
Maven 依赖加载流程如下

1. 首先，将 parent 的直接依赖，间接依赖，还有依赖管理，插入本项目，放入本项目的直接依赖，间接依赖还有依赖管理之前。
2. 对于直接依赖，如果有 version，那么就依次放入 DependencyMap 中。如果没有 version ，则从依赖管理中查出来 version，之后放入 DependencyMap 中。key 为依赖的 groupId + artifactId， value 为 version ，后放入的会把之前放入的相同 key 的 value 替换。
3. 对于每个依赖，各自按照步骤 1 和 2 加载自己的 pom 文件，但是如果第一步中的本项目 dependency management 中有依赖的版本，使用本项目 dependency management 的依赖版本，生成 TransitiveDependencyMap ，这里面就包含了所有的间接依赖。
4. 所有间接依赖的 TransitiveDependencyMap ， 对于项目的 DependencyMap 里面没有的 key ，依次放入项目的 DependencyMap 。
5. 如果 TransitiveDependencyMap 里面还有间接依赖，那么递归执行步骤 3 和 4 。

由于是先放入本项目的 DependencyMap ，再去递归 TransitiveDependencyMap ，这就解释了 Maven 依赖的最短路径原则。可用文中 1.1 做示例如下：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1693284208435-61a29e04-4a7f-4ba3-a791-cf774f03fd53.png#averageHue=%23fafafa&clientId=u6e513954-c3a4-4&from=paste&id=u95b3aa27&originHeight=1173&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u4782a8ed-ba4c-4141-8564-d5c0a15dc28&title=)![](https://cdn.nlark.com/yuque/0/2023/png/396745/1693284208459-39e999e8-273f-443f-a1fc-097e3394dea4.png#averageHue=%23fafafa&clientId=u6e513954-c3a4-4&from=paste&id=ud8420e12&originHeight=1106&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u60242e99-7aa2-499d-b08d-5747fc1c077&title=)
<a name="BB8Gz"></a>
## 四、总结
本次主要讲解了 Maven 依赖包含的几大部分，以及 Maven 依赖三大原则，并且对每种依赖原则都做了具体图解和示例，方便可以直接在项目中运行调试。期间对 Maven 冲突导致的常见报错进行讲解方便在开发过程中快速定位问题。最后讲解了 Maven 加载包进入项目中的整个流程，并按照文中 1.1 所讲的“最短路径优先原则“做为示例图解，进而加深对 Maven 加载包流程的理解。
