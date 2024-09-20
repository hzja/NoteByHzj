Java commons-cli<br />通常向终端中输入命令时，无论是启动 GUI 应用程序还是仅启动终端应用程序，都可以使用 命令行选项(options or switches or flags) （以下简称选项）来修改应用程序的运行方式。这是 POSIX 规范 设定的标准，因此能够检测和解析选项对 Java 程序员而言是很有用的技能。<br />Java 中有若干种解析选项的方法，其中最常用的是 Apache Commons CLI 库，简称 commons-cli。
<a name="aga3d"></a>
## 安装 commons-cli
如果使用类似 Maven 之类的项目管理系统以及集成开发环境(Integrated Development Environment)（简称 IDE），可以在项目属性（比如 pom.xml 配置文件或者 Eclipse 和 NetBeans 的配置选项卡）中安装 Apache Commons CLI 库。<br />而如果采用手动方式管理库，则可以从 Apache 网站下载 该库的最新版本。下载到本地的是几个捆绑在一起的 JAR 文件，只需要其中的一个文件 commons-cli-X.Y.jar（其中 X 和 Y 代指最新版本号）。把这个 JAR 文件或手动或使用 IDE 添加到项目，就可以在代码中使用了。
<a name="SFKkf"></a>
## 将库导入至 Java 代码
在使用 commons-cli 库之前，必须首先导入它。对于本次选项解析的简单示例而言，可以先在 Main.java 文件中简单写入以下标准代码：
```java
package com.opensource.myoptparser;
import org.apache.commons.cli.*;
public class Main {
    public static void main(String[] args) {
        // code 
    }
}
```
至此在 Java 中解析选项的准备工作已经做好了。
<a name="hfaJt"></a>
## 在 Java 中定义布尔选项
要实现解析选项，首先要定义应用程序可接收的有效选项。使用 Option（注意是单数）类来创建选项对象，使用 Options（注意是复数）类来追踪项目中创建的所有选项。<br />首先为选项创建一个组，按照惯例命名为 options：
```java
//code    
Options options = new Options();
```
接下来，通过列出短选项（即选项名简写）、长选项（即全写）、默认布尔值（LCTT 译注：设置是否需要选项参数，指定为 false 时此选项不带参，即为布尔选项）和帮助信息来定义选项，然后设置该选项是否为必需项（LCTT 译注：下方创建 alpha 对象的代码中未手动设置此项），最后将该选项添加到包含所有选项的 options 组对象中。在下面几行代码中，只创建了一个选项，命名为 alpha：
```java
//define options
Option alpha = new Option("a", "alpha", false, "Activate feature alpha");
options.addOption(alpha);
```
<a name="KEXXp"></a>
## 在 Java 中定义带参选项
有时用户需要通过选项提供 `true` 或 `false` 以外的信息，比如给出配置文件、输入文件或诸如日期、颜色这样的设置项值。这种情况可以使用 `builder` 方法，根据选项名简写为其创建属性（例如，`-c` 是短选项，`--config` 是长选项）。完成定义后，再将定义好的选项添加到 `options` 组中：
```java
Option config = Option.builder("c").longOpt("config")
    .argName("config")
    .hasArg()
    .required(true)
    .desc("set config file").build();
options.addOption(config);
```
`builder` 函数可以用来设置短选项、长选项、是否为必需项（本段代码中必需项设置为 `true`，也就意味着用户启动程序时必须提供此选项，否则应用程序无法运行）、帮助信息等。
<a name="DUHX9"></a>
## 使用 Java 解析选项
定义并添加所有可能用到的选项后，需要对用户提供的参数进行迭代处理，检测是否有参数同预设的有效短选项列表中的内容相匹配。为此要创建命令行 `CommandLine` 本身的一个实例，其中包含用户提供的所有参数（包含有效选项和无效选项）。为了处理这些参数，还要创建一个 `CommandLineParser` 对象，在代码中将其命名为 `parser`。最后，还可以创建一个 `HelpFormatter` 对象（将其命名为 `helper`），当参数中缺少某些必需项或者用户使用 `--help` 或 `-h` 选项时，此对象可以自动向用户提供一些有用的信息。
```java
// define parser
CommandLine cmd;
CommandLineParser parser = new BasicParser();
HelpFormatter helper = new HelpFormatter();
```
最后，添加一些条件判断来分析用户提供的选项，假设这些选项已经作为命令行输入被获取并存储在 cmd 变量中。这个示例应用程序有两种不同类型的选项，但对这两种类型都可以使用 `.hasOption` 方法加上短选项名称来检测选项是否存在。检测到一个存在的选项后，就可以对数据做进一步操作了。
```java
try {
    cmd = parser.parse(options, args);
    if(cmd.hasOption("a")) {
        System.out.println("Alpha activated");
    }
    if (cmd.hasOption("c")) {
        String opt_config = cmd.getOptionValue("config");
        System.out.println("Config set to " + opt_config);
    }
} catch (ParseException e) {
    System.out.println(e.getMessage());
    helper.printHelp("Usage:", options);
    System.exit(0);
}
```
解析过程有可能会产生错误，因为有时可能缺少某些必需项如本例中的 `-c` 或 `--config` 选项。这时程序会打印一条帮助信息，并立即结束运行。考虑到此错误（Java 术语中称为异常），在 `main` 方法的开头要添加语句声明可能的异常：
```java
public static void main(String[] args) throws ParseException {
```
示例程序至此就大功告成了。
<a name="uJ68e"></a>
## 测试代码
可以通过调整传递给代码的默认参数来在 IDE 中测试应用程序，或者创建一个 JAR 文件并在终端运行测试。这个过程可能会因 IDE 的不同而不同。<br />首先，省略必需项 `-c` 或 `--config` 选项，检测解析器的异常处理：
```bash
$ java -jar dist/myapp.jar                 
Missing required option: c
usage: Usage:
 -a,--alpha             Activate feature alpha
 -c,--config &lt;config&gt;   Set config file
```
然后提供输入选项再进行测试：
```bash
java -jar dist/myantapp.jar --config foo -a
Alpha activated
Config set to foo
```
<a name="MtYus"></a>
## 选项解析
为用户提供选项功能对任何应用程序来说都是很重要的。有了 Java 和 Apache Commons，要实现这个功能并不难。<br />以下是完整的演示代码，供参考：
```java
package com.opensource.myapp;
import org.apache.commons.cli.*;
public class Main {

    /**
     * @param args the command line arguments
     * @throws org.apache.commons.cli.ParseException
     */ 
    public static void main(String[] args) throws ParseException {
        // define options
        Options options = new Options();

        Option alpha = new Option("a", "alpha", false, "Activate feature alpha");
        options.addOption(alpha);

        Option config = Option.builder("c").longOpt("config")
            .argName("config")
            .hasArg()
            .required(true)
            .desc("Set config file").build();
        options.addOption(config);

        // define parser
        CommandLine cmd;
        CommandLineParser parser = new BasicParser();
        HelpFormatter helper = new HelpFormatter();
        try {
            cmd = parser.parse(options, args);
            if(cmd.hasOption("a")) {
                System.out.println("Alpha activated");
            }

            if (cmd.hasOption("c")) {
                String opt_config = cmd.getOptionValue("config");
                System.out.println("Config set to " + opt_config);
            }
        } catch (ParseException e) {
            System.out.println(e.getMessage());
            helper.printHelp("Usage:", options);
            System.exit(0);
        }
    }
}
```
<a name="USKwX"></a>
## 使用 Java 和选项
选项使用户可以调整命令的工作方式。使用 Java 时解析选项的方法有很多，其中之一的 commons-cli 是一个强大而灵活的开源解决方案。
