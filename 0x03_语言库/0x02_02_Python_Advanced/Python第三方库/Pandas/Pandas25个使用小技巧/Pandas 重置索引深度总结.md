PythonPandas<br />来讨论 Pandas 中的 `reset_index()` 方法，包括为什么需要在 Pandas 中重置 DataFrame 的索引，以及应该如何应用该方法<br />使用 Kaggle 上的数据集样本 Animal Shelter Analytics 来作为测试数据
<a name="H211U"></a>
## Pandas 中的 Reset_Index() 是什么？
如果使用 Pandas 的 `read_csv()` 方法读取 csv 文件而不指定任何索引，则生成的 DataFrame 将具有默认的基于整数的索引，第一行从 0 开始，随后每行增加 1：
```python
import pandas as pd
import numpy as np

df = pd.read_csv('Austin_Animal_Center_Intakes.csv').head()
df
```
Output:
```python
	Animal ID	Name	DateTime	MonthYear	Found Location	Intake Type	Intake Condition	Animal Type	Sex upon Intake	Age upon Intake	Breed	Color
0	A786884	*Brock	01/03/2019 04:19:00 PM	01/03/2019 04:19:00 PM	2501 Magin Meadow Dr in Austin (TX)	Stray	Normal	Dog	Neutered Male	2 years	Beagle Mix	Tricolor
1	A706918	Belle	07/05/2015 12:59:00 PM	07/05/2015 12:59:00 PM	9409 Bluegrass Dr in Austin (TX)	Stray	Normal	Dog	Spayed Female	8 years	English Springer Spaniel	White/Liver
2	A724273	Runster	04/14/2016 06:43:00 PM	04/14/2016 06:43:00 PM	2818 Palomino Trail in Austin (TX)	Stray	Normal	Dog	Intact Male	11 months	Basenji Mix	Sable/White
3	A665644	NaN	10/21/2013 07:59:00 AM	10/21/2013 07:59:00 AM	Austin (TX)	Stray	Sick	Cat	Intact Female	4 weeks	Domestic Shorthair Mix	Calico
4	A682524	Rio	06/29/2014 10:38:00 AM	06/29/2014 10:38:00 AM	800 Grove Blvd in Austin (TX)	Stray	Normal	Dog	Neutered Male	4 years	Doberman Pinsch/Australian Cattle Dog	Tan/Gray
```
在某些情况下，可能希望拥有更有意义的行标签，因此将选择 DataFrame 的其中一列作为 DataFrame 索引。可以使用 `read_csv()` 方法的 index_col 参数直接执行此操作：
```python
df = pd.read_csv('Austin_Animal_Center_Intakes.csv', index_col='Animal ID').head()
df
```
Output:
```python
Animal ID	Name	DateTime	MonthYear	Found Location	Intake Type	Intake Condition	Animal Type	Sex upon Intake	Age upon Intake	Breed	Color
A786884	*Brock	01/03/2019 04:19:00 PM	01/03/2019 04:19:00 PM	2501 Magin Meadow Dr in Austin (TX)	Stray	Normal	Dog	Neutered Male	2 years	Beagle Mix	Tricolor
A706918	Belle	07/05/2015 12:59:00 PM	07/05/2015 12:59:00 PM	9409 Bluegrass Dr in Austin (TX)	Stray	Normal	Dog	Spayed Female	8 years	English Springer Spaniel	White/Liver
A724273	Runster	04/14/2016 06:43:00 PM	04/14/2016 06:43:00 PM	2818 Palomino Trail in Austin (TX)	Stray	Normal	Dog	Intact Male	11 months	Basenji Mix	Sable/White
A665644	NaN	10/21/2013 07:59:00 AM	10/21/2013 07:59:00 AM	Austin (TX)	Stray	Sick	Cat	Intact Female	4 weeks	Domestic Shorthair Mix	Calico
A682524	Rio	06/29/2014 10:38:00 AM	06/29/2014 10:38:00 AM	800 Grove Blvd in Austin (TX)	Stray	Normal	Dog	Neutered Male	4 years	Doberman Pinsch/Australian Cattle Dog	Tan/Gray
```
或者还可以使用 `set_index()` 方法将 DataFrame 的任何列设置为 DataFrame 索引：
```python
df = pd.read_csv('Austin_Animal_Center_Intakes.csv').head()
df.set_index('Animal ID', inplace=True)
df
```
Output:
```python
Animal ID	Name	DateTime	MonthYear	Found Location	Intake Type	Intake Condition	Animal Type	Sex upon Intake	Age upon Intake	Breed	Color
A786884	*Brock	01/03/2019 04:19:00 PM	01/03/2019 04:19:00 PM	2501 Magin Meadow Dr in Austin (TX)	Stray	Normal	Dog	Neutered Male	2 years	Beagle Mix	Tricolor
A706918	Belle	07/05/2015 12:59:00 PM	07/05/2015 12:59:00 PM	9409 Bluegrass Dr in Austin (TX)	Stray	Normal	Dog	Spayed Female	8 years	English Springer Spaniel	White/Liver
A724273	Runster	04/14/2016 06:43:00 PM	04/14/2016 06:43:00 PM	2818 Palomino Trail in Austin (TX)	Stray	Normal	Dog	Intact Male	11 months	Basenji Mix	Sable/White
A665644	NaN	10/21/2013 07:59:00 AM	10/21/2013 07:59:00 AM	Austin (TX)	Stray	Sick	Cat	Intact Female	4 weeks	Domestic Shorthair Mix	Calico
A682524	Rio	06/29/2014 10:38:00 AM	06/29/2014 10:38:00 AM	800 Grove Blvd in Austin (TX)	Stray	Normal	Dog	Neutered Male	4 years	Doberman Pinsch/Australian Cattle Dog	Tan/Gray
```
如果在某个时候需要恢复默认的数字索引呢，这时就可以使用 `reset_index()`函数了
```python
df.reset_index()
```
Output:
```python
	Animal ID	Name	DateTime	MonthYear	Found Location	Intake Type	Intake Condition	Animal Type	Sex upon Intake	Age upon Intake	Breed	Color
0	A786884	*Brock	01/03/2019 04:19:00 PM	01/03/2019 04:19:00 PM	2501 Magin Meadow Dr in Austin (TX)	Stray	Normal	Dog	Neutered Male	2 years	Beagle Mix	Tricolor
1	A706918	Belle	07/05/2015 12:59:00 PM	07/05/2015 12:59:00 PM	9409 Bluegrass Dr in Austin (TX)	Stray	Normal	Dog	Spayed Female	8 years	English Springer Spaniel	White/Liver
2	A724273	Runster	04/14/2016 06:43:00 PM	04/14/2016 06:43:00 PM	2818 Palomino Trail in Austin (TX)	Stray	Normal	Dog	Intact Male	11 months	Basenji Mix	Sable/White
3	A665644	NaN	10/21/2013 07:59:00 AM	10/21/2013 07:59:00 AM	Austin (TX)	Stray	Sick	Cat	Intact Female	4 weeks	Domestic Shorthair Mix	Calico
4	A682524	Rio	06/29/2014 10:38:00 AM	06/29/2014 10:38:00 AM	800 Grove Blvd in Austin (TX)	Stray	Normal	Dog	Neutered Male	4 years	Doberman Pinsch/Australian Cattle Dog	Tan/Gray
```
此方法的默认行为包括用默认的基于整数的索引替换现有的 DataFrame 索引，并将旧索引转换为与旧索引同名的新列（或名称索引）。此外，默认情况下，`reset_index()` 方法会从 MultiIndex 中删除所有级别并且不会影响原始 DataFrame 数据，而是创建一个新的
<a name="M3CyJ"></a>
## 何时使用 `Reset_Index()` 方法
`reset_index()` 方法将 DataFrame 索引重置为默认数字索引，在以下情况下特别有用：

- 执行数据整理时——尤其是过滤数据或删除缺失值等预处理操作，会导致较小的 DataFrame 具有不再连续的数字索引
- 当索引应该被视为一个常见的 DataFrame 列时
- 当索引标签没有提供有关数据的任何有价值的信息时
<a name="LspQg"></a>
## 如何调整 `Reset_Index()` 方法
前面的讨论中，看到了不向它传递任何参数时，`reset_index()` 方法是如何工作的，当然如果有需要，可以通过调整方法的各种参数来更改此默认行为。看看最有用的三种参数：level、drop 和 inplace
<a name="PVoQ9"></a>
### level
此参数采用整数、字符串、元组或列表作为可能的数据类型，并且仅适用于具有 MultiIndex 的 DataFrame，如下所示：
```python
df_multiindex = pd.read_csv('Austin_Animal_Center_Intakes.csv', index_col=['Animal ID', 'Name']).head()
df_multiindex
```
Output:
```python
Animal ID	Name	DateTime	MonthYear	Found Location	Intake Type	Intake Condition	Animal Type	Sex upon Intake	Age upon Intake	Breed	Color
A786884	*Brock	01/03/2019 04:19:00 PM	01/03/2019 04:19:00 PM	2501 Magin Meadow Dr in Austin (TX)	Stray	Normal	Dog	Neutered Male	2 years	Beagle Mix	Tricolor
A706918	Belle	07/05/2015 12:59:00 PM	07/05/2015 12:59:00 PM	9409 Bluegrass Dr in Austin (TX)	Stray	Normal	Dog	Spayed Female	8 years	English Springer Spaniel	White/Liver
A724273	Runster	04/14/2016 06:43:00 PM	04/14/2016 06:43:00 PM	2818 Palomino Trail in Austin (TX)	Stray	Normal	Dog	Intact Male	11 months	Basenji Mix	Sable/White
A665644	NaN	10/21/2013 07:59:00 AM	10/21/2013 07:59:00 AM	Austin (TX)	Stray	Sick	Cat	Intact Female	4 weeks	Domestic Shorthair Mix	Calico
A682524	Rio	06/29/2014 10:38:00 AM	06/29/2014 10:38:00 AM	800 Grove Blvd in Austin (TX)	Stray	Normal	Dog	Neutered Male	4 years	Doberman Pinsch/Australian Cattle Dog	Tan/Gray
```
事实上，如果现在检查上面 DataFrame 的索引，会发现它不是一个常见的 DataFrame 索引，而是一个 MultiIndex 对象：
```python
df_multiindex.index
```
Output:
```python
MultiIndex([('A786884',  '*Brock'),
            ('A706918',   'Belle'),
            ('A724273', 'Runster'),
            ('A665644',       nan),
            ('A682524',     'Rio')],
           names=['Animal ID', 'Name'])
```
默认情况下，`reset_index()` 方法参数 level (level=None) 会移除 MultiIndex 的所有级别：
```python
df_multiindex.reset_index()
```
Output:
```python
	Animal ID	Name	DateTime	MonthYear	Found Location	Intake Type	Intake Condition	Animal Type	Sex upon Intake	Age upon Intake	Breed	Color
0	A786884	*Brock	01/03/2019 04:19:00 PM	01/03/2019 04:19:00 PM	2501 Magin Meadow Dr in Austin (TX)	Stray	Normal	Dog	Neutered Male	2 years	Beagle Mix	Tricolor
1	A706918	Belle	07/05/2015 12:59:00 PM	07/05/2015 12:59:00 PM	9409 Bluegrass Dr in Austin (TX)	Stray	Normal	Dog	Spayed Female	8 years	English Springer Spaniel	White/Liver
2	A724273	Runster	04/14/2016 06:43:00 PM	04/14/2016 06:43:00 PM	2818 Palomino Trail in Austin (TX)	Stray	Normal	Dog	Intact Male	11 months	Basenji Mix	Sable/White
3	A665644	NaN	10/21/2013 07:59:00 AM	10/21/2013 07:59:00 AM	Austin (TX)	Stray	Sick	Cat	Intact Female	4 weeks	Domestic Shorthair Mix	Calico
4	A682524	Rio	06/29/2014 10:38:00 AM	06/29/2014 10:38:00 AM	800 Grove Blvd in Austin (TX)	Stray	Normal	Dog	Neutered Male	4 years	Doberman Pinsch/Australian Cattle Dog	Tan/Gray
```
可以看到 DataFrame 的两个索引都被转换为通用 DataFrame 列，而索引被重置为默认的基于整数的索引<br />相反，如果显式传递 level 的值，则此参数会从 DataFrame 索引中删除选定的级别，并将它们作为常见的 DataFrame 列返回（除非选择使用 drop 参数从 DataFrame 中完全删除此信息）。比较以下操作：
```python
df_multiindex.reset_index(level='Animal ID')
```
Output:
```python
Name	Animal ID	DateTime	MonthYear	Found Location	Intake Type	Intake Condition	Animal Type	Sex upon Intake	Age upon Intake	Breed	Color
*Brock	A786884	01/03/2019 04:19:00 PM	01/03/2019 04:19:00 PM	2501 Magin Meadow Dr in Austin (TX)	Stray	Normal	Dog	Neutered Male	2 years	Beagle Mix	Tricolor
Belle	A706918	07/05/2015 12:59:00 PM	07/05/2015 12:59:00 PM	9409 Bluegrass Dr in Austin (TX)	Stray	Normal	Dog	Spayed Female	8 years	English Springer Spaniel	White/Liver
Runster	A724273	04/14/2016 06:43:00 PM	04/14/2016 06:43:00 PM	2818 Palomino Trail in Austin (TX)	Stray	Normal	Dog	Intact Male	11 months	Basenji Mix	Sable/White
NaN	A665644	10/21/2013 07:59:00 AM	10/21/2013 07:59:00 AM	Austin (TX)	Stray	Sick	Cat	Intact Female	4 weeks	Domestic Shorthair Mix	Calico
Rio	A682524	06/29/2014 10:38:00 AM	06/29/2014 10:38:00 AM	800 Grove Blvd in Austin (TX)	Stray	Normal	Dog	Neutered Male	4 years	Doberman Pinsch/Australian Cattle Dog	Tan/Gray
```
最开始 Animal ID 是 DataFrame 的索引之一，当设置 level 参数后，将其从索引中删除并作为称为 Animal ID 的公共列插入到 DataFrame 中
```python
df_multiindex.reset_index(level='Name')
```
Output:
```python
Animal ID	Name	DateTime	MonthYear	Found Location	Intake Type	Intake Condition	Animal Type	Sex upon Intake	Age upon Intake	Breed	Color
A786884	*Brock	01/03/2019 04:19:00 PM	01/03/2019 04:19:00 PM	2501 Magin Meadow Dr in Austin (TX)	Stray	Normal	Dog	Neutered Male	2 years	Beagle Mix	Tricolor
A706918	Belle	07/05/2015 12:59:00 PM	07/05/2015 12:59:00 PM	9409 Bluegrass Dr in Austin (TX)	Stray	Normal	Dog	Spayed Female	8 years	English Springer Spaniel	White/Liver
A724273	Runster	04/14/2016 06:43:00 PM	04/14/2016 06:43:00 PM	2818 Palomino Trail in Austin (TX)	Stray	Normal	Dog	Intact Male	11 months	Basenji Mix	Sable/White
A665644	NaN	10/21/2013 07:59:00 AM	10/21/2013 07:59:00 AM	Austin (TX)	Stray	Sick	Cat	Intact Female	4 weeks	Domestic Shorthair Mix	Calico
A682524	Rio	06/29/2014 10:38:00 AM	06/29/2014 10:38:00 AM	800 Grove Blvd in Austin (TX)	Stray	Normal	Dog	Neutered Male	4 years	Doberman Pinsch/Australian Cattle Dog	Tan/Gray
```
在这里，Name 最初是 DataFrame 的索引之一，设置完level参数后，就变成了一个常用的列，叫做Name
<a name="f8P8q"></a>
### drop
此参数决定在索引重置后是否将旧索引保留为通用 DataFrame 列，或者将其从 DataFrame 中完全删除。默认情况下 (drop=False) 是进行保留的，正如在前面的所有示例中看到的那样。否则，如果不想将旧索引保留为列，可以在索引重置后将其从 DataFrame 中完全删除（drop=True）：
```python
df
```
Output:
```python
Animal ID	Name	DateTime	MonthYear	Found Location	Intake Type	Intake Condition	Animal Type	Sex upon Intake	Age upon Intake	Breed	Color
A786884	*Brock	01/03/2019 04:19:00 PM	01/03/2019 04:19:00 PM	2501 Magin Meadow Dr in Austin (TX)	Stray	Normal	Dog	Neutered Male	2 years	Beagle Mix	Tricolor
A706918	Belle	07/05/2015 12:59:00 PM	07/05/2015 12:59:00 PM	9409 Bluegrass Dr in Austin (TX)	Stray	Normal	Dog	Spayed Female	8 years	English Springer Spaniel	White/Liver
A724273	Runster	04/14/2016 06:43:00 PM	04/14/2016 06:43:00 PM	2818 Palomino Trail in Austin (TX)	Stray	Normal	Dog	Intact Male	11 months	Basenji Mix	Sable/White
A665644	NaN	10/21/2013 07:59:00 AM	10/21/2013 07:59:00 AM	Austin (TX)	Stray	Sick	Cat	Intact Female	4 weeks	Domestic Shorthair Mix	Calico
A682524	Rio	06/29/2014 10:38:00 AM	06/29/2014 10:38:00 AM	800 Grove Blvd in Austin (TX)	Stray	Normal	Dog	Neutered Male	4 years	Doberman Pinsch/Australian Cattle Dog	Tan/Gray
```
增加 drop 参数
```python
df.reset_index(drop=True)
```
Output:
```python
	Name	DateTime	MonthYear	Found Location	Intake Type	Intake Condition	Animal Type	Sex upon Intake	Age upon Intake	Breed	Color
0	*Brock	01/03/2019 04:19:00 PM	01/03/2019 04:19:00 PM	2501 Magin Meadow Dr in Austin (TX)	Stray	Normal	Dog	Neutered Male	2 years	Beagle Mix	Tricolor
1	Belle	07/05/2015 12:59:00 PM	07/05/2015 12:59:00 PM	9409 Bluegrass Dr in Austin (TX)	Stray	Normal	Dog	Spayed Female	8 years	English Springer Spaniel	White/Liver
2	Runster	04/14/2016 06:43:00 PM	04/14/2016 06:43:00 PM	2818 Palomino Trail in Austin (TX)	Stray	Normal	Dog	Intact Male	11 months	Basenji Mix	Sable/White
3	NaN	10/21/2013 07:59:00 AM	10/21/2013 07:59:00 AM	Austin (TX)	Stray	Sick	Cat	Intact Female	4 weeks	Domestic Shorthair Mix	Calico
4	Rio	06/29/2014 10:38:00 AM	06/29/2014 10:38:00 AM	800 Grove Blvd in Austin (TX)	Stray	Normal	Dog	Neutered Male	4 years	Doberman Pinsch/Australian Cattle Dog	Tan/Gray
```
在上面的 DataFrame 中，旧索引中包含的信息已完全从 DataFrame 中删除了<br />drop 参数也适用于具有 MultiIndex 的 DataFrame，就像之前创建的那样：
```python
df_multiindex
```
Output:
```python
Animal ID	Name	DateTime	MonthYear	Found Location	Intake Type	Intake Condition	Animal Type	Sex upon Intake	Age upon Intake	Breed	Color
A786884	*Brock	01/03/2019 04:19:00 PM	01/03/2019 04:19:00 PM	2501 Magin Meadow Dr in Austin (TX)	Stray	Normal	Dog	Neutered Male	2 years	Beagle Mix	Tricolor
A706918	Belle	07/05/2015 12:59:00 PM	07/05/2015 12:59:00 PM	9409 Bluegrass Dr in Austin (TX)	Stray	Normal	Dog	Spayed Female	8 years	English Springer Spaniel	White/Liver
A724273	Runster	04/14/2016 06:43:00 PM	04/14/2016 06:43:00 PM	2818 Palomino Trail in Austin (TX)	Stray	Normal	Dog	Intact Male	11 months	Basenji Mix	Sable/White
A665644	NaN	10/21/2013 07:59:00 AM	10/21/2013 07:59:00 AM	Austin (TX)	Stray	Sick	Cat	Intact Female	4 weeks	Domestic Shorthair Mix	Calico
A682524	Rio	06/29/2014 10:38:00 AM	06/29/2014 10:38:00 AM	800 Grove Blvd in Austin (TX)	Stray	Normal	Dog	Neutered Male	4 years	Doberman Pinsch/Australian Cattle Dog	Tan/Gray
```
增加 drop 参数
```python
df_multiindex.reset_index(drop=True)
```
Output:
```python
	DateTime	MonthYear	Found Location	Intake Type	Intake Condition	Animal Type	Sex upon Intake	Age upon Intake	Breed	Color
0	01/03/2019 04:19:00 PM	01/03/2019 04:19:00 PM	2501 Magin Meadow Dr in Austin (TX)	Stray	Normal	Dog	Neutered Male	2 years	Beagle Mix	Tricolor
1	07/05/2015 12:59:00 PM	07/05/2015 12:59:00 PM	9409 Bluegrass Dr in Austin (TX)	Stray	Normal	Dog	Spayed Female	8 years	English Springer Spaniel	White/Liver
2	04/14/2016 06:43:00 PM	04/14/2016 06:43:00 PM	2818 Palomino Trail in Austin (TX)	Stray	Normal	Dog	Intact Male	11 months	Basenji Mix	Sable/White
3	10/21/2013 07:59:00 AM	10/21/2013 07:59:00 AM	Austin (TX)	Stray	Sick	Cat	Intact Female	4 weeks	Domestic Shorthair Mix	Calico
4	06/29/2014 10:38:00 AM	06/29/2014 10:38:00 AM	800 Grove Blvd in Austin (TX)	Stray	Normal	Dog	Neutered Male	4 years	Doberman Pinsch/Australian Cattle Dog	Tan/Gray
```
两个旧索引都已从 Dataframe 中完全删除，并且索引已重置为默认值<br />当然，可以结合 drop 和 level 参数，指定要从 DataFrame 中完全删除哪些旧索引：
```python
df_multiindex.reset_index(level='Animal ID', drop=True)
```
Output:
```python
	DateTime	MonthYear	Found Location	Intake Type	Intake Condition	Animal Type	Sex upon Intake	Age upon Intake	Breed	Color
Name										
*Brock	01/03/2019 04:19:00 PM	01/03/2019 04:19:00 PM	2501 Magin Meadow Dr in Austin (TX)	Stray	Normal	Dog	Neutered Male	2 years	Beagle Mix	Tricolor
Belle	07/05/2015 12:59:00 PM	07/05/2015 12:59:00 PM	9409 Bluegrass Dr in Austin (TX)	Stray	Normal	Dog	Spayed Female	8 years	English Springer Spaniel	White/Liver
Runster	04/14/2016 06:43:00 PM	04/14/2016 06:43:00 PM	2818 Palomino Trail in Austin (TX)	Stray	Normal	Dog	Intact Male	11 months	Basenji Mix	Sable/White
NaN	10/21/2013 07:59:00 AM	10/21/2013 07:59:00 AM	Austin (TX)	Stray	Sick	Cat	Intact Female	4 weeks	Domestic Shorthair Mix	Calico
Rio	06/29/2014 10:38:00 AM	06/29/2014 10:38:00 AM	800 Grove Blvd in Austin (TX)	Stray	Normal	Dog	Neutered Male	4 years	Doberman Pinsch/Australian Cattle Dog	Tan/Gray
```
旧索引 Animal ID 已从索引和 DataFrame 本身中删除，另一个索引 Name 被保留为 DataFrame 的当前索引
<a name="bl2I0"></a>
### inplace
该参数决定是直接修改原来的 DataFrame 还是新建一个 DataFrame 对象。默认情况下，它会使用新索引 (`inplace=False`) 创建一个新的 DataFrame，并保持原始 DataFrame 不变。使用默认参数再次运行 `reset_index()` 方法，然后将结果与原始 DataFrame 进行比较：
```python
df.reset_index()
```
Output:
```python
	Animal ID	Name	DateTime	MonthYear	Found Location	Intake Type	Intake Condition	Animal Type	Sex upon Intake	Age upon Intake	Breed	Color
0	A786884	*Brock	01/03/2019 04:19:00 PM	01/03/2019 04:19:00 PM	2501 Magin Meadow Dr in Austin (TX)	Stray	Normal	Dog	Neutered Male	2 years	Beagle Mix	Tricolor
1	A706918	Belle	07/05/2015 12:59:00 PM	07/05/2015 12:59:00 PM	9409 Bluegrass Dr in Austin (TX)	Stray	Normal	Dog	Spayed Female	8 years	English Springer Spaniel	White/Liver
2	A724273	Runster	04/14/2016 06:43:00 PM	04/14/2016 06:43:00 PM	2818 Palomino Trail in Austin (TX)	Stray	Normal	Dog	Intact Male	11 months	Basenji Mix	Sable/White
3	A665644	NaN	10/21/2013 07:59:00 AM	10/21/2013 07:59:00 AM	Austin (TX)	Stray	Sick	Cat	Intact Female	4 weeks	Domestic Shorthair Mix	Calico
4	A682524	Rio	06/29/2014 10:38:00 AM	06/29/2014 10:38:00 AM	800 Grove Blvd in Austin (TX)	Stray	Normal	Dog	Neutered Male	4 years	Doberman Pinsch/Australian Cattle Dog	Tan/Gray
```
```python
df
```
Output:
```python
Animal ID	Name	DateTime	MonthYear	Found Location	Intake Type	Intake Condition	Animal Type	Sex upon Intake	Age upon Intake	Breed	Color
A786884	*Brock	01/03/2019 04:19:00 PM	01/03/2019 04:19:00 PM	2501 Magin Meadow Dr in Austin (TX)	Stray	Normal	Dog	Neutered Male	2 years	Beagle Mix	Tricolor
A706918	Belle	07/05/2015 12:59:00 PM	07/05/2015 12:59:00 PM	9409 Bluegrass Dr in Austin (TX)	Stray	Normal	Dog	Spayed Female	8 years	English Springer Spaniel	White/Liver
A724273	Runster	04/14/2016 06:43:00 PM	04/14/2016 06:43:00 PM	2818 Palomino Trail in Austin (TX)	Stray	Normal	Dog	Intact Male	11 months	Basenji Mix	Sable/White
A665644	NaN	10/21/2013 07:59:00 AM	10/21/2013 07:59:00 AM	Austin (TX)	Stray	Sick	Cat	Intact Female	4 weeks	Domestic Shorthair Mix	Calico
A682524	Rio	06/29/2014 10:38:00 AM	06/29/2014 10:38:00 AM	800 Grove Blvd in Austin (TX)	Stray	Normal	Dog	Neutered Male	4 years	Doberman Pinsch/Australian Cattle Dog	Tan/Gray
```
即使将索引重置为运行第一段代码的默认数字，原始 DataFrame 仍然保持不变。如果需要将原始 DataFrame 重新分配给对其应用 `reset_index()` 方法的结果，可以直接重新分配它（`df = df.reset_index()`）或将参数 `inplace=True` 传递给该方法：
```python
df.reset_index(inplace=True)
df
```
Output:
```python
	Animal ID	Name	DateTime	MonthYear	Found Location	Intake Type	Intake Condition	Animal Type	Sex upon Intake	Age upon Intake	Breed	Color
0	A786884	*Brock	01/03/2019 04:19:00 PM	01/03/2019 04:19:00 PM	2501 Magin Meadow Dr in Austin (TX)	Stray	Normal	Dog	Neutered Male	2 years	Beagle Mix	Tricolor
1	A706918	Belle	07/05/2015 12:59:00 PM	07/05/2015 12:59:00 PM	9409 Bluegrass Dr in Austin (TX)	Stray	Normal	Dog	Spayed Female	8 years	English Springer Spaniel	White/Liver
2	A724273	Runster	04/14/2016 06:43:00 PM	04/14/2016 06:43:00 PM	2818 Palomino Trail in Austin (TX)	Stray	Normal	Dog	Intact Male	11 months	Basenji Mix	Sable/White
3	A665644	NaN	10/21/2013 07:59:00 AM	10/21/2013 07:59:00 AM	Austin (TX)	Stray	Sick	Cat	Intact Female	4 weeks	Domestic Shorthair Mix	Calico
4	A682524	Rio	06/29/2014 10:38:00 AM	06/29/2014 10:38:00 AM	800 Grove Blvd in Austin (TX)	Stray	Normal	Dog	Neutered Male	4 years	Doberman Pinsch/Australian Cattle Dog	Tan/Gray
```
看一看到现在更改已直接应用于原始 DataFrame 之上了
<a name="YhBHE"></a>
## 应用实例：删除缺失值后重置索引
将到目前为止讨论的所有内容付诸实践，看看当从 DataFrame 中删除缺失值时，重置 DataFrame 索引是如何有用的<br />首先，恢复最开始时创建的第一个 DataFrame，它具有默认数字索引：
```python
df = pd.read_csv('Austin_Animal_Center_Intakes.csv').head()
df
```
Output:
```python
	Animal ID	Name	DateTime	MonthYear	Found Location	Intake Type	Intake Condition	Animal Type	Sex upon Intake	Age upon Intake	Breed	Color
0	A786884	*Brock	01/03/2019 04:19:00 PM	01/03/2019 04:19:00 PM	2501 Magin Meadow Dr in Austin (TX)	Stray	Normal	Dog	Neutered Male	2 years	Beagle Mix	Tricolor
1	A706918	Belle	07/05/2015 12:59:00 PM	07/05/2015 12:59:00 PM	9409 Bluegrass Dr in Austin (TX)	Stray	Normal	Dog	Spayed Female	8 years	English Springer Spaniel	White/Liver
2	A724273	Runster	04/14/2016 06:43:00 PM	04/14/2016 06:43:00 PM	2818 Palomino Trail in Austin (TX)	Stray	Normal	Dog	Intact Male	11 months	Basenji Mix	Sable/White
3	A665644	NaN	10/21/2013 07:59:00 AM	10/21/2013 07:59:00 AM	Austin (TX)	Stray	Sick	Cat	Intact Female	4 weeks	Domestic Shorthair Mix	Calico
4	A682524	Rio	06/29/2014 10:38:00 AM	06/29/2014 10:38:00 AM	800 Grove Blvd in Austin (TX)	Stray	Normal	Dog	Neutered Male	4 years	Doberman Pinsch/Australian Cattle Dog	Tan/Gray
```
可以看到 DataFrame 中有一个缺失值，使用 `dropna()` 方法删除具有缺失值的整行
```python
df.dropna(inplace=True)
df
```
Output:
```python
	Animal ID	Name	DateTime	MonthYear	Found Location	Intake Type	Intake Condition	Animal Type	Sex upon Intake	Age upon Intake	Breed	Color
0	A786884	*Brock	01/03/2019 04:19:00 PM	01/03/2019 04:19:00 PM	2501 Magin Meadow Dr in Austin (TX)	Stray	Normal	Dog	Neutered Male	2 years	Beagle Mix	Tricolor
1	A706918	Belle	07/05/2015 12:59:00 PM	07/05/2015 12:59:00 PM	9409 Bluegrass Dr in Austin (TX)	Stray	Normal	Dog	Spayed Female	8 years	English Springer Spaniel	White/Liver
2	A724273	Runster	04/14/2016 06:43:00 PM	04/14/2016 06:43:00 PM	2818 Palomino Trail in Austin (TX)	Stray	Normal	Dog	Intact Male	11 months	Basenji Mix	Sable/White
4	A682524	Rio	06/29/2014 10:38:00 AM	06/29/2014 10:38:00 AM	800 Grove Blvd in Austin (TX)	Stray	Normal	Dog	Neutered Male	4 years	Doberman Pinsch/Australian Cattle Dog	Tan/Gray
```
该行已从 DataFrame 中删除，但是索引不再是连续的：0、1、2、4。重新设置它：
```python
df.reset_index()
```
Output:
```python
	index	Animal ID	Name	DateTime	MonthYear	Found Location	Intake Type	Intake Condition	Animal Type	Sex upon Intake	Age upon Intake	Breed	Color
0	0	A786884	*Brock	01/03/2019 04:19:00 PM	01/03/2019 04:19:00 PM	2501 Magin Meadow Dr in Austin (TX)	Stray	Normal	Dog	Neutered Male	2 years	Beagle Mix	Tricolor
1	1	A706918	Belle	07/05/2015 12:59:00 PM	07/05/2015 12:59:00 PM	9409 Bluegrass Dr in Austin (TX)	Stray	Normal	Dog	Spayed Female	8 years	English Springer Spaniel	White/Liver
2	2	A724273	Runster	04/14/2016 06:43:00 PM	04/14/2016 06:43:00 PM	2818 Palomino Trail in Austin (TX)	Stray	Normal	Dog	Intact Male	11 months	Basenji Mix	Sable/White
3	4	A682524	Rio	06/29/2014 10:38:00 AM	06/29/2014 10:38:00 AM	800 Grove Blvd in Austin (TX)	Stray	Normal	Dog	Neutered Male	4 years	Doberman Pinsch/Australian Cattle Dog	Tan/Gray
```
现在索引是连续的了，但是由于没有显式传递 drop 参数，旧索引被转换为列，具有默认名称 index，下面从 DataFrame 中完全删除旧索引：
```python
df.reset_index(drop=True)
```
Output:
```python
	Animal ID	Name	DateTime	MonthYear	Found Location	Intake Type	Intake Condition	Animal Type	Sex upon Intake	Age upon Intake	Breed	Color
0	A786884	*Brock	01/03/2019 04:19:00 PM	01/03/2019 04:19:00 PM	2501 Magin Meadow Dr in Austin (TX)	Stray	Normal	Dog	Neutered Male	2 years	Beagle Mix	Tricolor
1	A706918	Belle	07/05/2015 12:59:00 PM	07/05/2015 12:59:00 PM	9409 Bluegrass Dr in Austin (TX)	Stray	Normal	Dog	Spayed Female	8 years	English Springer Spaniel	White/Liver
2	A724273	Runster	04/14/2016 06:43:00 PM	04/14/2016 06:43:00 PM	2818 Palomino Trail in Austin (TX)	Stray	Normal	Dog	Intact Male	11 months	Basenji Mix	Sable/White
3	A682524	Rio	06/29/2014 10:38:00 AM	06/29/2014 10:38:00 AM	800 Grove Blvd in Austin (TX)	Stray	Normal	Dog	Neutered Male	4 years	Doberman Pinsch/Australian Cattle Dog	Tan/Gray
```
现在彻底摆脱了无意义的旧索引，当前索引是连续的。最后一步是使用 inplace 参数将这些修改保存到原始 DataFrame 中：
```python
df.reset_index(drop=True, inplace=True)
df
```
Output:
```python
Animal ID	Name	DateTime	MonthYear	Found Location	Intake Type	Intake Condition	Animal Type	Sex upon Intake	Age upon Intake	Breed	Color
0	A786884	*Brock	01/03/2019 04:19:00 PM	01/03/2019 04:19:00 PM	2501 Magin Meadow Dr in Austin (TX)	Stray	Normal	Dog	Neutered Male	2 years	Beagle Mix	Tricolor
1	A706918	Belle	07/05/2015 12:59:00 PM	07/05/2015 12:59:00 PM	9409 Bluegrass Dr in Austin (TX)	Stray	Normal	Dog	Spayed Female	8 years	English Springer Spaniel	White/Liver
2	A724273	Runster	04/14/2016 06:43:00 PM	04/14/2016 06:43:00 PM	2818 Palomino Trail in Austin (TX)	Stray	Normal	Dog	Intact Male	11 months	Basenji Mix	Sable/White
3	A682524	Rio	06/29/2014 10:38:00 AM	06/29/2014 10:38:00 AM	800 Grove Blvd in Austin (TX)	Stray	Normal	Dog	Neutered Male	4 years	Doberman Pinsch/Australian Cattle Dog	Tan/Gray
```
<a name="PxDDQ"></a>
## 总结
从多个方面讨论了 `reset_index()` 方法

- `reset_index()` 方法的默认行为
- 如何恢复 DataFrame 的默认数字索引
- 何时使用 `reset_index()` 方法
- 该方法最重要的几个参数
- 如何使用 MultiIndex
- 如何从 DataFrame 中完全删除旧索引
- 如何将修改直接保存到原始 DataFrame 中
