Python 词云图<br />词云是一种数据可视化技术，用于表示文本数据，其中每个单词的大小表示其出现的频率或重要性。可以使用词云突出显示重要的文本数据点。词云被广泛用于分析来自社交网络网站的数据。为了在Python中生成词云，需要的模块是– matplotlib，pandas和wordcloud。要安装这些软件包，请运行以下命令：
```bash
pip install matplotlib
pip install pandas
pip install wordcloud
```
<a name="9x2YK"></a>
### 1、字数
可以设置要在tagcloud上显示的最大单词数。为此，请使用WordCloud()函数的max_words关键字参数。
```python
# importing the necessery modules
from wordcloud import WordCloud
import matplotlib.pyplot as plt
import csv
# file object is created
file_ob = open(r"linuxmi.com.csv")
# reader object is created
reader_ob = csv.reader(file_ob)
# contents of reader object is stored .
# data is stored in list of list  format.
reader_contents = list(reader_ob)
# empty string is declare
text = ""
# iterating through list of rows
for row in reader_contents :
    #  iterating through words in the row
    for word in row :
        # concatenate the words
        text = text + " " + word
# show only 50 words in the wordcloud .
wordcloud = WordCloud(width=480,  height=480, max_words=50).generate(text)
# plot the WordCloud image
plt.figure()
plt.imshow(wordcloud,  interpolation="bilinear")
plt.axis("off")
plt.margins(x=0, y=0)
plt.show()
```
<a name="66Jv6"></a>
### 2、删除一些单词
可以删除一些不想显示的词。为此，将这些单词传递给WordCloud()函数的停用词列表参数。
```python
# importing the necessery modules
from wordcloud import WordCloud
import matplotlib.pyplot as plt
import csv
# file object is created
file_ob = open(r"linuxmi.com.csv")
# reader object is created
reader_ob = csv.reader(file_ob)
# contents of reader object is stored .
# data is stored in list of list  format.
reader_contents = list(reader_ob)
# empty string is declare
text = ""
# iterating through list of rows
for row in reader_contents :
    #  iterating through words in the row
    for word in row :
        # concatenate the words
        text = text + " " + word
# remove Python , Matplotlib , Color Words from WordCloud .
wordcloud =  WordCloud(width=480, height=480,
                       stopwords=["Python",  "Matplotlib","Color"]).generate(text)
# plot the WordCloud image
plt.figure()
plt.imshow(wordcloud,  interpolation="bilinear")
plt.axis("off")
plt.margins(x=0, y=0)
plt.show()
```
<a name="7qNDK"></a>
### 3、更改背景
可以更改wordcloud背景的颜色。为此，请使用WordCloud()函数的background_color关键字参数。
```python
# importing the necessery modules
from wordcloud import WordCloud
import matplotlib.pyplot as plt
import csv
# file object is created
file_ob = open(r"linuxmi.com.csv")
# reader object is created
reader_ob = csv.reader(file_ob)
# contents of reader object is stored .
# data is stored in list of list  format.
reader_contents = list(reader_ob)
# empty string is declare
text = ""
# iterating through list of rows
for row in reader_contents :
    #  iterating through words in the row
    for word in row :
        # concatenate the words
        text = text + " " + word
wordcloud = WordCloud(width=480, height=480,  background_color="pink").generate(text)
# plot the WordCloud image
plt.figure()
plt.imshow(wordcloud,  interpolation="bilinear")
plt.axis("off")
plt.margins(x=0, y=0)
plt.show()
```
<a name="9ltHu"></a>
### 4、更改单词的颜色
可以使用WordCloud()函数的colormap关键字参数来更改单词的颜色。
```python
# importing the necessery modules
from wordcloud import WordCloud
import matplotlib.pyplot as plt
import csv
# file object is created
file_ob = open(r"linuxmi.com.csv")
# reader object is created
reader_ob = csv.reader(file_ob)
# contents of reader object is stored .
# data is stored in list of list  format.
reader_contents = list(reader_ob)
# empty string is declare
text = ""
# iterating through list of rows
for row in reader_contents :
    #  iterating through words in the row
    for word in row :
        # concatenate the words
        text = text + " " + word
wordcloud = WordCloud(width=480, height=480,  colormap="Oranges_r").generate(text)
# plot the WordCloud image
plt.figure()
plt.imshow(wordcloud,  interpolation="bilinear")
plt.axis("off")
plt.margins(x=0, y=0)
plt.show()
```
<a name="hmyT5"></a>
### 5、设置最大和最小字体
可以控制wordcloud的最小和最大字体大小。为此，请使用WordCloud()函数的max_font_size和min_font_size关键字参数。
```python
# importing the necessery modules
from wordcloud import WordCloud
import matplotlib.pyplot as plt
import csv
# file object is created
file_ob = open(r"linuxmi.com.csv")
# reader object is created
reader_ob = csv.reader(file_ob)
# contents of reader object is stored .
# data is stored in list of list  format.
reader_contents = list(reader_ob)
# empty string is declare
text = ""
# iterating through list of rows
for row in reader_contents :
    #  iterating through words in the row
    for word in row :
        # concatenate the words
        text = text + " " + word
wordcloud = WordCloud(width=480, height=480, max_font_size=20,  min_font_size=10).generate(text)
plt.figure()
plt.imshow(wordcloud,  interpolation="bilinear")
plt.axis("off")
plt.margins(x=0, y=0)
plt.show()
```

