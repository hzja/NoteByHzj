Python<br />文本是一种非结构化数据。根据Wikipedia，非结构化数据被描述为“没有预定义的数据模型或没有以预定义的方式组织的信息。<br />不幸的是，计算机不像人类;机器无法像人类那样阅读原始文本。当处理文本数据时，不能直接从原始文本转换到机器学习模型。相反，必须遵循这样一个过程：首先清理文本，然后将其编码为机器可读的格式。<br />来介绍一些清理文本的方法。
<a name="bZtJU"></a>
#### 标准化
当写作时，会因为不同的原因将句子/段落中的不同单词大写。例如，开始一个新句子用大写字母，或者如果某物是名词，会用大写第一个字母来表示正在谈论的是一个地点/人，等等。<br />人类，可以阅读文本和直观地看出，“The”这是一个句子的开头，而同一个词“the”，这是句子后面的单词。但是，电脑不能——“The”和“the”被视为两个不同的词。<br />因此，标准化单词的大小写是很重要的，这样每个单词都是相同的大小写，而计算机不会将同一个单词处理为两个不同的单词。
```python
# Python例子
text = "The UK lockdown restrictions will be dropped in the summer so we can go partying again!" 

# 小写化文本
text = text.lower()
print(text)

>>>> the uk lockdown restrictions will be dropped in the summer so we can go partying again!
```
<a name="sE5ky"></a>
#### 删除停用词
在大多数自然语言任务中，希望机器学习模型识别文档中为文档提供价值的单词。例如，在一个情感分析任务中，希望找到指向文本情感的一个(或多个)词。<br />英语(我相信同样也适用于大多数语言)，有些词比其他的词更频繁地使用，但他们不一定能创造更多的价值，因此可以肯定地说，可以忽略它们，在文本中删除。<br />注意：移除停用词并不总是最好的主意!
```python
# 导入库
import nltk
from nltk.corpus import stopwords
nltk.download("stopwords")

stop_words = set(stopwords.words("english"))
print(stop_words)

>>>> {'over', 'is', 'than', 'can', 'these', "isn't", 'so', 'my', 'each', 'an', 'between', 'through', 'up', 'where', 'hadn', 'very', "you'll", 'while', "weren't", 'too', 'doesn', 'only', 'needn', 'has', 'just', 'd', 'some', 'into', 've', 'didn', 'further', 'why', 'mightn', 'and', 'haven', 'own', "mightn't", 'during', 'both', 'me', 'shan', "doesn't", 'theirs', 'herself', 'the', 'few', 'our', 'its', 'yourself', 'under', 'at', "you've", 're', 'themselves', 'y', 'ma', 'because', 'him', 'above', 'such', 'we', "wouldn't", 'of', 'from', 'hers', 'nor', "shouldn't", 'a', 'hasn', 'them', 'myself', 'this', 'being', 'your', 'those', 'i', 'if', 'couldn', 'not', 'will', 'it', 'm', 'to', 'isn', 'aren', 'when', 'o', 'about', 'their', 'more', 'been', "needn't", 'had', 'll', 'most', 'against', 'once', 'how', "didn't", "shan't", 'there', 'all', "should've", 'he', "don't", 'she', 'which', 'below', 'on', 'no', 'yourselves', "wasn't", 'shouldn', 'by', 'be', 'have', 'does', "aren't", 'itself', 'same', 'should', 'in', 'before', 'am', "won't", 'having', "you'd", 'mustn', 'for', "that'll", 'that', "couldn't", 'wasn', 'won', "hasn't", 'as', 'until', 'wouldn', "mustn't", 'his', 'ain', "you're", 'out', "she's", 'other', 'are', 't', 'you', 'off', 'yours', 'ourselves', 'himself', 'down', "haven't", 'ours', 'now', "hadn't", 'do', 's', 'her', 'with', "it's", 'then', 'weren', 'any', 'after', 'whom', 'what', 'who', 'but', 'again', 'here', 'did', 'doing', 'were', 'they', 'was', 'or', 'don'}

# 示例文本
text = "The UK lockdown restrictions will be dropped in the summer so we can go partying again!"

# 删除stopwords
text = " ".join([word for word in text.split() if word not in stop_words])

print(text)

>>>> uk lockdown restrictions dropped summer go partying again!
```
<a name="QOqA9"></a>
#### Unicode
表情符号和其他非ASCII字符应该格式化为Unicode。<br />从本质上讲，Unicode是一种通用的字符编码标准，所有语言中的每个字符和符号都被赋予一个代码。Unicode是必需的，因为允许使用各种不同的语言检索或连接数据。<br />注意：示例代码来自Python指南
```python
# 创建unicode字符串
text_unicode = "Python is easy \u200c to learn" 

# 将文本编码为ASCII格式
text_encode = text_unicode.encode(encoding="ascii", errors="ignore")

# 解码文本
text_decode = text_encode.decode()

# 清除文本以删除额外的空白
clean_text = " ".join([word for word in text_decode.split()])
print(clean_text)

>>>> Python is easy to learn.
```
<a name="rSeBY"></a>
#### 删除url，标签，标点符号，提及，等等。
根据处理的数据类型，可能会面临各种各样的挑战。例如，如果在处理来自Twitter的数据，就会发现各种各样的标签和提及——用Twitter的行话来说，这是指一条包含另一个用户名的推文。<br />如果这些特征对试图解决的问题没有价值，那么最好从数据中删除它们。然而，由于在许多实例中不能依赖于已定义的字符，所以可以利用名为Regex的模式匹配工具的强大功能来帮助我们。
```python
import re

# 删除提及
text = "You should get @BlockFiZac from @BlockFi to talk about bitcoin lending, stablecoins, institution adoption, and the future of crypto"

text = re.sub("@\S+", "", text)
print(text)

>>>> You should get  from  to talk about bitcoin lending, stablecoins, institution adoption, and the future of crypto
------------------------------------------------------------------
# 删除记号
text = """#BITCOIN LOVES MARCH 13th A year ago the price of Bitcoin collapsed to $3,800 one of the lowest levels in the last 4 years. Today, exactly one year later it reaches the new all-time high of $60,000 Thank you Bitcoin for always making my birthday exciting"""

text = re.sub("\$", "", text)
print(text)

>>>> #BITCOIN LOVES MARCH 13th A year ago the price of Bitcoin collapsed to  3,800 one of the lowest levels in the last 4 years. Today, exactly one year  later it reaches the new all-time high of 60,000 Thank you Bitcoin for  always making my birthday exciting
------------------------------------------------------------------
# 删除urls
text = "Did someone just say “Feature Engineering”? https://buff.ly/3rRzL0s"

text = re.sub("https?:\/\/.*[\r\n]*", "", text)
print(text)

>>>> Did someone just say “Feature Engineering”?
------------------------------------------------------------------
# 删除hashtags 
text = """.#FreedomofExpression which includes #FreedomToProtest should be the cornerstone of any democracy. I’m looking forward to speaking in the 2 day debate on the #PoliceCrackdownBill & explaining why I will be voting against it."""

text = re.sub("#", "", text)
print(text)

>>>> .FreedomofExpression which includes FreedomToProtest should be the  cornerstone of any democracy. I’m looking forward to speaking in the 2 day  debate on the PoliceCrackdownBill & explaining why I will be voting against it.
------------------------------------------------------------------
# 删除标点符号
import string 

text = "Thank you! Not making sense. Just adding, lots of random punctuation."

punct = set(string.punctuation) 
text = "".join([ch for ch in tweet if ch not in punct])

print(text)

>>>> Thank you Not making sense Just adding lots of random punctuation
```
<a name="dOZKx"></a>
#### 词干提取&词形还原
当在做一个NLP任务时，可能希望电脑能够理解“walked”、“walk”和“walking”都只是同一个单词的不同时态，否则它们就会被区别对待。<br />词干提取和词形还原都是用于NLP中规范化文本的技术——为了进一步简化这个定义，只是将一个单词简化到它的核心词根。<br />根据维基百科的定义：

- 词干提取——在语言形态学和信息检索中，词干提取是将单词缩减为词干、词根或词根的过程。
- 词形还原——引理化在语言学中是指将一个单词的各种形式还原初始形式，这样它们就可以作为一个单词来分析。

尽管这两种技术的定义非常相似，但它们减少单词的方法却截然不同，这意味着这两种技术的结果并不一致。
```python
import nltk
from nltk.stem.porter import PorterStemmer
from nltk.stem import WordNetLemmatizer

words = ["walk", "walking", "walked", "walks", "ran", "run", "running", "runs"]
-----------------------------------------------------------------
# 词干提取
stemmer = PorterStemmer()

for word in words: 
    print(word + " ---> " + stemmer.stem(word))
>>>> walk ---> walk
     walking ---> walk
     walked ---> walk
     walks ---> walk
     ran ---> ran
     run ---> run
     running ---> run
     runs ---> run
------------------------------------------------------------------
# 词形还原
lemmatizer = WordNetLemmatizer()

for word in words:
    print(word + " ---> " + lemmatizer.lemmatize(word))

>>>> walk ---> walk 
     walking ---> walking 
     walked ---> walked 
     walks ---> walk 
     ran ---> ran 
     run ---> run 
     running ---> running 
     runs ---> run
```
