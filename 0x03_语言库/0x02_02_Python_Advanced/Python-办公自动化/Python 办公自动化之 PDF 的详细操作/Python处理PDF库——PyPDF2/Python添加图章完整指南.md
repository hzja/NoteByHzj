在实际工作和项目中，给PDF文件添加图章是一个常见的需求。本文将详细介绍如何使用Python和PyPDF2库来实现这一功能，并提供丰富的示例代码以全面展示实际应用。
<a name="ECFFO"></a>
## 1、安装PyPDF2库
首先，确保安装了PyPDF2库。可以使用以下命令进行安装：
```bash
pip install PyPDF2
```
<a name="Vq76d"></a>
## 2、创建PDF文件
为了演示添加图章的过程，首先创建一个简单的PDF文件。以下是一个使用reportlab库生成PDF的示例代码：
```python
from reportlab.pdfgen import canvas

def create_pdf(file_path):
    c = canvas.Canvas(file_path)
    c.drawString(100, 750, "Hello, PDF!")
    c.save()

create_pdf("example.pdf")
```
<a name="pkYd3"></a>
## 3、添加图章
现在，将创建一个图章，并将其添加到上面生成的PDF文件中。以下是完整的示例代码：
```python
from PyPDF2 import PdfReader, PdfWriter

def add_stamp(input_path, output_path, stamp_path, x=100, y=100):
    # 读取PDF文件和图章
    with open(input_path, "rb") as file:
        pdf_reader = PdfReader(file)
        stamp_reader = PdfReader(stamp_path)

    # 创建PDF写入对象
    with open(output_path, "wb") as file:
        pdf_writer = PdfWriter(file)

        # 获取PDF页面
        pdf_page = pdf_reader.pages[0]
        stamp_page = stamp_reader.pages[0]

        # 合并页面并添加图章
        pdf_page.merge_page(stamp_page)
        pdf_writer.add_page(pdf_page)

        # 保存结果
        pdf_writer.write(file)

# 添加图章到PDF文件
add_stamp("example.pdf", "stamped_example.pdf", "stamp.pdf")
```
在上述代码中，使用PyPDF2库读取PDF文件和图章，然后合并页面并保存结果。可以通过调整x和y参数来指定图章的位置。
<a name="xH5Hv"></a>
## 4、完整代码
以下是完整的添加图章的Python代码，包括创建PDF文件和添加图章的步骤：
```python
from reportlab.pdfgen import canvas
from PyPDF2 import PdfReader, PdfWriter

def create_pdf(file_path):
    c = canvas.Canvas(file_path)
    c.drawString(100, 750, "Hello, PDF!")
    c.save()

def add_stamp(input_path, output_path, stamp_path, x=100, y=100):
    with open(input_path, "rb") as file:
        pdf_reader = PdfReader(file)
        stamp_reader = PdfReader(stamp_path)

    with open(output_path, "wb") as file:
        pdf_writer = PdfWriter(file)

        pdf_page = pdf_reader.pages[0]
        stamp_page = stamp_reader.pages[0]

        pdf_page.merge_page(stamp_page)
        pdf_writer.add_page(pdf_page)

        pdf_writer.write(file)

# 创建PDF文件
create_pdf("example.pdf")

# 添加图章到PDF文件
add_stamp("example.pdf", "stamped_example.pdf", "stamp.pdf")
```
<a name="iq5NG"></a>
## 5、自定义图章
如果想要使用自定义图章，可以通过reportlab库创建一个带有文字或图形的PDF文件，然后将其作为图章添加到目标PDF文件中。以下是一个简单的示例代码：
```python
from reportlab.pdfgen import canvas

def create_stamp(file_path, text="Stamp"):
    c = canvas.Canvas(file_path)
    c.drawString(10, 10, text)
    c.save()

# 创建自定义图章
create_stamp("custom_stamp.pdf", "Custom Stamp")

# 添加自定义图章到PDF文件
add_stamp("example.pdf", "stamped_example.pdf", "custom_stamp.pdf")
```
在这个示例中，使用reportlab创建了一个包含自定义文本的PDF文件作为图章，然后通过前述的`add_stamp`函数将其添加到目标PDF文件中。
<a name="IpCxe"></a>
## 6、处理多页PDF
如果目标PDF文件有多页，可以通过循环遍历每一页并分别添加图章。以下是修改后的代码：
```python
def add_stamp_to_all_pages(input_path, output_path, stamp_path, x=100, y=100):
    with open(input_path, "rb") as file:
        pdf_reader = PdfReader(file)
        stamp_reader = PdfReader(stamp_path)

    with open(output_path, "wb") as file:
        pdf_writer = PdfWriter(file)

        for pdf_page in pdf_reader.pages:
            stamp_page = stamp_reader.pages[0]
            pdf_page.merge_page(stamp_page)
            pdf_writer.add_page(pdf_page)

        pdf_writer.write(file)

# 添加图章到多页PDF文件
add_stamp_to_all_pages("multi_page_example.pdf", "stamped_multi_page_example.pdf", "stamp.pdf")
```
这样，通过循环遍历每一页，将图章添加到所有页面，实现了对多页PDF文件的处理。
<a name="pht31"></a>
## 总结
在本文中，详细介绍了如何使用Python通过PyPDF2库给PDF文件添加图章。首先，学习如何使用reportlab库创建简单的PDF文件，作为后续添加图章的目标。然后，展示了如何读取PDF文件和图章，并通过PyPDF2库实现图章的合并与保存。<br />通过丰富的示例代码，演示如何自定义图章，无论是包含文本还是其他图形。对于多页PDF文件，提供了循环处理每一页的方法，确保在整个文档中添加图章。<br />这篇文章为大家提供一个全面的了解，使他们能够灵活应用这一功能，例如在合同签名、文件标记等场景中。通过掌握这一技能，可以更高效地进行PDF文档处理，满足个性化的需求。
