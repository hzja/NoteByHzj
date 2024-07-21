Python Word文档
<a name="leWxa"></a>
## 安装 python-docx
处理 Word 需要用到 `python-docx` 库，目前版本为 `0.8.10` ，执行如下安装命令：
```bash
$ pip install python-docx
################# 运行结果 ################
C:\Users\Fcant>pip install python-docx
Looking in indexes: https://pypi.doubanio.com/simple
Collecting python-docx
  Downloading https://pypi.doubanio.com/packages/e4/83/c66a1934ed5ed8ab1dbb9931f1779079f8bca0f6bbc5793c06c4b5e7d671/python-docx-0.8.10.tar.gz (5.5MB)
     |████████████████████████████████| 5.5MB 3.2MB/s
Requirement already satisfied: lxml>=2.3.2 in c:\users\y\appdata\local\programs\python\python37\lib\site-packages (from python-docx) (4.5.0)
Building wheels for collected packages: python-docx
  Building wheel for python-docx (setup.py) ... done
  Created wheel for python-docx: filename=python_docx-0.8.10-cp37-none-any.whl size=184496 sha256=7ac76d3eec848a255b4f197d07e7b78ab33598c814d536d9b3c90b5a3e2a57fb
  Stored in directory: C:\Users\Y\AppData\Local\pip\Cache\wheels\05\7d\71\bb534b75918095724d0342119154c3d0fc035cedfe2f6c9a6c
Successfully built python-docx
Installing collected packages: python-docx
Successfully installed python-docx-0.8.10
```
如果提示以上信息则安装成功。
<a name="AeWWe"></a>
## 写入 Word
平时在操作 Word 写文档的时候，一般分为几部分：`标题`、`章节`、`段落`、`图片`、`表格`、`引用`以及`项目符号编号`等。下面就按这几部分如何用 Python 操作来一一介绍。
<a name="zhsbN"></a>
### 标题
文档标题创建比较简单，通过 `Document()` 创建出一个空白文档，只要调用 `add_heading` 方法就能创建标题。
```python
# 导入库
from docx import Document
from docx.shared import Pt
from docx.shared import Inches
from docx.oxml.ns import qn
# 新建空白文档
doc1 = Document()
# 新增文档标题
doc1.add_heading('如何使用 Python 创建和操作 Word',0)
# 保存文件
doc1.save('word1.docx')
```
这样就完成了创建文档和文章标题的操作，下面运行程序，会生成名为 word1.docx 的文档。
<a name="RZNfA"></a>
### 章节与段落
有了文章标题，来看章节和段落是怎么操作的，在上面代码后面增加章节和段落操作的代码如下：
```python
# 导入库
from docx import Document
from docx.shared import Pt
from docx.shared import Inches
from docx.oxml.ns import qn
# 新建空白文档
doc1 = Document()
# 新增文档标题
doc1.add_heading('如何使用 Python 创建和操作 Word',0)
# 创建段落描述
doc1.add_paragraph(' Word 文档在现在的生活和工作中都用的比较多，平时都使用 wps 或者 office 来对 Word 进行处理，可能没想过它可以用 Python 生成，下面就介绍具体如何操作……')
# 创建一级标题
doc1.add_heading('安装 python-docx 库',1)
# 创建段落描述
doc1.add_paragraph('现在开始来介绍如何安装 python-docx 库，具体需要以下两步操作：')
# 创建二级标题
doc1.add_heading('第一步：安装 Python',2)
# 创建段落描述
doc1.add_paragraph('在python官网下载python安装包进行安装。')
# 创建三级标题
doc1.add_heading('第二步：安装 python-docx 库',3)
# 创建段落描述
doc1.add_paragraph('window下win+R输入CMD打开命令行，输入pip install python-docx即可下载。')
# 保存文件
doc1.save('word2.docx')
```
 `add_heading` 方法用来增加文章标题，不过通过上面代码能知道，这个方法的**第二个参数为数字**，其实这个就是用**来标示几级标题的**，在平时就用来标示章节。add_paragraph 方法则是用来在文章中增加段落的。
<a name="LuU6N"></a>
### 字体和引用
通过 `add_paragraph` 方法增加了三个段落，现在对段落中字体操作，以及引用段落的操作。继续修改以上代码，**增加对文章字体字号、加粗、倾斜等操作**，具体代码如下：
```python
# 导入库
from docx import Document
from docx.shared import Pt
from docx.shared import Inches
from docx.oxml.ns import qn
from docx.shared import RGBColor
# 新建空白文档
doc1 = Document()
# 新增文档标题
doc1.add_heading('如何使用 Python 创建和操作 Word',0)
# 创建段落描述
doc1.add_paragraph(' Word 文档在现在的生活和工作中都用的比较多，平时都使用 wps 或者 office 来对 Word 进行处理，可能没想过它可以用 Python 生成，下面介绍具体如何操作……')
# 创建一级标题
doc1.add_heading('安装 python-docx 库',1)
# 创建段落描述
doc1.add_paragraph('现在开始来介绍如何安装 python-docx 库，具体需要以下两步操作：')
# 创建二级标题
doc1.add_heading('第一步：安装 Python',2)
# 创建段落描述
doc1.add_paragraph('在python官网下载python安装包进行安装。')
# 创建三级标题
doc1.add_heading('第二步：安装 python-docx 库',3)
# 创建段落描述
doc1.add_paragraph('window下win+R输入CMD打开命令行，输入pip install python-docx即可下载。')
# 创建段落，添加文档内容
paragraph = doc1.add_paragraph('这是第二步的安装描述！')
# 段落中增加文字，并设置字体字号
run = paragraph.add_run('(注意：这里设置了字号为20)')
run.font.size = Pt(20)
# 设置英文字体
run = doc1.add_paragraph('这里设置英文字体：').add_run('This Font is Times New Roman ')
run.font.name = 'Times New Roman'
# 设置中文字体
run = doc1.add_paragraph('这里设置中文字体：').add_run('当前字体为黑体')
run.font.name='黑体'
r = run._element
r.rPr.rFonts.set(qn('w:eastAsia'), '黑体')
# 设置斜体
run = doc1.add_paragraph('这段设置：').add_run('文字的是斜体 ')
run.italic = True
# 设置粗体
run = doc1.add_paragraph('这段再设置：').add_run('这里设置粗体').bold = True
# 设置字体带下划线
run = doc1.add_paragraph('这段为下划线：').add_run('这里设置带下划线').underline = True
# 设置字体颜色
run = doc1.add_paragraph('这段字体为红色：').add_run('这里设置字体为红色')
run.font.color.rgb = RGBColor(0xFF, 0x00, 0x00)
# 增加引用
doc1.add_paragraph('这里是引用的一段话：用Python改变人生，改变世界，FIGHTING。', style='Intense Quote')
# 保存文件
doc1.save('word2.docx')
```
上面代码主要是针对段落字体的各种设置，每段代码都标有注释应该比较容易理解。
<a name="JsdM7"></a>
### 项目列表
平时在使用 Word 时，为了能展示更清晰，会用到项目符号和编号，将内容通过列表的方式展示出来，下面新建一个文件 word1.py 并编写如下代码：
```python
# 导入库
from docx import Document
from docx.shared import Pt
from docx.shared import Inches
from docx.oxml.ns import qn
# 新建文档
doc2 = Document()
doc2.add_paragraph('哪个不是动物：')
# 增加无序列表
doc2.add_paragraph(
    '苹果', style='List Bullet'
)
doc2.add_paragraph(
    '喜洋洋', style='List Bullet'
)
doc2.add_paragraph(
    '懒洋洋', style='List Bullet'
)
doc2.add_paragraph(
    '沸洋洋', style='List Bullet'
)
doc2.add_paragraph(
    '灰太狼', style='List Bullet'
)
doc2.add_paragraph('2020年度计划：')
# 增加有序列表
doc2.add_paragraph(
    'CSDN达到博客专家', style='List Number'
)
doc2.add_paragraph(
    '每周健身三天', style='List Number'
)
doc2.add_paragraph(
    '每天学习一个新知识点', style='List Number'
)
doc2.add_paragraph(
    '学习50本书', style='List Number'
)
doc2.add_paragraph(
    '减少加班时间', style='List Number'
)
# 保存文件
doc2.save('word1.docx')
```
<a name="0LKUT"></a>
### 图片和表格
平时编辑文章时，插入图片和表格也是经常使用到的，用 Python 该如何操作插入图片和表格？首先找个图片，这用了 Python的logo 标志图，文件名为 python-logo.png，利用`add_picture`添加图片；利用`add_table`添加表格，然后在 word1.py 文件中增加如下代码：
```python
# 导入库
from docx import Document
from docx.shared import Pt
from docx.shared import Inches
from docx.oxml.ns import qn
# 新建文档
doc2 = Document()
doc2.add_paragraph('哪个不是动物：')
# 增加无序列表
doc2.add_paragraph(
    '苹果', style='List Bullet'
)
doc2.add_paragraph(
    '喜洋洋', style='List Bullet'
)
doc2.add_paragraph(
    '懒洋洋', style='List Bullet'
)
doc2.add_paragraph(
    '沸洋洋', style='List Bullet'
)
doc2.add_paragraph(
    '灰太狼', style='List Bullet'
)
doc2.add_paragraph('2020年度计划：')
# 增加有序列表
doc2.add_paragraph(
    'CSDN达到博客专家', style='List Number'
)
doc2.add_paragraph(
    '每周健身三天', style='List Number'
)
doc2.add_paragraph(
    '每天学习一个新知识点', style='List Number'
)
doc2.add_paragraph(
    '学习50本书', style='List Number'
)
doc2.add_paragraph(
    '减少加班时间', style='List Number'
)
doc2.add_heading('图片',2)
# 增加图像
doc2.add_picture('C:/Users/Y/Pictures/python-logo.png', width=Inches(5.5))
doc2.add_heading('表格',2)
# 增加表格，这是表格头
table = doc2.add_table(rows=1, cols=4)
hdr_cells = table.rows[0].cells
hdr_cells[0].text = '编号'
hdr_cells[1].text = '姓名'
hdr_cells[2].text = '职业'
# 这是表格数据
records = (
    (1, '张三', '电工'),
    (2, '张五', '老板'),
    (3, '马六', 'IT'),
    (4, '李四', '工程师')
)
# 遍历数据并展示
for id, name, work in records:
    row_cells = table.add_row().cells
    row_cells[0].text = str(id)
    row_cells[1].text = name
    row_cells[2].text = work
# 手动增加分页
doc2.add_page_break()
# 保存文件
doc2.save('word1.docx')
```
<a name="ycerZ"></a>
## 读取 Word 文件
上面写了很多用 Python 创建空白 Word 文件格式化字体并保存到文件中，接下来再简单看下如何读取已有的 Word 文件，看如下代码：
```python
# 引入库
from docx import Document
# 打开文档1
doc1 = Document('word1.docx')
# 读取每段内容
pl = [ paragraph.text for paragraph in doc1.paragraphs]
print('###### 输出word1文章的内容 ######')
# 输出读取到的内容
for i in pl:
    print(i)
# 打开文档2
doc2 = Document('word2.docx')
print('\n###### 输出word2文章内容 ######')
pl2 = [ paragraph.text for paragraph in doc2.paragraphs]
# 输出读取到的内容
for j in pl2:
    print(j)
# 读取表格材料，并输出结果
tables = [table for table in doc.tables]
for table in tables:
    for row in table.rows:
        for cell in row.cells:
            print(cell.text, end='  ')
        print()
    print('\n')
```
以上代码是将之前输出的两个文档内容都读取出来，这里只是打印到控制台，并没有做其他处理。
