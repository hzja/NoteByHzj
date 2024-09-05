Linux<br />which，whereis和whatis都是Linux环境中常用的命令行工具，在日常使用Linux时应熟练掌握其区别和使用方法。
<a name="vQ2Dn"></a>
## which
which命令用于查询在PATH环境变量中定义的命令的位置，它会在所有PATH环境变量路径中查找指定的命令。如果存在多个同名的命令，which只会显示最先被找到的命令的路径。要使用which命令，只需在终端中输入which并加上需要查询的命令名。如下所示：
```bash
which python
```
这样将返回Python解释器的路径。在上述命令中，which将在PATH环境变量中查找Python命令的位置，并返回第一个找到的Python解释器所在的路径。需要尽早确定可执行文件的位置时，which命令很有用。
<a name="PGGcL"></a>
## whereis
whereis命令与which类似，不同的是whereis不仅会在PATH环境变量中查找命令，还会在系统预定义的固定路径中搜索其他文件资源，如命令的手册（man）和源代码等。whereis不会显示指定命令的路径，除非该命令被安装并且至少有一个目标文件在标准目录中找到。以下是whereis命令的示例：
```bash
whereis python
```
执行该命令将显示Python二进制文件、手册页和源代码所在的位置。whereis命令比which命令更适合搜索除PATH外的固定目录中的命令，因此可用于定位特定文件类型的位置。
<a name="XJ7TP"></a>
## whatis
whatis命令用于显示给定命令的单行摘要。whatis命令返回查询结果的简短描述，并在Linus中主要用于查询命令的功能和用法。以下是whatis命令的示例：
```bash
whatis python
```
在这个例子中，whatis命令将返回一个字符串，描述python命令的功能。whatis命令在需要快速知道命令用途的情况下，比直接查阅手册页（man page）更快捷。<br />根据上述介绍和示例，可以看出which，whereis和whatis三者的主要区别来自于搜索的内容和返回的结果。which命令主要用于找到PATH环境变量中命令的位置；whereis命令将在系统的固定目录中查找除了PATH以外的其他资源；whatis命令主要用于获得特定命令的功能和用途的描述。在日常使用Linux时，了解这些命令的使用方法和区别将有助于快速高效地定位和使用各种命令。
