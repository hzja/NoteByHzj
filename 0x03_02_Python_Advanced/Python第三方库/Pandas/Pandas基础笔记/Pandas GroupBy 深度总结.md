PythonPandas
<a name="S85lE"></a>
## **使用 Groupby 三个步骤**
首先要知道，任何 groupby 过程都涉及以下 3 个步骤的某种组合：

- **根据定义的标准将原始对象分成组**
- **对每个组应用某些函数**
- **整合结果**

先来大致浏览下今天用到的测试数据集
```python
import pandas as pd
import numpy as np

pd.set_option('max_columns', None)

df = pd.read_csv('complete.csv')
df = df[['awardYear', 'category', 'prizeAmount', 'prizeAmountAdjusted', 'name', 'gender', 'birth_continent']]
df.head()
```
Output:
```python
	awardYear	category	prizeAmount	prizeAmountAdjusted	name	gender	birth_continent
0	2001	Economic Sciences	10000000	12295082	A. Michael Spence	male	North America
1	1975	Physics	630000	3404179	Aage N. Bohr	male	Europe
2	2004	Chemistry	10000000	11762861	Aaron Ciechanover	male	Asia
3	1982	Chemistry	1150000	3102518	Aaron Klug	male	Europe
4	1979	Physics	800000	2988048	Abdus Salam	male	Asia
```
<a name="N8a47"></a>
## **将原始对象拆分为组**
在这个阶段，调用 pandas `DataFrame.groupby()` 函数。使用它根据预定义的标准将数据分组，沿行（默认情况下，axis=0）或列（axis=1）。换句话说，此函数将标签映射到组的名称。<br />例如，在案例中，可以按奖项类别对诺贝尔奖的数据进行分组：
```python
grouped = df.groupby('category')
```
也可以使用多个列来执行数据分组，传递一个列列表即可。首先按奖项类别对数据进行分组，然后在每个创建的组中，将根据获奖年份应用额外的分组：
```python
grouped_category_year = df.groupby(['category', 'awardYear'])
```
现在，如果尝试打印刚刚创建的两个 GroupBy 对象之一，实际上将看不到任何组：
```python
print(grouped)
```
Output:
```python
<pandas.core.groupby.generic.DataFrameGroupBy object at 0x0000026083789DF0>
```
要注意的是，创建 GroupBy 对象成功与否，只检查是否通过了正确的映射；再显式地对该对象使用某些方法或提取其某些属性之前，都不会真正执行拆分-应用-组合链的任何操作<br />为了简要检查生成的 GroupBy 对象并检查组的拆分方式，可以从中提取组或索引属性。它们都返回一个字典，其中键是创建的组，值是原始 DataFrame 中每个组的实例的轴标签列表（对于组属性）或索引（对于索引属性）：
```python
grouped.indices
```
Output:
```python
{'Chemistry': array([  2,   3,   7,   9,  10,  11,  13,  14,  15,  17,  19,  39,  62,
         64,  66,  71,  75,  80,  81,  86,  92, 104, 107, 112, 129, 135,
        153, 169, 175, 178, 181, 188, 197, 199, 203, 210, 215, 223, 227,
        239, 247, 249, 258, 264, 265, 268, 272, 274, 280, 282, 284, 289,
        296, 298, 310, 311, 317, 318, 337, 341, 343, 348, 352, 357, 362,
        365, 366, 372, 374, 384, 394, 395, 396, 415, 416, 419, 434, 440,
        442, 444, 446, 448, 450, 455, 456, 459, 461, 463, 465, 469, 475,
        504, 505, 508, 518, 522, 523, 524, 539, 549, 558, 559, 563, 567,
        571, 572, 585, 591, 596, 599, 627, 630, 632, 641, 643, 644, 648,
        659, 661, 666, 667, 668, 671, 673, 679, 681, 686, 713, 715, 717,
        719, 720, 722, 723, 725, 726, 729, 732, 738, 742, 744, 746, 751,
        756, 759, 763, 766, 773, 776, 798, 810, 813, 814, 817, 827, 828,
        829, 832, 839, 848, 853, 855, 862, 866, 880, 885, 886, 888, 889,
        892, 894, 897, 902, 904, 914, 915, 920, 921, 922, 940, 941, 943,
        946, 947], dtype=int64),
 'Economic Sciences': array([  0,   5,  45,  46,  58,  90,  96, 139, 140, 145, 152, 156, 157,
        180, 187, 193, 207, 219, 231, 232, 246, 250, 269, 279, 283, 295,
        305, 324, 346, 369, 418, 422, 425, 426, 430, 432, 438, 458, 467,
        476, 485, 510, 525, 527, 537, 538, 546, 580, 594, 595, 605, 611,
        636, 637, 657, 669, 670, 678, 700, 708, 716, 724, 734, 737, 739,
        745, 747, 749, 750, 753, 758, 767, 800, 805, 854, 856, 860, 864,
        871, 882, 896, 912, 916, 924], dtype=int64),
 'Literature': array([ 21,  31,  40,  49,  52,  98, 100, 101, 102, 111, 115, 142, 149,
        159, 170, 177, 201, 202, 220, 221, 233, 235, 237, 253, 257, 259,
        275, 277, 278, 286, 312, 315, 316, 321, 326, 333, 345, 347, 350,
        355, 359, 364, 370, 373, 385, 397, 400, 403, 406, 411, 435, 439,
        441, 454, 468, 479, 480, 482, 483, 492, 501, 506, 511, 516, 556,
        569, 581, 602, 604, 606, 613, 614, 618, 631, 633, 635, 640, 652,
        653, 655, 656, 665, 675, 683, 699, 761, 765, 771, 774, 777, 779,
        780, 784, 786, 788, 796, 799, 803, 836, 840, 842, 850, 861, 867,
        868, 878, 881, 883, 910, 917, 919, 927, 928, 929, 930, 936],
       dtype=int64),
 'Peace': array([  6,  12,  16,  25,  26,  27,  34,  36,  44,  47,  48,  54,  61,
         65,  72,  78,  79,  82,  95,  99, 116, 119, 120, 126, 137, 146,
        151, 166, 167, 171, 200, 204, 205, 206, 209, 213, 225, 236, 240,
        244, 255, 260, 266, 267, 270, 287, 303, 320, 329, 356, 360, 361,
        377, 386, 387, 388, 389, 390, 391, 392, 393, 433, 447, 449, 471,
        477, 481, 489, 491, 500, 512, 514, 517, 528, 529, 530, 533, 534,
        540, 542, 544, 545, 547, 553, 555, 560, 562, 574, 578, 590, 593,
        603, 607, 608, 609, 612, 615, 616, 617, 619, 620, 628, 634, 639,
        642, 664, 677, 688, 697, 703, 705, 710, 727, 736, 787, 793, 795,
        806, 823, 846, 847, 852, 865, 875, 876, 877, 895, 926, 934, 935,
        937, 944, 948, 949], dtype=int64),
 'Physics': array([  1,   4,   8,  20,  23,  24,  30,  32,  38,  51,  59,  60,  67,
         68,  69,  70,  74,  84,  89,  97, 103, 105, 108, 109, 114, 117,
        118, 122, 125, 127, 128, 130, 133, 141, 143, 144, 155, 162, 163,
        164, 165, 168, 173, 174, 176, 179, 183, 195, 212, 214, 216, 222,
        224, 228, 230, 234, 238, 241, 243, 251, 256, 263, 271, 276, 291,
        292, 297, 301, 306, 307, 308, 323, 327, 328, 330, 335, 336, 338,
        349, 351, 353, 354, 363, 367, 375, 376, 378, 381, 382, 398, 399,
        402, 404, 405, 408, 410, 412, 413, 420, 421, 424, 428, 429, 436,
        445, 451, 453, 457, 460, 462, 470, 472, 487, 495, 498, 499, 509,
        513, 515, 521, 526, 532, 535, 536, 541, 548, 550, 552, 557, 561,
        564, 565, 566, 573, 576, 577, 579, 583, 586, 588, 592, 601, 610,
        621, 622, 623, 629, 647, 650, 651, 654, 658, 674, 676, 682, 684,
        690, 691, 693, 694, 695, 696, 698, 702, 707, 711, 714, 721, 730,
        731, 735, 743, 752, 755, 770, 772, 775, 781, 785, 790, 792, 797,
        801, 802, 808, 822, 833, 834, 835, 844, 851, 870, 872, 879, 884,
        887, 890, 893, 900, 901, 903, 905, 907, 908, 909, 913, 925, 931,
        932, 933, 938, 942, 945], dtype=int64),
 'Physiology or Medicine': array([ 18,  22,  28,  29,  33,  35,  37,  41,  42,  43,  50,  53,  55,
         56,  57,  63,  73,  76,  77,  83,  85,  87,  88,  91,  93,  94,
        106, 110, 113, 121, 123, 124, 131, 132, 134, 136, 138, 147, 148,
        150, 154, 158, 160, 161, 172, 182, 184, 185, 186, 189, 190, 191,
        192, 194, 196, 198, 208, 211, 217, 218, 226, 229, 242, 245, 248,
        252, 254, 261, 262, 273, 281, 285, 288, 290, 293, 294, 299, 300,
        302, 304, 309, 313, 314, 319, 322, 325, 331, 332, 334, 339, 340,
        342, 344, 358, 368, 371, 379, 380, 383, 401, 407, 409, 414, 417,
        423, 427, 431, 437, 443, 452, 464, 466, 473, 474, 478, 484, 486,
        488, 490, 493, 494, 496, 497, 502, 503, 507, 519, 520, 531, 543,
        551, 554, 568, 570, 575, 582, 584, 587, 589, 597, 598, 600, 624,
        625, 626, 638, 645, 646, 649, 660, 662, 663, 672, 680, 685, 687,
        689, 692, 701, 704, 706, 709, 712, 718, 728, 733, 740, 741, 748,
        754, 757, 760, 762, 764, 768, 769, 778, 782, 783, 789, 791, 794,
        804, 807, 809, 811, 812, 815, 816, 818, 819, 820, 821, 824, 825,
        826, 830, 831, 837, 838, 841, 843, 845, 849, 857, 858, 859, 863,
        869, 873, 874, 891, 898, 899, 906, 911, 918, 923, 939], dtype=int64)}
```
要查找 GroupBy 对象中的组数，可以从中提取 ngroups 属性或调用 Python 标准库的 len 函数：
```python
print(grouped.ngroups)
print(len(grouped))
```
Output:
```python
6
6
```
如果需要可视化每个组的所有或部分条目，那么可以遍历 GroupBy 对象：
```python
for name, entries in grouped:
    print(f'First 2 entries for the "{name}" category:')
    print(30*'-')
    print(entries.head(2), '\n\n')
```
Output:
```python
First 2 entries for the "Chemistry" category:
------------------------------
   awardYear   category  prizeAmount  prizeAmountAdjusted               name  \
2       2004  Chemistry     10000000             11762861  Aaron Ciechanover   
3       1982  Chemistry      1150000              3102518         Aaron Klug   

  gender birth_continent  
2   male            Asia  
3   male          Europe   

First 2 entries for the "Economic Sciences" category:
------------------------------
   awardYear           category  prizeAmount  prizeAmountAdjusted  \
0       2001  Economic Sciences     10000000             12295082   
5       2019  Economic Sciences      9000000              9000000   

                name gender birth_continent  
0  A. Michael Spence   male   North America  
5   Abhijit Banerjee   male            Asia   

First 2 entries for the "Literature" category:
------------------------------
    awardYear    category  prizeAmount  prizeAmountAdjusted  \
21       1957  Literature       208629              2697789   
31       1970  Literature       400000              3177966   

                     name gender birth_continent  
21           Albert Camus   male          Africa  
31  Alexandr Solzhenitsyn   male          Europe   

First 2 entries for the "Peace" category:
------------------------------
    awardYear category  prizeAmount  prizeAmountAdjusted  \
6        2019    Peace      9000000              9000000   
12       1980    Peace       880000              2889667   

                     name gender birth_continent  
6          Abiy Ahmed Ali   male          Africa  
12  Adolfo Pérez Esquivel   male   South America   

First 2 entries for the "Physics" category:
------------------------------
   awardYear category  prizeAmount  prizeAmountAdjusted          name gender  \
1       1975  Physics       630000              3404179  Aage N. Bohr   male   
4       1979  Physics       800000              2988048   Abdus Salam   male   

  birth_continent  
1          Europe  
4            Asia   

First 2 entries for the "Physiology or Medicine" category:
------------------------------
    awardYear                category  prizeAmount  prizeAmountAdjusted  \
18       1963  Physiology or Medicine       265000              2839286   
22       1974  Physiology or Medicine       550000              3263449   

             name gender birth_continent  
18   Alan Hodgkin   male          Europe  
22  Albert Claude   male          Europe
```
相反，如果想以 DataFrame 的形式选择单个组，应该在 GroupBy 对象上使用 `get_group()` 方法：
```python
grouped.get_group('Economic Sciences')
```
Output:
```python
	awardYear	category	prizeAmount	prizeAmountAdjusted	name	gender	birth_continent
0	2001	Economic Sciences	10000000	12295082	A. Michael Spence	male	North America
5	2019	Economic Sciences	9000000	9000000	Abhijit Banerjee	male	Asia
45	2012	Economic Sciences	8000000	8361204	Alvin E. Roth	male	North America
46	1998	Economic Sciences	7600000	9713701	Amartya Sen	male	Asia
58	2015	Economic Sciences	8000000	8384572	Angus Deaton	male	Europe
…	…	…	…	…	…	…	…
882	2002	Economic Sciences	10000000	12034660	Vernon L. Smith	male	North America
896	1973	Economic Sciences	510000	3331882	Wassily Leontief	male	Europe
912	2018	Economic Sciences	9000000	9000000	William D. Nordhaus	male	North America
916	1990	Economic Sciences	4000000	6329114	William F. Sharpe	male	North America
924	1996	Economic Sciences	7400000	9490424	William Vickrey	male	North America
```
<a name="nb0Ou"></a>
## **按组应用函数**
在拆分原始数据并检查结果组之后，可以对每个组执行以下操作之一或其组合：

- **Aggregation（聚合）：计算每个组的汇总统计量（例如，组大小、平均值、中位数或总和）并为许多数据点输出单个数字**
- **Transformation（变换）：按组进行一些操作，例如计算每个组的z-score**
- **Filtration（过滤）：根据预定义的条件拒绝某些组，例如组大小、平均值、中位数或总和，还可以包括从每个组中过滤掉特定的行**
<a name="Xar8K"></a>
### **Aggregation**
要聚合 GroupBy 对象的数据（即按组计算汇总统计量），可以在对象上使用 `agg()` 方法：
```python
# Showing only 1 decimal for all float numbers
pd.options.display.float_format = '{:.1f}'.format

grouped.agg(np.mean)
```
Output:
```python
	awardYear	prizeAmount	prizeAmountAdjusted
category			
Chemistry	1972.3	3629279.4	6257868.1
Economic Sciences	1996.1	6105845.2	7837779.2
Literature	1960.9	2493811.2	5598256.3
Peace	1964.5	3124879.2	6163906.9
Physics	1971.1	3407938.6	6086978.2
Physiology or Medicine	1970.4	3072972.9	5738300.7
```
上面的代码生成一个 DataFrame，其中组名作为其新索引，每个数字列的平均值作为分组<br />可以直接在 GroupBy 对象上应用其他相应的 Pandas 方法，而不仅仅是使用 `agg()` 方法。最常用的方法是 `mean()`、`median()`、`mode()`、`sum()`、`size()`、`count()`、`min()`、`max()`、`std()`、`var()`（计算每个的方差 group)、`describe()`（按组输出描述性统计信息）和 `nunique()`（给出每个组中唯一值的数量）
```python
grouped.sum()
```
Output:
```python
	awardYear	prizeAmount	prizeAmountAdjusted
category			
Chemistry	362912	667787418	1151447726
Economic Sciences	167674	512891000	658373449
Literature	227468	289282102	649397731
Peace	263248	418733807	825963521
Physics	419837	725890928	1296526352
Physiology or Medicine	431508	672981066	1256687857
```
通常情况下只对某些特定列或列的统计信息感兴趣，因此需要指定它们。在上面的例子中，绝对不想总结所有年份，相应的可能希望按奖品类别对奖品价值求和。为此可以选择 GroupBy 对象的 PrizeAmountAdjusted 列，就像选择 DataFrame 的列，然后对其应用 `sum()` 函数：
```python
grouped['prizeAmountAdjusted'].sum()
```
Output:
```python
category
Chemistry                 1151447726
Economic Sciences          658373449
Literature                 649397731
Peace                      825963521
Physics                   1296526352
Physiology or Medicine    1256687857
Name: prizeAmountAdjusted, dtype: int64
```
对于上面的代码片段，可以在选择必要的列之前使用对 GroupBy 对象应用函数的等效语法：`grouped.sum()['prizeAmountAdjusted']`。但是前面的语法更可取，因为它的性能更好，尤其是在大型数据集上，效果更为明显<br />如果需要聚合两列或更多列的数据，使用双方括号：
```python
grouped[['prizeAmount', 'prizeAmountAdjusted']].sum()
```
Output:
```python
	prizeAmount	prizeAmountAdjusted
category		
Chemistry	667787418	1151447726
Economic Sciences	512891000	658373449
Literature	289282102	649397731
Peace	418733807	825963521
Physics	725890928	1296526352
Physiology or Medicine	672981066	1256687857
```
可以一次将多个函数应用于 GroupBy 对象的一列或多列。为此再次需要 `agg()` 方法和感兴趣的函数列表：
```python
grouped[['prizeAmount', 'prizeAmountAdjusted']].agg([np.sum, np.mean, np.std])
```
Output:
```python
	prizeAmount	prizeAmountAdjusted
sum	mean	std	sum	mean	std
category						
Chemistry	667787418	3629279.4	4070588.4	1151447726	6257868.1	3276027.2
Economic Sciences	512891000	6105845.2	3787630.1	658373449	7837779.2	3313153.2
Literature	289282102	2493811.2	3653734.0	649397731	5598256.3	3029512.1
Peace	418733807	3124879.2	3934390.9	825963521	6163906.9	3189886.1
Physics	725890928	3407938.6	4013073.0	1296526352	6086978.2	3294268.5
Physiology or Medicine	672981066	3072972.9	3898539.3	1256687857	5738300.7	3241781.0
```
此外，可以考虑通过传递字典将不同的聚合函数应用于 GroupBy 对象的不同列：
```python
grouped.agg({'prizeAmount': [np.sum, np.size], 'prizeAmountAdjusted': np.mean})
```
Output:
```python
	prizeAmount	prizeAmountAdjusted
sum	size	mean
category			
Chemistry	667787418	184	6257868.1
Economic Sciences	512891000	84	7837779.2
Literature	289282102	116	5598256.3
Peace	418733807	134	6163906.9
Physics	725890928	213	6086978.2
Physiology or Medicine	672981066	219	5738300.7
```
<a name="T3K7v"></a>
### **Transformation**
与聚合方法不同，转换方法返回一个新的 DataFrame，其形状和索引与原始 DataFrame 相同，但具有转换后的各个值。这里需要注意的是，transformation 一定不能修改原始 DataFrame 中的任何值，也就是这些操作不能原地执行<br />转换 GroupBy 对象数据的最常见的 Pandas 方法是 `transform()`。例如它可以帮助计算每个组的 z-score：
```python
grouped[['prizeAmount', 'prizeAmountAdjusted']].transform(lambda x: (x - x.mean()) / x.std())
```
Output:
```python
	prizeAmount	prizeAmountAdjusted
0	1.0	1.3
1	-0.7	-0.8
2	1.6	1.7
3	-0.6	-1.0
4	-0.6	-0.9
…	…	…
945	-0.7	-0.8
946	-0.8	-1.1
947	-0.9	0.3
948	-0.5	-1.0
949	-0.7	-1.0
```
使用转换方法，还可以用组均值、中位数、众数或任何其他值替换缺失数据：
```python
grouped['gender'].transform(lambda x: x.fillna(x.mode()[0]))
```
Output:
```python
0        male
1        male
2        male
3        male
4        male
        ...  
945      male
946      male
947    female
948      male
949      male
Name: gender, Length: 950, dtype: object
```
当然还可以使用其他一些 Pandas 方法来转换 GroupBy 对象的数据：`bfill()`、`ffill()`、`diff()`、`pct_change()`、`rank()`、`shift()`、`quantile()`等
<a name="X0xt5"></a>
### **Filtration**
过滤方法根据预定义的条件从每个组中丢弃组或特定行，并返回原始数据的子集。例如希望只保留所有组中某个列的值，其中该列的组均值大于预定义值。在 DataFrame 的情况下，过滤掉所有组均值小于 7,000,000 的prizeAmountAdjusted 列，并在输出中仅保留该列：
```python
grouped['prizeAmountAdjusted'].filter(lambda x: x.mean() > 7000000)
```
Output:
```python
0      12295082
5       9000000
45      8361204
46      9713701
58      8384572
         ...   
882    12034660
896     3331882
912     9000000
916     6329114
924     9490424
Name: prizeAmountAdjusted, Length: 84, dtype: int64
```
另一个例子是过滤掉具有超过一定数量元素的组：
```python
grouped['prizeAmountAdjusted'].filter(lambda x: len(x) < 100)
```
Output:
```python
0      12295082
5       9000000
45      8361204
46      9713701
58      8384572
         ...   
882    12034660
896     3331882
912     9000000
916     6329114
924     9490424
Name: prizeAmountAdjusted, Length: 84, dtype: int64
```
在上述两个操作中，使用了 `filter()` 方法，将 lambda 函数作为参数传递。这样的函数，应用于整个组，根据该组与预定义统计条件的比较结果返回 `True` 或 `False`。换句话说，`filter()`方法中的函数决定了哪些组保留在新的 DataFrame 中<br />除了过滤掉整个组之外，还可以从每个组中丢弃某些行。这里有一些有用的方法是 `first()`、`last()` 和 `nth()`。将其中一个应用于 GroupBy 对象会相应地返回每个组的第一个/最后一个/第 n 个条目：
```python
grouped.last()
```
Output:
```python
	awardYear	prizeAmount	prizeAmountAdjusted	name	gender	birth_continent
category						
Chemistry	1911	140695	7327865	Marie Curie	female	Europe
Economic Sciences	1996	7400000	9490424	William Vickrey	male	North America
Literature	1968	350000	3052326	Yasunari Kawabata	male	Asia
Peace	1963	265000	2839286	International Committee of the Red Cross	male	Asia
Physics	1972	480000	3345725	John Bardeen	male	North America
Physiology or Medicine	2016	8000000	8301051	Yoshinori Ohsumi	male	Asia
```
对于 `nth()` 方法，必须传递表示要为每个组返回的条目索引的整数：
```python
grouped.nth(1)
```
Output:
```python
	awardYear	prizeAmount	prizeAmountAdjusted	name	gender	birth_continent
category						
Chemistry	1982	1150000	3102518	Aaron Klug	male	Europe
Economic Sciences	2019	9000000	9000000	Abhijit Banerjee	male	Asia
Literature	1970	400000	3177966	Alexandr Solzhenitsyn	male	Europe
Peace	1980	880000	2889667	Adolfo Pérez Esquivel	male	South America
Physics	1979	800000	2988048	Abdus Salam	male	Asia
Physiology or Medicine	1974	550000	3263449	Albert Claude	male	Europe
```
上面的代码收集了所有组的第二个条目<br />另外两个过滤每个组中的行的方法是 `head()` 和 `tail()`，分别返回每个组的第一/最后 n 行（默认为 5）：
```python
grouped.head(3)
```
Output:
```python
	awardYear	category	prizeAmount	prizeAmountAdjusted	name	gender	birth_continent
0	2001	Economic Sciences	10000000	12295082	A. Michael Spence	male	North America
1	1975	Physics	630000	3404179	Aage N. Bohr	male	Europe
2	2004	Chemistry	10000000	11762861	Aaron Ciechanover	male	Asia
3	1982	Chemistry	1150000	3102518	Aaron Klug	male	Europe
4	1979	Physics	800000	2988048	Abdus Salam	male	Asia
5	2019	Economic Sciences	9000000	9000000	Abhijit Banerjee	male	Asia
6	2019	Peace	9000000	9000000	Abiy Ahmed Ali	male	Africa
7	2009	Chemistry	10000000	10958504	Ada E. Yonath	female	Asia
8	2011	Physics	10000000	10545557	Adam G. Riess	male	North America
12	1980	Peace	880000	2889667	Adolfo Pérez Esquivel	male	South America
16	2007	Peace	10000000	11301989	Al Gore	male	North America
18	1963	Physiology or Medicine	265000	2839286	Alan Hodgkin	male	Europe
21	1957	Literature	208629	2697789	Albert Camus	male	Africa
22	1974	Physiology or Medicine	550000	3263449	Albert Claude	male	Europe
28	1937	Physiology or Medicine	158463	4716161	Albert Szent-Györgyi	male	Europe
31	1970	Literature	400000	3177966	Alexandr Solzhenitsyn	male	Europe
40	2013	Literature	8000000	8365867	Alice Munro	female	North America
45	2012	Economic Sciences	8000000	8361204	Alvin E. Roth	male	North America
```
<a name="qVp87"></a>
### **整合结果**
split-apply-combine 链的最后一个阶段——合并结果——由Ppandas 在后台执行。它包括获取在 GroupBy 对象上执行的所有操作的输出并将它们重新组合在一起，生成新的数据结构，例如 Series 或 DataFrame。将此数据结构分配给一个变量，可以用它来解决其他任务
<a name="Ry5uO"></a>
## **总结**
介绍了使用 pandas groupby 函数和使用结果对象的许多知识

- **分组过程所包括的步骤**
- **split-apply-combine 链是如何一步一步工作的**
- **如何创建 GroupBy 对象**
- **如何简要检查 GroupBy 对象**
- **GroupBy 对象的属性**
- **可应用于 GroupBy 对象的操作**
- **如何按组计算汇总统计量以及可用于此目的的方法**
- **如何一次将多个函数应用于 GroupBy 对象的一列或多列**
- **如何将不同的聚合函数应用于 GroupBy 对象的不同列**
- **如何以及为什么要转换原始 DataFrame 中的值**
- **如何过滤 GroupBy 对象的组或每个组的特定行**
- **Pandas 如何组合分组过程的结果**
- **分组过程产生的数据结构**
