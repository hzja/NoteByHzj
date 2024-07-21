Python Template<br />很多时候需要进行生成报告、输出文件或字符串的任务。它们或多或少都会遵循某种模式，通常这些模式是如此相似，以至于希望拥有一个可以重用并直接输入数据的模板。<br />幸运的是，Python提供了一个可以模板工具类：`string.Template`。利用模板工具类根据当前使用的数据生成输出文件，以及以相同的方式操作字符串。该工具将此类用于生成报告文件。<br />环境基于Python 3.9.0（CPython）。
<a name="U0FTn"></a>
### 使用`string.Template`相对于其他解决方案的优势
1、无需其他依赖项，开箱即用，因此不需要使用`pip install`命令安装。<br />2、它是轻量级的，当然诸如`Jinja2`和`Mako`之类的模板引擎已被广泛使用。<br />3、关注点分离：可以使用模板文件将其移动到外部位置，而不是直接在代码中嵌入字符串操作和报告生成。<br />如果要更改报告的结构或设计，则可以交换模板文件，而无需更改代码。由于这些优点，一些知名的第三方库和工具正在使用它。Wily是一个例子，在2018年底，Wily的发明者和维护者Anthony Shaw希望支持HTML作为wily生成的报告的输出格式。<br />**示例：生成最佳图书的报告**<br />在讨论了使用Python的内置`string.Template`类背后的动机之后，看一下第一个实际示例。假设有一个名为`data.json`的JSON文件，其中包含作者姓名和书名的映射，如下所示。
```json
{
    "Dale Carnegie": "How To Win Friends And Influence People",
    "Daniel Kahneman": "Thinking, Fast and Slow",
    "Leo Tolstoy": "Anna Karenina",
    "William Shakespeare": "Hamlet",
    "Franz Kafka": "The Trial"
}
```
现在的任务是以一种可以与他人共享的方式（例如， 大型杂志、公司或博主）。使用HTML格式的简单表格就足够了。现在的问题是：如何生成此HTML表格？当然，可以手动执行此操作，也可以为每本书创建占位符。但是后面如果能拥有更通用的版本是非常可取的，因为可以扩展列表内容或更改结构设计。现在可以利用Python的`string.Template`类！首先创建实际的模板，如下所示。在这里，将文件称为`template.html`。
```html
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>Great Books of All Time</title>
    <link rel="stylesheet" href="https://cdn.jsdelivr.net/npm/bootstrap@4.5.3/dist/css/bootstrap.min.css" integrity="sha384-TX8t27EcRE3e/ihU7zmQxVncDAy5uIKz4rEkgIXeMed4M0jlfIDPvg6uqKI2xXr2" crossorigin="anonymous">
</head>
<body>
    <div class="container">
        <h1>Great Books of All Time</h1>
        <table class="table">
            <thead>
                <tr>
                    <th scope="col">#</th>
                    <th scope="col">Author</th>
                    <th scope="col">Book Title</th>
                </tr>
            </thead>
            <tbody>
                ${elements}
            </tbody>
        </table>
    </div>
</body>
</html>
```
该文件本身非常初级。使用引导程序进行样式设置，并创建了最终表的基本结构。表头已包含在内，但数据仍然丢失。请注意，在`tbody`元素中，使用了一个占位符`${elements}`来标记稍后将注入书籍列表的位置。把所有都已设置为实现生成所需输出的Python脚本！因此，在当前工作目录中创建一个名为`report.py`的新Python文件。首先，导入所需的两个内置模块，并从JSON文件加载数据。
```python
# report.py
import json
import string
with open("data.json") as f:
    data = json.loads(f.read())
```
现在，`data`变量是一个字典，其中包含作者的名称（键）和书名（值）作为键值对。接下来，生成HTML表，将其放入模板中（还记得占位符吗？）。因此，初始化一个空字符串，向其添加新的表行，如下所示。
```python
content = ""
for i, (author, title) in enumerate(data.items()):
    content += "<tr>"
    content += f"<td>{i + 1}</td>"
    content += f"<td>{author}</td>"
    content += f"<td>{title}</td>"
    content += "</tr>"
```
该代码段显示了遍历数据字典中的所有项目，并将书名以及作者的名字放在相应的HTML标签中。创建了最终的HTML表。在下一步中，需要加载之前创建的模板文件：
```python
with open("template.html") as t:
    template = string.Template(t.read())
```
注意，`string.Template`接受一个字符串，而不是一个文件路径。因此，还可以提供在程序中先前创建的字符串，而无需将其保存到文件中。提供了`template.html`文件的内容。最后使用模板的`replace()`方法将占位符元素替换为存储在变量内容中的字符串。该方法返回一个字符串，将其存储在变量`final_output`中。最后但并非最不重要的一点是，创建了一个名为`report.html`的新文件，并将最终输出写入该文件。
```python
final_output = template.substitute(elements=content)
with open("report.html", "w") as output:
    output.write(final_output)
```
现在已经生成了第一个文件报告！如果在浏览器中打开`report.html`文件，则可以看到结果。
<a name="Py7oH"></a>
### `safe_substitution()`方法
现在，已经构建了第一个`string.Template`用例，分享一个常见情况及其解决方案：安全替换。举个例子：有一个字符串，想在其中输入一个人的名字和姓氏。可以按照以下步骤进行操作：
```python
# safe_substitution.py
import string
template_string = "Your name is ${firstname} ${lastname}"
t = string.Template(template_string)
result = t.substitute(firstname="Florian", lastname="Dahlitz")
print(result)
```
但是，如果错过传递一个或另一个的值会怎样？它引发一个`KeyError`。为避免这种情况，可以利用`safe_substitution()`方法。在这种情况下，`safe`意味着Python在任何情况下都尝试返回有效字符串。因此，如果找不到任何值，则不会替换占位符。按以下方式调整代码：
```python
# safe_substitution.py
import string
template_string = "Your name is ${firstname} ${lastname}"
t = string.Template(template_string)
result = t.safe_substitute(firstname="Florian")
print(result)  # Your name is Florian ${lastname}
```
在某些情况下，这可能是一个更优雅的解决方案，甚至是必须的行为。但是这可能在其他地方引起意外的副作用。
