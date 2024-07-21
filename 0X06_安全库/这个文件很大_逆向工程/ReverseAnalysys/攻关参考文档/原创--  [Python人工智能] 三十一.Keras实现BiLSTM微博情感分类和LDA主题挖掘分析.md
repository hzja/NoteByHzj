# 原创
：  [Python人工智能] 三十一.Keras实现BiLSTM微博情感分类和LDA主题挖掘分析

# [Python人工智能] 三十一.Keras实现BiLSTM微博情感分类和LDA主题挖掘分析

<font color="red">**从本专栏开始，作者正式研究Python深度学习、神经网络及人工智能相关知识。前一篇文章通过Keras深度学习构建CNN模型识别阿拉伯手写文字图像，一篇非常经典的图像分类文字。这篇文章将结合文本挖掘介绍微博情感分类知识，包括数据预处理、机器学习和深度学习的情感分类，后续结合LDA进行主题挖掘。基础性文章，希望对您有所帮助！**</font>

#### 文章目录

本专栏主要结合作者之前的博客、AI经验和相关视频及论文介绍，后面随着深入会讲解更多的Python人工智能案例及应用。基础性文章，希望对您有所帮助，如果文章中存在错误或不足之处，还请海涵！作者作为人工智能的菜鸟，希望大家能与我在这一笔一划的博客中成长起来。写了这么多年博客，尝试第一个付费专栏，为小宝赚点奶粉钱，但更多博客尤其基础性文章，还是会继续免费分享，该专栏也会用心撰写，望对得起读者。如果有问题随时私聊我，只望您能从这个系列中学到知识，一起加油喔~

**前文赏析：**

---


## 一.BiLSTM模型

LSTM的全称是Long Short-Term Memory，它是RNN（Recurrent Neural Network）的一种。LSTM由于其设计的特点，非常适合用于对时序数据的建模，如文本数据。BiLSTM是Bi-directional Long Short-Term Memory的缩写，是由前向LSTM与后向LSTM组合而成。两者在自然语言处理任务中都常被用来建模上下文信息。

### 1.RNN

循环神经网络英文是Recurrent Neural Networks，简称RNN。假设有一组数据data0、data1、data2、data3，使用同一个神经网络预测它们，得到对应的结果。如果数据之间是有关系的，比如做菜下料的前后步骤，英文单词的顺序，如何让数据之间的关联也被神经网络学习呢？这就要用到——RNN。

RNN结构如下图所示，按照时间点t-1、t、t+1，每个时刻有不同的x，每次计算会考虑上一步的state和这一步的x(t)，再输出y值。在该数学形式中，每次RNN运行完之后都会产生s(t)，当RNN要分析x(t+1)时，此刻的y(t+1)是由s(t)和s(t+1)共同创造的，s(t)可看作上一步的记忆。多个神经网络NN的累积就转换成了循环神经网络，其简化图如下图的左边所示。

RNN常用于自然语言处理、机器翻译、语音识别、图像识别等领域。

---


### 2.LSTM和BiLSTM

RNN是在有序的数据上进行学习的，RNN会像人一样对先前的数据发生记忆，但有时候也会像老爷爷一样忘记先前所说。为了解决RNN的这个弊端，提出了LTSM技术，它的英文全称是Long short-term memory，长短期记忆，也是当下最流行的RNN之一。

LSTM是在普通的RNN上面做了一些改进，LSTM RNN多了三个控制器，即输入、输出、忘记控制器。左边多了个条主线，例如电影的主线剧情，而原本的RNN体系变成了分线剧情，并且三个控制器都在分线上。

通过这三个gate能够很好地控制我们的RNN，基于这些控制机制，LSTM是延缓记忆的良药，从而带来更好的结果。

---


## 二.数据集介绍

**数据描述：**

|类型|描述
|------
|数据概览|36 万多条，带情感标注 新浪微博，包含 4 种情感，其中喜悦约 20 万条，愤怒、厌恶、低落各约 5 万条
|推荐实验|情感/观点/评论 倾向性分析
|数据来源|新浪微博
|原数据集|微博情感分析数据集，网上搜集，具体作者、来源不详
|数据描述|微博总体数目为361744： 喜悦-199496、愤怒-51714、厌恶-55267、低落-55267
|对应类标|0: 喜悦, 1: 愤怒, 2: 厌恶, 3: 低落

**数据示例：**

注意，做到实验分析，作者才发现“厌恶-55267”和“低落-55267”的数据集完全相同，因此我们做三分类问题，更重要的是思想。

**下载地址：**

**参考链接：**

---


## 三.数据预处理

### 1.数据读取

首先，我们尝试读取微博数据，看看各个情感类别的数量。

```
# -*- coding: utf-8 -*-
"""
Created on Mon Sep 27 22:21:53 2021
@author: xiuzhang
"""
import pandas as pd

pd_all = pd.read_csv('simplifyweibo_4_moods.csv')
moods = {0: '喜悦', 1: '愤怒', 2: '厌恶', 3: '低落'}

print('微博数目（总体）：%d' % pd_all.shape[0])

for label, mood in moods.items(): 
    print('微博数目（{}）：{}'.format(mood,pd_all[pd_all.label==label].shape[0]))

```

其输出结果如下图所示：

---


### 2.中文分词及停用词过滤

接下来，通常是读取文本数据并进行中文分词和数据清洗，停用词过滤主要是清洗掉特殊字符和无关的特征词（如“我们”“这样”“之前”等）。注意通过如下所示的核心代码可以按行读取文件内容，比如输出10行内容。

```
k = 0
for line in range(len(pd_all)): #label review
    dict_label = pd_all['label'][line]
    dict_review = pd_all['review'][line]
    print(dict_label, dict_review)
    k += 1
    if k&gt;10:
        break

```

输出结果如下图所示：

> 
<mark>**Python文本挖掘学习小建议**</mark><br/> (1) 如果你是一名文本分析初学者，在写代码过程中，尽量多输出中间结果（打桩调试），这样会让你更加清晰每个步骤所完成的内容，同时方便自主调试我们代码。<br/> (2) 个人建议将中文分词及停用词过滤的文件单独存成另一份文件，这样便于后续的深度学习分类和主题挖掘实验。


包含词频统计的完整代码如下：

```
# -*- coding: utf-8 -*-
"""
Created on Mon Sep 27 22:21:53 2021
@author: xiuzhang
"""
import pandas as pd
import jieba
import csv
from collections import Counter

#-----------------------------------------------------------------------------
#样本数量统计
pd_all = pd.read_csv('simplifyweibo_4_moods.csv')
moods = {0: '喜悦', 1: '愤怒', 2: '厌恶', 3: '低落'}

print('微博数目（总体）：%d' % pd_all.shape[0])
for label, mood in moods.items(): 
    print('微博数目（{}）：{}'.format(mood,pd_all[pd_all.label==label].shape[0]))
    
#-----------------------------------------------------------------------------
#中文分词和停用词过滤
cut_words = ""
all_words = ""
stopwords = ["[", "]", "）", "（", ")", "(", "【", "】", "！", "，", "$",
             "·", "？", ".", "、", "-", "—", ":", "：", "《", "》", "=",
             "。", "…", "“", "?", "”", "~", " ", "－", "+", "\\", "‘",
             "～", "；", "’", "...", "..", "&amp;", "#",  "....", ",", 
             "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10"
             "的", "和", "之", "了", "哦", "那", "一个",  ]

fw = open('simplifyweibo_4_moods_fenci.csv', 'w', encoding='utf-8')

for line in range(len(pd_all)): #label review
    dict_label = pd_all['label'][line]
    dict_review = pd_all['review'][line]
    #print(dict_label, dict_review)
    
    #jieba分词并过滤停用词
    cut_words = ""
    data = dict_review.strip('\n')
    data = data.replace(",", "")    #一定要过滤符号 ","否则多列
    seg_list = jieba.cut(data, cut_all=False)
    for seg in seg_list:
        if seg not in stopwords:
            cut_words += seg + " "
    all_words += cut_words
    #print(cut_words)
    
    fw.write(str(dict_label)+","+str(cut_words)+"\n")
    #print(str(dict_label)+","+str(cut_words)+"\n")
else:
    fw.close()

#-----------------------------------------------------------------------------
#词频统计
all_words = all_words.split()
print(all_words)

c = Counter()
for x in all_words:
    if len(x)&gt;1 and x != '\r\n':
        c[x] += 1
        
#输出词频最高的前10个词
print('\n词频统计结果：')
for (k,v) in c.most_common(10):
    print("%s:%d"%(k,v))

```

输出结果如下图所示：

统计出现次数最多的10个单词如下图所示，我们可以通过大连理工大学情感词或词性来计算喜怒哀乐不同类别的常见特征词。读者可以去尝试下，也可以看我之前写的博客。

---


### 3.词频可视化分析

接着我们读取分词后的CSV文件，然后运用PyEcharts绘制词云图。

该部分代码如下：

```
# -*- coding: utf-8 -*-
"""
Created on Mon Sep 27 22:21:53 2021
@author: xiuzhang
"""
import pandas as pd
import csv
from collections import Counter

#-----------------------------------------------------------------------------
#读取分词后特征词
cut_words = ""
all_words = ""
pd_all = pd.read_csv('simplifyweibo_4_moods_fenci.csv')
moods = {0: '喜悦', 1: '愤怒', 2: '厌恶', 3: '低落'}
print('微博数目（总体）：%d' % pd_all.shape[0])
for label, mood in moods.items(): 
    print('微博数目（{}）：{}'.format(mood,pd_all[pd_all.label==label].shape[0]))
    
for line in range(len(pd_all)): #label review
    dict_label = pd_all['label'][line]
    dict_review = pd_all['review'][line]
    cut_words = dict_review
    all_words += str(cut_words)
    #print(cut_words)

#-----------------------------------------------------------------------------
#词频统计
all_words = all_words.split()
c = Counter()
for x in all_words:
    if len(x)&gt;1 and x != '\r\n':
        c[x] += 1
print('\n词频统计结果：')
for (k,v) in c.most_common(10):
    print("%s:%d"%(k,v))
    
#存储数据
name ="data-word-count.csv"
fw = open(name, 'w', encoding='utf-8')
i = 1
for (k,v) in c.most_common(len(c)):
    fw.write(str(i)+','+str(k)+','+str(v)+'\n')
    i = i + 1
else:
    print("Over write file!")
    fw.close()

#-----------------------------------------------------------------------------
#词云分析
from pyecharts import options as opts
from pyecharts.charts import WordCloud
from pyecharts.globals import SymbolType

#生成数据 word = [('A',10), ('B',9), ('C',8)] 列表+Tuple
words = []
for (k,v) in c.most_common(200):
    words.append((k,v))

#渲染图
def wordcloud_base() -&gt; WordCloud:
    c = (
        WordCloud()
        .add("", words, word_size_range=[20, 40], shape='diamond') #shape=SymbolType.ROUND_RECT
        .set_global_opts(title_opts=opts.TitleOpts(title='词云图'))
    )
    return c
wordcloud_base().render('微博评论情感分析词云图.html')

```

输出结果如下图所示：

<mark>其实上图效果并不很好，如果想要获得更高质量的效果，且对比多种情感，怎么实现呢？请读者下来自行提升，下面是对应的效果图。</mark>

---


## 四.机器学习情感分析

经过上述的预处理后，接下来我们将直接介绍BiLSTM实现微博情感分类。数据集修改为：

再次强调，作者代码中会含详细注释和中间过程输出，也建议初学者这样尝试。或许代码风格不是很美观，但只有掌握每行代码的实质，为什么怎样做？会得到怎么的输出结果，才能为后续做深入的分析和工程提供更好地编程习惯和理解。如果您是大佬，也请原谅我喜欢使用 print 的不足吧！

```
# -*- coding: utf-8 -*-
"""
Created on Mon Sep 27 22:21:53 2021
@author: xiuzhang
"""
import pandas as pd
import numpy as np
from collections import Counter
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

#-----------------------------------------------------------------------------
#读取分词后特征词
pd_all = pd.read_csv('weibo_3_fenci.csv')
moods = {0: '喜悦', 1: '愤怒', 2: '低落'}
print('微博数目（总体）：%d' % pd_all.shape[0])
for label, mood in moods.items(): 
    print('微博数目（{}）：{}'.format(mood,pd_all[pd_all.label==label].shape[0]))

labels = []
contents = []
for line in range(len(pd_all)):  #label review
    labels.append(pd_all['label'][line])
    contents.append(str(pd_all['review'][line]))
print(len(labels),len(contents))

#-----------------------------------------------------------------------------
#TFIDF计算
#将文本中的词语转换为词频矩阵 矩阵元素a[i][j] 表示j词在i类文本下的词频
vectorizer = CountVectorizer(min_df=50)   #MemoryError控制参数

#该类会统计每个词语的tf-idf权值
transformer = TfidfTransformer()

#第一个fit_transform是计算tf-idf 第二个fit_transform是将文本转为词频矩阵
tfidf = transformer.fit_transform(vectorizer.fit_transform(contents))
for n in tfidf[:5]:
    print(n)
print(type(tfidf))

#获取词袋模型中的所有词语  
word = vectorizer.get_feature_names()
for n in word[:10]:
    print(n)
print("单词数量:", len(word))  #74806 12884

#将tf-idf矩阵抽取出来，元素w[i][j]表示j词在i类文本中的tf-idf权重
X = coo_matrix(tfidf, dtype=np.float32).toarray() #稀疏矩阵 float
#X = tfidf.toarray()
print(X.shape)
print(X[:10])

#数据划分
X_train, X_test, y_train, y_test = train_test_split(X, 
                                                    labels, 
                                                    test_size=0.3, 
                                                    random_state=1)
print(len(X_train),len(X_test),len(y_train),len(y_test))

#-----------------------------------------------------------------------------
# 逻辑回归分类方法模型
LR = LogisticRegression(solver='liblinear')
LR.fit(X_train, y_train)
print('模型的准确度:{}'.format(LR.score(X_test, y_test)))
pre = LR.predict(X_test)
print("逻辑回归分类")
print(len(pre), len(y_test))
print(classification_report(y_test, pre, digits=4))
print("\n")

```

其运行结果如下图所示，效果不是很理想。你是否想到了优化方法呢？

---


## 五.BiLSTM微博情感分类

### 1.模型构建

核心代码：

```
num_labels = 3
model = Sequential()
model.add(Embedding(max_words+1, 128, input_length=max_len))
model.add(Bidirectional(CuDNNLSTM(128)))
model.add(Dense(128, activation='relu'))
model.add(Dropout(0.3))
model.add(Dense(num_labels, activation='softmax'))
model.summary()
model.compile(loss="categorical_crossentropy",
              optimizer='adam',  # RMSprop()
              metrics=["accuracy"])

```

完整代码如下图所示：

```
# -*- coding: utf-8 -*-
"""
Created on Mon Sep 27 22:21:53 2021
@author: xiuzhang
"""
import pickle
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns
from sklearn import metrics
from sklearn.preprocessing import LabelEncoder,OneHotEncoder
from keras.models import Model
from keras.layers import LSTM, Activation, Dense, Dropout, Input, Embedding
from keras.layers import Convolution1D, MaxPool1D, Flatten, CuDNNLSTM
from keras.optimizers import RMSprop
from keras.layers import Bidirectional
from keras.preprocessing.text import Tokenizer
from keras.preprocessing import sequence
from keras.callbacks import EarlyStopping
from keras.models import load_model
from keras.models import Sequential
from keras.layers.merge import concatenate

import os
import tensorflow as tf
os.environ["CUDA_DEVICES_ORDER"] = "PCI_BUS_IS"
os.environ["CUDA_VISIBLE_DEVICES"] = "0"

#指定了每个GPU进程中使用显存的上限,0.9表示可以使用GPU 90%的资源进行训练
gpu_options = tf.GPUOptions(per_process_gpu_memory_fraction=0.8)
sess = tf.Session(config=tf.ConfigProto(gpu_options=gpu_options))

#---------------------------------------第一步 数据读取------------------------------------
#读取测数据集
train_df = pd.read_csv("weibo_3_fenci_train.csv")
val_df = pd.read_csv("weibo_3_fenci_val.csv")
test_df = pd.read_csv("weibo_3_fenci_test.csv")

#指定数据类型 否则AttributeError: 'float' object has no attribute 'lower' 存在文本为空的现象
print(train_df.head())

#解决中文显示问题
plt.rcParams['font.sans-serif'] = ['KaiTi'] # 指定默认字体 SimHei黑体
plt.rcParams['axes.unicode_minus'] = False # 解决保存图像是负号'

#---------------------------------第二步 OneHotEncoder()编码---------------------------------
#对数据集的标签数据进行编码
train_y = train_df.label
print("Label:")
print(train_y[:10])
val_y = val_df.label
test_y = test_df.label
le = LabelEncoder()
train_y = le.fit_transform(train_y).reshape(-1,1)
print("LabelEncoder")
print(train_y[:10])    #99675
print(len(train_y))
val_y = le.transform(val_y).reshape(-1,1)
test_y = le.transform(test_y).reshape(-1,1)

#对数据集的标签数据进行one-hot编码
ohe = OneHotEncoder()
train_y = ohe.fit_transform(train_y).toarray()
val_y = ohe.transform(val_y).toarray()
test_y = ohe.transform(test_y).toarray()
print("OneHotEncoder:")
print(train_y[:10])

#-------------------------------第三步 使用Tokenizer对词组进行编码-------------------------------
#使用Tokenizer对词组进行编码
#当我们创建了一个Tokenizer对象后，使用该对象的fit_on_texts()函数，以空格去识别每个词,
#可以将输入的文本中的每个词编号，编号是根据词频的，词频越大，编号越小。
max_words = 5000
max_len = 600
tok = Tokenizer(num_words=max_words)  ## 使用的最大词语数为5000
#print(train_df.cutword[:5])
tok.fit_on_texts(train_df.cutword)
print(tok)

#保存训练好的Tokenizer和导入
with open('tok.pickle', 'wb') as handle:
    pickle.dump(tok, handle, protocol=pickle.HIGHEST_PROTOCOL)
with open('tok.pickle', 'rb') as handle:
    tok = pickle.load(handle)

#使用word_index属性可以看到每次词对应的编码
#使用word_counts属性可以看到每个词对应的频数
for ii,iterm in enumerate(tok.word_index.items()):
    if ii &lt; 10:
        print(iterm)
    else:
        break
print("===================")  
for ii,iterm in enumerate(tok.word_counts.items()):
    if ii &lt; 10:
        print(iterm)
    else:
        break
        
#使用tok.texts_to_sequences()将数据转化为序列
#使用sequence.pad_sequences()将每个序列调整为相同的长度
print(train_df.cutword)
data_train = train_df.cutword.astype(str)
data_val = val_df.cutword.astype(str)
data_test = test_df.cutword.astype(str)

train_seq = tok.texts_to_sequences(data_train)
val_seq = tok.texts_to_sequences(data_val)
test_seq = tok.texts_to_sequences(data_test)

#将每个序列调整为相同的长度
train_seq_mat = sequence.pad_sequences(train_seq,maxlen=max_len)
val_seq_mat = sequence.pad_sequences(val_seq,maxlen=max_len)
test_seq_mat = sequence.pad_sequences(test_seq,maxlen=max_len)
print(train_seq_mat.shape)  #(35000, 600)
print(val_seq_mat.shape)    #(5000, 600)
print(test_seq_mat.shape)   #(10000, 600)
print(train_seq_mat[:2])

#-------------------------------第四步 建立LSTM模型并训练-------------------------------
num_labels = 3
model = Sequential()
model.add(Embedding(max_words+1, 128, input_length=max_len))

#model.add(Bidirectional(LSTM(128, dropout=0.2, recurrent_dropout=0.1)))
model.add(Bidirectional(CuDNNLSTM(128)))
model.add(Dense(128, activation='relu'))
model.add(Dropout(0.3))
model.add(Dense(num_labels, activation='softmax'))
model.summary()
model.compile(loss="categorical_crossentropy",
              optimizer='adam',  # RMSprop()
              metrics=["accuracy"])
# 增加判断 防止再次训练
flag = "train"
if flag == "train":
    print("模型训练")
    ## 模型训练
    model_fit = model.fit(train_seq_mat, train_y, batch_size=128, epochs=10,
                          validation_data=(val_seq_mat,val_y),
                          callbacks=[EarlyStopping(monitor='val_loss',min_delta=0.0001)]  
                         )
    # 保存模型
    model.save('my_model.h5')  
    del model  # deletes the existing model
else:
    print("模型预测")
    # 导入已经训练好的模型
    model = load_model('my_model.h5')
    
    #--------------------------------------第五步 预测及评估--------------------------------
    #对测试集进行预测
    test_pre = model.predict(test_seq_mat)
    
    #评价预测效果，计算混淆矩阵
    confm = metrics.confusion_matrix(np.argmax(test_pre,axis=1),
                                     np.argmax(test_y,axis=1))
    print(confm)
    #混淆矩阵可视化
    Labname = ['喜悦','愤怒', '哀伤']
    print(metrics.classification_report(np.argmax(test_pre,axis=1),
                                        np.argmax(test_y,axis=1),
                                        digits=4))
    
    plt.figure(figsize=(8,8))
    sns.heatmap(confm.T, square=True, annot=True,
                fmt='d', cbar=False, linewidths=.6,
                cmap="YlGnBu")
    plt.xlabel('True label',size = 14)
    plt.ylabel('Predicted label', size = 14)
    plt.xticks(np.arange(3), Labname, size = 12)
    plt.yticks(np.arange(3), Labname, size = 12)
    plt.show()

    #--------------------------------------第六步 验证算法--------------------------------
    #使用tok对验证数据集重新预处理，并使用训练好的模型进行预测
    data_val = val_df.cutword.astype(str)
    val_seq = tok.texts_to_sequences(data_val)
    val_seq_mat = sequence.pad_sequences(val_seq,maxlen=max_len)
    val_pre = model.predict(val_seq_mat)
    print(metrics.classification_report(np.argmax(val_pre,axis=1),
                                        np.argmax(val_y,axis=1),
                                        digits=4))

```

---


### 2.实验分析

作者习惯将数据随机划分为训练集、测试集和验证集（乱序），然后调用模型进行实验。

构建模型如下，读者可以尝试对比各种参数。

```
_________________________________________________________________
Layer (type)                 Output Shape              Param #   
=================================================================
embedding_1 (Embedding)      (None, 600, 128)          640128    
_________________________________________________________________
bidirectional_1 (Bidirection (None, 256)               264192    
_________________________________________________________________
dense_1 (Dense)              (None, 128)               32896     
_________________________________________________________________
dropout_1 (Dropout)          (None, 128)               0         
_________________________________________________________________
dense_2 (Dense)              (None, 3)                 387       
=================================================================
Total params: 937,603
Trainable params: 937,603
Non-trainable params: 0
_________________________________________________________________

```

训练过程中输出的结果如下所示：

```
Epoch 1/10
179981/179981 [==============================] - 30s 167us/step - loss: 0.8538 - acc: 0.6531 - val_loss: 0.8231 - val_acc: 0.6633
Epoch 2/10
179981/179981 [==============================] - 28s 158us/step - loss: 0.7923 - acc: 0.6752 - val_loss: 0.7910 - val_acc: 0.6769
Epoch 3/10
179981/179981 [==============================] - 30s 167us/step - loss: 0.7499 - acc: 0.6938 - val_loss: 0.7750 - val_acc: 0.6879
Epoch 4/10
179981/179981 [==============================] - 29s 164us/step - loss: 0.7169 - acc: 0.7085 - val_loss: 0.7736 - val_acc: 0.6880
Epoch 5/10
179981/179981 [==============================] - 31s 171us/step - loss: 0.6880 - acc: 0.7219 - val_loss: 0.7802 - val_acc: 0.6922

```

最终实验结果如下所示。

```
             precision    recall  f1-score   support

          0     0.2378    0.5555    0.3330      5282
          1     0.9665    0.7101    0.8187     62355
          2     0.1297    0.6526    0.2164      2352

avg / total     0.8833    0.6965    0.7618     69989

             precision    recall  f1-score   support

          0     0.2331    0.5654    0.3301      4093
          1     0.9662    0.7052    0.8153     48625
          2     0.1292    0.6340    0.2147      1869

avg / total     0.8826    0.6922    0.7584     54587

```

同样我们可以输出混淆矩阵。

如果实验效果仍然不佳，读者需要从数据增强（不均衡问题）、数据集标注、数据清洗、停用词过滤、情感特征词提取等方面进行优化。每篇论文

---


## 六.总结

写到这里，这篇文章就介绍结束了，真心希望这篇文章对您有所帮助，加油~

下载地址：

(By:Eastmount 2021-09-29 夜于武汉 http://blog.csdn.net/eastmount/ )

---

