# 原创
：  [Python人工智能] 二十三.基于机器学习和TFIDF的情感分类（含详细的NLP数据清洗）

# [Python人工智能] 二十三.基于机器学习和TFIDF的情感分类（含详细的NLP数据清洗）

从本专栏开始，作者正式研究Python深度学习、神经网络及人工智能相关知识。前一篇文章分享了自定义情感词典（大连理工词典）实现情感分析和情绪分类的过程。这篇文章将详细讲解自然语言处理过程，基于机器学习和TFIDF的情感分类算法，并进行了各种分类算法（SVM、RF、LR、Boosting）对比。这篇文章主要结合作者的书籍《Python网络数据爬取及分析从入门到精通（分析篇）》进行讲解，再次带领大家好好看看Python中文文本分析的基本步骤。个人感觉还不错，基础性文章，希望对您有所帮助~

本专栏主要结合作者之前的博客、AI经验和相关视频及论文介绍，后面随着深入会讲解更多的Python人工智能案例及应用。基础性文章，希望对您有所帮助，如果文章中存在错误或不足之处，还请海涵！作者作为人工智能的菜鸟，希望大家能与我在这一笔一划的博客中成长起来。写了这么多年博客，尝试第一个付费专栏，为小宝赚点奶粉钱，但更多博客尤其基础性文章，还是会继续免费分享，该专栏也会用心撰写，望对得起读者。如果有问题随时私聊我，只望您能从这个系列中学到知识，一起加油喔~

TF下载地址：[https://github.com/eastmountyxz/AI-for-TensorFlow](https://github.com/eastmountyxz/AI-for-TensorFlow)<br/> Keras下载地址：[https://github.com/eastmountyxz/AI-for-Keras](https://github.com/eastmountyxz/AI-for-Keras)<br/> 情感分析地址：[https://github.com/eastmountyxz/Sentiment-Analysis](https://github.com/eastmountyxz/Sentiment-Analysis)

#### 文章目录

同时推荐前面作者另外五个Python系列文章。从2014年开始，作者主要写了三个Python系列文章，分别是基础知识、网络爬虫和数据分析。2018年陆续增加了Python图像识别和Python人工智能专栏。

前文：<br/> [[Python人工智能] 一.TensorFlow2.0环境搭建及神经网络入门](https://blog.csdn.net/Eastmount/article/details/103282042)<br/> [[Python人工智能] 二.TensorFlow基础及一元直线预测案例](https://blog.csdn.net/Eastmount/article/details/103322331)<br/> [[Python人工智能] 三.TensorFlow基础之Session、变量、传入值和激励函数](https://blog.csdn.net/Eastmount/article/details/103336214)<br/> [[Python人工智能] 四.TensorFlow创建回归神经网络及Optimizer优化器](https://blog.csdn.net/Eastmount/article/details/103410289)<br/> [[Python人工智能] 五.Tensorboard可视化基本用法及绘制整个神经网络](https://blog.csdn.net/Eastmount/article/details/103569858)<br/> [[Python人工智能] 六.TensorFlow实现分类学习及MNIST手写体识别案例](https://blog.csdn.net/Eastmount/article/details/103586271)<br/> [[Python人工智能] 七.什么是过拟合及dropout解决神经网络中的过拟合问题](https://blog.csdn.net/Eastmount/article/details/103595096)<br/> [[Python人工智能] 八.卷积神经网络CNN原理详解及TensorFlow编写CNN](https://blog.csdn.net/Eastmount/article/details/103620235)<br/> [[Python人工智能] 九.gensim词向量Word2Vec安装及《庆余年》中文短文本相似度计算](https://blog.csdn.net/Eastmount/article/details/103647573)<br/> [[Python人工智能] 十.Tensorflow+Opencv实现CNN自定义图像分类案例及与机器学习KNN图像分类算法对比](https://blog.csdn.net/Eastmount/article/details/103757386)<br/> [[Python人工智能] 十一.Tensorflow如何保存神经网络参数](https://blog.csdn.net/Eastmount/article/details/103810291)<br/> [[Python人工智能] 十二.循环神经网络RNN和LSTM原理详解及TensorFlow编写RNN分类案例](https://blog.csdn.net/Eastmount/article/details/103811752)<br/> [[Python人工智能] 十三.如何评价神经网络、loss曲线图绘制、图像分类案例的F值计算](https://blog.csdn.net/Eastmount/article/details/103847406)<br/> [[Python人工智能] 十四.循环神经网络LSTM RNN回归案例之sin曲线预测](https://blog.csdn.net/Eastmount/article/details/103914851)<br/> [[Python人工智能] 十五.无监督学习Autoencoder原理及聚类可视化案例详解](https://blog.csdn.net/Eastmount/article/details/103990297)<br/> [[Python人工智能] 十六.Keras环境搭建、入门基础及回归神经网络案例](https://blog.csdn.net/Eastmount/article/details/104313140)<br/> [[Python人工智能] 十七.Keras搭建分类神经网络及MNIST数字图像案例分析](https://blog.csdn.net/Eastmount/article/details/104366166)<br/> [[Python人工智能] 十八.Keras搭建卷积神经网络及CNN原理详解](https://blog.csdn.net/Eastmount/article/details/104399015)<br/> [[Python人工智能] 十九.Keras搭建循环神经网络分类案例及RNN原理详解](https://blog.csdn.net/Eastmount/article/details/104458677)<br/> [[Python人工智能] 二十.基于Keras+RNN的文本分类vs基于传统机器学习的文本分类](https://blog.csdn.net/Eastmount/article/details/105165164)<br/> [[Python人工智能] 二十一.Word2Vec+CNN中文文本分类详解及与机器学习（RF\DTC\SVM\KNN\NB\LR）分类对比](https://blog.csdn.net/Eastmount/article/details/107004660)<br/> [[Python人工智能] 二十二.基于大连理工情感词典的情感分析和情绪计算](https://blog.csdn.net/Eastmount/article/details/107877713)<br/> [《人工智能狂潮》读后感——什么是人工智能？(一)](https://blog.csdn.net/Eastmount/article/details/104161047)

---


在数据分析和数据挖掘中，通常需要经历前期准备、数据爬取、数据预处理、数据分析、数据可视化、评估分析等步骤，而数据分析之前的工作几乎要花费数据工程师近一半的工作时间，其中的数据预处理也将直接影响后续模型分析的好坏。图是数据预处理的基本步骤，包括中文分词、词性标注、数据清洗、特征提取（向量空间模型存储）、权重计算（TF-IDF）等。

## 一.中文分词

当读者使用Python爬取了中文数据集之后，首先需要对数据集进行中文分词处理。由于英文中的词与词之间是采用空格关联的，按照空格可以直接划分词组，所以不需要进行分词处理，而中文汉字之间是紧密相连的，并且存在语义，词与词之间没有明显的分隔点，所以需要借助中文分词技术将语料中的句子按空格分割，变成一段段词序列。下面开始详细介绍中文分词技术及Jiaba中文分词工具。

中文分词（Chinese Word Segmentation）指将汉字序列切分成一个个单独的词或词串序列，它能够在没有词边界的中文字符串中建立分隔标志，通常采用空格分隔。下面举个简单示例，对句子“我是程序员”进行分词操作。

```
输入：我是程序员
输出1：我\是\程\序\员
输出2：我是\是程\程序\序员
输出3：我\是\程序员

```

简单举个例子，代码中主要导入Jieba扩展包，然后调用其函数进行中文分词。

```
#encoding=utf-8  
import jieba  
  
text = "北京理工大学生前来应聘"  

data = jieba.cut(text,cut_all=True)   #全模式
print("[全模式]: ", " ".join(data))
  
data = jieba.cut(text,cut_all=False)  #精确模式  
print("[精确模式]: ", " ".join(data))
   
data = jieba.cut(text)  #默认是精确模式 
print("[默认模式]: ", " ".join(data))

data = jieba.cut_for_search(text)  #搜索引擎模式   
print("[搜索引擎模式]: ", " ".join(data))


```

上述代码输出如下，包括全模式、精确模式和搜索引擎模式输出的结果。

---


## 二.数据清洗

在分析语料的过程中，通常会存在一些脏数据或噪声词组干扰我们的实验结果，这就需要对分词后的语料进行数据清洗（Data Cleaning）。比如前面使用Jieba工具进行中文分词，它可能存在一些脏数据或停用词，如“我们”、“的”、“吗”等。这些词降低了数据质量，为了得到更好的分析结果，需要对数据集进行数据清洗或停用词过滤等操作。

这里主要讲解停用词过滤，将这些出现频率高却不影响文本主题的停用词删除。在Jieb分词过程中引入stop_words.txt停用词词典，如果存在则过滤即可。

下面是从大众点评、美团之类的网站抓取“黄果树瀑布”的评论信息，我们通过Jieba工具对其进行中文分词。

**完整代码：**

```
# -*- coding:utf-8 -*-
import csv
import pandas as pd
import numpy as np
import jieba
import jieba.analyse

#添加自定义词典和停用词典
jieba.load_userdict("user_dict.txt")
stop_list = pd.read_csv('stop_words.txt',
                        engine='python',
                        encoding='utf-8',
                        delimiter="\n",
                        names=['t'])['t'].tolist()

#中文分词函数
def txt_cut(juzi):
    return [w for w in jieba.lcut(juzi) if w not in stop_list]

#写入分词结果
fw = open('fenci_data.csv', "a+", newline = '',encoding = 'gb18030')
writer = csv.writer(fw)  
writer.writerow(['content','label'])

# 使用csv.DictReader读取文件中的信息
labels = []
contents = []
file = "data.csv"
with open(file, "r", encoding="UTF-8") as f:
    reader = csv.DictReader(f)
    for row in reader:
        # 数据元素获取
        if row['label'] == '好评':
            res = 0
        else:
            res = 1
        labels.append(res)
        content = row['content']
        seglist = txt_cut(content)
        output = ' '.join(list(seglist))            #空格拼接
        contents.append(output)
        
        #文件写入
        tlist = []
        tlist.append(output)
        tlist.append(res)
        writer.writerow(tlist)

print(labels[:5])
print(contents[:5])
fw.close()

```

运行结果如下图所示，一方面它将特殊标点符号、停用词过滤，另一方面导入了user_dict.txt词典，将“黄果树瀑布”、“风景区”等专有名词分词，否则它可能会划分为“黄果树”和“瀑布”、“风景”和“区”。

> 
还记得小时候，常常守在电视机前，等候《西游记》的播出。“你挑着担,我牵着马。翻山涉水两肩双滑……"熟悉的歌曲，又在耳边响起时。 这歌词中的水，就有贵州的水，准确的说，是贵州的黄果树瀑布；那一帘瀑布，流进了我们的童年，让我们流连忘返。 黄果树瀑布并不是只有一个瀑布，而是一个大景区，包括陡坡塘瀑布、天星桥景区、黄果树大瀑布，其中黄果树大瀑布是最有名的。


> 
记得 小时候 守 电视机 前 等候 西游记 播出 挑 担 牵 马 翻山 涉水 两肩 双滑 熟悉 歌曲 耳边 响起 时 歌词 中 水 贵州 水 准确 说 贵州 黄果树瀑布 那一帘 瀑布 流进 童年 流连忘返 黄果树瀑布 瀑布 景区 包括 陡坡 塘 瀑布 天星桥 景区 黄果树 瀑布 黄果树 瀑布 有名


---


## 三.特征提取及TF-IDF计算

### 1.基本概念

权重计算是指通过特征权重来衡量特征项在文档表示中的重要程度，给特征词赋予一定的权重来衡量统计文本特征词。TF-IDF（Term Frequency-Invers Document Frequency）是近年来用于数据分析和信息处理经典的权重计算技术。该技术根据特征词在文本中出现的次数和在整个语料中出现的文档频率来计算该特征词在整个语料中的重要程度，其优点是能过滤掉一些常见却无关紧要的词语，尽可能多的保留影响程度高的特征词。

TF-IDF的计算公式如下，式中TF-IDF表示词频TF和倒文本词频IDF的乘积，TF-IDF中权重与特征项在文档中出现的频率成正比，与在整个语料中出现该特征项的文档数成反比。TF-IDF值越大则该特征词对这个文本的重要程度越高。

其中，TF词频的计算公式如下，n<sub>i,j</sub> 为特征词 t<sub>i</sub> 在训练文本 D<sub>j</sub> 中出现的次数，分母是文本 D<sub>j</sub> 中所有特征词的个数，计算的结果即为某个特征词的词频。

倒文档频率（Inverse Document Frequency，简称IDF）是Spark Jones在1972年提出的，用于计算词与文献相关权重的经典方法。计算公式如下，参数|D|表示语料的文本总数，|D<sub>t</sub>| 表示文本所包含特征词 t<sub>j</sub> 的数量。

在倒文档频率方法中，权重是随着特征词的文档数量的变化呈反向变化。如某些常用词“我们”、“但是”、“的”等，在所有文档中出现频率很高，但它的IDF值却非常低。甚至如果它每篇文档都出现，则log1的计算结果为0，从而降低了这些常用词的作用；相反，如果某篇介绍“人工智能”的词，仅仅在该篇文档中出现很多次，它的作用就非常高。

TF-IDF技术的核心思想是如果某个特征词在一篇文章中出现的频率TF高，并且在其他文章中很少出现，则认为此词或者短语具有很好的类别区分能力，适合用来做权重计算。TF-IDF算法简单快速，结果也符合实际情况，是文本挖掘、情感分析、主题分布等领域的常用手段。

### 2.代码实现

Scikit-Learn中主要使用Scikit-Learn中的两个类CountVectorizer和TfidfTransformer，用来计算词频和TF-IDF值。

**完整代码：**

```
# -*- coding:utf-8 -*-
import csv
import pandas as pd
import numpy as np
import jieba
import jieba.analyse
from scipy.sparse import coo_matrix
from sklearn import feature_extraction  
from sklearn.feature_extraction.text import TfidfVectorizer
from sklearn.feature_extraction.text import CountVectorizer
from sklearn.feature_extraction.text import TfidfTransformer

#----------------------------------第一步 读取文件--------------------------------
with open('fenci_data.csv', 'r', encoding='UTF-8') as f:
    reader = csv.DictReader(f)
    labels = []
    contents = []
    for row in reader:
        labels.append(row['label']) #0-好评 1-差评
        contents.append(row['content'])

print(labels[:5])
print(contents[:5])

#----------------------------------第二步 数据预处理--------------------------------
#将文本中的词语转换为词频矩阵 矩阵元素a[i][j] 表示j词在i类文本下的词频
vectorizer = CountVectorizer()

#该类会统计每个词语的tf-idf权值
transformer = TfidfTransformer()

#第一个fit_transform是计算tf-idf 第二个fit_transform是将文本转为词频矩阵
tfidf = transformer.fit_transform(vectorizer.fit_transform(contents))
for n in tfidf[:5]:
    print(n)
print(type(tfidf))

# 获取词袋模型中的所有词语  
word = vectorizer.get_feature_names()
for n in word[:10]:
    print(n)
print("单词数量:", len(word))

#将tf-idf矩阵抽取出来，元素w[i][j]表示j词在i类文本中的tf-idf权重
#X = tfidf.toarray()
X = coo_matrix(tfidf, dtype=np.float32).toarray() #稀疏矩阵 注意float
print(X.shape)
print(X[:10])


```

输出结果如下所示：

```
&lt;class 'scipy.sparse.csr.csr_matrix'&gt;
aaaaa
achievements
amazing
ananananan
ancient
anshun
aperture
app

单词数量: 20254
(6074, 20254)
[[0. 0. 0. ... 0. 0. 0.]
 [0. 0. 0. ... 0. 0. 0.]
 [0. 0. 0. ... 0. 0. 0.]
 ...
 [0. 0. 0. ... 0. 0. 0.]
 [0. 0. 0. ... 0. 0. 0.]
 [0. 0. 0. ... 0. 0. 0.]]

```

### 3.MemoryError内存溢出错误

当我们数据量很大时，矩阵往往存储不了这么大的数据，会出现如下错误：

我提供的解决方法如下：

---


## 四.基于逻辑回归的情感分类

获取文本TF-IDF值之后，本小节简单讲解使用TF-IDF值进行情感分类的过程，主要包括如下步骤：

**逻辑回归完整代码：**

```
# -*- coding:utf-8 -*-
import csv
import pandas as pd
import numpy as np
import jieba
import jieba.analyse
from scipy.sparse import coo_matrix
from sklearn import feature_extraction  
from sklearn.feature_extraction.text import TfidfVectorizer
from sklearn.feature_extraction.text import CountVectorizer
from sklearn.feature_extraction.text import TfidfTransformer
from sklearn.model_selection import train_test_split
from sklearn.metrics import classification_report
from sklearn.linear_model import LogisticRegression
from sklearn.ensemble import RandomForestClassifier
from sklearn.tree import DecisionTreeClassifier
from sklearn import svm
from sklearn import neighbors
from sklearn.naive_bayes import MultinomialNB

#----------------------------------第一步 读取文件--------------------------------
with open('fenci_data.csv', 'r', encoding='UTF-8') as f:
    reader = csv.DictReader(f)
    labels = []
    contents = []
    for row in reader:
        labels.append(row['label']) #0-好评 1-差评
        contents.append(row['content'])

print(labels[:5])
print(contents[:5])

#----------------------------------第二步 数据预处理--------------------------------
#将文本中的词语转换为词频矩阵 矩阵元素a[i][j] 表示j词在i类文本下的词频
vectorizer = CountVectorizer(min_df=5)

#该类会统计每个词语的tf-idf权值
transformer = TfidfTransformer()

#第一个fit_transform是计算tf-idf 第二个fit_transform是将文本转为词频矩阵
tfidf = transformer.fit_transform(vectorizer.fit_transform(contents))
for n in tfidf[:5]:
    print(n)
print(type(tfidf))

# 获取词袋模型中的所有词语  
word = vectorizer.get_feature_names()
for n in word[:10]:
    print(n)
print("单词数量:", len(word))

#将tf-idf矩阵抽取出来，元素w[i][j]表示j词在i类文本中的tf-idf权重
#X = tfidf.toarray()
X = coo_matrix(tfidf, dtype=np.float32).toarray() #稀疏矩阵 注意float
print(X.shape)
print(X[:10])

#----------------------------------第三步 数据划分--------------------------------
#使用 train_test_split 分割 X y 列表
X_train, X_test, y_train, y_test = train_test_split(X, 
                                                    labels, 
                                                    test_size=0.3, 
                                                    random_state=1)

#--------------------------------第四步 机器学习分类--------------------------------
# 逻辑回归分类方法模型
LR = LogisticRegression(solver='liblinear')
LR.fit(X_train, y_train)
print('模型的准确度:{}'.format(LR.score(X_test, y_test)))
pre = LR.predict(X_test)
print("逻辑回归分类")
print(len(pre), len(y_test))
print(classification_report(y_test, pre))
print("\n")

```

运行结果如下图所示：

---


## 五.算法性能评估

算法评价很多实时需要我们自己编写程序去实现，比如绘制ROC曲线、统计各种特征信息、显示4位数结果。这里作者尝试自定义准确率（Precision）、召回率（Recall）和F特征值（F-measure），其计算公式如下：

 
      
       
        
        
          P 
         
        
          r 
         
        
          e 
         
        
          c 
         
        
          i 
         
        
          s 
         
        
          i 
         
        
          o 
         
        
          n 
         
        
          = 
         
         
          
          
            正 
           
          
            确 
           
          
            被 
           
          
            预 
           
          
            测 
           
          
            的 
           
          
            总 
           
          
            数 
           
          
          
          
            预 
           
          
            测 
           
          
            出 
           
          
            的 
           
          
            分 
           
          
            类 
           
          
            总 
           
          
            数 
           
          
         
        
       
         Precision = \frac{正确被预测的总数}{预测出的分类总数} 
        
       
     Precision=预测出的分类总数正确被预测的总数​

 
      
       
        
        
          R 
         
        
          e 
         
        
          c 
         
        
          a 
         
        
          l 
         
        
          l 
         
        
          = 
         
         
          
          
            正 
           
          
            确 
           
          
            被 
           
          
            预 
           
          
            测 
           
          
            的 
           
          
            总 
           
          
            数 
           
          
          
          
            测 
           
          
            试 
           
          
            集 
           
          
            中 
           
          
            存 
           
          
            在 
           
          
            的 
           
          
            分 
           
          
            类 
           
          
            总 
           
          
            数 
           
          
         
        
       
         Recall = \frac{正确被预测的总数}{测试集中存在的分类总数} 
        
       
     Recall=测试集中存在的分类总数正确被预测的总数​

 
      
       
        
        
          F 
         
        
          − 
         
        
          m 
         
        
          e 
         
        
          a 
         
        
          s 
         
        
          u 
         
        
          r 
         
        
          e 
         
        
          = 
         
         
          
          
            2 
           
          
            ∗ 
           
          
            P 
           
          
            r 
           
          
            e 
           
          
            c 
           
          
            i 
           
          
            s 
           
          
            i 
           
          
            o 
           
          
            n 
           
          
            ∗ 
           
          
            R 
           
          
            e 
           
          
            c 
           
          
            a 
           
          
            l 
           
          
            l 
           
          
          
          
            ( 
           
          
            P 
           
          
            r 
           
          
            e 
           
          
            c 
           
          
            i 
           
          
            s 
           
          
            i 
           
          
            o 
           
          
            n 
           
          
            + 
           
          
            R 
           
          
            e 
           
          
            c 
           
          
            a 
           
          
            l 
           
          
            l 
           
          
            ) 
           
          
         
        
       
         F-measure = \frac{2*Precision*Recall}{(Precision+Recall)} 
        
       
     F−measure=(Precision+Recall)2∗Precision∗Recall​

由于本文主要针对2分类问题，其实验评估主要分为0和1两类，完整代码如下：

```
# -*- coding:utf-8 -*-
import csv
import pandas as pd
import numpy as np
import jieba
import jieba.analyse
from scipy.sparse import coo_matrix
from sklearn import feature_extraction  
from sklearn.feature_extraction.text import TfidfVectorizer
from sklearn.feature_extraction.text import CountVectorizer
from sklearn.feature_extraction.text import TfidfTransformer
from sklearn.model_selection import train_test_split
from sklearn.metrics import classification_report
from sklearn.linear_model import LogisticRegression
from sklearn.ensemble import RandomForestClassifier
from sklearn.tree import DecisionTreeClassifier
from sklearn import svm
from sklearn import neighbors
from sklearn.naive_bayes import MultinomialNB

#----------------------------------第一步 读取文件--------------------------------
with open('fenci_data.csv', 'r', encoding='UTF-8') as f:
    reader = csv.DictReader(f)
    labels = []
    contents = []
    for row in reader:
        labels.append(row['label']) #0-好评 1-差评
        contents.append(row['content'])

print(labels[:5])
print(contents[:5])

#----------------------------------第二步 数据预处理--------------------------------
#将文本中的词语转换为词频矩阵 矩阵元素a[i][j] 表示j词在i类文本下的词频
vectorizer = CountVectorizer(min_df=5)

#该类会统计每个词语的tf-idf权值
transformer = TfidfTransformer()

#第一个fit_transform是计算tf-idf 第二个fit_transform是将文本转为词频矩阵
tfidf = transformer.fit_transform(vectorizer.fit_transform(contents))
for n in tfidf[:5]:
    print(n)
print(type(tfidf))

# 获取词袋模型中的所有词语  
word = vectorizer.get_feature_names()
for n in word[:10]:
    print(n)
print("单词数量:", len(word))

#将tf-idf矩阵抽取出来，元素w[i][j]表示j词在i类文本中的tf-idf权重
#X = tfidf.toarray()
X = coo_matrix(tfidf, dtype=np.float32).toarray() #稀疏矩阵 注意float
print(X.shape)
print(X[:10])

#----------------------------------第三步 数据划分--------------------------------
#使用 train_test_split 分割 X y 列表
X_train, X_test, y_train, y_test = train_test_split(X, 
                                                    labels, 
                                                    test_size=0.3, 
                                                    random_state=1)

#--------------------------------第四步 机器学习分类--------------------------------
# 逻辑回归分类方法模型
LR = LogisticRegression(solver='liblinear')
LR.fit(X_train, y_train)
print('模型的准确度:{}'.format(LR.score(X_test, y_test)))
pre = LR.predict(X_test)
print("逻辑回归分类")
print(len(pre), len(y_test))
print(classification_report(y_test, pre))

#----------------------------------第五步 评价结果--------------------------------
def classification_pj(name, y_test, pre):
    print("算法评价:", name)
    
    # 正确率 Precision = 正确识别的个体总数 /  识别出的个体总数
    # 召回率 Recall = 正确识别的个体总数 /  测试集中存在的个体总数
    # F值 F-measure = 正确率 * 召回率 * 2 / (正确率 + 召回率)

    YC_B, YC_G = 0,0  #预测 bad good
    ZQ_B, ZQ_G = 0,0  #正确
    CZ_B, CZ_G = 0,0  #存在

    #0-good 1-bad 同时计算防止类标变化
    i = 0
    while i&lt;len(pre):
        z = int(y_test[i])   #真实 
        y = int(pre[i])      #预测

        if z==0:
            CZ_G += 1
        else:
            CZ_B += 1
            
        if y==0:
            YC_G += 1
        else:
            YC_B += 1

        if z==y and z==0 and y==0:
            ZQ_G += 1
        elif z==y and z==1 and y==1:
            ZQ_B += 1
        i = i + 1

    print(ZQ_B, ZQ_G, YC_B, YC_G, CZ_B, CZ_G)
    print("")

    # 结果输出
    P_G = ZQ_G * 1.0 / YC_G
    P_B = ZQ_B * 1.0 / YC_B
    print("Precision Good 0:", P_G)
    print("Precision Bad 1:", P_B)

    R_G = ZQ_G * 1.0 / CZ_G
    R_B = ZQ_B * 1.0 / CZ_B
    print("Recall Good 0:", R_G)
    print("Recall Bad 1:", R_B)

    F_G = 2 * P_G * R_G / (P_G + R_G)
    F_B = 2 * P_B * R_B / (P_B + R_B)
    print("F-measure Good 0:", F_G)
    print("F-measure Bad 1:", F_B)

#函数调用
classification_pj("LogisticRegression", y_test, pre)

```

输出结果如下：

```
逻辑回归分类
1823 1823
              precision    recall  f1-score   support

           0       0.94      0.99      0.97      1520
           1       0.93      0.70      0.80       303

    accuracy                           0.94      1823
   macro avg       0.94      0.85      0.88      1823
weighted avg       0.94      0.94      0.94      1823

算法评价: LogisticRegression
213 1504 229 1594 303 1520

Precision Good 0: 0.9435382685069009
Precision Bad 1: 0.9301310043668122
Recall Good 0: 0.9894736842105263
Recall Bad 1: 0.7029702970297029
F-measure Good 0: 0.9659601798330122
F-measure Bad 1: 0.800751879699248

```

---


## 六.算法对比实验

### 1.RandomForest

代码如下：

```
# 随机森林分类方法模型 n_estimators：森林中树的数量
clf = RandomForestClassifier(n_estimators=20)
clf.fit(X_train, y_train)
print('模型的准确度:{}'.format(clf.score(X_test, y_test)))
print("\n")
pre = clf.predict(X_test)
print('预测结果:', pre[:10])
print(len(pre), len(y_test))
print(classification_report(y_test, pre))
classification_pj("RandomForest", y_test, pre)
print("\n")

```

输出结果：

### 2.SVM

代码如下：

```
# SVM分类方法模型
SVM = svm.LinearSVC() #支持向量机分类器LinearSVC
SVM.fit(X_train, y_train)
print('模型的准确度:{}'.format(SVM.score(X_test, y_test)))
pre = SVM.predict(X_test)
print("支持向量机分类")
print(len(pre), len(y_test))
print(classification_report(y_test, pre))
classification_pj("LinearSVC", y_test, pre)
print("\n")

```

输出结果：

### 3.朴素贝叶斯

代码如下：

```
#朴素贝叶斯模型
nb = MultinomialNB()
nb.fit(X_train, y_train)
print('模型的准确度:{}'.format(nb.score(X_test, y_test)))
pre = nb.predict(X_test)
print("朴素贝叶斯分类")
print(len(pre), len(y_test))
print(classification_report(y_test, pre))
classification_pj("MultinomialNB", y_test, pre)
print("\n")

```

输出结果：

### 4.KNN

该算法准确率不高，并且执行时间较长，不建议大家用于文本分析。某些情况的算法对比倒是还行，核心代码如下：

```
#最近邻算法
knn = neighbors.KNeighborsClassifier(n_neighbors=7) 
knn.fit(X_train, y_train)
print('模型的准确度:{}'.format(knn.score(X_test, y_test)))
pre = knn.predict(X_test)
print("最近邻分类")
print(classification_report(y_test, pre))
classification_pj("KNeighbors", y_test, pre)
print("\n")

```

输出结果：

### 5.决策树

代码如下：

```
#决策树算法
dtc = DecisionTreeClassifier()
dtc.fit(X_train, y_train)
print('模型的准确度:{}'.format(dtc.score(X_test, y_test)))
pre = dtc.predict(X_test)
print("决策树分类")
print(len(pre), len(y_test))
print(classification_report(y_test, pre))
classification_pj("DecisionTreeClassifier", y_test, pre)
print("\n")

```

输出结果：

### 6.SGD

代码如下：

```
#SGD分类模型
from sklearn.linear_model.stochastic_gradient import SGDClassifier
sgd = SGDClassifier()
sgd.fit(X_train, y_train)
print('模型的准确度:{}'.format(sgd.score(X_test, y_test)))
pre = sgd.predict(X_test)
print("SGD分类")
print(len(pre), len(y_test))
print(classification_report(y_test, pre))
classification_pj("SGDClassifier", y_test, pre)
print("\n")

```

输出结果：

### 7.MLP

该算法时间比较慢，核心代码如下：

```
#MLP分类模型
from sklearn.neural_network.multilayer_perceptron import MLPClassifier
mlp = MLPClassifier()
mlp.fit(X_train, y_train)
print('模型的准确度:{}'.format(mlp.score(X_test, y_test)))
pre = mlp.predict(X_test)
print("MLP分类")
print(len(pre), len(y_test))
print(classification_report(y_test, pre))
classification_pj("MLPClassifier", y_test, pre)
print("\n")

```

输出结果：

### 8.GradientBoosting

该算法时间比较慢，代码如下：

```
#GradientBoosting分类模型
from sklearn.ensemble import GradientBoostingClassifier
gb = GradientBoostingClassifier()
gb.fit(X_train, y_train)
print('模型的准确度:{}'.format(gb.score(X_test, y_test)))
pre = gb.predict(X_test)
print("GradientBoosting分类")
print(len(pre), len(y_test))
print(classification_report(y_test, pre))
classification_pj("GradientBoostingClassifier", y_test, pre)
print("\n")

```

输出结果：

### 9.AdaBoost

代码如下：

```
#AdaBoost分类模型
from sklearn.ensemble import AdaBoostClassifier
AdaBoost = AdaBoostClassifier()
AdaBoost.fit(X_train, y_train)
print('模型的准确度:{}'.format(AdaBoost.score(X_test, y_test)))
pre = AdaBoost.predict(X_test)
print("AdaBoost分类")
print(len(pre), len(y_test))
print(classification_report(y_test, pre))
classification_pj("AdaBoostClassifier", y_test, pre)
print("\n")


```

输出结果：

---


## 七.总结

写到这里，这篇文章就结束了，下一篇我将带领大家看看深度学习（BiLSTM-CNN）的情感分类方法。希望对您有所帮助，同时文章中不足或错误的地方，欢迎读者提出。这些实验都是我在做论文研究或项目评价常见的一些问题，希望读者带着这些问题，结合自己的需求进行深入的思考，更希望大家能学以致用。最后如果文章对您有帮助，请点赞、评论、收藏，这将是我分享最大的动力。

总之，本文通过Sklearn实现了各种机器学习的情感分类算法，并且我们可以进行实验对比，如下图所示，发现随机森林、SVM、SGD、MLP效果还不错，当然不同数据集效果是不同的，大家需要结合自己的数据集去完成。github下载代码，记得关注点赞喔。

最后，作为人工智能的菜鸟，我希望自己能不断进步并深入，后续将它应用于图像识别、网络安全、对抗样本等领域，指导大家撰写简单的学术论文，一起加油！感谢这些年遇到很多以前进步的博友，共勉~

最近参加了奇安信和清华大学举办的大数据安全比赛，收获非常多，也意识到了鸿沟般的差距。我主要分析的是HC和恶意家族网站分类，大概能从200万条真实网站钟识别了十万多个HC网站，涉及数据抓取、恶意流量检测、跳转劫持判断、NLP和大数据等方法。最后五个方向获奖的主要是清华、中科院信工所、阿里巴巴团队，也有北大、浙大、上交等团队，好厉害，好想学习他们的writeup。真的非常珍惜这样的实战机会，希望未来继续加油，某年能冲进前三名拿个奖。虽然自己很菜，但接下来还是会分享我的大数据分析方法，与大家一起进步。未知攻，焉知防，安全路上还请给位朋友和大佬多多请教，也希望自己能在学术和实战两个方向都进步。有差距不可怕，重要的是我努力过，分享过，加油。最后感谢女神点指导和开导，哈哈~

(By:Eastmount 2020-08-17 周一下午3点写于武汉 [http://blog.csdn.net/eastmount/](http://blog.csdn.net/eastmount/) )

---


<mark>2020年8月18新开的“娜璋AI安全之家”，主要围绕Python大数据分析、网络空间安全、人工智能、Web渗透及攻防技术进行讲解，同时分享CCF、SCI、南核北核论文的算法实现。娜璋之家会更加系统，并重构作者的所有文章，从零讲解Python和安全，写了近十年文章，真心想把自己所学所感所做分享出来，还请各位多多指教，真诚邀请您的关注！谢谢。</mark>

参考文献：<br/> [1] 杨秀璋《Python网络数据爬取及分析从入门到精通（分析篇）》<br/> [2] [https://blog.csdn.net/WANG_hl/article/details/105234432](https://blog.csdn.net/WANG_hl/article/details/105234432)<br/> [3] [https://blog.csdn.net/qq_27590277/article/details/106894245](https://blog.csdn.net/WANG_hl/article/details/105234432)<br/> [4] [https://www.cnblogs.com/alivinfer/p/12892147.html](https://blog.csdn.net/WANG_hl/article/details/105234432)<br/> [5] [https://blog.csdn.net/qq_28626909/article/details/80382029](https://blog.csdn.net/WANG_hl/article/details/105234432)<br/> [6] [https://www.jianshu.com/p/3da3f5608a7c](https://blog.csdn.net/WANG_hl/article/details/105234432)
