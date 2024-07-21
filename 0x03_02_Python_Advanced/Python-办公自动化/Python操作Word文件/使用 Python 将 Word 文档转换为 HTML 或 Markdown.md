Python<br />基于 Python 的 CLI — Mammoth的帮助下，以简单的方式将 .docx word 文档转换为简单的网页文档 ( .html ) 或 Markdown 文档 ( .md ) 。<br />据统计Statista调查（2020年1月6日），Microsoft Office套件是目前最流行的办公软件。可以使用 Microsoft Word 轻松地做快速笔记、简短报告、教程文档等。而且，可能希望将文档内容作为 Web 文档 ( .html )) 或 Markdown 文档 ( .md )与一些朋友、同事、客户共享。过去，在网络上托管一些网络文档可能会很昂贵，但现在云服务对于公共文档（例如GitHub Pages）来说非常便宜甚至免费。
<a name="LEtHm"></a>
### Install Mammoth
确保PC 上安装了 Python 和 PIP。然后，打开 CMD 或终端并使用以下命令：
```bash
pip install mammoth
```
<a name="VmPDT"></a>
### 将Docx 转换为HTML
使用命令行：
```bash
$ mammoth input_name.docx output_name.html
```
使用Python：
```python
import mammoth
with open("sample.docx", "rb") as docx_file:
    result = mammoth.convert_to_html(docx_file)
with open("sample.html", "w") as html_file:
    html_file.write(result.value)
```
<a name="y0PZb"></a>
### 将Docx 转换为MD
使用命令行：
```bash
$ mammoth .\sample.docx output.md --output-format=markdown
```
使用Python：
```python
with open("sample.docx", "rb") as docx_file:
    result = mammoth.convert_to_markdown(docx_file)
with open("sample.md", "w") as markdown_file:
    markdown_file.write(result.value)
```
