<a name="ylM5T"></a>
### 1、将所有待合并的jar包放到一个空目录中，通过jar命令解压得到所有的.class文件。
```bash
jar -xvf xxx.jar
```
xxx.jar必须为具体的jar，不能为 *.jar。虽然不会报错，但是解压无效，会立马返回。<br />jar 命令<br />用法：` jar {ctxui}[vfmOMe] [jar-file] [manifest-file] [entry-point] [-C dir] files …`<br />选项包括：<br />-c 创建新的归档文件<br />-t 列出归档目录<br />-x 解压缩已归档的指定（或所有）文件<br />-u 更新现有的归档文件<br />-v 在标准输出中生存详细输出<br />-f 指定归档文件名<br />-m 包含指定清单文件中的清单信息<br />-e 为捆绑到可执行 jar 文件的独立应用程序指定应用程序入口点<br />-O 仅存储；不使用任何ZIP压缩<br />-M 不创建条目的清单文件<br />-i 为指定的 jar 文件生成索引信息<br />-C 更改为指定的目录并包含其中的文件<br />如果有任何目录文件，则对其进行递归处理。<br />清单文件名、归档文件名和入口点名的指定顺序与“m”、“f”和“e”标志的指定顺序相同。<br />解压后将得到：原本包结构的class文件、META-INF文件夹、assembly-descriptor.xml文件。<br />（1）META-INF/MANIFEST.MF
```
Manifest-Version: 1.0
```
（2）assembly-descriptor.xml
```xml
<assembly>
  <id>release</id>
  <formats>
    <format>zip</format>
  </formats>
  <fileSets>
    <fileSet>
      <includes>
        <include>README*</include>
        <include>LICENSE*</include>
      </includes>
    </fileSet>
    <fileSet>
      <directory>target</directory>
      <outputDirectory></outputDirectory>
      <includes>
        <include>gson-*.jar</include>
      </includes>
    </fileSet>
  </fileSets>
</assembly>
```
<a name="PqGjf"></a>
### 2、删除临时目录下所有的jar文件。否则这些jar会和解压好的class一起被打包进去。
```bash
del /F *.jar
```
del 命令
```bash
查看帮助 del /?
DEL [/p] [/F] [/S] [/Q] [/A[[:]attributes]] names
ERASE [/P] [/F] [/S] [/Q] [/A[[:]attributes]] names
```
names 指定一个或多个文件或者目录列表。通配符可用来删除多个文件。如果指定了一个目录，该目录中的所有文件都会被删除。<br />/P 删除每一个文件之前提示确认<br />/F 强制删除只读文件<br />/S 删除所有子目录中的指定的文件<br />/Q 安静模式。删除全局通配符时，不要求确认<br />/A 根据熟悉选择要删除的文件<br />属性 R只读文件，S系统文件，H隐藏文件，A存档文件，I无内容索引文件，L重分析点、-表示”否”的前缀
<a name="E4tGp"></a>
### 3、合并所有.class文件至jar，需要切换至该临时目录，不然生成的jar会包含临时目录。注意最后还有个点，表示输出到当前目录。
```bash
jar -cvfM game.jar .
```
使用 jd-gui.exe 来查看就会发现这个jar整合了所有的jar源码，而不是以*.jar的方式显示出来了。<br />另外，如果用proguardgui.bat 进行混淆，如果引用的是第三方库，建议先将自己的jar混淆后再合并。
