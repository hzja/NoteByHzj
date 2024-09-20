Java<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1687616889781-cc12bef2-27ea-4279-ad76-a22dba2c60e2.jpeg#averageHue=%23d1cfd2&clientId=u0d66556c-e4b4-4&from=paste&id=u3c0307bd&originHeight=186&originWidth=309&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u9009f0fc-9de4-41e2-9706-49337acee88&title=)
<a name="bwqw7"></a>
## 前言
如果想构建一个支持命令行参数的程序，那么 jcommander 非常适合你，jcommander 是一个只有几十 kb 的 Java 命令行参数解析工具，可以通过注解的方式快速实现命令行参数解析。<br />这篇教程会通过介绍 jcommadner ，快速的创建一个命令行程序，最后支持的命令参数功能如下图。<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1687616889721-52cc1a4f-6003-44e0-ae6e-36db1b5657fa.jpeg#averageHue=%23161615&clientId=u0d66556c-e4b4-4&from=paste&id=ue9fc74dd&originHeight=785&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u435414ed-b2cd-4f34-b352-a70e1167a65&title=)<br />这个命令行工具仿照 git 操作命令，主要提供了如下功能命令：

1. `git-app.jar -help` 查看命令帮助信息。
2. `git-app.jar -version` 查看当前版本号。
3. `git-app.jar clone http://xxxx` 通过 URL 克隆一个仓库。
4. `git-app.jar add file1 file2` 暂存 file1 文件 file2 文件。
5. `git-app.jar commit -m "注释"` 提交并添加注释。
<a name="LpY2x"></a>
## jcommander 引入
截目前，最新版本如下：
```xml
<!-- https://mvnrepository.com/artifact/com.beust/jcommander -->
<dependency>
  <groupId>com.beust</groupId>
  <artifactId>jcommander</artifactId>
  <version>1.82</version>
</dependency>
```
<a name="NVNVx"></a>
## jcommander 参数绑定
命令行解析中，参数解析与绑定是最实用的一个场景，jcommander 使用 Parameter 注解进行参数绑定。定义一个 GitCommandOptions.java 类来测试参数绑定。
```java
package com.byte.jcommander.v1;

import com.beust.jcommander.Parameter;

public class GitCommandOptions {
    @Parameter(names = {"clone"},
        description = "克隆远程仓库数据")
    private String cloneUrl;

    public String getCloneUrl() {
        return cloneUrl;
    }
}
```
使用 jcommander 结合 `GitCommandOptions` 来解析参数。
```java
package com.wdbyte.jcommander.v1;

import com.beust.jcommander.JCommander;

public class GitApp {

    public static void main(String[] args) {
        // args = new String[]{"clone","http://www.byte.com/test.git"};
        GitCommandOptions gitCommandOptions = new GitCommandOptions();
        JCommander commander = JCommander.newBuilder()
        .addObject(gitCommandOptions)
        .build();
        commander.parse(args);
        System.out.println("clone " + gitCommandOptions.getCloneUrl());
    }
}
```
打包后可以执行命令参数：
```bash
$ java -jar git-app.jar clone http://www.byte.com/test.git
clone http://www.byte.com/test.git
```
这里是一个字符串参数，需要在命令中输出参数值，对于 boolean 类型的参数，不需要传值，有命令即为 true 值。
<a name="MuBRJ"></a>
### 参数名称
`@Parameter` 注解中的 names 属性可以定义参数的名称。且可以指定多个参数名称，再添加 `version` 参数和 help 参数，同时设置参数别名。这两个参数是 boolean 类型。
```java
@Parameter(names = {"help", "-help", "-h"},
    description = "查看帮助信息",
    help = true)
private boolean help;

@Parameter(names = {"version", "-version", "-v"},
    description = "显示当前版本号")
private boolean version = false;
```
<a name="tZSOx"></a>
### 参数限制
clone 参数可以接受一个要克隆的 URL 链接，但是正常情况下只需要一个 URL 链接。可以通过 `arity = 1` 进行限制。
```java
@Parameter(names = {"clone"},
    description = "克隆远程仓库数据",
    arity = 1)
private String cloneUrl;
```
<a name="LeKZ9"></a>
### 帮助信息
使用 `usage()` 参数可以打印命令帮助信息。
```java
GitCommandOptions gitCommandOptions = new GitCommandOptions();
JCommander commander = JCommander.newBuilder()
    .addObject(gitCommandOptions)
    .build();
commander.parse(args);
// 打印帮助信息
commander.usage();
```
运行输出帮助信息：
```bash
$ java -jar git-app.jar
Usage: <main class> [options]
  Options:
    clone
      克隆远程仓库数据
    help, -help, -h
      查看帮助信息
    version, -version, -v
      显示当前版本号
      Default: false
```
虽然正确的输出了帮助信息，但是其中有 main class 这段，是因为没有指定项目名称，指定项目名称为 git-app。
```java
JCommander commander = JCommander.newBuilder()
            .programName("git-app")
            .addObject(gitCommandOptions)
            .build();
```
<a name="gfWfr"></a>
### 参数排序
在帮助信息中，如果想要自定义参数顺序，可以通过 order = 来排序，数字越小越靠前。
```java
@Parameter(names = {"version", "-version", "-v"},
    description = "显示当前版本号",
    order = 2)
private boolean version = false;
```
<a name="sMd1e"></a>
### 参数绑定完整测试
```java
package com.byte.jcommander.v2;
import com.beust.jcommander.Parameter;

public class GitCommandOptions {

    @Parameter(names = {"help", "-help", "-h"},
        description = "查看帮助信息",
        order = 1,
        help = true)
    private boolean help;

    @Parameter(names = {"clone"},
        description = "克隆远程仓库数据",
        order = 3,
        arity = 1)
    private String cloneUrl;

    @Parameter(names = {"version", "-version", "-v"},
        description = "显示当前版本号",
        order = 2)
    private boolean version = false;
    //...get method
}
```
GitApp.java
```java
package com.byte.jcommander.v2;

import com.beust.jcommander.JCommander;

public class GitApp {

    public static void main(String[] args) {
        GitCommandOptions gitCommandOptions = new GitCommandOptions();
        JCommander commander = JCommander.newBuilder()
            .programName("git-app")
            .addObject(gitCommandOptions)
            .build();
        commander.parse(args);
        // 打印帮助信息
        if (gitCommandOptions.isHelp()) {
            commander.usage();
            return;
        }
        if (gitCommandOptions.isVersion()) {
            System.out.println("git version 2.24.3 (Apple Git-128)");
            return;
        }
        if (gitCommandOptions.getCloneUrl() != null) {
            System.out.println("clone " + gitCommandOptions.getCloneUrl());
        }
    }
}
```
运行测试：<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1687616889765-234d6d82-187b-4035-ae24-47940adbeb62.jpeg#averageHue=%231b1e1c&clientId=u0d66556c-e4b4-4&from=paste&id=u2f3b4d31&originHeight=473&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u70df03a2-2850-4b11-929e-4e91e8bcedc&title=)
<a name="iYj7C"></a>
## jcommander 参数验证
在上面的例子中， 假设 clone 命令传入的参数必须是一个 URL，那么就要进行参数验证，jcommander 也提供了特有的参数验证方式。

1. 编写参数验证类，需要实现 IParameterValidator 接口。
```java
package com.byte.jcommander.v3;

import java.net.MalformedURLException;
import java.net.URL;

import com.beust.jcommander.IParameterValidator;
import com.beust.jcommander.ParameterException;

public class UrlParameterValidator implements IParameterValidator {
    @Override
    public void validate(String key, String value) throws ParameterException {
        try {
            new URL(value);
        } catch (MalformedURLException e) {
            throw new ParameterException("参数 " + key + " 的值必须是 URL 格式");
        }
    }
}
```

2. clone 参数指定验证类。
```java
@Parameter(names = {"clone"},
    description = "克隆远程仓库数据",
    validateWith = UrlParameterValidator.class,
    order = 3,
    arity = 1)
private String cloneUrl;
```
运行测试：
```bash
$ java -jar git-app.jar clone https://www.byte.com/test.git
clone https://www.byte.com/test.git

$ java -jar git-app.jar clone test.git
Exception in thread "main" com.beust.jcommander.ParameterException: 参数 clone 的值必须是 URL 格式
    at com.wdbyte.jcommander.v3.UrlParameterValidator.validate(UrlParameterValidator.java:19)
    at com.beust.jcommander.ParameterDescription.validateParameter(ParameterDescription.java:377)
    at com.beust.jcommander.ParameterDescription.validateParameter(ParameterDescription.java:344)
```
<a name="ujngi"></a>
## jcommander 子命令
在使用 git 时，经常会使用下面两个命令。

1. `git add file1 file2` 暂存 file1 文件 file2 文件。
2. `git commit -m "注释"` 提交并添加注释。
<a name="m8xkA"></a>
### 什么是子命令
这是一种很常见的操作，git commit 除了可以跟 -m 子参数外，还可以跟各种参数，通过 git 帮助文档可以看到。
```bash
git commit [-a | --interactive | --patch] [-s] [-v] [-u<mode>] [--amend]
           [--dry-run] [(-c | -C | --fixup | --squash) <commit>]
           [-F <file> | -m <msg>] [--reset-author] [--allow-empty]
           [--allow-empty-message] [--no-verify] [-e] [--author=<author>]
           [--date=<date>] [--cleanup=<mode>] [--[no-]status]
           [-i | -o] [-S[<keyid>]] [--] [<file>...]
```
这种有子参数的情况，可以称 commit 为 git 的一个子命令，使用 jcommander 如何配置子命令呢？
<a name="aslPS"></a>
### jcommander 子命令实现
新增子命令对应的参数类 GitCommandCommit.java.
```java
package com.byte.jcommander;

import com.beust.jcommander.Parameter;
import com.beust.jcommander.Parameters;

/**
 * git commit -m "desc"
 */
@Parameters(commandDescription = "提交文件", commandNames = "commit")
public class GitCommandCommit {
  
    public static final String COMMAND = "commit";
  
    @Parameter(names = {"-comment", "-m"},
        description = "请输入注释",
        arity = 1,
        required = true)
    private String comment;

    public String getComment() {
        return comment;
    }
}
```
代码中使用 `@Parameters` 注解指定了子命令为 commit，同时使用 `@Paramete` 注解指定子参数 `-m`，同时 `-m` 参数是必须的，使用属性 `required = true` 来指定。<br />**使用 **`**GitCommandCommit**`**：**<br />使用 `addCommand` 添加 Commit 命令参数类。
```java
GitCommandOptions gitCommandOptions = new GitCommandOptions();
GitCommandCommit commandCommit = new GitCommandCommit();
JCommander commander = JCommander.newBuilder()
    .programName("git-app")
    .addObject(gitCommandOptions)
    .addCommand(commandCommit)
    .build();
commander.parse(args);

String parsedCommand = commander.getParsedCommand();
if ("commit".equals(parsedCommand)) {
    System.out.println(commandCommit.getComment());
}
```
运行测试：
```bash
$ java -jar git-app.jar commit -m '注释一下'
注释一下
```
同上，可以添加 add 命令对应的参数类：GitCommandAdd.java. 这次定义一个 List 类型参数，但是不在属性上指定子参数名称。
```java
package com.byte.jcommander.v5;

import java.util.List;

import com.beust.jcommander.Parameter;
import com.beust.jcommander.Parameters;

/**
 * git add file1 file2
 */
@Parameters(commandDescription = "暂存文件", commandNames = "add", separators = " ")
public class GitCommandAdd {
    public static final String COMMAND = "add";
    @Parameter(description = "暂存文件列表")
    private List<String> files;

    public List<String> getFiles() {
        return files;
    }
}
```
同样添加到子命令：
```java
JCommander commander = JCommander.newBuilder()
    .programName("git-app")
    .addObject(gitCommandOptions)
    .addCommand(commandCommit)
    .addCommand(commandAdd)
    .build();
commander.parse(args);
if ("add".equals(parsedCommand)) {
    for (String file : commandAdd.getFiles()) {
        System.out.println("暂存文件：" + file);
    }
}
```
运行测试：
```bash
$ java -jar git-app.jar add file1.txt file2.txt
暂存文件：file1.txt
暂存文件：file2.txt
```
<a name="KPIBg"></a>
## jcommander 参数转换
在上面的 `GitCommandAdd` 代码中，add 命令传入的都是文件路径，现在是使用 `List<String>` 来接收入参，通常情况想可能需要直接转换成方便操作的类型，如 File 或者 Path，这该如何方面的转换呢，jcommander 也提供了方便转换类。<br />首先编写一个转换类 `FilePathConverter` 用于把入参转换成 Path 类，同时校验文件是否存在
```java
package com.wdbyte.jcommander;

import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;

import com.beust.jcommander.IStringConverter;
import com.beust.jcommander.ParameterException;

public class FilePathConverter implements IStringConverter<Path> {

    @Override
    public Path convert(String filePath) {
        Path path = Paths.get(filePath);
        if (Files.exists(path)) {
            return path;
        }
        throw new ParameterException(String.format("文件不存在，path:%s", filePath));
    }
}
```
通过注解指定转换类：
```java
@Parameter(description = "暂存文件列表", converter = FilePathConverter.class)
private List<Path> files;
```
打包测试：
```bash
$ java -jar git-app.jar add file1 file2
文件不存在，path:file1

$ ls -l
total 12448
drwxr-xr-x  2 darcy  staff    64B  6 15 21:10 archive-tmp
drwxr-xr-x  3 darcy  staff    96B  6 15 21:10 classes
drwxr-xr-x  3 darcy  staff    96B  6 15 21:10 generated-sources
-rw-r--r--  1 darcy  staff   5.6M  6 16 20:44 git-app.jar

$ git-app.jar git-app.jar
暂存文件：git-app.jar
```
<a name="dfPiV"></a>
## 总体测试
![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1687616889753-ea5f5df5-1f9c-40d6-8828-a09c2abea3ed.jpeg#averageHue=%23171717&clientId=u0d66556c-e4b4-4&from=paste&id=ue6fe6aa4&originHeight=932&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u38c532c4-703d-4a3a-928a-792d6ac2e7f&title=)<br />参考：[https://jcommander.org/](https://jcommander.org/)
