Python
<a name="bcGqP"></a>
## 混合派

1. Knock Knock:从Python发送通知到移动设备、桌面或电子邮件。
2. tqdm:可扩展的Python和CLI进度条，内置对pandas的支持。
3. Colorama:简单的跨平台彩色终端文本。
4. pandas -log:提供熊猫基本操作的反馈。非常适合调试长管道链。
5. Pandas-flavor:扩展pandas DataFrame/Series的简单方法。
6. More-Itertools:增加了类似于itertools的额外功能。
7. streamlit:为机器学习项目创建应用程序的简单方法。
<a name="T8RWb"></a>
## 数据清理和操作

1. ftfy:修复mojibake和Unicode文本中的其他故障。
2. janitor:有很多很酷的功能来清理数据。
3. Optimus:另一个数据清理包。
4. Great-experctations:一个检查数据是否符合预期的好工具。
<a name="ODLzy"></a>
## 数据探索和建模

1. Pandas-profile:创建一个包含来自pandas DataFrame的统计数据的HTML报告。
2. dabl:允许使用可视化和预处理进行数据探索。
3. pydqc:允许比较两个数据集之间的统计数据。
4. pandas-summary:对panda DataFrames描述功能的扩展。
5. pivottable-js:pands在jupyter notebook的拖放功能。
<a name="jyYr8"></a>
## 数据结构

1. Bounter:有效的计数器，使用有限(有界)的数量的内存，无论数据大小。
2. Python -bloomfilter:可扩展的Bloom Filter，使用Python实现。
3. datasketch:提供概率数据结构，如LSH、加权MinHash、HyperLogLog等。
4. ranges:Python的连续范围、范围集和范围令数据结构
<a name="7jDde"></a>
## 性能检查和优化

1. Py-spy: Python程序的采样分析器。
2. pyperf:用于运行Python基准测试的工具箱。
3. snakeviz:一个浏览器内的Python配置文件查看器，对jupyter notebook有很大的支持。
4. Cachier: Python函数的持久、无延迟、本地和跨机缓存。
5. Faiss:用于高效的相似性搜索和密集向量聚类的库。
<a name="RB6YJ"></a>
## Python内置函数
Python自带的内置函数。函数无需导入，直接使用。例如要计算-3.2的绝对值，直接使用abs函数，方法是
```python
abs(-3.2)
```
<a name="T5HMW"></a>
## Python标准库
Python自带的标准库。Python标准库无需安装，只需要先通过`import`方法导入便可使用其中的方法。例如导入`string`模块，然后使用其中的`find`方法：
```python
import stringstring.find('abcde','b')
```
<a name="DiJ53"></a>
## 第三方库
Python的第三方库。这些库需要先进行安装（部分可能需要配置）。
<a name="gKnG2"></a>
## 外部工具
非Python写成的库或包，用于Python数据工作的相关工具。
<a name="qAErM"></a>
## 1、文件读写
文件的读写包括常见的txt、Excel、xml、二进制文件以及其他格式的数据文本，主要用于本地数据的读写。
<a name="YQvjV"></a>
### 1. `open(name[, mode[, buffering]])`

- **类型：**Python内置函数
- **描述：**Python默认的文件读写方法
- **推荐度：**★★★
<a name="NKsmr"></a>
### 2. `numpy.loadtxt`、`numpy.load`和`numpy.fromfile`

- **类型：**第三方库
- **描述：**Numpy自带的读写函数，包括loadtxt、load和fromfile，用于文本、二进制文件读写
- **推荐度：**★★★
<a name="apXkO"></a>
### 3. `pandas.read_*`

- **类型：**第三方库
- **描述：**Pandas自带的read文件方法，例如ead_csv、read_fwf、read_table等，用于文本、Excel、二进制文件、HDF5、表格、SAS文件、SQL数据库、Stata文件等的读写
- **推荐度：**★★★
<a name="VmRMR"></a>
### 4. xlrd

- **类型：**第三方库
- **描述：**用于Excel文件读取
- **推荐度：**★★
<a name="ydquT"></a>
### 5. xlwt

- **类型：**第三方库
- **描述：**用于Excel文件写入
- **推荐度：**★★
<a name="AntzY"></a>
### 6. pyexcel-xl

- **类型：**第三方库
- **描述：**用于Excel文件读写
- **推荐度：**★★
<a name="wSizZ"></a>
### 7. xluntils

- **类型：**第三方库
- **描述：**用于Excel文件读写
- **推荐度：**★★
<a name="O5giL"></a>
### 8. pyExcelerator

- **类型：**第三方库
- **描述：**用于Excel文件读写
- **推荐度：**★
<a name="WN0sn"></a>
### 9. openpyxl

- **类型：**第三方库
- **描述：**用于Excel文件读写
- **推荐度：**★
<a name="dOrGg"></a>
### 10. lxml

- **类型：**第三方库
- **描述：**xml和HTML读取和解析
- **推荐度：**★★★
<a name="DDndg"></a>
### 11. xml

- **类型：**Python标准库
- **描述：**xml对象解析和格式化处理
- **推荐度：**★★★
<a name="t48JA"></a>
### 12. libxml2

- **类型：**第三方库
- **描述：**xml对象解析和格式化处理
- **推荐度：**★
<a name="frLK0"></a>
### 13. xpath

- **类型：**第三方库
- **描述：**xml对象解析和格式化处理
- **推荐度：**★★
<a name="p3LXF"></a>
### 14. win32com

- **类型：**第三方库
- **描述：**有关Windows系统操作、Office（Word、Excel等）文件读写等的综合应用库
- **推荐度：**★
<a name="mMOX6"></a>
## 2、网络抓取和解析
网络抓取和解析用于从互联网中抓取信息，并对HTML对象进行处理，有关xml对象的解析和处理的库在“文件读写”中找到。
<a name="HSvrm"></a>
### 15. requests

- **类型：**第三方库
- **描述：**网络请求库，提供多种网络请求方法并可定义复杂的发送信息
- **推荐度：**★★★
<a name="KbsW9"></a>
### 16. urllib

- **类型：**Python标准库
- **描述：**Python自带的库，简单的读取特定URL并获得返回的信息
- **推荐度：**★★
<a name="deMpM"></a>
### 17. urllib2

- **类型：**Python标准库
- **描述：**Python自带的库，读取特定URL并获得返回的信息，相对于urllib可处理更多HTTP信息，例如cookie、身份验证、重定向等
- **推荐度：**★★
<a name="TsgZc"></a>
### 18. urlparse

- **类型：**Python标准库
- **描述：**Python自带的URL解析库，可自动解析URL不同的域、参数、路径等
- **推荐度：**★★★
<a name="Xv1i5"></a>
### 19. HTMLParser

- **类型：**Python标准库
- **描述：**Python自带的HTML解析模块，能够很容易的实现HTML文件的分析
- **推荐度：**★★★
<a name="jUCKJ"></a>
### 20. Scapy

- **类型：**第三方库
- **描述：**分布式爬虫框架，可用于模拟用户发送、侦听和解析并伪装网络报文，常用于大型网络数据爬取
- **推荐度：**★★★
<a name="lgkb2"></a>
### 21. Beautiful Soup

- **类型：**第三方库
- **描述：**Beautiful Soup是网页数据解析和格式化处理工具，通常配合Python的urllib、urllib2等库一起使用
- **推荐度：**★★★
<a name="VmYMy"></a>
## 3、数据库连接
数据库连接可用于连接众多数据库以及访问通用数据库接口，可用于数据库维护、管理和增、删、改、查等日常操作。
<a name="Bq7qz"></a>
### 22. mysql-connector-python

- **类型：**第三方库
- **描述：**MySQL官方驱动连接程序
- **推荐度：**★★★
<a name="BKtM4"></a>
### 23. pymysql

- **类型：**第三方库
- **描述：**MySQL连接库，支持Python3
- **推荐度：**★★★
<a name="oDQ7e"></a>
### 24. MySQL-python

- **类型：**第三方库
- **描述：**MySQL连接库
- **推荐度：**★★
<a name="L4dv7"></a>
### 25. cx_Oracle

- **类型：**第三方库
- **描述：**Oracle连接库
- **推荐度：**★★★
<a name="CnN3U"></a>
### 26. psycopg2

- **类型：**第三方库
- **描述：**Python编程语言中非常受欢迎的PostgreSQL适配器
- **推荐度：**★★★
<a name="gVNbi"></a>
### 27. redis

- **类型：**Python标准库
- **描述：**Redis连接库
- **推荐度：**★★★
<a name="xMqE5"></a>
### 28. pymongo

- **类型：**第三方库
- **描述：**MongoDB官方驱动连接程序
- **推荐度：**★★★
<a name="B0nu3"></a>
### 29. HappyBase

- **类型：**第三方库
- **描述：**HBase连接库
- **推荐度：**★★★
<a name="iKsjk"></a>
### 30. py2neo

- **类型：**第三方库
- **描述：**Neo4j连接库
- **推荐度：**★★★
<a name="z4QjG"></a>
### 31. cassandra-driver

- **类型：**第三方库
- **描述：**Cassandra（1.2+）和DataStax Enterprise（3.1+）连接库
- **推荐度：**★★★
<a name="eRtsv"></a>
### 32. sqlite3

- **类型：**Python标准库
- **描述：**Python自带的模块，用于操作SQLite数据库
- **推荐度：**★★★
<a name="yGMrK"></a>
### 33. pysqlite2

- **类型：**第三方库
- **描述：**SQLite 3.x连接库
- **推荐度：**★★
<a name="wfjF6"></a>
### 34. bsddb3

- **类型：**第三方库
- **描述：**Berkeley DB连接库
<a name="hmPCR"></a>
### 35. bsddb

- **类型：**Python标准库
- **描述：**Python自带的模块，提供了一个到Berkeley DB库的接口
- **推荐度：**★★
<a name="bbzBi"></a>
### 36. dbhash

- **类型：**Python标准库
- **描述：**Python自带的模块，dbhash模块提供了使用BSD数据库库打开数据库的功能。该模块镜像了提供对DBM样式数据库访问的其他Python数据库模块的接口。bsddb模块需要使用dbhash
- **推荐度：**★★
<a name="aturB"></a>
### 37. adodb

- **类型：**第三方库
- **描述：**ADOdb是一个数据库抽象库，支持常见的数据和数据库接口并可自行进行数据库扩展，该库可以对不同数据库中的语法进行解析和差异化处理，具有很高的通用性
- **推荐度：**★★★
<a name="zZlBe"></a>
### 38. SQLObject

- **类型：**第三方库
- **描述：**SQLObject是一种流行的对象关系管理器，用于向数据库提供对象接口，其中表为类、行为实例、列为属性
- **推荐度：**★★
<a name="CgPe6"></a>
### 39. SQLAlchemy

- **类型：**第三方库
- **描述：**SQLAlchemy是Python SQL工具包和对象关系映射器，为应用程序开发人员提供了SQL的全部功能和灵活性控制
- **推荐度：**★★
<a name="W9Qpi"></a>
### 40. ctypes

- **类型：**第三方库
- **描述：**ctypes是Python的一个外部库，提供和C语言兼容的数据类型，可以很方便地调用C DLL中的函数
- **推荐度：**★★★
<a name="P4553"></a>
### 41. pyodbc

- **类型：**第三方库
- **描述：**Python通过ODBC访问数据库的接口库
- **推荐度：**★★★
<a name="Q4ONm"></a>
### 42. Jython

- **类型：**第三方库
- **描述：**Python通过JDBC访问数据库的接口库
- **推荐度：**★★★
<a name="z7He8"></a>
## 4、数据清洗转换
数据清洗转换主用于数据正式应用之前的预处理工作。
<a name="AbtRe"></a>
### 43. `frozenset([iterable])`

- **类型：**Python内置函数
- **描述：**返回一个新的frozenset对象，可选择从iterable取得的元素
- **推荐度：**★★★
<a name="Maiis"></a>
### 44. `int(x)`

- **类型：**Python内置函数
- **描述：**返回x的整数部分
- **推荐度：**★★★
<a name="o8ViJ"></a>
### 45. `isinstance(object, classinfo)`

- **类型：**Python内置函数
- **描述：**返回object是否是指定的classinfo实例信息
- **推荐度：**★★★
<a name="cZQ34"></a>
### 46. `len(s)`

- **类型：**Python内置函数
- **描述：**返回对象的长度或项目数量
- **推荐度：**★★★
<a name="iDAHU"></a>
### 47. `long(x)`

- **类型：**Python内置函数
- **描述：**返回由字符串或数字x构造的长整型对象
- **推荐度：**★★★
<a name="pTsCK"></a>
### 48. `max(iterable[, key])`

- **类型：**Python内置函数
- **描述：**返回一个可迭代或最大的两个或多个参数中的最大项
- **推荐度：**★★★
<a name="XmZX1"></a>
### 49. `min(iterable[, key])`

- **类型：**Python内置函数
- **描述：**返回一个可迭代或最大的两个或多个参数中的最小项
- **推荐度：**★★★
<a name="eRqU3"></a>
### 50. `range(start, stop[, step])`

- **类型：**Python内置函数
- **描述：**用于与for循环一起创建循环列表，通过指定start（开始）、stop（结束）和step（步长）控制迭代次数并获取循环值
- **推荐度：**★★★
<a name="MSfsL"></a>
### 51. `raw_input(prompt)`

- **类型：**Python内置函数
- **描述：**捕获用户输入并作为字符串返回（不推荐使用input作为用户输入的捕获函数）
- **推荐度：**★★★
<a name="hyrXY"></a>
### 52. `round(number[, ndigits])`

- **类型：**Python内置函数
- **描述：**返回number小数点后ndigits位的四舍五入的浮点数
- **推荐度：**★★★
<a name="vVLUs"></a>
### 53. `set([iterable])`

- **类型：**Python内置函数
- **描述：**返回一个新的集合对象，可选择从iterable获取的元素
- **推荐度：**★★★
<a name="DoPmc"></a>
### 54. `slice(start, stop[, step])`

- **类型：**Python内置函数
- **描述：**返回表示由范围（start、stop、step）指定的索引集的切片对象
- **推荐度：**★★
<a name="m6V1R"></a>
### 55. `sorted(iterable[, cmp[, key[, reverse]]])`

- **类型：**Python内置函数
- **描述：**从iterable的项中返回一个新的排序列表
- **推荐度：**★★★
<a name="VS5Mw"></a>
### 56. `xrange(start, stop[, step])`

- **类型：**Python内置函数
- **描述：**此函数与range()非常相似，但返回一个xrange对象而不是列表
- **推荐度：**★★★
<a name="Tlt9M"></a>
### 57. `string`

- **类型：**Python标准库
- **描述：**字符串处理库，可实现字符串查找、分割、组合、替换、去重、大小写转换及其他格式化处理
- **推荐度：**★★★
<a name="x3M8r"></a>
### 58. `re`

- **类型：**Python标准库
- **描述：**正则表达式模块，在文本和字符串处理中经常使用
- **推荐度：**★★★
<a name="uCNb3"></a>
### 59. `random`

- **类型：**Python标准库
- **描述：**该模块为各种分布实现伪随机数生成器，支持数据均匀分布、正态（高斯）分布、对数正态分布、负指数分布、伽马和β分布等
- **推荐度：**★★★
<a name="ObUyQ"></a>
### 60. `os`

- **类型：**Python标准库
- **描述：**用于新建、删除、权限修改、切换路径等目录操作，以及调用执行系统命令
- **推荐度：**★★★
<a name="q3hc1"></a>
### 61. `os.path`

- **类型：**Python标准库
- **描述：**针对目录的遍历、组合、分割、判断等操作，常用于数据文件的判断、查找、合并
- **推荐度：**★★★
<a name="er0lj"></a>
### 62. `prettytable`

- **类型：**Python标准库
- **描述：**格式化表格输出模块
- **推荐度：**★★
<a name="OAiAZ"></a>
### 63. `json`

- **类型：**Python标准库
- **描述：**Python对象与json对象的转换
- **推荐度：**★★★
<a name="q1652"></a>
### 64. `base64`

- **类型：**Python标准库
- **描述：**将任意二进制字符串编码和解码为文本字符串的Base16，Base32和Base64
- **推荐度：**★★★
<a name="O7vSZ"></a>
## 5、数据计算和统计分析
数据计算和统计分析主要用于数据探查、计算和初步数据分析等工作。
<a name="SIA14"></a>
### 65. numpy

- **类型：**第三方库
- **描述：**NumPy是Python科学计算的基础工具包，很多Python数据计算工作库都依赖它
- **推荐度：**★★★
<a name="Mple4"></a>
### 66. scipy

- **类型：**第三方库
- **描述：**Scipy是一组专门解决科学和工程计算不同场景的主题工具包
- **推荐度：**★★★
<a name="sfAws"></a>
### 67. pandas

- **类型：**第三方库
- **描述：**Pandas是一个用于Python数据分析的库，它的主要作用是进行数据分析。Pandas提供用于进行结构化数据分析的二维的表格型数据结构DataFrame，类似于R中的数据框，能提供类似于数据库中的切片、切块、聚合、选择子集等精细化操作，为数据分析提供了便捷
- **推荐度：**★★★
<a name="wdO4L"></a>
### 68. statsmodels

- **类型：**第三方库
- **描述：**Statsmodels是Python的统计建模和计量经济学工具包，包括一些描述性统计、统计模型估计和统计测试，集成了多种线性回归模型、广义线性回归模型、离散数据分布模型、时间序列分析模型、非参数估计、生存分析、主成分分析、核密度估计以及广泛的统计测试和绘图等功能
- **推荐度：**★★★
<a name="KDP5j"></a>
### 69. `abs(x)`

- **类型：**Python内置函数
- **描述：**返回x的绝对值
- **推荐度：**★★★
<a name="oh0KB"></a>
### 70. `cmp(x, y)`

- **类型：**Python内置函数
- **描述：**比较两个对象x和y，并根据结果返回一个整数。如果x <y，则返回值为负数，如果x == y则为零，如果x> y则返回值为正
- **推荐度：**★★
<a name="N5HoW"></a>
### 71. `float(x)`

- **类型：**Python内置函数
- **描述：**返回从数字或字符串x构造的浮点数
- **推荐度：**★★★
<a name="UkH9n"></a>
### 72. `pow(x, y[, z])`

- **类型：**Python内置函数
- **描述：**返回x的y次幂。如果z存在，则返回x的y次幂，模z
- **推荐度：**★★★
<a name="sJ6qW"></a>
### 73. `sum(iterable[, start])`

- **类型：**Python内置函数
- **描述：**从左到右依次迭代，返回总和
- **推荐度：**★★★
<a name="JjAn9"></a>
### 74. `math`

- **类型：**Python标准库
- **描述：**数学函数库，包括正弦、余弦、正切、余切、弧度转换、对数运算、圆周率、绝对值、取整等数学计算方法
- **推荐度：**★★★
<a name="eMqVZ"></a>
### 75. `cmath`

- **类型：**Python标准库
- **描述：**与math基本一致，区别是cmath运算的是复数
- **推荐度：**★★
<a name="Ao7BW"></a>
### 76. `decimal`

- **类型：**Python标准库
- **描述：**10进制浮点运算
- **推荐度：**★★
<a name="qCnLA"></a>
### 77. `fractions`

- **类型：**Python标准库
- **描述：**分数模块提供对有理数算术的支持
- **推荐度：**★★
<a name="TTnva"></a>
## 6、自然语言处理和文本挖掘
自然语言处理和文本挖掘库主要用于以自然语言文本为对象的数据处理和建模。
<a name="vi8Fr"></a>
### 78. nltk

- **类型：**第三方库
- **描述：**NLTK是一个Python自然语言处理工具，它用于对自然语言进行分类、解析和语义理解。目前已经有超过50种语料库和词汇资源
- **推荐度：**★★★
<a name="i5LLz"></a>
### 79. pattern

- **类型：**第三方库
- **描述：**Pattern是一个网络数据挖掘Python工具包，提供了用于网络挖掘（如网络服务、网络爬虫等）、自然语言处理（如词性标注、情感分析等）、机器学习（如向量空间模型、分类模型等）、图形化的网络分析模型
- **推荐度：**★★★
<a name="IvkrC"></a>
### 80. gensim

- **类型：**第三方库
- **描述：**Gensim是一个专业的主题模型（发掘文字中隐含主题的一种统计建模方法）Python工具包，用来提供可扩展统计语义、分析纯文本语义结构以及检索语义上相似的文档
- **推荐度：**★★★
<a name="HIBzL"></a>
### 81. 结巴分词

- **类型：**第三方库
- **描述：**结巴分词是国内流行的Python文本处理工具包，分词模式分为三种模式：精确模式、全模式和搜索引擎模式，支持繁体分词、自定义词典等，是非常好的Python中文分词解决方案，可以实现分词、词典管理、关键字抽取、词性标注等
- **推荐度：**★★★
<a name="uJmje"></a>
### 82. SnowNLP

- **类型：**第三方库
- **描述：**SnowNLP是一个Python写的类库，可以方便的处理中文文本内容。该库是受到了TextBlob的启发而针对中文处理写的类库，和TextBlob不同的是这里没有用NLTK，所有的算法都是自己实现的，并且自带了一些训练好的字典
- **推荐度：**★★
<a name="hIDYY"></a>
### 83. smallseg

- **类型：**第三方库
- **描述：**Smallseg是一个开源的、基于DFA的轻量级的中文分词工具包。可自定义词典、切割后返回登录词列表和未登录词列表、有一定的新词识别能力
- **推荐度：**★★
<a name="GjaMU"></a>
### 84. spaCy

- **类型：**第三方库
- **描述：**spaCy是一个Python自然语言处理工具包，它结合Python和Cython使得自然语言处理能力达到了工业强度
- **推荐度：**★★★
<a name="Bt2Kq"></a>
### 85. TextBlob

- **类型：**第三方库
- **描述：**TextBlob 是一个处理文本数据的Python库，可用来做词性标注、情感分析、文本翻译、名词短语抽取、文本分类等
- **推荐度：**★★
<a name="Sk43J"></a>
### 86. PyNLPI

- **类型：**第三方库
- **描述：**PyNLPI是一个适合各种自然语言处理任务的集合库，可用于中文文本分词、关键字分析等，尤其重要的是其支持中英文映射，支持UTF-8和GBK编码的字符串等
- **推荐度：**★★★
<a name="iPs22"></a>
### 87. synonyms

- **类型：**第三方库
- **描述：**中文近义词工具包，可用于自然语言理解的很多任务：文本对齐，推荐算法，相似度计算，语义偏移，关键字提取，概念提取，自动摘要，搜索引擎等。
- **推荐度：**★★★
<a name="LhcCD"></a>
## 7、图像和视频处理
图像处理和视频处理主要适用于基于图像的操作、处理、分析和挖掘，如人脸识别、图像识别、目标跟踪、图像理解等。
<a name="f5czC"></a>
### 88. PIL/Pillow

- **类型：**第三方库
- **描述：**PIL是一个常用的图像读取、处理和分析的库，提供了多种数据处理、变换的操作方法和属性。PIL仅支持到2.7版本且已经很久没有更新，一群志愿者基于PIL发布了新的分支Pillow。Pillow同时支持Python2和Python3并且加入很多新的功能
- **推荐度：**★★
<a name="kSRp8"></a>
### 89. OpenCV

- **类型：**第三方库
- **描述：**OpenCV是一个强大的图像和视频工作库。它提供了多种程序接口，支持跨平台（包括移动端）应用。OpenCV的设计效率很高，它以优化的C / C ++编写，库可以利用多核处理。除了对图像进行基本处理外，还支持图像数据建模，并预制了多种图像识别引擎，如人脸识别
- **推荐度：**★★★
<a name="QFilO"></a>
### 90. scikit-image

- **类型：**第三方库
- **描述：**scikit-image（也称skimage）是一个图像处理库，支持颜色模式转换、滤镜、绘图、图像处理、特征检测等多种功能
- **推荐度：**★★
<a name="N7i0l"></a>
### 91. imageop

- **类型：**Python标准库
- **描述：**Python自带的函数，对图像基本操作，包括裁剪、缩放、模式转换
- **推荐度：**★
<a name="yJzvi"></a>
### 92. colorsys

- **类型：**Python标准库
- **描述：**Python自带的函数，实现不同图像色彩模式的转换
- **推荐度：**★
<a name="FRCfe"></a>
### 93. imghdr

- **类型：**Python标准库
- **描述：**Python自带的函数，返回图像文件的类型
- **推荐度：**★
<a name="Tg0b2"></a>
## 8、音频处理
音频处理主要适用于基于声音的处理、分析和建模，主要应用于语音识别、语音合成、语义理解等。
<a name="fB9mz"></a>
### 94. TimeSide

- **类型：**第三方库
- **描述：**TimeSide是一个能够进行音频分析、成像、转码、流媒体和标签处理的Python框架，可以对任何音频或视频内容非常大的数据集进行复杂的处理
- **推荐度：**★★★
<a name="hVWGD"></a>
### 95. audiolazy

- **类型：**第三方库
- **描述：**audiolazy是一个用于实时声音数据流处理的库，支持实时数据应用处理、无限数据序列表示、数据流表示等
- **推荐度：**★★
<a name="ujIv6"></a>
### 96. pydub

- **类型：**第三方库
- **描述：**pydub支持多种格式声音文件，可进行多种信号处理（例如压缩、均衡、归一化）、信号生成（例如正弦、方波、锯齿等）、音效注册、静音处理等
- **推荐度：**★★★
<a name="VVhx9"></a>
### 97. audioop

- **类型：**Python标准库
- **描述：**Python自带的函数，可实现对声音片段的一些常用操作
- **推荐度：**★★
<a name="cOrhh"></a>
### 98. tinytag

- **类型：**第三方库
- **描述：**tinytag用于读取多种声音文件的元数据，涵盖MP3、OGG、OPUS、MP4、M4A、FLAC、WMA、Wave等格式
- **推荐度：**★★
<a name="mzBiQ"></a>
### 99. aifc

- **类型：**Python标准库
- **描述：**Python自带的函数，读写AIFF和AIFC文件
- **推荐度：**★
<a name="czNs2"></a>
### 100. sunau

- **类型：**Python标准库
- **描述：**Python自带的函数，读写Sun AU文件
- **推荐度：**★
<a name="q0Pxh"></a>
### 101. wave

- **类型：**Python标准库
- **描述：**Python自带的函数，读写WAV文件
- **推荐度：**★★
<a name="ZqNrN"></a>
### 102. chunk

- **类型：**Python标准库
- **描述：**Python自带的函数，读取EA IFF 85块格式的文件
- **推荐度：**★
<a name="nwmw1"></a>
### 103. sndhdr

- **类型：**Python标准库
- **描述：**Python自带的函数，返回声音文件的类型
- **推荐度：**★
<a name="qfnvj"></a>
### 104. ossaudiodev

- **类型：**Python标准库
- **描述：**该模块支持访问OSS（开放声音系统）音频接口
- **推荐度：**★★★
<a name="jDcyk"></a>
## 9、数据挖掘/机器学习/深度学习
数据挖掘、机器学习和深度学习等是Python进行数据建模和挖掘学习的核心模块。
<a name="OL61x"></a>
### 105. Scikit-Learn

- **类型：**第三方库
- **描述：**scikit-learn（也称SKlearn）是一个基于Python的机器学习综合库，内置监督式学习和非监督式学习机器学习方法，包括各种回归、聚类、分类、流式学习、异常检测、神经网络、集成方法等主流算法类别，同时支持预置数据集、数据预处理、模型选择和评估等方法，是一个非常完整、流行的机器学习工具库
- **推荐度：**★★★
<a name="XM85E"></a>
### 106. TensorFlow

- **类型：**第三方库
- **描述：**TensorFlow 是谷歌的第二代机器学习系统，内建深度学习的扩展支持，任何能够用计算流图形来表达的计算，都可以使用 TensorFlow
- **推荐度：**★★★
<a name="CXP2H"></a>
### 107. NuPIC

- **类型：**第三方库
- **描述：**NuPIC是一个以HTM（分层时间记忆）学习算法为工具的机器智能平台。NuPIC适合于各种各样的问题，尤其适用于检测异常和预测应用
- **推荐度：**★★★
<a name="wPOFg"></a>
### 108. PyTorch

- **类型：**第三方库
- **描述：**PyTorch是FaceBook推出的深度学习框架，它基于Python（而非lua）产生，它提供的动态计算图是显著区别于Tensorflow等其他学习框架的地方。
- **推荐度：**★★
<a name="cilen"></a>
### 109. Orange

- **类型：**第三方库
- **描述：**Orange通过图形化操作界面，提供交互式数据分析功能，尤其适用于分类、聚类、回归、特征选择和交叉验证工作
- **推荐度：**★★★
<a name="iO5OS"></a>
### 110. theano

- **类型：**第三方库
- **描述：**Theano是非常成熟的深度学习库。它与Numpy紧密集成，支持GPU计算、单元测试和自我验证
- **推荐度：**★★★
<a name="DN3LG"></a>
### 111. keras

- **类型：**第三方库
- **描述：**Keras是一个用Python编写的高级神经网络API，能够运行在TensorFlow或者Theano之上，它的开发重点是实现快速实验
- **推荐度：**★★
<a name="dP7KP"></a>
### 112. neurolab

- **类型：**第三方库
- **描述：**Neurolab是具有灵活网络配置和Python学习算法的基本神经网络算法库。它包含通过递归神经网络（RNN）实现的不同变体，该库是同类RNN API中最好的选择之一
- **推荐度：**★★
<a name="wczoS"></a>
### 113. PyLearn2

- **类型：**第三方库
- **描述：**PyLearn2是基于Theano的深度学习库，它旨在提供极大的灵活性，并使研究人员可以进行自由可控制，参数和属性的灵活、开放配置是亮点
- **推荐度：**★★★
<a name="jgJJh"></a>
### 114. OverFeat

- **类型：**第三方库
- **描述：**OverFeat是一个深度学习库，主要用于图片分类、定位物体检测
- **推荐度：**★★
<a name="CanA7"></a>
### 115. Pyevolve

- **类型：**第三方库
- **描述：**Pyevolve是一个完整的遗传算法框架，也支持遗传编程
- **推荐度：**★★
<a name="y2y1B"></a>
### 116. Caffe2

- **类型：**第三方库
- **描述：**Cafffe2也是FaceBook推出的深度学习框架，相比于PyTorch 更适合于研究，Caffe2 适合大规模部署，主要用于计算机视觉，它对图像识别的分类具有很好的应用效果
- **推荐度：**★★
<a name="xsd6Z"></a>
## 10、数据可视化
数据可视化主要用于做数据结果展示、数据模型验证、图形交互和探查等方面。
<a name="BpIht"></a>
### 117. Matplotlib

- **类型：**第三方库
- **描述：**Matplotlib是Python的2D绘图库，它以各种硬拷贝格式和跨平台的交互式环境生成出版质量级别的图形，开发者可以仅需要几行代码，便可以生成多种高质量图形
- **推荐度：**★★★
<a name="vSsof"></a>
### 118. pyecharts

- **类型：**第三方库
- **描述：**基于百度Echarts的强大的可视化工具库，其提供的图形功能众多，尤其对于复杂关系的展示能力较强
- **推荐度：**★★★
<a name="OzpP6"></a>
### 119. seaborn

- **类型：**第三方库
- **描述：**Seaborn是在Matplotlib的基础上进行了更高级的API封装，它可以作为Matplotlib的补充
- **推荐度：**★★★
<a name="o6B9X"></a>
### 120. bokeh

- **类型：**第三方库
- **描述：**Bokeh是一种交互式可视化库，可以在WEB浏览器中实现美观的视觉效果
- **推荐度：**★★★
<a name="lbOFd"></a>
### 121. Plotly

- **类型：**第三方库
- **描述：**Plotly提供的图形库可以进行在线WEB交互，并提供具有出版品质的图形，支持线图、散点图、区域图、条形图、误差条、框图、直方图、热图、子图、多轴、极坐标图、气泡图、玫瑰图、热力图、漏斗图等众多图形
- **推荐度：**★★★
<a name="BHSF4"></a>
### 122. VisPy

- **类型：**第三方库
- **描述：**VisPy是用于交互式科学可视化的Python库，旨在实现快速，可扩展和易于使用
- **推荐度：**★★
<a name="HEqk1"></a>
### 123. PyQtGraph

- **类型：**第三方库
- **描述：**PyQtGraph是一个建立在PyQt4 / PySide和numpy之上的纯Python图形和GUI库，主要用于数学/科学/工程应用
- **推荐度：**★★
<a name="lTZfh"></a>
### 124. ggplot

- **类型：**第三方库
- **描述：**ggplot是用Python实现的图形输出库，类似于 R中的图形展示版本
- **推荐度：**★★★
<a name="yHqKS"></a>
## 11、交互学习和集成开发
交互学习和集成开发主要用来做Python开发、调试和集成之用，包括Python集成开发环境和IDE。
<a name="iN3Ye"></a>
### 125. IPython/ Jupyter

- **类型：**第三方库
- **描述：**IPython 是一个基于Python 的交互式shell，比默认的Python shell 好用得多，支持变量自动补全、自动缩进、交互式帮助、魔法命令、系统命令等，内置了许多很有用的功能和函数。从IPython4.0开始，IPython衍生出了IPython和Jupyter两个分支。在该分支正式出现之前，IPython其实已经拥有了ipython notebook功能，因此，Jupyter更像是一个ipython notebook的升级版。
- **推荐度：**★★★
<a name="wLTwi"></a>
### 126. Elpy

- **类型：**第三方库
- **描述：**Elpy是Emacs用于Python的开发环境，它结合并配置了许多其他软件包，它们都是用Emacs Lisp和Python编写的
- **推荐度：**★★
<a name="q8LTC"></a>
### 127. PTVS

- **类型：**第三方库
- **描述：**Visual Studio 的 Python 工具
- **推荐度：**★★
<a name="NcRHF"></a>
### 128. PyCharm

- **类型：**外部工具
- **描述：**PyCharm带有一整套可以帮助用户在使用Python语言开发时提高其效率的工具，比如调试、语法高亮、项目管理、代码跳转、智能提示、自动完成、单元测试、版本控制并可集成IPython、系统终端命令行等，在PyCharm里几乎就可以实现所有有关Python工作的全部过程
- **推荐度：**★★★
<a name="dlcBP"></a>
### 129. LiClipse

- **类型：**外部工具
- **描述：**LiClipse是基于Eclipse的免费多语言 IDE，通过其中的PyDev可支持 Python开发应用
- **推荐度：**★★
<a name="KL4EF"></a>
### 130. Spyder

- **类型：**外部工具
- **描述：**Spyder是一个开源的Python IDE，由IPython和众多流行的Python库的支持，是一个具备高级编辑、交互式测试、调试以及数字计算环境的交互式开发环境
- **推荐度：**★★
<a name="ORcpq"></a>
## 12、其他Python协同数据工作工具
其他Python协同数据工作工具指除了上述主题以外，其他在数据工作中常用的工具或库。
<a name="NB797"></a>
### 131. tesseract-ocr

- **类型：**外部工具
- **描述：**这是一个Google支持的开源OCR图文识别项目，支持超过200种语言（包括中文），并支持自定义训练字符集，支持跨Windows、Linux、Mac OSX 多平台使用
- **推荐度：**★★★
<a name="y4QVH"></a>
### 132. RPython

- **类型：**第三方库
- **描述：**R集成库
- **推荐度：**★★★
<a name="qywtA"></a>
### 133. Rpy2

- **类型：**第三方库
- **描述：**Python连接R的库
<a name="TL8y4"></a>
### 134. matpython

- **类型：**第三方库
- **描述：**MATLAB集成库
- **推荐度：**★★★
<a name="mZSaW"></a>
### 135. Lunatic Python

- **类型：**第三方库
- **描述：**Lua集成库
- **推荐度：**★★
<a name="qYzDg"></a>
### 136. PyCall.jl

- **类型：**第三方库
- **描述：**Julia集成库
- **推荐度：**★★
<a name="dtrHv"></a>
### 137. PySpark

- **类型：**第三方库
- **描述：**Spark提供的Python API
- **推荐度：**★★★
<a name="PLV45"></a>
### 138. dumbo

- **类型：**第三方库
- **描述：**这个模块可以让Pythoner轻松的编写和运行 Hadoop 程序，程序版本比较早，可以作为参考
- **推荐度：**★★
<a name="hcNiK"></a>
### 139. dpark

- **类型：**第三方库
- **描述：**Python对Spark的克隆版本，类MapReduce框架
- **推荐度：**★★
<a name="G0OQI"></a>
### 140. streamparse

- **类型：**第三方库
- **描述：**Streamparse允许通过Storm对实时数据流运行Python代码
- **推荐度：**★★★
