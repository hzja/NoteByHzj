Python ini配置文件
<a name="gYAa3"></a>
### 简介
开发人员每天都在处理一些大型而复杂的项目， 而配置文件会节省不少时间。在处理配置文件过程中，无需更改源代码本身，只需要调整配置文件即可访问不同的API接口、更新基础URL信息或其它事情。<br />尽管可以通过多种方式来支持软件中的配置文件，包括JSON，YAML和纯文本文件。
<a name="GieZl"></a>
### ini配置文件结构
在编写代码之前，看一下`ini`实际配置文件的结构。
```
[DEFAULT]
admin_page = no
landing_page = yes
moderator_page = no  # looks good here
[moderator]
moderator_page = yes
[admin]
admin_page = yes
moderator_page = yes
```
在当前的示例中，有一个名为`config.ini`的配置文件。它由三个部分组成，每个部分都由一个标题和一个键-值对列表组成，该标题封装在方括号内。<br />`moderator`和`admin`模块只是键值对的集合，但`DEFAULT`模块（第一个模块）有些特殊。如果其他模块之一未提供某个键的值，则它就会包含默认值。因此，如果想尝试访问其他模块中的值，但该键不存在，则解析器将从默认部分（如果存在）中返回该值，而不是引发`KeyError`。<br />在本例中，通过此配置文件管理用户的页面访问。因此，默认部分代表普通用户的权限，而`moderator`和`admin`模块分别包含协管员和管理员的权限。
<a name="id2OE"></a>
### 访问文件的内容
`ConfigParser`对象是主要的配置解析器和configparser模块的主要对象。可以使用映射协议实现自己的配置解析器。尽管`ConfigParser`接受很多参数，这里使用他的默认值。在工作目录中创建一个名为`parser_playground.py`的新文件。首先，从`configparser`模块导入`ConfigParser`类，并创建该类的实例。
```python
# parser_playground.py
from configparser import ConfigParser
config = ConfigParser()
```
配置对象不包含任何信息， 要更改此设置，需要先读取`config.ini`文件。这可以通过调用`ConfigParser`实例（此处为`config`）的`read()`方法来完成。
```python
# previous code in parser_playground.py
config.read("config.ini")
```
读取配置文件后，可以开始探索如何访问其中存储的信息。首先要列出所有可用的部分。这可以通过使用`ConfigParser`的`sections()`方法来实现：
```python
# previous code in parser_playground.py
print(f"Sections: {config.sections()}")  # Sections: ['moderator', 'admin']
```
此外，可以使用解析器的`has_section()`方法检查某个模块是否存在：
```python
# previous code in parser_playground.py
print(f'Does a section called "admin" exist: {config.has_section("admin")}')  # True
print(f'Does a section called "user" exist: {config.has_section("user")}')  # False
print(f'Does a section called "DEFAULT" exist: {config.has_section("DEFAULT")}')  # False
```
:::warning
注意：调用`sections()`方法时，既未列出默认模块·，也没有被`has_section()`方法确认。
:::
接下来，要访问各个值。但是在使用其标识符访问特定值之前，可以使用`options()`方法列出一个模块的所有可用选项，并提供模块名作为参数：
```python
# Previous code in parser_playground.py
print(f'Options: {config.options("admin")}')  # Options: ['admin_page', 'moderator_page', 'landing_page']
```
此外，可以利用`has_option()`方法来检查给定部分是否包含某个选项：
```python
# Previous code in parser_playground.py
print(f'"admin_page" in "admin" section: {config.has_option("admin", "admin_page")}')
```
要访问模块的值，可以使用解析器的`get()`方法并提供区域名称和选项名称。这些值将始终为字符串（如果存在）。如果需要其他格式的文件，请考虑使用相应的`getboolean()`，`getint()`和`getfloat()`方法。他们将尝试将字符串解析为所需的数据类型。<br />总结本节内容时有个概念需要提到，它就是映射协议访问。这个通用名称意味着可以像处理字典一样访问值。也就是说，可以使用`config ["section"] ["option"]`类似的方法来访问某个值，甚至检查某个模块中是否存在某个选项：
```python
# Previous code in parser_playground.py
print("admin_page" in config["admin"])  # True
print(config["admin"]["admin_page"])  # yes
```
<a name="iyYUA"></a>
### 修改信息
如何添加或更改信息并将其写回到配置文件中。同样，从模块开始。要添加一个模块，可以使用`ConfigParser`的`add_section()`方法。它接受模块名称作为字符串，并将相应模块添加到解析器。提供不同的数据类型会导致`TypeError`。如果该模块已经存在，则会引发`DuplicateSectionError`。尝试将该模块命名为默认值会导致`ValueError`。
```python
# Previous code in parser_playground.py
config.add_section("unknown")
print(f'Sections: {config.sections()}')  # Sections: ['moderator', 'admin', 'unknown']
```
要删除一个模块，只需使用`remove_section()`方法。
```python
# Previous code in parser_playground.py
config.remove_section("unknown")
print(f'Sections: {config.sections()}')  # Sections: ['moderator', 'admin']
```
Python的`ConfigParser`对象提供了类似的操作选项的方法。例如，调用`set()`方法，不仅可以向模块添加新选项，还可以更新现有选项。同样，如果想完全删除某个选项，可以使用解析器的`remove_option()`方法。
```python
# Previous code in parser_playground.py
config.set("admin", "admin_page", "false")
config.remove_option("admin", "moderator_page")
print(f'Options in "admin" section: {config.items("admin")}')
```
处理完配置后，可以将其写回到相同或不同的文件，如下所示：
```python
# Previous code in parser_playground.py
with open("config1.ini", "w") as f:
    config.write(f)
```
<a name="q5TdP"></a>
### 插值
最后但也很重要的一点，看一下使`ConfigParser`优于Python的json模块的东西：插值。插值意味着可以在调用某些`get()`方法返回值之前对其进行预处理。`configparser`模块提供两个插值类：`BasicInterpolation`和`ExtendedInterpolation`。第一个只允许重用配置文件中同一模块中的选项，并且其语句不如后一个类的语句优雅。这就是为什么只用`ExtendedInterpolation`类。<br />以下代码段显示了使用`Extended Interpolation`语句的配置文件。
```
# interpolation_config.ini
[paths]
root_dir = /home/florian
downloads_dir = ${root_dir}/Downloads
[destinations]
app_dir = ${paths:downloads_dir}/application/python
```
本质上，第一个模块定义了根目录的路径。该路径用作第二个选项的前缀，即下载目录的路径。在第二个模块中，有一个选项`app_dir`，它重用了`pahts`模块中对`downloads`目录的定义。<br />为了掌握这一点，在实例化解析器时让`ConfigParser`使用`ExtendedInterpolation`作为插值类型：
```python
# interpolation.py
from configparser import ConfigParser
from configparser import ExtendedInterpolation
config = ConfigParser(interpolation=ExtendedInterpolation())
config.read("interpolation_config.ini")
```
如果现在打印`destinations`模块的`app_dir`选项的值，则会得到一个插值字符串。
```python
# Previous code in interpolation.py
print(config.get("destinations", "app_dir"))
```
```bash
$ python interpolation.py
/home/florian/Downloads/application/python
```
