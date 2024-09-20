<a name="25baaed7"></a>
## 透视表(pivot_table)

[透视表](https://baike.baidu.com/item/%E6%95%B0%E6%8D%AE%E9%80%8F%E8%A7%86%E8%A1%A8/4013030?fr=aladdin)是一种可以对数据**动态排布**并且**分类汇总**的表格格式。或许大多数人都在Excel使用过数据透视表（如下图），也体会到它的强大功能，而在pandas中它被称作pivot_table。

![](https://pic4.zhimg.com/80/v2-8bb58f4d9bf08efafb0053c66ce4c53f_hd.jpg#align=left&display=inline&height=307&originHeight=307&originWidth=436&status=done&style=none&width=436)

```
import pandas as pd
import numpy as np
df = pd.read_csv('h:/James_Harden.csv',encoding='utf8')
df.tail()
```

![最后5场比赛数据.jpg](./img/1599210130470-c307abbc-f3f8-40fa-9e68-4d7331e78738.jpeg)

pivot_table(data, values=None, index=None, columns=None,**aggfunc='mean'**, fill_value=None, margins=False, dropna=True, margins_name='All')

[pivot_table](https://pandas.pydata.org/pandas-docs/stable/reference/api/pandas.pivot_table.html)有四个最重要的参数**index、values、columns、aggfunc**，本文以这四个参数为中心讲解pivot操作是如何进行。

<a name="Index"></a>
### Index

每个pivot_table必须拥有一个**index，**如果想查看哈登对阵每个队伍的得分，首先我们将**对手**设置为**index：**

```
pd.pivot_table(df,index=[u'对手'])
```

![index 对手.jpg](./img/1599210146225-ad84cbdd-de74-4ad0-a797-8a3cc5e74383.jpeg)

**对手**成为了第一层索引，还想看看对阵同一对手在不同主客场下的数据，试着将**对手**与**胜负**与**主客场**都设置为**index**

```
pd.pivot_table(df,index=[u'对手',u'主客场'])
```

![index 对手 主客场.jpg](./img/1599210162509-755f8e01-9855-4088-a178-1b14d6f47f83.jpeg)

试着交换下它们的顺序，数据结果一样：

```
pd.pivot_table(df,index=[u'主客场',u'对手'])
```

![index 调换顺序.jpg](./img/1599210177754-ff216064-d874-464c-aa6c-1dfd0b14acc3.jpeg)

看完上面几个操作，**Index就是层次字段，要通过透视表获取什么信息就按照相应的顺序设置字段**，所以在进行pivot之前你也需要足够了解你的数据。

<a name="8faefb83"></a>
### **Values**

通过上面的操作，我们获取了james harden在对阵对手时的**所有数据**，而Values可以对需要的计算数据进行筛选，如果我们只需要james harden在主客场和不同胜负情况下的得分、篮板与助攻三项数据：

```
pd.pivot_table(df,index=[u'主客场',u'胜负'],values=[u'得分',u'助攻',u'篮板'])
```

![values.jpg](./img/1599210193902-f7d40a05-84a4-4f23-bc5c-dbb8089ebcd3.jpeg)

<a name="e99fd96e"></a>
### **Aggfunc**

aggfunc参数可以设置我们对数据聚合时进行的函数操作。

当我们未设置aggfunc时，它默认**aggfunc='mean'**计算均值。我们还想要获得james harden在主客场和不同胜负情况下的**总**得分、**总**篮板、**总**助攻时：

```
pd.pivot_table(df,index=[u'主客场',u'胜负'],values=[u'得分',u'助攻',u'篮板'],aggfunc=[np.sum,np.mean])
```

![aggfunc.jpg](./img/1599210441744-a9e3ee72-f8c4-416f-8606-c9c217a5bd1e.jpeg)

<a name="f753d189"></a>
### **Columns**

Columns类似Index可以设置**列层次字段**，它不是一个必要参数，作为一种分割数据的可选方式

```
#fill_value填充空值,margins=True进行汇总
pd.pivot_table(df,index=[u'主客场'],columns=[u'对手'],values=[u'得分'],aggfunc=[np.sum],fill_value=0,margins=1)
```

![columns.jpg](./img/1599210456096-c6addcec-90f4-46f9-94bc-d5683358375a.jpeg)

```
table=pd.pivot_table(df,index=[u'对手',u'胜负'],columns=[u'主客场'],values=[u'得分',u'助攻',u'篮板'],aggfunc=[np.mean],fill_value=0)
```

![助攻得分篮板均值.jpg](./img/1599210471936-201687f1-881e-4f43-a429-ea4cd027d003.jpeg)

<a name="65e5df1c"></a>
## pivot_table vs. groupby

```
pd.pivot_table(df,index=[字段1],values=[字段2],aggfunc=[函数],fill_value=0)
df.groupby([字段1])[字段2].agg(函数).fillna(0)
```

上面两个函数完全等价，pivot_table仿佛是加入了columns与margin功能的groupby函数，比groupby更加灵活。

<a name="query"></a>
### query

当表格生成后如何查询某一项数据呢？举例：根据上表查询哈登对阵灰熊时的数据

```
table.query('对手 == ["灰熊"]')
```

![灰熊.jpg](./img/1599210496135-09770c14-0c0b-4c85-ac81-1275ae56101b.jpeg)

<a name="3f009121"></a>
## Cheat Sheet

![cheat sheet.jpg](./img/1599210509954-018c1772-f556-488b-89d3-bdcffde84898.jpeg)

<a name="Source"></a>
## Source

[https://zhuanlan.zhihu.com/p/31952948?from_voters_page=true](https://zhuanlan.zhihu.com/p/31952948?from_voters_page=true)<br />[https://pandas.pydata.org/pandas-docs/stable/reference/api/pandas.pivot_table.html](https://pandas.pydata.org/pandas-docs/stable/reference/api/pandas.pivot_table.html)
