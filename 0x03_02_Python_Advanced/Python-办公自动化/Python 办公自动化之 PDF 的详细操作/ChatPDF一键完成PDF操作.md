PythonPDF<br />ChatPDF 是一个基于 Python 的 PDF 处理工具，它可以用于创建、合并、拆分、加密和解密 PDF 文件，以及提取文本和图像等。ChatPDF 提供了简单易用的 API，使得用户可以方便地完成各种 PDF 处理任务。<br />下面来看一下怎么使用 ChatPDF ！
<a name="BYcEW"></a>
### 安装
首先需要安装 ChatPDF。可以在终端中使用以下命令进行安装：
```bash
pip install chatpdf
```
<a name="mnCmd"></a>
### 创建 PDF 文件
现在，让我们看一下如何使用 ChatPDF 创建 PDF 文件。以下是一个简单的示例：
```python
from chatpdf import ChatPDF

pdf = ChatPDF()
pdf.add_page()
pdf.set_font("Arial", size=12)
pdf.cell(200, 10, txt="Hello, World!")
pdf.output("hello.pdf")
```
这段代码将创建一个新的 PDF 文件，并在其中添加一个页面和一段文本。最后，它将输出到名为 "hello.pdf" 的文件中。
<a name="ogzSb"></a>
### 合并 PDF 文件
现在，让我们看一下如何使用 ChatPDF 合并多个 PDF 文件。以下是一个简单的示例：
```python
from chatpdf import merge

pdf_files = ['file1.pdf', 'file2.pdf', 'file3.pdf']
output_file = 'output.pdf'

merge(pdf_files, output_file)
```
以上代码将会合并 file1.pdf、file2.pdf 和 file3.pdf 这三个文件，并将合并后的结果保存到 output.pdf 文件中。
<a name="iV4lG"></a>
### 拆分 PDF 文件
将一个 PDF 文件拆分成多个 PDF 文件，可以使用 ChatPDF 库的 `split()` 方法。下面是一个示例代码：
```python
from chatpdf import split

pdf_file = 'input.pdf'
output_folder = 'output'

split(pdf_file, output_folder)
```
以上代码将会将 input.pdf 文件拆分成多个 PDF 文件，并保存到 output 文件夹中。
<a name="Rxb4K"></a>
### 提取 PDF 页面
从一个 PDF 文件中提取某些页面，可以使用 ChatPDF 库的 `extract_pages()` 方法。下面是一个示例代码：
```python
from chatpdf import extract_pages

pdf_file = 'input.pdf'
output_file = 'output.pdf'
pages = [1, 3, 5]

extract_pages(pdf_file, output_file, pages)
```
以上代码将会从 input.pdf 文件中提取第 1、3、5 页，并保存到 output.pdf 文件中。
<a name="ZYY5u"></a>
### 旋转 PDF 页面
将 PDF 页面旋转一定角度，可以使用 ChatPDF 库的 `rotate_pages()` 方法。下面是一个示例代码：
```python
from chatpdf import rotate_pages

pdf_file = 'input.pdf'
output_file = 'output.pdf'
pages = [1, 3]
rotation_angle = 90

rotate_pages(pdf_file, output_file, pages, rotation_angle)
```
以上代码将会将 input.pdf 文件中的第 1、3 页旋转 90 度，并保存到 output.pdf 文件中。
<a name="XNUOA"></a>
### 加密 PDF 文件
将 PDF 文件加密，可以使用 ChatPDF 库的 `encrypt()` 方法。下面是一个示例代码：
```python
from chatpdf import encrypt

pdf_file = 'input.pdf'
output_file = 'output.pdf'
password = 'mypassword'

encrypt(pdf_file, output_file, password)
```
以上代码将会将 input.pdf 文件加密，并使用密码 mypassword 进行保护。加密后的文件保存到 output.pdf 文件中。
<a name="kMhZM"></a>
### 解密 PDF 文件
将加密的 PDF 文件解密，可以使用 ChatPDF 库的 `decrypt()` 方法。下面是一个示例代码：
```python
from chatpdf import decrypt

pdf_file = 'input.pdf'
output_file = 'output.pdf'
password = 'mypassword'

decrypt(pdf_file, output_file, password)
```
以上代码将会将 input.pdf 文件解密，并使用密码 mypassword 进行解密。
