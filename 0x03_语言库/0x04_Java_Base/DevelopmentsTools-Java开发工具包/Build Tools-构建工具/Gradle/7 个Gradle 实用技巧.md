Gradle<br />`Gradle`在`android`开发中应用地十分广泛，这里主要介绍了使用`gradle`的一些实用技巧，主要包括以下内容：

1. `Gradle`依赖树查询
2. 使用循环优化`Gradle`依赖管理
3. 支持代码提示的`Gradle`依赖管理
4. `Gradle`模块化
5. `Library`模块`Gradle`代码复用
6. 资源文件分包
7. `AAR`依赖与源码依赖快速切换
<a name="LCFWO"></a>
## 1、Gradle依赖树查询
有时在分析依赖冲突时，需要查看依赖树，常用的查看依赖树的命令为
```groovy
gradlew app:dependencies
```
不过这种命令行方式查看依赖树出来的信息太多，看的有些费劲<br />所以官方又推出了`Scan`工具来帮助我们更加方便地查看依赖树<br />在项目根目录位置下运行`gradle build \--scan`即可，然后会生成 `HTML` 格式的分析文件的分析文件<br />分析文件会直接上传到`Scan`官网，命令行最后会给出远程地址<br />第一次跑会让你在 `Scan` 官网注册一下，邮件确认后就能看了<br />`scan` 工具是按照依赖变体挨个分类的，`debugCompileClassPath` 就是 `dedug` 打包中的依赖包了<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1618205185293-26bc0233-4fff-4be6-aac7-cd2f1eb33671.webp#averageHue=%23f3f6f3&height=900&id=QQOND&originHeight=900&originWidth=1046&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=shadow&title=&width=1046)如上，使用这种方式分析依赖树更加方便简洁
<a name="x3Y1b"></a>
## 2、使用循环优化Gradle依赖管理
如下所示，常常使用`ext`来管理依赖
```groovy
dependencies {
    implementation fileTree(include: ['*.jar'], dir: 'libs')
    implementation rootProject.ext.dependencies["appcompat-v7"]
    implementation rootProject.ext.dependencies["cardview-v7"]
    implementation rootProject.ext.dependencies["design"]
    implementation rootProject.ext.dependencies["constraint-layout"]
    annotationProcessor rootProject.ext.dependencies["glide_compiler"]
    ...
}
```
这样虽然实现了依赖的统一管理，但是随着项目越来越大，依赖也会越来越多，常常会有几十甚至上百行，导致`build.gradle`越来越长<br />有没有一种好的方式不在 `build.gradle` 中写这么多的依赖配置？<br />有，就是使用循环遍历依赖。<br />示例如下，首先添加`config.gradle`
```groovy
ext{
    dependencies = [
        // base
        "appcompat-v7"                      : "com.android.support:appcompat-v7:${version["supportLibraryVersion"]}"，
        ...
    ]

    annotationProcessor = [
        "glide_compiler"                    : "com.github.bumptech.glide:compiler:${version["glideVersion"]}",
        ...
    ]

    apiFileDependencies = [
        "launchstarter"                                   :"libs/launchstarter-release-1.0.0.aar"
    ]

    debugImplementationDependencies = [
        "MethodTraceMan"                                  : "com.github.zhengcx:MethodTraceMan:1.0.7"
    ]

    ...

        implementationExcludes = [
            "com.android.support.test.espresso:espresso-idling-resource:3.0.2" : [
                'com.android.support' : 'support-annotations'
            ]
        ]

    ...
}
```
然后在`build.gradle`中配置如下：
```groovy
apply from config.gradle
...
def implementationDependencies = project.ext.dependencies
def processors = project.ext.annotationProcesso
def implementationExcludes = project.ext.implementationExcludes
dependencies{
    // 处理所有的 xxximplementation 依赖
    implementationDependencies.each { k, v -> implementation v }   
    // 处理 annotationProcessor 依赖
    processors.each { k, v -> annotationProcessor v }
    // 处理所有包含 exclude 的依赖
    implementationExcludes.each { entry ->
        implementation(entry.key) {
            entry.value.each { childEntry ->
                exclude(group: childEntry)
            }
        }
    }
    ...
}
```
这样做的优点在于<br />1.后续添加依赖不需要改动`build.gradle`,直接在`config.gradle`中添加即可<br />2.精简了`build.gradle`的长度
<a name="RZ5s4"></a>
## 3、支持代码提示的Gradle依赖管理
上面介绍了通过`config.gradle`管理依赖的方法<br />在添加`Gradle`依赖时，还有一些痛点<br />1.不支持代码提示<br />2.不支持单击跳转<br />3.多模块开发时，不同模块相同的依赖需要复制粘贴<br />使用`buildSrc`+`kotlin`可以解决这个问题<br />效果如下：<br />![](https://cdn.nlark.com/yuque/0/2021/gif/396745/1618205185304-6e5b733e-65cc-4f51-9c66-aee239974850.gif#averageHue=%23fcfbfb&height=798&id=hTJcc&originHeight=798&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=shadow&title=&width=1080)<br />由于`buildSrc`是对全局的所有`module`的配置,所以可以在所有`module`中直接使用。
<a name="xV8WV"></a>
### `buildSrc` vs `includeBuild`
上面介绍的方法使用的是`buildSrc`，使用起来比较方便。<br />不过它的缺点在于构建速度上会慢一些，使用`includeBuild`可以实现同样的效果，两者实现的最终效果是差不多的。
<a name="SHQuD"></a>
## 4、Gradle模块化
在开发中，引入一些插件时，有时需要在`build.gradle`中引入一些配置，比如`greendao`，推送，`tinker`等<br />这些其实是可以封装在相应`gradle`文件中，然后通过`apply from`引入<br />举个例子，例如在使用`greendao`数据库时，需要在`build.gradle`中指定版本<br />这种时候应该新建一个`greendao-config.gradle`
```groovy
apply plugin: 'org.greenrobot.greendao'
//greenDao指定版本和路劲等
greendao {
    //数据库的schema版本，也可以理解为数据库版本号
    schemaVersion 1
    //设置DaoMaster、DaoSession、Dao包名，也就是要放置这些类的包的全路径。
    daoPackage 'com.example.ausu.big_progect.dao'
    //设置DaoMaster、DaoSession、Dao目录
    targetGenDir 'src/main/java'
}
```
然后再在`build.gradle`中引入
```
apply from 'greendao-config.gradle'
```
这样做主要有2个优点<br />1.单一职责原则，将`greendao`的相关配置封装在一个文件里，不与其他文件混淆<br />2.精简了`build.gradle`的代码，同时后续修改数据库相关时不需要修改`build.gradle`的代码
<a name="z6ZtC"></a>
## 5、Library模块Gradle代码复用
随着项目的越来越大，`Library Module`也越建越多，每个`Module`都有自己的`build.gradle`<br />但其实每个`build.gradle`的内容都差不多，能不能将重复的部分封装起来复用？<br />可以做一个 `basic` 抽取，同样将共有参数/信息提取到 `basic.gradle` 中，每个 `module` `apply`，这样就是减少了不少代码量
```groovy
apply plugin: 'com.android.library'
apply plugin: 'kotlin-android'
apply plugin: 'kotlin-android-extensions'
apply plugin: 'kotlin-kapt'
android {
    // 指定用于编译项目的 API 级别
    compileSdkVersion Versions.compileSDK
    // 指定在生成项目时要使用的 SDK 工具的版本，Android Studio 3.0 后不需要手动配置。
    buildToolsVersion Versions.buildTools
    // 指定 Android 插件适用于所有构建版本的版本属性的默认值
    defaultConfig {
        minSdkVersion Versions.minSDK
        targetSdkVersion Versions.targetSDK
        versionCode 1
        versionName "1.0"
    }
    // 配置 Java 编译(编码格式、编译级别、生成字节码版本)
    compileOptions {
        encoding = 'utf-8'
        sourceCompatibility JavaVersion.VERSION_1_8
        targetCompatibility JavaVersion.VERSION_1_8
    }
    kotlinOptions {
        jvmTarget = JavaVersion.VERSION_1_8.toString()
    }
    lintOptions {
        // lint 异常后继续执行
        abortOnError false
    }
}
dependencies {
    implementation fileTree(dir: 'libs', include: ['*.jar'])
    ...
}
```
然后在相应的模块的`build.gradle`中引入即可
```groovy
apply from:"../basic.gradle"
dependencies {
    api Deps.constraintLayout
    api Deps.retrofit
}
```
这样是不是简洁很多？读者可根据项目实际情况判断是否适合抽取`basic.gradle`使用
<a name="kdqqF"></a>
## 6、资源文件分包
随着项目越来越大，项目中的资源文件也越来越大，比如`layout`与`drawable`文件夹下的文件数量常常可达几百甚至上千个<br />能不能像代码一样，对资源文件进行分包呢？<br />答案是可以的，主要是利用`gradle`的`sourceSets`属性<br />可以将资源文件像代码一样按业务分包,具体操作如下<br />1.新建res_xxx目录<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1618205185326-246b0509-2caa-4a25-b31d-4ac141760927.webp#averageHue=%23f6f3f1&height=522&id=eGTyN&originHeight=522&originWidth=303&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=shadow&title=&width=303)<br />在 `main` 目录下新建 `res_core`, `res_feed`（根据业务模块命名）等目录，在`res_core`中新建`res`目录中相同的文件夹如：`layout`、`drawable-xxhdpi`、`values`等。<br />2.在`gradle`中配置`res_xx`目录
```groovy
android {
    //...
    sourceSets {
        main {
            res.srcDirs(
                    'src/main/res',
                    'src/main/res_core',
                    'src/main/res_feed',
            )
        }
    }
}
```
以上就完成了资源文件分包,这样做主要有几点好处<br />1.按业务分包查找方便，结构清晰<br />2.`strings.xml`等`key-value`型文件多人修改可以减少冲突<br />3.当删除模块或做组件化改造时资源文件删除或迁移方便，不必像以前一样一个个去找
<a name="lUrKa"></a>
## 7、AAR依赖与源码依赖快速切换
当项目中`Module`越来越多，为了加快编译速度，常常把`Module`发布成`AAR`，然后在项目中直接依赖`AAR`<br />但是有时候又需要修改`AAR`，就需要依赖于源码<br />所以需要一个可以快速地切换依赖`AAR`与依赖源码的方式<br />下面举个例子，以`retrofit`为例<br />假如要修改`retrofit`的源码，修改步骤如下：<br />1.首先下载`retrofit`，可以放到和项目同级的目录,并修改目录名为`retrofit-source`,以便区分<br />2.在`settings.gradle`文件中添加需要修改的`aar`库的源码`project`
```groovy
include ':retrofit-source'
project(':retrofit-source').projectDir = new File("../retrofit-source")
```
3.替换`aar`为源码<br />`build.gradle(android)` 脚本中添加替换策略
```groovy
allprojects {
    repositories {
...
    }
 
    configurations.all {
        resolutionStrategy {
            dependencySubstitution {
                substitute module( "com.squareup.retrofit2:retrofit") with project(':retofit-source')
            }
        }
    }
}
```
如上几步，就可以比较方便地实现`aar`依赖与源码依赖间的互换了<br />这样做的主要优点在于<br />1.不需要修改原有的依赖配置，而是通过全局的配置，利用本地的源码替换掉`aar`，侵入性低<br />2.如果有多个`Module`依赖于同一个`aar`，不需要重复修改，只需在根目录`build.gradle`中修改一处
