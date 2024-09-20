Maven<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636344692800-7ec89dca-4ef1-4cd8-b496-0835e8108bbf.webp#averageHue=%23eae7ee&clientId=u5c399771-06e7-4&from=paste&id=u9cdedc04&originHeight=618&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u863016a8-5ae6-4ffc-a329-6c29fe03cee&title=)
<a name="kKChq"></a>
## 网络问题，无法下载 maven 依赖
一般来说，如果没有配置，Maven会在默认的中央仓库下载各项依赖，而访问这个地址，网络速度不仅慢，网络质量还差。这就导致了maven 依赖下载速度过慢，有时候下载还可能会失败。<br />maven 依赖下载失败，这就会导致 IDEA 找不到相关依赖。<br />那这个问题解决办法也比较简单，可以通过设置镜像(mirrors)地址解决。<br />打开本地 maven 配置文件，默认地址如下：`${user.home}/.m2/settings.xml`，然后找到 `mirrors`节点，在这下面添加以下配置：
```xml
<mirrors>
    <mirror>
      <id>alimaven</id>
      <name>aliyun maven</name>
      <url>http://maven.aliyun.com/nexus/content/groups/public/</url>
      <mirrorOf>central</mirrorOf>        
    </mirror>
</mirrors>
```
<a name="v0MxS"></a>
## lastUpdated 文件导致依赖无法正常导入
当更新镜像地址之后，有可能还会碰到 maven 依赖无法导入。<br />maven 依赖下载的时候，如果碰到网络较差，依赖下载失败，maven 本地将会生成 lastUpdated 为后缀的文件。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636344692772-1cc49fb1-c55e-48da-8354-49c101d851b0.webp#averageHue=%2351504c&clientId=u5c399771-06e7-4&from=paste&id=u9856b763&originHeight=120&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uec1fdb1d-f9a9-4f5a-8893-0062e50c245&title=)<br />那正常的 maven 依赖如下<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636344692796-bd9bc314-25a5-4c2a-a793-9cff98cbc0f1.webp#averageHue=%23424240&clientId=u5c399771-06e7-4&from=paste&id=uc023321c&originHeight=127&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=udb90c6e3-9285-4284-a8c9-ce6ec6e57c8&title=)<br />lastUpdated 为后缀的文件作用如下：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636344692848-8148a37b-bd8a-4a30-ba06-17bbf40a257b.webp#averageHue=%23f4f4f4&clientId=u5c399771-06e7-4&from=paste&id=u7132cad1&originHeight=512&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u63ef69ca-daca-4856-b441-7c052ee38a3&title=)<br />只要 lastUpdated 文件存在，maven 依赖将不会重新下载，所以做简单的方法就是，删除 lastUpdated 文件，重新刷新下载即可。<br />这里提供一键删除 lastUpdated 文件脚本(macos)
```bash
# 这里写你的仓库路径
REPOSITORY_PATH=~/.m2/repository
echo 正在搜索...
find $REPOSITORY_PATH -name "*lastUpdated*" | xargs rm -fr
echo 搜索完

windows 脚本如下：
# windows脚本
@echo off
rem create by NettQun
  
rem 这里写你的仓库路径
set REPOSITORY_PATH=D:\Java\maven-repository\maven-aliyun\repository
rem 正在搜索...
for /f "delims=" %%i in ('dir /b /s "%REPOSITORY_PATH%\*lastUpdated*"') do (
    echo %%i
    del /s /q "%%i"
)
rem 搜索完毕
pause
```
<a name="shQQW"></a>
## Maven 依赖冲突，引用错误
前两个问题主要跟本地网络有比较大的关系，那这个问题，主要是因为 maven 特性导致。<br />那 maven 有一个依赖传递的特性，如果 A 依赖 B，而 B 依赖 C，那么 C 这个依赖就会通过 B 间接传递给 A。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636344692853-a107913a-f25c-459f-b054-55cc7f34a16f.webp#averageHue=%23f8f8f6&clientId=u5c399771-06e7-4&from=paste&id=u7c3cc58a&originHeight=412&originWidth=692&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uc0dddc46-447a-4b59-95c8-8ed766e6cbc&title=)<br />那如果有多个间接依赖存在，但是彼此版本却不一样，这就会导致依赖冲突。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636344714903-2158ad5b-dbcd-44e4-89bf-0474772172dc.webp#averageHue=%23fcfbf8&clientId=u5c399771-06e7-4&from=paste&id=u6cafc0c8&originHeight=331&originWidth=681&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u60b5d2c6-8331-41ab-bab4-3edee4da597&title=)<br />如上所示，如果 A 应用 使用了 E2.0 新增某些类或者方法，那这个时候由于依赖冲突，A 实际上间接依赖的是 E1.0，这就导致 IDEA 编译的时候找不到 E 新增这些类或方法。<br />那这个问题解决办法，也比较简单，有冲突，就直接解决冲突就可以了。<br />可以在 IDEA 中安装一个 **Maven Helper** 插件，然后打开 pom 文件，点击 **Dependency Analyzer** 选项，在这里面选中 **Conflicts** 按钮，就可以看到当前所有冲突的依赖包。<br />选中其中一个依赖包，就可以在右侧看到所有冲突依赖包的版本。选中其中一个版本，右键选中 Exclude 即可。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636344693319-d1f61ae9-dd2d-4c30-ba1a-c26ac3395ef4.webp#averageHue=%231a1b21&clientId=u5c399771-06e7-4&from=paste&id=uae53ad20&originHeight=873&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u16da22f1-704f-49f5-a472-28a20e39c87&title=)<br />上面的办法通过排除其他间接依赖从而解决问题。那下面还有一种解决办法，将间接依赖转化为直接依赖。<br />可以把间接依赖直接写在项目 pom 文件里，这样 maven 将会直接使用这个依赖，其他间接依赖将会失效。
<a name="FFcc0"></a>
## Maven 间接依赖异常
这个问题，就比较诡异，这也是刚开头提到那个问题。<br />项目中依赖关系如下所示。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636344693253-6b1bb418-8dc0-419c-8b24-752e685b9967.webp#averageHue=%23fdfcfc&clientId=u5c399771-06e7-4&from=paste&id=u751bb804&originHeight=331&originWidth=761&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u8c82d6d5-b0f7-4909-8504-cb2c440f284&title=)<br />A 应用依赖 B ，那 B 是一个开源项目，但是公司内部对 B 进行一些修改。修改之后， B 依赖 C ，所以 A 中使用 C 依赖某些类。<br />那由于本地仓库已经下载了 B 开源版本，那当本地拉取 A 应用源码，那依赖关系就变成了 图中虚线关系。<br />IDEA 缺失了 C 依赖，这就导致项目编译错误，无法找到 C 中一些类。<br />那这个问题解决办法，就是在本地仓库找到 B 依赖，然后删除拉取即可。<br />解决办法比较简单，关键需要找到中间这个依赖。
<a name="I3cOz"></a>
## IDEA 本地缓存异常
最后一种情况，跟 IDEA 有比较大关系。<br />IDEA 之所以快，那是因为 IDEA 打开新的工程的时候，它会构建项目索引以及生成相关缓存。<br />那有时候，如果 IDEA 缓存被破坏，那就有可能导致 IDEA 无法识别 maven 依赖， 从而导致 IDEA 无法找到某些类。<br />这种情况需要删除原先的缓存以及索引，等待 IDEA 重新构建缓存以及索引即可。<br />在 IDEA 中点击 **File/Invalidate Cached** 即可。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636344693343-d3b987af-9898-46ff-9478-813ecd7d7ad0.webp#averageHue=%238e96a0&clientId=u5c399771-06e7-4&from=paste&id=u9d869ce5&originHeight=1016&originWidth=492&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u4c1fe4f8-0fae-4992-af4c-e01d65c1310&title=)
<a name="yzYZm"></a>
## 最后
如果如果碰到新的情况，以上几种解决办法都没办法解决，那么只能尝试使用终极解决办法试试看了。<br />**重启大法，重启 IDEA，重启电脑。**
